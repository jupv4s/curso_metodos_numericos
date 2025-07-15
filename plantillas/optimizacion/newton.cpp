#include <iostream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

// metodo de newton para calcular raices
double newton(double a, double epsilon, int iteraciones, double (*f)(double), double (*f_derivada)(double))
{
    // se inicializan las variables
    double raiz = a;
    int iteracion = 0;

    // se itera para encontrar las raices
    while(iteracion < iteraciones)
    {
        raiz = raiz - f(raiz)/f_derivada(raiz);
        iteracion += 1;
    }
    return raiz;
}

// metodo de integracion del trapecio
double trapecio(double (*f)(double), double a, double b, double numero_paso)
{
    double n = numero_paso;
    double x = a; // posicion a iterar, partiendo desde "a" del intervalo (a, b)
    double h = (b - a)/n;
    double resultado = 0;

    while (x < b)
    {
        double promedio = 0.5*(f(x) + f(x + h));
        double integracion_i = h*promedio;
        resultado += integracion_i;
        x = x + h;
    }
    return resultado;
}

// metodo de integracion de montecarlo
double montecarlo(double (*f)(double), double a, double b, int numero_paso)
{
    // Generador de números aleatorios en el intervalo [a, b]
    //random_device rd;
    //mt19937 gen(rd());
    int seed = 300;
    mt19937 gen(seed);
    uniform_real_distribution<> dis(a, b);

    double n = numero_paso;
    double suma = 0.0;

    // Generamos N puntos aleatorios en el intervalo [a, b]
    for (int i = 0; i < n; ++i)
    {
        double x = dis(gen);
        suma += f(x);
    }

    // La integral aproximada es el promedio de los valores de la función multiplicado por el ancho del intervalo
    return (b - a)*suma/n;
}

// declaramos la funcion a la que se le quiere hallar sus raices
// f(x) = x^3 - 9x^2 + 23x - 15
double f(double x)
{
    return x*x*x - 9*x*x + 23*x - 15;
}

// declaramos su derivada
// f'(x) = 3x^2 - 18x + 23
double df(double x)
{
    return 3*x*x - 18*x + 23;
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Parametros del metodo
    double epsilon;
    //double epsilon = 1e-4;  // 1e-4 = 0.0001
    int iteraciones = 1e+4; // 1e+4 = 10000

    cout << "-------------------------------------------------------------" << endl;
    cout << "Tenemos el polinomio de una variable real              " << endl;
    cout << "f(x) = x³ - 9x² + 23x - 15                             " << endl;
    cout << "Cuya derivada corresponde a lo siguiente               " << endl;
    cout << "f'(x) = 3x² - 18x + 23                                 " << endl;
    cout << "-------------------------------------------------------------" << endl;

    cout << "Cálculo de las raices de f(x), ingrese la precisión: "; cin >> epsilon;
    cout << "Sus raices (reales) están dadas por:" << endl;

    // Calculamos sus raices, para ello el valor que toma "a" debe ser lo mas cercano posible a la raiz que se desea encontrar
    double raiz1 = newton(0.0, epsilon, iteraciones, f, df);
    cout << "x_1 = " << raiz1 << endl;

    double raiz2 = newton(2.5, epsilon, iteraciones, f, df);
    cout << "x_2 = " << raiz2 << endl;

    double raiz3 = newton(4.5, epsilon, iteraciones, f, df);
    cout << "x_3 = " << raiz3 << endl;

    cout << "-------------------------------------------------------------" << endl;
    
    // Ahora integramos con la regla del trapecio
    /*El numero de pasos representa en que cantidad se dividira el intervalo (a, b), cuanto mayor sea el numero de pasos, mayor sera la precision del resultado*/
    double a = 0;  // principio del intervalo
    double b = 10; // final del intervalo
    double pasos;

    cout << "Integral ∫f(x)dx por Trapecio, ingrese el numero de paso N: "; cin >> pasos;
    double int_trapecio = trapecio(f, a, b, pasos);
    cout << "Utilizando la regla del Trapecio: ∫f(x)dx = " << int_trapecio << endl;

    cout << "-------------------------------------------------------------" << endl;

    cout << "Integral ∫f(x)dx por Montecarlo, ingrese el numero de paso N: "; cin >> pasos;
    double int_montecarlo = montecarlo(f, a, b, pasos);
    cout << "Utilizando el metodo de Montecarlo: ∫f(x)dx = " << int_montecarlo << endl;
    cout << "-------------------------------------------------------------" << endl;

    return 0;
}