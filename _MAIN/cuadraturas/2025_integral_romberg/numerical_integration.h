#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

#include <iostream>   // Para std::cerr
#include <functional> // Para std::function

class NumericalIntegrator
{
private:
    double posicion_inicial;
    double posicion_final;

public:
    NumericalIntegrator();
    ~NumericalIntegrator();
    NumericalIntegrator(double a, double b);

    double Trapecio(std::function<double(double)> f, int n_subintervals);
    double Romberg(std::function<double(double)> f, double tolerance);
};

#endif
