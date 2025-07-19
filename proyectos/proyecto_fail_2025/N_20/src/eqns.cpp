#include "eqns.h"
#include <cmath>
#include <vector>
#include <functional> // Necesario para std::function

// Declaramos las funciones de ayuda como 'static' para limitar su alcance a este archivo.
static int indice(int i, int j, int k, int N);
static void obtenerCoordenadas(int idx, const ParametrosFisicos& config, double& x, double& y, double& z);
static void calcularFuentes(double t, const ParametrosFisicos& config, std::vector<double>& dJdt, std::vector<double>& curlJ);
static void interpolarCampos(const std::vector<double>& y, const ParametrosFisicos& config, double x, double y_pos, double z, double& Ex, double& Ey, double& Ez, double& Bx, double& By, double& Bz);


// --- Implementación de la función pública que crea la función del sistema ---

rk4::ODEFunction crearFuncionDelSistema(const ParametrosFisicos& config, const DiferenciasFinitas& fdm) {
    
    // Devolvemos una lambda que captura los parámetros y el operador por referencia.
    return [=](double t, const std::vector<double>& y) -> std::vector<double> {
        const int N = config.N;
        const int N3 = N * N * N;
        const size_t num_field_vars = 12 * N3;
        const double c2 = config.c * config.c;
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
        calcularFuentes(t, config, dJdt, curlJ);

        for (int i = 0; i < N3; ++i) {
            dydt[i + 0*N3] = VEx[i]; dydt[i + 1*N3] = VEy[i]; dydt[i + 2*N3] = VEz[i];
            dydt[i + 3*N3] = VBx[i]; dydt[i + 4*N3] = VBy[i]; dydt[i + 5*N3] = VBz[i];
            
            // Ecuación para E: d(V_E)/dt = c^2*nabla^2(E) - 4*pi*dJ/dt
            dydt[i + 6*N3] = c2 * lap_Ex[i] - four_pi * dJdt[i + 0*N3];
            dydt[i + 7*N3] = c2 * lap_Ey[i] - four_pi * dJdt[i + 1*N3];
            dydt[i + 8*N3] = c2 * lap_Ez[i] - four_pi * dJdt[i + 2*N3];
            
            // Ecuación para B: d(V_B)/dt = c^2*nabla^2(B) + 4*pi*c*curl(J)
            dydt[i + 9*N3]  = c2 * lap_Bx[i] + (four_pi * config.c) * curlJ[i + 0*N3];
            dydt[i + 10*N3] = c2 * lap_By[i] + (four_pi * config.c) * curlJ[i + 1*N3];
            dydt[i + 11*N3] = c2 * lap_Bz[i] + (four_pi * config.c) * curlJ[i + 2*N3];
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
            interpolarCampos(y, config, qx, qy, qz, E_p[0], E_p[1], E_p[2], B_p[0], B_p[1], B_p[2]);
            
            double Fx = config.q * (E_p[0] + (vy * B_p[2] - vz * B_p[1]) / config.c);
            double Fy = config.q * (E_p[1] + (vz * B_p[0] - vx * B_p[2]) / config.c);
            double Fz = config.q * (E_p[2] + (vx * B_p[1] - vy * B_p[0]) / config.c);

            dydt[num_field_vars + 0] = vx;
            dydt[num_field_vars + 1] = vy;
            dydt[num_field_vars + 2] = vz;
            dydt[num_field_vars + 3] = Fx / config.m;
            dydt[num_field_vars + 4] = Fy / config.m;
            dydt[num_field_vars + 5] = Fz / config.m;
        }
        
        return dydt;
    };
}


// --- Implementación de las funciones de ayuda estáticas ---

static int indice(int i, int j, int k, int N) {
    return i * N * N + j * N + k;
}

static void obtenerCoordenadas(int idx, const ParametrosFisicos& config, double& x, double& y, double& z) {
    const int N = config.N;
    int i = idx / (N * N);
    int j = (idx % (N * N)) / N;
    int k = idx % N;
    x = i * config.h;
    y = j * config.h;
    z = k * config.h;
}

static void calcularFuentes(double t, const ParametrosFisicos& config, std::vector<double>& dJdt, std::vector<double>& curlJ) {
    const int N = config.N;
    const int N3 = N * N * N;
    dJdt.assign(3 * N3, 0.0);
    curlJ.assign(3 * N3, 0.0);

    double* dJdt_z = &dJdt[2 * N3];
    double* curlJ_x = &curlJ[0 * N3];
    double* curlJ_y = &curlJ[1 * N3];
    std::vector<double> Jz(N3, 0.0);

    for (int i = 0; i < N3; ++i) {
        double x, y, z;
        obtenerCoordenadas(i, config, x, y, z);
        double z_eff = z - 0.5 + 0.125 * cos(config.omega * t);
        double arg = -config.sigma * ((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) + z_eff * z_eff);
        double rho_c = config.rho0 * exp(arg);
        
        Jz[i] = rho_c * (config.omega * sin(config.omega * t));
        double drho_c_dt = rho_c * (-config.sigma * (2.0 * z_eff * (-0.125 * config.omega * sin(config.omega * t))));
        double dv_z_dt = config.omega * config.omega * cos(config.omega * t);
        dJdt_z[i] = drho_c_dt * (config.omega * sin(config.omega * t)) + rho_c * dv_z_dt;
    }
    
    const double h_inv = 1.0 / config.h;
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int k = 1; k < N - 1; ++k) {
                int idx_local = indice(i, j, k, N);
                curlJ_x[idx_local] = (Jz[indice(i, j + 1, k, N)] - Jz[indice(i, j - 1, k, N)]) * 0.5 * h_inv;
                curlJ_y[idx_local] = -(Jz[indice(i + 1, j, k, N)] - Jz[indice(i - 1, j, k, N)]) * 0.5 * h_inv;
            }
        }
    }
}

static void interpolarCampos(const std::vector<double>& y, const ParametrosFisicos& config, double x, double y_pos, double z, double& Ex, double& Ey, double& Ez, double& Bx, double& By, double& Bz) {
    const int N = config.N;
    const int N3 = N * N * N;
    double x_idx = x / config.h;
    double y_idx = y_pos / config.h;
    double z_idx = z / config.h;

    int i0 = floor(x_idx); int i1 = i0 + 1;
    int j0 = floor(y_idx); int j1 = j0 + 1;
    int k0 = floor(z_idx); int k1 = k0 + 1;

    double xd = x_idx - i0;
    double yd = y_idx - j0;
    double zd = z_idx - k0;

    std::function<double(const double*)> lerp = [&](const double* field) {
        double c00 = field[indice(i0,j0,k0,N)]*(1-xd) + field[indice(i1,j0,k0,N)]*xd;
        double c01 = field[indice(i0,j0,k1,N)]*(1-xd) + field[indice(i1,j0,k1,N)]*xd;
        double c10 = field[indice(i0,j1,k0,N)]*(1-xd) + field[indice(i1,j1,k0,N)]*xd;
        double c11 = field[indice(i0,j1,k1,N)]*(1-xd) + field[indice(i1,j1,k1,N)]*xd;
        double c0 = c00*(1-yd) + c10*yd;
        double c1 = c01*(1-yd) + c11*yd;
        return c0*(1-zd) + c1*zd;
    };

    Ex = lerp(&y[0 * N3]); Ey = lerp(&y[1 * N3]); Ez = lerp(&y[2 * N3]);
    Bx = lerp(&y[3 * N3]); By = lerp(&y[4 * N3]); Bz = lerp(&y[5 * N3]);
}