#include "Vector.h" // Incluye la plantilla Vector.h
#include "Matrix.h" // Incluye la plantilla Matrix.h
#include <iostream> // Para std::cout, std::endl
#include <vector>   // Para std::vector (si no está ya incluido por Matrix.h/Vector.h)
#include <iomanip>  // Para std::setprecision, std::fixed

int main()
{
    std::cout << "--- Demostración de la Clase Matrix ---" << std::endl;
    //std::cout << std::fixed << std::setprecision(4); // Formato de salida para flotantes

    // 1. Creación de matrices
    std::cout << "\n1. Creación de Matrices:" << std::endl;
    Matrix<double> m1(2, 3); // Matriz 2x3 inicializada con ceros
    std::cout << "Matriz m1 (2x3, ceros):\n" << m1 << std::endl;

    std::vector<std::vector<double>> vals = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    Matrix<double> m2(vals); // Matriz 2x3 inicializada con valores
    std::cout << "Matriz m2 (2x3, valores):\n" << m2 << std::endl;

    Matrix<double> identity_m(3); // Matriz identidad 3x3
    std::cout << "Matriz Identidad (3x3):\n" << identity_m << std::endl;

    // 2. Acceso a elementos
    std::cout << "\n2. Acceso a Elementos:" << std::endl;
    std::cout << "Elemento m2(0, 1): " << m2(0, 1) << std::endl; // Acceso de lectura
    m2(0, 1) = 99.0; // Acceso de escritura
    std::cout << "m2 después de m2(0, 1) = 99.0:\n" << m2 << std::endl;

    // Intentar acceder fuera de rango (causará una excepción)
    try {
        std::cout << "Intentando acceder a m2(5, 5)..." << std::endl;
        m2(5, 5) = 1.0;
    } catch (const std::out_of_range& e) {
        std::cerr << "Capturada excepción: " << e.what() << std::endl;
    }

    // 3. Operaciones Aritméticas
    std::cout << "\n3. Operaciones Aritméticas:" << std::endl;
    Matrix<double> m3({
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    });
    std::cout << "Matriz m3:\n" << m3 << std::endl;

    Matrix<double> sum_m = m2 + m3;
    std::cout << "m2 + m3:\n" << sum_m << std::endl;

    Matrix<double> diff_m = m3 - m2;
    std::cout << "m3 - m2:\n" << diff_m << std::endl;

    Matrix<double> scaled_m = 2.5 * m1;
    std::cout << "2.5 * m1 (matriz de ceros escalada):\n" << scaled_m << std::endl;

    // 4. Multiplicación de Matrices
    std::cout << "\n4. Multiplicación de Matrices:" << std::endl;
    Matrix<double> A({
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0}
    }); // 3x2
    Matrix<double> B({
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    }); // 2x3
    std::cout << "Matriz A (3x2):\n" << A << std::endl;
    std::cout << "Matriz B (2x3):\n" << B << std::endl;

    Matrix<double> C = A * B; // Resultado: 3x3
    std::cout << "A * B (Matriz C - 3x3):\n" << C << std::endl;

    // 5. Multiplicación Matriz por Vector y Vector por Matriz
    std::cout << "\n5. Multiplicación Matriz * Vector y Vector * Matriz:" << std::endl;
    Vector<double> v1({1.0, 2.0}); // Ahora usa el constructor con initializer_list de Vector
    std::cout << "Vector v1: " << v1 << std::endl;
    Vector<double> result_mv = A * v1; // Matriz A (3x2) * Vector v1 (2x1) -> Vector (3x1)
    std::cout << "A * v1: " << result_mv << std::endl;

    Vector<double> v2_compatible({1.0, 2.0}); // Vector de tamaño 2, compatible con B
    std::cout << "Vector v2_compatible: " << v2_compatible << std::endl;
    Vector<double> result_vm_corrected = v2_compatible * B; // Vector v2_compatible (1x2) * Matriz B (2x3) -> Vector (1x3)
    std::cout << "v2_compatible * B: " << result_vm_corrected << std::endl;

    // 6. Transposición
    std::cout << "\n6. Transposición:" << std::endl;
    Matrix<double> A_transposed = transpose(A);
    std::cout << "Transpuesta de A (original 3x2, transpuesta 2x3):\n" << A_transposed << std::endl;

    // 7. Eliminación Gaussiana, Determinante e Inversa
    std::cout << "\n7. Eliminación Gaussiana, Determinante e Inversa:" << std::endl;
    Matrix<double> D({
        {2.0, 1.0, -1.0},
        {-3.0, -1.0, 2.0},
        {-2.0, 1.0, 2.0}
    });
    std::cout << "Matriz D:\n" << D << std::endl;

    Matrix<double> E_gauss = gauss_elimination(D);
    std::cout << "D después de Eliminación Gaussiana:\n" << E_gauss << std::endl;

    double det_D = determinant(D);
    std::cout << "Determinante de D: " << det_D << std::endl;

    try {
        Matrix<double> D_inverse = inverse(D);
        std::cout << "Inversa de D:\n" << D_inverse << std::endl;

        // Verificar la inversa: D * D_inverse debería ser la identidad
        Matrix<double> check_identity = D * D_inverse;
        std::cout << "D * D_inverse (debería ser Identidad):\n" << check_identity << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Capturada excepción: " << e.what() << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Capturada excepción (argumento inválido): " << e.what() << std::endl;
    }

    // Ejemplo de matriz singular (determinante 0)
    std::cout << "\n--- Ejemplo de Matriz Singular ---" << std::endl;
    Matrix<double> singular_m({
        {1.0, 2.0},
        {2.0, 4.0}
    });
    std::cout << "Matriz Singular:\n" << singular_m << std::endl;
    double det_singular = determinant(singular_m);
    std::cout << "Determinante de Matriz Singular: " << det_singular << std::endl;
    try {
        Matrix<double> inv_singular = inverse(singular_m);
        std::cout << "Inversa de Matriz Singular:\n" << inv_singular << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Capturada excepción al calcular inversa de matriz singular: " << e.what() << std::endl;
    }

    // Demostración de norm() para un Vector<T>
    std::cout << "\n--- Demostración de norm() (método de la clase Vector) ---" << std::endl;
    Vector<double> vec_for_norm({3.0, 4.0, 0.0});
    std::cout << "Vector para norm: " << vec_for_norm << std::endl;
    std::cout << "Norma 2 del vector: " << vec_for_norm.norm() << std::endl;


    std::cout << "\n--- Fin de la Demostración ---" << std::endl;

    return 0;
}