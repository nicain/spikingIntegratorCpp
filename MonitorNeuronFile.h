#ifndef MONITORNEURONFILE_H
#define MONITORNEURONFILE_H

#include "MonitorNeuron.h"
#include "Monitor.h"
#include <string>
#include <fstream>

class Brain;

class MonitorNeuronFile: public MonitorNeuron
{	
public:
	
	// Constructor+Destructor:
	MonitorNeuronFile(Brain&, Pool&, int, State, string);
	MonitorNeuronFile(Brain&, Pool&, int, State);
	void construct(Brain&, Pool&, int, State, string);
	~MonitorNeuronFile();

	// Member data:
	string fileName;
	ofstream outputFile;
	
	// Member functions:
	void init();
	void propogate();
	void close();

};

#endif
