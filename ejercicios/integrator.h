#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <vector>
#include <functional>
#include <string>

// Alias para el estado del sistema (x, y, vx, vy)
using State = std::vector<double>;

// Alias para la función de derivadas, f(t, y)
using DerivativeFunc = std::function<State(double, const State&)>;

class AdaptiveIntegrator {
public:
    AdaptiveIntegrator(DerivativeFunc func, double tol);

    // Realiza la integración y guarda los resultados en archivos
    void integrate(const State& y0, double t_start, double t_end, double dt_initial);

private:
    State rk4_step(double t, const State& y, double h);
    
    DerivativeFunc m_func;
    double m_tol;
};

#endif // INTEGRATOR_H