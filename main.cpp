//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

#include <string>
#include <boost/lexical_cast.hpp>
#include "Brain.h"
#include "PoolODE.h"
//#include "PoolPoisson.h"
//#include "PoolBGFile.h"
//#include "PoolBGSL.h"
//#include "PoolBGHPoisson.h"
//#include "PoolBGInHPoisson.h"
//#include "PoolBGOU.h"
//#include "PoolRecEx.h"
//#include "PoolRecInh.h"
//#include "SpikeList.h"
#include "Monitor.h"
//#include "MonitorNeuron.h"
//#include "MonitorNeuronFile.h"
//#include "MonitorBrain.h"

// Add variable names to scope:
using namespace std;
using boost::random::seed_seq;
using boost::uuids::random_generator;
using boost::uuids::uuid;
using boost::lexical_cast;

int main( int argc,      // Number of strings in array argv
		 char *argv[])
{	
	// Make settings string:
//	string argString = argv[1];
//	for(int i = 2; i < argc; i++)
//		argString += string("_") + argv[i];
	
	if(argc < 11)
	{
		cout << "Not enough inputs. Input format: Coh tOn tOff tMax Corr saveRes recBGspks recINspks I0 w N" << endl;
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
//	
	// Network dimension settings:
	const double BgFR = 2400; // background noise firing rate
	const double InFR1 = 40 + .4*Coh; // "correct choice" input firing rate
	const double InFR2 = 40 - .4*Coh; // "false choice" input firing rate
	
//	// Connectivity settings:
//	double w = 1;
//	double wPlus = 1.7;
//	
//	// Derived from passed args:
//	const double InputPoolFRSel1 = 40 + .4*Coh;
//	const double InputPoolFRSel2 = 40 - .4*Coh;
//	
//	// Network dimension settings, derived from settings:
//	const int NE = NN*frEx;
//	const int NI = NN-NE;
//	const int NSel = NE*frSel;
//	const int NNSel = NE - 2*NSel;
//	
//	// Connectivity settings, derived from settings:
//	const double wMinus = (1 - frSel*(wPlus - 1)/(1 - frSel));
	
	//========================================================================//
	//========================== Create Network ==============================//
	//========================================================================//
	
	// Main network:
	Brain Network;
    //SWK NHC
    // Output UUID, so that FR can be automatically computed
	cout << Network.UUID_string << endl;
	
    // Monitor time, if you want:
    //MonitorBrain brainMonitor(Network);
	
	// Test poisson population
	PoolPoisson BG1("BG1",Network,N,0,BgFR*N,0,0,tOff,tMax);
	PoolPoisson BG2("BG2",Network,N,0,BgFR*N,0,0,tOff,tMax);
	PoolPoisson In1("In1",Network,N,0,InFR1*N,0,tOn,tOff,tMax);
	PoolPoisson In2("In2",Network,N,0,InFR2*N,0,tOn,tOff,tMax);
	PoolODE ODE("ODE",Network,w,tMax,270,108,0.154,0.2609,0.0497,0.1,0.641,I0);
	

	//========================================================================//
	//=========================== Run Network ================================//
	//========================================================================//
	

	
	Network.init();
	
	ODE.run(In1,BG1,In2,BG2);
	
//	for(int i = 0;i<ODE.S1.size();i=i+1000)
//	{
//		cout << "timepoint " << i << ": "<<  ODE.S1[i] << " " << ODE.S2[i] << endl;
//	}
	
	cout << "finished" << endl;
	
//	while (Network.t < tMax)
//	{
//		Network.run(1);
//	}
//	
//	Network.close();
//	
//	if (saveResults)
//	{
//		Network.spikesToFile();
//	}
	
	return 0;
}
