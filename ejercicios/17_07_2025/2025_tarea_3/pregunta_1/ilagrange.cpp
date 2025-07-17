#include "ilagrange.h"
#include <stdexcept>

// Constructor: inicializa los vectores de datos.
Lagrange::Lagrange(const std::vector<double>& puntosX, const std::vector<double>& puntosY)
{
    if (puntosX.size() != puntosY.size()) {
        throw std::invalid_argument("Los vectores X e Y deben tener el mismo tamaño.");
    }
    if (puntosX.empty()) {
        throw std::invalid_argument("Los vectores de datos no pueden estar vacíos.");
    }
    x = puntosX;
    y = puntosY;
    n = x.size();
}

// Evalúa el polinomio de Lagrange.
double Lagrange::evaluar(double x_eval) const
{
    double suma_total = 0.0;

    for (int i = 0; i < n; ++i)
    {
        double producto_base = 1.0;
        for (int j = 0; j < n; ++j)
        {
            // Solo incluye los términos donde j es diferente de i.
            if (i != j)
            {
                producto_base *= (x_eval - x[j]) / (x[i] - x[j]);
            }
        }
        suma_total += y[i] * producto_base;
    }

    return suma_total;
}