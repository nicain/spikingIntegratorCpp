#ifndef POOLBGINHPOISSON_H
#define POOLBGINHPOISSON_H

#include "PoolBG.h"
#include <boost/random.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using boost::random::binomial_distribution;
using boost::random::normal_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolBGInHPoisson: public PoolBG
{	
	
	static const double sigma = .001*4; //4
	static const double resetGammaDeltaT = 50;
	
	boost::numeric::ublas::identity_matrix<double> *IdM;
	boost::numeric::ublas::scalar_matrix<double> *OnesM;
	
  public:
	
	// Constructor+Destructor:
	PoolBGInHPoisson(string, Brain&, int, bool, double, double, double, double);
	void construct(double, double, double, double);
	~PoolBGInHPoisson();
	
	// Member data:
	double mu;
	double rho;
	double tOn;
	double tOff;

	// Stuff needed for computations:
	boost::numeric::ublas::vector<double> *gamma;
	boost::numeric::ublas::vector<double> *muV;
	boost::numeric::ublas::matrix<double> *S;
	valarray<double> *masterTrain;
	int i;
	double nextGammaResetT;
	
	// Dists for RNG:
	normal_distribution<double> *normDist;
	variate_generator<mt19937&,normal_distribution<> > *normRnd;
	exponential_distribution<double> *expDist;
	variate_generator<mt19937&,exponential_distribution<double> > *expRnd;

	// Member functions:
	void init();
	void propogate();
	void setGamma();
};

#endif
