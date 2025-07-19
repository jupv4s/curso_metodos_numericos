#include "ode_solver.h"

// Implementación del constructor
OdeSolver::OdeSolver(FunctionType func) : f(func) {}

// Implementación del método de Euler
void OdeSolver::solve_euler(double y0, double t0, double tf, double h, std::vector<double>& t_out, std::vector<double>& y_out) {
    t_out.clear();
    y_out.clear();

    double y = y0;
    for (double t = t0; t <= tf; t += h) {
        t_out.push_back(t);
        y_out.push_back(y);
        y = y + h * f(t, y);
    }
}

// Implementación del método Predictor-Corrector
void OdeSolver::solve_predictor_corrector(double y0, double t0, double tf, double h, std::vector<double>& t_out, std::vector<double>& y_out) {
    t_out.clear();
    y_out.clear();

    double y = y0;
    for (double t = t0; t <= tf; t += h) {
        t_out.push_back(t);
        y_out.push_back(y);

        // Paso Predictor (Euler)
        double y_pred = y + h * f(t, y);
        // Paso Corrector
        y = y + (h / 2.0) * (f(t, y) + f(t + h, y_pred));
    }
}