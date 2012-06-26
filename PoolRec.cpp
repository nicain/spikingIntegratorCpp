#include <string>
#include "PoolBG.h"
#include "PoolRec.h"
#include "PoolRecEx.h"
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;

using namespace std;

const double PoolRec::VE;
const double PoolRec::VI;
const double PoolRec::VMin;
const double PoolRec::VMax;
const double PoolRec::VReset;
const double PoolRec::alpha;
const double PoolRec::K;

PoolRec::PoolRec(string poolName_in, Brain &parentPool_in, int N_in): Pool(poolName_in, parentPool_in, N_in)
{
	construct();
};

PoolRec::PoolRec(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in): Pool(poolName_in, parentPool_in, N_in, recordSpikes_in)
{
	construct();
};

void PoolRec::construct()
{
	uniDist = new uniform_real_distribution<double>(0,1);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);
	
	// Set member data:
	V = new valarray<double>((double)0, N);
	ISyn = new valarray<double>((double)0, N);
    ISynBG = new valarray<double>((double)0, N);
    ISynInput = new valarray<double>((double)0, N);
    
    ISynRecAMPA = new valarray<double>((double)0, N);
    ISynRecNMDA = new valarray<double>((double)0, N);
    ISynRecGABA = new valarray<double>((double)0, N);
    
    SInput = new valarray<double>((double)0, N);
    SBG = new valarray<double>((double)0, N);
	
	// Set up for RK:
	RK1 = new valarray<double>((double)0, N);
	RK2 = new valarray<double>((double)0, N);
	RK3 = new valarray<double>((double)0, N);
	RK4 = new valarray<double>((double)0, N);
	
	// Initialize connection vectors, and weight vector:
	BG_Inputs_AMPA = new vector< valarray<double>* >;
	Ex_Inputs_AMPA = new vector<double*>;
	Ex_Inputs_AMPA_w = new vector<double>;
	Ex_Inputs_NMDA = new vector<double*>;
	Ex_Inputs_NMDA_w = new vector<double>;
	Inh_Inputs_GABA = new vector<double*>;
	
	// Create helper vectors:
	unitVector = new valarray<double>((double)1, N);
	VTmp = new valarray<double>((double)0, N);
	thresholdTest = new valarray<bool>(false, N);
    
    ISynPoolSum = 0;
    ISynInputPoolSum = 0;
    ISynBGPoolSum = 0;
    ISynRecAMPASum = 0;
    ISynRecNMDASum = 0;
    ISynRecGABASum = 0;
	
};

PoolRec::~PoolRec() 
{
	delete V;
	delete ISyn;
    delete ISynBG;
    delete ISynRecAMPA;
    delete ISynRecNMDA;
    delete ISynRecGABA;
	
	delete RK1;
	delete RK2;
	delete RK3;
	delete RK4;
	
	delete uniRnd;
	delete uniDist;
	
	delete BG_Inputs_AMPA;
	delete Ex_Inputs_AMPA;
	delete Ex_Inputs_AMPA_w;
	delete Ex_Inputs_NMDA;
	delete Ex_Inputs_NMDA_w;
	delete Inh_Inputs_GABA;
	
	delete unitVector;
	delete VTmp;
	delete thresholdTest;
    
    delete SInput;
    delete SBG;
	
};

