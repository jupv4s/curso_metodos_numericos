#include "metodo_de_lineas.h"
#include <iostream>
#include <fstream>
#include <cmath>

MetodoDeLineas::MetodoDeLineas(const PhysicsParameters& params) :
    m_params(params),
    m_fdm(params.N, params.L), // <-- Cambio de nombre
    m_solver(create_maxwell_system_function(m_params, m_fdm)),
    m_y(12 * params.N * params.N * params.N, 0.0)
{}

void MetodoDeLineas::run()
{
    const double t_start_fields = 0.0;
    const double t_end_fields = 100.0;
    const double t_start_particle = 100.0;
    const double t_end_particle = 104.0;
    const double tol = 1e-4;
    const double dt_ini = 1e-2;

    std::cout << "Fase 1: Evolucionando campos hasta T=" << t_end_fields << " (con N=" << m_params.N << ")..." << std::endl;
    m_y = m_solver.integrar_adaptativo(m_y, t_start_fields, t_end_fields, dt_ini, tol);
    
    std::cout << "Fase 1 completada. Guardando mapa de densidad..." << std::endl;
    save_field_slice("fields_T100.csv");

    std::cout << "\nFase 2: Introduciendo particula y evolucionando hasta T=" << t_end_particle << "..." << std::endl;
    const size_t num_field_vars = 12 * m_params.N * m_params.N * m_params.N;
    m_y.resize(num_field_vars + 6);
    m_y[num_field_vars + 0] = 0.25; m_y[num_field_vars + 1] = 0.25; m_y[num_field_vars + 2] = 0.0;
    m_y[num_field_vars + 3] = 0.0;  m_y[num_field_vars + 4] = 0.0;  m_y[num_field_vars + 5] = 0.0;

    m_solver.integrar_adaptativo(m_y, t_start_particle, t_end_particle, dt_ini, tol, "particle_trajectory.csv");
    std::cout << "\nSimulacion finalizada." << std::endl;
}

void MetodoDeLineas::save_field_slice(const std::string& filename) const
{
    std::ofstream file(filename);
    //file << "x,y,Ex,Ey,Ez,magE,Bx,By,Bz,magB\n"; // para exportar a CSV
    file << "x\ty\tEx\tEy\tEz\tmagE\tBx\tBy\tBz\tmagB\n"; // para exportar a dat
    const int N = m_params.N;
    const int N3 = N * N * N;
    const int k_slice = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int idx = i*N*N + j*N + k_slice;
            double x = i * m_params.h;
            double y_pos = j * m_params.h;
            double Ex = m_y[idx + 0*N3]; double Ey = m_y[idx + 1*N3]; double Ez = m_y[idx + 2*N3];
            double Bx = m_y[idx + 3*N3]; double By = m_y[idx + 4*N3]; double Bz = m_y[idx + 5*N3];
            double magE = sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
            double magB = sqrt(Bx*Bx + By*By + Bz*Bz);
            //file << x << "," << y_pos << "," << Ex << "," << Ey << "," << Ez << "," << magE // para exportar a CSV
            //     << "," << Bx << "," << By << "," << Bz << "," << magB << "\n";
            file << x << "\t" << y_pos << "\t" << Ex << "\t" << Ey << "\t" << Ez << "\t" << magE // para exportar a dat
                 << "\t" << Bx << "\t" << By << "\t" << Bz << "\t" << magB << "\n";
        }
    }
}