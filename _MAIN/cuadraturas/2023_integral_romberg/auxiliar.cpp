#include "auxiliar.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

// Definir PI si no está en cmath (a veces no está M_PI por defecto)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Función a integrar: f(t) = cos(x * cos(t))
// Nota: Esta función es interna a `solveProblem3b` o su scope,
// por eso la definimos aquí.
double integral_function(double t, double x_val_for_integral) {
    return std::cos(x_val_for_integral * std::cos(t));
}

void solveProblem3b() {
    std::cout << std::fixed << std::setprecision(10); //

    // Integration limits for t
    const double T_MIN = 0.0; //
    const double T_MAX = M_PI; //

    // Range for x and step size
    const double X_START = 0.0; //
    const double X_END = 10.0; //
    const double DELTA_X = 0.1; //

    // Output header for potential
    std::ofstream cfile("cuadratura.dat"); //
    cfile << "x_value g_x_value" << std::endl; //

    // Crear una instancia de NumericalIntegrator con los límites de integración
    NumericalIntegrator integrator(T_MIN, T_MAX); //

    std::cout << "\n--- Resultados de Cuadratura de Romberg (Problema 3b) ---" << std::endl;

    for (double x = X_START; x <= X_END; x += DELTA_X) { //
        // Calcular la integral usando el método de Romberg de la clase NumericalIntegrator
        // Usamos una lambda para "envolver" la función integral_function(t, x) y pasarla a Romberg
        // Capturamos 'x' para que la función integral_function lo use.
        double integral_val = integrator.Romberg([x](double t){ return integral_function(t, x); }, 10); //

        // Calculate g(x) = (1/pi) * integral_val
        double g_x = integral_val / M_PI; //

        cfile << x << " " << g_x << std::endl; //
        // Opcional: mostrar en consola para feedback inmediato
        std::cout << "x = " << std::setw(6) << x << ", g(x) = " << g_x << std::endl;
    }

    cfile.close(); // Cerrar el archivo después de escribir
    std::cout << "Resultados guardados en 'cuadratura.dat'" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
}
