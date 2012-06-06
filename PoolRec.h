#ifndef POOLREC_H
#define POOLREC_H

#include <valarray>
#include <boost/random.hpp>
#include "Pool.h"
#include "Monitor.h"

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
	static const double VE = 0;
	static const double VI = -70;
	static const double VMin = -70;
	static const double VMax = -50;
	static const double VReset = -55;
	static const double alpha = .5;
	static const double K = -.062;
	double gext_AMPA;
	double grec_AMPA;
	double gNMDA;
	double gGABA;
	double dt_times_gL_over_cm;
	double dt_over_cm;
	
	// For RK time-stepping in V!
	valarray<double> *RK1;
	valarray<double> *RK2;
	valarray<double> *RK3;
	valarray<double> *RK4;
	
	// Small stuff:
	int i;
	double STmp;
	
	// Constructor+Destructor:
	PoolRec(string, Brain&, int);
	PoolRec(string, Brain&, int, bool);
	void construct();
	~PoolRec();
	
	// RNG:
	uniform_real_distribution<double> *uniDist;
	variate_generator<mt19937&,uniform_real_distribution<double> > *uniRnd;

	// Member data:
	valarray<double> *V;
	valarray<double> *ISyn;
	vector< valarray<double>* > *BG_Inputs_AMPA;
	vector<double*> *Ex_Inputs_AMPA;
	vector<double> *Ex_Inputs_AMPA_w;
	vector<double*> *Ex_Inputs_NMDA;
	vector<double> *Ex_Inputs_NMDA_w;
	vector<double*> *Inh_Inputs_GABA;
    
    // Currents to record:
    valarray<double> *ISynBG;
    valarray<double> *ISynRecAMPA;
    valarray<double> *ISynRecNMDA;
    valarray<double> *ISynRecGABA;
	
	// Helper vectors
	valarray<double> *unitVector;
	valarray<double> *VTmp;
	valarray<bool> *thresholdTest;
	
	// Member functions:
	void connectTo(PoolBG &BGPool_in);
	void connectTo(PoolRecEx &BGPool_in, double wIn);
	void connectTo(PoolRecInh &BGPool_in);
	void init();	
	void updateV();
	double* getStateLocation(int, State);
	virtual double* getStateLocationConductance(int, State);
	

};

#endif
