#include "metodo_de_lineas.h"
#include "eqns.h"

int main()
{
    // 1. Definir parámetros para la simulación
    PhysicsParameters params;
    // Usa N=20 para pruebas rápidas, N=50 para la simulación final
    params.N = 20; 
    params.L = 1.0;
    params.h = params.L / (params.N - 1);
    params.rho0 = 1.0;
    params.omega = 1.0;
    params.sigma = 10.0;
    params.q = 1.0;
    params.m = 1.0;

    // 2. Crear el objeto que orquesta la simulación
    MetodoDeLineas sim(params);

    // 3. Ejecutar el proceso completo
    sim.run();

    return 0;
}