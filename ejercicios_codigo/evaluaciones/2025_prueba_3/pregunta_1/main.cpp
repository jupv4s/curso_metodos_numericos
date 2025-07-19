#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include "rk_4.h"
#include "spline.h"

using namespace std;

constexpr double sigma = 10.0, rho = 28.0, veta = 8.0/3.0;

// calcula la derivada del sistema de lorenz para un estado x y tiempo t
vector<double> lorenz(double t, vector<double> x)
{
    vector<double> dx(3);
    dx[0] = sigma * (x[1] - x[0]);      // dx/dt = sigma * (y - x)
    dx[1] = x[0] * (rho - x[2]) - x[1]; // dy/dt = x * (rho - z) - y
    dx[2] = x[0] * x[1] - veta * x[2];  // dz/dt = x * y - beta * z
    return dx;
}

int main()
{
    // integra el sistema de lorenz y guarda los resultados en lorenz_sol.dat
    rk4 integrador(lorenz);
    vector<double> estado_inicial = {5.05105452195349, 8.53834330708889, 12.14620018788295};
    double tiempo_inicial = 0.0;
    double tiempo_final = 1.08;
    double paso_inicial = 1e-3;
    double tolerancia = 1e-8;
    string archivo_salida = "lorenz.dat";
    integrador.integrar_adaptativo(estado_inicial, tiempo_inicial, tiempo_final, paso_inicial, tolerancia, archivo_salida);

    // lee los datos generados por la integración (t, x, y, z)
    vector<double> t, x, y, z;
    double T, X, Y, Z;
    
    ifstream archivo("lorenz.dat"); string header; getline(archivo, header);
    
    while (archivo >> T >> X >> Y >> Z) { t.push_back(T); x.push_back(X); y.push_back(Y); z.push_back(Z); }
    archivo.close();
    
    if (t.empty()) { cerr << "error: no se leyeron datos" << endl; return 1; }
    cout << "datos leídos: " << t.size() << endl;

    // crea splines cúbicos para cada variable y evalúa en t = sqrt(2)
    Spline sx(t, x, Spline::Natural), sy(t, y, Spline::Natural), sz(t, z, Spline::Natural);
    double teval = sqrt(2.0);
    cout << "en t = sqrt(2): x = " << sx.evaluar(teval)
         << ", y = " << sy.evaluar(teval)
         << ", z = " << sz.evaluar(teval) << endl;
    return 0;
}