#ifndef MONITOR_H
#define MONITOR_H

class Brain;

enum State {S_X, S_AMPA, S_NMDA, S_GABA, S_V, S_ISyn, S_ISynBG, S_ISynRecAMPA, S_ISynRecNMDA, S_ISynRecGABA, S_ISynPoolSum, S_ISynInput,S_ISynInputPoolSum, S_ISynBGPoolSum, S_ISynRecAMPASum, S_ISynRecNMDASum, S_ISynRecGABASum, S_SInputSum, S_SBGSum, S_GABA_pooled,S_AMPA_pooled};

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
