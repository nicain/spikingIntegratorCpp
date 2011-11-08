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
	const float tOff = 1000;
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
	ExPool GESel1("GESel1", NSel, recordSel1Spikes);
	ExPool GESel2("GESel2", NSel, recordSel1Spikes);
	ExPool GENSel("GENSel", NNSel, recordNSel1Spikes);
	
	//========================================================================//
	//=========================== Make Connections ===========================//
	//========================================================================//
	
	// Connections to GESel1:
//	GESel1.connectTo(&BGESel1);
	GESel1.connectTo(&InputSel1);
//	GESel1.connectTo(&GESel1, wPlus);
//	GESel1.connectTo(&GESel2, wMinus);
//	GESel1.connectTo(&GENSel, w);
//	
//	// Connections to GESel2:
//	GESel2.connectTo(&BGESel2);
//	GESel2.connectTo(&InputSel2);
//	GESel2.connectTo(&GESel1, wMinus);
//	GESel2.connectTo(&GESel2, wPlus);
//	GESel2.connectTo(&GENSel, w);
//	
//	// Connections to GENSel:
//	GENSel.connectTo(&BGENSel);
//	GENSel.connectTo(&GESel1, w);
//	GENSel.connectTo(&GESel2, w);
//	GENSel.connectTo(&GENSel, w);
	
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
//		GESel2.propogate();
//		GENSel.propogate();




		
//	cout << (*GESel1.AMPA)[12] << "\t" << (*GESel2.AMPA)[0] << endl;
		//Increment time:
//		cout << t << endl;

		t += dt;
	};

//	cout << BGESel1.getFR() << endl;
//	cout << BGESel2.getFR() << endl;
//	cout << BGENSel.getFR() << endl;
//	cout << BGI.getFR() << endl;
//	
//	cout << InputSel1.getFR() << endl;
//	cout << InputSel2.getFR() << endl;
//	
//	cout << GESel1.getFR() << endl;
	
	for (int i=0; i<=(*GESel1.spikeRecord_n).size()-1; i++) {
			cout << (*GESel1.spikeRecord_n)[i] << "\t" << (*GESel1.spikeRecord_t)[i] << endl;
	}
	
	cout << (*GESel1.spikeRecord_n).size() << endl;
//	cout << (*GESel2.spikeRecord_n)[12] << endl;
//	cout << GENSel.getFR() << endl;
	
	return 0;


}


//	cout << BGESel1.getFR() << endl;
//	InputSel1.writeSpikes(UUID_string);

//============================================================================//
//============================ Banished Code =================================//
//============================================================================//

	


















