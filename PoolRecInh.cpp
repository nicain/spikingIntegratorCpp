#include <string>
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRec;

using namespace std;

const float PoolRecInh::tau_GABA;
float PoolRecInh::tau_GABA_Inv_times_dt = Brain::dt*(1.0/PoolRecInh::tau_GABA);
const float PoolRecInh::gext_AMPA_I;
const float PoolRecInh::grec_AMPA_I;
const float PoolRecInh::gNMDA_I;
const float PoolRecInh::gGABA_I;
const float PoolRecInh::gl_I;
const float PoolRecInh::cm_I;
float PoolRecInh::dt_times_gL_I_over_cm_I = Brain::dt*(PoolRecInh::gl_I/PoolRecInh::cm_I);
float PoolRecInh::dt_over_cm_I = Brain::dt*(1/PoolRecInh::cm_I);

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
	GABA = new valarray<float>((float)0, N);

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
	(*GABA)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
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
	(*GABA) -= tau_GABA_Inv_times_dt*(*GABA);
	
	// Update state vars sums:
	GABA_pooled = (*GABA).sum();
}
