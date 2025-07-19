#ifndef EQNS_H
#define EQNS_H

#include "rk_4.h"
#include "diferencias_finitas.h" // <-- Se actualiza el nombre del include
#include <vector>

// Estructura para agrupar todos los parámetros del problema
struct PhysicsParameters {
    int N;
    double L, h;
    double sigma, omega, rho0, q, m;
    const double c = 1.0;
};

// Se actualiza el tipo del parámetro en la firma de la función
rk4::ODEFunction create_maxwell_system_function(const PhysicsParameters& params, const DiferenciasFinitas& fdm);

#endif