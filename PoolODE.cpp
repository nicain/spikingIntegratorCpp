#include <string>
#include "Brain.h"
#include "PoolODE.h"

class Pool;

using namespace boost::random;
using namespace std;



PoolODE::PoolODE(string poolName_in,
						 Brain &parentPool_in,
						 double tMax_in,
						 double a_in, double b_in, double c_in, 
				         double Jii_in, double Jij_in, double JAij_in,double JAin_in, double JAbg_in,
						 double tau_in, double gamma_in, double I0_in
						 
							): Pool(poolName_in, 
												 parentPool_in, 
												 0, 
												 0)

{
	construct(parentPool_in,tMax_in,a_in,b_in,c_in,Jii_in,Jij_in,JAij_in,JAin_in,JAbg_in,tau_in,gamma_in,I0_in);
};

void PoolODE::construct(Brain &parentPool_in, double tmax_in, double a_in, 
						double b_in, double c_in, double Jii_in, double Jij_in, double JAij_in, double JAin_in, double JAbg_in, 
						double tau_in, double gamma_in,double I0_in)
{	
	// Set member data:
	dt = parentPool_in.dt*0.001;
	tmax = tmax_in;
	a = a_in;
	b = b_in;
	c = c_in;
	Jii = Jii_in;
	Jij = Jij_in;
	JAij = JAij_in;
	JAin = JAin_in;
	JAbg = JAbg_in;
	tau = tau_in;
	gamma = gamma_in;
	I0 = I0_in;
	L = (double)(tmax/dt);
	
	// Set vector lengths:
	S1.resize(L,0);
	X1.resize(L,0);
	F1.resize(L,0);
	S2.resize(L,0);
	X2.resize(L,0);
	F2.resize(L,0);
	
};

void PoolODE::init()
{

	S1[0] = 0;
	S2[0] = 0;
	
}

void PoolODE::run(PoolPoisson &I1, PoolPoisson &IBG1, PoolPoisson &I2, PoolPoisson &IBG2)
{
	T = 1;
	for ( t=dt; t <= tmax; t = t + dt)
	{
		X1[T-1] = JAin*I1.spks[T-1] + JAbg*IBG1.spks[T-1] + I0 + Jii*S1[T-1] - Jij*S2[T-1];
		X2[T-1] = JAin*I2.spks[T-1] + JAbg*IBG2.spks[T-1] + I0 + Jii*S2[T-1] - Jij*S1[T-1];
		
		F1[T-1] = phi(X1[T-1],X2[T-1])-gaus(X1[T-1]);
		F2[T-1] = phi(X2[T-1],X1[T-1])-gaus(X2[T-2]);
		 
		S1[T] = S1[T-1] + dt*( -S1[T-1]/tau + gamma*(1-S1[T-1])*F1[T-1] );
		S2[T] = S2[T-1] + dt*( -S2[T-1]/tau + gamma*(1-S2[T-1])*F2[T-1] );
		T++;
	}
		 
};

void PoolODE::run2(PoolPoisson &I1, PoolPoisson &IBG1, PoolPoisson &I2, PoolPoisson &IBG2,PoolPoisson &I01,PoolPoisson &I02)
{
	T = 1;
	for ( t=dt; t <= tmax; t = t + dt)
	{
		X1[T-1] = JAin*I1.spks[T-1] + JAbg*IBG1.spks[T-1] - JAbg*I01.spks[T-1] + I0 + Jii*S1[T-1] + Jij*S2[T-1];
		X2[T-1] = JAin*I2.spks[T-1] + JAbg*IBG2.spks[T-1] - JAbg*I02.spks[T-1] + I0 + Jii*S2[T-1] + Jij*S1[T-1];
		
		F1[T-1] = phi(X1[T-1],X2[T-1]);
		F2[T-1] = phi(X2[T-1],X1[T-1]);
		
		S1[T] = S1[T-1] + dt*(-S1[T-1]/tau + gamma*(1-S1[T-1])*F1[T-1]);
		S2[T] = S2[T-1] + dt*(-S2[T-1]/tau + gamma*(1-S2[T-1])*F2[T-1]);
		T++;
	}
	
};

void PoolODE::run3(PoolPoisson &I1, PoolPoisson &IBG1, PoolPoisson &I2, PoolPoisson &IBG2,PoolPoisson &INS1,PoolPoisson &INH1,PoolPoisson &INS2,PoolPoisson &INH2)
{
	T = 1;
	for ( t=dt; t <= tmax; t = t + dt)
	{
		X1[T-1] = JAin*I1.spks[T-1] + JAbg*IBG1.spks[T-1] + JAbg*INS1.spks[T-1] - JAbg*INH1.spks[T-1] + Jii*S1[T-1] + Jij*S2[T-1] + I0;
		X2[T-1] = JAin*I2.spks[T-1] + JAbg*IBG2.spks[T-1] + JAbg*INS2.spks[T-1] - JAbg*INH2.spks[T-1] + Jii*S2[T-1] + Jij*S1[T-1] + I0;
		
		F1[T-1] = phi(X1[T-1],X2[T-1]);
		F2[T-1] = phi(X2[T-1],X1[T-1]);
		
		S1[T] = S1[T-1] + dt*(-S1[T-1]/tau + gamma*(1-S1[T-1])*F1[T-1]);
		S2[T] = S2[T-1] + dt*(-S2[T-1]/tau + gamma*(1-S2[T-1])*F2[T-1]);
		T++;
	}
	
};

PoolODE::~PoolODE() 
{
	
	
};




void PoolODE::propogate() 
{
	
	
	
};

double PoolODE::phi(double x1, double x2)
{
	double nom = a*x1 - fA(x2) - b;
	double denom = 1 - exp(    -c * ( a*x1 - fA(x2) - b )  );
	return nom/denom;

};


double PoolODE::fA(double x)
{
	
	if(x < 0.4)
	{ 
		return 0;
	}
	else
	{
		return JAij*(-276*x + 106);
	}	
	
	
};

double PoolODE::gaus(double x)
{
	double bla[3] ={ 1.8951, 110.1119, 0.4};
	
	return bla[0]*exp(-bla[1]*(x-bla[2])*(x-bla[2]));

};