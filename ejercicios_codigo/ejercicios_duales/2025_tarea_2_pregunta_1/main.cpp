#include <iostream>
#include <vector>

#include "Rational.h"
#include "Dual.h"       // La plantilla Dual generalizada
#include "Polynomial.h" // La nueva plantilla de Polinomios

// Alias para los tipos de datos
using RationalInt = Rational<long long>;
using DualRational = Dual<RationalInt, 5>;

using namespace std;

int main()
{
    // 1. Definimos el polinomio usando sus coeficientes racionales
    // p(x) = 11x⁴ + (4/23)x³ + x² + (9/14)x + 24
    Polynomial<RationalInt> p({
        RationalInt(24),       // Coeficiente de x^0
        RationalInt(9, 14),    // Coeficiente de x^1
        RationalInt(1),        // Coeficiente de x^2
        RationalInt(4, 23),    // Coeficiente de x^3
        RationalInt(11)        // Coeficiente de x^4
    });

    // 2. Punto de evaluación y número dual para la diferenciación
    RationalInt x0(3, 4);
    DualRational x_dual(x0, RationalInt(1)); // Representa x₀ + ε

    // 3. Evaluación del polinomio con el número dual
    // Aquí es donde la magia sucede:
    // El método p.evaluate() toma un DualRational y devuelve un DualRational.
    DualRational result = p.evaluate(x_dual);

    // 4. Factoriales para escalar los coeficientes (igual que antes)
    RationalInt fact2(2), fact3(6), fact4(24);

    // 5. Extracción de los resultados (igual que antes)
    RationalInt p_val = result[0];
    RationalInt p_d1  = result[1];
    RationalInt p_d2  = result[2] * fact2;
    RationalInt p_d3  = result[3] * fact3;
    RationalInt p_d4  = result[4] * fact4;

    // 6. Impresión de los resultados (igual que antes)
    cout << "Resultados para p(x) en x₀ = " << x0 << endl;
    cout << "------------------------------------------" << endl;
    cout << "p(x₀)    = " << p_val << endl;
    cout << "p'(x₀)   = " << p_d1 << endl;
    cout << "p''(x₀)  = " << p_d2 << endl;
    cout << "p'''(x₀) = " << p_d3 << endl;
    cout << "p(4)(x₀) = " << p_d4 << endl;

    return 0;
}