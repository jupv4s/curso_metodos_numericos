#include "metodo_de_lineas.h"
#include <iostream>
#include <fstream>
#include <cmath>

MetodoDeLineas::MetodoDeLineas(const ParametrosFisicos& config) :
    m_config(config),
    m_fdm(config.N, config.L),
    m_solver(crearFuncionDelSistema(m_config, m_fdm)),
    m_y(12 * config.N * config.N * config.N, 0.0)
{}

void MetodoDeLineas::ejecutar() {
    const double t_start_fields = 0.0;
    const double t_end_fields = 100.0;
    const double t_start_particle = 100.0;
    const double t_end_particle = 104.0;
    const double tol = 1e-4;
    const double dt_ini = 1e-2;

    std::cout << "Fase 1: Evolucionando campos con rk4 hasta T=" << t_end_fields << " (con N=" << m_config.N << ")..." << std::endl;
    m_y = m_solver.integrar_adaptativo(m_y, t_start_fields, t_end_fields, dt_ini, tol);
    
    std::cout << "Fase 1 completada. Guardando mapa de densidad..." << std::endl;
    guardarCorteDelCampo("data/fields_T100.dat");

    std::cout << "\nFase 2: Introduciendo particula y evolucionando hasta T=" << t_end_particle << "..." << std::endl;
    const size_t num_field_vars = 12 * m_config.N * m_config.N * m_config.N;
    m_y.resize(num_field_vars + 6);
    m_y[num_field_vars + 0] = 0.25; m_y[num_field_vars + 1] = 0.25; m_y[num_field_vars + 2] = 0.0;
    m_y[num_field_vars + 3] = 0.0;  m_y[num_field_vars + 4] = 0.0;  m_y[num_field_vars + 5] = 0.0;

    m_solver.integrar_adaptativo(m_y, t_start_particle, t_end_particle, dt_ini, tol, "data/particle_trajectory.dat");
    std::cout << "\nSimulacion finalizada." << std::endl;
}

void MetodoDeLineas::guardarCorteDelCampo(const std::string& filename) const {
    std::ofstream file(filename);
    file << "x\ty\tEx\tEy\tEz\tmagE\tBx\tBy\tBz\tmagB\n";
    const int N = m_config.N;
    const int N3 = N * N * N;
    const int k_slice = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int idx = i*N*N + j*N + k_slice;
            double x = i * m_config.h;
            double y_pos = j * m_config.h;
            double Ex = m_y[idx + 0*N3]; double Ey = m_y[idx + 1*N3]; double Ez = m_y[idx + 2*N3];
            double Bx = m_y[idx + 3*N3]; double By = m_y[idx + 4*N3]; double Bz = m_y[idx + 5*N3];
            double magE = sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
            double magB = sqrt(Bx*Bx + By*By + Bz*Bz);
            file << x << "\t" << y_pos << "\t" << Ex << "\t" << Ey << "\t" << Ez << "\t" << magE 
                 << "\t" << Bx << "\t" << By << "\t" << Bz << "\t" << magB << "\n";
        }
    }
}