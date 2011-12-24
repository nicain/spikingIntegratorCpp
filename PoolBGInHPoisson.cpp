#include <string>
#include "PoolBGInHPoisson.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

const double PoolBGInHPoisson::sigma;
const double PoolBGInHPoisson::resetGammaDeltaT;

PoolBGInHPoisson::PoolBGInHPoisson(string poolName_in,
						  Brain &parentPool_in,
						  int N_in, 
						  bool recordSpikes_in, 
						  double mu_in,
						  double rho_in,
						  double tOn_in,
						  double tOff_in): PoolBG(poolName_in, 
												  parentPool_in, 
												  N_in, 
												  recordSpikes_in)
{
	construct(mu_in, rho_in, tOn_in, tOff_in);
};

void PoolBGInHPoisson::construct(double mu_in, double rho_in, double tOn_in, double tOff_in)
{	
	
	// Set member data:
	mu = mu_in*.001;  //Conversion for ms., from Hz in.
	rho = rho_in;
	tOn = tOn_in;
	tOff = tOff_in;
	masterTrain = new valarray<double>((double)0, N);
	
	// Matrix and vector initializations:
	gamma = new boost::numeric::ublas::vector<double> (N,double(1));
	muV = new boost::numeric::ublas::vector<double> (N,mu);
	S = new boost::numeric::ublas::matrix<double> (N,N);
	IdM = new boost::numeric::ublas::identity_matrix<double> (N,N);
	OnesM = new boost::numeric::ublas::scalar_matrix<double> (N,N,1);
	
	// Create the correlating matrix:
	double b = PoolBGInHPoisson::sigma*sqrt(1 - rho);
	double a = (PoolBGInHPoisson::sigma*sqrt(1 + rho*(double(N)-1))-b)/double(N);
	(*S) = a*(*OnesM) + b*(*IdM);
	delete IdM;
	delete OnesM;
	
	// Random number generators:
	normDist = new normal_distribution<double>(0, 1);
	normRnd = new variate_generator<mt19937&,normal_distribution<double> >(parentBrain->myRNG, *normDist);	
	expDist = new exponential_distribution<double>(1);
	expRnd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDist);
};


PoolBGInHPoisson::~PoolBGInHPoisson() 
{
	delete gamma;
	delete muV;
	delete S;

	delete masterTrain;
	delete normRnd;
	delete normDist;
};


void PoolBGInHPoisson::init()
{
	setGamma();

	nextGammaResetT = parentBrain->t + resetGammaDeltaT;
	
	for (i=0; i<N; i++) 
	{
		(*masterTrain)[i] += 1/((*gamma)[i])*(*expRnd)();
//		cout << "masterTrain[" << i << "]    " << (*masterTrain)[i] << endl;
	}


};

void PoolBGInHPoisson::propogate() 
{
	if (parentBrain->t > nextGammaResetT) 
	{
		setGamma();
		nextGammaResetT += resetGammaDeltaT;
	}
	
	if ((tOn < parentBrain->t) && (parentBrain->t < tOff))
	{		
		for (i=0; i<N; i++) 
		{


			while ((*masterTrain)[i] <= parentBrain->t) 
			{				
				

				
				// Generate the spike:
				(*AMPA)[i] += 1;
				
				// Record the spike:
				if (recordSpikes)
				{
					spikeList->addSpikeSorted(i, (*masterTrain)[i]);
				}
				
				(*masterTrain)[i] += 1/((*gamma)[i])*(*expRnd)();
			}
		}
	}
	else
	{
		for (i=0; i<N; i++) 
		{
			(*masterTrain)[i] = parentBrain->t + 1/((*gamma)[i])*(*expRnd)();	
		}
	}
}

void PoolBGInHPoisson::setGamma()
{
	
	// Generate an IID gamma vector:
	for (i=0; i<N; i++) {
		(*gamma)[i] = (*normRnd)();		 // mu
	}
	
	// Correlate the gamma values:
	(*gamma) = boost::numeric::ublas::prod((*S),(*gamma));
		
	// Set the mean:
	(*gamma) += (*muV);
	
	// Ensure each gamma is positive:
	for (i=0; i<N; i++) {
		if ((*gamma)[i] < 0) {
			(*gamma)[i] = 0;
		}
	}
	
	// Reset the next spike time, based on new gammas:
	for (i=0; i<N; i++) 
	{
		(*masterTrain)[i] = parentBrain->t + 1/((*gamma)[i])*(*expRnd)();	
	}
	
	cout << 1000*(*gamma)[0] << endl;

};


















