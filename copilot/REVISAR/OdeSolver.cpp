#include "OdeSolver.h"
#include <cmath>
#include <algorithm>
#include <limits>

// ====================================================================================
// Implementación de los métodos de la clase OdeSolver
// ====================================================================================

// --- Constructores y Destructor ---

OdeSolver::OdeSolver()
    : t_i(0.0), t_f(0.0), h(0.0), function_f(nullptr) {}

OdeSolver::OdeSolver(const OdeSolver& other)
    : t_i(other.t_i), t_f(other.t_f), h(other.h), function_f(other.function_f) {}

OdeSolver::~OdeSolver() {} // No hay recursos dinámicos que liberar aquí

OdeSolver::OdeSolver(double T_i, double T_f, double H, std::function<Vector<double>(double, const Vector<double>&)> f)
    : t_i(T_i), t_f(T_f), h(H), function_f(f) {}


// --- Métodos auxiliares privados ---

Vector<double> OdeSolver::rk4_single_step(double current_t, const Vector<double>& Y_current, double step_h_val) {
    Vector<double> K1, K2, K3, K4;

    K1 = step_h_val * this->function_f(current_t, Y_current);
    K2 = step_h_val * this->function_f(current_t + (step_h_val * 0.5), Y_current + K1 * 0.5);
    K3 = step_h_val * this->function_f(current_t + (step_h_val * 0.5), Y_current + K2 * 0.5);
    K4 = step_h_val * this->function_f(current_t + step_h_val, Y_current + K3);

    return Y_current + (1.0 / 6.0) * (K1 + 2.0 * K2 + 2.0 * K3 + K4);
}


// --- Implementación de los métodos de integración pública ---

void OdeSolver::Euler(Vector<double> Y_initial, std::string archivo)
{
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para Euler.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl; // Imprimir condiciones iniciales

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) { // Ajustar el último paso
            actual_h = final_t - current_t;
        }

        Vector<double> K1 = f(current_t, Y);

        Y = Y + actual_h * K1;

        current_t += actual_h;
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}

void OdeSolver::EC(Vector<double> Y_initial, std::string archivo)
{
    // Método de Euler-Cromer: Asume Y[0]=posición, Y[1]=velocidad,
    // y f devuelve (velocidad, aceleración) donde aceleración depende de la posición.
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: Euler-Cromer requiere al menos 2 componentes (posicion, velocidad).");
    }

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para Euler-Cromer.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl;

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Vector<double> K_derivs = f(current_t, Y); // K_derivs[0] = v, K_derivs[1] = a

        Y[1] = Y[1] + actual_h * K_derivs[1]; // Actualizar velocidad
        Y[0] = Y[0] + actual_h * Y[1];       // Actualizar posición con la NUEVA velocidad

        current_t += actual_h;
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}


void OdeSolver::Verlet(Vector<double> Y_initial, std::string archivo)
{
    // Algoritmo de Verlet Velocity. Para sistemas de 2do orden d^2x/dt^2 = a(x).
    // Asume Y[0]=posición, Y[1]=velocidad, y f devuelve (velocidad, aceleración)
    // donde aceleración solo depende de la posición (Y[0]).
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: Verlet requiere al menos 2 componentes (posicion, velocidad).");
    }

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para Verlet.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl;

    // Primer paso especial para inicializar la velocidad
    Vector<double> initial_derivs = f(current_t, Y);
    Y[1] = Y[1] + 0.5 * step_h * initial_derivs[1]; // Y[1] ahora es v(t + h/2)

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Y[0] = Y[0] + actual_h * Y[1]; // Actualizar posición

        Vector<double> new_derivs = f(current_t + actual_h, Y);
        double new_acceleration = new_derivs[1];

        Y[1] = Y[1] + 0.5 * actual_h * new_acceleration; // Actualizar velocidad

        current_t += actual_h;
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}


void OdeSolver::RK2(Vector<double> Y_initial, std::string archivo)
{
    // Runge-Kutta de 2do Orden (Método del Punto Medio)
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    Vector<double> K1, K2;

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para RK2.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl;

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
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}


void OdeSolver::RK4(Vector<double> Y_initial, std::string archivo)
{
    // Runge-Kutta de 4to Orden (Clásico). Utiliza el método auxiliar rk4_single_step.
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    Vector<double> Y = Y_initial;

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para RK4.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl;

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Y = rk4_single_step(current_t, Y, actual_h);

        current_t += actual_h;
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}

void OdeSolver::PEFRL(Vector<double> Y_initial, std::string archivo)
{
    // PEFRL (Position-Extended Forest-Ruth-Like) Symplectic Integrator (4th Order).
    // Para sistemas de 2do orden d^2x/dt^2 = a(x), donde aceleración 'a' SOLO depende de la posición (Y[0]).
    double current_t = this->t_i;
    double final_t = this->t_f;
    double step_h = this->h;
    std::function<Vector<double>(double, const Vector<double>&)> f = this->function_f;
    Vector<double> Y = Y_initial;

    if (Y.size() < 2) {
        throw std::invalid_argument("Error: PEFRL requiere al menos 2 componentes (posicion, velocidad).");
    }

    // Constantes de PEFRL
    const double alpha = 0.1786178958448091;
    const double beta = -0.2123418310626054;
    const double gamma = 0.8564119069179095;
    const double delta = 1.0 - 2.0 * gamma;
    const double epsilon_pefrl = -0.06626458266981849;

    std::ofstream data(archivo);
    if (!data.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo '" + archivo + "' para PEFRL.");
    }
    data << std::scientific << std::setprecision(10);

    data << current_t << "\t" << Y << std::endl;

    while (current_t < final_t)
    {
        double actual_h = step_h;
        if (current_t + step_h > final_t) {
            actual_h = final_t - current_t;
        }

        Vector<double> Y_temp = Y;
        Vector<double> K_derivs;

        // Pasos del algoritmo PEFRL
        Y_temp[1] = Y_temp[1] + alpha * actual_h * f(current_t, Y_temp)[1];
        Y_temp[0] = Y_temp[0] + beta * actual_h * Y_temp[1];
        Y_temp[1] = Y_temp[1] + gamma * actual_h * f(current_t + beta * actual_h, Y_temp)[1];
        Y_temp[0] = Y_temp[0] + delta * actual_h * Y_temp[1];
        Y_temp[1] = Y_temp[1] + epsilon_pefrl * actual_h * f(current_t + (beta + delta) * actual_h, Y_temp)[1];
        Y_temp[0] = Y_temp[0] + gamma * actual_h * Y_temp[1];
        Y_temp[1] = Y_temp[1] + beta * actual_h * f(current_t + (beta + delta + gamma) * actual_h, Y_temp)[1];
        Y_temp[0] = Y_temp[0] + alpha * actual_h * Y_temp[1];

        Y = Y_temp; // El estado final del paso

        current_t += actual_h;
        data << current_t << "\t" << Y << std::endl;
    }
    data.close();
}