#include <string>
#include "PoolRecEx.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRec;

using namespace std;

const double PoolRecEx::tau_AMPA;
const double PoolRecEx::tau_AMPA_rise;
const double PoolRecEx::tau_NMDA;
double PoolRecEx::tau_AMPA_Inv_times_dt = Brain::dt*(1.0/PoolRecEx::tau_AMPA);
double PoolRecEx::tau_AMPA_rise_Inv_times_dt = Brain::dt*(1.0/PoolRecEx::tau_AMPA_rise);
double PoolRecEx::one_minus_tau_NMDA_Inv_times_dt = 1-(Brain::dt*(1.0/PoolRecEx::tau_NMDA));
double PoolRecEx::alpha_times_dt = Brain::dt*PoolRecEx::alpha;

const double PoolRecEx::gext_AMPA_E;
const double PoolRecEx::grec_AMPA_E;
const double PoolRecEx::gNMDA_E;
const double PoolRecEx::gGABA_E;
const double PoolRecEx::gl_E;
const double PoolRecEx::cm_E;
double PoolRecEx::dt_times_gL_E_over_cm_E = Brain::dt*(PoolRecEx::gl_E/PoolRecEx::cm_E);
double PoolRecEx::dt_over_cm_E = Brain::dt*(1/PoolRecEx::cm_E);

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
	AMPA = new valarray<double>((double)0, N);
	NMDA = new valarray<double>((double)0, N);
	X = new valarray<double>((double)0, N);	


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
	(*AMPA)[(*thresholdTest)] += valarray<double>(1,(*thresholdTest).sum());
	(*X)[(*thresholdTest)] += valarray<double>(1,(*thresholdTest).sum());
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
//	(*AMPA) -= tau_AMPA_Inv_times_dt*(*AMPA);
//	(*X) -= tau_AMPA_rise_Inv_times_dt*(*X);
	
	(*AMPA) *= exp(-tau_AMPA_Inv_times_dt);
	(*X) *= exp(-tau_AMPA_rise_Inv_times_dt);
	
	(*NMDA) = one_minus_tau_NMDA_Inv_times_dt*(*NMDA) + alpha_times_dt*(*X)*(*unitVector - (*NMDA));
	
	// Update state vars sums:
	AMPA_pooled = (*AMPA).sum();
	NMDA_pooled = (*NMDA).sum();
}

double* PoolRecEx::getStateLocationConductance(int whichNeuron, State whichState) 
{
	double *returnAddress;
	
	switch (whichState)
	{
		case S_AMPA:
			returnAddress = &((*AMPA)[whichNeuron]);
			break;
		case S_NMDA:
			returnAddress = &((*NMDA)[whichNeuron]);
			break;
		case S_X:
			returnAddress = &((*X)[whichNeuron]);
			break;
		default:
			cout << "Unacceptable Monitor variable." << endl;
			exit(-1);
	}
	
	return returnAddress;
};
