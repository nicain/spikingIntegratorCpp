//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

#include <time.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include "Brain.h"
#include "PoolODE.h"
#include "Monitor.h"
#include <iostream>
#include <fstream>

// Add variable names to scope:
using namespace std;
using boost::random::seed_seq;
using boost::uuids::random_generator;
using boost::uuids::uuid;
using boost::lexical_cast;

time_t t1;
time_t t2;

int main( int argc,      // Number of strings in array argv
		 char *argv[])
{	
	
	if(argc < 11)
	{
		cout << "Not enough inputs. Input format: Coh tOn tOff tMax Corr saveRes recBGspks recINspks I0 w N runs" << endl;
		return 0;
	}
	
	
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	// Params passed in args:
	const double Coh = atof(argv[1]);
	const double tOn = atof(argv[2]);
	const double tOff = atof(argv[3]);
	const double tMax = atof(argv[4]);
	const double inputCorrelation = atof(argv[5]);
	const bool saveResults = lexical_cast<bool>(argv[6]);
	const bool recordBGSpikes = lexical_cast<bool>(argv[7]);
	const bool recordInputSpikes = lexical_cast<bool>(argv[8]);
	const double I0 = atof(argv[9]);
	const double w = atof(argv[10]);
	const int N = atof(argv[11]);
	const int runs = atof(argv[12]);
//	
	// Network dimension settings:
	const double BgFR = 2400; // background noise firing rate
	const double InFR1 = 40 + .4*Coh; // "correct choice" input firing rate
	const double InFR2 = 40 - .4*Coh; // "false choice" input firing rate
	
	// variables for thresholding
	double Th_start = 0.1;
	double Th_max = 0.4;
	double Th_step = 0.01;
	int L = 1 + (Th_max-Th_start)/Th_step;
	int count;
	int Thi;
	bool f;
	double acc[L];
	double hits[L];
	for(int i=0;i < L ; i++) hits[i] = 0;
	double misses[L];
	for(int i=0;i < L ; i++) misses[i] = 0;
	double times[L][runs];
	double times_mean[L];
					
	// Main network:
	Brain Network;
	
	// Output UUID, so that FR can be automatically computed
	cout << Network.UUID_string << endl;
	
	// Poisson population
	PoolPoisson BG1("BG1",Network,N,0,BgFR*N,0,0,tOff,tMax);
	PoolPoisson BG2("BG2",Network,N,0,BgFR*N,0,0,tOff,tMax);
	PoolPoisson In1("In1",Network,N,0,InFR1*N,0,tOn,tOff,tMax);
	PoolPoisson In2("In2",Network,N,0,InFR2*N,0,tOn,tOff,tMax);
	
	// ODE system
	PoolODE ODE("ODE",Network,w,tMax,270,108,0.154,0.2609,-0.0497,0.1,0.641,I0);

	for(int j = 0; j < runs ; j++)
	{		
		cout << "Run " << j << endl;
		Network.init();	
		ODE.run(In1,BG1,In2,BG2);

//	int L = ODE.S1.size();	
//	cout << "I0 = " << I0 << ": "<<  ODE.S1[L-10] << " " << ODE.S2[L-10] << endl;
		Thi = 0;
		for(double Th = Th_start; Th <= Th_max ; Th = Th + Th_step)
		{
			f = true;
			count = 0;
			while(f)
			{
				if(ODE.S1[count] >= Th)
				{
					hits[Thi]++;
					times[Thi][j] = count;
					f = false;
				}
				if(ODE.S2[count] >= Th)
				{
					misses[Thi]++;
					times[Thi][j] = count;
					f = false;
				}
				count++;
			}
			Thi++;
		}
	
	}
				
				
				ofstream myfile;
				myfile.open("data.txt");	
	

				for(int i=0;i < L ; i++)
				{
				acc[i] = hits[i]/(hits[i]+misses[i]);
				times_mean[i] = 0;
				for(int j = 0;j < runs ; j++) times_mean[i] = times_mean[i] + times[i][j]*0.00001/runs;
				
				cout << acc[i] << " " << times_mean[i] << endl; 
				
				myfile << times_mean[i] << " " << acc[i] << endl;
				
				}
				myfile.close();
			
				
				
				
	cout << "finished" << endl;
	return 0;
}
