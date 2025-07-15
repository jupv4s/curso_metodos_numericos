#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "Dual2D.h"

// Alias para mayor claridad
using Dual = Dual2D<double>;

// Estructura simple para almacenar puntos
struct Point
{
    double x, y;
};

// La función g(x,y) del problema, implementada con la plantilla.
// g(x,y) = cos^2(x) * sin(y) + sin(x*y)
template <class T>
T g(const T& x, const T& y)
{
    return pow(cos(x), 2.0) * sin(y) + sin(x * y);
}

constexpr double TOL = 1e-9;

// Clasifica el punto según la Hessiana
std::string classify(double hxx, double hyy, double hxy)
{
    double det_H = hxx * hyy - hxy * hxy;
    if (std::abs(det_H) < TOL) return "Indeterminado (criterio no concluyente)";
    if (det_H > 0) return hxx > 0 ? "Mínimo local" : "Máximo local";
    return "Punto silla";
}

// Imprime los resultados de forma compacta
void print_results(const Point& p, double gx, double gy, double hxx, double hyy, double hxy, const std::string& clasif) {
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Punto (" << p.x << ", " << p.y << ")\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Gradiente:      [" << gx << ", " << gy << "]\n";
    std::cout << "Hessiana:       | " << std::setw(10) << hxx << "  " << std::setw(10) << hxy << " |\n"
              << "                | " << std::setw(10) << hxy << "  " << std::setw(10) << hyy << " |\n";
    std::cout << "Determinante:   " << hxx * hyy - hxy * hxy << "\n";
    std::cout << "Clasificación:  " << clasif << "\n";
}

// Analiza un punto
void analyze_point(const Point& p)
{
    Dual x_dual(p.x, 1.0, 0.0), y_dual(p.y, 0.0, 1.0);
    Dual result = g(x_dual, y_dual);
    double gx = result.df_dx(), gy = result.df_dy();
    double hxx = result.d2f_dx2(), hyy = result.d2f_dy2(), hxy = result.d2f_dxdy();
    std::string clasif = classify(hxx, hyy, hxy);
    print_results(p, gx, gy, hxx, hyy, hxy, clasif);
}

int main()
{
    const double PI = acos(-1.0);

    // Conjunto de 3 puntos para análisis general
    std::cout << "======= Análisis General de Puntos =======" << std::endl;
    std::vector<Point> points1 = {
        {0.0, PI / 2.0},
        {1.0, 1.0},
        {PI / 4.0, PI / 3.0}
    };
    for (const auto& p : points1)
    {
        analyze_point(p);
    }
    
    std::cout << "\n======= Búsqueda de Puntos con Clasificaciones Específicas =======" << std::endl;
    // Conjunto de 3 puntos adicionales buscando distintas clasificaciones
    std::vector<Point> points2 = {
        {PI / 2.0, 3.0},   // Potencial punto silla
        {PI / 2.0, -1.0},  // Potencial mínimo/máximo
        {1.5, -1.5}        // Otro punto para explorar
    };
    for (const auto& p : points2)
    {
        analyze_point(p);
    }
    
    std::cout << "---------------------------------------------------------" << std::endl;

    return 0;
}