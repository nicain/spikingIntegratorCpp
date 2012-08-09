#include <string>
#include "PoolBG.h"
#include "PoolRecHybrid.h"
#include "PoolRecEx.h"
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRecInh;

using namespace std;

const double PoolRecHybrid::VE;
const double PoolRecHybrid::VI;
const double PoolRecHybrid::Vm;
const double PoolRecHybrid::VMin;
const double PoolRecHybrid::VMax;
const double PoolRecHybrid::VReset;
const double PoolRecHybrid::alpha;
const double PoolRecHybrid::K;
double PoolRecHybrid::tau_GABA_Inv_times_dt = Brain::dt*(1.0/(PoolRecInh::tau_GABA));

PoolRecHybrid::PoolRecHybrid(string poolName_in, Brain &parentPool_in, int N_in): Pool(poolName_in, parentPool_in, N_in)
{
	construct();
};

void PoolRecHybrid::construct()
{
//	uniDist = new uniform_real_distribution<double>(0,1);
//	uniRnd = new variate_generator<mt19937&,uniform_real_distribution<double> >(parentBrain->myRNG, *uniDist);
	
	// Set member data:
//	V = new valarray<double>((double)0, N);
//	ISyn = new valarray<double>((double)0, N);
//    ISynBG = new valarray<double>((double)0, N);
//    ISynInput = new valarray<double>((double)0, N);
    
//    ISynRecAMPA = new valarray<double>((double)0, N);
//    ISynRecNMDA = new valarray<double>((double)0, N);
//    ISynRecGABA = new valarray<double>((double)0, N);
    
//    SInput = new valarray<double>((double)0, N);
//    SBG = new valarray<double>((double)0, N);
	
	// Initialize connection vectors, and weight vector:
	BG_Inputs_AMPA = new vector< valarray<double>* >;
	Ex_Inputs_AMPA = new vector<double*>;
	Ex_Inputs_AMPA_w = new vector<double>;
	Ex_Inputs_NMDA = new vector<double*>;
	Ex_Inputs_NMDA_w = new vector<double>;
	Inh_Inputs_GABA = new vector<double*>;
	
	// Create helper vectors:
//	unitVector = new valarray<double>((double)1, N);
//	VTmp = new valarray<double>((double)0, N);
//	thresholdTest = new valarray<bool>(false, N);
    
//    ISynPoolSum = 0;
//    ISynInputPoolSum = 0;
//    ISynBGPoolSum = 0;
//    ISynRecAMPASum = 0;
//    ISynRecNMDASum = 0;
//    ISynRecGABASum = 0;
    
    VAvgE = (VE - Vm)/1000; // converted to volts
    VAvgNMDA = VAvgE/(1 + exp(K*Vm)/3.57); // converted to volts
    VAvgI = (VI - Vm)/1000; // converted to volts

//    VAvgE = (-56.186)/1000; // converted to volts
//    VAvgNMDA = VAvgE/(1 + exp(K*Vm)/3.57); // converted to volts
//    VAvgI = (-13.0878)/1000; // converted to volts
    
    ITot = 0;
    
    GABA_pooled = 0;
    
    // Specific to Inh:
    gext_AMPA = PoolRecInh::gext_AMPA_I;
    grec_AMPA = PoolRecInh::grec_AMPA_I;
    gNMDA     = PoolRecInh::gNMDA_I;
    gGABA     = PoolRecInh::gGABA_I;
	
};

PoolRecHybrid::~PoolRecHybrid() 
{
//	delete V;
//	delete ISyn;
//    delete ISynBG;
//    delete ISynRecAMPA;
//    delete ISynRecNMDA;
//    delete ISynRecGABA;
	
	
//	delete uniRnd;
//	delete uniDist;
	
	delete BG_Inputs_AMPA;
	delete Ex_Inputs_AMPA;
	delete Ex_Inputs_AMPA_w;
	delete Ex_Inputs_NMDA;
	delete Ex_Inputs_NMDA_w;
	delete Inh_Inputs_GABA;
	
//	delete unitVector;
//	delete VTmp;
//	delete thresholdTest;
//    
//    delete SInput;
//    delete SBG;
	
};

void PoolRecHybrid::updateV()
{
    // Compute current coming into the cell
    ITot = 0;
    
	// First, the background pools:
	for (i = 0; i < (*BG_Inputs_AMPA).size(); i++)
	{
//        if ((*(this->parentBrain)).isBGInput((*BG_Inputs_AMPA)[i])) {
//            
//        } else 
//        {
//            
//        }
        ITot += -VAvgE *1000* gext_AMPA * 1920;//-VAvgE *1000* gext_AMPA * (*((*BG_Inputs_AMPA)[i])).sum(); // Isyn has to be in nA at all times
	}

    
    
	
	// Then, recurrent AMPA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_AMPA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_AMPA)[i])) * ((*Ex_Inputs_AMPA_w)[i]);
	}
    
//    cout << "STmp: " << STmp << endl;
    
	ITot += -VAvgE *1000*grec_AMPA * STmp*float(N);    
    

	// Next, recurrent NMDA:
	STmp = 0;
	for (i = 0; i < (*Ex_Inputs_NMDA).size(); i++)
	{
		STmp += (*((*Ex_Inputs_NMDA)[i])) * ((*Ex_Inputs_NMDA_w)[i]);
	}
    
    
	ITot += -VAvgNMDA *1000*gNMDA * STmp*float(N);
    
//    cout << "ITotRecHybrid: "<< ITot << "\t(" << - VAvgNMDA * 1000 << ")\t" << STmp << endl;

      
    

	// Finally, recurrent GABA:
	STmp = 0;
	for (i = 0; i < (*Inh_Inputs_GABA).size(); i++)
	{
		STmp += (*((*Inh_Inputs_GABA)[i]));
	}


	ITot += - VAvgI * 1000 * gGABA * STmp*float(N);
    
  
