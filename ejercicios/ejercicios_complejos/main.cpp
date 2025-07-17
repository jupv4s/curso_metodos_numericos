#include <iostream>
#include <fstream>
#include <cmath>
#include "Complex.h"
#include "Polynomial.h"

using namespace std;

int main()
{
    // Tamaño de la imagen
    int width = 400;
    int height = 400;
    // Número máximo de iteraciones
    int max_iter = 30;
    // Tolerancia para considerar que llegó a una raíz
    double tol = 0.0001;
    // Rango del plano complejo
    double xmin = -1.5;
    double xmax = 1.5;
    double ymin = -1.5;
    double ymax = 1.5;

    // Definir el polinomio z^3 - 1
    Polynomial<Complex<double>> polinomio;
    polinomio[0] = Complex<double>(-1, 0); // término independiente
    polinomio[1] = Complex<double>(0, 0);
    polinomio[2] = Complex<double>(0, 0);
    polinomio[3] = Complex<double>(1, 0); // z^3

    // Derivada del polinomio
    Polynomial<Complex<double>> derivada = polinomio.derivative();

    // Raíces conocidas de z^3 - 1
    Complex<double> raiz1(1, 0);
    double re = -0.5;
    double im = std::sqrt(3.0) / 2.0;
    Complex<double> raiz2(re, im);
    Complex<double> raiz3(re, -im);

    // Abrir archivo para imagen PPM
    ofstream imagen("newton_fractal.ppm");
    imagen << "P3\n" << width << " " << height << "\n255\n";

    // Recorrer cada píxel de la imagen
    for (int fila = 0; fila < height; fila++)
    {
        for (int columna = 0; columna < width; columna++)
        {
            // Convertir píxel a número complejo
            double x = xmin + (xmax - xmin) * columna / (width - 1);
            double y = ymin + (ymax - ymin) * fila / (height - 1);
            Complex<double> z(x, y);

            int iteraciones = 0;
            // Método de Newton
            while (iteraciones < max_iter)
            {
                Complex<double> valor = polinomio(z);
                Complex<double> deriv = derivada(z);
                if (abs(deriv) < tol)
                {
                    break; // Evitar división por cero
                }
                Complex<double> siguiente = z - valor / deriv;
                if (abs(siguiente - z) < tol)
                {
                    break; // Ya llegó a una raíz
                }
                z = siguiente;
                iteraciones++;
            }

            // Ver a qué raíz llegó
            int color = 0;
            if (abs(z - raiz1) < tol)
            {
                color = 0;
            } else if (abs(z - raiz2) < tol)
            {
                color = 1;
            } else if (abs(z - raiz3) < tol)
            {
                color = 2;
            }

            // Colores simples para cada raíz
            int r = 0;
            int g = 0;
            int b = 0;
            if (color == 0) {
                r = 255 - iteraciones * 8;
            } else if (color == 1)
            {
                g = 255 - iteraciones * 8;
            } else if (color == 2)
            {
                b = 255 - iteraciones * 8;
            }
            // Evitar valores negativos
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;

            imagen << r << " " << g << " " << b << " ";
        }
        imagen << "\n";
    }
    imagen.close();
    return 0;
}