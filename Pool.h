#ifndef POOL_H
#define POOL_H

/* TODO:
 1) Undo commented addConnection
 */

class Brain;
class SpikeList;	

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
	
	// Member functions:
	virtual void init();
	virtual void propogate();
	virtual void updateS();
	virtual void updateV();
	void print();
	void toFile(string appendString);

};

#endif
