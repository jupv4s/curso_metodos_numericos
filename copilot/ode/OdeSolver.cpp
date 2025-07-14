#include "OdeSolver.h"
#include <cmath>
#include <algorithm>
#include <limits>

// ====================================================================================
// Implementación de los métodos de la clase OdeSolver
// ====================================================================================

// --- Constructores y Destructor ---

OdeSolver::OdeSolver() // ¡Nombre de la clase en la definición del constructor!
    : t_i(0.0), t_f(0.0), h(0.0), function_f(nullptr)
{
    // Constructor por defecto
}

OdeSolver::OdeSolver(const OdeSolver& other) // ¡Nombre de la clase en la definición del constructor de copia!
    : t_i(other.t_i), t_f(other.t_f), h(other.h), function_f(other.function_f)
{
    // Constructor de copia
}

OdeSolver::~OdeSolver() // ¡Nombre de la clase en la definición del destructor!
{
    // Destructor: No hay recursos dinámicos que liberar aquí, std::function se gestiona sola.
}

OdeSolver::OdeSolver(double T_i, double T_f, double H, std::function<Vector<double>(double, const Vector<double>&)> f) // ¡Nombre de la clase en la definición del constructor principal!
    : t_i(T_i), t_f(T_f), h(H), function_f(f)
{
    // Constructor principal
}


// --- Métodos auxiliares privados ---

Vector<double> OdeSolver::rk4_single_step(double current_t, const Vector<double>& Y_current, double step_h_val) { // ¡Nombre de la clase en la definición del método!
    Vector<double> K1, K2, K3, K4;

    K1 = step_h_val * this->function_f(current_t, Y_current);
    K2 = step_h_val * this->function_f(current_t + (step_h_val * 0.5), Y_current + K1 * 0.5);
    K3 = step_h_val * this->function_f(current_t + (step_h_val * 0.5), Y_current + K2 * 0.5);
    K4 = step_h_val * this->function_f(current_t + step_h_val, Y_current + K3);

    return Y_current + (K1 + 2.0*K2 + 2.0*K3 + K4) / 6.0;
}


// --- Implementación de los métodos de integración pública (retornan OdeSolution) ---

OdeSolution OdeSolver::Euler(Vector<double> Y_initial) // ¡Nombre de la clase en la definición del método!
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    OdeSolution result;

    // Imprimir las condiciones iniciales (ahora en el objeto result)
    result.times.push_back(current_t);
    result.states.push_back(Y);

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Vector<double> K1 = f(current_t, Y);

        Y = Y + actual_h * K1;

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result; // Retorna el objeto OdeSolution
}

OdeSolution OdeSolver::EulerCromer(Vector<double> Y_initial) // ¡Nombre de la clase en la definición del método!
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: Euler-Cromer requiere al menos 2 componentes (posicion, velocidad).");
    }

    OdeSolution result;
    result.times.push_back(current_t);
    result.states.push_back(Y);

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Vector<double> K_derivs = f(current_t, Y);

        Y[1] = Y[1] + actual_h * K_derivs[1];
        Y[0] = Y[0] + actual_h * Y[1];

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result;
}

OdeSolution OdeSolver::Verlet(Vector<double> Y_initial)
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: Verlet requiere al menos 2 componentes (posicion, velocidad).");
    }

    OdeSolution result;
    result.times.push_back(current_t);
    result.states.push_back(Y);

    Vector<double> initial_derivs = f(current_t, Y);
    Y[1] = Y[1] + 0.5 * step_h * initial_derivs[1];

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Y[0] = Y[0] + actual_h * Y[1];

        Vector<double> new_derivs = f(current_t + actual_h, Y);
        double new_acceleration = new_derivs[1];

        Y[1] = Y[1] + 0.5 * actual_h * new_acceleration;

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result;
}

OdeSolution OdeSolver::RK2(Vector<double> Y_initial)
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    Vector<double> K1, K2;

    OdeSolution result;
    result.times.push_back(current_t);
    result.states.push_back(Y);

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        K1 = actual_h * f(current_t, Y);
        K2 = actual_h * f(current_t + (actual_h * 0.5), Y + K1 * 0.5);

        Y = Y + K2;

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result;
}

OdeSolution OdeSolver::RK4(Vector<double> Y_initial)
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    Vector<double> Y = Y_initial;

    OdeSolution result;
    result.times.push_back(current_t);
    result.states.push_back(Y);

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Y = rk4_single_step(current_t, Y, actual_h);

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result;
}

OdeSolution OdeSolver::PEFRL(Vector<double> Y_initial)
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: PEFRL requiere al menos 2 componentes (posicion, velocidad).");
    }

    const double alpha = 0.1786178958448091;
    const double beta = -0.2123418310626054;
    const double gamma = 0.8564119069179095;
    const double delta = 1.0 - 2.0 * gamma;
    const double epsilon_pefrl = -0.06626458266981849;

    OdeSolution result;
    result.times.push_back(current_t);
    result.states.push_back(Y);

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Vector<double> Y_temp = Y;
        Vector<double> K_derivs;

        K_derivs = f(current_t, Y_temp);
        Y_temp[1] = Y_temp[1] + alpha * actual_h * K_derivs[1];

        Y_temp[0] = Y_temp[0] + beta * actual_h * Y_temp[1];

        K_derivs = f(current_t + beta * actual_h, Y_temp);
        Y_temp[1] = Y_temp[1] + gamma * actual_h * K_derivs[1];

        Y_temp[0] = Y_temp[0] + delta * actual_h * Y_temp[1];

        K_derivs = f(current_t + (beta + delta) * actual_h, Y_temp);
        Y_temp[1] = Y_temp[1] + epsilon_pefrl * actual_h * K_derivs[1];

        Y_temp[0] = Y_temp[0] + gamma * actual_h * Y_temp[1];

        K_derivs = f(current_t + (beta + delta + gamma) * actual_h, Y_temp);
        Y_temp[1] = Y_temp[1] + beta * actual_h * K_derivs[1];

        Y_temp[0] = Y_temp[0] + alpha * actual_h * Y_temp[1];

        Y = Y_temp;

        current_t += actual_h;
        result.times.push_back(current_t);
        result.states.push_back(Y);
    }
    return result;
}