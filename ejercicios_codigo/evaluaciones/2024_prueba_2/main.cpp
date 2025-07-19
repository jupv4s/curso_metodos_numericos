#include <iostream>
#include "Complex.h"
#include "sistema.h"

int main()
{
    // Crear una instancia del sistema.
    Sistema sistema;

    // Definir las propiedades de las partículas según el problema.
    // Para masa y velocidad, usaremos 0.0 ya que no son necesarias para este cálculo.
    const double masa_default = 0.0;
    const Complex<double> vel_default(0.0, 0.0);

    // Añadir las partículas al sistema.
    sistema.agregarParticula(masa_default, 1.0, Complex<double>(1.0, 1.0), vel_default);   // q1 en z1
    sistema.agregarParticula(masa_default, -2.0, Complex<double>(-1.0, 1.0), vel_default);  // q2 en z2
    sistema.agregarParticula(masa_default, 3.0, Complex<double>(-1.0, -1.0), vel_default); // q3 en z3
    sistema.agregarParticula(masa_default, -4.0, Complex<double>(1.0, -1.0), vel_default);  // q4 en z4

    // Calcular las fuerzas y guardar el resultado en un archivo.
    sistema.calcularYGuardarFuerzas("fuerzas_output.txt");

    return 0;
}