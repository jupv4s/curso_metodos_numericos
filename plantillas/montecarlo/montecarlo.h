#ifndef MONTECARLO_INTEGRATOR_H
#define MONTECARLO_INTEGRATOR_H

#include <functional>

class MonteCarloIntegrator
{
public:
    // Método para aproximar la integral de una función 'func'
    // en el intervalo [a, b] usando 'num_muestras' puntos.
    double aproximar(
        double a,
        double b,
        long long num_muestras,
        const std::function<double(double)>& func
    );
};

#endif