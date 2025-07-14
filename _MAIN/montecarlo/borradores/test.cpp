#include "montecarlo_estimator.h" // Incluimos la clase extendida
#include <iostream>
#include <iomanip> // Para formatear la salida

// --- Constantes y configuraciones globales (o a nivel de archivo) ---
const int OUTPUT_PRECISION = 12; // Cantidad de decimales para la salida

// Parámetros por defecto para el toroide
const double TORUS_R_MAJOR_VAL = 5.0; // R
const double TORUS_R_MINOR_VAL = 2.0; // r
const int TORUS_N_SAMPLES_VAL = 1000000; // N

int main()
{
    // Configura la salida para mostrar muchos decimales
    std::cout << std::fixed << std::setprecision(OUTPUT_PRECISION);

    // Crea una instancia de MonteCarloEstimator para el toroide.
    // Toda la lógica de cálculo está encapsulada en el constructor.
    MonteCarloEstimator torus_solver(TORUS_R_MAJOR_VAL, TORUS_R_MINOR_VAL, TORUS_N_SAMPLES_VAL, "Volumen del Toroide");
    
    // Muestra el resultado.
    torus_solver.show();

    // Opcional: Calcular el volumen exacto para comparar
    // double exact_volume = 2 * M_PI * M_PI * TORUS_R_MAJOR_VAL * TORUS_R_MINOR_VAL * TORUS_R_MINOR_VAL;
    // std::cout << "Volumen exacto del toroide: " << exact_volume << std::endl;

    return 0;
}
