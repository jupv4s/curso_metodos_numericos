#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

#include <functional>

class NumericalIntegrator
{
private:
    double posicion_inicial; // Límite inferior 'a'
    double posicion_final;   // Límite superior 'b'

public:
    // Constructor para establecer los límites [a, b]
    NumericalIntegrator(double a, double b);
    
    // Método de la Regla del Trapecio (requerido por Romberg)
    double Trapecio(std::function<double(double)> f, int n_subintervals);
    
    // Método de Romberg
    double Romberg(std::function<double(double)> f, int n_iterations);
};

#endif