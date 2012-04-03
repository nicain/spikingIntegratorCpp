#ifndef POOL_H
#define POOL_H

/* TODO:
 1) Undo commented addConnection
 */

#include "Monitor.h"

class Brain;
class SpikeList;
#include <boost/random.hpp>


using namespace std;

class Pool
{	
public:

	
	// Constructor+Destructor:
	Pool(string, Brain&, int);
	Pool(string, Brain&, int, bool);
	void construct(string, Brain&, int, bool);
	~Pool();

	// Member data:
	string poolName;
	int N;
	Brain *parentBrain;
	bool recordSpikes;
	SpikeList *spikeList;
	int L;
	vector<double> spks;
	
	// Member functions:
	virtual void init();
	virtual void propogate();
	virtual void updateS();
	virtual void updateV();
	virtual double* getStateLocation(int, State);
	void print();
	void toFile(string appendString);
	void toFileExact(string fileNamePrefix);
	double getFR();
	

	
};

#endif
