#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

#include <iostream>   // Para std::cerr (salida básica de errores)
#include <functional> // Para std::function

// Clase para integración numérica
class NumericalIntegrator
{
private:
    double posicion_inicial; // Límite inferior de integración 'a'
    double posicion_final;   // Límite superior de integración 'b'

public:
    // Constructor por defecto
    NumericalIntegrator();
    // Destructor
    ~NumericalIntegrator();
    
    // Constructor parametrizado: establece los límites de integración [a, b]
    NumericalIntegrator(double a, double b);
    
    // n: Número de subintervalos, iteraciones o muestras, según el método.

    // Regla del Trapecio Compuesta
    double Trapecio(std::function<double(double)> f, int n_subintervals);
    
    // Método de Romberg
    // tolerance: La tolerancia deseada para la convergencia.
    double Romberg(std::function<double(double)> f, double tolerance);
};

#endif
