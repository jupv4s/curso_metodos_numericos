#include "functionAUX.h"
#include <iostream>      // Para std::cout, std::fixed, std::setprecision, std::endl
#include <iomanip>       // Para std::fixed, std::setprecision


using namespace std;

const double PI = M_PI;

void solveProblem1b()
{
    // Configuramos la salida
    cout << fixed << setprecision(10); 

    // Definimos la función a integrar f(t) = g(r(t)) * ||dr(t)/dt||
    auto integrand_f = [&](double t)
    {
        // Componentes de r(t)
        double x_t = cos(PI * t);
        double y_t = t * sin(PI * t);

        // g(x,y) = x*sin(y) + 2
        double g_r_t = x_t * sin(y_t) + 2.0;

        // Componentes de dr(t)/dt
        double dx_dt = -PI * sin(PI * t);
        double dy_dt = sin(PI * t) + PI * t * cos(PI * t);

        // Magnitud de dr(t)/dt
        double norm_dr_dt = sqrt(dx_dt * dx_dt + dy_dt * dy_dt);

        return g_r_t * norm_dr_dt;
    };

    // Límites de integración [0, 1]
    double a = 0.0;
    double b = 1.0;

    // Tolerancia
    double tolerance = 1e-6;

    // Creamos una instancia del integrador
    NumericalIntegrator integrator(a, b);

    // Calculamos la integral
    double integral_value = integrator.Romberg(integrand_f, tolerance);

    // Imprimimos el resultado
    cout << "El valor de la integral es: " << integral_value << endl;
}
