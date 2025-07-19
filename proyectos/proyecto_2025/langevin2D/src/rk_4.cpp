#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "rk_4.h"

rk4::rk4(const ODEFunction& f) : f(f) {}

void rk4::integrar_paso_fijo(const std::vector<double>& y0, double t0, double tf, double h, const std::string& archivo_salida)
{
    std::ofstream data(archivo_salida);
    data << "# t";
    for (size_t i = 0; i < y0.size(); ++i) data << "\ty" << i;
    data << "\n";
    data << std::scientific << std::setprecision(10);

    std::vector<double> y = y0;
    double t = t0;
    while (t <= tf)
    {
        data << t;
        for (size_t i = 0; i < y.size(); ++i) data << "\t" << y[i];
        data << "\n";
        y = rk4_step(t, y, h, f);
        t += h;
        if (t > tf && tf - (t-h) > 1e-12)
        {
            t = tf;
            y = rk4_step(t-h, y, tf-(t-h), f);
            data << t;
            for (size_t i = 0; i < y.size(); ++i) data << "\t" << y[i];
            data << "\n";
        }
    }
    data.close();
}

void rk4::integrar_adaptativo(const std::vector<double>& y0, double t0, double tf, double h_inicial, double tol, const std::string& archivo_salida)
{
    double t = t0;
    double h = h_inicial;
    const double h_max = 1e-2;
    const double h_min = 1e-10;

    std::ofstream data(archivo_salida);
    data << "# t";
    for (size_t i = 0; i < y0.size(); ++i) data << "\ty" << i;
    data << "\n";
    data << std::scientific << std::setprecision(10);

    std::vector<double> y = y0;
    while (t < tf)
    {
        if (t + h > tf) h = tf - t;

        std::vector<double> y_full = rk4_step(t, y, h, f);
        double h_half = h / 2.0;
        std::vector<double> y_half = rk4_step(t, y, h_half, f);
        std::vector<double> y_half_2 = rk4_step(t + h_half, y_half, h_half, f);
        // Calcular el error directamente
        double error = 0.0;
        for (size_t i = 0; i < y_full.size(); ++i)
            error += (y_half_2[i] - y_full[i]) * (y_half_2[i] - y_full[i]);
        error = std::sqrt(error);

        if (error <= tol)
        {
            data << t;
            for (size_t i = 0; i < y.size(); ++i) data << "\t" << y[i];
            data << "\n";
            y = y_half_2;
            t += h;
        }

        double factor = std::pow(tol / (error + 1e-20), 0.2);
        h *= factor;
        if (h > h_max) h = h_max;
        if (h < h_min) h = h_min;
    }
    data.close();
}

std::vector<double> rk4::rk4_step(double t, const std::vector<double>& y, double h, const ODEFunction& f)
{
    std::vector<double> k1 = f(t, y);
    std::vector<double> k2(y.size()), k3(y.size()), k4(y.size()), temp(y.size()), result(y.size());

    // k2 = f(t + 0.5 * h, y + 0.5 * h * k1)
    for (size_t i = 0; i < y.size(); ++i)
        temp[i] = y[i] + 0.5 * h * k1[i];
    k2 = f(t + 0.5 * h, temp);

    // k3 = f(t + 0.5 * h, y + 0.5 * h * k2)
    for (size_t i = 0; i < y.size(); ++i)
        temp[i] = y[i] + 0.5 * h * k2[i];
    k3 = f(t + 0.5 * h, temp);

    // k4 = f(t + h, y + h * k3)
    for (size_t i = 0; i < y.size(); ++i)
        temp[i] = y[i] + h * k3[i];
    k4 = f(t + h, temp);

    // result = y + (h/6) * (k1 + 2*k2 + 2*k3 + k4)
    for (size_t i = 0; i < y.size(); ++i)
        result[i] = y[i] + (h / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    return result;
}