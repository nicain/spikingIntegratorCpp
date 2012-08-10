#ifndef POOLRECHYBRID_H
#define POOLRECHYBRID_H

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

class PoolRecHybrid: public Pool
{	
	
  public:
	
	// Used in updateV:
	static const double VE = 0;
	static const double VI = -70;
    static const double Vm = -52.5;
	static const double VMin = -70;
	static const double VMax = -50;
	static const double VReset = -55;
	static const double alpha = .5;
	static const double K = -.062;
    
    static const double xVals[100];
    static const double yVals[100];
    
	double gext_AMPA;
	double grec_AMPA;
	double gNMDA;
	double gGABA;
	double dt_times_gL_over_cm;
	double dt_over_cm;
    static double tau_GABA_Inv_times_dt;
	
	// Small stuff:
	int i;
	double STmp;
	
	// Constructor+Destructor:
	PoolRecHybrid(string, Brain&, int);
	PoolRecHybrid(string, Brain&, int, bool);
	void construct();
	~PoolRecHybrid();
	
	// RNG:
//	uniform_real_distribution<double> *uniDist;
//	variate_generator<mt19937&,uniform_real_distribution<double> > *uniRnd;

	// Member data:
//	valarray<double> *V;
//	valarray<double> *ISyn;
	vector< valarray<double>* > *BG_Inputs_AMPA;
	vector<double*> *Ex_Inputs_AMPA;
	vector<double> *Ex_Inputs_AMPA_w;
	vector<double*> *Ex_Inputs_NMDA;
	vector<double> *Ex_Inputs_NMDA_w;
	vector<double*> *Inh_Inputs_GABA;
    
    // Mean Voltages for current calculation:
    double VAvgE; 
    double VAvgNMDA;
    double VAvgI; 

    
    // Avg Total current flowing into the pop, that will be pushed through phi
    double ITot;
    
    double GABA_pooled;
    
    
//    double xVals[100];
//    double yVals[100];
    double xMin;
    double xDelta;
    
//    double ISynPoolSum;
//    double ISynInputPoolSum;
//    double ISynBGPoolSum;
//    double ISynRecAMPASum;
//    double ISynRecNMDASum;
//    double ISynRecGABASum;
//    double SInputSum;
//    double SBGSum;
    
//    // Currents to record:
//    valarray<double> *ISynBG;
//    valarray<double> *ISynInput;
//    valarray<double> *ISynRecAMPA;
//    valarray<double> *ISynRecNMDA;
//    valarray<double> *ISynRecGABA;
    
//    // Synapse variables to record:
//    valarray<double> *SInput;
//    valarray<double> *SBG;
	
//	// Helper vectors
//	valarray<double> *unitVector;
//	valarray<double> *VTmp;
//	valarray<bool> *thresholdTest;
	
	// Member functions:
	void connectTo(PoolBG &BGPool_in);
	void connectTo(PoolRecEx &BGPool_in, double wIn);
	void connectTo(PoolRecInh &BGPool_in);
	void connectTo(PoolRecHybrid &BGPool_in);
	void init();	
	void updateV();
	void propogate();
	void updateS();
    
//	double* getStateLocation(int, State);
//	double* getStateLocation(State);
//	virtual double* getStateLocationConductance(int, State);
	

};

#endif
