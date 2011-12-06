#ifndef POOLBG_H
#define POOLBG_H

#include "Pool.h"
#include "Monitor.h"
#include <valarray>

using namespace std;
class PoolBGFile;

class PoolBG: public Pool
{	
	
	static const double tau_AMPA = 2;
	static double tau_AMPA_Inv_times_dt;

	
  public:
	
	// Constructor+Destructor:
	PoolBG(string, Brain&, int);
	PoolBG(string, Brain&, int, bool);
	void construct();
	~PoolBG();

	// Member data:
	valarray<double> *AMPA;
	
	// Member functions:
	void updateS();
	double* getStateLocation(int, State);
	
	// Friends:
	friend class PoolBGFile;
};

#endif