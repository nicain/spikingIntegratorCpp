#include <string>
#include "PoolBG.h"
#include "PoolRec.h"
#include "PoolRecEx.h"
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;

using namespace std;

const float PoolRec::VE;
const float PoolRec::VI;
const float PoolRec::VMin;
const float PoolRec::VMax;
const float PoolRec::VReset;
const float PoolRec::alpha;
const float PoolRec::K;

PoolRec::PoolRec(string poolName_in, Brain &parentPool_in, int N_in): Pool(poolName_in, parentPool_in, N_in)
{
	construct();
};

PoolRec::PoolRec(string poolName_in, Brain &parentPool_in, int N_in, bool recordSpikes_in): Pool(poolName_in, parentPool_in, N_in, recordSpikes_in)
{
	construct();
};

void PoolRec::construct()
{
	uniDist = new uniform_real_distribution<float>(0,1);
	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<float> >(parentBrain->myRNG, *uniDist);
	
	// Set member data:
	V = new valarray<float>((float)0, N);
	ISyn = new valarray<float>((float)0, N);
	
	// Initialize connection vectors, and weight vector:
	BG_Inputs_AMPA = new vector< valarray<float>* >;
	Ex_Inputs_AMPA = new vector<float*>;
	Ex_Inputs_AMPA_w = new vector<float>;
	Ex_Inputs_NMDA = new vector<float*>;
	Ex_Inputs_NMDA_w = new vector<float>;
	Inh_Inputs_GABA = new vector<float*>;
	
	// Create helper vectors:
	unitVector = new valarray<float>((float)1, N);
	VTmp = new valarray<float>((float)0, N);
	thresholdTest = new valarray<bool>(false, N);
	
};

PoolRec::~PoolRec() 
{
	delete V;
	delete ISyn;
	
	delete uniRnd;
	delete uniDist;
	
	delete BG_Inputs_AMPA;
	delete Ex_Inputs_AMPA;
	delete Ex_Inputs_AMPA_w;
	delete Ex_Inputs_NMDA;
	delete Ex_Inputs_NMDA_w;
	delete Inh_Inputs_GABA;
	
	delete unitVector;
	delete VTmp;
	delete thresholdTest;
	
};

void PoolRec::updateV()
{
	//	 Compute current coming into the cell:
	(*ISyn) = valarray<float>((float)0, N);
	(*VTmp) = ((*V) - VE * (*unitVector));
	
	// First, the background pools:
	for (i = 0; i < (*BG_Inputs_AMPA).size(); i++)
	{
		(*ISyn) += gext_AMPA * (*VTmp) * (*((*BG_Inputs_AMPA)[i]));
	}
	
	// Then, recurrent AMPA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_AMPA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_AMPA)[i])) * ((*Ex_Inputs_AMPA_w)[i]);
		
	}
	(*ISyn) += grec_AMPA * STmp * (*VTmp);
	
	// Next, recurrent NMDA:
	(*VTmp) /= (*unitVector) + exp(K*(*V))/3.57;
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_NMDA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_NMDA)[i])) * ((*Ex_Inputs_NMDA_w)[i]);
	}
	(*ISyn) += gNMDA * STmp * (*VTmp);
	
	// Finally, recurrent GABA:
	(*VTmp) = ((*V) - VI * (*unitVector));
	STmp = 0;
	for (i = 0; i < (*Inh_Inputs_GABA).size(); i++)
	{
		STmp += (*((*Inh_Inputs_GABA)[i]));
	}
	(*ISyn) += gGABA * STmp * (*VTmp);
	
	// Update voltage:			
	(*VTmp) = ((*V) - VMin * (*unitVector));	//TODO: optimize below:
	(*V) -= dt_times_gL_over_cm * (*VTmp) + dt_over_cm*(*ISyn);
}

void PoolRec::init()
{
	for (i=0; i<N; i++) 
	{
		(*V)[i] = (*uniRnd)()*5-55;
	};	
}

void PoolRec::connectTo(PoolBG &BGPool_in)
{
	BG_Inputs_AMPA->push_back(BGPool_in.AMPA);
}

void PoolRec::connectTo(PoolRecEx &ExPool_in, float wIn)
{
	Ex_Inputs_AMPA->push_back(&(ExPool_in).AMPA_pooled);
	Ex_Inputs_AMPA_w->push_back(wIn);
	Ex_Inputs_NMDA->push_back(&(ExPool_in).NMDA_pooled);
	Ex_Inputs_NMDA_w->push_back(wIn);
}

void PoolRec::connectTo(PoolRecInh &InhPool_in)
{
	Inh_Inputs_GABA->push_back(&(InhPool_in).GABA_pooled);
}
