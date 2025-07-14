#include <iostream>
#include "Vector.h"
#include "Dual.h"

Dual<double> x(2.0, 1.0);  // x = 2.0, con derivada 1.0 (como dx/dx = 1)

Vector<Dual<double>> f({
    x * x,         // f₁(x) = x² → f₁'(x) = 2x
    sin(x),        // f₂(x) = sin(x) → f₂'(x) = cos(x)
    exp(x)         // f₃(x) = eˣ → f₃'(x) = eˣ
});

std::cout << "Evaluación de f(x):\n";
for (std::size_t i = 0; i < f.size(); ++i)
{
    std::cout << "f" << i << "(x) = " << f[i].real()
              << ", f" << i << "'(x) = " << f[i].dual() << std::endl;
}