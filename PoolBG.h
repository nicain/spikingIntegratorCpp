#ifndef POOLBG_H
#define POOLBG_H

#include "Pool.h"
#include "Monitor.h"
#include <valarray>

using namespace std;
class PoolBGFile;

class PoolBG: public Pool
{	
	
	static const float tau_AMPA = 2;
	static float tau_AMPA_Inv_times_dt;

	
  public:
	
	// Constructor+Destructor:
	PoolBG(string, Brain&, int);
	PoolBG(string, Brain&, int, bool);
	void construct();
	~PoolBG();

	// Member data:
	valarray<float> *AMPA;
	
	// Member functions:
	void updateS();
	float* getStateLocation(int, State);
	
	// Friends:
	friend class PoolBGFile;
};



#endif


//class test
//{
//	static const char arr[3][4] ;
//public:
//	test(void);
//public:
//	~test(void);
//};
//
//const char test::arr[3][4] = {"a", "bb", "ccc"};