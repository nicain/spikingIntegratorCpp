#ifndef POOLBGOU_H
#define POOLBGOU_H

#include "PoolBG.h"
#include <boost/random.hpp>

using boost::random::normal_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolBGOU: public PoolBG
{	
  public:
	
	// Constructor+Destructor:
	PoolBGOU(string, Brain&, int, bool, double, double, double);
    PoolBGOU(string, Brain&, int, bool, double, double, double, double);
	void constructFR(double, double, double);
    void constructMuSigma(double, double, double, double);
	~PoolBGOU();
	
	// Member data:
	double FR;
	double tOn;
	double tOff;

	// Stuff needed for computations:
	double mu;
	double sigma;
	
	// Dists for RNG:
	normal_distribution<double> *normDist;
	variate_generator<mt19937&,normal_distribution<> > *normRnd;
	
	// vectorized rng:
	valarray<double> *randVec;

	// Member functions:
	void init();
	void updateS();
};

#endif
