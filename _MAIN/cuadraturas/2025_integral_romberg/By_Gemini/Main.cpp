#include <iostream>
#include <cmath>
#include <functional> // Para std::function
#include <iomanip>    // Para std::setprecision y std::fixed

// Asegúrate de incluir tu clase NumericalIntegrator y Matrix
#include "numerical_integration.h"
#include "Matrix.h" // Necesaria porque NumericalIntegrator::Romberg la usa internamente

// Puedes definir pi como una constante. M_PI está en <cmath>
const double PI = M_PI; 

int main() {
    // Configurar la salida para mostrar al menos seis cifras significativas
    // y para formato fijo (sin notación científica a menos que sea muy pequeño/grande)
    std::cout << std::fixed << std::setprecision(10); 

    // Definir la función a integrar f(t) = g(r(t)) * ||dr(t)/dt||
    // Esta es la función completa a integrar en el intervalo [0, 1].
    auto integrand_f = [&](double t) {
        // 1. Componentes de la curva parametrizada r(t) = (cos(pi*t), t*sin(pi*t))
        double x_t = std::cos(PI * t);
        double y_t = t * std::sin(PI * t);

        // 2. Evaluar g(x,y) = x*sin(y) + 2 en r(t)
        double g_r_t = x_t * std::sin(y_t) + 2.0;

        // 3. Calcular las componentes de la derivada dr(t)/dt
        // dx/dt = -pi*sin(pi*t)
        double dx_dt = -PI * std::sin(PI * t);
        // dy/dt = sin(pi*t) + pi*t*cos(pi*t) (usando la regla del producto)
        double dy_dt = std::sin(PI * t) + PI * t * std::cos(PI * t);

        // 4. Calcular la magnitud (norma) de dr(t)/dt = sqrt((dx/dt)^2 + (dy/dt)^2)
        double norm_dr_dt = std::sqrt(dx_dt * dx_dt + dy_dt * dy_dt);

        // 5. El integrando final es el producto de g(r(t)) y ||dr(t)/dt||
        return g_r_t * norm_dr_dt;
    };

    // Límites de integración para t: [0, 1] 
    double a = 0.0;
    double b = 1.0;

    // Tolerancia requerida para el método de Romberg: 10^-6 
    double tolerance = 1e-6;

    // Crear una instancia del integrador numérico con los límites definidos
    NumericalIntegrator integrator(a, b);

    // Calcular la integral utilizando el método de Romberg con la tolerancia especificada
    double integral_value = integrator.Romberg(integrand_f, tolerance);

    // Imprimir el valor de la integral con la precisión requerida
    // El problema pide al menos seis cifras significativas para la raíz del problema 1a,
    // pero para la integral se especifica la tolerancia, por lo que una alta precisión es apropiada.
    std::cout << "El valor de la integral es: " << integral_value << std::endl;

    return 0;
}
