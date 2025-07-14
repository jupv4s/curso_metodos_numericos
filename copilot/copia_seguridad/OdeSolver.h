#ifndef ODESOLVER_H // Se mantiene el include guard original por compatibilidad
#define ODESOLVER_H

#include "Vector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <functional>
#include <stdexcept>

class OdeSolver
{
private:
    double t_i; // tiempo inicial
    double t_f; // tiempo final
    double h;   // paso del tiempo
    // Función de derivadas: toma tiempo (double) y estado (const Vector<double>&), retorna el vector de derivadas
    std::function<Vector<double>(double, const Vector<double>&)> function_f;

    // Método auxiliar para realizar un solo paso de RK4
    Vector<double> rk4_single_step(double current_t, const Vector<double>& Y_current, double step_h_val);

public:
    // Constructores y Destructor
    OdeSolver();
    OdeSolver(const OdeSolver& other);
    ~OdeSolver();
    OdeSolver(double T_i, double T_f, double H, std::function<Vector<double>(double, const Vector<double>&)> f);

    // Métodos de integración pública
    void Euler(Vector<double> Y_initial, std::string archivo);
    void EC(Vector<double> Y_initial, std::string archivo);
    void Verlet(Vector<double> Y_initial, std::string archivo);
    void RK2(Vector<double> Y_initial, std::string archivo);
    void RK4(Vector<double> Y_initial, std::string archivo);
    void PEFRL(Vector<double> Y_initial, std::string archivo);
};

#endif // ODESOLVER_H