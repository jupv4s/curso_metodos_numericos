#include "eqns.h"
#include <cmath>
#include <vector>

// Declaramos las funciones de ayuda como 'static' para limitar su alcance a este archivo.
static int index(int i, int j, int k, int N);
static void get_coords(int idx, const PhysicsParameters& p, double& x, double& y, double& z);
static void calculate_sources(double t, const PhysicsParameters& p, std::vector<double>& dJdt, std::vector<double>& curlJ);
static void interpolate_fields(const std::vector<double>& y, const PhysicsParameters& p, double x, double y_pos, double z, double& Ex, double& Ey, double& Ez, double& Bx, double& By, double& Bz);


// --- Implementación de la función pública que crea la función del sistema ---

rk4::ODEFunction create_maxwell_system_function(const PhysicsParameters& params, const DiferenciasFinitas& fdm) {
    
    // Devolvemos una lambda que captura los parámetros y el operador por referencia.
    return [=](double t, const std::vector<double>& y) -> std::vector<double> {
        const int N = params.N;
        const int N3 = N * N * N;
        const size_t num_field_vars = 12 * N3;
        const double c2 = params.c * params.c;
        const double four_pi = 4.0 * M_PI;
        
        std::vector<double> dydt(y.size(), 0.0);

        // --- Parte 1: Evolución de los Campos E y B ---
        const double* Ex = &y[0*N3]; const double* Ey = &y[1*N3]; const double* Ez = &y[2*N3];
        const double* Bx = &y[3*N3]; const double* By = &y[4*N3]; const double* Bz = &y[5*N3];
        const double* VEx = &y[6*N3]; const double* VEy = &y[7*N3]; const double* VEz = &y[8*N3];
        const double* VBx = &y[9*N3]; const double* VBy = &y[10*N3]; const double* VBz = &y[11*N3];

        std::vector<double> lap_Ex(N3), lap_Ey(N3), lap_Ez(N3);
        std::vector<double> lap_Bx(N3), lap_By(N3), lap_Bz(N3);

        fdm.apply(std::vector<double>(Ex, Ex + N3), lap_Ex);
        fdm.apply(std::vector<double>(Ey, Ey + N3), lap_Ey);
        fdm.apply(std::vector<double>(Ez, Ez + N3), lap_Ez);
        fdm.apply(std::vector<double>(Bx, Bx + N3), lap_Bx);
        fdm.apply(std::vector<double>(By, By + N3), lap_By);
        fdm.apply(std::vector<double>(Bz, Bz + N3), lap_Bz);

        std::vector<double> dJdt, curlJ;
        calculate_sources(t, params, dJdt, curlJ);

        for (int i = 0; i < N3; ++i) {
            dydt[i + 0*N3] = VEx[i]; dydt[i + 1*N3] = VEy[i]; dydt[i + 2*N3] = VEz[i];
            dydt[i + 3*N3] = VBx[i]; dydt[i + 4*N3] = VBy[i]; dydt[i + 5*N3] = VBz[i];
            dydt[i + 6*N3] = c2 * lap_Ex[i] + four_pi / c2 * dJdt[i + 0*N3];
            dydt[i + 7*N3] = c2 * lap_Ey[i] + four_pi / c2 * dJdt[i + 1*N3];
            dydt[i + 8*N3] = c2 * lap_Ez[i] + four_pi / c2 * dJdt[i + 2*N3];
            dydt[i + 9*N3]  = c2 * lap_Bx[i] - (four_pi / params.c) * curlJ[i + 0*N3];
            dydt[i + 10*N3] = c2 * lap_By[i] - (four_pi / params.c) * curlJ[i + 1*N3];
            dydt[i + 11*N3] = c2 * lap_Bz[i] - (four_pi / params.c) * curlJ[i + 2*N3];
        }

        // --- Parte 2: Evolución de la partícula (si existe) ---
        if (y.size() > num_field_vars) {
            const double qx = y[num_field_vars + 0];
            const double qy = y[num_field_vars + 1];
            const double qz = y[num_field_vars + 2];
            const double vx = y[num_field_vars + 3];
            const double vy = y[num_field_vars + 4];
            const double vz = y[num_field_vars + 5];

            double E_p[3], B_p[3];
            interpolate_fields(y, params, qx, qy, qz, E_p[0], E_p[1], E_p[2], B_p[0], B_p[1], B_p[2]);
            
            double Fx = params.q * (E_p[0] + (vy * B_p[2] - vz * B_p[1]) / params.c);
            double Fy = params.q * (E_p[1] + (vz * B_p[0] - vx * B_p[2]) / params.c);
            double Fz = params.q * (E_p[2] + (vx * B_p[1] - vy * B_p[0]) / params.c);

            dydt[num_field_vars + 0] = vx;
            dydt[num_field_vars + 1] = vy;
            dydt[num_field_vars + 2] = vz;
            dydt[num_field_vars + 3] = Fx / params.m;
            dydt[num_field_vars + 4] = Fy / params.m;
            dydt[num_field_vars + 5] = Fz / params.m;
        }
        
        return dydt;
    };
}


