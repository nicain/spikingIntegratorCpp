#ifndef POOLREC_H
#define POOLREC_H

#include <valarray>
#include <boost/random.hpp>
#include "Pool.h"

using namespace std;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

class PoolBG;
class PoolRecEx;
class PoolRecInh;

class PoolRec: public Pool
{	
	
  public:
	
	// Used in updateV:
	static const float VE = 0;
	static const float VI = -70;
	static const float VMin = -70;
	static const float VMax = -50;
	static const float VReset = -55;
	static const float alpha = .5;
	static const float K = -.062;
	float gext_AMPA;
	float grec_AMPA;
	float gNMDA;
	float gGABA;
	float dt_times_gL_over_cm;
	float dt_over_cm;
	
	// Small stuff:
	int i;
	float STmp;
	
	// Constructor+Destructor:
	PoolRec(string, Brain&, int);
	PoolRec(string, Brain&, int, bool);
	void construct();
	~PoolRec();
	
	// RNG:
	uniform_real_distribution<float> *uniDist;
	variate_generator<mt19937&,uniform_real_distribution<float> > *uniRnd;

	// Member data:
	valarray<float> *V;
	valarray<float> *ISyn;
	vector< valarray<float>* > *BG_Inputs_AMPA;
	vector<float*> *Ex_Inputs_AMPA;
	vector<float> *Ex_Inputs_AMPA_w;
	vector<float*> *Ex_Inputs_NMDA;
	vector<float> *Ex_Inputs_NMDA_w;
	vector<float*> *Inh_Inputs_GABA;
	
	// Helper vectors
	valarray<float> *unitVector;
	valarray<float> *VTmp;
	valarray<bool> *thresholdTest;
	
	// Member functions:
	void connectTo(PoolBG &BGPool_in);
	void connectTo(PoolRecEx &BGPool_in, float wIn);
	void connectTo(PoolRecInh &BGPool_in);
	void init();	
	void updateV();
	

};

#endif
