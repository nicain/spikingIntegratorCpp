#include <fstream>
#include "SI_settings.h"
#include "ExPool.h"

using namespace std;
using namespace SI;

ExPool::ExPool (string poolName_in,
				int totalNeurons_in,
				bool recordSpikes_in) 
{
	// Set the name:
	poolName = poolName_in;

	// Setting primitives:
	//tau_AMPA_Inv_times_dt = 1/tau_AMPA*dt;
	recordSpikes = recordSpikes_in;
	totalNeurons = totalNeurons_in;
	
	// Spike recording:
	if (recordSpikes)
	{
		spikeRecord_t = new vector<float>;
		spikeRecord_n = new vector<int>;
	}
	
};

ExPool::~ExPool () 
{
	if (recordSpikes)
	{
		delete spikeRecord_t;
		delete spikeRecord_n;
	}
};

void ExPool::init()
{
	
};

void ExPool::propogate() 
{

}

float ExPool::getFR()
{
	return float((*spikeRecord_n).size())/(totalNeurons);
}

void ExPool::writeSpikes(string UUID_string)
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

