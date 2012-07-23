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
	PoolBGHPoisson BGESel1("BGESel1", Network, NSel, recordBGSpikes, BgFRE, 0, 0, tOff);  //1700->.7 // 2400->30
	PoolBGHPoisson BGESel2("BGESel2", Network, NSel, recordBGSpikes, BgFRE, 0, 0, tOff);
		
	// Input populations:
	PoolBGHPoisson InputSel1("InputSel1", Network, NSel, recordInputSpikes, InputPoolFRSel1, inputCorrelation, tOn, tOff);
	PoolBGHPoisson InputSel2("InputSel2", Network, NSel, recordInputSpikes, InputPoolFRSel2, inputCorrelation, tOn, tOff);
    Network.addInputPool(InputSel1);
    Network.addInputPool(InputSel2);
    
    // IBar Offset:
    PoolBGOU BGEOU1("BGEOU1", Network, NSel, false,-.416*2, .01, 0, tOff);
    PoolBGOU BGEOU2("BGEOU2", Network, NSel, false,-.416*2, .01, 0, tOff);

	
    
    
//    PoolBGOU::PoolBGOU(string poolName_in,
//                       Brain &parentPool_in,
//                       int N_in, 
//                       bool recordSpikes_in, 
//                       double mu_in,
//                       double sigma_in,
//                       double tOn_in,
//                       double tOff_in): PoolBG(poolName_in, 
//                                               parentPool_in, 
//                                               N_in, 
//                                               recordSpikes_in)
    
    
    
    
    
	// Excitatory populations:
	PoolRecEx GESel1("GESel1", Network, NSel, true);
	PoolRecEx GESel2("GESel2", Network, NSel, true);
	
	//========================================================================//
	//========================== Connect Network =============================//
	//========================================================================//

    //    MonitorPoolFile GESel1MonitorSBGSum(Network, GESel1, S_SBGSum, "GESel1SBGSum");
    //    MonitorPoolFile GESel1MonitorSBGIBarSum(Network, BGEOU1, S_AMPASum, "BGEOU1Sum");
    //    MonitorNeuronFile GESel1MonitorSBGIBar0(Network, BGEOU1, 0, S_AMPA, "BGEOU1_0");
    //    MonitorPoolFile GESel1MonitorSInputSum(Network, GESel1, S_SInputSum, "GESel1SInputSum");
    //    MonitorPoolFile GESel1MonitorConstInput(Network, GESel1, S_ISynInputPoolSum, "GESel1PoolInput");
    //    MonitorPoolFile GESel1MonitorConstBG(Network, GESel1, S_ISynBGPoolSum, "GESel1PoolBG");
    //    MonitorPoolFile GESel1MonitorAMPA(Network, GESel1, S_ISynRecAMPASum, "GESel1PoolRecAMPA");
    //    MonitorPoolFile GESel1MonitorNMDA(Network, GESel1, S_ISynRecNMDASum, "GESel1PoolRecNMDA");
    //    MonitorPoolFile GESel1MonitorGABA(Network, GESel1, S_ISynRecGABASum, "GESel1PoolRecGABA");
    
    

	// Connections to GESel1:
	GESel1.connectTo(BGESel1);
	GESel1.connectTo(InputSel1);
    GESel1.connectTo(BGEOU1);

	// Connections to GESel2:
	GESel2.connectTo(BGESel2);
	GESel2.connectTo(InputSel2);
    GESel2.connectTo(BGEOU2);
	
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
