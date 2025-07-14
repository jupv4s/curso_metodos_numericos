#include "CommonHeaders.h"

// Definición de la función de derivadas para el oscilador armónico
Vector<double> oscilador_armonico(double t, const Vector<double>& Y)
{
    Vector<double> dYdt(2);
    dYdt[0] = Y[1];
    dYdt[1] = -1.0 * Y[0];
    return dYdt;
}

// Función auxiliar para exportar un vector y mostrar mensaje de error si falla
// T es el tipo de dato del vector

template<typename T>
void exportar_vector(const Vector<T>& v, const std::string& nombre_archivo)
{
    if (!DataManager::exportCustomVector(v, nombre_archivo))
    {
        std::cerr << "Falló la exportación de " << nombre_archivo << "." << std::endl;
    }
}

// Función auxiliar para exportar una matriz y mostrar mensaje de error si falla
void exportar_matriz(const Matrix<double>& m, const std::string& nombre_archivo, int precision = 1)
{
    if (!DataManager::exportMatrix(m, nombre_archivo, precision))
    {
        std::cerr << "Falló la exportación de " << nombre_archivo << "." << std::endl;
    }
}

// Función auxiliar para exportar una solución de ODE y mostrar mensaje de error si falla
void exportar_ode(const OdeSolution& sol, const std::string& nombre_archivo)
{
    if (!DataManager::exportOdeSolution(sol, nombre_archivo))
    {
        std::cerr << "Falló la exportación de " << nombre_archivo << "." << std::endl;
    }
}

int main()
{
    // --- Exportación de Vectores y Matrices ---
    std::cout << "--- Demostración de exportación con DataManager ---" << std::endl;

    // 1. Creamos un Vector<int>
    Vector<int> v = {10, 20, 30, 40, 50};
    exportar_vector(v, "mi_vector_int.txt");

    // 2. Creamos un Vector<double>
    Vector<double> w({1.1, 2.2, 3.3, 4.4});
    exportar_vector(w, "mi_vector_double.txt");

    // 3. Creamos una Matrix<double> de dimension 3x3
    Matrix<double> A(3, 3);
    A(0,0) = 1.0; A(0,1) = 2.0; A(0,2) = 3.0;
    A(1,0) = 4.0; A(1,1) = 5.0; A(1,2) = 6.0;
    A(2,0) = 7.0; A(2,1) = 8.0; A(2,2) = 9.0;
    exportar_matriz(A, "mi_matriz_double.txt", 1);

    // --- Uso con ODESolver ---
    std::cout << "\n--- Usando con ODESolver ---" << std::endl;
    Vector<double> initial_conditions = {1.0, 0.0};
    double initial_time = 0.0;
    double final_time = 2.0 * M_PI * 2;
    double time_step = 0.01;

    OdeSolver solver(initial_time, final_time, time_step, oscilador_armonico);
    OdeSolution rk4_solution = solver.RK4(initial_conditions);
    exportar_ode(rk4_solution, "rk4_oscilador.txt");

    return 0;
}