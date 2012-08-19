#include <string>
#include "PoolBG.h"
#include "PoolRecHybridEx.h"
#include "PoolRecEx.h"
#include "PoolRecInh.h"
#include "Brain.h"

class Brain;
class Pool;
class PoolRecInh;
class PoolRecEx;

using namespace std;

const double PoolRecHybridEx::VE;
const double PoolRecHybridEx::VI;
const double PoolRecHybridEx::Vm;
const double PoolRecHybridEx::VMin;
const double PoolRecHybridEx::VMax;
const double PoolRecHybridEx::VReset;
const double PoolRecHybridEx::alpha;
const double PoolRecHybridEx::K;
//double PoolRecHybridEx::tau_GABA_Inv_times_dt = Brain::dt*(1.0/(PoolRecInh::tau_GABA));


const double PoolRecHybridEx::xVals[100] = {-0.000503731837245,-0.000502424224158626,-0.000501116611072252,-0.000499808997985879,-0.000498501384899505,-0.000497193771813131,-0.000495886158726758,-0.000494578545640384,-0.00049327093255401,-0.000491963319467636,-0.000490655706381263,-0.000489348093294889,-0.000488040480208515,-0.000486732867122141,-0.000485425254035768,-0.000484117640949394,-0.00048281002786302,-0.000481502414776646,-0.000480194801690273,-0.000478887188603899,-0.000477579575517525,-0.000476271962431151,-0.000474964349344778,-0.000473656736258404,-0.00047234912317203,-0.000471041510085657,-0.000469733896999283,-0.000468426283912909,-0.000467118670826535,-0.000465811057740162,-0.000464503444653788,-0.000463195831567414,-0.00046188821848104,-0.000460580605394667,-0.000459272992308293,-0.000457965379221919,-0.000456657766135545,-0.000455350153049172,-0.000454042539962798,-0.000452734926876424,-0.00045142731379005,-0.000450119700703677,-0.000448812087617303,-0.000447504474530929,-0.000446196861444556,-0.000444889248358182,-0.000443581635271808,-0.000442274022185434,-0.000440966409099061,-0.000439658796012687,-0.000438351182926313,-0.000437043569839939,-0.000435735956753566,-0.000434428343667192,-0.000433120730580818,-0.000431813117494444,-0.000430505504408071,-0.000429197891321697,-0.000427890278235323,-0.000426582665148949,-0.000425275052062576,-0.000423967438976202,-0.000422659825889828,-0.000421352212803455,-0.000420044599717081,-0.000418736986630707,-0.000417429373544333,-0.00041612176045796,-0.000414814147371586,-0.000413506534285212,-0.000412198921198838,-0.000410891308112465,-0.000409583695026091,-0.000408276081939717,-0.000406968468853343,-0.00040566085576697,-0.000404353242680596,-0.000403045629594222,-0.000401738016507848,-0.000400430403421475,-0.000399122790335101,-0.000397815177248727,-0.000396507564162353,-0.00039519995107598,-0.000393892337989606,-0.000392584724903232,-0.000391277111816859,-0.000389969498730485,-0.000388661885644111,-0.000387354272557737,-0.000386046659471364,-0.00038473904638499,-0.000383431433298616,-0.000382123820212242,-0.000380816207125869,-0.000379508594039495,-0.000378200980953121,-0.000376893367866747,-0.000375585754780374,-0.000374278141694};

const double PoolRecHybridEx::yVals[100] = {15.3541301232,14.8679881442503,14.3736726282886,13.8736870381756,13.3705348367721,12.8667194869388,12.3647444515364,11.8671131934258,11.3763291754676,10.8948958605226,10.4253167114515,9.9700951911152,9.5317347623743,9.1127388880896,8.71532465858756,8.33792118899524,7.9762765707967,7.62608223747675,7.28302962252022,6.94350821273625,6.60744114490181,6.27586706290349,5.94985411853424,5.63114685228497,5.32217039679554,5.02537980672302,4.74314888553163,4.47705760289664,4.22823730305903,3.9965607197977,3.77574936878835,3.55764102701853,3.3347859829491,3.109191150441,2.88956921204048,2.68476942207309,2.50059364253234,2.33480434445173,2.18385372864129,2.04419399591104,1.91227734707102,1.78458805875247,1.65933743394292,1.53730587596621,1.41948265084965,1.30685702462057,1.20041826330629,1.10111393793079,1.00916767470258,0.924189738312024,0.84577100577745,0.773502354117191,0.706974660349577,0.645778801492943,0.589505654565614,0.537746096585923,0.490091004572203,0.446143136665144,0.405633995995889,0.368373902577822,0.334174311869343,0.30284667932885,0.274202460414745,0.248053110585424,0.224210085299287,0.202484840014733,0.182688830190161,0.164633511283972,0.148130338754563,0.132991118991032,0.119070303112195,0.106305262777859,0.0946429103985577,0.0840301583848258,0.0744139191471983,0.0657411050962099,0.0579586286423951,0.0510134021962891,0.0448523381684258,0.0394223489693402,0.034670347009567,0.0305432446996409,0.0269879544500966,0.0239513886714687,0.0213804597742918,0.0192220801691006,0.0174231622664298,0.0159306184768141,0.0146913612107881,0.0136523028788866,0.012760355891644,0.0119624326595953,0.0112054455932749,0.0104363071032177,0.00960192959995811,0.00864922549403102,0.00752510719597098,0.00617648711631268,0.0045502776655908,0.00259339125434};


