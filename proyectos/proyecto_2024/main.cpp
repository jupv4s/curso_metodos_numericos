#include "dependencias/ajuste.h"

// Set de datos, donde:
// Col1: L
// Col2: T^2
vector<vector<double>> datos =
{   //   L       T^2
    {0.800,    2.96},
    {0.760,    2.66},
    {0.720,    2.53},
    {0.680,    2.40},
    {0.640,    2.34},
    {0.600,    2.10},
    {0.560,    1.90},
    {0.520,    1.77},
    {0.480,    1.56},
    {0.440,    1.35},
    {0.420,    1.28}
};

int main()
{
    cout << endl;
    cout << "Aplicando un ajuste lineal a los datos se obtiene:" << endl << endl;

    // Ajuste lineal para las columnas 'x' e 'y'
    Ajuste<double> A_1(datos);
    vector<double> cts_ajuste = A_1.lineal();

    /*Buscamos obtener la ecuacion de la recta que mejor se ajusta a los datos, por lo que queremos formar algo del estilo:
        
    f(x) = mx + b ---> T^2 = m*L + b
        
    donde f(x) = y = T^2, x = L, m = cts_ajuste[1] y b = cts_ajuste[0]*/

    cout << "---------------------------------" << endl;
    cout << "Pendiente: "  << cts_ajuste[1] << endl;
    cout << "Intercepto: " << cts_ajuste[0] << endl;
    cout << "---------------------------------" << endl;

    cout << "Así obtenemos la ecuación de la recta que mejor ajusta a los datos medidos:" << endl;

    cout << "T^2 = " << cts_ajuste[1] << "*L + " << cts_ajuste[0] << endl;

    return 0;
}