#ifndef NEWTON_SOLVER_H
#define NEWTON_SOLVER_H

#include <vector>
#include <functional> // Para std::function
#include <iostream>   // Para cout, cerr
#include <cmath>      // Para std::abs
#include <iomanip>    // Para std::setprecision
#include <limits>     // Para std::numeric_limits

#include "Matrix.h"

//int contador_gauss = 0;

template <typename T>
class NewtonSolver
{
private:
    std::vector<T> initial_guess; // Valor inicial del vector de incógnitas
    int dimension;                // Número de variables

public:
    // Constructor
    explicit NewtonSolver(std::vector<T> initial_guess_vec);

    // Método para resolver el sistema no lineal usando Newton
    // 'F': Función vectorial F(x) = 0
    // 'Jacobian': Función que calcula la matriz Jacobiana J(x)
    // 'max_iterations': Número máximo de iteraciones
    // 'tolerance': Tolerancia para el criterio de convergencia
    std::vector<T> solve(
        std::function<std::vector<T>(const std::vector<T>&)> F,
        std::function<Matrix<T>(const std::vector<T>&)> Jacobian,
        int max_iterations,
        T tolerance
    );
};

template <typename T>
NewtonSolver<T>::NewtonSolver(std::vector<T> initial_guess_vec)
    : initial_guess(std::move(initial_guess_vec)),
      dimension(static_cast<int>(initial_guess.size())) {}

template <typename T>
std::vector<T> NewtonSolver<T>::solve(
    std::function<std::vector<T>(const std::vector<T>&)> F,
    std::function<Matrix<T>(const std::vector<T>&)> Jacobian,
    int max_iterations,
    T tolerance
) {
    std::vector<T> x = initial_guess;
    std::vector<T> delta_x(dimension);

    std::cout << std::fixed << std::setprecision(8);

    for (int i = 0; i < max_iterations; ++i)
    {
        std::vector<T> f_val = F(x);
        Matrix<T> J_val = Jacobian(x);

        // Verificación de singularidad del Jacobiano
        T det_J = determinante(J_val);
        if (std::abs(det_J) < std::numeric_limits<T>::epsilon() * 1e3)
        {
            std::cerr << "Advertencia: Jacobiano casi singular en la iteración " << i << ". Determinante: " << det_J << std::endl;
        }

        Matrix<T> J_inv = inversa(J_val);

        // Cálculo de delta_x = J(x_n)^-1 * F(x_n)
        delta_x = J_inv * f_val;

        // Actualización de x: x_{n+1} = x_n - delta_x
        for (int j = 0; j < dimension; ++j) {
            x[j] = x[j] - delta_x[j];
        }

        // Criterio de convergencia: ||delta_x|| < tolerancia
        T current_norm = norm2(delta_x);
        std::cout << "Iteracion " << i + 1 << ": x = " << x << ", ||delta_x|| = " << current_norm << std::endl;

        if (current_norm < tolerance)
        {
            std::cout << "Convergencia alcanzada en " << i + 1 << " iteraciones." << std::endl;
            return x;
        }
    }

    std::cerr << "Advertencia: El método de Newton no convergió en " << max_iterations << " iteraciones." << std::endl;
    return x; // Retorna la última aproximación
}

#endif
