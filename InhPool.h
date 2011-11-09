#ifndef INHPOOL_H
#define INHPOOL_H

#include <valarray>
#include <boost/random.hpp>

using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

class BGPool;
class ExPool;

using namespace std;

class InhPool 
{
private:
	int i;
	
	float tau_GABA_Inv_times_dt;
	float dt_over_cm_I;
	
	float dt_times_gL_I_over_cm_I; // TODO: make sure you change constants for inh pool here!
	float STmp;
	
	valarray<float> *unitVector;
	valarray<float> *VTmp;
	valarray<bool> *thresholdTest;
	
	uniform_real_distribution<float> *uniDist;
	variate_generator<mt19937&,uniform_real_distribution<float> > *uniRnd;
	
	vector< valarray<float>* > *BG_Inputs_AMPA;
	vector<float*> *Ex_Inputs_AMPA;
	vector<float> *Ex_Inputs_AMPA_w;
	vector<float*> *Ex_Inputs_NMDA;
	vector<float> *Ex_Inputs_NMDA_w;
	vector<float*> *Inh_Inputs_GABA;
	
public:
	
	valarray<float> *V; 
	valarray<float> *GABA;
	valarray<float> *ISyn;
	
	string poolName;
	int totalNeurons;
	bool recordSpikes;
	
	vector<int> *spikeRecord_n;
	vector<float> *spikeRecord_t;
	
	float GABA_pooled;
	
	InhPool(string, int, bool);
	~InhPool();
	
	void init();
	void propogate();
	void connectTo(BGPool *BGPool_in);
	void connectTo(ExPool *ExPool_in, float w);
	void connectTo(InhPool *InhPool_in);
	float getFR();
	void writeSpikes(string UUID_string);
};

#endif
