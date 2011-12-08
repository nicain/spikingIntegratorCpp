/* TODO:
 1) Undo comments
 */

//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

#include <string>
#include "Brain.h"
#include "PoolBGFile.h"
#include "PoolBGPoisson.h"
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

int main(int argc, char** argv)
{
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	
	// Params passed in args:(Not implemented yet)
	const double Coh = 50;
	const double tOn = 0;
	const double tOff = 2000;
	const double tMax = tOff;
	const double inputCorrelation = .1;
	
	// Network dimension settings:
	const int NN = 2000;
	const double frEx = .8;
	const double frSel = .15;
	const double BgFREPool = 2400;
	const double BgFRIPool = 2400;
	
	// Connectivity settings:
	double w = 1;
	double wPlus = 1.7;
	
	// Derived from passed args:
	const double InputPoolFRSel1 = 40 + .4*Coh;
	const double InputPoolFRSel2 = 40 - .4*Coh;
	
	// Network dimension settings, derived from settings:
	const double frInh = 1-frEx;	
	const int NE = NN*frEx;
	const int NI = NN-NE;
	const int NSel = NE*frSel;
	const int NNSel = NE - 2*NSel;
	
	// Connectivity settings, derived from settings:
	const double wMinus = (1 - frSel*(wPlus - 1)/(1 - frSel));
	
	// Background settings, derived from settings:
	const double BFRE = BgFREPool;
	const double BFRI = BgFRIPool;
	
	//========================================================================//
	//========================== Create Network ==============================//
	//========================================================================//
	
	// Main network:
	Brain Network;
	
	// Backgroud populations:
//	PoolBGPoisson BGESel1("BGESel1", Network, NSel, true, BgFREPool, inputCorrelation, tOn, tOff);
	PoolBGFile BGESel1("BGESel1", Network, "BGSpikes/BGESel1_1.ntf");
	PoolBGFile BGESel2("BGESel2", Network, "BGSpikes/BGESel2_1.ntf");
	PoolBGFile BGENSel("BGENSel", Network, "BGSpikes/BGENSel_1.ntf");
	PoolBGFile BGI("BGI", Network, "BGSpikes/BGI_1.ntf");
	
	// Input populations:
	PoolBGFile InputSel1("InputSel1", Network, "BGSpikes/InputSel1_1.ntf");
	PoolBGFile InputSel2("InputSel2", Network, "BGSpikes/InputSel2_1.ntf");
	
	// Excitatory populations:
	PoolRecEx GESel1("GESel1", Network, NSel, false);
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
	
	
	MonitorBrain brainMonitor(Network);
	MonitorNeuronFile tmpMonitor(Network, GESel2, 0, S_ISyn);	
	
	
	Network.init();
	
	while (Network.t < 1000)
	{
		Network.run(100);
	}
	
//	GESel1.toFile("all");
	GESel2.toFile("blah");
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















