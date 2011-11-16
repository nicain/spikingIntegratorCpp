#ifndef POOLBGFILE_H
#define POOLBGFILE_H

#include "PoolBG.h"

using namespace std;


class PoolBGFile: public PoolBG
{	
  public:
	
	int currInd;
	float deltaT;
	
	// Constructor+Destructor:
	PoolBGFile(string, Brain&, string);
	PoolBGFile(string, Brain&, bool, string);
	void construct(string, string);
	~PoolBGFile();
	
	// Member functions:
	void init();
	void propogate();
};

#endif
