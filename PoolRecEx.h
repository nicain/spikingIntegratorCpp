#ifndef POOLRECEX_H
#define POOLRECEX_H

#include <valarray>
#include "PoolRec.h"
#include "SpikeList.h"
#include "Monitor.h"

using namespace std;

class PoolRecEx: public PoolRec
{	
	
  public:
	
//	static const double aPre = 4;	
	
	static const double tau_AMPA = 2;
	static const double tau_AMPA_rise = 2;
	static const double tau_NMDA = 100;
	static double tau_AMPA_Inv_times_dt;
	static double tau_AMPA_rise_Inv_times_dt;
	static double one_minus_tau_NMDA_Inv_times_dt;
	static double alpha_times_dt;
	
	static const double gext_AMPA_E = .000000001*2.1;
	static const double grec_AMPA_E = .000000001*.05;
	static const double gNMDA_E = .000000001*.165;
	static const double gGABA_E = .000000001*1.3;
	static const double gl_E = .000000001*25;
	static const double cm_E = .000001*.5;
	static double dt_times_gL_E_over_cm_E;
	static double dt_over_cm_E;
	
	int i;
	
	// Constructor+Destructor:
	PoolRecEx(string, Brain&, int);
	PoolRecEx(string, Brain&, int, bool);
	void construct();
	~PoolRecEx();

	// Member data:
	valarray<double> *AMPA;
	valarray<double> *NMDA;
	valarray<double> *X;
	double AMPA_pooled;
	double NMDA_pooled;
	double X_pooled;
	
	// Member functions:
	void propogate();
	void updateS();
	double* getStateLocationConductance(int, State);
    double* getStateLocation(State);

};

#endif
