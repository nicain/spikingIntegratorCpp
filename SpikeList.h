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
	
		// Iterators:
		int counter;
		int whereToInsert;
		vector<int>::iterator nBegin;
		vector<double>::iterator tBegin;
		vector<double>::reverse_iterator tBeginR;
		
		// Member functions:
		void construct(string);
		void addSpike(int n_in, double t_in);
		void addSpike(int n_in, double t_in, int position);
		void addSpikeSorted(int n_in, double t_in);
		int insertionPositionFromEnd(double t_in);
		int size();
		string getSpikeString(int whichSpike);
		void toFile(string UUID_string);
		void print();
		bool isSorted();
};

#endif
	