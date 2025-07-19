#ifndef RK_4_H
#define RK_4_H

#include <iostream>
#include <string>
#include <functional>
#include <vector>

class rk4
{
public:
    using ODEFunction = std::function<std::vector<double>(double, const std::vector<double>&)>;

private:
    ODEFunction f;
    static std::vector<double> rk4_step(double t, const std::vector<double>& y, double h, const ODEFunction& f);

public:
    rk4(const ODEFunction& f);
    
    // Versión que guarda en archivo
    std::vector<double> integrar_adaptativo(const std::vector<double>& y0, double t0, double tf, double h_inicial, double tol, const std::string& archivo_salida);
    
    // --- NUEVA VERSIÓN AÑADIDA ---
    // Versión que NO escribe en archivo, solo devuelve el resultado.
    std::vector<double> integrar_adaptativo(const std::vector<double>& y0, double t0, double tf, double h_inicial, double tol);
};

#endif