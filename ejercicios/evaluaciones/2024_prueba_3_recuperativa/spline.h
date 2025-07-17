#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>

using VecD = std::vector<double>;

class Spline {
public:
    enum Tipo { Natural, Periodico };

    Spline(const std::string& archivo, Tipo tipo = Natural);
    Spline(const VecD& puntosX, const VecD& puntosY, Tipo tipo = Natural);

    double evaluar(double x_eval) const;
    void exportar(const std::string& archivo, int cantidadPuntos) const;
    double getXmin() const;
    double getXmax() const;

private:
    VecD x, y, a, b, c, d;
    int n;
    Tipo tipoSpline;

    void construirNatural();
    void construirPeriodico();
    int buscarTramo(double x_eval) const;
};

// Exporta dos splines (trayectoria 2D) a un archivo, generando cantidadPuntos puntos
void exportarTrayectoria2D(const Spline& sx, const Spline& sy, const std::string& archivo, int cantidadPuntos);

#endif