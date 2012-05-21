//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator
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


int main( int argc,      // Number of strings in array argv
		 char *argv[])
{	
	
	if(argc < 1)
	{
		cout << "Not enough inputs. Input format: Coh tOn tOff tMax Corr saveRes recBGspks recINspks I0 JAin JAbg N runs" << endl;
		return 0;
	}
	
	//
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	// general parameters:
	const double Coh = 6.4;//atof(argv[1]);
	const double tOn = 0.5;//atof(argv[2]);
	const double tOff = 6;//atof(argv[3]);
	const double tMax = 6;//atof(argv[4]);
	const double inputCorrelation = 0;//atof(argv[5]);
	const bool saveResults = 1;//lexical_cast<bool>(argv[6]);
	const bool recordBGSpikes = 0;//lexical_cast<bool>(argv[7]);
	const bool recordInputSpikes = 0;//lexical_cast<bool>(argv[8]);
	const int N = 240;//atof(argv[12]);
	const int runs = atof(argv[1]);
	const int stepsz = 1;
		
	// Network dimension settings:
		//  weights
	double JAii = 0;//9.9026E-4;
	double JAij = 0;//6.5177E-5;
	double JNii = 0.2609;
	double JNij = 0.0497;
	double JAext= 5.2E-4;
		// H function parameters
	double a = 239400*JAii + 270; 
	double b = 97000*JAii + 108; 	
	double c = -30*JAii + 0.1540;
		// other
	double tau = 0.1;
	double gamma = 0.641;
		// firing rates
	const double BgFR = 2400; // background noise firing rate
	const double InFR1 = 40 + .4*Coh; // "correct choice" input firing rate
	const double InFR2 = 40 - .4*Coh; // "false choice" input firing rate
	const double I0 = 0.3255 - JAext*(BgFR+2240 - 4000);
		
	// Variables for thresholding:
	double Th_start = 5;
	double Th_max = 40;
	double Th_step = 0.05;
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
	
	//PoolPoisson I01("I01",Network,N,0,-I0/JAbg*N,0,0,tOff,tMax);
	//PoolPoisson I02("I02",Network,N,0,-I0/JAbg*N,0,0,tOff,tMax);

	PoolPoisson INS1("INS1",Network,N,0,2240*N,0,0,tOff,tMax);
	PoolPoisson INH1("INH1",Network,N,0,4000*N,0,0,tOff,tMax);
	//PoolPoisson INS2("INS2",Network,N,0,2240*N,0,0,tOff,tMax);
	//PoolPoisson INH2("INH2",Network,N,0,4000*N,0,0,tOff,tMax);
	
	//PoolPoisson In2("In2",Network,N,0,InFR2*N,0,tOn,tOff,tMax);
	//PoolPoisson In2("In2",Network,N,0,InFR2*N,0,tOn,tOff,tMax);
	
	// ODE system
	PoolODE ODE("ODE",Network,tMax,a,b,c,JNii,JNij,JAij,JAext,JAext,tau,gamma,I0);
	
	// file declaration
	ofstream myfile;
	if(saveResults) myfile.open(argv[2]);

	//double tot = 1/(0.001*Network.dt);
		
	
	
    //double temp[2];
	
	// simulate "runs" trials
	for(int j = 0; j < runs ; j++)
	{		
		Network.init();	
		//ODE.run(In1,BG1,In2,BG2);
		//ODE.run2(In1,BG1,In2,BG2,I01,I02);
		ODE.run3(In1,BG1,In2,BG2,INS1,INH1,INS1,INH1);
		
		if(saveResults)
		{
			//int K = ODE.S1.size();
//			temp[1] = 0;
//			temp[2] = 0;
//			for(int k = (tOff-1)/(Network.dt*0.001); k < tOff/(Network.dt*0.001) ; k++)
//			{
//				temp[1] = temp[1] + ODE.S1[k]/tot;
//				temp[2] = temp[2] + ODE.S2[k]/tot;
//			}
//				
//			myfile << temp[1] << " " << temp[2] << endl;
			myfile << ODE.S1[10000] << " " << ODE.S2[10000] << " " << ODE.S1[25000] << " " << ODE.S2[25000] << " " << ODE.S1[50000] << " " << ODE.S2[50000] << endl;
			
	//		for(int i=0;i < K ; i=i+stepsz) myfile << ODE.X1[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << ODE.X2[i] << " ";
//			myfile << endl;	
//			for(int i=0;i < K ; i=i+stepsz) myfile << ODE.F1[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << ODE.F2[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << ODE.S1[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << ODE.S2[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << BG1.spks[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << BG2.spks[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << In1.spks[i] << " ";
//			myfile << endl;
//			for(int i=0;i < K ; i=i+stepsz) myfile << In2.spks[i] << " ";
//			myfile << endl;
			//for(int i=0;i < K ; i=i+stepsz) myfile << INS1.spks[i] << " ";
			//myfile << endl;
			//for(int i=0;i < K ; i=i+stepsz) myfile << INH1.spks[i] << " ";
			//myfile << endl;
			
		}
		int K = ODE.S1.size();
	//cout << "I0 = " << I0 << ": "<<  ODE.S1[K-10] << " " << ODE.S2[K-10] << endl;
		Thi = 0;
		for(double Th = Th_start; Th <= Th_max ; Th = Th + Th_step)
		{
			f = true;
			count = tOn/(Network.dt*0.001)-1;
			while(f)
			{
				if(ODE.F1[count] >= Th && ODE.F2[count] >= Th){
					f = false;
				}
				
				else
				{
					if(ODE.F1[count] >= Th)
					{
						hits[Thi]++;
						times[Thi][j] = count+1;
						f = false;
					}
					else
					{
						if(ODE.F2[count] >= Th)
						{
							misses[Thi]++;
							times[Thi][j] = count+1;
							f = false;
						}
					}
				}
				
				if(count == ODE.S2.size()){
					f = false;
				}
				count++;
			}
			Thi++;
		}
	
	}

	if(saveResults) myfile.close();
	
				
				myfile.open(argv[3]);	
	

				for(int i=0;i < L ; i++)
				{
					acc[i] = hits[i]/(hits[i]+misses[i]);
					times_mean[i] = 0;
					for(int j = 0;j < runs ; j++) times_mean[i] = times_mean[i] + times[i][j]*Network.dt*0.001/(hits[i]+misses[i]);
				
					//cout << acc[i] << " " << times_mean[i] << endl; 

					myfile << times_mean[i] << " " << acc[i] << endl;
				
				}
				myfile.close();
			
	
				
				
				
	cout << "finished" << endl;
	return 0;
	
}
