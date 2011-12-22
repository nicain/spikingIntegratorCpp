#ifndef POOLBGHPOISSON_H
#define POOLBGHPOISSON_H

#include "PoolBG.h"
#include <boost/random.hpp>

using boost::random::binomial_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolBGHPoisson: public PoolBG
{	
  public:
	
	// Constructor+Destructor:
	PoolBGHPoisson(string, Brain&, int, bool, double, double, double, double);
	void construct(double, double, double, double);
	~PoolBGHPoisson();
	
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
	int whoSpiked;
	int numSpikesInCorrPool;
	int i;
	
	// Dists for RNG:
	binomial_distribution<> *binomDist;
	exponential_distribution<double> *expDist;
	uniform_real_distribution<double> *uniDist;
	variate_generator<mt19937&,binomial_distribution<> > *binomRnd;
	variate_generator<mt19937&,exponential_distribution<double> > *expRnd;
	variate_generator<mt19937&,uniform_real_distribution<double> > *uniRnd;
	
	
	// Member functions:
	void init();
	void propogate();
	double getInputCorrelation(double p, int N);
};

#endif
