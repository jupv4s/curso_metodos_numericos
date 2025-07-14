#include "matrix.h" // Incluimos nuestro archivo de encabezado de la clase matrix
#include <iostream> // Para std::cout y std::endl
#include <vector>   // Para std::vector

int main() {
    std::cout << "--- Demostracion de la clase matrix para 'double' ---" << std::endl;

    // 1. Creación de una matriz
    std::cout << "\n1. Creando una matriz 3x3 'A':" << std::endl;
    // Podemos inicializarla de varias maneras:
    // a) Usando el constructor con filas y columnas:
    // matrix A(3, 3);
    // A(0,0) = 1.0; A(0,1) = 2.0; A(0,2) = 3.0;
    // A(1,0) = 0.0; A(1,1) = 1.0; A(1,2) = 4.0;
    // A(2,0) = 5.0; A(2,1) = 6.0; A(2,2) = 0.0;

    // b) O usando un std::vector<std::vector<double>>
    std::vector<std::vector<double>> dataA = {
        {1.0, 2.0, 3.0},
        {0.0, 1.0, 4.0},
        {5.0, 6.0, 0.0}
    };
    matrix A(dataA);
    std::cout << "Matriz A:\n" << A << std::endl;

    // 2. Creación de un vector
    std::cout << "\n2. Creando un vector 'v':" << std::endl;
    std::vector<double> v = {10.0, 20.0, 30.0};
    std::cout << "Vector v:\n" << v << std::endl;

    // 3. Operación: Multiplicación de Matriz por Vector (A * v)
    std::cout << "\n3. Calculando el producto A * v:" << std::endl;
    if (A.ncol() == v.size()) {
        std::vector<double> result_Av = A * v;
        std::cout << "Resultado (A * v):\n" << result_Av << std::endl;
    } else {
        std::cout << "Error: Las dimensiones de la matriz y el vector no coinciden para la multiplicacion." << std::endl;
    }

    // 4. Operación: Transpuesta de la Matriz (A^T)
    std::cout << "\n4. Calculando la transpuesta de A (A^T):" << std::endl;
    matrix A_transposed = transpuesta(A);
    std::cout << "Matriz A^T:\n" << A_transposed << std::endl;

    // 5. Operación: Determinante de la Matriz (det(A))
    std::cout << "\n5. Calculando el determinante de A (det(A)):" << std::endl;
    // Asegurarse de que la matriz es cuadrada para el determinante
    if (A.nrow() == A.ncol()) {
        double det_A = determinante(A);
        std::cout << "Determinante de A: " << det_A << std::endl;

        // 6. Operación: Inversa de la Matriz (A^-1)
        std::cout << "\n6. Calculando la inversa de A (A^-1):" << std::endl;
        // Solo si el determinante no es cero (o muy cercano a cero)
        if (std::abs(det_A) > 1e-9) { // Usamos un pequeño epsilon para comparar doubles con cero
            matrix A_inverse = inversa(A);
            std::cout << "Matriz A^-1:\n" << A_inverse << std::endl;

            // Opcional: Verificar A * A^-1 (debería ser la identidad)
            std::cout << "\nVerificando A * A^-1 (deberia ser la matriz identidad):" << std::endl;
            matrix identity_check = A * A_inverse;
            std::cout << "A * A^-1:\n" << identity_check << std::endl;

        } else {
            std::cout << "El determinante es cero (o muy cercano a cero). La matriz A no tiene inversa." << std::endl;
        }
    } else {
        std::cout << "La matriz A no es cuadrada, no se puede calcular el determinante o la inversa." << std::endl;
    }

    std::cout << "\n--- Fin de la demostracion ---" << std::endl;

    return 0;
}
