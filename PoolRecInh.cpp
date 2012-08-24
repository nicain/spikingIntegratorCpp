#include <string>
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRec;

using namespace std;

const double PoolRecInh::tau_GABA;
double PoolRecInh::tau_GABA_Inv_times_dt = Brain::dt*(1.0/PoolRecInh::tau_GABA);
const double PoolRecInh::gext_AMPA_I;
const double PoolRecInh::grec_AMPA_I;
const double PoolRecInh::gNMDA_I;
const double PoolRecInh::gGABA_I;
const double PoolRecInh::gl_I;
const double PoolRecInh::cm_I;
double PoolRecInh::dt_times_gL_I_over_cm_I = Brain::dt*(PoolRecInh::gl_I/PoolRecInh::cm_I);
double PoolRecInh::dt_over_cm_I = Brain::dt*(1/PoolRecInh::cm_I);

PoolRecInh::PoolRecInh(string poolName_in, Brain &parentPool_in, int N_in): PoolRec(poolName_in, parentPool_in, N_in)
{
	construct();
};

PoolRecInh::PoolRecInh(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in): PoolRec(poolName_in, parentPool_in, N_in, recordSpikes_in)
{
	construct();
};

void PoolRecInh::construct()
{	
	// Initialize connection vectors, and weight vector:
	GABA = new valarray<double>((double)0, N);

	gext_AMPA = PoolRecInh::gext_AMPA_I;
	grec_AMPA = PoolRecInh::grec_AMPA_I;
	gNMDA = PoolRecInh::gNMDA_I;
	gGABA = PoolRecInh::gGABA_I;
	dt_times_gL_over_cm = PoolRecInh::dt_times_gL_I_over_cm_I;
	dt_over_cm = PoolRecInh::dt_over_cm_I;
};

PoolRecInh::~PoolRecInh() 
{
	delete GABA;
};

void PoolRecInh::propogate()
{	
	// Determine who spiked, and update state vars:
	(*thresholdTest) = (*V) > PoolRec::VMax;
	(*V)[(*thresholdTest)] = PoolRec::VReset;
	(*GABA)[(*thresholdTest)] += valarray<double>(1,(*thresholdTest).sum());
	if (recordSpikes)
	{
		for (i = 0; i < N; i++) 
		{
			if ((*thresholdTest)[i]==true)
			{
				spikeList->addSpike(i,parentBrain->t);
			}
		}
	}
}

void PoolRecInh::updateS()
{
	// Update state vars:
//	(*GABA) -= tau_GABA_Inv_times_dt*(*GABA);
	(*GABA) *= exp(-tau_GABA_Inv_times_dt);
	
	// Update state vars sums:
	GABA_pooled = (*GABA).sum();
    
//    if (poolName == "GI") {
//        cout << "GABA_pooled Spiking: " << GABA_pooled  << "(" << (*ISyn).sum() << ")" << endl;
//    }
    
}

double* PoolRecInh::getStateLocation(State whichState) 
{
	double *returnAddress;
	
	switch (whichState)
	{
		case S_GABA_pooled:
			returnAddress = &GABA_pooled;
			break;
		case S_ISynPoolSum:
			returnAddress = &ISynPoolSum;
			break;      
		case S_VPoolSum:
			returnAddress = &VPoolSum;
			break;  
		default:
			cout << "Unacceptable Monitor variable." << endl;
			exit(-1);
	}
	
	return returnAddress;
};
