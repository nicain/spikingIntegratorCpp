#ifndef MONITORPOOL_H
#define MONITORPOOL_H

#include "Monitor.h"
 
class Brain;
class Pool;

class MonitorPool: public Monitor
{	
public:
	
	// Constructor+Destructor:
	MonitorPool(Brain&, Pool&, State);
	MonitorPool(Brain&, Pool&);
	void construct(Pool&, State);
	~MonitorPool();

	// Member data:
	Pool *whichPool;
	State whichState;
	double* stateLocation;
	string outString;
	
	// Member functions:
	string getString();
	void propogate();
};

#endif
