#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "rk_4.h"

rk4::rk4(const ODEFunction& f) : f(f) {}

std::vector<double> rk4::rk4_step(double t, const std::vector<double>& y, double h, const ODEFunction& f)
{
    std::vector<double> k1 = f(t, y);
    std::vector<double> k2(y.size()), k3(y.size()), k4(y.size()), temp(y.size()), result(y.size());

    for (size_t i = 0; i < y.size(); ++i) temp[i] = y[i] + 0.5 * h * k1[i];
    k2 = f(t + 0.5 * h, temp);

    for (size_t i = 0; i < y.size(); ++i) temp[i] = y[i] + 0.5 * h * k2[i];
    k3 = f(t + 0.5 * h, temp);

    for (size_t i = 0; i < y.size(); ++i) temp[i] = y[i] + h * k3[i];
    k4 = f(t + h, temp);

    for (size_t i = 0; i < y.size(); ++i)
        result[i] = y[i] + (h / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    return result;
}

// Versión original (corregida) que escribe en archivo
std::vector<double> rk4::integrar_adaptativo(const std::vector<double>& y0, double t0, double tf, double h_inicial, double tol, const std::string& archivo_salida)
{
    std::ofstream data(archivo_salida);
    data << "# t";
    for (size_t i = 0; i < y0.size(); ++i) data << "\ty" << i;
    data << "\n";
    data << std::scientific << std::setprecision(10);

    double t = t0;
    double h = h_inicial;
    std::vector<double> y = y0;

    const double h_max = 1e-2;
    const double h_min = 1e-10;

    data << t;
    for (size_t i = 0; i < y.size(); ++i) data << "\t" << y[i];
    data << "\n";

    while (t < tf)
    {
        if (t + h > tf) h = tf - t;

        std::vector<double> y_full = rk4_step(t, y, h, f);
        double h_half = h / 2.0;
        std::vector<double> y_half_1 = rk4_step(t, y, h_half, f);
        std::vector<double> y_half_2 = rk4_step(t + h_half, y_half_1, h_half, f);
        
        double error_norm_sq = 0.0;
        for (size_t i = 0; i < y_full.size(); ++i) {
            double diff = y_half_2[i] - y_full[i];
            error_norm_sq += diff * diff;
        }
        double error = std::sqrt(error_norm_sq) / 15.0;

        if (error <= tol)
        {
            t += h;
            y = y_half_2;

            data << t;
            for (size_t i = 0; i < y.size(); ++i) data << "\t" << y[i];
            data << "\n";
        }

        double factor = 0.9 * std::pow(tol / (error + 1e-20), 0.2);
        h *= factor;
        if (h > h_max) h = h_max;
        if (h < h_min) h = h_min;
    }
    data.close();
    return y;
}

// --- NUEVA VERSIÓN AÑADIDA ---
// Versión que NO escribe en archivo, solo devuelve el resultado.
std::vector<double> rk4::integrar_adaptativo(const std::vector<double>& y0, double t0, double tf, double h_inicial, double tol)
{
    double t = t0;
    double h = h_inicial;
    std::vector<double> y = y0;

    const double h_max = 1e-2;
    const double h_min = 1e-10;

    while (t < tf)
    {
        if (t + h > tf) h = tf - t;

        std::vector<double> y_full = rk4_step(t, y, h, f);
        double h_half = h / 2.0;
        std::vector<double> y_half_1 = rk4_step(t, y, h_half, f);
        std::vector<double> y_half_2 = rk4_step(t + h_half, y_half_1, h_half, f);
        
        double error_norm_sq = 0.0;
        for (size_t i = 0; i < y_full.size(); ++i) {
            double diff = y_half_2[i] - y_full[i];
            error_norm_sq += diff * diff;
        }
        double error = std::sqrt(error_norm_sq) / 15.0;

        if (error <= tol) {
            t += h;
            y = y_half_2;
        }

        double factor = 0.9 * std::pow(tol / (error + 1e-20), 0.2);
        h *= factor;
        if (h > h_max) h = h_max;
        if (h < h_min) h = h_min;
    }
    return y;
}