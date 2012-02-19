#include <string>
#include "PoolBGHPoissonSIP.h"
#include "Brain.h"
#include "SpikeList.h"

class PoolBG;

using namespace boost::random;
using namespace std;

PoolBGHPoissonSIP::PoolBGHPoissonSIP(string poolName_in,
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

void PoolBGHPoissonSIP::construct(double FR_in, double Corr_in, double tOn_in, double tOff_in)
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
		expDist = new exponential_distribution<double>(gamma*Corr_pooled);
		expDistInd = new exponential_distribution<double>(gamma*(1-Corr_pooled)*N);
       	expRndInd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDistInd);
	}
	else 
	{
		expDist = new exponential_distribution<double>(gamma*N);
	}
	expRnd = new variate_generator<mt19937&,exponential_distribution<double> >(parentBrain->myRNG, *expDist);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);
	
};


PoolBGHPoissonSIP::~PoolBGHPoissonSIP() 
{

	if (Corr != 0) 
	{
        delete expDistInd;
        delete expRndInd;
	}
	
	delete uniRnd;
	delete expRnd;
	delete uniDist;
	delete expDist;
	
};


void PoolBGHPoissonSIP::init()
{
	masterTrain = (*expRnd)();
    if (Corr != 0) 
    {
        indTrain = (*expRndInd)();
    }
	
	// Maybe randomize ampa vals? Something like 3-->6...
};



void PoolBGHPoissonSIP::propogate() 
{
	if ((tOn < parentBrain->t) && (parentBrain->t < tOff))
	{
		
        if (Corr == 0)
        {	
            while (masterTrain <= parentBrain->t)
            {
                // This means a spike happened in master neuron, in this step...
                masterTrain += (*expRnd)();
            
                if ((tOn < masterTrain) && (masterTrain < tOff))
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
            }
        }
        else
        {
        
            while ((masterTrain <= parentBrain->t) || indTrain <= parentBrain->t)
            {
                // Don't advance until no more spikes, indep. and master.
                
                if (masterTrain < indTrain)
                {
                    // Next spike was a master spike:
                    
                    masterTrain += (*expRnd)(); // Set up NEXT master spike
                    
                    if ((tOn < masterTrain) && (masterTrain < tOff))
                    {
                        // Generate spikes in all neurons:
                        for (i=0; i<N; i++) 
                        {						
                            (*AMPA)[i] += 1;
                            
                            // Record the spike:
                            if (recordSpikes)
                            {
                                spikeList->addSpike(i, masterTrain);
                            }						
                        }
                    }
                    
                }
                else
                {
                    // Next spike was an ind. spike:
                    
                    indTrain += (*expRndInd)(); // Set up NEXT ind. spike
                    
                    if ((tOn < indTrain) && (indTrain < tOff))
                    {
                        // Generate the spike:
                        whoSpiked = int(N*(*uniRnd)());
                        (*AMPA)[whoSpiked] += 1;
                        
                        // Record the spike:
                        if (recordSpikes)
                        {
                            spikeList->addSpike(whoSpiked, indTrain);
                        }
                    }
                    
                }
                
            }
        }
	}
	else
	{
		masterTrain = parentBrain->t;
        if (Corr != 0) 
        {
            indTrain = parentBrain->t;
        }
        
	}
}

double PoolBGHPoissonSIP::getInputCorrelation(double p, int N)
{		
	return p/(p + double(N)*(1 - p));
};




















