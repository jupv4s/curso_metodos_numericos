#include <iostream>
#include <vector>
#include <string>

#include "rk_4.h"
#include "spline.h"

// Parámetros del sistema acoplado Lorenz-Rössler
const double SIGMA = 10.0;
const double RHO = 28.0;
const double BETA = 8.0 / 3.0;
const double A = 0.2;
const double B = 0.2;
const double C = 5.7;

// Coeficientes de acoplamiento (asumidos)
const double EPSILON_1 = 0.1;
const double EPSILON_2 = 0.1;


// Definición de la función del sistema de EDOs
// y[0]=x1, y[1]=x2, y[2]=x3, y[3]=x4, y[4]=x5, y[5]=x6
std::vector<double> sistema_acoplado(double t, const std::vector<double>& y) {
    std::vector<double> dy(6);

    // Ecuaciones de Lorenz acopladas
    dy[0] = SIGMA * (y[1] - y[0]);
    dy[1] = y[0] * (RHO - y[2]) - y[1];
    dy[2] = y[0] * y[1] - BETA * y[2] + EPSILON_1 * y[5];

    // Ecuaciones de Rössler acopladas
    dy[3] = -y[4] - y[5];
    dy[4] = y[3] + A * y[4];
    dy[5] = B + y[5] * (y[3] - C) + EPSILON_2 * y[2];

    return dy;
}


int main() {
    // --- PARTE 1: RESOLUCIÓN DEL SISTEMA ---
    std::cout << "Resolviendo el sistema de EDOs..." << std::endl;

    // Condiciones iniciales y parámetros de integración
    std::vector<double> y0 = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    double t0 = 0.0;
    double tf = 1000.0;
    double h_inicial = 1e-3;
    double tol = 1e-9;
    std::string archivo_salida = "sistema_acoplado.dat";

    // Crear el integrador y resolver el sistema
    rk4 integrador(sistema_acoplado);
    integrador.integrar_adaptativo(y0, t0, tf, h_inicial, tol, archivo_salida);

    std::cout << "Sistema resuelto. Datos guardados en " << archivo_salida << std::endl;


    // --- PARTE 2: ANÁLISIS CON SPLINES ---
    std::cout << "Iniciando análisis con Splines..." << std::endl;

    // Cargar los datos de la simulación
    std::ifstream data_in(archivo_salida);
    VecD t_all, x6_all;
    double t, x1, x2, x3, x4, x5, x6;
    std::string header;
    std::getline(data_in, header); // Omitir cabecera

    while (data_in >> t >> x1 >> x2 >> x3 >> x4 >> x5 >> x6) {
        t_all.push_back(t);
        x6_all.push_back(x6);
    }
    data_in.close();

    // Filtrar el segmento de interés (t en [989, 998])
    VecD t_segment, x6_segment;
    for (size_t i = 0; i < t_all.size(); ++i) {
        if (t_all[i] >= 989.0 && t_all[i] <= 998.0) {
            t_segment.push_back(t_all[i]);
            x6_segment.push_back(x6_all[i]);
        }
    }

    // Dividir en puntos pares e impares y guardarlos en archivos
    std::ofstream pares_out("puntos_pares.txt");
    std::ofstream impares_out("puntos_impares.txt");
    VecD t_pares, x6_pares; // Para construir el spline

    for (size_t i = 0; i < t_segment.size(); ++i) {
        if (i % 2 == 0) { // Índice par
            pares_out << t_segment[i] << " " << x6_segment[i] << "\n";
            t_pares.push_back(t_segment[i]);
            x6_pares.push_back(x6_segment[i]);
        } else { // Índice impar
            impares_out << t_segment[i] << " " << x6_segment[i] << "\n";
        }
    }
    pares_out.close();
    impares_out.close();

    // Construir el spline con los puntos pares
    Spline spline_x6(t_pares, x6_pares, Spline::Natural);

    // Exportar la curva del spline a un archivo para graficar
    spline_x6.exportar("spline_recuperado.txt", 500);

    std::cout << "Análisis completado. Se generaron los archivos para Gnuplot." << std::endl;

    return 0;
}