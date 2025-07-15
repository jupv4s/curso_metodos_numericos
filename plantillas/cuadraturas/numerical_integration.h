#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

#include <iostream>   // Para std::cerr (salida básica de errores)
#include <functional> // ¡MUY IMPORTANTE! Para std::function

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
    
    // Métodos de integración:
    // f: Ahora es std::function<double(double)> para mayor flexibilidad
    // n: Número de subintervalos, iteraciones o muestras, según el método.

    // Regla del Trapecio Compuesta
    double Trapecio(std::function<double(double)> f, int n_subintervals);
    
    // Regla de Simpson 1/3 Compuesta
    // n_subintervals debe ser par.
    double Simpson(std::function<double(double)> f, int n_subintervals);
    
    // Método de Romberg
    // n_iterations: Número de niveles de refinamiento.
    double Romberg(std::function<double(double)> f, int n_iterations);
    
    // Cuadratura de Gauss-Legendre (implementación fija a 3 puntos)
    double GaussLegendre(std::function<double(double)> f);

    // Integración por Montecarlo
    double Montecarlo(std::function<double(double)> f, int n_samples);
};

#endif // NUMERICAL_INTEGRATION_H
