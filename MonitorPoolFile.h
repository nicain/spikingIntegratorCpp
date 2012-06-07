#ifndef MONITORPOOLFILE_H
#define MONITORPOOLFILE_H

#include "MonitorPool.h"
#include "Monitor.h"
#include <string>
#include <fstream>
 
class Brain;

class MonitorPoolFile: public MonitorPool
{	
public:
	
	// Constructor+Destructor:
	MonitorPoolFile(Brain&, Pool&, State, string);
	MonitorPoolFile(Brain&, Pool&, State);
	void construct(Brain&, Pool&, State, string);
	~MonitorPoolFile();

	// Member data:
	string fileName;
	ofstream outputFile;
	
	// Member functions:
	void init();
	void propogate();
	void close();

};

#endif
