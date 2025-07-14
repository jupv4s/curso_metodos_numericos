#include <iostream>   // Para std::cout, std::endl
#include "mlb_vector.h" // Incluye tu nueva clase mlb_vector

// Opcional: Si aún usas la clase 'matrix' y sus funciones
#include "mlb_matrix.h"

int main()
{
    std::cout << "--- Usando mlb_vector (ámbito global) ---" << std::endl;

    // Crear mlb_vector directamente (sin mlb::)
    mlb_vector<double> v1(3); // Vector de 3 elementos, inicializados a 0.0
    v1[0] = 1.0;
    v1[1] = 2.0;
    v1[2] = 3.0;
    std::cout << "v1: " << v1 << std::endl;

    mlb_vector<double> v2({4.0, 5.0, 6.0}); // Inicializar desde un std::vector (o initializer list)
    std::cout << "v2: " << v2 << std::endl;

    // Operaciones con mlb_vector
    mlb_vector<double> v_sum = v1 + v2;
    std::cout << "v1 + v2: " << v_sum << std::endl;

    mlb_vector<double> v_scaled = 2.5 * v1;
    std::cout << "2.5 * v1: " << v_scaled << std::endl;

    double dot_product = v1 * v2; // Producto escalar
    std::cout << "v1 . v2 (producto escalar): " << dot_product << std::endl;

    // Modulo
    std::cout << "Modulo de v1: " << v1.modulo() << std::endl;
 
    return 0;
}
