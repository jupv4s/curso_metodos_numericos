#include <iostream>
#include "Vector.h"

int main()
{
    Vector<double> u({3.0, -2.0, 1.0});
    Vector<double> v({1.0, 0.0, 4.0});

    std::cout << "u = " << u << std::endl;
    std::cout << "v = " << v << std::endl;

    std::cout << "u + v = " << (u + v) << std::endl;
    std::cout << "u - v = " << (u - v) << std::endl;
    std::cout << "Producto escalar u · v = " << (u * v) << std::endl;
    std::cout << "Producto vectorial u x v = " << cross(u, v) << std::endl;

    std::cout << "||u|| = " << norm(u) << std::endl;
    std::cout << "Vector unitario de u = " << normalize(u) << std::endl;

    return 0;
}