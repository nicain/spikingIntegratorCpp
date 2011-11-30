#include <iostream>
#include <sstream>
#include "Brain.h"
#include "Pool.h"
#include "MonitorNeuronFile.h"
 
class Monitor;

using namespace std;

MonitorNeuronFile::MonitorNeuronFile(Brain &parentPool_in, Pool &whichPool_in, int whichNeuron_in, State whichState_in, string fileName_in): MonitorNeuron(parentPool_in, whichPool_in, whichNeuron_in, whichState_in)
{
	construct(parentPool_in, whichPool_in, whichNeuron_in, whichState_in, fileName_in);
}; 

MonitorNeuronFile::MonitorNeuronFile(Brain &parentPool_in, Pool &whichPool_in, int whichNeuron_in, State whichState_in): MonitorNeuron(parentPool_in, whichPool_in, whichNeuron_in, whichState_in)
{
	stringstream out;
	out << whichPool_in.poolName << "_" << whichNeuron_in;
	construct(parentPool_in, whichPool_in, whichNeuron_in, whichState_in, out.str());
};

void MonitorNeuronFile::construct(Brain &parentPool_in, Pool &whichPool_in, int whichNeuron_in, State whichState_in, string fileName_in)
{
	string fileTypeExtension = ".dat";
	fileName = fileName_in + fileTypeExtension;
}

MonitorNeuronFile::~MonitorNeuronFile() 
{

};

void MonitorNeuronFile::init() 
{
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	outputFile.open(charFileName);
};

void MonitorNeuronFile::propogate() 
{
	outputFile << getString() << endl;
};

void MonitorNeuronFile::close() 
{
	outputFile.close();
};

	