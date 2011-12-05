#ifndef MONITORNEURON_H
#define MONITORNEURON_H

#include "Monitor.h"
 
class Brain;
class Pool;

class MonitorNeuron: public Monitor
{	
public:
	
	// Constructor+Destructor:
	MonitorNeuron(Brain&, Pool&, int, State);
	MonitorNeuron(Brain&, Pool&, int);
	void construct(Pool&, int, State);
	~MonitorNeuron();

	// Member data:
	Pool *whichPool;
	int whichNeuron;
	State whichState;
	double* stateLocation;
	string outString;
	
	// Member functions:
	string getString();
	void propogate();
};

#endif
