#include <iostream>
#include "solver_linear_regression.h"

using namespace std;

int main()
{    
    // Crea una instancia de linear_regression que carga los datos y calcula los coeficientes.
    // La regresión se realiza inmediatamente en el constructor.

    linear_regression solver("data_simple.txt", "linear");
    solver.show();

    linear_regression solver2("data_multi.txt", "linear");
    solver2.show();

    return 0;
}