//void propogate_background_spikes(float *masterTime, 
//								 float **Pre_Post_connect,
//								 variate_generator<mt19937&,exponential_distribution<float> > *expRnd,
//								 variate_generator<mt19937&,uniform_real_distribution<float> > *unifRnd,
//								 variate_generator<mt19937&,binomial_distribution<> > *binomRnd);
	
	
	//	//========================================================================//
	//	//========================== Create State Vars ===========================//
	//	//========================================================================//
	//	
	//	// Create external AMPA state variable:
	//	valarray<float> S_ext_AMPA_GE((float)0, NE);
	//	valarray<float> S_ext_AMPA_GI((float)0, NI);
	//	
	//	// Create internal AMPA state variable (pre-synaptic, so exc. only):
	//	valarray<float> S_int_AMPA_GESel1((float)0, NSel);
	//	valarray<float> S_int_AMPA_GESel2((float)0, NSel);
	//	valarray<float> S_int_AMPA_GENSel((float)0, NNSel);
	//	
	//	// Create internal "X" state variable (pre-synaptic, so exc. only):
	//	valarray<float> S_X_GESel1((float)0, NSel);
	//	valarray<float> S_X_GESel2((float)0, NSel);
	//	valarray<float> S_X_GENSel((float)0, NNSel);
	//	
	//	// Create internal NMDA state variable (pre-synaptic, so exc. only):
	//	valarray<float> S_NMDA_GESel1((float)0, NSel);
	//	valarray<float> S_NMDA_GESel2((float)0, NSel);
	//	valarray<float> S_NMDA_GENSel((float)0, NNSel);
	//	
	//	// Create internal GABA state variable (pre-synaptic, so inh. only):
	//	valarray<float> S_GABA_GI((float)0, NI);
	//	
	//	// Create Voltage state variable:
	//	valarray<float> V_GESel1((float)0, NSel);
	//	valarray<float> V_GESel2((float)0, NSel);
	//	valarray<float> V_GENSel((float)0, NNSel);
	//	valarray<float> V_GEI((float)0, NI);
	//
	//	//========================================================================//
	//	//=========================== Connect Pools ==============================//
	//	//========================================================================//
	//	
	//	// Connect background pools:
	////	float** BGE_GE_connect;
	////	BGE_GE_connect = new float*[NE*BgPoolSize];
	////	int counter = 0;
	////	for (i = 0; i < NE; i++)
	////	{
	////		for (j = 0; j < BgPoolSize; j++)
	////		{
	////			BGE_GE_connect[counter] = &S_ext_AMPA_GE[i];
	////			counter += 1;
	////		}
	////	}
	////	// Connect background pools:
	////	float** BGI_GI_connect;
	////	BGI_GI_connect = new float*[NI*BgPoolSize];
	////	counter = 0;
	////	for (i = 0; i < NI; i++)
	////	{
	////		for (j = 0; j < BgPoolSize; j++)
	////		{
	////			BGI_GI_connect[counter] = &S_ext_AMPA_GI[i];
	////			counter += 1;
	////		}
	////	}
	////
	////	// Connect input pools:
	////	float** InputSel1_GSel1_connect;
	////	InputSel1_GSel1_connect = new float*[totalInputPoolSize];
	////	counter = 0;
	////	for (i = GESel1Min; i < GESel1Max; i++)
	////	{
	////		for (j = 0; j < InputPoolSize; j++)
	////		{
	////			InputSel1_GSel1_connect[counter] = &S_ext_AMPA_GE[i];
	////			counter += 1;
	////		}
	////	}
	////	// Connect input pools:
	////	float** InputSel2_GSel2_connect;
	////	InputSel2_GSel2_connect = new float*[totalInputPoolSize];
	////	counter = 0;
	////	for (i = GESel2Min; i < GESel2Max; i++)
	////	{
	////		for (j = 0; j < InputPoolSize; j++)
	////		{
	////			InputSel2_GSel2_connect[counter] = &S_ext_AMPA_GE[i];
	////			counter += 1;
	////		}
	////	}
	
	
	//========================================================================//
	//========================= Initialize Network ===========================//
	//========================================================================//
	
	//	// Initialize background pools:
	//	propogate_BG_spikes(&BGE_Spike_Time, BGE_GE_connect, &expRndBGE);
	//	propogate_BG_spikes(&BGI_Spike_Time, BGI_GI_connect, &expRndBGI);
	//	
	//	// Initialize input pools:
	//	propogate_Input_spikes(&InputSel1_Spike_Time_Master, InputSel1_GSel1_connect, &expRndPostSel1, &unifRndSelPop, &binomRndSelPop);
	//	propogate_Input_spikes(&InputSel2_Spike_Time_Master, InputSel2_GSel2_connect, &expRndPostSel2, &unifRndSelPop, &binomRndSelPop);
	
	// Initialize Voltages:
	//	for (i=0; i<NSel; i++) {V_GESel1[i] = unifRndSelPop()*.005-.055;};
	//	for (i=0; i<NSel; i++) {V_GESel2[i] = unifRndSelPop()*.005-.055;};
	//	for (i=0; i<NNSel; i++) {V_GENSel[i] = unifRndSelPop()*.005-.055;};
	//	for (i=0; i<NI; i++) {V_GEI[i] = unifRndSelPop()*.005-.055;};

	
	
	

