#include <string>
#include "PoolRecEx.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRec;

using namespace std;

const float PoolRecEx::tau_AMPA;
const float PoolRecEx::tau_AMPA_rise;
const float PoolRecEx::tau_NMDA;
float PoolRecEx::tau_AMPA_Inv_times_dt = Brain::dt*(1.0/PoolRecEx::tau_AMPA);
float PoolRecEx::tau_AMPA_rise_Inv_times_dt = Brain::dt*(1.0/PoolRecEx::tau_AMPA_rise);
float PoolRecEx::one_minus_tau_NMDA_Inv_times_dt = 1-(Brain::dt*(1.0/PoolRecEx::tau_NMDA));
float PoolRecEx::alpha_times_dt = Brain::dt*PoolRecEx::alpha;

const float PoolRecEx::gext_AMPA_E;
const float PoolRecEx::grec_AMPA_E;
const float PoolRecEx::gNMDA_E;
const float PoolRecEx::gGABA_E;
const float PoolRecEx::gl_E;
const float PoolRecEx::cm_E;
float PoolRecEx::dt_times_gL_E_over_cm_E = Brain::dt*(PoolRecEx::gl_E/PoolRecEx::cm_E);
float PoolRecEx::dt_over_cm_E = Brain::dt*(1/PoolRecEx::cm_E);

PoolRecEx::PoolRecEx(string poolName_in, Brain &parentPool_in, int N_in): PoolRec(poolName_in, parentPool_in, N_in)
{
	construct();
};

PoolRecEx::PoolRecEx(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in): PoolRec(poolName_in, parentPool_in, N_in, recordSpikes_in)
{
	construct();
};

void PoolRecEx::construct()
{	
	// Initialize connection vectors, and weight vector:
	AMPA = new valarray<float>((float)0, N);
	NMDA = new valarray<float>((float)0, N);
	X = new valarray<float>((float)0, N);	


	gext_AMPA = PoolRecEx::gext_AMPA_E;
	grec_AMPA = PoolRecEx::grec_AMPA_E;
	gNMDA = PoolRecEx::gNMDA_E;
	gGABA = PoolRecEx::gGABA_E;
	dt_times_gL_over_cm = PoolRecEx::dt_times_gL_E_over_cm_E;
	dt_over_cm = PoolRecEx::dt_over_cm_E;
	
	
};

PoolRecEx::~PoolRecEx() 
{
	delete AMPA;
	delete NMDA;
	delete X;
};

void PoolRecEx::propogate()
{	
	// Determine who spiked, and update state vars:
	(*thresholdTest) = (*V) > PoolRec::VMax;
	(*V)[(*thresholdTest)] = PoolRec::VReset;
	(*AMPA)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
	(*X)[(*thresholdTest)] += valarray<float>(1,(*thresholdTest).sum());
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

void PoolRecEx::updateS()
{
	// Update state vars:
	(*AMPA) -= tau_AMPA_Inv_times_dt*(*AMPA);
	(*X) -= tau_AMPA_rise_Inv_times_dt*(*X);
	(*NMDA) = one_minus_tau_NMDA_Inv_times_dt*(*NMDA) + alpha_times_dt*(*X)*(*unitVector - (*NMDA));
	
	// Update state vars sums:
	AMPA_pooled = (*AMPA).sum();
	NMDA_pooled = (*NMDA).sum();
}
