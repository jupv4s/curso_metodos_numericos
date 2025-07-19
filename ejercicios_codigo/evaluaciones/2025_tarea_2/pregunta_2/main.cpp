#include <iostream>
#include <vector>
#include <string>
#include <cmath> // Para M_PI

#include "matriz_exponencial.h"

using namespace std;

int main()
{
    // Definimos la unidad imaginaria y el ángulo theta usando tu clase Complex
    const CustomComplex i(0.0, 1.0);
    const double theta = M_PI / 2.0;
    const double tolerance = 1e-9;

    // Definimos las matrices de Pauli usando CustomComplex
    Matrix<CustomComplex> sigma_x({{{0.0, 0.0}, {1.0, 0.0}},
                                   {{1.0, 0.0}, {0.0, 0.0}}});

    Matrix<CustomComplex> sigma_y({{{0.0, 0.0}, {0.0,-1.0}},
                                   {{0.0, 1.0}, {0.0, 0.0}}});

    Matrix<CustomComplex> sigma_z({{{1.0, 0.0}, {0.0, 0.0}},
                                   {{0.0, 0.0}, {-1.0,0.0}}});
    
    vector<pair<string, Matrix<CustomComplex>>> pauli_matrices = {
        {"sigma_x", sigma_x},
        {"sigma_y", sigma_y},
        {"sigma_z", sigma_z}
    };

    cout << "Calculando U(theta) = exp(i * theta * sigma) para theta = pi/2" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (const auto& pauli : pauli_matrices)
    {
        cout << "\n--- Matriz: " << pauli.first << " ---" << endl;

        // Argumento de la exponencial: A = i * theta * sigma
        Matrix<CustomComplex> A = (i * CustomComplex(theta)) * pauli.second;

        // Llamamos a las funciones definidas externamente
        Matrix<CustomComplex> U_euler = exponencial_euler(pauli.second, theta);
        Matrix<CustomComplex> U_taylor = exponencial_taylor(A, tolerance);
        
        cout << "\nResultado con Relacion de Euler:\n" << U_euler << endl;
        cout << "\nResultado con Serie de Taylor:\n" << U_taylor << endl;
        
        cout << "\nComparando resultados..." << endl;
        if (son_matrices_cercanas(U_euler, U_taylor, tolerance))
        {
            cout << "OK: Los resultados coinciden dentro de la tolerancia." << endl;
        }
        else
        {
            cout << "ERROR: Los resultados NO coinciden." << endl;
        }
        cout << "------------------------------------------------------------" << endl;
    }

    return 0;
}