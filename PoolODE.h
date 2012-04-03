#include "Pool.h"
#include "PoolPoisson.h"

using namespace std;

class PoolODE: public Pool
	{	
	public:
		
		// Constructor+Destructor:
		PoolODE(string,Brain&, double, double, double, double, double, double, double, double,double,double);
		void construct(Brain&, double, double, double, double, double, double, double, double,double,double);
		~PoolODE();
		
		// counters:
		int T;
		
		// constant:
		double w;
		double a;
		double b;
		double c;
		double Jii;
		double Jij;
		double tau;
		double gamma;
		double tmax;	
		double dt;
		double t;
		double I0;
		
		// update arrays
		vector<double> S1;
		vector<double> X1;
		vector<double> S2;
		vector<double> X2;
		
		// Member functions:
		void init();
		void propogate();
		void run(PoolPoisson, PoolPoisson, PoolPoisson, PoolPoisson);
		
	};

	