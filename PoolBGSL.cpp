#include <string>
#include <iostream>
#include "PoolBGSL.h"
#include "Brain.h"
#include "SpikeList.h"
#include <set>

class PoolBG;
class SpikeList;

using namespace std;

PoolBGSL::PoolBGSL(string poolName_in, Brain &parentPool_in, SpikeList &spikeList_in): PoolBG(poolName_in, parentPool_in, 0)
{

	// Set member data:
	delete spikeList;
	spikeList = new SpikeList(poolName_in);
	
	set<int> *nameSet = new set<int>;
	pair< set<int>::iterator, bool > pr;
	for (int i = 0; i <	(spikeList_in.n)->size(); i++) {
		pr = nameSet->insert((*(spikeList_in.n))[i]);	
	}

	AMPA->resize(nameSet->size());
	
	for (int i = 0; i <	(spikeList_in.n)->size(); i++) {
		spikeList->addSpike((*(spikeList_in.n))[i], (*(spikeList_in.t))[i]);
	}
};

PoolBGSL::~PoolBGSL() 
{

};

void PoolBGSL::init() 
{
	currInd=0;
};

void PoolBGSL::propogate() 
{	
	while  ((*(spikeList->t))[currInd] < (parentBrain->t + parentBrain->dt ) && (currInd < (spikeList->t)->size()))
	{	
		deltaT =  (*(spikeList->t))[currInd] - parentBrain->t;
		
		(*AMPA)[(*(spikeList->n))[currInd]] += exp(deltaT/PoolBG::tau_AMPA);
		
		currInd += 1;
	}
};






















