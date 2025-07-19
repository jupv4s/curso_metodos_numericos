#include <iostream>
#include <vector>
#include <cmath>
#include "pefrl.h"

// Parámetros del potencial
const double sigma = 10.0;
const double r0 = 10.0;

// Definición de la función de fuerza F(q), donde q = {x, y}
std::vector<double> fuerza(const std::vector<double>& q) {
    double x = q[0];
    double y = q[1];
    
    double r = std::sqrt(x * x + y * y);
    
    if (r < 1e-9) { // Evitar división por cero en el origen
        return {0.0, 0.0};
    }
    
    double common_factor = (sigma * sigma / r) * std::exp(-sigma * sigma * (r - r0) * (r - r0));
    
    double fx = -common_factor * x;
    double fy = -common_factor * y;
    
    return {fx, fy};
}

int main() {
    // Crear una instancia del integrador con la función de fuerza
    pefrl integrador(fuerza);

    // --- Simulación para la Partícula 1 ---
    std::vector<double> q1_0 = {-3.0, 0.1};
    std::vector<double> v1_0 = {3.0e-3, 0.0};
    integrador.integrar(q1_0, v1_0, 0.0, 20000.0, 0.2, "trayectoria1.txt");
    std::cout << "Simulación de la partícula 1 completada. Datos guardados en trayectoria1.txt" << std::endl;

    // --- Simulación para la Partícula 2 ---
    std::vector<double> q2_0 = {3.0, -0.1};
    std::vector<double> v2_0 = {-3.0e-3, 0.0};
    integrador.integrar(q2_0, v2_0, 0.0, 20000.0, 0.2, "trayectoria2.txt");
    std::cout << "Simulación de la partícula 2 completada. Datos guardados en trayectoria2.txt" << std::endl;

    return 0;
}