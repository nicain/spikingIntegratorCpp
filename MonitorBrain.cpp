#include <iostream>
#include <string>
#include "Brain.h"
#include "MonitorBrain.h"
class Monitor;
 
using namespace std;

MonitorBrain::MonitorBrain(Brain &parentPool_in): Monitor(parentPool_in)
{

};

MonitorBrain::~MonitorBrain() 
{

};

string MonitorBrain::getString() 
{
	
	outString = str( boost::format("%9.4f") % parentBrain->t );
	return outString;
};

void MonitorBrain::propogate() 
{
	cout << getString() << endl;
};


