#include "integrator.h"
#include "interpolator.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <numeric>

// --- Definición del sistema físico ---

const double g = -0.0001;

// Función que define el sistema de EDOs de primer orden
// dy/dt = f(t, y) donde y = [x, y, vx, vy]
State system_derivatives(double t, const State& y) {
    double x = y[0];
    double y_pos = y[1];
    double vx = y[2];
    double vy = y[3];

    double r2 = x * x + y_pos * y_pos;
    if (r2 < 1e-12) { // Evitar división por cero
        return {vx, vy, 0.0, g};
    }
    
    double theta = std::atan2(y_pos, x);
    
    double cos_3theta = std::cos(3 * theta);
    double sin_3theta = std::sin(3 * theta);

    double cosh_arg = 50.0 * (r2 - 0.5 * sin_3theta - 1.0);
    double cosh_val = std::cosh(cosh_arg);
    double denom_cosh = cosh_val * cosh_val;
    
    double factor_comun = -2500.0 / denom_cosh;
    
    double Fx = factor_comun * (2.0 * x + (3.0 * y_pos * cos_3theta) / (2.0 * r2));
    double Fy = factor_comun * (2.0 * y_pos - (3.0 * x * cos_3theta) / (2.0 * r2)) + g;
    
    // m=1, entonces a = F
    return {vx, vy, Fx, Fy};
}

// --- Función principal ---
int main() {
    // --- 1. Configuración de la simulación ---
    State y0 = {0.1, 0.0, 0.01, 0.01}; // [x, y, vx, vy]
    double t_start = 0.0;
    double t_end = 5000.0;
    double dt_initial = 0.001;
    double tolerance = 1e-15;

    // --- 2. Ejecución de la simulación ---
    AdaptiveIntegrator integrator(system_derivatives, tolerance);
    integrator.integrate(y0, t_start, t_end, dt_initial);
    
    // --- 3. Experimento de interpolación ---
    std::cout << "\nIniciando experimento de interpolación...\n";

    // Leer los datos del archivo generado
    std::vector<double> t_vec, x_vec, y_vec;
    std::ifstream infile("trajectory.csv");
    std::string line;
    std::getline(infile, line); // Omitir cabecera
    while (std::getline(infile, line)) {
        double t, x, y, vx, vy, ke;
        char comma;
        std::stringstream ss(line);
        ss >> t >> comma >> x >> comma >> y >> comma >> vx >> comma >> vy >> comma >> ke;
        t_vec.push_back(t);
        x_vec.push_back(x);
        y_vec.push_back(y);
    }
    infile.close();

    // Tomar el 1% de la trayectoria
    int n_total = t_vec.size();
    int n_slice = static_cast<int>(n_total * 0.01);
    
    // Submuestrear: conservar solo puntos pares
    std::vector<double> t_data, x_data, y_data;
    std::vector<double> t_removed, x_real, y_real;

    for(int i = 0; i < n_slice; ++i) {
        if (i % 2 == 0) {
            t_data.push_back(t_vec[i]);
            x_data.push_back(x_vec[i]);
            y_data.push_back(y_vec[i]);
        } else {
            t_removed.push_back(t_vec[i]);
            x_real.push_back(x_vec[i]);
            y_real.push_back(y_vec[i]);
        }
    }

    std::cout << "Puntos en la muestra del 1%: " << n_slice << std::endl;
    std::cout << "Puntos usados para interpolar (nodos): " << t_data.size() << std::endl;
    std::cout << "Puntos a restaurar: " << t_removed.size() << std::endl;

    // Crear interpoladores Spline
    CubicSplineInterpolator spline_x(t_data, x_data);
    CubicSplineInterpolator spline_y(t_data, y_data);

    // Restaurar los puntos y calcular error
    std::ofstream interp_file("interpolation_results.csv");
    interp_file << "t_removed,x_real,y_real,x_restored,y_restored\n";
    
    double total_error_sq = 0.0;

    for (size_t i = 0; i < t_removed.size(); ++i) {
        double x_restored = spline_x.interpolate(t_removed[i]);
        double y_restored = spline_y.interpolate(t_removed[i]);

        double err_x = x_restored - x_real[i];
        double err_y = y_restored - y_real[i];
        total_error_sq += err_x*err_x + err_y*err_y;

        interp_file << std::fixed << std::setprecision(10) 
                    << t_removed[i] << "," << x_real[i] << "," << y_real[i] << ","
                    << x_restored << "," << y_restored << "\n";
    }
    interp_file.close();

    double total_error = std::sqrt(total_error_sq);
    std::cout << "Experimento de interpolación completado." << std::endl;
    std::cout << "Error Euclidiano total en la restauración: " << std::scientific << total_error << std::endl;
    std::cout << "Resultados guardados en interpolation_results.csv\n";

    return 0;
}