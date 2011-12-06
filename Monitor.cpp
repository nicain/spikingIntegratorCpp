#include "Brain.h"
#include "Monitor.h"

using namespace std;

Monitor::Monitor(Brain &parentPool_in)
{
	parentBrain = &parentPool_in;
	parentPool_in.addMonitor(*this);
};

Monitor::~Monitor() 
{
 
};

void Monitor::init() 
{

};

void Monitor::propogate() 
{

};

void Monitor::close() 
{
	
};

