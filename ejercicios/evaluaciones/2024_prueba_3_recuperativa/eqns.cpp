#include "eqns.h"
#include <vector>
#include <cmath>

// implementacion
double mp = 3, mE = 1; // masa de la particula y la elipse
double sc = 200; 

double a = 0.5, b = 1; // semiejes de la elipe

double Ie = (a*a+b*b)*mE; // momento de inercia de la elipse

double Power(double x, int n)
{
  double rs = 1;

  for (int i = 1; i <= n; ++i)
    rs *= x;
  return rs;
}

double Power(double x, double y)
{
  return pow(x,y);
}

double sech(double x) // secante hiperbolica
{
  return 1./cosh(x);
}

double Sqrt(double x)
{
  return sqrt(x);
}

std::vector<double> f(double t, const std::vector<double>& QP)
{
  std::vector<double> dQP(10);
  double x, y, vx, vy, xo, yo, vox, voy, f, vf; 
  
  // variables de la particula
  x  = QP[0];
  vx = QP[1];
  y  = QP[2];
  vy = QP[3];
  
  // variables de la elipse
  xo  = QP[4];
  vox = QP[5];
  yo  = QP[6];
  voy = QP[7];
  
  // con f = phi y vf = dphi
  f    = QP[8];
  vf   = QP[9];
  
  double pa2   = Power(a,2);
  double pb2   = Power(b,2);
  double ps2   = Power(sin(f),2);
  double pc2   = Power(cos(f),2);
  double cf    = cos(f);
  double sf    = sin(f);
  double scxy  = Power(-(sf*(x - xo)) + cf*(y - yo),2);
  double csxy  = Power(cf*(x - xo) + sf*(y - yo),2);
  double psec2 = Power(sech(1000*(1 - scxy/pb2 - csxy/pa2)),2);
  double pech2 = Power(sech(1000*(1.2 - scxy/pb2 - csxy/pa2)),2);
  
  
  dQP[0] = vx;
  dQP[1] = (-1000*(psec2 - pech2)*((pb2*pc2 + pa2*ps2)*(x - xo) - ((pa2 - pb2)*cf*sf + (pa2 - pb2)*cf*sf)*(y - yo)))/(pa2*pb2*mp);
  
  dQP[2] = vy;
  dQP[3] = (-1000*(psec2 - pech2)*(-(pa2*cf*sf - pb2*cf*sf)*(x - xo) + (pa2*pc2 + pb2*ps2)*(y - yo)))/(pa2*pb2*mp);
  
  dQP[4] = vox;
  dQP[5] = (1000*(psec2 - pech2)*((pb2*pc2 + pa2*ps2)*(x - xo) - (pa2 - pb2)*cf*sf*(y - yo)))/(pa2*pb2*mE);

  dQP[6] = voy;
  dQP[7] = (1000*(psec2 - pech2)*(-(pa2 - pb2)*cf*sf*(x - xo) + (pa2*pc2 + pb2*ps2)*(y - yo)))/(pa2*pb2*mE);

  dQP[8] = vf;
  dQP[9] = (-1000*(pa2 - pb2)*(psec2 - pech2)*(sf*(x - xo) - cf*(y - yo))*(cf*(x - xo) + sf*(y - yo)))/(pa2*pb2*Ie);


  return dQP;
}