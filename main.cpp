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
#include "MonitorPool.h"
#include "MonitorPoolFile.h"
#include "MonitorBrain.h"
#include "PoolRecHybrid.h"


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
	const bool saveResults = lexical_cast<bool>(argv[6]);
	const bool recordBGSpikes = lexical_cast<bool>(argv[7]);
	const bool recordInputSpikes = lexical_cast<bool>(argv[8]);
	
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
	Brain Network(argString);
    
    // Output UUID, so that FR can be automatically computed
	cout << Network.UUID_string << endl;
	
    // Monitor time, if you want:
    MonitorBrain brainMonitor(Network);
	
	// Backgroud populations:
	PoolBGHPoisson BGESel1("BGESel1", Network, NSel, recordBGSpikes, BgFRE, 0, 0, tOff);
	PoolBGHPoisson BGESel2("BGESel2", Network, NSel, recordBGSpikes, BgFRE, 0, 0, tOff);
	PoolBGHPoisson BGENSel("BGENSel", Network, NNSel, false, BgFRE, 0, 0, tOff);
    
	PoolBGHPoisson BGI("BGI", Network, NI, false, BgFRI+100, 0, 0, tOff);

		
	// Input populations:
	PoolBGHPoisson InputSel1("InputSel1", Network, NSel, recordInputSpikes, InputPoolFRSel1, inputCorrelation, tOn, tOff);
	PoolBGHPoisson InputSel2("InputSel2", Network, NSel, recordInputSpikes, InputPoolFRSel2, inputCorrelation, tOn, tOff);
    Network.addInputPool(InputSel1);
    Network.addInputPool(InputSel2);
	
	// Excitatory populations:
	PoolRecEx GESel1("GESel1", Network, NSel, true);
	PoolRecEx GESel2("GESel2", Network, NSel, true);
	PoolRecEx GENSel("GENSel", Network, NNSel, false);
	
	// Inhibitory populations:	
    PoolRecInh GI("GI", Network, NI, true);
//	PoolRecHybrid GI("GI", Network, NI);
	
	//========================================================================//
	//========================== Connect Network =============================//
	//========================================================================//

//    MonitorPoolFile GESel1MonitorSBGSum(Network, GI, S_SBGSum, "GI_papeSBGSum");
    
//    MonitorPoolFile GESel1MonitorSBGSum(Network, GESel1, S_SBGSum, "GESel1SBGSum");
//    MonitorPoolFile GESel1MonitorSInputSum(Network, GESel1, S_SInputSum, "GESel1SInputSum");
//    MonitorPoolFile GESel1MonitorConstInput(Network, GESel1, S_ISynInputPoolSum, "GESel1PoolInput");
//    MonitorPoolFile GESel1MonitorConstBG(Network, GESel1, S_ISynBGPoolSum, "GESel1PoolBG");
//    MonitorPoolFile GESel1MonitorAMPA(Network, GESel1, S_ISynRecAMPASum, "GESel1PoolRecAMPA");
//    MonitorPoolFile GESel1MonitorNMDA(Network, GESel1, S_ISynRecNMDASum, "GESel1PoolRecNMDA");
//    MonitorPoolFile GESel1MonitorGABA(Network, GESel1, S_ISynRecGABASum, "GESel1PoolRecGABA");

//    MonitorPoolFile GESel2MonitorSBGSum(Network, GESel2, S_SBGSum, "GESel2SBGSum");
//    MonitorPoolFile GESel2MonitorSInputSum(Network, GESel2, S_SInputSum, "GESel2SInputSum");

//    MonitorPoolFile GESel2MonitorConstInput(Network, GESel2, S_ISynInputPoolSum, "GESel2PoolInput");
//    MonitorPoolFile GESel2MonitorConstBG(Network, GESel2, S_ISynBGPoolSum, "GESel2PoolBG");
//    MonitorPoolFile GESel2MonitorAMPA(Network, GESel2, S_ISynRecAMPASum, "GESel2PoolRecAMPA");
//    MonitorPoolFile GESel2MonitorNMDA(Network, GESel2, S_ISynRecNMDASum, "GESel2PoolRecNMDA");
//    MonitorPoolFile GESel2MonitorGABA(Network, GESel2, S_ISynRecGABASum, "GESel2PoolRecGABA");
    
//    MonitorNeuronFile GESel1MonitorConstRecAMPA(Network, GESel1, 0, S_ISynRecAMPA, "GESel1RecAMPA0");
//    MonitorNeuronFile GESel1MonitorConstRecNMDA(Network, GESel1, 0, S_ISynRecNMDA, "GESel1RecNMDA0");
//    MonitorNeuronFile GESel1MonitorConstRecGABA0(Network, GESel1, 0, S_ISynRecGABA, "GESel1RecGABA0");
//    MonitorNeuronFile GESel1MonitorConstRecGABA1(Network, GESel1, 1, S_ISynRecGABA, "GESel1RecGABA1");
//    MonitorNeuronFile GESel1MonitorConstTot(Network, GESel1, 0, S_ISyn, "GESel1MonitorTotN0");
    
//    MonitorNeuronFile GESel2MonitorConstBG(Network, GESel2, 0, S_ISynBG, "GESel2MonitorBG");
//    MonitorNeuronFile GESel2MonitorConstRecAMPA(Network, GESel2, 0, S_ISynRecAMPA, "GESel2MonitorRecAMPA");
//    MonitorNeuronFile GESel2MonitorConstRecNMDA(Network, GESel2, 0, S_ISynRecNMDA, "GESel2MonitorRecNMDA");
//    MonitorNeuronFile GESel2MonitorConstRecGABA(Network, GESel2, 0, S_ISynRecGABA, "GESel2MonitorRecGABA");
//    MonitorNeuronFile GESel2MonitorConstTot(Network, GESel2, 0, S_ISyn, "GESel2MonitorTot");
    
//    MonitorPoolFile GESel1MonitorPoolTot(Network, GESel1, S_ISynPoolSum, "GESel1MonitorPoolTot");
//    MonitorPoolFile GESel2MonitorPoolTot(Network, GESel2, S_ISynPoolSum, "GESel2MonitorPoolTot");
//    MonitorPoolFile GENSelMonitorPoolTot(Network, GENSel, S_ISynPoolSum, "GENSelMonitorPoolTot");
//    MonitorPoolFile GIMonitorPoolTot(Network, GI, S_ISynPoolSum, "GIMonitorPoolTot");
    
//    int NRecordN = 2;
//    MonitorNeuronFile** ITotSel1 = new MonitorNeuronFile*[NRecordN];
//    string MonitorFileNamePrefix1 = "GESel1MonitorTot_";
//    for (int i=0; i<NRecordN; i++){
//        ITotSel1[i] = new MonitorNeuronFile(Network, GESel1, i, S_ISyn, MonitorFileNamePrefix1 + boost::lexical_cast<string>(i));
//    }
	
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
    
	Network.init();
	
	while (Network.t < tMax)
	{
		Network.run(1);
	}
	
	Network.close();
	
	if (saveResults)
	{
		Network.spikesToFile();
	}
	
	return 0;
}
