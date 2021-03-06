#include <string>
#include "PoolBGHPoisson.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

PoolBGHPoisson::PoolBGHPoisson(string poolName_in,
						  Brain &parentPool_in,
						  int N_in, 
						  bool recordSpikes_in, 
						  double FR_in,
						  double Corr_in,
						  double tOn_in,
						  double tOff_in): PoolBG(poolName_in, 
												  parentPool_in, 
												  N_in, 
												  recordSpikes_in)
{
	construct(FR_in, Corr_in, tOn_in, tOff_in);
};

void PoolBGHPoisson::construct(double FR_in, double Corr_in, double tOn_in, double tOff_in)
{	
	
	// Set member data:
	FR = FR_in;
	Corr = Corr_in;
	tOn = tOn_in;
	tOff = tOff_in;
	
	// Set other stuff:
	gamma = FR*.001;
	Corr_pooled = getInputCorrelation(Corr, 1);
	
	// Random number generators:
	uniDist = new uniform_real_distribution<double>(0,1);
	if (Corr != 0) {
		expDist = new exponential_distribution<double>(gamma/Corr_pooled);
		binomDist = new binomial_distribution<>(N, Corr_pooled);
		binomRnd = new variate_generator<mt19937&,binomial_distribution<> >(parentBrain->myRNG, *binomDist);
		randArray = new int[N];
		for(i = 0 ; i < N; i++ ) 
		{
			randArray[i] = i;
		}
	}
	else 
	{
		expDist = new exponential_distribution<double>(gamma*N);
	}
	expRnd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDist);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);
	
	
};


PoolBGHPoisson::~PoolBGHPoisson() 
{

	if (Corr != 0) 
	{
		delete binomRnd;
		delete binomDist;
		delete randArray;
	}
	
	delete uniRnd;
	delete expRnd;
	delete uniDist;
	delete expDist;
	
};


void PoolBGHPoisson::init()
{
	masterTrain = (*expRnd)();
	
	// Maybe randomize ampa vals? Something like 3-->6...
};



void PoolBGHPoisson::propogate() 
{
	if ((tOn < parentBrain->t) && (parentBrain->t < tOff))
	{		
		while (masterTrain <= parentBrain->t) 
		{
			// This means a spike happened in master neuron, in this step...
			masterTrain += (*expRnd)();
			if (Corr == 0)
			{	
				// Generate the spike:
				whoSpiked = int(N*(*uniRnd)());
				(*AMPA)[whoSpiked] += 1;
				
				// Record the spike:
				if (recordSpikes)
				{
					spikeList->addSpike(whoSpiked, masterTrain);
				}
			}
			else 
			{
				numSpikesInCorrPool = (*binomRnd)();
				for (i=0; i<numSpikesInCorrPool; i++) 
				{						
					// Generate the spike:
					ind2Swap = i+int((*uniRnd)()*(N-i));
					swap(randArray[i], randArray[ind2Swap]);
					whoSpiked = randArray[i];
					(*AMPA)[whoSpiked] += 1;
					
					// Record the spike:
					if (recordSpikes)
					{
						spikeList->addSpike(whoSpiked, masterTrain);
					}						
				}
			}
		}
	}
	else
	{
		masterTrain = parentBrain->t;
	}
}

double PoolBGHPoisson::getInputCorrelation(double p, int N)
{		
	return p/(p + double(N)*(1 - p));
};




















