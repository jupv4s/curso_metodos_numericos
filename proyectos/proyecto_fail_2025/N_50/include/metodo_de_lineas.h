#ifndef METODO_DE_LINEAS_H
#define METODO_DE_LINEAS_H

#include "rk_4.h"
#include "diferencias_finitas.h" // <-- Cambio de nombre
#include "eqns.h"
#include <vector>
#include <string>

class MetodoDeLineas {
public:
    MetodoDeLineas(const PhysicsParameters& params);
    void run();

private:
    PhysicsParameters m_params;
    DiferenciasFinitas m_fdm; // <-- Cambio de nombre
    rk4 m_solver;
    std::vector<double> m_y;

    void save_field_slice(const std::string& filename) const;
};

#endif