//    cout << "ITotRecHybrid: "<< ITot << "\t(" << - VAvgI * 1000 << ")\t" << STmp << endl;
    

    

//	
;
//	
//	// Update voltage:			
//	(*VTmp) = ((*V) - VMin * (*unitVector));	//TODO: optimize below:
//	
//	(*RK1) = -dt_times_gL_over_cm*(*VTmp) - dt_over_cm*(*ISyn);
//	(*RK2) = -dt_times_gL_over_cm*(*VTmp + .5*(*RK1)) - dt_over_cm*(*ISyn);
//	(*RK3) = -dt_times_gL_over_cm*(*VTmp + .5*(*RK2)) - dt_over_cm*(*ISyn);
//	(*RK4) = -dt_times_gL_over_cm*(*VTmp + (*RK3)) - dt_over_cm*(*ISyn);
//	
//	(*V) += (double(1)/double(6))*(   (*RK1) + double(2)*(*RK2) + double(2)*(*RK3) + (*RK4)    );
//	(*V) -= dt_times_gL_over_cm * (*VTmp) + dt_over_cm*(*ISyn);
}

void PoolRecHybrid::init()
{
    //	for (i=0; i<N; i++) 
    //	{
    //		(*V)[i] = (*uniRnd)()*5-55;
    //	};	
}

void PoolRecHybrid::propogate()
{	
    // Empty because this type of pool does not spike!
}

void PoolRecHybrid::updateS()
{
    // Use the phi function on current to get pooled gaba:
    
    // Linear regime coefficients:
    double b_L = -421.126874581926;
    double m_L = -3199963.26955727;
    
    // Exponential regime coefficients:
    double b_E = 0;
    double m_E = 0.000178584361655417;
    double a_E = -85967.1514767565;
    
    // Cutoff between the regimes:
    double M = -0.000143533420332;
    
    double phi;
    
    if (ITot < M) {
        
        // Linear:
        phi = b_L + m_L*ITot;
        
    } else {
        
        // Exponential:
         phi = b_E + m_E*exp(a_E*ITot);   
    }
    
    GABA_pooled += .2*Brain::dt*(-GABA_pooled + phi);
    
    cout << "GABA_pooled Hybrid: " << GABA_pooled   << "(" << ITot << ")" << endl;
    
    
}

void PoolRecHybrid::connectTo(PoolBG &BGPool_in)
{
	BG_Inputs_AMPA->push_back(BGPool_in.AMPA);
}

void PoolRecHybrid::connectTo(PoolRecEx &ExPool_in, double wIn)
{
	Ex_Inputs_AMPA->push_back(&(ExPool_in).AMPA_pooled);
	Ex_Inputs_AMPA_w->push_back(wIn);
	Ex_Inputs_NMDA->push_back(&(ExPool_in).NMDA_pooled);
	Ex_Inputs_NMDA_w->push_back(wIn);
}

void PoolRecHybrid::connectTo(PoolRecInh &InhPool_in)
{
	Inh_Inputs_GABA->push_back(&(InhPool_in).GABA_pooled);
}

void PoolRecHybrid::connectTo(PoolRecHybrid &InhPool_in)
{
	Inh_Inputs_GABA->push_back(&(InhPool_in).GABA_pooled);
}

//double* PoolRec::getStateLocation(int whichNeuron, State whichState) 
//{
//	
//	double *returnAddress;
//	
//	switch (whichState)
//	{
//		case S_V:
//			returnAddress = &((*V)[whichNeuron]);
//			break;
//		case S_ISyn:
//			returnAddress = &((*ISyn)[whichNeuron]);
//			break;
//		case S_ISynBG:
//			returnAddress = &((*ISynBG)[whichNeuron]);
//			break;
//		case S_ISynRecAMPA:
//			returnAddress = &((*ISynRecAMPA)[whichNeuron]);
//			break;
//		case S_ISynRecNMDA:
//			returnAddress = &((*ISynRecNMDA)[whichNeuron]);
//			break;
//		case S_ISynRecGABA:
//			returnAddress = &((*ISynRecGABA)[whichNeuron]);
//			break;          
//		case S_ISynInput:
//			returnAddress = &((*ISynInput)[whichNeuron]);
//			break;            
//            
//		default:
//			returnAddress = getStateLocationConductance(whichNeuron, whichState);
//	}
//	
//	return returnAddress;
//};
//
//double* PoolRec::getStateLocation(State whichState) 
//{
//	
//	double *returnAddress;
//	
//	switch (whichState)
//	{
//		case S_ISynPoolSum:
//			returnAddress = &ISynPoolSum;
//			break;         
//		case S_ISynInputPoolSum:
//			returnAddress = &ISynInputPoolSum;
//			break;   
//		case S_ISynBGPoolSum:
//			returnAddress = &ISynBGPoolSum;
//			break;
//		case S_ISynRecAMPASum:
//			returnAddress = &ISynRecAMPASum;
//			break;
//		case S_ISynRecNMDASum:
//			returnAddress = &ISynRecNMDASum;
//			break;
//		case S_ISynRecGABASum:
//			returnAddress = &ISynRecGABASum;
//            break;
//		case S_SBGSum:
//			returnAddress = &SBGSum;
//            break;
//		case S_SInputSum:
//			returnAddress = &SInputSum;
//			break;
//            
//            
//		default:
//			returnAddress = 0;
//	}
//	
//
//    
//	return returnAddress;
//};
//
//double* PoolRec::getStateLocationConductance(int whichNeuron, State whichState) 
//{
//	return 0;
//};
