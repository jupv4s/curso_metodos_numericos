#include "OdeSolver.h"   // Incluye tu clase ODESolver
#include <iostream>      // Para std::cout y std::cerr

// Define la función de las EDOs. Para el oscilador armónico simple:
// dy/dt = v
// dv/dt = -omega^2 * y
// Donde Y[0] es la posición y Y[1] es la velocidad.
Vector<double> oscilador_armonico(double t, const Vector<double>& Y)
{
    const double omega_squared = 1.0; // omega^2 = k/m, asumimos 1 para simplificar
    Vector<double> dYdt(2); // Un vector para las derivadas
    dYdt[0] = Y[1];                         // dy/dt = v
    dYdt[1] = -omega_squared * Y[0];        // dv/dt = -omega^2 * y
    return dYdt;
}

int main()
{
    // 1. Define las condiciones iniciales del sistema (posición y velocidad)
    Vector<double> initial_conditions({1.0, 0.0}); // Posición inicial = 1.0, Velocidad inicial = 0.0

    // 2. Define los parámetros de la simulación
    double initial_time = 0.0;
    double final_time = 10.0;
    double time_step = 0.01;

    // 3. Crea una instancia de ODESolver
    //    t_inicial, t_final, paso_tiempo, función_de_derivadas
    OdeSolver solver(initial_time, final_time, time_step, oscilador_armonico);

    // 4. Ejecuta un método de integración y guarda los resultados
    try {
        std::cout << "Ejecutando Euler..." << std::endl;
        solver.Euler(initial_conditions, "euler_oscilador.dat");
        std::cout << "Resultados guardados en euler_oscilador.dat" << std::endl;

        std::cout << "Ejecutando RK4..." << std::endl;
        solver.RK4(initial_conditions, "rk4_oscilador.dat");
        std::cout << "Resultados guardados en rk4_oscilador.dat" << std::endl;

        // Si quieres probar otro método, solo descomenta:
        std::cout << "Ejecutando PEFRL..." << std::endl;
        solver.PEFRL(initial_conditions, "pefrl_oscilador.dat");
        std::cout << "Resultados guardados en pefrl_oscilador.dat" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error durante la simulación: " << e.what() << std::endl;
        return 1; // Retorna un código de error
    }

    std::cout << "Simulación completada con éxito." << std::endl;
    return 0; // Retorna éxito
}