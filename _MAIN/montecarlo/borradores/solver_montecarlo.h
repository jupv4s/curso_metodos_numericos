#ifndef MONTE_CARLO_ESTIMATOR_H
#define MONTE_CARLO_ESTIMATOR_H

#include <iostream>     // Para std::cerr, std::cout
#include <vector>       // Para std::vector si fuera necesario para funciones auxiliares
#include <string>       // Para mensajes de error
#include <cmath>        // Para std::cos, std::sin, std::sqrt, std::atan2, std::pow
#include <random>       // Para std::random_device, std::mt19937, std::uniform_real_distribution
#include <limits>       // Para std::numeric_limits<double>::quiet_NaN
#include <iomanip>      // Para std::setprecision, std::fixed

// Definir PI si no está en cmath (depende del compilador y el estándar C++)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Un alias de tipo para una función que toma un double y retorna un double (para integración 1D)
using Function1DType = double (*)(double);

// Un alias de tipo para una función que toma dos doubles y retorna un bool (para área 2D)
using Predicate2DType = bool (*)(double, double);

// Clase para realizar estimaciones usando el método de Montecarlo
class MonteCarloEstimator
{
private:
    // Límites del espacio de muestreo (el "recuadro")
    double x_min, x_max;
    double y_min, y_max;

    std::mt19937 random_engine; // Motor de números aleatorios

    // Almacenará el resultado del cálculo
    double estimated_result;
    std::string description; // Descripción de lo que se calculó

    // Método privado para generar un número aleatorio uniforme dentro de un rango
    double getRandomNumber(double min_val, double max_val);

    // --- Funciones auxiliares para el problema de la figura polar ---
    int param_a; // Parámetro 'a' de r(theta)
    int param_b; // Parámetro 'b' de r(theta)

    // Predicado específico para la figura r(theta) = sin^a(theta) + cos^b(theta)
    // Este método es interno y usa los miembros param_a y param_b
    bool isInsidePolarFigure(double x, double y) const;

public:
    // Constructor por defecto. No se recomienda usar sin inicializar límites.
    MonteCarloEstimator();
    // Destructor
    ~MonteCarloEstimator();
    
    // Constructor genérico para integración 1D
    MonteCarloEstimator(double a, double b, int n_samples, Function1DType f, const std::string& desc = "Integracion 1D");

    // Constructor genérico para estimación de área 2D
    MonteCarloEstimator(double x0, double x1, double y0, double y1, int n_samples, Predicate2DType is_inside_func, const std::string& desc = "Area 2D");

    // Constructor específico para el Problema 3 del PDF (figura polar)
    // Define los límites del recuadro, el número de muestras y los parámetros a, b.
    MonteCarloEstimator(int n_samples, int a_param, int b_param, const std::string& desc = "Area de figura polar (Problema 3)");
    
    // Método público para mostrar el resultado y la descripción
    void show() const;

    // Getter para el resultado si es necesario acceder a él directamente
    double getResult() const { return estimated_result; }
};

#endif // MONTE_CARLO_ESTIMATOR_H
