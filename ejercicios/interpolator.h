#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <vector>
#include <string>

// --- Interpolador de Lagrange (solicitado, pero no recomendado para muchos puntos) ---
class LagrangeInterpolator {
public:
    LagrangeInterpolator(const std::vector<double>& x, const std::vector<double>& y);
    double interpolate(double x_new) const;

private:
    std::vector<double> m_x;
    std::vector<double> m_y;
};


// --- Interpolador de Spline Cúbico ---
class CubicSplineInterpolator {
public:
    enum class BoundaryType { NATURAL, PERIODIC };
    CubicSplineInterpolator(const std::vector<double>& x, const std::vector<double>& y, BoundaryType type = BoundaryType::NATURAL);
    double interpolate(double x_new) const;

private:
    // Resuelve un sistema tridiagonal Ax=d
    std::vector<double> solve_tridiagonal(const std::vector<double>& a, const std::vector<double>& b, 
                                          const std::vector<double>& c, const std::vector<double>& d) const;
    
    std::vector<double> m_x;
    std::vector<double> m_y;
    std::vector<double> m_M; // Segundas derivadas en los nodos
};

#endif // INTERPOLATOR_H