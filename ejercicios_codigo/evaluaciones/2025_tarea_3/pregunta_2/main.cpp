#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "ode_solver.h"

using namespace std;

// --- Definición del Problema Específico ---
// EDO: dy/dt = sin(t) - y
double ode_function(double t, double y)
{
    return sin(t) - y;
}

// Solución analítica exacta para el cálculo del error
double analytical_solution(double t) {
    return 1.5 * exp(-t) + 0.5 * (sin(t) - cos(t));
}

// --- Función `main`: Orquestador Principal ---
int main() {
    const double y0 = 1.0, t0 = 0.0, tf = 5.0, h = 0.05;

    OdeSolver solver(ode_function);

    vector<double> t_grid, y_euler, y_pc;

    cout << "Resolviendo con el método de Euler..." << endl;
    solver.solve_euler(y0, t0, tf, h, t_grid, y_euler);

    cout << "Resolviendo con el método Predictor-Corrector..." << endl;
    solver.solve_predictor_corrector(y0, t0, tf, h, t_grid, y_pc);

    string filename = "ode_comparison.txt";
    cout << "Escribiendo resultados en '" << filename << "'..." << endl;
    ofstream out(filename);
    out << scientific << setprecision(8);
    out << "# t\ty_euler\t\ty_pc\t\ty_exacta\terror_euler\terror_pc\n";

    for (size_t i = 0; i < t_grid.size(); ++i) {
        double t = t_grid[i];
        double y_exact = analytical_solution(t);
        out << t << "\t"
            << y_euler[i] << "\t"
            << y_pc[i] << "\t"
            << y_exact << "\t"
            << abs(y_euler[i] - y_exact) << "\t"
            << abs(y_pc[i] - y_exact) << "\n";
    }
    out.close();

    cout << "\nProceso completado." << endl;
    return 0;
}