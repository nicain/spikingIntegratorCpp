#ifndef BGPOOL_H
#define BGPOOL_H

#include <valarray>
#include <boost/random.hpp>

using boost::random::binomial_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class BGPool 
{
private:
	int whoSpiked;
	float tau_AMPA_Inv_times_dt;
	valarray<float> *tmpAMPA;
	valarray<bool> **masks;
	int i;
	int j;
	float FR;
	float Corr;
	float Corr_pooled;
	float tOn;
	float tOff;
	int numSpikesInCorrPool;
	int ind2Swap;
	int* randArray;
	float gamma;
	
	float getInputCorrelation(float p, int N);
	
	binomial_distribution<> *binomDist;
	exponential_distribution<float> *expDist;
	uniform_real_distribution<float> *uniDist;
	variate_generator<mt19937&,binomial_distribution<> > *binomRnd;
	variate_generator<mt19937&,exponential_distribution<float> > *expRnd;
	variate_generator<mt19937&,uniform_real_distribution<float> > *uniRnd;
	
public:
	string poolName;
	float masterTrain;
	int totalNeurons;
	int neuronsPerSubPool;
	int numberSubPools;
	bool recordSpikes;
	
	//TODO: move some of this stuff to private
	
	valarray<float> *AMPA;
	vector<int> *spikeRecord_n;
	vector<float> *spikeRecord_t;
	valarray<float> *AMPA_pooled;
	
	BGPool(string, int, int, float, float, bool, float, float);
	~BGPool();
	
	void init();
	void propogate();
	float getFR();
	void writeSpikes(string UUID_string);
};

#endif
