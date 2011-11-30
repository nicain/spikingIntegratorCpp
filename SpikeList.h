#ifndef SPIKELIST_H
#define SPIKELIST_H

#include <vector>
#include <string>

using namespace std;

class SpikeList
{	 
	public:
		
		// Constructor+Destructor:
		SpikeList();
		SpikeList(string);
		SpikeList(string, string);
		~SpikeList();

		// Member data:
		string prefix;
		vector<int> *n;
		vector<double> *t;
		
		// Member functions:
		void construct(string);
		void addSpike(int n_in, double t_in);
		int size();
		string getSpikeString(int whichSpike);
		void toFile(string UUID_string);
		void print();
};

#endif
	