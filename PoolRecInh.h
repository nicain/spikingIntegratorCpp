#ifndef POOLRECINH_H
#define POOLRECINH_H

#include <valarray>
#include "PoolRec.h"
#include "SpikeList.h"

using namespace std;

class PoolRecInh: public PoolRec
{	
	
  public:
	
	static const double tau_GABA = 5;
	static double tau_GABA_Inv_times_dt;
	static const double gext_AMPA_I = .000000001*1.62;
	static const double grec_AMPA_I = .000000001*.04;
	static const double gNMDA_I = .000000001*.13;
	static const double gGABA_I = .000000001*1.0;
	static const double gl_I = .000000001*20;
	static const double cm_I = .000001*.2;
	static double dt_times_gL_I_over_cm_I;
	static double dt_over_cm_I;
	
	int i;
	
	// Constructor+Destructor:
	PoolRecInh(string, Brain&, int);
	PoolRecInh(string, Brain&, int, bool);
	void construct();
	~PoolRecInh();

	// Member data:
	valarray<double> *GABA;
//	double GABA_pooled;
	
	// Member functions:
	void propogate();
	void updateS();

};

#endif
