#include <cmath>
#include <fstream>
#include <stdexcept>
#include "spline.h"

// constructor que carga los datos desde un archivo y construye el spline
Spline::Spline(const std::string& archivo, Tipo tipo)
{
    
    std::ifstream data(archivo);
    VecD nuevos_x, nuevos_y;
    double xval, yval;
    // lee pares (x, y) del archivo
    while (data >> xval >> yval)
    {
        nuevos_x.push_back(xval);
        nuevos_y.push_back(yval);
    }
    data.close();

    // verifica que se hayan leído datos válidos
    if (nuevos_x.empty() || nuevos_y.empty()) {
        throw std::runtime_error("El archivo está vacío o no contiene datos válidos.");
    }

    // inicializa los vectores y parámetros
    x = nuevos_x;
    y = nuevos_y;
    n = static_cast<int>(x.size()) - 1;
    tipoSpline = tipo;
    a = VecD(n);
    b = VecD(n);
    c = VecD(n+1);
    d = VecD(n);

    // construye el spline según el tipo
    if (tipoSpline == Natural)
        construirNatural();
    else
        construirPeriodico();
}

// constructor que recibe directamente los puntos y construye el spline
Spline::Spline(const VecD& puntosX, const VecD& puntosY, Tipo tipo)
{
    
    x = puntosX;
    y = puntosY;
    n = static_cast<int>(x.size()) - 1;
    tipoSpline = tipo;
    a = VecD(n);
    b = VecD(n);
    c = VecD(n+1);
    d = VecD(n);
    // construye el spline según el tipo
    if (tipoSpline == Natural)
        construirNatural();
    else
        construirPeriodico();
}

// construye el spline cúbico natural
void Spline::construirNatural()
{
    
    VecD h(n);
    // calcula las distancias entre puntos consecutivos
    for (int i = 0; i < n; ++i)
    {
        h[i] = x[i+1] - x[i];
    }
    VecD alpha(n);
    // calcula el vector alpha para el sistema
    for (int i = 1; i < n; ++i)
    {
        alpha[i] = (3.0/h[i])*(y[i+1]-y[i]) - (3.0/h[i-1])*(y[i]-y[i-1]);
    }
    VecD l(n+1), mu(n+1), z(n+1);
    l[0] = 1.0; mu[0] = 0.0; z[0] = 0.0;
    // resuelve el sistema tridiagonal
    for (int i = 1; i < n; ++i)
    {
        l[i] = 2.0*(x[i+1]-x[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }
    l[n] = 1.0; z[n] = 0.0; c[n] = 0.0;
    // calcula los coeficientes del spline
    for (int j = n-1; j >= 0; --j)
    {
        c[j] = z[j] - mu[j]*c[j+1];
        b[j] = (y[j+1]-y[j])/h[j] - h[j]*(c[j+1]+2.0*c[j])/3.0;
        d[j] = (c[j+1]-c[j])/(3.0*h[j]);
        a[j] = y[j];
    }
}

// construye el spline cúbico periódico
void Spline::construirPeriodico()
{
    
    VecD h(n);
    // calcula las distancias entre puntos consecutivos
    for (int i = 0; i < n; ++i)
    {
        h[i] = x[i+1] - x[i];
    }
    VecD alpha(n+1);
    // calcula el vector alpha considerando condiciones periódicas
    for (int i = 1; i < n; ++i)
    {
        alpha[i] = (3.0/h[i])*(y[i+1]-y[i]) - (3.0/h[i-1])*(y[i]-y[i-1]);
    }
    alpha[0] = (3.0/h[0])*(y[1]-y[0]) - (3.0/h[n-1])*(y[0]-y[n]);
    alpha[n] = (3.0/h[n-1])*(y[n]-y[n-1]) - (3.0/h[0])*(y[1]-y[0]);
    // matriz cíclica para condiciones periódicas
    VecD l(n+1, 0.0), mu(n+1, 0.0), z(n+1, 0.0);
    l[0] = 2.0*(x[1]-x[0]);
    mu[0] = 0.5;
    z[0] = alpha[0]/l[0];
    for (int i = 1; i < n; ++i)
    {
        l[i] = 2.0*(x[i+1]-x[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }
    l[n] = 2.0*(x[n]-x[n-1]);
    z[n] = alpha[n]/l[n];
    c[n] = z[n];
    // calcula los coeficientes del spline
    for (int j = n-1; j >= 0; --j)
    {
        c[j] = z[j] - mu[j]*c[j+1];
        b[j] = (y[j+1]-y[j])/h[j] - h[j]*(c[j+1]+2.0*c[j])/3.0;
        d[j] = (c[j+1]-c[j])/(3.0*h[j]);
        a[j] = y[j];
    }
}

// busca el tramo del spline donde se encuentra el valor x_eval
int Spline::buscarTramo(double x_eval) const
{
    
    if (x_eval <= x[0]) return 0;
    if (x_eval >= x[n]) return n-1;
    for (int i = 0; i < n; ++i)
    {
        if (x_eval >= x[i] && x_eval < x[i+1])
            return i;
    }
    return n-1;
}

// evalúa el spline en el punto x_eval
double Spline::evaluar(double x_eval) const
{
    
    int i = buscarTramo(x_eval);
    double dx = x_eval - x[i];
    return a[i] + b[i]*dx + c[i]*dx*dx + d[i]*dx*dx*dx;
}

// exporta los valores del spline a un archivo, generando cantidadPuntos puntos
void Spline::exportar(const std::string& archivo, int cantidadPuntos) const
{
    
    std::ofstream out(archivo);
    double xmin = x[0];
    double xmax = x[n];
    double paso = (xmax - xmin) / (cantidadPuntos-1);
    for (int i = 0; i < cantidadPuntos; ++i)
    {
        double xval = xmin + i*paso;
        out << xval << " " << evaluar(xval) << std::endl;
    }
    out.close();
}