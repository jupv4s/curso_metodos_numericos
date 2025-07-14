#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include "Vector.h"
#include "OdeSolution.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <functional>
#include <stdexcept>

class OdeSolver // ¡Nombre de la clase cambiado!
{
private:
    double t_i; // tiempo inicial
    double t_f; // tiempo final
    double h;   // paso del tiempo
    std::function<Vector<double>(double, const Vector<double>&)> function_f; // Función de derivadas
    Vector<double> rk4_single_step(double current_t, const Vector<double>& Y_current, double step_h_val);

public:
    // --- Constructores y Destructor ---
    OdeSolver(); // Constructor por defecto
    OdeSolver(const OdeSolver& other); // Constructor de copia
    ~OdeSolver(); // Destructor
    OdeSolver(double T_i, double T_f, double H, std::function<Vector<double>(double, const Vector<double>&)> f); // Constructor principal

    // --- Métodos de integración pública (retornan OdeSolution) ---
    OdeSolution Euler(Vector<double> Y_initial);
    OdeSolution EulerCromer(Vector<double> Y_initial);
    OdeSolution Verlet(Vector<double> Y_initial);
    OdeSolution RK2(Vector<double> Y_initial);
    OdeSolution RK4(Vector<double> Y_initial);
    OdeSolution PEFRL(Vector<double> Y_initial);
};

#endif