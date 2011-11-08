#include <fstream>
#include "SI_settings.h"
#include "ExPool.h"


using namespace std;
using namespace SI;

ExPool::ExPool (string poolName_in,
				int totalNeurons_in,
				bool recordSpikes_in) 
{
	// Set the name:
	poolName = poolName_in;

	// Setting primitives:
	tau_AMPA_Inv_times_dt = 1/tau_AMPA*dt;
	tau_AMPA_rise_Inv_times_dt = 1/tau_AMPA_rise*dt;
	one_minus_tau_NMDA_Inv_times_dt = 1-dt/tau_NMDA;
	alpha_times_dt = alpha*dt;
	dt_times_gL_E_over_cm_E = dt*gL_E/cm_E;
	recordSpikes = recordSpikes_in;
	totalNeurons = totalNeurons_in;
	
	// Create helper vectors:
	unitVector = new valarray<float>((float)1, totalNeurons);
	VTmp = new valarray<float>((float)0, totalNeurons);
	thresholdTest = new valarray<bool>(false, totalNeurons);
	inds = new valarray<int>(totalNeurons);
	for (i=0; i<(*inds).size(); ++i)
	{
		(*inds)[i] = i+1;
	}
	spiked = new valarray<int>;
	
	// Spike recording:
	if (recordSpikes)
	{
		spikeRecord_t = new vector<float>;
		spikeRecord_n = new vector<int>;
	}
	
	// Initialize connection vectors, and weight vector:
	BG_Inputs_AMPA = new vector< valarray<float>* >;
	Ex_Inputs_AMPA = new vector<float*>;
	Ex_Inputs_AMPA_w = new vector<float>;
	Ex_Inputs_NMDA = new vector<float*>;
	Ex_Inputs_NMDA_w = new vector<float>;
	Inh_Inputs_GABA = new vector<float*>;
	
	// Initialize state vectors:
	AMPA = new valarray<float>((float)0, totalNeurons);
	NMDA = new valarray<float>((float)0, totalNeurons);
	ISyn = new valarray<float>((float)0, totalNeurons);
	V = new valarray<float>((float)-50, totalNeurons);
	X = new valarray<float>((float)0, totalNeurons);
};

ExPool::~ExPool () 
{
	if (recordSpikes)
	{
		delete spikeRecord_t;
		delete spikeRecord_n;
	}
	delete unitVector;
	delete VTmp;
	delete thresholdTest;
	delete inds;
	delete spiked;
	
	delete BG_Inputs_AMPA;
	delete Ex_Inputs_AMPA;
	delete Ex_Inputs_AMPA_w;
	delete Ex_Inputs_NMDA;
	delete Ex_Inputs_NMDA_w;
	delete Inh_Inputs_GABA;
	
	delete AMPA;
	delete NMDA;
	delete ISyn;
	delete V;
	delete X;
};

void ExPool::init()
{
	// TODO: randomize initial voltages, and other state vars
};

void ExPool::propogate() 
{
//	 Compute current coming into the cell:
	(*ISyn) = valarray<float>((float)0, totalNeurons);
	(*VTmp) = ((*V) - VE * (*unitVector));
//
	
//	// First, the background pools:
	for (i = 0; i < (*BG_Inputs_AMPA).size(); i++)
	{
		(*ISyn) += gext_AMPA_E * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
	}
	
//
//	// Then, recurrent AMPA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_AMPA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_AMPA)[i])) * ((*Ex_Inputs_AMPA_w)[i]);
		cout << "recAMPA"<<(*Ex_Inputs_AMPA).size() << endl;
	}
	(*ISyn) += grec_AMPA_E * STmp * (*VTmp);
	

//
//	// Next, recurrent NMDA:
	(*VTmp) /= (*unitVector) + exp(K*(*V))/3.57;
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_NMDA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_NMDA)[i])) * ((*Ex_Inputs_NMDA_w)[i]);
	}
	(*ISyn) += gNMDA_E * STmp * (*VTmp);
	

//
//	// Finally, recurrent GABA:
	(*VTmp) = ((*V) - VI * (*unitVector));
	STmp = 0;
	for (i = 0; i < (*Inh_Inputs_GABA).size(); i++)
	{
		STmp += (*((*Inh_Inputs_GABA)[i]));
	}
	(*ISyn) += gGABA_E * STmp * (*VTmp);
	

//	
//	// Update voltage:			
	(*VTmp) = ((*V) - VMin * (*unitVector));
	(*V) -= float(.001)*dt_times_gL_E_over_cm_E * (*VTmp) + (dt/cm_E)*(*ISyn);

//
//	// Determine who spiked, and update state vars:
	(*thresholdTest) = (*V) > VMax;
	(*V)[(*thresholdTest)] = VReset;
	(*AMPA)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
	(*NMDA)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
	if (recordSpikes)
	{
		for (i = 0; i <= totalNeurons; i++) 
		{
			if ((*thresholdTest)[i]==true)
			{
				(*spikeRecord_n).push_back(i);
				(*spikeRecord_t).push_back(t);
			}
		}
	}
//	
//	// Update state vars:
	(*AMPA) -= tau_AMPA_Inv_times_dt*(*AMPA);
	(*X) -= tau_AMPA_rise_Inv_times_dt*(*X);
	(*NMDA) = one_minus_tau_NMDA_Inv_times_dt*(*NMDA) + alpha_times_dt*(*X)*(*unitVector - (*NMDA));
	
	// Update state vars sums:
	AMPA_pooled = (*AMPA).sum();
	NMDA_pooled = (*NMDA).sum();
}

void ExPool::connectTo(BGPool *BGPool_in)
{
	(*BG_Inputs_AMPA).push_back((*BGPool_in).AMPA_pooled);
}

void ExPool::connectTo(ExPool *ExPool_in, float wIn)
{
	(*Ex_Inputs_AMPA).push_back(&(*ExPool_in).AMPA_pooled);
	(*Ex_Inputs_AMPA_w).push_back(wIn);
	(*Ex_Inputs_NMDA).push_back(&(*ExPool_in).NMDA_pooled);
	(*Ex_Inputs_NMDA_w).push_back(wIn);
}

//void ExPool::connectTo(InhPool *InhPool_in) TODO:
//{
//	cout << "InhConnect" <<endl;
//}

float ExPool::getFR()
{
	return float((*spikeRecord_n).size())/(totalNeurons);
}

void ExPool::writeSpikes(string UUID_string)
{
	// Set up file:
	string myUnderscore = "_";
	string fileTypeExtension = ".ntf";
	string fileName = poolName + myUnderscore + UUID_string + fileTypeExtension;
	ofstream myfile;
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	myfile.open(charFileName);
	
	// Write the spikes in "name-time format":
	for (i = 1; i <= (*spikeRecord_t).size()-1; i++)
	{
		myfile << poolName << "_" << (*spikeRecord_n)[i] << "\t" << (*spikeRecord_t)[i] << endl;
	}
	
	myfile.close();
	return;
};

