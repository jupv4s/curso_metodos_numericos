#include "montecarlo.h"
#include <random>
#include <iostream>

double MonteCarloIntegrator::aproximar(
    double a,
    double b,
    long long num_muestras,
    const std::function<double(double)>& func)
{
    if (a >= b)
    {
        std::cerr << "Error: El límite inferior 'a' debe ser menor que 'b'." << std::endl;
        return 0.0;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);

    double suma_total = 0.0;
    for (long long i = 0; i < num_muestras; ++i)
    {
        double punto_aleatorio = dis(gen);
        suma_total += func(punto_aleatorio);
    }

    double valor_promedio = suma_total / num_muestras;
    double intervalo = b - a;

    return intervalo * valor_promedio;
}