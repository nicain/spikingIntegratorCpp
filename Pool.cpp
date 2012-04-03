#include <string>
#include "Brain.h"
#include "SpikeList.h"
#include "Pool.h"

using namespace std;

Pool::Pool(string poolName_in, Brain &parentPool_in, int N_in)
{
	construct(poolName_in, parentPool_in, N_in, false);
};

Pool::Pool(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in)
{
	construct(poolName_in, parentPool_in, N_in, recordSpikes_in);
};

void Pool::construct(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in)
{
	parentPool_in.addPool(*this);

	// Set member data:
	poolName = poolName_in;
	N = N_in;
	recordSpikes = recordSpikes_in;
	parentBrain = &parentPool_in;
	spikeList = new SpikeList(poolName);
	
	

};

Pool::~Pool() 
{
	delete spikeList;
};

void Pool::print() 
{
	spikeList->print();
};

void Pool::init() 
{
	
};

void Pool::propogate() 
{

};

void Pool::updateS() 
{ 
	
};

void Pool::updateV() 
{
	
};

double* Pool::getStateLocation(int whichNeuron, State whichState) 
{
	return 0;
};

void Pool::toFile(string appendString) 
{
	string myUnderscore = "_";
	string fileNamePrefix = poolName + myUnderscore + appendString;
	
	spikeList->toFile(fileNamePrefix);
};

void Pool::toFileExact(string fileNamePrefix) 
{	
	spikeList->toFile(fileNamePrefix);
};

double Pool::getFR() 
{
	return double((spikeList->size()))/parentBrain->t/(double(N))*double(1000);
};

