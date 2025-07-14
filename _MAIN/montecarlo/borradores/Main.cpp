#include "montecarlo_estimator.h"
#include <iostream>
#include <iomanip>
#include <cmath> // Para std::sin, std::cos, std::pow, std::sqrt, std::atan2

// Definir PI si no está en cmath (a veces no está M_PI por defecto)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

const int OUTPUT_PRECISION = 12; // Cantidad de decimales para la salida

int main()
{
    std::cout << std::fixed << std::setprecision(OUTPUT_PRECISION);

    // --- Problema 3a: Área de la figura polar r(theta) = sin^a(theta) + cos^b(theta) ---
    // Parámetros del problema
    const int A_POLAR = 1;
    const int B_POLAR = 9;
    const int N_SAMPLES_POLAR = 10000000;

    // 1. Definir el predicado (lambda) para la figura polar
    // Capturamos A_POLAR y B_POLAR en la lambda
    auto is_inside_polar_figure = [A_POLAR, B_POLAR](double x, double y) -> bool {
        double r_prime = std::sqrt(x * x + y * y);
        if (r_prime == 0.0) {
            return true;
        }
        double theta_prime = std::atan2(y, x);
        double r_figure = std::pow(std::sin(theta_prime), A_POLAR) + std::pow(std::cos(theta_prime), B_POLAR);
        return r_prime <= r_figure && r_figure >= 0; // Asegurarse que el radio sea no negativo
    };

    // 2. Definir los límites del recuadro
    // Basado en el análisis previo, la figura cabe en [-1.0, 1.0] x [-1.0, 1.0]
    double x_min_polar = -1.0;
    double x_max_polar = 1.0;
    double y_min_polar = -1.0;
    double y_max_polar = 1.0;

    // 3. Crear el estimador usando el constructor genérico 2D
    MonteCarloEstimator problem3a_solver(
        x_min_polar, x_max_polar,
        y_min_polar, y_max_polar,
        N_SAMPLES_POLAR,
        is_inside_polar_figure, // ¡Aquí pasamos la lambda!
        "Area de figura polar: r(theta) = sin^1(theta) + cos^9(theta) (Prueba 3a)"
    );
    problem3a_solver.show();

    // --- Problema del Toroide: (R − sqrt(x^2 + y^2))^2 + z^2 = r^2 ---
    // Parámetros del toroide
    const double R_MAJOR_TORUS = 5.0;
    const double R_MINOR_TORUS = 2.0;
    const int N_SAMPLES_TORUS = 10000000;

    // 1. Definir el predicado (lambda) para el toroide
    auto is_inside_torus_figure = [R_MAJOR_TORUS, R_MINOR_TORUS](double x, double y, double z) -> bool {
        double eq = std::pow(R_MAJOR_TORUS - std::sqrt(x * x + y * y), 2) + z * z;
        return eq <= std::pow(R_MINOR_TORUS, 2);
    };

    // 2. Definir los límites del recuadro (cubo)
    // L = 4 * (r + R) en tu ejemplo anterior, lo que significa un cubo de lado L centrado en (0,0,0)
    double L_torus = 4 * (R_MINOR_TORUS + R_MAJOR_TORUS);
    double x_min_torus = -L_torus / 2.0;
    double x_max_torus = L_torus / 2.0;
    double y_min_torus = -L_torus / 2.0;
    double y_max_torus = L_torus / 2.0;
    double z_min_torus = -L_torus / 2.0;
    double z_max_torus = L_torus / 2.0;

    // 3. Crear el estimador usando el constructor genérico 3D
    MonteCarloEstimator torus_solver(
        x_min_torus, x_max_torus,
        y_min_torus, y_max_torus,
        z_min_torus, z_max_torus,
        N_SAMPLES_TORUS,
        is_inside_torus_figure, // ¡Aquí pasamos la lambda!
        "Volumen del Toroide"
    );
    torus_solver.show();

    // Puedes agregar más problemas aquí de la misma manera:
    // Para una esfera, defines is_inside_sphere lambda y sus límites, y usas el constructor 3D.
    // Para un rectángulo, defines is_inside_rectangle lambda y sus límites, y usas el constructor 2D.

    return 0;
}
