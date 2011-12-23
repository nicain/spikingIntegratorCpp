#include <string>
#include "PoolBGInHPoisson.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

const double PoolBGInHPoisson::sigma;

PoolBGInHPoisson::PoolBGInHPoisson(string poolName_in,
						  Brain &parentPool_in,
						  int N_in, 
						  bool recordSpikes_in, 
						  double mu_in,
						  double rho_in,
						  double Corr_in,
						  double tOn_in,
						  double tOff_in): PoolBG(poolName_in, 
												  parentPool_in, 
												  N_in, 
												  recordSpikes_in)
{
	construct(mu_in, rho_in, Corr_in, tOn_in, tOff_in);
};

void PoolBGInHPoisson::construct(double mu_in, double rho_in, double Corr_in, double tOn_in, double tOff_in)
{	
	
	// Set member data:
	mu = mu_in;
	rho = rho_in;
	Corr = Corr_in;
	tOn = tOn_in;
	tOff = tOff_in;
	gamma = new valarray<double>((double)0, N);
	
	// Random number generators:

	normDist = new normal_distribution<double>(mu, PoolBGInHPoisson::sigma);
	normRnd = new variate_generator<mt19937&,normal_distribution<double> >(parentBrain->myRNG, *normDist);	
//	uniDist = new uniform_real_distribution<double>(0,1);
//	if (Corr != 0) {
//		expDist = new exponential_distribution<double>(gamma/Corr_pooled);
//		binomDist = new binomial_distribution<>(N, Corr_pooled);
//		binomRnd = new variate_generator<mt19937&,binomial_distribution<> >(parentBrain->myRNG, *binomDist);
//		randArray = new int[N];
//		for(i = 0 ; i < N; i++ ) 
//		{
//			randArray[i] = i;
//		}
//	}
//	else 
//	{
//		expDist = new exponential_distribution<double>(gamma*N);
//	}
//	expRnd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDist);
//	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);


};


PoolBGInHPoisson::~PoolBGInHPoisson() 
{
	delete gamma;
	
//	if (Corr != 0) 
//	{
//		delete binomRnd;
//		delete binomDist;
//	}
//	
//	delete uniRnd;
//	delete expRnd;
	delete normRnd;
	delete normDist;
//	delete uniDist;
//	delete expDist;

};


void PoolBGInHPoisson::init()
{
//	setCorrPooled(Corr);
	setGamma();
};



void PoolBGInHPoisson::propogate() 
{
//	if ((tOn < parentBrain->t) && (parentBrain->t < tOff))
//	{		
//		while (masterTrain <= parentBrain->t) 
//		{
//			// This means a spike happened in master neuron, in this step...
//			masterTrain += (*expRnd)();
//			if (Corr == 0)
//			{	
//				// Generate the spike:
//				whoSpiked = int(N*(*uniRnd)());
//				(*AMPA)[whoSpiked] += 1;
//				
//				// Record the spike:
//				if (recordSpikes)
//				{
//					spikeList->addSpike(whoSpiked, masterTrain);
//				}
//			}
//			else 
//			{
//				numSpikesInCorrPool = (*binomRnd)();
//				for (i=0; i<numSpikesInCorrPool; i++) 
//				{						
//					// Generate the spike:
//					ind2Swap = i+int((*uniRnd)()*(N-i));
//					swap(randArray[i], randArray[ind2Swap]);
//					whoSpiked = randArray[i];
//					(*AMPA)[whoSpiked] += 1;
//					
//					// Record the spike:
//					if (recordSpikes)
//					{
//						spikeList->addSpike(whoSpiked, masterTrain);
//					}						
//				}
//			}
//		}
//	}
//	else
//	{
//		masterTrain = parentBrain->t;
//	}
}

//void PoolBGInHPoisson::setCorrPooled(double p)
//{		
//	Corr_pooled = p/(p + double(1)*(1 - p));
//};

void PoolBGInHPoisson::setGamma()
{
	for (i=0; i<N; i++)
	{
		(*gamma)[i] = (*normRnd)()*.001;		
		if ((*gamma)[i] < 0)
		{
			(*gamma)[i] = 0;
		}
	}

};


















