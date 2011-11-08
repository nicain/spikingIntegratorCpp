#ifndef SI_SETTINGS_H
#define SI_SETTINGS_H

#include <boost/random.hpp>

//============================================================================//
//============================ Main Settings =================================//
//============================================================================//

namespace SI {
	
	// Simulation Settings:
	extern const float tBegin;
	extern const float dt;
	extern float t;
	extern const bool recordBGSpikes;
	extern const bool recordInputSpikes;
	extern const bool recordSel1Spikes;
	extern const bool recordNSel1Spikes;
	
	// Network dimension settings:
	extern const int NN;
	extern const float frEx;
	extern const float frSel;
	
	// Connectivity settings:
	extern const float w;
	extern const float wPlus;
	
	// Background settings:
	extern const int BgPoolSize;
	extern const float BgFREPool;
	extern const float BgFRIPool;
	extern const float BGCorr;
	
	// Input settings:
	extern const int InputPoolSize;
	
	// Voltage Settings:
	extern const float VMin;
	extern const float VMax;
	extern const float VReset;
	extern const float VE;
	extern const float VI;
	
	// Capacitance Settings:
	extern const float cm_E;
	extern const float cm_I;
	
	// Conductance Settings:
	extern const float gL_E;
	extern const float gext_AMPA_E;
	extern const float grec_AMPA_E;
	extern const float gNMDA_E;
	extern const float gGABA_E;
	extern const float gL_I;
	extern const float gext_AMPA_I;
	extern const float grec_AMPA_I;
	extern const float gNMDA_I;
	extern const float gGABA_I;
	
	// Refractory effects:
	extern const float tRef_E;
	extern const float tRef_I;
	
	// Time Constants:
	extern const float tau_AMPA;
	extern const float tau_NMDA;
	extern const float tau_GABA;
	extern const float tau_AMPA_rise;
	
	// Misc. Constants:
	extern const float alpha;
	extern const float K;
	
	// RNG:
	extern boost::random::mt19937 myRNG;
}

#endif
