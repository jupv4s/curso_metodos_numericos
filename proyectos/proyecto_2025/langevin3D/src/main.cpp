#include <iostream>
#include <vector>
#include <string>
#include "rk_4.h"
#include "langevin3D.h"

using namespace std;

const vector<double> Y0 = {0.5, 0.0, 0.5, 0.1, 0.5, 0.2}; // {x(0), vx(0), y(0), vy(0), z(0), vz(0)}
const double T0 = 0.0;     // tiempo inicial
const double TF = 500.0;   // tiempo final
const double H = 0.01;     // paso de tiempo
const double GAMMA = 0.5;  // coeficiente de friccion

void run_simulation(const string& name, LangevinSystem3D& system_function)
{
    cout << "Iniciando simulación 3D con ruido " << name << "..." << endl;
    string filename = "data/langevin3d_" + name + ".dat";

    rk4 solver(system_function);
    solver.integrar_paso_fijo(Y0, T0, TF, H, filename);

    cout << "-> Datos guardados en '" << filename << "'" << endl;
}

int main()
{
    // creamos una instancia del sistema para cada tipo de ruido
    LangevinSystem3D uniform_system(LangevinSystem3D::UNIFORM, GAMMA);
    LangevinSystem3D gaussian_system(LangevinSystem3D::GAUSSIAN, GAMMA);

    // ejecutamos las dos simulaciones
    run_simulation("uniforme", uniform_system);
    run_simulation("gaussiano", gaussian_system);

    cout << "\nSimulaciones 3D completadas." << endl;
    return 0;
}