#include <fstream>
#include "SI_settings.h"
#include "BGPool.h"

using namespace boost::random;
using namespace std;
using namespace SI;


float BGPool::getInputCorrelation(float p, int N)
{		
	return p/(p + float(N)*(1 - p));
};

void BGPool::init()
{
	masterTrain = (*expRnd)();
	
	// Maybe randomize ampa vals? Something like 3-->6...
};

void BGPool::propogate() 
{
	if ((tOn < t) && (t < tOff))
	{		
		while (masterTrain <= t) 
		{
			// This means a spike happened in master neuron, in this step...
			masterTrain += (*expRnd)();
			if (Corr == 0)
			{	
				// Generate the spike:
				whoSpiked = int(totalNeurons*(*uniRnd)());
				(*AMPA)[whoSpiked] += 1;
				
				// Record the spike:
				if (recordSpikes)
				{
					(*spikeRecord_n).push_back(whoSpiked);
					(*spikeRecord_t).push_back(masterTrain);
				}
			}
			else 
			{
				numSpikesInCorrPool = (*binomRnd)();
				for (i=0; i<numSpikesInCorrPool; i++) 
				{						
					// Generate the spike:
					ind2Swap = i+int((*uniRnd)()*(totalNeurons-i));
					swap(randArray[i], randArray[ind2Swap]);
					whoSpiked = randArray[i];
					(*AMPA)[whoSpiked] += 1;
					
					// Record the spike:
					if (recordSpikes)
					{
						(*spikeRecord_n).push_back(whoSpiked);
						(*spikeRecord_t).push_back(masterTrain);
					}						
				}
			}
		}
	}
	else
	{
		masterTrain = t;
	}
	
	// Propogate state vars:
	(*AMPA) -= tau_AMPA_Inv_times_dt*(*AMPA);
	
	// Pooling:
	if (neuronsPerSubPool != 1)
	{
		for(i = 0; i < numberSubPools; i++)
		{
			*tmpAMPA = (*AMPA)[*masks[i]];
			(*AMPA_pooled)[i] = (*tmpAMPA).sum();
		}
	}
}

float BGPool::getFR()
{
	return float((*spikeRecord_n).size())/(totalNeurons)/t*1000;
}

void BGPool::writeSpikes(string UUID_string)
{
	// Set up file:
	string myUnderscore = "_";
	string fileTypeExtension = ".ntf";
	string fileName = poolName + myUnderscore + UUID_string + fileTypeExtension;
	ofstream myfile;
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	myfile.open(charFileName);
	
	// Write the spikes in "name-time format":
	for (i = 1; i <= (*spikeRecord_t).size()-1; i++)
	{
		myfile << poolName << "_" << (*spikeRecord_n)[i] << "\t" << (*spikeRecord_t)[i] << endl;
	}
	
	myfile.close();
	return;
};

BGPool::BGPool (string poolName_in,
				int neuronsPerSubPool_in, 
				int numberSubPools_in, 
				float FR_in,
				float Corr_in,
				bool recordSpikes_in, 
				float tOn_in,
				float tOff_in) 
{
	// Set the name:
	poolName = poolName_in;
	
	// Setting primitives:
	tOn = tOn_in;
	tOff = tOff_in;
	tau_AMPA_Inv_times_dt = 1/tau_AMPA*dt;
	FR = FR_in;
	recordSpikes = recordSpikes_in;
	neuronsPerSubPool = neuronsPerSubPool_in;
	Corr = Corr_in;
	Corr_pooled = getInputCorrelation(Corr, neuronsPerSubPool);
	numberSubPools = numberSubPools_in;
	totalNeurons = neuronsPerSubPool_in*numberSubPools_in;
	gamma = FR*.001;
	
	// Random number generators:
	uniDist = new uniform_real_distribution<float>(0,1);
	if (Corr_in != 0) {
		expDist = new exponential_distribution<float>(gamma/Corr_pooled);
		binomDist = new binomial_distribution<>(totalNeurons, Corr_pooled);
		binomRnd = new variate_generator<mt19937&,binomial_distribution<> >(myRNG, *binomDist);
		randArray = new int[totalNeurons];
		for(i = 0 ; i < totalNeurons; i++ ) 
		{
			randArray[i] = i;
		}
	}
	else 
	{
		expDist = new exponential_distribution<float>(gamma*totalNeurons);
	}
	expRnd = new variate_generator<mt19937&,exponential_distribution<float> >(myRNG, *expDist);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<float> >(myRNG, *uniDist);
	
	// Setting state vectors:
	AMPA = new valarray<float>((float)0, totalNeurons);
	if (neuronsPerSubPool == 1)
	{
		AMPA_pooled = AMPA;
	}
	else 
	{
		AMPA_pooled = new valarray<float>((float)0, numberSubPools);
		masks = new valarray<bool>*[numberSubPools];
		for(i = 0; i < numberSubPools; i++)
		{
			masks[i] = new valarray<bool>(10);
			for (j = i*neuronsPerSubPool; j < ((i+1)*neuronsPerSubPool); j++) 
			{
				(*masks[i])[j]=true;
			}
		}
		tmpAMPA = new valarray<float>((float)0, neuronsPerSubPool);
	}
	
	// Spike recording:
	if (recordSpikes)
	{
		spikeRecord_t = new vector<float>;
		spikeRecord_n = new vector<int>;
	}
};

BGPool::~BGPool () 
{
	delete AMPA;
	if (neuronsPerSubPool != 1)
	{
		delete AMPA_pooled;
		for(i = 0; i < numberSubPools; i++)
		{
			delete masks[i];
		}
		delete masks;
		delete tmpAMPA;
	}
	if (recordSpikes)
	{
		delete spikeRecord_t;
		delete spikeRecord_n;
	}
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