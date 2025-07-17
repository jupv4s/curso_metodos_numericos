#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <iomanip>
#include "rk_4.h"

// --- 1. Parámetros y Constantes Globales ---
// Definimos los parámetros de la simulación fuera de main para mayor claridad.
const std::vector<double> Y0 = {0.5, 0.0, 0.5, 0.1}; // {x(0), vx(0), y(0), vy(0)}
const double T0 = 0.0;
const double TF = 500.0;
const double H = 0.01;
const double GAMMA = 0.5;

// --- 2. Clase para el Sistema de Ecuaciones (Functor) ---
// Esta clase encapsula toda la lógica de la EDO y el ruido.
// Se comporta como una función, por lo que podemos pasarla a nuestro solver rk4.
class LangevinSystem {
public:
    enum NoiseType { UNIFORM, GAUSSIAN };

private:
    NoiseType noise_type_;
    std::mt19937 gen_; // Cada sistema tiene su propio generador de números aleatorios.
    std::uniform_real_distribution<> uniform_dist_;
    std::normal_distribution<> gaussian_dist_;

public:
    // El constructor inicializa el tipo de ruido y los generadores.
    LangevinSystem(NoiseType type)
        : noise_type_(type),
          gen_(std::random_device{}()), // Inicializa con una semilla aleatoria
          uniform_dist_(-0.1, 0.1),
          gaussian_dist_(0.0, 0.05) {}

    // El operator() permite que los objetos de esta clase sean llamados como si fueran funciones.
    std::vector<double> operator()(double t, const std::vector<double>& y) {
        double eta_x, eta_y;

        if (noise_type_ == UNIFORM) {
            eta_x = uniform_dist_(gen_);
            eta_y = uniform_dist_(gen_);
        } else {
            eta_x = gaussian_dist_(gen_);
            eta_y = gaussian_dist_(gen_);
        }

        // y[0]=x, y[1]=vx, y[2]=y, y[3]=vy
        double vx = y[1];
        double vy = y[3];
        double dvxdt = -GAMMA * vx + eta_x;
        double dvydt = -GAMMA * vy + eta_y;

        return {vx, dvxdt, vy, dvydt};
    }
};

// --- 3. Función Auxiliar para Ejecutar una Simulación ---
// Encapsula la lógica repetitiva de correr el solver y guardar los datos.
void run_simulation(const std::string& name, LangevinSystem& system_function) {
    std::cout << "Iniciando simulación con ruido " << name << "..." << std::endl;
    std::string filename = "langevin_" + name + ".txt";

    rk4 solver(system_function);
    solver.integrar_paso_fijo(Y0, T0, TF, H, filename);

    std::cout << "-> Datos guardados en '" << filename << "'" << std::endl;
}

// --- 4. Función `main` Limpia y Concisa ---
// Ahora main solo orquesta las operaciones de alto nivel.
int main() {
    // Creamos una instancia del sistema para cada tipo de ruido.
    LangevinSystem uniform_system(LangevinSystem::UNIFORM);
    LangevinSystem gaussian_system(LangevinSystem::GAUSSIAN);

    // Ejecutamos las dos simulaciones.
    run_simulation("uniforme", uniform_system);
    run_simulation("gaussiano", gaussian_system);

    std::cout << "\nSimulaciones completadas." << std::endl;
    return 0;
}