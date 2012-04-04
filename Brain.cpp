/* TODO:
 1) Undo commented destructor 
 2) Undo commented childPool
 */

#include "Brain.h"
#include "Pool.h" 
#include "Monitor.h"

using namespace std;
using boost::uuids::uuid;
const double Brain::dt;

Brain::Brain()
{
	// Initialize with noname:
	construct("");	
} 

Brain::Brain(string poolName_in)
{
	// Initialize with given name:
	construct(poolName_in);
}

string Brain::removeHyphens(boost::uuids::uuid uuid)
{
	string s1 = boost::uuids::to_string(uuid);
	s1.erase(s1.begin()+8, s1.begin()+9);
	s1.erase(s1.begin()+12, s1.begin()+13);
	s1.erase(s1.begin()+16, s1.begin()+17);
	s1.erase(s1.begin()+20, s1.begin()+21);
	return s1;
}

void Brain::construct(string poolName_in)
{    
    // Set the name:
	poolName = poolName_in;
	
	// Create UUID as string:
	myUuid = random_generator()();
	UUID_string = removeHyphens(myUuid);
	
	// Random number generators:
	mySeed = seed_seq(UUID_string); 
//	myRNG.seed(1);
	myRNG.seed(mySeed);
	
	// Initialize as empty:
	childPool = new vector<Pool*>;
	monitor = new vector<Monitor*>;
};

Brain::~Brain() 
{
	delete childPool;
};

void Brain::addPool(Pool &poolToAdd)
{

	childPool->push_back(&poolToAdd);
}

void Brain::addMonitor(Monitor &monitorToAdd)
{
	
	monitor->push_back(&monitorToAdd);
}

void Brain::init()
{
	t=0;
	for (i = 0; i < childPool->size(); i++)
	{
			
		(*childPool)[i]->init();
		
	};

	for (i = 0; i < monitor->size(); i++)
	{
		(*monitor)[i]->init();
	};
}

void Brain::run(double deltaT) 
{
	 //Important: This scheme does synchronous updating
	stopT = t + deltaT;
	
	while (t < stopT)
	{
		
		// Update monitors:
		for (i = 0; i < monitor->size(); i++)
		{
			(*monitor)[i]->propogate();
		};
		
		// Figure out who all spiked, and bump their state vars:
		for (i = 0; i < childPool->size(); i++)
		{
			(*childPool)[i]->propogate();
		};
		
		// Apply the DE's for the state vars
		for (i = 0; i < childPool->size(); i++)
		{
			(*childPool)[i]->updateS();
		};
		
		// Update the voltages of the conductance pops:
		for (i = 0; i < childPool->size(); i++)
		{
			(*childPool)[i]->updateV();
		};

		// Increment time:
		t += dt;
		

	};
	
};

void Brain::spikesToFile()
{
	spikesToFile(this->UUID_string + "_" + this->poolName);
}

void Brain::spikesToFile(string inputString)
{
	// Print out child pool stuff
	for (i = 0; i < childPool->size(); i++)
	{
		if ((*childPool)[i]->recordSpikes) {
			(*((*childPool)[i])).toFile(inputString);
		}
	};
}

void Brain::close()
{
	// Close out monitors:
	for (i = 0; i < monitor->size(); i++)
	{
		(*monitor)[i]->close();
	};
}


