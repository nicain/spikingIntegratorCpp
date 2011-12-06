#ifndef MONITORBRAIN_H
#define MONITORBRAIN_H

#include "Monitor.h"
 
class Brain;

class MonitorBrain: public Monitor
{	
public:
	
	// Constructor+Destructor:
	MonitorBrain(Brain&);
	~MonitorBrain();

	// Member data:
	string outString;
	
	// Member functions:
	string getString();
	void propogate();
};

#endif
