#include <iostream>
#include <fstream>
#include <type_traits>
#include "Rational.h"
#include "Complex.h"
#include "Dual.h"

// Función f(z) = (z^3 - 3) / (z^5 - z^3 - 5)
template <class T>
T f(const T& z)
{
    T z3 = z * z * z;
    T z5 = z3 * z * z;
    
    // Crear constantes usando los constructores apropiados
    T three, five;
    
    // Para Complex<double>
    if constexpr (std::is_same_v<T, Complex<double>>) {
        three = T(3.0);
        five = T(5.0);
    }
    // Para Complex<Rational<long long>>
    else if constexpr (std::is_same_v<T, Complex<Rational<long long>>>) {
        three = T(Rational<long long>(3));
        five = T(Rational<long long>(5));
    }
    // Para Dual<Complex<double>, 4>
    else if constexpr (std::is_same_v<T, Dual<Complex<double>, 4>>) {
        three = T(Complex<double>(3.0));
        five = T(Complex<double>(5.0));
    }
    // Para otros tipos
    else {
        three = T(3);
        five = T(5);
    }
    
    return (z3 - three) / (z5 - z3 - five);
}

int main() {
    // Parte 1: Expansión de Taylor en z = i
    std::cout << "=== Expansion de Taylor en z = i ===" << std::endl;
    
    using Cxd = Complex<double>;
    using DualCxd = Dual<Cxd, 4>;
    
    // z = i + ε para diferenciación automática
    DualCxd z_dual(Cxd(0, 1), Cxd(1, 0));
    DualCxd result = f(z_dual);
    
    std::cout << "f(z) ≈ " << result[0] << std::endl;
    std::cout << "     + (" << result[1] << ")(z - i)" << std::endl;
    std::cout << "     + (" << result[2] << ")(z - i)^2" << std::endl;
    std::cout << "     + (" << result[3] << ")(z - i)^3" << std::endl;
    
    // Parte 2: Evaluación con racionales
    std::cout << "\n=== Evaluacion con racionales ===" << std::endl;
    
    using Rat = Rational<long long>;
    using Cxr = Complex<Rat>;
    
    Cxr z1(Rat(1, 2), Rat(1, 1));  // 0.5 + i
    Cxr z2(Rat(1, 1), Rat(1, 2));  // 1 + 0.5i
    
    auto r1 = f(z1);
    auto r2 = f(z2);
    
    std::cout << "f(0.5 + i) = " << r1 << std::endl;
    std::cout << "f(1 + 0.5i) = " << r2 << std::endl;
    
    // Guardar en archivo
    std::ofstream file("resultados.txt");
    if (file) {
        file << "f(0.5 + i) = " << r1 << std::endl;
        file << "f(1 + 0.5i) = " << r2 << std::endl;
        std::cout << "Resultados guardados en 'resultados.txt'" << std::endl;
    }
    
    return 0;
}