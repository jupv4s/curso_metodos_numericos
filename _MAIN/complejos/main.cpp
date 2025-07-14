#include <iostream>
#include "Complex.h"

int main()
{
    // Creamos algunos números complejos
    Complex<double> z1(3.0, 4.0);   // z1 = 3 + 4i
    Complex<double> z2(1.0, -2.0);  // z2 = 1 - 2i

    std::cout << "z1 = " << z1 << std::endl;
    std::cout << "z2 = " << z2 << std::endl;

    // Operaciones básicas
    std::cout << "z1 + z2 = " << (z1 + z2) << std::endl;
    std::cout << "z1 - z2 = " << (z1 - z2) << std::endl;
    std::cout << "z1 * z2 = " << (z1 * z2) << std::endl;
    std::cout << "z1 / z2 = " << (z1 / z2) << std::endl;

    // Funciones matemáticas
    std::cout << "abs(z1) = " << abs(z1) << std::endl;
    std::cout << "arg(z1) = " << arg(z1) << " radianes" << std::endl;
    std::cout << "conj(z2) = " << conj(z2) << std::endl;
    std::cout << "sqrt(z1) = " << sqrt(z1) << std::endl;
    std::cout << "exp(z1) = " << exp(z1) << std::endl;
    std::cout << "log(z1) = " << log(z1) << std::endl;

    // Forma polar
    std::cout << "Representación polar de z1: " << to_polar(z1) << std::endl;

    // Crear número complejo desde polar
    double r = 2.0;
    double theta = M_PI / 4; // 45 grados
    Complex<double> z3 = polar(r, theta);

    std::cout << "z3 (desde forma polar r=2, θ=π/4): " << z3 << std::endl;
    std::cout << "Representación polar de z3: " << to_polar(z3) << std::endl;

    return 0;