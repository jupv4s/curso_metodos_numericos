#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <vector>
#include <string>

class Lagrange
{
public:
    // Constructor que toma los puntos de datos.
    Lagrange(const std::vector<double>& puntosX, const std::vector<double>& puntosY);

    // Evalúa el polinomio de Lagrange en un punto x_eval.
    double evaluar(double x_eval) const;

private:
    std::vector<double> x; // Coordenadas x de los puntos de datos.
    std::vector<double> y; // Coordenadas y de los puntos de datos.
    int n;                 // Número de puntos de datos.
};

#endif