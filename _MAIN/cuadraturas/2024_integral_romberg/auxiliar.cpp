#include "auxiliar.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>

// Definir PI si no está en cmath (a veces M_PI no está por defecto)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Integrando para la función F(x,y)
// theta_val es la variable de integración
// x_param y y_param son los parámetros fijos para esta integral
double integrand_F(double theta_val, double x_param, double y_param) {
    return std::pow(std::cos(theta_val), 2.0 * x_param - 1.0) *
           std::pow(std::sin(theta_val), 2.0 * y_param - 1.0);
}

// Integrando para la función G(x,y)
// t_val es la variable de integración
// x_param y y_param son los parámetros fijos para esta integral
double integrand_G(double t_val, double x_param, double y_param) {
    return std::pow(t_val, x_param - 1.0) *
           std::pow(1.0 - t_val, y_param - 1.0);
}


void solveProblem1() {
    std::cout << std::fixed << std::setprecision(10);

    // Límites de integración para F(x,y)
    const double F_T_MIN = 0.0;
    const double F_T_MAX = M_PI / 2.0;

    // Límites de integración para G(x,y)
    const double G_T_MIN = 0.0;
    const double G_T_MAX = 1.0;

    // Rango del parámetro s
    const double S_START = 0.0;
    const double S_END = 2.0 * M_PI;
    const double DELTA_S = 0.01; // Paso para s (determina la suavidad de la curva)

    // Crear instancias de NumericalIntegrator
    NumericalIntegrator integrator_F(F_T_MIN, F_T_MAX);
    NumericalIntegrator integrator_G(G_T_MIN, G_T_MAX);

    // Abrir archivo para guardar los resultados
    std::ofstream outfile("beta_functions.dat");
    if (!outfile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo beta_functions.dat para escritura." << std::endl;
        return;
    }
    outfile << "s x_s y_s F_val G_val" << std::endl; // Encabezado del archivo

    std::cout << "\n--- Calculando F(x,y) y G(x,y) sobre la parametrización ---" << std::endl;

    for (double s = S_START; s <= S_END; s += DELTA_S) {
        // Calcular x(s) y y(s) para el s actual
        double current_x = 1.5 + std::cos(s);
        double current_y = 1.5 + std::sin(s);

        // Calcular F(current_x, current_y) usando Romberg
        // Notar la lambda que captura current_x y current_y
        double F_val = 2.0 * integrator_F.Romberg(
            [current_x, current_y](double theta_val) {
                return integrand_F(theta_val, current_x, current_y);
            }, 10); // 10 iteraciones de Romberg, puedes ajustarlo

        // Calcular G(current_x, current_y) usando Romberg
        double G_val = integrator_G.Romberg(
            [current_x, current_y](double t_val) {
                return integrand_G(t_val, current_x, current_y);
            }, 10); // 10 iteraciones de Romberg, puedes ajustarlo

        // Escribir los resultados en el archivo
        outfile << s << " " << current_x << " " << current_y << " " << F_val << " " << G_val << std::endl;

        // Opcional: mostrar un pequeño progreso en consola
        // std::cout << "s = " << std::setw(8) << s << ", F = " << F_val << ", G = " << G_val << std::endl;
    }

    outfile.close();
    std::cout << "Resultados guardados en 'beta_functions.dat'" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
}
