#ifndef SI_SETTINGS_H
#define SI_SETTINGS_H

#include <boost/random.hpp>

//============================================================================//
//============================ Main Settings =================================//
//============================================================================//

namespace SI {
	
	// Simulation Settings:
	float tBegin = 0;
	float dt = 0.02;
	float t = 0;
	bool recordBGSpikes = true;
	bool recordInputSpikes = true;
	bool recordSelSpikes = true;
	bool recordNSelSpikes = true;
	bool recordInhSpikes = true;
	
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
	float cm_E = .000001*.5;
	float cm_I = .000001*.2;
	
	// Conductance Settings:			
	float gL_E = .000000001*25;
	float gext_AMPA_E = .000000001*2.1;
	float grec_AMPA_E = .000000001*.05;
	float gNMDA_E = .000000001*.165;
	float gGABA_E = .000000001*1.3;
	float gL_I = .000000001*20;
	float gext_AMPA_I = .000000001*1.62;
	float grec_AMPA_I = .000000001*.04;
	float gNMDA_I = .000000001*.13;
	float gGABA_I = .000000001*1.0;
	
	// Time constants:					
	float tau_AMPA = 2;
	float tau_NMDA = 100;
	float tau_GABA = 5;
	float tau_AMPA_rise = 2;
	
	// Misc. constants:					
	float alpha = .5;
	float K = -.062;
	
	// RNG:
	boost::random::mt19937 myRNG;
}

#endif
