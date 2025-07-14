#ifndef MONTE_CARLO_ESTIMATOR_H
#define MONTE_CARLO_ESTIMATOR_H

#include <iostream>     // Para std::cerr, std::cout
#include <vector>       // Para std::vector si fuera necesario para funciones auxiliares
#include <string>       // Para mensajes de error
#include <cmath>        // Para std::cos, std::sin, std::sqrt, std::atan2, std::pow
#include <random>       // Para std::random_device, std::mt19937, std::uniform_real_distribution
#include <limits>       // Para std::numeric_limits<double>::quiet_NaN
#include <iomanip>      // Para std::setprecision, std::fixed
#include <functional>   // Para std::function

// Definir PI si no está en cmath (depende del compilador y el estándar C++)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Un alias de tipo para una función que toma un double y retorna un double (para integración 1D)
using Function1DType = double (*)(double);

// Un alias de tipo para una función que toma dos doubles y retorna un bool (para área 2D)
using Predicate2DType = bool (*)(double, double);

// Un alias de tipo para una función que toma tres doubles y retorna un bool (para volumen 3D)
using Predicate3DType = std::function<bool(double, double, double)>;


// Clase para realizar estimaciones usando el método de Montecarlo
class MonteCarloEstimator
{
private:
    // Límites del espacio de muestreo (el "recuadro")
    double x_min, x_max;
    double y_min, y_max;
    double z_min, z_max; // Nuevo para 3D

    std::mt19937 random_engine; // Motor de números aleatorios

    // Almacenará el resultado del cálculo
    double estimated_result;
    std::string description; // Descripción de lo que se calculó

    // Método privado para generar un número aleatorio uniforme dentro de un rango
    double getRandomNumber(double, double); // Sin nombres de parámetros

    // --- Funciones auxiliares para el problema de la figura polar (problema 3a) ---
    int param_a_polar; // Parámetro 'a' de r(theta)
    int param_b_polar; // Parámetro 'b' de r(theta)
    bool isInsidePolarFigure(double, double) const; // Sin nombres de parámetros

    // --- Funciones auxiliares para el problema del toroide ---
    double R_toroide; // Radio mayor del toroide
    double r_toroide; // Radio menor del toroide
    bool isInsideTorus(double, double, double) const; // Sin nombres de parámetros


public:
    // Constructor por defecto. No se recomienda usar sin inicializar límites.
    MonteCarloEstimator();
    // Destructor
    ~MonteCarloEstimator();
    
    // Constructor genérico para integración 1D
    MonteCarloEstimator(double, double, int, Function1DType, const std::string& = "Integracion 1D"); // Sin nombres de parámetros

    // Constructor genérico para estimación de área 2D
    MonteCarloEstimator(double, double, double, double, int, Predicate2DType, const std::string& = "Area 2D"); // Sin nombres de parámetros

    // Constructor específico para el Problema 3a del PDF (figura polar)
    MonteCarloEstimator(int, int, int, const std::string& = "Area de figura polar (Problema 3a)"); // Sin nombres de parámetros

    // Constructor específico para el problema del Toroide (volumen 3D)
    MonteCarloEstimator(double, double, int, const std::string& = "Volumen de Toroide"); // Sin nombres de parámetros
    
    // Método genérico para estimar volumen en 3D
    double estimateVolume3D(Predicate3DType, int); // Sin nombres de parámetros

    // Método público para mostrar el resultado y la descripción
    void show() const;

    // Getter para el resultado si es necesario acceder a él directamente
    double getResult() const { return estimated_result; }
};

#endif // MONTE_CARLO_ESTIMATOR_H
