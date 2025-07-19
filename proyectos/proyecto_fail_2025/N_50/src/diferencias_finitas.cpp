#include "diferencias_finitas.h"
#include <algorithm>
//#include <omp.h>

DiferenciasFinitas::DiferenciasFinitas(int N_val, double L_val)
    : N(N_val), h(L_val / (N_val - 1)), h2_inv(1.0 / (h * h)) {}

int DiferenciasFinitas::index(int i, int j, int k) const {
    i = std::max(0, std::min(N - 1, i));
    j = std::max(0, std::min(N - 1, j));
    k = std::max(0, std::min(N - 1, k));
    return i * N * N + j * N + k;
}

void DiferenciasFinitas::apply(const std::vector<double>& u, std::vector<double>& Lu) const {
    Lu.assign(N * N * N, 0.0);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                if (i == 0 || i == N - 1 || j == 0 || j == N - 1 || k == 0 || k == N - 1) {
                    Lu[index(i, j, k)] = 0.0;
                    continue;
                }
                const double u_center = u[index(i, j, k)];
                const double u_px = u[index(i + 1, j, k)];
                const double u_nx = u[index(i - 1, j, k)];
                const double u_py = u[index(i, j + 1, k)];
                const double u_ny = u[index(i, j - 1, k)];
                const double u_pz = u[index(i, j, k + 1)];
                const double u_nz = u[index(i, j, k - 1)];
                Lu[index(i, j, k)] = (u_px + u_nx + u_py + u_ny + u_pz + u_nz - 6.0 * u_center) * h2_inv;
            }
        }
    }
}