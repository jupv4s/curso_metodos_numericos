#include "interpolator.h"
#include <stdexcept>
#include <algorithm> // Para std::upper_bound

// --- Implementación de LagrangeInterpolator ---

LagrangeInterpolator::LagrangeInterpolator(const std::vector<double>& x, const std::vector<double>& y)
    : m_x(x), m_y(y) {
    if (m_x.size() != m_y.size() || m_x.empty()) {
        throw std::invalid_argument("Los vectores de datos de Lagrange deben tener el mismo tamaño y no ser vacíos.");
    }
}

double LagrangeInterpolator::interpolate(double x_new) const {
    double result = 0.0;
    for (size_t j = 0; j < m_x.size(); ++j) {
        double basis_poly = 1.0;
        for (size_t k = 0; k < m_x.size(); ++k) {
            if (k != j) {
                basis_poly *= (x_new - m_x[k]) / (m_x[j] - m_x[k]);
            }
        }
        result += m_y[j] * basis_poly;
    }
    return result;
}


// --- Implementación de CubicSplineInterpolator ---

CubicSplineInterpolator::CubicSplineInterpolator(const std::vector<double>& x, const std::vector<double>& y, BoundaryType type)
    : m_x(x), m_y(y) {
    if (x.size() != y.size() || x.size() < 2) {
        throw std::invalid_argument("Los vectores de datos de Spline deben tener el mismo tamaño y al menos 2 puntos.");
    }

    int n = x.size();
    std::vector<double> h(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        h[i] = x[i+1] - x[i];
    }

    // Configurar sistema tridiagonal A*M = d
    std::vector<double> A_diag(n), A_upper(n - 1), A_lower(n - 1);
    std::vector<double> d(n);

    for (int i = 1; i < n - 1; ++i) {
        A_diag[i] = 2.0 * (h[i-1] + h[i]);
        A_upper[i] = h[i];
        A_lower[i-1] = h[i-1];
        d[i] = 6.0 * ((y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1]);
    }

    if (type == BoundaryType::NATURAL) {
        A_diag[0] = 1.0;
        d[0] = 0.0;
        A_diag[n - 1] = 1.0;
        d[n - 1] = 0.0;
    } else { // PERIODIC no está implementado aquí, pero la estructura lo permite
        throw std::runtime_error("Spline periódico no implementado en esta versión.");
    }

    // Resolver para M
    m_M = solve_tridiagonal(A_lower, A_diag, A_upper, d);
}

double CubicSplineInterpolator::interpolate(double x_new) const {
    // Encontrar el intervalo correcto [x_j, x_{j+1}]
    auto it = std::upper_bound(m_x.begin(), m_x.end(), x_new);
    if (it == m_x.begin()) it++;
    if (it == m_x.end()) it--;
    
    int j = std::distance(m_x.begin(), it) - 1;

    double h_j = m_x[j+1] - m_x[j];
    
    double A = (m_x[j+1] - x_new) / h_j;
    double B = (x_new - m_x[j]) / h_j;
    double C = (A*A*A - A) * h_j*h_j / 6.0;
    double D = (B*B*B - B) * h_j*h_j / 6.0;
    
    return A * m_y[j] + B * m_y[j+1] + C * m_M[j] + D * m_M[j+1];
}

// Implementación del Algoritmo de Thomas para resolver sistemas tridiagonales
std::vector<double> CubicSplineInterpolator::solve_tridiagonal(
    const std::vector<double>& a, const std::vector<double>& b, 
    const std::vector<double>& c, const std::vector<double>& d) const {
    int n = d.size();
    std::vector<double> c_prime(n, 0.0);
    std::vector<double> d_prime(n, 0.0);
    std::vector<double> x(n);

    // Forward elimination
    c_prime[0] = c[0] / b[0];
    d_prime[0] = d[0] / b[0];
    for (int i = 1; i < n; ++i) {
        double m = 1.0 / (b[i] - a[i-1] * c_prime[i-1]);
        if (i < n-1) c_prime[i] = c[i] * m;
        d_prime[i] = (d[i] - a[i-1] * d_prime[i-1]) * m;
    }

    // Back substitution
    x[n - 1] = d_prime[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        x[i] = d_prime[i] - c_prime[i] * x[i+1];
    }
    return x;
}