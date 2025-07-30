#include "../../plantillas/regresion/Matrix.h"
#include <iostream>
#include <vector>

int main() {
    // Datos de ejemplo (x, y)
    std::vector<std::vector<double>> datos = {
        {0.0, 1.0},
        {1.0, 6.0},
        {2.0, 15.0},
        {3.0, 28.0},
        {4.0, 45.0}
    };

    Matrix<double> X(datos.size(), 3);
    Matrix<double> Y(datos.size(), 1);
    for (std::size_t i = 0; i < datos.size(); ++i) {
        double x = datos[i][0];
        X(i, 0) = 1.0;      // Intercepto
        X(i, 1) = x;        // Término lineal
        X(i, 2) = x * x;    // Término cuadrático
        Y(i, 0) = datos[i][1];
    }

    Matrix<double> Xt = transpuesta(X);
    Matrix<double> XtX = Xt * X;
    Matrix<double> XtX_inv = inversa(XtX);
    if (XtX_inv.nrow() == 0) {
        std::cerr << "Error: no se pudo invertir la matriz" << std::endl;
        return 1;
    }

    Matrix<double> coef = (XtX_inv * Xt) * Y;

    std::cout << "Coeficientes calculados (c, b, a):\n";
    for (std::size_t i = 0; i < coef.nrow(); ++i) {
        std::cout << coef(i,0) << (i + 1 < coef.nrow() ? " " : "\n");
    }

    std::cout << "Modelo: y = a*x^2 + b*x + c" << std::endl;
    std::cout << "a = " << coef(2,0) << "\n";
    std::cout << "b = " << coef(1,0) << "\n";
    std::cout << "c = " << coef(0,0) << std::endl;

    return 0;
}
