#include <iostream>
#include <cmath>
#include "Dual.h"
#include "Complex.h"

using namespace std;

// definimos la exponencial para duales 
template <class T, size_t N>
Dual<T, N> exp(const Dual<T, N>& x)
{
    Dual<T, N> result;
    result[0] = exp(x[0]);
    for (size_t k = 1; k < N; ++k)
    {
        result[k] = x[k] * result[0];
    }
    return result;
}

// defininimos el cosh para duales
template <class T, size_t N>
Dual<T, N> cosh(const Dual<T, N>& x)
{
    return (exp(x) + exp(-x)) / Dual<T, N>(T(2.0, 0.0));
}

// ahora definimos la función f(z) del enunciado
// f(z) = \sum_{k=1}^{12} \frac{\cosh(kz)}{z^2 + k^2}
template <class T>
T f(const T& z) 
{
    T sum{};
    for (int k = 1; k <= 12; ++k)
    {
        if constexpr (is_same_v<T, Dual<Complex<double>, 2>>)
        {
            T k_val(Complex<double>(static_cast<double>(k), 0.0), Complex<double>(0.0, 0.0));
            sum = sum + cosh(k_val * z) / (z * z + k_val * k_val);
        }
        else if constexpr (is_same_v<T, Complex<double>>)
        {
            T k_val(static_cast<double>(k), 0.0);
            sum = sum + cosh(k_val * z) / (z * z + k_val * k_val);
        }
        else
        {
            T k_val(static_cast<double>(k));
            sum = sum + cosh(k_val * z) / (z * z + k_val * k_val);
        }
    }
    return sum;
}


int main()
{
    // definimos alias para evitar escribir tanto
    using Cxd = Complex<double>;
    using DualCxd = Dual<Cxd, 2>;

    // definimos el punto a evaluar z0 = i + 0.002
    Cxd z0(0.002, 1.0);

    // evaluamos f(z) en z0:
    DualCxd z_dual(z0, Cxd(1.0, 0.0));
    DualCxd result = f(z_dual);

    // mostramos en pantalla f(z0) y f'(z0)
    cout << "f(z0)     = " << result[0] << endl; // con result[0] = c_0 = f(z0)
    cout << "f'(z0)    = " << result[1] << endl; // con result[1] = c_1 = f'(z0)

    return 0;
}