//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

#include <string>
#include <boost/lexical_cast.hpp>
#include "Brain.h"
#include "PoolBGFile.h"
#include "PoolBGPoisson.h"
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
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	
	// Simulation settings:
	string resultDir = "../../Results"
	
	// Params passed in args:
	const double Coh = atof(argv[1]);
	const double tOn = atof(argv[2]);
	const double tOff = atof(argv[3]);
	const double tMax = atof(argv[4]);
	const double inputCorrelation = atof(argv[5]);
	const bool saveResults = lexical_cast<bool>(argv[6]);
	const bool recordBGSpikes = lexical_cast<bool>(argv[7]);
	
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
	
	// Main network:
	Brain Network;
	
//	// Backgroud populations:
//	PoolBGOU BGESel1("BGESel1", Network, NSel, true, BgFREPool, tOn, tOff);
//	PoolBGFile BGESel1("BGESel1", Network, "BGSpikes/BGESel1_1.ntf");
//	PoolBGFile BGESel2("BGESel2", Network, "BGSpikes/BGESel2_1.ntf");
//	PoolBGFile BGENSel("BGENSel", Network, "BGSpikes/BGENSel_1.ntf");
//	PoolBGFile BGI("BGI", Network, "BGSpikes/BGI_1.ntf");

	PoolBGPoisson BGESel1("BGESel1", Network, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
	PoolBGPoisson BGESel2("BGESel2", Network, NSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
	PoolBGPoisson BGENSel("BGENSel", Network, NNSel, recordBGSpikes, BgFRE, inputCorrelation, tOn, tOff);
	PoolBGPoisson BGI("BGI", Network, NI, recordBGSpikes, BgFRI, tOn, inputCorrelation, tOff);
	
//	PoolBGOU BGESel1("BGESel1", Network, NSel, true, BgFRE, tOn, tOff);
//	PoolBGOU BGESel2("BGESel2", Network, NSel, true, BgFRE, tOn, tOff);
//	PoolBGOU BGENSel("BGENSel", Network, NNSel, true, BgFRE, tOn, tOff);
//	PoolBGOU BGI("BGI", Network, NI, true, BgFRI, tOn, tOff);
	
	// Input populations:
	PoolBGPoisson InputSel1("InputSel1", Network, NSel, false, InputPoolFRSel1, inputCorrelation, tOn, tOff);
	PoolBGPoisson InputSel2("InputSel2", Network, NSel, false, InputPoolFRSel2, inputCorrelation, tOn, tOff);
	
	//	PoolBGFile InputSel1("InputSel1", Network, "BGSpikes/InputSel1_1.ntf");
	//	PoolBGFile InputSel2("InputSel2", Network, "BGSpikes/InputSel2_1.ntf");
	
	// Excitatory populations:
	PoolRecEx GESel1("GESel1", Network, NSel, true);
	PoolRecEx GESel2("GESel2", Network, NSel, true);
	PoolRecEx GENSel("GENSel", Network, NNSel, false);
	
	// Inhibitory populations:
	PoolRecInh GI("GI", Network, NI, false);
	
	//========================================================================//
	//========================== Connect Network =============================//
	//========================================================================//
	
	// Connections to GESel1:
	GESel1.connectTo(BGESel1);
	GESel1.connectTo(InputSel1);
	GESel1.connectTo(GESel1, wPlus);
	GESel1.connectTo(GESel2, wMinus);
	GESel1.connectTo(GENSel, wMinus);
	GESel1.connectTo(GI);
	
	// Connections to GESel2:
	GESel2.connectTo(BGESel2);
	GESel2.connectTo(InputSel2);
	GESel2.connectTo(GESel1, wMinus);
	GESel2.connectTo(GESel2, wPlus);
	GESel2.connectTo(GENSel, wMinus);
	GESel2.connectTo(GI);
	
	// Connections to GENSel:
	GENSel.connectTo(BGENSel);
	GENSel.connectTo(GESel1, w);
	GENSel.connectTo(GESel2, w);
	GENSel.connectTo(GENSel, w);
	GENSel.connectTo(GI);
	
	// Connections to GI:
	GI.connectTo(BGI);
	GI.connectTo(GESel1, w);
	GI.connectTo(GESel2, w);
	GI.connectTo(GENSel, w);
	GI.connectTo(GI);
	
	//========================================================================//
	//=========================== Run Network ================================//
	//========================================================================//
	
	
	//	MonitorNeuronFile tmpMonitor(Network, InputSel1, 0, S_AMPA);
	//	MonitorNeuron tmpMonitor2(Network, BGESel1, 0, S_AMPA);
	//	MonitorNeuronFile tmpMonitor(Network, BGESel1, 0, S_AMPA);
	
	
//	MonitorBrain brainMonitor(Network);
	
	
	Network.init();
	
	while (Network.t < tMax)
	{
		Network.run(100);
	}
	
	if (saveResults) {
		Network.spikesToFile();
	}
	
//	GESel1.toFile("blah");
//	GESel2.toFile("blah");
	
	// Spike output:
	//	GESel1.toFile("all");
	//	GESel2.toFile("all");
	//	GENSel.toFile("all");
	//	GI.toFile("all");
	//	BGESel1.toFile("all");
	//	BGESel2.toFile("all");
	//	BGENSel.toFile("all");
	//	BGI.toFile("all");
	//	InputSel1.toFile("all");
	//	InputSel2.toFile("all");
	
	
	Network.close();
	
	return 0;
}