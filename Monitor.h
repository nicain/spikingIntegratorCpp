#ifndef MONITOR_H
#define MONITOR_H

class Brain;

enum State {S_X, S_AMPA, S_NMDA, S_GABA, S_V, S_ISyn, S_GABA_pooled};

class Monitor
{	
public:
	 
	// Constructor+Destructor:
	Monitor(Brain&);
	~Monitor();

	// Member data:
	Brain *parentBrain;
	
	// Member functions:
	virtual void init();
	virtual void propogate();
	virtual void close();

};

#endif
