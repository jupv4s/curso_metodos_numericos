#ifndef METODO_DE_LINEAS_H
#define METODO_DE_LINEAS_H

#include "rk_4.h"
#include "diferencias_finitas.h"
#include "eqns.h"
#include <vector>
#include <string>

class MetodoDeLineas {
public:
    MetodoDeLineas(const ParametrosFisicos& config);
    void ejecutar();

private:
    ParametrosFisicos m_config;
    DiferenciasFinitas m_fdm;
    rk4 m_solver;
    std::vector<double> m_y;

    void guardarCorteDelCampo(const std::string& filename) const;
};

#endif