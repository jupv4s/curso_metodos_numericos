// OdeSolution.h
#ifndef ODE_SOLUTION_H // Mantengo el include guard con mayúsculas y guiones bajos como es convención
#define ODE_SOLUTION_H

#include "Vector.h"
#include <vector>

// Estructura para contener los resultados de una simulación de EDOs
struct OdeSolution
{
    std::vector<double> times;          // Vector de tiempos
    std::vector<Vector<double>> states; // Vector de estados (cada elemento es un Vector<double>)

    // Constructor por defecto
    OdeSolution() = default;

    // Constructor con inicializadores, si se necesita
    OdeSolution(std::vector<double> t, std::vector<Vector<double>> s)
        : times(std::move(t)), states(std::move(s)) {}

    // Método para obtener el número de puntos de datos
    std::size_t num_points() const {
        return times.size();
    }
};

#endif // ODE_SOLUTION_H