void PoolRec::updateV()
{
	//	 Compute current coming into the cell:.000005436
	(*ISyn) = valarray<double>((double)0, N);
	(*ISynBG) = valarray<double>((double)0, N);
	(*ISynInput) = valarray<double>((double)0, N);
	(*ISynRecAMPA) = valarray<double>((double)0, N);
	(*ISynRecNMDA) = valarray<double>((double)0, N);
	(*ISynRecGABA) = valarray<double>((double)0, N);
	(*VTmp) = ((*V) - VE * (*unitVector));
    
    (*SInput) = valarray<double>((double)0, N);
	(*SBG) = valarray<double>((double)0, N);
	cout << "new" << endl;
	// First, the background pools:
	for (i = 0; i < (*BG_Inputs_AMPA).size(); i++)
	{
        if ((*(this->parentBrain)).isBGInput((*BG_Inputs_AMPA)[i])) {
            (*ISynInput) += gext_AMPA * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
            (*SInput) += (*((*BG_Inputs_AMPA)[i]));
        } else 
        {
            (*ISynBG) += gext_AMPA * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
            (*SBG) += (*((*BG_Inputs_AMPA)[i]));
        }
        (*ISyn) += gext_AMPA * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
	}
    ISynInputPoolSum = (*ISynInput).sum();///(*ISynInput).size();
    ISynBGPoolSum = (*ISynBG).sum();///(*ISynBG).size();
    SBGSum = (*SBG).sum();///(*ISynBG).size();
    SInputSum = (*SInput).sum();///(*ISynBG).size();
//    cout <<  SInputSum << " " << &SInputSum << endl;
//    cout <<  SBGSum << " " << &SBGSum << endl;
	
	// Then, recurrent AMPA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_AMPA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_AMPA)[i])) * ((*Ex_Inputs_AMPA_w)[i]);
		
	}
	(*ISyn) += grec_AMPA * STmp * (*VTmp);
    (*ISynRecAMPA) += grec_AMPA * STmp * (*VTmp);
    ISynRecAMPASum = (*ISynRecAMPA).sum();///(*ISynRecAMPA).size();
	
	// Next, recurrent NMDA:
	(*VTmp) /= (*unitVector) + exp(K*(*V))/3.57;
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_NMDA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_NMDA)[i])) * ((*Ex_Inputs_NMDA_w)[i]);
	}
	(*ISyn) += gNMDA * STmp * (*VTmp);
    (*ISynRecNMDA) += gNMDA * STmp * (*VTmp);
    ISynRecNMDASum = (*ISynRecNMDA).sum();///(*ISynRecNMDA).size();
	
	// Finally, recurrent GABA:
	(*VTmp) = ((*V) - VI * (*unitVector));
	STmp = 0;
	for (i = 0; i < (*Inh_Inputs_GABA).size(); i++)
	{
		STmp += (*((*Inh_Inputs_GABA)[i]));
	}
	(*ISyn) += gGABA * STmp * (*VTmp);
    (*ISynRecGABA) += gGABA * STmp * (*VTmp);
    ISynPoolSum = (*ISyn).sum();///(*ISyn).size();
    ISynRecGABASum = (*ISynRecGABA).sum();///(*ISynRecGABA).size();
	
	// Update voltage:			
	(*VTmp) = ((*V) - VMin * (*unitVector));	//TODO: optimize below:
	
	(*RK1) = -dt_times_gL_over_cm*(*VTmp) - dt_over_cm*(*ISyn);
	(*RK2) = -dt_times_gL_over_cm*(*VTmp + .5*(*RK1)) - dt_over_cm*(*ISyn);
	(*RK3) = -dt_times_gL_over_cm*(*VTmp + .5*(*RK2)) - dt_over_cm*(*ISyn);
	(*RK4) = -dt_times_gL_over_cm*(*VTmp + (*RK3)) - dt_over_cm*(*ISyn);
	
	(*V) += (double(1)/double(6))*(   (*RK1) + double(2)*(*RK2) + double(2)*(*RK3) + (*RK4)    );
//	(*V) -= dt_times_gL_over_cm * (*VTmp) + dt_over_cm*(*ISyn);
}

void PoolRec::init()
{
	for (i=0; i<N; i++) 
	{
		(*V)[i] = (*uniRnd)()*5-55;
	};	
}

void PoolRec::connectTo(PoolBG &BGPool_in)
{
	BG_Inputs_AMPA->push_back(BGPool_in.AMPA);
}

void PoolRec::connectTo(PoolRecEx &ExPool_in, double wIn)
{
	Ex_Inputs_AMPA->push_back(&(ExPool_in).AMPA_pooled);
	Ex_Inputs_AMPA_w->push_back(wIn);
	Ex_Inputs_NMDA->push_back(&(ExPool_in).NMDA_pooled);
	Ex_Inputs_NMDA_w->push_back(wIn);
}

void PoolRec::connectTo(PoolRecInh &InhPool_in)
{
	Inh_Inputs_GABA->push_back(&(InhPool_in).GABA_pooled);
}

double* PoolRec::getStateLocation(int whichNeuron, State whichState) 
{
	
	double *returnAddress;
	
	switch (whichState)
	{
		case S_V:
			returnAddress = &((*V)[whichNeuron]);
			break;
		case S_ISyn:
			returnAddress = &((*ISyn)[whichNeuron]);
			break;
		case S_ISynBG:
			returnAddress = &((*ISynBG)[whichNeuron]);
			break;
		case S_ISynRecAMPA:
			returnAddress = &((*ISynRecAMPA)[whichNeuron]);
			break;
		case S_ISynRecNMDA:
			returnAddress = &((*ISynRecNMDA)[whichNeuron]);
			break;
		case S_ISynRecGABA:
			returnAddress = &((*ISynRecGABA)[whichNeuron]);
			break;          
		case S_ISynInput:
			returnAddress = &((*ISynInput)[whichNeuron]);
			break;            
            
		default:
			returnAddress = getStateLocationConductance(whichNeuron, whichState);
	}
	
	return returnAddress;
};

double* PoolRec::getStateLocation(State whichState) 
{
	
	double *returnAddress;
	
	switch (whichState)
	{
		case S_ISynPoolSum:
			returnAddress = &ISynPoolSum;
			break;         
		case S_ISynInputPoolSum:
			returnAddress = &ISynInputPoolSum;
			break;   
		case S_ISynBGPoolSum:
			returnAddress = &ISynBGPoolSum;
			break;
		case S_ISynRecAMPASum:
			returnAddress = &ISynRecAMPASum;
			break;
		case S_ISynRecNMDASum:
			returnAddress = &ISynRecNMDASum;
			break;
		case S_ISynRecGABASum:
			returnAddress = &ISynRecGABASum;
            break;
		case S_SBGSum:
			returnAddress = &SBGSum;
            break;
		case S_SInputSum:
			returnAddress = &SInputSum;
			break;
            
            
		default:
			returnAddress = 0;
	}
	

    
	return returnAddress;
};

double* PoolRec::getStateLocationConductance(int whichNeuron, State whichState) 
{
	return 0;
};
