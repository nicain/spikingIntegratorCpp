//============================================================================//
//============================== Preamble ====================================//
//============================================================================//

// File name:  spikingIntegrator.cpp
// Purpose:    spiking simulator

// Include external stuff:
#include <string>
#include <valarray>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include "SI_functions.h"
#include "SI_settings.h"
#include "BGPool.h"
#include "ExPool.h"
#include "InhPool.h"

// Add variable names to scope:
using namespace std;
using namespace SI;
using boost::random::seed_seq;
using boost::uuids::random_generator;
using boost::uuids::uuid;

int main(int argc, char** argv)
{
	//========================================================================//
	//======================== Initializations ===============================//
	//========================================================================//
	
	// Params passed in args:(Not implemented yet)
	const float Coh = 50;
	const float tOn = 0;
	const float tOff = 2000;
	const float tMax = tOff;
	const float inputCorrelation = .1;
	
	// Derived from passed args:
	const float InputPoolFRSel1 = 40 + .4*Coh;
	const float InputPoolFRSel2 = 40 - .4*Coh;
	
	// Network dimension settings, derived from settings:
	const float frInh = 1-frEx;	
	const int NE = NN*frEx;
	const int NI = NN-NE;
	const int NSel = NE*frSel;
	const int NNSel = NE - 2*NSel;
	
	// Connectivity settings, derived from settings:
	const float wMinus = (1 - frSel*(wPlus - 1)/(1 - frSel));
	
	// Background settings, derived from settings:
	const float BFRE = BgFREPool/BgPoolSize;
	const float BFRI = BgFRIPool/BgPoolSize;
	
	//========================================================================//
	//========================== Create Network ==============================//
	//========================================================================//
	
	// Create UUID as string:
	uuid uuid = random_generator()();
	string UUID_string = remove_hyphens(uuid);
	
	// Random number generators:
	seed_seq mySeed = seed_seq(UUID_string); 
	myRNG.seed(mySeed);
	
	// Create background populations:	
	BGPool BGESel1("BGESel1", BgPoolSize, NSel, BFRE, BGCorr, recordBGSpikes, tBegin, tMax);
	BGPool BGESel2("BGESel2", BgPoolSize, NSel, BFRE, BGCorr, recordBGSpikes, tBegin, tMax);
	BGPool BGENSel("BGENSel", BgPoolSize, NNSel, BFRE, BGCorr, recordBGSpikes, tBegin, tMax);
	BGPool BGI("BGI", BgPoolSize, NI, BFRI, BGCorr, recordBGSpikes, tBegin, tMax);
	
	// Create input populations:
	BGPool InputSel1("InputSel1", InputPoolSize, NSel, InputPoolFRSel1, inputCorrelation, recordInputSpikes, tOn, tOff);
	BGPool InputSel2("InputSel2", InputPoolSize, NSel, InputPoolFRSel2, inputCorrelation, recordInputSpikes, tOn, tOff);
	
	// Create excitatory populations:
	ExPool GESel1("GESel1", NSel, recordSelSpikes);
	ExPool GESel2("GESel2", NSel, recordSelSpikes);
	ExPool GENSel("GENSel", NNSel, recordNSelSpikes);
	
	// Create inhibitory population:
	InhPool GI("GI", NI, recordInhSpikes);
	
	//========================================================================//
	//=========================== Make Connections ===========================//
	//========================================================================//
	
	// Connections to GESel1:
	GESel1.connectTo(&BGESel1);
	GESel1.connectTo(&InputSel1);
	GESel1.connectTo(&GESel1, wPlus);
	GESel1.connectTo(&GESel2, wMinus);
	GESel1.connectTo(&GENSel, wMinus);
	GESel1.connectTo(&GI);
	
	// Connections to GESel2:
	GESel2.connectTo(&BGESel2);
	GESel2.connectTo(&InputSel2);
	GESel2.connectTo(&GESel1, wMinus);
	GESel2.connectTo(&GESel2, wPlus);
	GESel2.connectTo(&GENSel, wMinus);
	GESel2.connectTo(&GI);

	// Connections to GENSel:
	GENSel.connectTo(&BGENSel);
	GENSel.connectTo(&GESel1, w);
	GENSel.connectTo(&GESel2, w);
	GENSel.connectTo(&GENSel, w);
	GENSel.connectTo(&GI);
	
	// Connections to GI:
	GI.connectTo(&BGI);
	GI.connectTo(&GESel1, w);
	GI.connectTo(&GESel2, w);
	GI.connectTo(&GENSel, w);
	GI.connectTo(&GI);
	
	//========================================================================//
	//========================= Initialize Network ===========================//
	//========================================================================//

	// Initialize clock:
	t = tBegin;	
	
	// Initialize background populations:
	BGESel1.init();
	BGESel2.init();
	BGENSel.init();
	BGI.init();
	
	// Initialize input populations:
	InputSel1.init();
	InputSel2.init();
	
	// Initialize excitatory populations:
	GESel1.init();
	GESel2.init();
	GENSel.init();
	
	// Initialize inhibirtory population:
	GI.init();
	
	//========================================================================//
	//============================= Run Network ==============================//
	//========================================================================//
	
	while (t < tMax)
	{
		// Propogate background populations:
		BGESel1.propogate();
		BGESel2.propogate();
		BGENSel.propogate();
		BGI.propogate();
		
		// Propogate input populations:
		InputSel1.propogate();
		InputSel2.propogate();
		
		// Propogate excitatory populations:
		GESel1.propogate();
		GESel2.propogate();
		GENSel.propogate();

		// Propogate inhibitory populations:
		GI.propogate();

		//Increment time:
		cout << t << "\t" << (*GESel1.V)[0] << "\t" << (*GESel1.AMPA)[0] << "\t" << (*GESel1.NMDA)[0] << "\t" << (*GESel1.X)[0]<< "\t" << (*GESel1.ISyn)[0] << endl;

		t += dt;
	};

	
	return 0;
}



//	cout << endl;	
//
//	cout << BGESel1.getFR() << endl;
//	cout << BGESel2.getFR() << endl;
//	cout << BGENSel.getFR() << endl;
//	cout << BGI.getFR() << endl;
//	
//	cout << InputSel1.getFR() << endl;
//	cout << InputSel2.getFR() << endl;
//	
//	cout << GESel1.getFR() << endl;
//	cout << GESel2.getFR() << endl;
//	cout << GENSel.getFR() << endl;
//	
//	cout << GI.getFR() << endl;
//	
//	GESel1.writeSpikes("A");
//	GESel2.writeSpikes("A");

//	GESel1.listSpikes();
//	GESel2.listSpikes();
//	GENSel.listSpikes();

//	for (int i=0; i<=(*GESel1.spikeRecord_n).size()-1; i++) {
//			cout << (*GESel1.spikeRecord_n)[i] << "\t" << (*GESel1.spikeRecord_t)[i] << endl;
//	}

//	cout << (*GESel1.spikeRecord_n).size() << endl;
//	cout << (*GESel2.spikeRecord_n)[12] << endl;
//	cout << GENSel.getFR() << endl;
