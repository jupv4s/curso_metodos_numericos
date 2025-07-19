#ifndef ODESOLVER_H
#define ODESOLVER_H

#include <vector>
#include <functional>
#include <string>

class OdeSolver {
public:
    // Define un alias para el tipo de función que la clase manejará.
    using FunctionType = std::function<double(double, double)>;

private:
    FunctionType f; // Almacena la función de la EDO.

public:
    // Constructor
    OdeSolver(FunctionType func);

    // Método de Euler
    void solve_euler(double y0, double t0, double tf, double h, std::vector<double>& t_out, std::vector<double>& y_out);

    // Método Predictor-Corrector
    void solve_predictor_corrector(double y0, double t0, double tf, double h, std::vector<double>& t_out, std::vector<double>& y_out);
};

#endif