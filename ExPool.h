#ifndef EXPOOL_H
#define EXPOOL_H

#include <valarray>
#include "BGPool.h"

using namespace std;

class ExPool 
{
private:
	int i;
	
	float tau_AMPA_Inv_times_dt;
	float tau_AMPA_rise_Inv_times_dt;
	float one_minus_tau_NMDA_Inv_times_dt;
	float alpha_times_dt;
	
	float dt_times_gL_E_over_cm_E; // TODO: make sure you change constants for inh pool here!
	float STmp;
	
	valarray<float> *unitVector;
	valarray<float> *VTmp;
	valarray<bool> *thresholdTest;
	valarray<int> *inds;
	valarray<int> *spiked;
	


	valarray<float> *X;
	
	vector< valarray<float>* > *BG_Inputs_AMPA;
	vector<float*> *Ex_Inputs_AMPA;
	vector<float> *Ex_Inputs_AMPA_w;
	vector<float*> *Ex_Inputs_NMDA;
	vector<float> *Ex_Inputs_NMDA_w;
	vector<float*> *Inh_Inputs_GABA;
	
public:
	
	valarray<float> *V;
	valarray<float> *AMPA;
	valarray<float> *NMDA;
	valarray<float> *ISyn;
	
	string poolName;
	int totalNeurons;
	bool recordSpikes;
	
	vector<int> *spikeRecord_n;
	vector<float> *spikeRecord_t;
	
	float AMPA_pooled;
	float NMDA_pooled;
	
	ExPool(string, int, bool);
	~ExPool();
	
	void init();
	void propogate();
	void connectTo(BGPool *BGPool_in);
	void connectTo(ExPool *ExPool_in, float w);
//	void connectTo(InhPool *InhPool_in); TODO:
	float getFR();
	void writeSpikes(string UUID_string);
};

#endif
