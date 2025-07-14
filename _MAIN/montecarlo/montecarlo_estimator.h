#ifndef MONTECARLO_ESTIMATOR_H
#define MONTECARLO_ESTIMATOR_H

#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <limits>
#include <iomanip>
#include <functional> // Para std::function

// Definir PI si no está en cmath
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Tipos de predicados (funciones que devuelven true si un punto está dentro)
using Predicate2D = std::function<bool(double, double)>;
using Predicate3D = std::function<bool(double, double, double)>;
// Podrías tener Function1D para integración también, pero nos enfocaremos en área/volumen

class MonteCarloEstimator
{
private:
    double x_min, x_max;
    double y_min, y_max;
    double z_min, z_max; // Para 3D. Si solo es 2D, z_min/max pueden ser 0.0

    std::mt19937 random_engine;
    double estimated_result;
    std::string description;
    int n_samples; // Guardar el número de muestras

    double getRandomNumber(double, double);

public:
    // Constructor para estimación de ÁREA (2D)
    // Recibe los límites de X e Y, el número de muestras y el predicado 2D.
    MonteCarloEstimator(double, double, double, double, int, Predicate2D, const std::string& = "Area 2D (Generico)");

    // Constructor para estimación de VOLUMEN (3D)
    // Recibe los límites de X, Y y Z, el número de muestras y el predicado 3D.
    MonteCarloEstimator(double, double, double, double, double, double, int, Predicate3D, const std::string& = "Volumen 3D (Generico)");

    // No necesitamos constructores específicos para Toroide o Figura Polar aquí.
    // Esos problemas se definirán a través del predicado y los límites.

    void show() const;
    double getResult() const { return estimated_result; }
};

#endif // MONTE_CARLO_ESTIMATOR_H
