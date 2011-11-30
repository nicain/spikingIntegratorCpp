#ifndef BRAIN_H
#define BRAIN_H

/* TODO:
	1) Undo commented childPool
*/

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/random.hpp>

using namespace std;
using boost::random::seed_seq;
using boost::uuids::random_generator;
using boost::uuids::uuid;

class Pool;
class Monitor;

class Brain
{	
public:
	
	static const float dt = .0001;
	int i;
	float stopT;
		
	// Constructor+Destructor:
	Brain();
	Brain(string);
	void construct(string);
	~Brain();

	// Member data:
	uuid myUuid;
	string UUID_string;
	seed_seq mySeed;
	boost::random::mt19937 myRNG;
	string poolName;
	float t;
	vector<Pool*> *childPool;
	vector<Monitor*> *monitor;
	
	// Member functions:
	void addPool(Pool &addPool);
	void addMonitor(Monitor &monitorToAdd);
	void init();
	void run(float);
	void spikesToFile(string fileName);
	void close();
	
	static string removeHyphens(boost::uuids::uuid);
};

#endif
