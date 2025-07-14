#include "solver_montecarlo.h" // Incluimos la nueva clase

int main()
{
    // Resolver el problema 3 del PDF: Estimar el área de la figura polar con a=1, b=9
    // Se eligen 10,000,000 muestras para una buena precisión.
    MonteCarloEstimator problem3_solver(10000000, 1, 9, "Area de la figura polar (Problema 3a)");
    problem3_solver.show();

    return 0;
}
