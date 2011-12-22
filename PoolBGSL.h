#ifndef POOLBGSL_H
#define POOLBGSL_H

#include "PoolBG.h"

using namespace std;

class SpikeList;

class PoolBGSL: public PoolBG
{	
  public:
	
	int currInd;
	double deltaT;
	
	// Constructor+Destructor:
	PoolBGSL(string, Brain&, SpikeList&);
	~PoolBGSL();
	
	// Member functions:
	void init();
	void propogate();
};

#endif