//		
//		// Propogate background spikes into each neuron:
//		propogate_BG_spikes(&BGE_Spike_Time, BGE_GE_connect, &expRndBGE);
//		propogate_BG_spikes(&BGI_Spike_Time, BGI_GI_connect, &expRndBGI);
//		
//		// Propogate input spikes into each neuron:
//		if (t > tOn && t < tOff) 
//		{
//			propogate_Input_spikes(&InputSel1_Spike_Time_Master, InputSel1_GSel1_connect, &expRndPostSel1, &unifRndSelPop, &binomRndSelPop);
//			propogate_Input_spikes(&InputSel2_Spike_Time_Master, InputSel2_GSel2_connect, &expRndPostSel2, &unifRndSelPop, &binomRndSelPop);
//		}
//		else
//		{
//			InputSel1_Spike_Time_Master = t + dt + .0001;
//			InputSel2_Spike_Time_Master = t + dt + .0001;
//		}
//		
//		
//		// Forward-euler applied to state variables:
//		S_ext_AMPA_GE -= tau_AMPA_Inv_times_dt*S_ext_AMPA_GE;
//		S_ext_AMPA_GI -= tau_AMPA_Inv_times_dt*S_ext_AMPA_GI; 
//
//		S_int_AMPA_GESel1 -= tau_AMPA_Inv_times_dt*S_int_AMPA_GESel1;
//		S_int_AMPA_GESel2 -= tau_AMPA_Inv_times_dt*S_int_AMPA_GESel2;
//		S_int_AMPA_GENSel -= tau_AMPA_Inv_times_dt*S_int_AMPA_GENSel;
//		
//		S_X_GESel1 -= tau_AMPA_rise_Inv_times_dt*S_X_GESel1;
//		S_X_GESel2 -= tau_AMPA_rise_Inv_times_dt*S_X_GESel2;
//		S_X_GENSel -= tau_AMPA_rise_Inv_times_dt*S_X_GENSel;
//		
//		S_NMDA_GESel1 = one_minus_tau_NMDA_Inv_times_dt*S_NMDA_GESel1 + alpha_dt*S_X_GESel1 - alpha_dt*S_X_GESel1*S_NMDA_GESel1;
//		S_NMDA_GESel2 = one_minus_tau_NMDA_Inv_times_dt*S_NMDA_GESel2 + alpha_dt*S_X_GESel2 - alpha_dt*S_X_GESel2*S_NMDA_GESel2;
//		S_NMDA_GENSel = one_minus_tau_NMDA_Inv_times_dt*S_NMDA_GENSel + alpha_dt*S_X_GENSel - alpha_dt*S_X_GENSel*S_NMDA_GENSel;
//
//		S_GABA_GI -= tau_GABA_Inv_times_dt*S_GABA_GI;
//		
//		
//		
//		
//		
//		
//
//
//		
//		
//		// TODO
//		//		external AMPA should be held pre, not post, synaptically.
//		//		Connect internal AMPA
//		//		Connect "X"
//		//		Connect GABA
//		
//
//		
//		
//		
//		
//
//		
//		 
//		
//
//		
//
//		
	
//	}
//		cout << t  << "\t" << S_ext_AMPA_GE[0]<< "\t" << S_ext_AMPA_GE[GESel2Min] << "\t" <<InputPoolFRPostSel1 << "\t" <<InputPoolFRPostSel2<<endl;	

	



