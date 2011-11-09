#include <fstream>
#include "SI_settings.h"
#include "ExPool.h"
#include "InhPool.h"
#include "BgPool.h"

using namespace boost::random;
using namespace std;
using namespace SI;

InhPool::InhPool(string poolName_in,
				int totalNeurons_in,
				bool recordSpikes_in) 
{
	// Set the name:
	poolName = poolName_in;
	
	// Random number generators:
	uniDist = new uniform_real_distribution<float>(0,1);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<float> >(myRNG, *uniDist);

	// Setting primitives:
	dt_over_cm_I = dt/cm_I;
	tau_GABA_Inv_times_dt = 1/tau_GABA*dt;
	dt_times_gL_I_over_cm_I = dt*gL_I/cm_I;
	recordSpikes = recordSpikes_in;
	totalNeurons = totalNeurons_in;
	
	// Create helper vectors:
	unitVector = new valarray<float>((float)1, totalNeurons);
	VTmp = new valarray<float>((float)0, totalNeurons);
	thresholdTest = new valarray<bool>(false, totalNeurons);
	
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
	GABA = new valarray<float>((float)0, totalNeurons);
	ISyn = new valarray<float>((float)0, totalNeurons);
	V = new valarray<float>((float)0, totalNeurons);
};

InhPool::~InhPool () 
{
	delete uniRnd;
	delete uniDist;
	
	if (recordSpikes)
	{
		delete spikeRecord_t;
		delete spikeRecord_n;
	}
	delete unitVector;
	delete VTmp;
	delete thresholdTest;
	
	delete BG_Inputs_AMPA;
	delete Ex_Inputs_AMPA;
	delete Ex_Inputs_AMPA_w;
	delete Ex_Inputs_NMDA;
	delete Ex_Inputs_NMDA_w;
	delete Inh_Inputs_GABA;
	
	delete GABA;
	delete ISyn;
	delete V;
};

void InhPool::init()
{
	for (i=0; i<totalNeurons; i++) {(*V)[i] = (*uniRnd)()*5-55;};
	
	GABA_pooled = 0;
	
};

void InhPool::propogate() 
{
	// Compute current coming into the cell:
	(*ISyn) = valarray<float>((float)0, totalNeurons);
	(*VTmp) = ((*V) - VE * (*unitVector));

	
	// First, the background pools:
	for (i = 0; i < (*BG_Inputs_AMPA).size(); i++)
	{
		(*ISyn) += gext_AMPA_I * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
	}
	

	// Then, recurrent AMPA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_AMPA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_AMPA)[i])) * ((*Ex_Inputs_AMPA_w)[i]);
	}
	(*ISyn) += grec_AMPA_I * STmp * (*VTmp);
	


	// Next, recurrent NMDA:
	(*VTmp) /= (*unitVector) + exp(K*(*V))/3.57;
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_NMDA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_NMDA)[i])) * ((*Ex_Inputs_NMDA_w)[i]);
	}
	(*ISyn) += gNMDA_I * STmp * (*VTmp);
	

	// Finally, recurrent GABA:
	(*VTmp) = ((*V) - VI * (*unitVector));
	STmp = 0;
	for (i = 0; i < (*Inh_Inputs_GABA).size(); i++)
	{
		STmp += (*((*Inh_Inputs_GABA)[i]));
	}
	(*ISyn) += gGABA_I * STmp * (*VTmp);
	

	// Update voltage:			
	(*VTmp) = ((*V) - VMin * (*unitVector));
	(*V) -= dt_times_gL_I_over_cm_I * (*VTmp) + dt_over_cm_I*(*ISyn);

	// Determine who spiked, and update state vars:
	(*thresholdTest) = (*V) > VMax;
	(*V)[(*thresholdTest)] = VReset;
	(*GABA)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
	if (recordSpikes)
	{
		for (i = 0; i < totalNeurons; i++) 
		{
			if ((*thresholdTest)[i]==true)
			{
				(*spikeRecord_n).push_back(i);
				(*spikeRecord_t).push_back(t);
			}
		}
	}
	
	// Update state vars:
	(*GABA) -= tau_GABA_Inv_times_dt*(*GABA);
	
	// Update state vars sums:
	GABA_pooled = (*GABA).sum();
}

void InhPool::connectTo(BGPool *BGPool_in)
{
	(*BG_Inputs_AMPA).push_back((*BGPool_in).AMPA_pooled);
}

void InhPool::connectTo(ExPool *ExPool_in, float wIn)
{
	(*Ex_Inputs_AMPA).push_back(&(* ExPool_in).AMPA_pooled);
	(*Ex_Inputs_AMPA_w).push_back(wIn);
	(*Ex_Inputs_NMDA).push_back(&(* ExPool_in).NMDA_pooled);
	(*Ex_Inputs_NMDA_w).push_back(wIn);
}

void InhPool::connectTo(InhPool *InhPool_in)
{
	(*Inh_Inputs_GABA).push_back(&(* InhPool_in).GABA_pooled);
}

float InhPool::getFR()
{
	return float((*spikeRecord_n).size())/(totalNeurons)/t*1000;
}

void InhPool::writeSpikes(string UUID_string)
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

