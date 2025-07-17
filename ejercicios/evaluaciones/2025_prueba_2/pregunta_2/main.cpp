#include <iostream>
#include <fstream>
#include "Polynomial.h"
#include "Complex.h"

using namespace std;

int main()
{
    // tamaño de la imagen
    //int width = 1600;
    //int height = 1600;
    // la imagen generada pesa 21.7 MiB, por lo que el tiempo de ejecución es relativamente mas alto

    int width = 800;
    int height = 800;
    // la imagen generada pesa 5.4 MiB, por lo que el tiempo de ejecución es relativamente mas bajo

    // numero maximo de iteraciones
    int max_iter = 100;
    // tolerancia para considerar que llego a una raiz
    double tol = 0.00001;
    // rango del plano complejo
    double xmin = -1.5;
    double xmax = 1.5;
    double ymin = -1.5;
    double ymax = 1.5;

    // definimos el polinomio z^4 - 1
    // los coeficientes son: -1, 0, 0, 0, 1 (de menor a mayor grado)
    Polynomial<Complex<double>> polinomio;
    polinomio[0] = Complex<double>(-1, 0); // termino independiente
    polinomio[1] = Complex<double>(0, 0);
    polinomio[2] = Complex<double>(0, 0);
    polinomio[3] = Complex<double>(0, 0);
    polinomio[4] = Complex<double>(1, 0); // z^4

    // derivada del polinomio
    Polynomial<Complex<double>> derivada = polinomio.derivative();

    // raices conocidas de z^4 - 1
    Complex<double> raiz1(1, 0);
    Complex<double> raiz2(-1, 0);
    Complex<double> raiz3(0, 1);
    Complex<double> raiz4(0, -1);

    // abrimos un archivo para expotrar a imagen formato PPM
    ofstream imagen("newton_fractal.ppm");
    imagen << "P3\n" << width << " " << height << "\n255\n";

    // recorremos cada pixel de la imagen
    for (int fila = 0; fila < height; fila++)
    {
        for (int columna = 0; columna < width; columna++)
        {
            // convertimos pixel a numero complejo
            double x = xmin + (xmax - xmin) * columna / (width - 1);
            double y = ymin + (ymax - ymin) * fila / (height - 1);
            Complex<double> z(x, y);

            int iteraciones = 0;
            // metodo de newton
            while (iteraciones < max_iter)
            {
                Complex<double> valor = polinomio(z);
                Complex<double> deriv = derivada(z);
                if (abs(deriv) < tol)
                {
                    break; // evitamos la division por cero
                }
                Complex<double> siguiente = z - valor / deriv;
                if (abs(siguiente - z) < tol)
                {
                    break;
                }
                z = siguiente;
                iteraciones++;
            }

            int color = 0;
            if (abs(z - raiz1) < tol)
            {
                color = 0;
            }
            else if (abs(z - raiz2) < tol)
            {
                color = 1;
            }
            else if (abs(z - raiz3) < tol)
            {
                color = 2;
            }
            else if (abs(z - raiz4) < tol)
            {
                color = 3;
            }

            // asignmaos colores simples para cada raiSz
            int r = 0;
            int g = 0;
            int b = 0;
            if (color == 0)
            {
                r = 255 - iteraciones * 8;
            }
            else if (color == 1)
            {
                g = 255 - iteraciones * 8;
            }
            else if (color == 2)
            {
                b = 255 - iteraciones * 8;
            }
            else if (color == 3)
            {
                r = g = b = 255 - iteraciones * 8;
            }
            else if (color == -1)
            {
               r = g = b = 0; // asignamos el color negro si no se alcanza la convergencia
            }
            // evitamos valores negativos
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