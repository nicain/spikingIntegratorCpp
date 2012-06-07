#include <iostream>
#include <string>
#include "Brain.h"
#include "Pool.h"
#include "MonitorPool.h"

class Monitor;
 
using namespace std;

MonitorPool::MonitorPool(Brain &parentPool_in, Pool &whichPool_in, State whichState_in): Monitor(parentPool_in)
{
	whichPool = &whichPool_in;
	whichState = whichState_in;
	stateLocation = whichPool->getStateLocation(whichState);
};

MonitorPool::~MonitorPool() 
{

};

string MonitorPool::getString() 
{
	
	outString = str( boost::format("%9.4f") % parentBrain->t );
	outString += "\t" + str(boost::format("%9.5e") % *stateLocation);
	return outString;
};

void MonitorPool::propogate() 
{
	cout << getString() << endl;
};