// --- Implementación de las funciones de ayuda estáticas ---

static int index(int i, int j, int k, int N) {
    return i * N * N + j * N + k;
}

static void get_coords(int idx, const PhysicsParameters& p, double& x, double& y, double& z) {
    const int N = p.N;
    int i = idx / (N * N);
    int j = (idx % (N * N)) / N;
    int k = idx % N;
    x = i * p.h;
    y = j * p.h;
    z = k * p.h;
}

static void calculate_sources(double t, const PhysicsParameters& p, std::vector<double>& dJdt, std::vector<double>& curlJ) {
    const int N = p.N;
    const int N3 = N * N * N;
    dJdt.assign(3 * N3, 0.0);
    curlJ.assign(3 * N3, 0.0);

    double* dJdt_z = &dJdt[2 * N3];
    double* curlJ_x = &curlJ[0 * N3];
    double* curlJ_y = &curlJ[1 * N3];
    std::vector<double> Jz(N3, 0.0);

    for (int i = 0; i < N3; ++i) {
        double x, y, z;
        get_coords(i, p, x, y, z);
        double z_eff = z - 0.5 + 0.125 * cos(p.omega * t);
        double arg = -p.sigma * ((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + z_eff * z_eff);
        double rho_c = p.rho0 * exp(arg);
        
        Jz[i] = rho_c * (p.omega * sin(p.omega * t));
        double drho_c_dt = rho_c * (-p.sigma * (2.0 * z_eff * (-0.125 * p.omega * sin(p.omega * t))));
        double dv_z_dt = p.omega * p.omega * cos(p.omega * t);
        dJdt_z[i] = drho_c_dt * (p.omega * sin(p.omega * t)) + rho_c * dv_z_dt;
    }
    
    const double h_inv = 1.0 / p.h;
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int k = 1; k < N - 1; ++k) {
                int idx_local = index(i, j, k, N);
                curlJ_x[idx_local] = (Jz[index(i, j + 1, k, N)] - Jz[index(i, j - 1, k, N)]) * 0.5 * h_inv;
                curlJ_y[idx_local] = -(Jz[index(i + 1, j, k, N)] - Jz[index(i - 1, j, k, N)]) * 0.5 * h_inv;
            }
        }
    }
}

static void interpolate_fields(const std::vector<double>& y, const PhysicsParameters& p, double x, double y_pos, double z, double& Ex, double& Ey, double& Ez, double& Bx, double& By, double& Bz) {
    const int N = p.N;
    const int N3 = N * N * N;
    double x_idx = x / p.h;
    double y_idx = y_pos / p.h;
    double z_idx = z / p.h;

    int i0 = floor(x_idx); int i1 = i0 + 1;
    int j0 = floor(y_idx); int j1 = j0 + 1;
    int k0 = floor(z_idx); int k1 = k0 + 1;

    double xd = x_idx - i0;
    double yd = y_idx - j0;
    double zd = z_idx - k0;

    auto lerp = [&](const double* field) {
        double c00 = field[index(i0,j0,k0,N)]*(1-xd) + field[index(i1,j0,k0,N)]*xd;
        double c01 = field[index(i0,j0,k1,N)]*(1-xd) + field[index(i1,j0,k1,N)]*xd;
        double c10 = field[index(i0,j1,k0,N)]*(1-xd) + field[index(i1,j1,k0,N)]*xd;
        double c11 = field[index(i0,j1,k1,N)]*(1-xd) + field[index(i1,j1,k1,N)]*xd;
        double c0 = c00*(1-yd) + c10*yd;
        double c1 = c01*(1-yd) + c11*yd;
        return c0*(1-zd) + c1*zd;
    };

    Ex = lerp(&y[0 * N3]); Ey = lerp(&y[1 * N3]); Ez = lerp(&y[2 * N3]);
    Bx = lerp(&y[3 * N3]); By = lerp(&y[4 * N3]); Bz = lerp(&y[5 * N3]);
}