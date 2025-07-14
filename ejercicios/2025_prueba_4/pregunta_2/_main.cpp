#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "newton_solver.h" // Tu clase solver

// Para usar M_PI (valor de pi)
#define _USE_MATH_DEFINES
#include <math.h>

// Tipo de dato para los cálculos
using T = double;

// --- Funciones F y Jacobiana (definidas fuera de main para más claridad) ---

// Función vectorial F(x) = gradiente(phi)
std::vector<T> F_gradient(const std::vector<T>& x_vec) {
    T x = x_vec[0], y = x_vec[1], z = x_vec[2];
    std::vector<T> f(3);
    T term_comun = 2.0 * (z - std::cos(x) * std::cos(y));

    f[0] = std::sin(2.0 * x) + term_comun * std::sin(x) * std::cos(y);
    f[1] = std::sin(2.0 * y) + term_comun * std::cos(x) * std::sin(y);
    f[2] = term_comun;
    return f;
}

// Función que calcula la matriz Jacobiana J(x) analíticamente
Matrix<T> Jacobian_analytic(const std::vector<T>& x_vec) {
    T x = x_vec[0], y = x_vec[1], z = x_vec[2];
    Matrix<T> J(3, 3);
    T cosx = std::cos(x), sinx = std::sin(x);
    T cosy = std::cos(y), siny = std::sin(y);
    T u = z - cosx * cosy;

    J(0, 0) = 2.0*std::cos(2*x) + 2.0*sinx*cosy*sinx*cosy + 2.0*u*cosx*cosy;
    J(0, 1) = 2.0*cosx*siny*sinx*cosy - 2.0*u*sinx*siny;
    J(0, 2) = 2.0 * sinx * cosy;
    J(1, 0) = J(0, 1); // La matriz Hessiana es simétrica
    J(1, 1) = 2.0*std::cos(2*y) + 2.0*cosx*siny*cosx*siny + 2.0*u*cosx*cosy;
    J(1, 2) = 2.0 * cosx * siny;
    J(2, 0) = J(0, 2);
    J(2, 1) = J(1, 2);
    J(2, 2) = 2.0;
    return J;
}

// --- Función auxiliar para ejecutar el solver ---
void find_and_print_solution(
    const std::string& description,
    const std::vector<T>& initial_guess,
    const std::function<Matrix<T>(const std::vector<T>&)>& Jacobian_func)
{
    std::cout << "--- Buscando " << description << " ---" << std::endl;
    NewtonSolver<T> solver(initial_guess);
    std::vector<T> solution = solver.solve(F_gradient, Jacobian_func, 50, 1e-9);
    std::cout << "Solucion encontrada: " << solution << std::endl;
    std::cout << "\n--------------------------------------------------\n" << std::endl;
}


int main() {
    // Definimos los puntos iniciales para cada mínimo
    std::vector<std::vector<T>> initial_points = {
        {0.1, 0.1, 0.9},    // Cerca de (0, 0, 1)
        {3.0, 0.2, -0.8},   // Cerca de (pi, 0, -1)
        {0.2, 3.0, -0.8}    // Cerca de (0, pi, -1)
    };
    
    // Llamamos a la función auxiliar para cada punto inicial
    find_and_print_solution("primer minimo", initial_points[0], Jacobian_analytic);
    find_and_print_solution("segundo minimo", initial_points[1], Jacobian_analytic);
    find_and_print_solution("tercer minimo", initial_points[2], Jacobian_analytic);

    return 0;
}