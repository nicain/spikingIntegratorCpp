
#include "Pool.h"
//#include <boost/random.hpp>

using boost::random::binomial_distribution;
using boost::random::exponential_distribution;
using boost::random::uniform_real_distribution;
using boost::random::variate_generator;
using boost::random::mt19937;

using namespace std;

class PoolPoisson: public Pool
{	
  public:
	
	// Constructor+Destructor:
	PoolPoisson(string, Brain&, int, bool, double, double, double, double,double);
	void construct(Brain&,double, double, double, double,double);
	~PoolPoisson();
	
	// Member data:
	double tau;
	double FR;
	double Corr;
	double tOn;
	double tOff;
	double tmax;	
	double dt;
	double t;
	int Nn;
	int T;
	double deltaT;
	
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


