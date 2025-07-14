#include "numerical_integration.h" // Incluye la declaración de la clase
#include "Matrix.h"                     // Se asume que tu clase Matrix<T> está disponible aquí.
#include <cmath>                        // Para std::pow, std::abs
#include <limits>                       // Para std::numeric_limits<double>::quiet_NaN()

int contador_gauss = 0;

// Constructor por defecto
NumericalIntegrator::NumericalIntegrator() : posicion_inicial(0.0), posicion_final(1.0) {}

// Destructor
NumericalIntegrator::~NumericalIntegrator() {}

// Constructor parametrizado
NumericalIntegrator::NumericalIntegrator(double a, double b) : posicion_inicial(a), posicion_final(b) 
{
    if (a >= b) {
        std::cerr << "Advertencia: El límite inicial debe ser menor que el límite final. Intercambiando valores." << std::endl;
        posicion_inicial = b;
        posicion_final = a;
    }
}

// Regla del Trapecio Compuesta
double NumericalIntegrator::Trapecio(std::function<double(double)> f, int n_subintervals)
{
    if (n_subintervals <= 0) {
        std::cerr << "Error: El número de subintervalos debe ser positivo para el método del Trapecio." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double h = (posicion_final - posicion_inicial) / n_subintervals;
    double resultado = 0.5 * (f(posicion_inicial) + f(posicion_final));

    for (int i = 1; i < n_subintervals; ++i) {
        resultado += f(posicion_inicial + i * h);
    }
    return resultado * h;
}

// Método de Romberg
double NumericalIntegrator::Romberg(std::function<double(double)> f, double tolerance)
{
    // Límite superior para el número de iteraciones para evitar bucles infinitos.
    // Una tabla de Romberg de tamaño 20x20 es más que suficiente para la mayoría de los casos.
    const int max_iterations = 20; 
    Matrix<double> R(max_iterations, max_iterations); 

    // Inicialización del primer elemento de la tabla R(0,0) con la regla del Trapecio
    // usando 1 subintervalo (h = b - a)
    double h = (posicion_final - posicion_inicial);
    R(0, 0) = 0.5 * h * (f(posicion_inicial) + f(posicion_final)); // T_0 (con N=1 subintervalo, o k=0 en 2^k)

    // Bucle principal para las iteraciones (k representa el nivel de refinamiento, de 1 a max_iterations-1)
    for (int k = 1; k < max_iterations; ++k) {
        // Calcular R(k,0) que corresponde a la aproximación del Trapecio con 2^k subintervalos.
        // Se utiliza la fórmula mejorada para obtener T_k a partir de T_{k-1} y los nuevos puntos medios.
        double sum_f_values_odd = 0.0;
        int num_intervals_prev_level = static_cast<int>(std::pow(2, k - 1)); // Número de intervalos en el nivel anterior
        h /= 2.0; // El tamaño del paso se reduce a la mitad en cada nuevo nivel

        // Sumar los valores de la función en los nuevos puntos intermedios impares
        // (los que no fueron evaluados en el nivel anterior)
        for (int i = 1; i <= num_intervals_prev_level; ++i) {
            sum_f_values_odd += f(posicion_inicial + (2 * i - 1) * h);
        }
        R(k, 0) = 0.5 * R(k-1, 0) + h * sum_f_values_odd; // Cálculo de T_k (R(k,0))

        // Extrapolación de Richardson para llenar la fila actual de la tabla de Romberg
        // (j representa el orden de la extrapolación, de 1 hasta k)
        for (int j = 1; j <= k; ++j) {
            double power_of_4 = std::pow(4, j);
            R(k, j) = R(k, j - 1) + (R(k, j - 1) - R(k - 1, j - 1)) / (power_of_4 - 1);
        }

        // Verificar la condición de convergencia después de calcular la última aproximación en la diagonal.
        // Comparamos el valor más refinado en la diagonal actual R(k,k) con el anterior R(k-1, k-1).
        if (k >= 1) { // Necesitamos al menos dos filas (k=0 y k=1) para hacer esta comparación.
            if (std::abs(R(k, k) - R(k - 1, k - 1)) < tolerance) {
                // Si la diferencia es menor que la tolerancia, consideramos que ha convergido.
                // Se retorna la aproximación más reciente y refinada.
                return R(k, k); 
            }
        }
    }
    // Si el método no converge dentro del número máximo de iteraciones,
    // se imprime una advertencia y se retorna la mejor aproximación encontrada.
    std::cerr << "Advertencia: El método de Romberg no convergió dentro del número máximo de iteraciones (" 
              << max_iterations << "). Retornando la mejor aproximación encontrada." << std::endl;
    return R(max_iterations - 1, max_iterations - 1); // Retorna el último valor calculado en la diagonal.
}
