#include <iostream>
#include <iomanip>
#include <random>
#include "eqns_cat.h"

const double PI = 3.14159265358979323846;

using namespace std;

int main()
{
    // Parámetros de la simulación
    long long num_muestras = 10000000; // 10 millones de muestras
    
    cout << "Calculando el area de la figura..." << endl;
    cout << "Numero de muestras: " << num_muestras << endl;

    // Inicializamos generador de números aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 2.0 * PI);

    // Realizamos el muestreo
    double suma_total = 0.0;
    for (long long i = 0; i < num_muestras; ++i)
    {
        double t_aleatorio = dis(gen);
        // Llamada directa a la función del integrando
        suma_total += integrando_area_gato(t_aleatorio);
    }

    // Calculamos el resultado final
    double valor_promedio = suma_total / num_muestras;
    double area_aproximada = (2.0 * PI) * valor_promedio;

    // Mostramos el resultado
    cout << fixed << setprecision(2);
    cout << "El area aproximada es: " << area_aproximada << endl;

    return 0;
}