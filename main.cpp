//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

#include <string>
#include <boost/lexical_cast.hpp>
#include "Brain.h"
#include "PoolBGFile.h"
#include "PoolBGSL.h"
#include "PoolBGHPoisson.h"
#include "PoolBGInHPoisson.h"
#include "PoolBGOU.h"
#include "PoolRecEx.h"
#include "PoolRecInh.h"
#include "SpikeList.h"
#include "Monitor.h"
#include "MonitorNeuron.h"
#include "MonitorNeuronFile.h"
#include "MonitorBrain.h"

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
	string argString = argv[1];
	for(int i = 2; i < argc; i++)
		argString += string("_") + argv[i];
	
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	
	// Params passed in args:
	const double Coh = atof(argv[1]);
	const double tOn = atof(argv[2]);
	const double tOff = atof(argv[3]);
	const double tMax = atof(argv[4]);
	const double inputCorrelation = atof(argv[5]);
	const double inputRho = atof(argv[6]);
	const bool saveResults = lexical_cast<bool>(argv[7]);
	const bool recordBGSpikes = lexical_cast<bool>(argv[8]);
	const bool recordInputSpikes = lexical_cast<bool>(argv[9]);
	
	// Network dimension settings:
	const int NN = 2000;
	const double frEx = .8;
	const double frSel = .15;
	const double BgFRE = 2400;
	const double BgFRI = 2400;
	
	// Connectivity settings:
	double w = 1;
	double wPlus = 1.7;
	
	// Derived from passed args:
	const double InputPoolFRSel1 = 40 + .4*Coh;
	const double InputPoolFRSel2 = 40 - .4*Coh;
	
	// Network dimension settings, derived from settings:
	const int NE = NN*frEx;
	const int NI = NN-NE;
	const int NSel = NE*frSel;
	const int NNSel = NE - 2*NSel;
	
	// Connectivity settings, derived from settings:
	const double wMinus = (1 - frSel*(wPlus - 1)/(1 - frSel));
	
	//========================================================================//
	//========================== Create Network ==============================//
	//========================================================================//
	
	Brain Network(argString);
	
	PoolBGInHPoisson InputSel1("InputSel1", Network, NSel, recordInputSpikes, InputPoolFRSel1, inputRho, inputCorrelation, tOn, tOff);
	
	Network.init();
	
	while (Network.t < tMax)
	{
		Network.run(100);
	}
	
