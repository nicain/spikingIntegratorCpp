#include <string>
#include "PoolBGOU.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

PoolBGOU::PoolBGOU(string poolName_in,
						  Brain &parentPool_in,
						  int N_in, 
						  bool recordSpikes_in, 
						  double FR_in,
						  double tOn_in,
						  double tOff_in): PoolBG(poolName_in, 
												  parentPool_in, 
												  N_in, 
												  recordSpikes_in)
{
	constructFR(FR_in, tOn_in, tOff_in);
};

PoolBGOU::PoolBGOU(string poolName_in,
                   Brain &parentPool_in,
                   int N_in, 
                   bool recordSpikes_in, 
                   double mu_in,
                   double sigma_in,
                   double tOn_in,
                   double tOff_in): PoolBG(poolName_in, 
                                           parentPool_in, 
                                           N_in, 
                                           recordSpikes_in)
{
	constructMuSigma(mu_in, sigma_in, tOn_in, tOff_in);
};

void PoolBGOU::constructFR(double FR_in, double tOn_in, double tOff_in)
{	
	
	// Set member data:
	FR = FR_in;
	tOn = tOn_in;
	tOff = tOff_in;
	
	// Set other stuff:
	mu = PoolBG::tau_AMPA*FR*.001;
	sigma = sqrt(mu/2); 
	


	normDist = new normal_distribution<double>(0, 1);
	normRnd = new variate_generator<mt19937&,normal_distribution<double> >(parentBrain->myRNG, *normDist);
	
	randVec = new valarray<double>(double(0), N);
};

void PoolBGOU::constructMuSigma(double mu_in, double sigma_in, double tOn_in, double tOff_in)
{	
	
	// Set member data:
	FR = -1;
	tOn = tOn_in;
	tOff = tOff_in;
	
	// Set other stuff:
	mu = mu_in;
	sigma = sigma_in;
  
    normDist = new normal_distribution<double>(0, 1);
//	normDist = new normal_distribution<double>(mu, sqrt((1 - exp((-2*Brain::dt)/(PoolBG::tau_AMPA)))*pow(sigma,double(2))));
	normRnd = new variate_generator<mt19937&,normal_distribution<double> >(parentBrain->myRNG, *normDist);
	
	randVec = new valarray<double>(double(0), N);
};


PoolBGOU::~PoolBGOU() 
{
	delete normDist;
	delete normRnd;
	delete randVec;
};


void PoolBGOU::init()
{
	for (int i=0; i<N; i++) {
		(*AMPA)[i] = mu + sigma*(*normRnd)();
		(*randVec)[i] = (*normRnd)();
	}
};



void PoolBGOU::updateS() 
{
	
	for (int i=0; i<N; i++) {
		(*randVec)[i] = (*normRnd)();
	}
	
    //(*AMPA) = ((*AMPA) - mu)/exp((Brain::dt)/(PoolBG::tau_AMPA)) + (*randVec);
    //(*AMPA) = ((*AMPA) - mu)/exp((Brain::dt)/(PoolBG::tau_AMPA)) + (*randVec);
    (*AMPA) = (*AMPA) + Brain::dt*((mu - (*AMPA))/PoolBG::tau_AMPA + sigma*sqrt(2*Brain::dt/PoolBG::tau_AMPA)*(*randVec));
    
    AMPASum = (*AMPA).sum();
    cout << AMPASum << endl;
//    cout << mu << endl;
}




















