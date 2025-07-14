#include <iostream>
#include <vector>
#include "Matrix.h" // Incluye tu clase Matrix<T>
#include "solver_linear_regression.h" // Incluye tu clase linear_regression

int main() {
    // 1. Definir el conjunto de datos de prueba
    // Para y = 2x + 1
    // Los datos se organizan como { {x1, y1}, {x2, y2}, ... }
    // En el formato de tu clase: { {x_predictor}, {y_response} }
    // En este caso, el dataset tiene una columna predictora y una de respuesta.
    std::vector<std::vector<double>> dataset = {
        {1.0, 3.0}, // (x, y)
        {2.0, 5.0}, // (x, y)
        {3.0, 7.0}, // (x, y)
        {4.0, 9.0}  // (x, y)
    };

    // 2. Crear una instancia de tu clase linear_regression
    linear_regression my_regression(dataset);

    // 3. Ejecutar la regresión lineal
    std::vector<double> coefficients = my_regression.lineal();

    // 4. Imprimir los resultados
    if (!coefficients.empty()) {
        std::cout << "Coeficientes de la regresión lineal:" << std::endl;
        // El primer coeficiente es el intercepto (b)
        std::cout << "  Intercepto (beta_0): " << coefficients[0] << std::endl;
        // El segundo coeficiente es la pendiente (m)
        std::cout << "  Pendiente (beta_1): " << coefficients[1] << std::endl;
        std::cout << "\nLo que se corresponde con la ecuación y = " 
                  << coefficients[1] << "x + " << coefficients[0] << std::endl;
    } else {
        std::cout << "No se pudieron calcular los coeficientes de regresión." << std::endl;
    }

    // --- Prueba con más predictores (opcional) ---
    // y = 3x1 + 2x2 + 1
    std::cout << "\n--- Prueba con dos predictores (x1, x2) ---" << std::endl;
    std::vector<std::vector<double>> dataset_multi = {
        {1.0, 1.0, 6.0}, // x1, x2, y
        {2.0, 1.0, 9.0},
        {1.0, 2.0, 8.0},
        {2.0, 2.0, 11.0}
    };
    linear_regression my_regression_multi(dataset_multi);
    std::vector<double> coefficients_multi = my_regression_multi.lineal();

    if (!coefficients_multi.empty()) {
        std::cout << "Coeficientes de la regresión lineal multivariable:" << std::endl;
        std::cout << "  Intercepto (beta_0): " << coefficients_multi[0] << std::endl;
        std::cout << "  Coeficiente de x1 (beta_1): " << coefficients_multi[1] << std::endl;
        std::cout << "  Coeficiente de x2 (beta_2): " << coefficients_multi[2] << std::endl;
    } else {
        std::cout << "No se pudieron calcular los coeficientes de regresión multivariable." << std::endl;
    }


    return 0;
}
