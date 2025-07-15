#include "numerical_integration.h" // Incluye la declaración de la clase
#include "Matrix.h"                     // Se asume que tu clase Matrix<T> está disponible aquí.
#include <cmath>                        // Para std::pow, etc.
#include <random>                       // Para std::random_device, std::mt19937, std::uniform_real_distribution
#include <limits>                       // Para std::numeric_limits<double>::quiet_NaN()

// Constructor por defecto
NumericalIntegrator::NumericalIntegrator() : posicion_inicial(0.0), posicion_final(1.0) {}

// Destructor
NumericalIntegrator::~NumericalIntegrator() {}

// Constructor parametrizado
NumericalIntegrator::NumericalIntegrator(double a, double b) : posicion_inicial(a), posicion_final(b) 
{
    if (a >= b) {
        std::cerr << "Advertencia: El límite inicial debe ser menor que el límite final. Intercambiando valores." << std::endl;
        posicion_inicial = b;
        posicion_final = a;
    }
}

// Regla del Trapecio Compuesta
double NumericalIntegrator::Trapecio(std::function<double(double)> f, int n_subintervals)
{
    if (n_subintervals <= 0) {
        std::cerr << "Error: El número de subintervalos debe ser positivo para el método del Trapecio." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double h = (posicion_final - posicion_inicial) / n_subintervals;
    double resultado = 0.5 * (f(posicion_inicial) + f(posicion_final));

    for (int i = 1; i < n_subintervals; ++i) {
        resultado += f(posicion_inicial + i * h);
    }
    return resultado * h;
}

// Regla de Simpson 1/3 Compuesta
double NumericalIntegrator::Simpson(std::function<double(double)> f, int n_subintervals)
{
    if (n_subintervals <= 0 || n_subintervals % 2 != 0) {
        std::cerr << "Error: El número de subintervalos debe ser par y positivo para el método de Simpson." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double h = (posicion_final - posicion_inicial) / n_subintervals;
    double resultado = f(posicion_inicial) + f(posicion_final);

    for (int i = 1; i < n_subintervals; ++i) {
        double x = posicion_inicial + i * h;
        if (i % 2 == 1) { // Puntos impares (multiplicar por 4)
            resultado += 4 * f(x);
        } else { // Puntos pares (multiplicar por 2)
            resultado += 2 * f(x);
        }
    }
    return resultado * (h / 3.0);
}

// Método de Romberg
double NumericalIntegrator::Romberg(std::function<double(double)> f, int n_iterations)
{
    if (n_iterations <= 0) {
        std::cerr << "Error: El número de iteraciones para Romberg debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Se asume que Matrix<double> está disponible
    Matrix<double> R(n_iterations, n_iterations); 

    // Calcula la primera columna de la tabla de Romberg usando la regla del trapecio
    // R(k, 0) = T_k (donde T_k es la aproximación del trapecio con 2^k subintervalos)
    for (int k = 0; k < n_iterations; ++k) {
        R(k, 0) = Trapecio(f, static_cast<int>(std::pow(2, k)));
    }

    // Extrapolación de Richardson para las columnas subsiguientes
    for (int j = 1; j < n_iterations; ++j) {
        for (int k = 0; k < n_iterations - j; ++k) {
            R(k, j) = R(k + 1, j - 1) + (R(k + 1, j - 1) - R(k, j - 1)) / (std::pow(4, j) - 1);
        }
    }
    // Para esta implementación particular de Romberg, el valor final suele estar en R(0, n_iterations - 1)
    return R(0, n_iterations - 1);
}

// Cuadratura de Gauss-Legendre (fija a 3 puntos)
double NumericalIntegrator::GaussLegendre(std::function<double(double)> f)
{
    // Nodos y pesos de Gauss-Legendre de 3 puntos para el intervalo [-1, 1]
    const double p1 = -0.7745966692;
    const double p2 = 0.0;
    const double p3 = 0.7745966692;
    
    const double w1 = 0.5555555556;
    const double w2 = 0.8888888889;
    const double w3 = 0.5555555556;

    // Transformar límites [a, b] al intervalo normalizado [-1, 1]
    double escala = (posicion_final - posicion_inicial) / 2.0;
    double desplazamiento = (posicion_final + posicion_inicial) / 2.0;

    return escala * (w1 * f(escala * p1 + desplazamiento) +
                     w2 * f(escala * p2 + desplazamiento) +
                     w3 * f(escala * p3 + desplazamiento));
}

// Integración por Montecarlo
double NumericalIntegrator::Montecarlo(std::function<double(double)> f, int n_samples)
{
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras debe ser positivo para el método de Montecarlo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(posicion_inicial, posicion_final);

    double sum_f_values = 0.0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = dis(gen);
        sum_f_values += f(x_rand);
    }

    double integral_approx = (posicion_final - posicion_inicial) * (sum_f_values / n_samples);
    return integral_approx;
}
