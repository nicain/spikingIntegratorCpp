#include <string>
#include <iostream>
#include "PoolBGFile.h"
#include "Brain.h"
#include "SpikeList.h"
#include <set>

class PoolBG;
class SpikeList;

using namespace std;

PoolBGFile::PoolBGFile(string poolName_in, Brain &parentPool_in, string fileName_in): PoolBG(poolName_in, parentPool_in, 0)
{
	construct(fileName_in, poolName_in);
};
 
PoolBGFile::PoolBGFile(string poolName_in, Brain &parentPool_in, bool recordSpikes_in, string fileName_in): PoolBG(poolName_in, parentPool_in, 0, recordSpikes_in)
{
	construct(fileName_in, poolName_in);
};

void PoolBGFile::construct(string fileName_in, string poolName_in)
{	
	// Set member data:
	delete spikeList;
	spikeList = new SpikeList(fileName_in, poolName_in);
	set<int> *nameSet = new set<int>;
	pair< set<int>::iterator, bool > pr;
	for (int i = 0; i <	(spikeList->n)->size(); i++) {
		pr = nameSet->insert((*(spikeList->n))[i]);	
	}
	

	
	AMPA->resize(nameSet->size());
};

PoolBGFile::~PoolBGFile() 
{

};

void PoolBGFile::init() 
{
	currInd=0;
};

void PoolBGFile::propogate() 
{	
	while  ((*(spikeList->t))[currInd] < (parentBrain->t + parentBrain->dt ) && (currInd < (spikeList->t)->size()))
	{	
		deltaT =  (*(spikeList->t))[currInd] - parentBrain->t;
		
		(*AMPA)[(*(spikeList->n))[currInd]] += exp(deltaT/PoolBG::tau_AMPA);
		
		currInd += 1;
	}
};






















