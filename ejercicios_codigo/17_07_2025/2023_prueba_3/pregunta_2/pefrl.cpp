#include "pefrl.h"
#include <fstream>
#include <iomanip>
#include <cmath>

pefrl::pefrl(const ForceFunction& force_func) : F(force_func) {}

void pefrl::pefrl_step(std::vector<double>& q, std::vector<double>& v, double h) {
    // Implementación del algoritmo PEFRL
    // q: posición (x, y), v: velocidad (vx, vy)
    // h: paso de tiempo

    // Paso 1: Actualizar posición
    for (size_t i = 0; i < q.size(); ++i) q[i] += xi * h * v[i];

    // Paso 2: Actualizar velocidad
    std::vector<double> a = F(q);
    for (size_t i = 0; i < v.size(); ++i) v[i] += (1.0 - 2.0 * lambda) / 2.0 * h * a[i];

    // Paso 3: Actualizar posición
    for (size_t i = 0; i < q.size(); ++i) q[i] += chi * h * v[i];
    
    // Paso 4: Actualizar velocidad
    a = F(q);
    for (size_t i = 0; i < v.size(); ++i) v[i] += lambda * h * a[i];

    // Paso 5: Actualizar posición
    for (size_t i = 0; i < q.size(); ++i) q[i] += (1.0 - 2.0 * (chi + xi)) * h * v[i];
    
    // Paso 6: Actualizar velocidad
    a = F(q);
    for (size_t i = 0; i < v.size(); ++i) v[i] += lambda * h * a[i];
    
    // Paso 7: Actualizar posición
    for (size_t i = 0; i < q.size(); ++i) q[i] += chi * h * v[i];

    // Paso 8: Actualizar velocidad
    a = F(q);
    for (size_t i = 0; i < v.size(); ++i) v[i] += (1.0 - 2.0 * lambda) / 2.0 * h * a[i];

    // Paso 9: Actualizar posición
    for (size_t i = 0; i < q.size(); ++i) q[i] += xi * h * v[i];
}


void pefrl::integrar(std::vector<double>& q0, std::vector<double>& v0, double t0, double tf, double h, const std::string& archivo_salida) {
    std::ofstream data(archivo_salida);
    data << "# t\tx\ty\tvx\tvy\n";
    data << std::scientific << std::setprecision(10);

    std::vector<double> q = q0;
    std::vector<double> v = v0;
    double t = t0;

    int steps = static_cast<int>((tf - t0) / h);

    for (int i = 0; i <= steps; ++i) {
        data << t << "\t" << q[0] << "\t" << q[1] << "\t" << v[0] << "\t" << v[1] << "\n";
        pefrl_step(q, v, h);
        t += h;
    }
    
    data.close();
}