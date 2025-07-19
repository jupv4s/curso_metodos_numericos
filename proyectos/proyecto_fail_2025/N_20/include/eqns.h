#ifndef EQNS_H
#define EQNS_H

#include <vector>
#include "rk_4.h"
#include "diferencias_finitas.h"

struct ParametrosFisicos
{
    int N;
    double L, h;
    double sigma, omega, rho0, q, m;
    const double c = 1.0;
};

rk4::ODEFunction crearFuncionDelSistema(const ParametrosFisicos& config, const DiferenciasFinitas& fdm); // <-- Cambio de nombre

#endif