//	// Main network:
//	Brain Network(argString);
//	
////	// Backgroud populations:
//	PoolBGHPoisson BGESel1("BGESel1", Network, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGESel2("BGESel2", Network, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGENSel("BGENSel", Network, NNSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGI("BGI", Network, NI, recordBGSpikes, BgFRI, tOn, inputCorrelation, tOff);
//		
//	// Input populations:
//	PoolBGHPoisson InputSel1("InputSel1", Network, NSel, recordInputSpikes, InputPoolFRSel1, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson InputSel2("InputSel2", Network, NSel, recordInputSpikes, InputPoolFRSel2, inputCorrelation, tOn, tOff);
//	
//	// Excitatory populations:
//	PoolRecEx GESel1("GESel1", Network, NSel, true);
//	PoolRecEx GESel2("GESel2", Network, NSel, true);
//	PoolRecEx GENSel("GENSel", Network, NNSel, false);
//	
//	// Inhibitory populations:
//	PoolRecInh GI("GI", Network, NI, false);
//	
//	//========================================================================//
//	//========================== Connect Network =============================//
//	//========================================================================//
//	
//	// Connections to GESel1:
//	GESel1.connectTo(BGESel1);
//	GESel1.connectTo(InputSel1);
//	GESel1.connectTo(GESel1, wPlus);
//	GESel1.connectTo(GESel2, wMinus);
//	GESel1.connectTo(GENSel, wMinus);
//	GESel1.connectTo(GI);
//	
//	// Connections to GESel2:
//	GESel2.connectTo(BGESel2);
//	GESel2.connectTo(InputSel2);
//	GESel2.connectTo(GESel1, wMinus);
//	GESel2.connectTo(GESel2, wPlus);
//	GESel2.connectTo(GENSel, wMinus);
//	GESel2.connectTo(GI);
//	
//	// Connections to GENSel:
//	GENSel.connectTo(BGENSel);
//	GENSel.connectTo(GESel1, w);
//	GENSel.connectTo(GESel2, w);
//	GENSel.connectTo(GENSel, w);
//	GENSel.connectTo(GI);
//	
//	// Connections to GI:
//	GI.connectTo(BGI);
//	GI.connectTo(GESel1, w);
//	GI.connectTo(GESel2, w);
//	GI.connectTo(GENSel, w);
//	GI.connectTo(GI);
//	
//	//========================================================================//
//	//=========================== Run Network ================================//
//	//========================================================================//
//
//	
//	Network.init();
//	
//	while (Network.t < tMax)
//	{
//		Network.run(100);
//	}
//	
//	GESel1.toFileExact(GESel1.poolName + "a_" + Network.UUID_string + "_" + argString);
//	GESel2.toFileExact(GESel2.poolName + "a_" + Network.UUID_string + "_" + argString);
//	
//	Network.close();
//	
//	// Main network:
//	Brain Networkb(argString);
//	
//	//	// Backgroud populations:
//	PoolBGHPoisson BGESel1b("BGESel1", Networkb, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGESel2b("BGESel2", Networkb, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGENSelb("BGENSel", Networkb, NNSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
//	PoolBGHPoisson BGIb("BGI", Networkb, NI, recordBGSpikes, BgFRI, tOn, inputCorrelation, tOff);
//	
//	// Input populations:
//	PoolBGSL InputSel1b("InputSel1", Networkb, *InputSel1.spikeList);
//	PoolBGSL InputSel2b("InputSel2", Networkb, *InputSel2.spikeList);
//	
//	// Excitatory populations:
//	PoolRecEx GESel1b("GESel1", Networkb, NSel, true);
//	PoolRecEx GESel2b("GESel2", Networkb, NSel, true);
//	PoolRecEx GENSelb("GENSel", Networkb, NNSel, false);
//	
//	// Inhibitory populations:
//	PoolRecInh GIb("GI", Networkb, NI, false);
//	
//	//========================================================================//
//	//========================== Connect Network =============================//
//	//========================================================================//
//	
//	// Connections to GESel1:
//	GESel1b.connectTo(BGESel1b);
//	GESel1b.connectTo(InputSel1b);
//	GESel1b.connectTo(GESel1b, wPlus);
//	GESel1b.connectTo(GESel2b, wMinus);
//	GESel1b.connectTo(GENSelb, wMinus);
//	GESel1b.connectTo(GIb);
//	
//	// Connections to GESel2:
//	GESel2b.connectTo(BGESel2b);
//	GESel2b.connectTo(InputSel2b);
//	GESel2b.connectTo(GESel1b, wMinus);
//	GESel2b.connectTo(GESel2b, wPlus);
//	GESel2b.connectTo(GENSelb, wMinus);
//	GESel2b.connectTo(GIb);
//	
//	// Connections to GENSel:
//	GENSelb.connectTo(BGENSelb);
//	GENSelb.connectTo(GESel1b, w);
//	GENSelb.connectTo(GESel2b, w);
//	GENSelb.connectTo(GENSelb, w);
//	GENSelb.connectTo(GIb);
//	
//	// Connections to GI:
//	GIb.connectTo(BGIb);
//	GIb.connectTo(GESel1b, w);
//	GIb.connectTo(GESel2b, w);
//	GIb.connectTo(GENSelb, w);
//	GIb.connectTo(GIb);
//	
//	Networkb.init();
//	
//	while (Networkb.t < tMax)
//	{
//		Networkb.run(100);
//	}
//	
//	GESel1b.toFileExact(GESel1b.poolName + "b_" + Network.UUID_string + "_" + argString);
//	GESel2b.toFileExact(GESel2b.poolName + "b_" + Network.UUID_string + "_" + argString);
//	
//	Network.close();
//	
//	cout << Network.UUID_string << endl;
	
	return 0;
}