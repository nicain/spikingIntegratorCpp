#ifndef SI_SETTINGS_H
#define SI_SETTINGS_H

#include <boost/random.hpp>

//============================================================================//
//============================ Main Settings =================================//
//============================================================================//

namespace SI {
	
	// Simulation Settings:
	float tBegin = 0;
	float dt = 0.1;
	float t = 0;
	bool recordBGSpikes = true;
	bool recordInputSpikes = true;
	bool recordSel1Spikes = true;
	bool recordNSel1Spikes = true;
	
	// Network dimension settings:
	int NN = 2000;
	float frEx = .8;
	float frSel = .15;
	
	// Connectivity settings:
	float w = 1;
	float wPlus = 1.7;
	
	// Background settings:
	int BgPoolSize = 1;
	float BgFREPool = 2400;
	float BgFRIPool = 2400;
	float BGCorr = 0;
	
	// Input settings:
	int InputPoolSize = 1;
	
	// Voltage Settings:
	float VMin = -70;
	float VMax = -50;
	float VReset = -55;
	float VE = 0;
	float VI = -70;
	
	// Capacitance Settings:
	float cm_E = .5;
	float cm_I = .2;
	
	// Conductance Settings:
	float gL_E = 25;
	float gext_AMPA_E = 2.1;
	float grec_AMPA_E = .05;
	float gNMDA_E = .165;
	float gGABA_E = 1.3;
	float gL_I = 20;
	float gext_AMPA_I = 1.62;
	float grec_AMPA_I = .04;
	float gNMDA_I = .13;
	float gGABA_I = 1.0;
	
	// Refractory effects:
	float tRef_E = 2;
	float tRef_I = 1;
	
	// Time ants:
	float tau_AMPA = 2;
	float tau_NMDA = 100;
	float tau_GABA = 5;
	float tau_AMPA_rise = 2;
	
	// Misc. ants:
	float alpha = .5;
	float K = .062;
	
	// RNG:
	boost::random::mt19937 myRNG;
}

#endif
