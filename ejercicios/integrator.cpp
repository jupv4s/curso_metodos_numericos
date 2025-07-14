#include "integrator.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <numeric> // Para std::inner_product

// --- Implementación de AdaptiveIntegrator ---

AdaptiveIntegrator::AdaptiveIntegrator(DerivativeFunc func, double tol)
    : m_func(func), m_tol(tol) {}

// Función auxiliar para operaciones vectoriales
State operator+(const State& a, const State& b) {
    State result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = a[i] + b[i];
    return result;
}

State operator*(double scalar, const State& a) {
    State result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = scalar * a[i];
    return result;
}

State AdaptiveIntegrator::rk4_step(double t, const State& y, double h) {
    State k1 = h * m_func(t, y);
    State k2 = h * m_func(t + 0.5 * h, y + 0.5 * k1);
    State k3 = h * m_func(t + 0.5 * h, y + 0.5 * k2);
    State k4 = h * m_func(t + h, y + k3);
    return y + (1.0/6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
}

void AdaptiveIntegrator::integrate(const State& y0, double t_start, double t_end, double dt_initial) {
    double t = t_start;
    State y = y0;
    double h = dt_initial;

    std::ofstream trajectory_file("trajectory.csv");
    trajectory_file << "t,x,y,vx,vy,kinetic_energy\n";
    
    std::cout << "Iniciando integración adaptativa...\n";

    while (t < t_end) {
        if (t + h > t_end) {
            h = t_end - t;
        }

        State y1 = rk4_step(t, y, h);
        State y_half = rk4_step(t, y, h / 2.0);
        State y2 = rk4_step(t + h / 2.0, y_half, h / 2.0);

        double error = 0.0;
        for (size_t i = 0; i < y.size(); ++i) {
            error += std::pow(y1[i] - y2[i], 2);
        }
        error = std::sqrt(error) / 15.0; // Estimador de error para RK4

        if (error < m_tol) {
            t += h;
            y = y2; // Usamos la estimación más precisa

            double kinetic_energy = 0.5 * (y[2] * y[2] + y[3] * y[3]);
            trajectory_file << std::fixed << std::setprecision(10) << t << "," 
                            << y[0] << "," << y[1] << "," << y[2] << "," << y[3] << "," 
                            << kinetic_energy << "\n";
        }

        if (error > 1e-20) {
            double h_new = 0.9 * h * std::pow(m_tol / error, 0.2); // El exponente 1/5 es para un método de 4(5) orden
            h = std::max(0.1 * h, std::min(h_new, 5.0 * h)); // Limitar cambio de paso
        } else {
            h *= 2.0; // Si el error es muy bajo, duplicar el paso
        }
    }

    trajectory_file.close();
    std::cout << "Integración completada. Resultados guardados en trajectory.csv\n";
}