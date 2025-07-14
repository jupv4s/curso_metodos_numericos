#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cmath>

#include "Matrix.h"
#include "newton_solver.h"

// Definimos el tipo de dato para los cálculos
using T = double;
using namespace std;

vector<T> F_gradient(const vector<T>& x_vec)
{
    T x = x_vec[0], y = x_vec[1], z = x_vec[2];
    vector<T> f(3);
    
    T term_comun = 2.0 * (z - cos(x) * cos(y));

    f[0] = sin(2.0 * x) + term_comun * sin(x) * cos(y);
    f[1] = sin(2.0 * y) + term_comun * cos(x) * sin(y);
    f[2] = term_comun;
    
    return f;
}

Matrix<T> Jacobian_numeric(const vector<T>& x_vec)
{
    const T h = 1e-7; // Un paso de diferencia muy pequeño
    const int n = x_vec.size();
    Matrix<T> J(n, n);
    
    vector<T> f_original = F_gradient(x_vec);

    // Iteramos por cada columna de la Jacobiana
    for (int j = 0; j < n; ++j)
    {
        vector<T> x_perturbed = x_vec;
        x_perturbed[j] += h; // Perturbamos la j-ésima variable
        
        vector<T> f_perturbed = F_gradient(x_perturbed);
        
        // Calculamos la j-ésima columna usando la fórmula de la diferencia finita
        for (int i = 0; i < n; ++i)
        {
            J(i, j) = (f_perturbed[i] - f_original[i]) / h;
        }
    }
    return J;
}

void find_and_print_solution(
    const string& description,
    const vector<T>& initial_guess) 
{
    cout << "--- Buscando " << description << " ---" << endl;
    NewtonSolver<T> solver(initial_guess);
    
    // Llamamos al solver pasando directamente la Jacobiana numérica
    vector<T> solution = solver.solve(F_gradient, Jacobian_numeric, 50, 1e-9);
    
    cout << "Solucion encontrada: " << solution << endl;
    cout << "\n--------------------------------------------------\n" << endl;
}

int main()
{
    // Definimos los puntos iniciales para cada uno de los tres mínimos
    vector<vector<T>> initial_points = {
        {0.1, 0.1, 0.9},    // Punto cercano al mínimo en (0, 0, 1)
        {3.0, 0.2, -0.8},   // Punto cercano al mínimo en (pi, 0, -1)
        {0.2, 3.0, -0.8}    // Punto cercano al mínimo en (0, pi, -1)
    };
    
    // Ejecutamos el solver para cada punto inicial
    find_and_print_solution("primer minimo", initial_points[0]);
    find_and_print_solution("segundo minimo", initial_points[1]);
    find_and_print_solution("tercer minimo", initial_points[2]);

    return 0;
}