#include "numerical_integration.h"
#include "Matrix.h"
#include <cmath>                        // Para std::pow, std::abs, std::sqrt
#include <limits>                       // Para std::numeric_limits
#include <random>                       // Para std::random_device, std::mt19937, std::uniform_real_distribution
#include <algorithm>                    // Para std::swap

// Si 'contador_gauss' se usa en Matrix.h y Matrix.cpp, debería ser 'extern' en Matrix.h
// y definido en *un solo* .cpp (ej. Matrix.cpp o un nuevo globals.cpp).
// Lo elimino de aquí para evitar posibles redefiniciones.
// Si lo necesitas aquí y no en Matrix.cpp, por favor, avísame.
int contador_gauss = 0;

NumericalIntegrator::NumericalIntegrator() : posicion_inicial(0.0), posicion_final(1.0) {}

NumericalIntegrator::~NumericalIntegrator() {}

NumericalIntegrator::NumericalIntegrator(double a, double b) : posicion_inicial(a), posicion_final(b)
{
    if (posicion_inicial >= posicion_final)
    {
        std::cerr << "Advertencia: El límite inicial debe ser menor que el límite final. Intercambiando valores." << std::endl;
        std::swap(posicion_inicial, posicion_final);
    }
}

double NumericalIntegrator::Trapecio(std::function<double(double)> f, int n_subintervals)
{
    if (n_subintervals <= 0)
    {
        std::cerr << "Error: El número de subintervalos debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double h = (posicion_final - posicion_inicial) / n_subintervals;
    double resultado = 0.5 * (f(posicion_inicial) + f(posicion_final));

    for (int i = 1; i < n_subintervals; ++i)
    {
        resultado += f(posicion_inicial + i * h);
    }
    return resultado * h;
}

double NumericalIntegrator::Romberg(std::function<double(double)> f, double tolerance)
{
    const int max_iterations = 20;
    Matrix<double> R(max_iterations, max_iterations);

    double h = (posicion_final - posicion_inicial);
    R(0, 0) = 0.5 * h * (f(posicion_inicial) + f(posicion_final)); // T_0

    for (int k = 1; k < max_iterations; ++k)
    {
        double sum_f_values_odd = 0.0;
        int num_intervals_prev_level = static_cast<int>(std::pow(2, k - 1));
        h /= 2.0;

        for (int i = 1; i <= num_intervals_prev_level; ++i)
        {
            sum_f_values_odd += f(posicion_inicial + (2 * i - 1) * h);
        }
        R(k, 0) = 0.5 * R(k-1, 0) + h * sum_f_values_odd; // T_k

        for (int j = 1; j <= k; ++j)
        {
            double power_of_4 = std::pow(4, j);
            R(k, j) = R(k, j - 1) + (R(k, j - 1) - R(k - 1, j - 1)) / (power_of_4 - 1);
        }

        if (k >= 1 && std::abs(R(k, k) - R(k - 1, k - 1)) < tolerance)
        {
            return R(k, k);
        }
    }
    std::cerr << "Advertencia: El método de Romberg no convergió dentro del número máximo de iteraciones ("
              << max_iterations << "). Retornando la mejor aproximación encontrada." << std::endl;
    return R(max_iterations - 1, max_iterations - 1);
}
