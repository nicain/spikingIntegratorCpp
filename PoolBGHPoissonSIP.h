#ifndef POOLBGHPOISSONSIP_H
#define POOLBGHPOISSONSIP_H

#include "PoolBG.h"
#include <boost/random.hpp>

using boost::random::binomial_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolBGHPoissonSIP: public PoolBG
{	
  public:
	
	// Constructor+Destructor:
	PoolBGHPoissonSIP(string, Brain&, int, bool, double, double, double, double);
	void construct(double, double, double, double);
	~PoolBGHPoissonSIP();
	
	// Member data:
	double FR;
	double Corr;
	double tOn;
	double tOff;

	// Stuff needed for computations:
	double gamma;
	double Corr_pooled;
	int* randArray;
	int ind2Swap;
	double masterTrain;
    double indTrain;
	int whoSpiked;
	int numSpikesInCorrPool;
	int i;
	
	// Dists for RNG:
	exponential_distribution<double> *expDist;
    exponential_distribution<double> *expDistInd;
	uniform_real_distribution<double> *uniDist;
	variate_generator<mt19937&,exponential_distribution<double> > *expRnd;
	variate_generator<mt19937&,exponential_distribution<double> > *expRndInd;
	variate_generator<mt19937&,uniform_real_distribution<double> > *uniRnd;
	
	
	// Member functions:
	void init();
	void propogate();
	double getInputCorrelation(double p, int N);
};

#endif
