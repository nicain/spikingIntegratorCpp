#include <iostream>
#include <sstream>
#include "Brain.h"
#include "Pool.h"
#include "MonitorPoolFile.h"
 
class Monitor;

using namespace std;

MonitorPoolFile::MonitorPoolFile(Brain &parentPool_in, Pool &whichPool_in, State whichState_in, string fileName_in): MonitorPool(parentPool_in, whichPool_in, whichState_in)
{
	construct(parentPool_in, whichPool_in, whichState_in, fileName_in);
}; 

MonitorPoolFile::MonitorPoolFile(Brain &parentPool_in, Pool &whichPool_in, State whichState_in): MonitorPool(parentPool_in, whichPool_in, whichState_in)
{
	stringstream out;
	out << whichPool_in.poolName;
	construct(parentPool_in, whichPool_in, whichState_in, out.str());
};

void MonitorPoolFile::construct(Brain &parentPool_in, Pool &whichPool_in, State whichState_in, string fileName_in)
{
	string fileTypeExtension = ".dat";
	fileName = fileName_in + fileTypeExtension;
}

MonitorPoolFile::~MonitorPoolFile() 
{

};

void MonitorPoolFile::init() 
{
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	outputFile.open(charFileName);
};

void MonitorPoolFile::propogate() 
{
	outputFile << getString() << endl;
};

void MonitorPoolFile::close() 
{
	outputFile.close();
};

	