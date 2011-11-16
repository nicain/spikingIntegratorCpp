#ifndef POOLRECINH_H
#define POOLRECINH_H

#include <valarray>
#include "PoolRec.h"
#include "SpikeList.h"

using namespace std;

class PoolRecInh: public PoolRec
{	
	
  public:
	
	static const float tau_GABA = 5;
	static float tau_GABA_Inv_times_dt;
	static const float gext_AMPA_I = .000000001*1.62;
	static const float grec_AMPA_I = .000000001*.04;
	static const float gNMDA_I = .000000001*.13;
	static const float gGABA_I = .000000001*1.0;
	static const float gl_I = .000000001*20;
	static const float cm_I = .000001*.2;
	static float dt_times_gL_I_over_cm_I;
	static float dt_over_cm_I;
	
	int i;
	
	// Constructor+Destructor:
	PoolRecInh(string, Brain&, int);
	PoolRecInh(string, Brain&, int, bool);
	void construct();
	~PoolRecInh();

	// Member data:
	valarray<float> *GABA;
	float GABA_pooled;
	
	// Member functions:
	void propogate();
	void updateS();

};

#endif
