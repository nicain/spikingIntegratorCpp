#include <iostream>
#include <string>
#include <sstream>
#include "Brain.h"
#include "Pool.h"
#include "MonitorNeuron.h"

class Monitor;

using namespace std;

MonitorNeuron::MonitorNeuron(Brain &parentPool_in, Pool &whichPool_in, int whichNeuron_in, State whichState_in): Monitor(parentPool_in)
{
	whichPool = &whichPool_in;
	whichNeuron = whichNeuron_in;
	whichState = whichState_in;
	stateLocation = whichPool->getStateLocation(whichNeuron, whichState);
};

MonitorNeuron::~MonitorNeuron() 
{

};

string MonitorNeuron::getString() 
{
	stringstream out;
	out << parentBrain->t << "\t" << *stateLocation;
	return out.str();
};

void MonitorNeuron::propogate() 
{
	cout << getString() << endl;
};