PoolRecHybridEx::PoolRecHybridEx(string poolName_in, Brain &parentPool_in, int N_in): Pool(poolName_in, parentPool_in, N_in)
{
	construct();
};

void PoolRecHybridEx::construct()
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
    
    AMPA_pooled = 0;
    NMDA_pooled = 0;
    

    gext_AMPA = PoolRecEx::gext_AMPA_E;
    grec_AMPA = PoolRecEx::grec_AMPA_E;
    gNMDA     = PoolRecEx::gNMDA_E;
    gGABA     = PoolRecEx::gGABA_E;
    

    

    
    xMin = xVals[0];
    xDelta = xVals[1] - xVals[0];
	
};

PoolRecHybridEx::~PoolRecHybridEx() 
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

void PoolRecHybridEx::updateV()
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

        ITot += -VAvgE *1000* gext_AMPA * (*((*BG_Inputs_AMPA)[i])).sum();
//        ITot += -VAvgE *1000* gext_AMPA *40;
        
        ITot -= -VAvgE *1000* gext_AMPA *50; // 160 not enough, 170 maybe too much
	}
    

    

    
//        ITot += -VAvgE *1000* gext_AMPA * 2000;
    
    
	
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
    
    cout << "ITotHybrid: "<< ITot << endl;
    

//    cout << "ITotHybrid: "<< ITot << endl;    
  
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

void PoolRecHybridEx::init()
{
    //	for (i=0; i<N; i++) 
    //	{
    //		(*V)[i] = (*uniRnd)()*5-55;
    //	};	
}

void PoolRecHybridEx::propogate()
{	
    // Empty because this type of pool does not spike!
}

void PoolRecHybridEx::updateS()
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
    AMPA_pooled += .2*Brain::dt*(-AMPA_pooled + phi);
    NMDA_pooled += Brain::dt*( -.01*NMDA_pooled + PoolRecEx::alpha*AMPA_pooled + PoolRecEx::alpha*AMPA_pooled*NMDA_pooled/N  );  //
    
    //	(*NMDA) = one_minus_tau_NMDA_Inv_times_dt*(*NMDA) + alpha_times_dt*(*X)*(*unitVector - (*NMDA));
    
//    cout << "PHI Hybrid: " << phi << endl;
    
    cout << "AMPA_pooled: "<< AMPA_pooled << endl;
    
    
}

void PoolRecHybridEx::connectTo(PoolBG &BGPool_in)
{
	BG_Inputs_AMPA->push_back(BGPool_in.AMPA);
}

void PoolRecHybridEx::connectTo(PoolRecEx &ExPool_in, double wIn)
{
	Ex_Inputs_AMPA->push_back(&(ExPool_in).AMPA_pooled);
	Ex_Inputs_AMPA_w->push_back(wIn);
	Ex_Inputs_NMDA->push_back(&(ExPool_in).NMDA_pooled);
	Ex_Inputs_NMDA_w->push_back(wIn);
}

void PoolRecHybridEx::connectTo(PoolRecInh &InhPool_in)
{
	Inh_Inputs_GABA->push_back(&(InhPool_in).GABA_pooled);
}

void PoolRecHybridEx::connectTo(PoolRecHybridEx &ExPool_in, double wIn)
{
	Ex_Inputs_AMPA->push_back(&(ExPool_in).AMPA_pooled);
	Ex_Inputs_AMPA_w->push_back(wIn);
	Ex_Inputs_NMDA->push_back(&(ExPool_in).NMDA_pooled);
	Ex_Inputs_NMDA_w->push_back(wIn);
}

double* PoolRecHybridEx::getStateLocation(State whichState) 
{
	double *returnAddress;
	
	switch (whichState)
	{
		case S_AMPA_pooled:
			returnAddress = &AMPA_pooled;
			break;
		case S_NMDA_pooled:
			returnAddress = &NMDA_pooled;
			break;
		default:
			cout << "Unacceptable Monitor variable." << endl;
			exit(-1);
	}
	
	return returnAddress;
};

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