//
//
//
//void propogate_background_spikes(float *masterTime, 
//						 float **Pre_Post_connect,
//						 variate_generator<mt19937&,exponential_distribution<float> > *expRnd,
// 						 variate_generator<mt19937&,uniform_real_distribution<float> > *unifRnd,
//						 variate_generator<mt19937&,binomial_distribution<> > *binomRnd)
//{
//	
//	if (SI::inputCorrelation == 0){
//		
//		// Input will generate pooled FR:
//		while (*masterTime <= SI::t) 
//		{
//			// This means a spike happened in this neuron! Increment...
//			*masterTime += (*expRnd)();
//
//			// And propogate:
//			if (SI::t !=0)
//			{
//				// change
//				*Pre_Post_connect[int(SI::NSel*SI::InputPoolSize*(*unifRnd)())] += 1;
//			}
//		}
//
//
//		
//	}
//	else {
//		
//		while (*masterTime <= SI::t) 
//		{
//			// This means a spike happened in master train neuron! 
//			*masterTime += (*expRnd)();
//			
//			// And propogate, with Knuth sampling w/o replacement:
//			if (SI::t !=0)
//			{
//				SI::numSpikesInCorrPool = (*binomRnd)();				
//				for (int i=0; i<SI::numSpikesInCorrPool; i++) 
//				{
//					SI::ind2Swap = i+int((*unifRnd)()*(SI::totalInputPoolSize-i));
//					swap(SI::randArray[i], SI::randArray[SI::ind2Swap]);
//					*Pre_Post_connect[SI::randArray[i]] += 1;				
//				}
//			}
//		} 
//	}
//}





//for (vector<float>::iterator i=(*timeVector).begin(); i != (*timeVector).end(); i++)
//{
//	
//	if ((*i) <= t) 
//	{
//		(*i) += (*expRnd)(); 
//		//if (t!=0){cout << i << "\t" << t << endl;}
//	}
//}



//	cout << S_ext_AMPA_GE[0] << endl;
//	*BGE_GE_connect[0] += 1;
//	cout << S_ext_AMPA_GE[0] << endl;	

//	cout << mypointer << endl;
//	 = &S_ext_AMPA_GE[0];	
//	cout << S_ext_AMPA_GE[0] << endl;
//	*mypointer += 1;
//	cout << S_ext_AMPA_GE[0] << endl;	

//	cout << S_ext_AMPA_GE[0] << &S_ext_AMPA_GE[0] << endl;
//	cout << S_ext_AMPA_GE[1] << &S_ext_AMPA_GE[1] << endl;




//	S_ext_AMPA_GE[0]=1;
//	S_ext_AMPA_GE[1]=2;
//	cout << S_ext_AMPA_GE[0] << S_ext_AMPA_GE[1] << endl;
//	S_ext_AMPA_GE *= 5;
//	cout << S_ext_AMPA_GE[0] << S_ext_AMPA_GE[1] << endl;
//	c = a + scalar_vector<double>(a.size(), 0.5);

//			if (t!=0){cout << i << "\t" << t << endl;}

//float** BGE_GE_connect;
//BGE_GE_connect = new float*[NE*BgPoolSize];
//int counter = 0;
//for (int i = 0; i < NE; i++)
//{
//	for (int j = 0; j < BgPoolSize; j++)
//	{
//		BGE_GE_connect[counter] = &S_ext_AMPA_GE[i];
//		counter += 1;
//	}
//}

//void propogate_BG_spikes(valarray<float> *timeVector, 
//						 float **Pre_Post_connect,
//						 variate_generator<mt19937&,exponential_distribution<float> > *expRnd)
//{
//	for (int i=0; i< (*timeVector).size(); i++)
//	{
//		while ((*timeVector)[i] <= SI::t) 
//		{
//			// This means a spike happened in this neuron! Increment...
//			(*timeVector)[i] += (*expRnd)();
//			
//			// And propogate:
//			if (SI::t!=0)
//			{
//				*Pre_Post_connect[i] += 1;
//			}
//		}
//	} 
//}

//void propogate_BG_spikes(valarray<float> *timeVector, 
//						 float **Pre_Post_connect,
//						 variate_generator<mt19937&,exponential_distribution<float> > *expRnd);

