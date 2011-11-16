#ifndef POOLRECEX_H
#define POOLRECEX_H

#include <valarray>
#include "PoolRec.h"
#include "SpikeList.h"

using namespace std;

class PoolRecEx: public PoolRec
{	
	
  public:
	
//	static const float aPre = 4;	
	
	static const float tau_AMPA = 2;
	static const float tau_AMPA_rise = 2;
	static const float tau_NMDA = 100;
	static float tau_AMPA_Inv_times_dt;
	static float tau_AMPA_rise_Inv_times_dt;
	static float one_minus_tau_NMDA_Inv_times_dt;
	static float alpha_times_dt;
	
	static const float gext_AMPA_E = .000000001*2.1;
	static const float grec_AMPA_E = .000000001*.05;
	static const float gNMDA_E = .000000001*.165;
	static const float gGABA_E = .000000001*1.3;
	static const float gl_E = .000000001*25;
	static const float cm_E = .000001*.5;
	static float dt_times_gL_E_over_cm_E;
	static float dt_over_cm_E;
	
	int i;
	
	// Constructor+Destructor:
	PoolRecEx(string, Brain&, int);
	PoolRecEx(string, Brain&, int, bool);
	void construct();
	~PoolRecEx();

	// Member data:
	valarray<float> *AMPA;
	valarray<float> *NMDA;
	valarray<float> *X;
	float AMPA_pooled;
	float NMDA_pooled;
	
	// Member functions:
	void propogate();
	void updateS();

};

#endif
