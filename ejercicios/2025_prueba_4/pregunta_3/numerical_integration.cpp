#include "numerical_integration.h"
#include "Matrix.h" // La integración de Romberg necesita la clase Matrix
#include <cmath>    // Para std::pow
#include <iostream> // Para std::cerr
#include <limits>   // Para std::numeric_limits

// Constructor parametrizado
NumericalIntegrator::NumericalIntegrator(double a, double b) : posicion_inicial(a), posicion_final(b) 
{
    if (a >= b)
    {
        std::cerr << "Advertencia: El límite inicial debe ser menor que el final. Intercambiando valores." << std::endl;
        posicion_inicial = b;
        posicion_final = a;
    }
}

// Regla del Trapecio Compuesta
double NumericalIntegrator::Trapecio(std::function<double(double)> f, int n_subintervals)
{
    if (n_subintervals <= 0)
    {
        std::cerr << "Error: El número de subintervalos debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double h = (posicion_final - posicion_inicial) / n_subintervals;
    double resultado = 0.5 * (f(posicion_inicial) + f(posicion_final));

    for (int i = 1; i < n_subintervals; ++i)
    {
        resultado += f(posicion_inicial + i * h);
    }
    return resultado * h;
}

// Método de Romberg
double NumericalIntegrator::Romberg(std::function<double(double)> f, int n_iterations)
{
    if (n_iterations <= 0)
    {
    
        std::cerr << "Error: El número de iteraciones para Romberg debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    Matrix<double> R(n_iterations, n_iterations); 

    // R(k, 0) usa la regla del trapecio con 2^k subintervalos
    for (int k = 0; k < n_iterations; ++k)
    {
        R(k, 0) = Trapecio(f, static_cast<int>(std::pow(2, k)));
    }

    // Extrapolación de Richardson
    for (int j = 1; j < n_iterations; ++j)
    {
        for (int k = 0; k < n_iterations - j; ++k)
        {
            double term_superior = R(k + 1, j - 1);
            double term_inferior = R(k, j - 1);
            R(k, j) = term_superior + (term_superior - term_inferior) / (std::pow(4, j) - 1);
        }
    }

    // El resultado más preciso se encuentra en la esquina superior derecha de la submatriz utilizada
    return R(0, n_iterations - 1);
}