#include "metodo_de_lineas.h"
#include "eqns.h"

int main()
{
    // 1. Definir parámetros
    ParametrosFisicos config; // <-- Cambio de nombre
    config.N = 50; 
    config.L = 1.0;
    config.h = config.L / (config.N - 1);
    config.rho0 = 1.0;
    config.omega = 1.0;
    config.sigma = 10.0;
    config.q = 1.0;
    config.m = 1.0;

    // 2. Crear el objeto que orquesta la simulación
    MetodoDeLineas sim(config); // <-- Se pasa el nuevo objeto

    // 3. Ejecutar el proceso completo
    sim.ejecutar();

    return 0;
}