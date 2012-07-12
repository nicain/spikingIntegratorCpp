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
	PoolBGHPoisson BGESel1("BGESel1", Network, NSel, recordBGSpikes, 1700, 0, 0, tOff);  //1700->.7 // 2400->30
	PoolRecEx GESel1("GESel1", Network, NSel, true);
    MonitorPoolFile GESel1MonitorSBGSum(Network, GESel1, S_SBGSum, "GESel1SBGSum");
    MonitorPoolFile GESel1MonitorIBGSum(Network, GESel1, S_ISynBGPoolSum, "GESel1IBGSum");
	GESel1.connectTo(BGESel1);
    
    PoolBGHPoisson BGESel2("BGESel2", Network, NSel, recordBGSpikes, 1800, 0, 0, tOff);  //1700->.7 // 2400->30
	PoolRecEx GESel2("GESel2", Network, NSel, true);
    MonitorPoolFile GESel2MonitorSBGSum(Network, GESel2, S_SBGSum, "GESel2SBGSum");
    MonitorPoolFile GESel2MonitorIBGSum(Network, GESel2, S_ISynBGPoolSum, "GESel2IBGSum");
	GESel2.connectTo(BGESel2);
    
    PoolBGHPoisson BGESel3("BGESel3", Network, NSel, recordBGSpikes, 1900, 0, 0, tOff);
	PoolRecEx GESel3("GESel3", Network, NSel, true);
    MonitorPoolFile GESel3MonitorSBGSum(Network, GESel3, S_SBGSum, "GESel3SBGSum");
    MonitorPoolFile GESel3MonitorIBGSum(Network, GESel3, S_ISynBGPoolSum, "GESel3IBGSum");
	GESel3.connectTo(BGESel3);
    
    PoolBGHPoisson BGESel4("BGESel4", Network, NSel, recordBGSpikes, 2000, 0, 0, tOff);
	PoolRecEx GESel4("GESel4", Network, NSel, true);
    MonitorPoolFile GESel4MonitorSBGSum(Network, GESel4, S_SBGSum, "GESel4SBGSum");
    MonitorPoolFile GESel4MonitorIBGSum(Network, GESel4, S_ISynBGPoolSum, "GESel4IBGSum");
	GESel4.connectTo(BGESel4);
    
    PoolBGHPoisson BGESel5("BGESel5", Network, NSel, recordBGSpikes, 2100, 0, 0, tOff);
	PoolRecEx GESel5("GESel5", Network, NSel, true);
    MonitorPoolFile GESel5MonitorSBGSum(Network, GESel5, S_SBGSum, "GESel5SBGSum");
    MonitorPoolFile GESel5MonitorIBGSum(Network, GESel5, S_ISynBGPoolSum, "GESel5IBGSum");
	GESel5.connectTo(BGESel5);
    
    PoolBGHPoisson BGESel6("BGESel6", Network, NSel, recordBGSpikes, 2200, 0, 0, tOff);
	PoolRecEx GESel6("GESel6", Network, NSel, true);
    MonitorPoolFile GESel6MonitorSBGSum(Network, GESel6, S_SBGSum, "GESel6SBGSum");
    MonitorPoolFile GESel6MonitorIBGSum(Network, GESel6, S_ISynBGPoolSum, "GESel6IBGSum");
	GESel6.connectTo(BGESel6);
    
    PoolBGHPoisson BGESel7("BGESel7", Network, NSel, recordBGSpikes, 2300, 0, 0, tOff);
	PoolRecEx GESel7("GESel7", Network, NSel, true);
    MonitorPoolFile GESel7MonitorSBGSum(Network, GESel7, S_SBGSum, "GESel7SBGSum");
    MonitorPoolFile GESel7MonitorIBGSum(Network, GESel7, S_ISynBGPoolSum, "GESel7IBGSum");
	GESel7.connectTo(BGESel7);
    
    PoolBGHPoisson BGESel8("BGESel8", Network, NSel, recordBGSpikes, 2400, 0, 0, tOff);
	PoolRecEx GESel8("GESel8", Network, NSel, true);
    MonitorPoolFile GESel8MonitorSBGSum(Network, GESel8, S_SBGSum, "GESel8SBGSum");
    MonitorPoolFile GESel8MonitorIBGSum(Network, GESel8, S_ISynBGPoolSum, "GESel8IBGSum");
	GESel8.connectTo(BGESel8);
    
    PoolBGHPoisson BGESel9("BGESel9", Network, NSel, recordBGSpikes, 2500, 0, 0, tOff);
	PoolRecEx GESel9("GESel9", Network, NSel, true);
    MonitorPoolFile GESel9MonitorSBGSum(Network, GESel9, S_SBGSum, "GESel9SBGSum");
    MonitorPoolFile GESel9MonitorIBGSum(Network, GESel9, S_ISynBGPoolSum, "GESel9IBGSum");
	GESel9.connectTo(BGESel9);
    
    PoolBGHPoisson BGESel10("BGESel10", Network, NSel, recordBGSpikes, 2600, 0, 0, tOff);
	PoolRecEx GESel10("GESel10", Network, NSel, true);
    MonitorPoolFile GESel10MonitorSBGSum(Network, GESel10, S_SBGSum, "GESel10SBGSum");
    MonitorPoolFile GESel10MonitorIBGSum(Network, GESel10, S_ISynBGPoolSum, "GESel10IBGSum");
	GESel10.connectTo(BGESel10);
    
    PoolBGHPoisson BGESel11("BGESel11", Network, NSel, recordBGSpikes, 2700, 0, 0, tOff);
	PoolRecEx GESel11("GESel11", Network, NSel, true);
    MonitorPoolFile GESel11MonitorSBGSum(Network, GESel11, S_SBGSum, "GESel11SBGSum");
    MonitorPoolFile GESel11MonitorIBGSum(Network, GESel11, S_ISynBGPoolSum, "GESel11IBGSum");
	GESel11.connectTo(BGESel11);
    
    PoolBGHPoisson BGESel12("BGESel12", Network, NSel, recordBGSpikes, 2800, 0, 0, tOff);
	PoolRecEx GESel12("GESel12", Network, NSel, true);
    MonitorPoolFile GESel12MonitorSBGSum(Network, GESel12, S_SBGSum, "GESel12SBGSum");
    MonitorPoolFile GESel12MonitorIBGSum(Network, GESel12, S_ISynBGPoolSum, "GESel12IBGSum");
	GESel12.connectTo(BGESel12);
    
    PoolBGHPoisson BGESel13("BGESel13", Network, NSel, recordBGSpikes, 2900, 0, 0, tOff);
	PoolRecEx GESel13("GESel13", Network, NSel, true);
    MonitorPoolFile GESel13MonitorSBGSum(Network, GESel13, S_SBGSum, "GESel13SBGSum");
    MonitorPoolFile GESel13MonitorIBGSum(Network, GESel13, S_ISynBGPoolSum, "GESel13IBGSum");
	GESel13.connectTo(BGESel13);
    
    
    
    
    
	
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
