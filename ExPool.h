#ifndef EXPOOL_H
#define EXPOOL_H

#include <valarray>

using namespace std;

class ExPool 
{
private:
	int i;
	
public:
	string poolName;
	int totalNeurons;
	bool recordSpikes;
	
	vector<int> *spikeRecord_n;
	vector<float> *spikeRecord_t;
	
	ExPool(string, int, bool);
	~ExPool();
	
	void init();
	void propogate();
	float getFR();
	void writeSpikes(string UUID_string);
};

#endif
