#include "numerical_integration.h"
#include <iostream>
#include <fstream>  // Para manejar archivos (ofstream)
#include <cmath>    // Para exp, sqrt, abs
#include <iomanip>  // Para setw, setprecision, left, fixed

// Se usa para no tener que escribir std:: antes de cada comando
using namespace std;

int main()
{
    ofstream archivo_salida("resultados.csv");

    if (!archivo_salida.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo para escribir los resultados." << endl;
        return 1;
    }

    // El integrador se crea para el intervalo fijo [-1, 1]
    NumericalIntegrator integrator(-1.0, 1.0);

    archivo_salida << left 
                   << setw(20) << "Variable_x" 
                   << "Valor_Integral" << endl;
    archivo_salida << "----------------------------------------" << endl;

    // Bucle para iterar x de 0 a 5 y calcular la integral
    for (double x = 0.0; x <= 5.0; x += 0.1)
    {
        // La función a integrar, definida de forma compacta como una lambda
        auto integrand = [x](double t)
        {
            if (abs(t) >= 1.0) return 0.0;
            return exp(x * t / 2.0) * sqrt(1.0 - t * t);
        };

        // Calcular el resultado usando el método de Romberg
        double result = integrator.Romberg(integrand, 10);
        
        archivo_salida << left 
                       << setw(20) << fixed << setprecision(8) << x 
                       << result << endl;
    }
    
    archivo_salida.close();

    cout << "✅ Resultados exportados correctamente a 'resultados.csv'." << endl;

    return 0;
}