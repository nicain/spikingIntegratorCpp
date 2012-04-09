#include <string>
#include "PoolPoisson.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

PoolPoisson::PoolPoisson(string poolName_in,
						  Brain &parentPool_in,
						  int N_in, 
						  bool recordSpikes_in, 
						  double FR_in,
						  double Corr_in,
						  double tOn_in,
						  double tOff_in,
						  double tmax_in): Pool(poolName_in, 
												  parentPool_in, 
												  N_in)
						
{
	construct(parentPool_in,FR_in, Corr_in, tOn_in, tOff_in, tmax_in);


};

void PoolPoisson::construct(Brain &parentPool_in, double FR_in, double Corr_in, double tOn_in, double tOff_in, double tmax_in)
{	
	
	// set constants
	tau = 2e-3;
	
	// Set member data:
	FR = FR_in;
	Corr = Corr_in;
	tOn = tOn_in;
	tOff = tOff_in;
	tmax = tmax_in;
	dt = parentPool_in.dt*0.001;

	L = (double)(tmax/dt);
	spks.resize(L,0);


		
	// Set other stuff:
	//gamma = FR*.001;
	Corr_pooled = getInputCorrelation(Corr, 1);
	// Random number generators:
	uniDist = new uniform_real_distribution<double>(0,1);
	if (Corr != 0) {
			cout << 1 << endl;
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
		expDist = new exponential_distribution<double>(FR);
	}
	expRnd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDist);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);

};


PoolPoisson::~PoolPoisson() 
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


void PoolPoisson::init()
{
	T = 0;
	masterTrain = (*expRnd)();
	t = dt;
	spks[T]= 0;
	
	if ((tOn < t) && (t < tOff))
	{		
		while (masterTrain <= t) 
		{
			// This means a spike happened.... in this step:
			if (Corr == 0)
			{	
				deltaT = t - masterTrain;
				// add the exponential
				spks[T] = spks[T] + (double)1/N * 1/tau * exp(-deltaT/tau);
				
			}
			else 
			{
				numSpikesInCorrPool = (*binomRnd)();
				for (i=0; i<numSpikesInCorrPool; i++) 
				{						
					// Generate the spike:
					ind2Swap = i+int((*uniRnd)()*(N-i));
					swap(randArray[i], randArray[ind2Swap]);
					//whoSpiked = randArray[i];
					
				}
			}
			masterTrain += (*expRnd)();
			
		}
		
	}
	else
	{
		masterTrain = t;
	}
	T++;
	
	for ( t=2*dt; t <= tmax; t = t + dt)
	{
		deltaT = dt;
		//decay lingering spike effects
		spks[T] = spks[T-1]*exp(-deltaT/tau);
		if ((tOn < t) && (t < tOff))
		{		
			while (masterTrain <= t) 
			{
				// This means a spike happened.... in this step:
				if (Corr == 0)
				{	
					deltaT = t - masterTrain;
					// add the spike
					spks[T] = spks[T] + (double)1/N * 1/tau * exp(-deltaT/tau);
				}
				else 
				{
					numSpikesInCorrPool = (*binomRnd)();
					for (i=0; i<numSpikesInCorrPool; i++) 
					{						
						// Generate the spike:
						ind2Swap = i+int((*uniRnd)()*(N-i));
						swap(randArray[i], randArray[ind2Swap]);
						//whoSpiked = randArray[i];
				
					}
				}
				masterTrain += (*expRnd)();

			}
			
		}
		else
		{
			masterTrain = t;
		}
		T++;
	
	}
	
	
};



void PoolPoisson::propogate() 
{

}

double PoolPoisson::getInputCorrelation(double p, int N)
{		
	return p/(p + double(N)*(1 - p));
};




















