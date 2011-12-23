#ifndef POOLBGINHPOISSON_H
#define POOLBGINHPOISSON_H

#include "PoolBG.h"
#include <boost/random.hpp>

using boost::random::binomial_distribution;
using boost::random::normal_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolBGInHPoisson: public PoolBG
{	
	
	static const double sigma = 4;
  public:
	
	// Constructor+Destructor:
	PoolBGInHPoisson(string, Brain&, int, bool, double, double, double, double, double);
	void construct(double, double, double, double, double);
	~PoolBGInHPoisson();
	
	// Member data:
	double mu;
	double rho;
	double Corr;
	double tOn;
	double tOff;

	// Stuff needed for computations:
	valarray<double> *gamma;
	int i;	
//	double gamma;
//	double Corr_pooled;
//	int* randArray;
//	int ind2Swap;
//	double masterTrain;
//	int whoSpiked;
//	int numSpikesInCorrPool;

	
	// Dists for RNG:
	normal_distribution<double> *normDist;
	variate_generator<mt19937&,normal_distribution<> > *normRnd;
//	binomial_distribution<> *binomDist;
//	exponential_distribution<double> *expDist;
//	uniform_real_distribution<double> *uniDist;
//	variate_generator<mt19937&,binomial_distribution<> > *binomRnd;
//	variate_generator<mt19937&,exponential_distribution<double> > *expRnd;
//	variate_generator<mt19937&,uniform_real_distribution<double> > *uniRnd;
	
	
	// Member functions:
	void init();
	void propogate();
//	void setCorrPooled(double p);
	void setGamma();
};

#endif
