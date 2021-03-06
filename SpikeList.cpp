
#include "SpikeList.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

SpikeList::SpikeList()
{ 
	construct("");
};

SpikeList::SpikeList(string prefix)
{
	construct(prefix);
};

SpikeList::SpikeList(string fileName, string prefix)
{
	construct(prefix);
	
	// Load the data:
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	ifstream inputFile(charFileName, ios::in);
	vector<string> SplitResult1;
	vector<string> SplitResult2;
	string inputLine;
	while (getline(inputFile, inputLine))
	{
		boost::algorithm::split(SplitResult1, inputLine,boost::is_any_of("\t"));
//		boost::algorithm::split(SplitResult2, SplitResult1[0],boost::is_any_of("_"));
		
//		cout << inputLine << endl;
//		cout << SplitResult1[0] << endl;
//		cout << SplitResult1[1] << endl;
		
		
		n->push_back(boost::lexical_cast<int>(SplitResult1[0]));
		t->push_back(boost::lexical_cast<double>(SplitResult1[1]));	
	}
	inputFile.close();
};

void SpikeList::construct(string prefix_in)
{
	prefix = prefix_in;
	n = new vector<int>;
	t = new vector<double>;
	
	nBegin = n->begin();
	tBegin = t->begin();
	tBeginR = t->rbegin();
	
};

SpikeList::~SpikeList() 
{
	delete n;
	delete t;
};

void SpikeList::addSpike(int n_in, double t_in) 
{
	n->push_back(n_in);
	t->push_back(t_in);
};

void SpikeList::addSpike(int n_in, double t_in, int position) 
{
	nBegin = n->begin();
	tBegin = t->begin();
	
	
	n->insert(nBegin + position, n_in);
	t->insert(tBegin + position, t_in);

};

int SpikeList::insertionPositionFromEnd(double t_in)
{
	counter = t->size() + 1;
//	cout << counter << " (size)" << endl;
	for (tBeginR=t->rbegin(); tBeginR < t->rend(); tBeginR++)
	{
		counter--;
		if (*tBeginR < t_in)
		{	
			return counter;
		};
	
	};
	
	return 0;
};

void SpikeList::addSpikeSorted(int n_in, double t_in) 
{
	whereToInsert = insertionPositionFromEnd(t_in);
	addSpike(n_in, t_in, whereToInsert);
};

int SpikeList::size() 
{
	return n->size();
};

string SpikeList::getSpikeString(int whichSpike) 
{
	stringstream out;
	
	string prefixPlus;
	if (prefix == "")
	{
		prefixPlus = this->prefix;
	}
	else
	{
		prefixPlus = this->prefix + "_";
	}
	
	out << (*n)[whichSpike] << "\t" << (*t)[whichSpike] << endl;
	
	return out.str();
};

void SpikeList::toFile(string fileNamePrefix) 
{
	// Set up file:
	string fileTypeExtension = ".ntf";
	string fileName = fileNamePrefix + fileTypeExtension;
	ofstream myfile;
	char *charFileName=new char[fileName.size()+1];
	charFileName[fileName.size()]=0;
	memcpy(charFileName,fileName.c_str(),fileName.size());
	myfile.open(charFileName);
	
	// Write the spikes in "name-time format":
	for (int i = 0; i <= t->size()-1; i++)
	{
//		myfile << fileNamePrefix << "_" << (*n)[i] << "\t" << (*t)[i] << endl;
		myfile << (*n)[i] << "\t" << (*t)[i] << endl;
	}
	
	myfile.close();
	return;
};

void SpikeList::print() 
{
	for (int i = 0; i < this->size(); i++)
	{
		cout << this->getSpikeString(i);
	}
};

bool SpikeList::isSorted()
{
	bool resultVal = 1;
	
	for (tBegin=t->begin()+1; tBegin < t->end(); tBegin++)
	{
		if ((*tBegin < *(tBegin-1))) 
		{
			resultVal = 0;
		}
	};
	
	return resultVal;
}
