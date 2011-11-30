#include <string>
#include "PoolBG.h"
#include "Brain.h"

class Brain;
class Pool;
class Monitor;

using namespace std;

const float PoolBG::tau_AMPA;
float PoolBG::tau_AMPA_Inv_times_dt = Brain::dt*(1.0/PoolBG::tau_AMPA);

PoolBG::PoolBG(string poolName_in, Brain &parentPool_in, int N_in): Pool(poolName_in, parentPool_in, N_in)
{
	construct();
};

PoolBG::PoolBG(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in): Pool(poolName_in, parentPool_in, N_in, recordSpikes_in)
{
	construct();
};

void PoolBG::construct()
{
	// Set member data:
	AMPA = new valarray<float>((float)0, N);
};

PoolBG::~PoolBG() 
{
	delete AMPA;
};

void PoolBG::updateS() 
{
//	(*AMPA) -= tau_AMPA_Inv_times_dt*(*AMPA);
	(*AMPA) *= exp(-tau_AMPA_Inv_times_dt);
};

float* PoolBG::getStateLocation(int whichNeuron, State whichState) 
{
	
	float *returnAddress;
	
	switch (whichState)
	{
		case S_AMPA:
			returnAddress = &((*AMPA)[whichNeuron]);
			break;
		default:
			cout << "Unacceptable Monitor variable." << endl;
			exit(-1);
	}
	
	return returnAddress;
};
























