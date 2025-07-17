#include <vector>

// declaración
extern double mp, mE; // masa de la particula y la elipse
extern double sc;
extern double a, b; // semiejes de la elipe
extern double Ie; // momento de inercia de la elipse

double Power(double x, int n);

double Power(double x, double y);

double sech(double x); // secante hiperbolica

double Sqrt(double x);

std::vector<double> f(double t, const std::vector<double>& QP);