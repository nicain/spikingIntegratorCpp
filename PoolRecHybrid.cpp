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


const double PoolRecHybrid::xVals[100] = {-0.00014285430399,-0.000142479167136667,-0.000142104030283333,-0.00014172889343,-0.000141353756576667,-0.000140978619723333,-0.00014060348287,-0.000140228346016667,-0.000139853209163333,-0.00013947807231,-0.000139102935456667,-0.000138727798603333,-0.00013835266175,-0.000137977524896667,-0.000137602388043333,-0.00013722725119,-0.000136852114336667,-0.000136476977483333,-0.00013610184063,-0.000135726703776667,-0.000135351566923333,-0.00013497643007,-0.000134601293216667,-0.000134226156363333,-0.00013385101951,-0.000133475882656667,-0.000133100745803333,-0.00013272560895,-0.000132350472096667,-0.000131975335243333,-0.00013160019839,-0.000131225061536667,-0.000130849924683333,-0.00013047478783,-0.000130099650976667,-0.000129724514123333,-0.00012934937727,-0.000128974240416667,-0.000128599103563333,-0.00012822396671,-0.000127848829856667,-0.000127473693003333,-0.00012709855615,-0.000126723419296667,-0.000126348282443333,-0.00012597314559,-0.000125598008736667,-0.000125222871883333,-0.00012484773503,-0.000124472598176667,-0.000124097461323333,-0.00012372232447,-0.000123347187616667,-0.000122972050763333,-0.00012259691391,-0.000122221777056667,-0.000121846640203333,-0.00012147150335,-0.000121096366496667,-0.000120721229643333,-0.00012034609279,-0.000119970955936667,-0.000119595819083333,-0.00011922068223,-0.000118845545376667,-0.000118470408523333,-0.00011809527167,-0.000117720134816667,-0.000117344997963333,-0.00011696986111,-0.000116594724256667,-0.000116219587403333,-0.00011584445055,-0.000115469313696667,-0.000115094176843333,-0.00011471903999,-0.000114343903136667,-0.000113968766283333,-0.00011359362943,-0.000113218492576667,-0.000112843355723333,-0.00011246821887,-0.000112093082016667,-0.000111717945163333,-0.00011134280831,-0.000110967671456667,-0.000110592534603333,-0.00011021739775,-0.000109842260896667,-0.000109467124043333,-0.00010909198719,-0.000108716850336667,-0.000108341713483333,-0.00010796657663,-0.000107591439776667,-0.000107216302923333,-0.00010684116607,-0.000106466029216667,-0.000106090892363333,-0.00010571575551};

const double PoolRecHybrid::yVals[100] = {34.7431171659,33.8213648723358,32.9075230555182,32.0025629629246,31.1074558420322,30.2231729403184,29.3506855052604,28.4909647843356,27.6449820250211,26.8137084747943,25.9981153811325,25.199173991513,24.417855553413,23.6551313143099,22.9119721906238,22.1885995724812,21.4828894744024,20.7922604029825,20.1141308648168,19.44562482671,18.7800403897734,18.1080020944313,17.422250220761,16.7269373583797,16.0299537341847,15.3401208452741,14.6698215532135,14.0322918920265,13.4405859109218,12.8926006382225,12.3599115344837,11.8123418300939,11.2448857899368,10.6808270137558,10.1449536122539,9.65783450468309,9.21706678325991,8.81246740986325,8.43384713040293,8.07101669078878,7.7137868369307,7.35395427494766,6.99241824630484,6.63268434428977,6.27825837481365,5.93264614378755,5.59935345712268,5.28188554056194,4.98274770305497,4.70139396999176,4.43669840215763,4.18753506033797,3.95277800531811,3.73130129788344,3.52197899881929,3.32368516891101,3.13529386894399,2.95571517279981,2.78433397273731,2.62087046299092,2.46505188458326,2.31660547853688,2.17525848587439,2.04073814761835,1.91277170479136,1.791086398416,1.67540946951484,1.56546815911048,1.46098970822549,1.36170135788246,1.26734977936838,1.17777953602546,1.09286595104693,1.01248435874267,0.936510093422582,0.864818489396538,0.797284880974436,0.733784602466161,0.674192988181603,0.618385372430653,0.566237089523193,0.517623473769118,0.472419859478312,0.430501580960667,0.391743972526069,0.356022368484407,0.323212103145571,0.293188510819446,0.265826925815925,0.241002682444892,0.21859111501624,0.198467557839854,0.180507345225623,0.164585811483437,0.150578290923183,0.138360117854751,0.127806626588027,0.118793151432902,0.111195026699264,0.104887586697};


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
    

    

    
    xMin = xVals[0];
    xDelta = xVals[1] - xVals[0];
	
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

//        ITot += -VAvgE *1000* gext_AMPA * (*((*BG_Inputs_AMPA)[i])).sum();
	}
        ITot += -VAvgE *1000* gext_AMPA * 2000;
    
    
	
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
    int xInd;
    double phi;
    
    if (ITot > xVals[99]){
        xInd = 98;
        ITot = xVals[99];
    } else {
        if (ITot < xVals[0]){
            xInd = 0;
            ITot = xVals[0];
        } else {
            xInd = floor((ITot-xVals[0])/xDelta);
        }
    };

    phi = yVals[xInd] +(yVals[xInd]-yVals[xInd+1])/(xVals[xInd]-xVals[xInd+1])*(ITot-xVals[xInd]);
    
    // For debugging:
//    cout << xVals[xInd] << "\t" <<  ITot << "\t" << xVals[xInd+1] << "\tX\t" << xInd << endl;
//    cout << yVals[xInd] << "\t" <<  phi << "\t" << yVals[xInd+1] << "\tY\t" << xInd << endl;
    



    // Apply a first-order filter:
    GABA_pooled += .2*Brain::dt*(-GABA_pooled + phi);
    
//    cout << "GABA_pooled Hybrid: " << GABA_pooled   << "(" << ITot << ")" << endl;
    
    cout << "ITotRecHybrid: "<< ITot << "\t" << GABA_pooled << endl;
    
    
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
