#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>


using VecD = std::vector<double>;

class Spline
{

public:
    enum Tipo { Natural, Periodico }; // tipo de spline

    Spline(const std::string& archivo, Tipo tipo = Natural); // desde archivo
    Spline(const VecD& puntosX, const VecD& puntosY, Tipo tipo = Natural); // desde vectores


    double evaluar(double x_eval) const; // evalúa spline
    void exportar(const std::string& archivo, int cantidadPuntos) const; // exporta a archivo


private:
    VecD x, y, a, b, c, d; // datos y coeficientes
    int n; // cantidad de tramos
    Tipo tipoSpline; // tipo actual


    void construirNatural(); // spline natural
    void construirPeriodico(); // spline periódico
    int buscarTramo(double x_eval) const; // busca tramo

};

#endif