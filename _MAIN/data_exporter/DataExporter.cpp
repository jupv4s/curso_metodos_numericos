#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip> // Para std::setprecision y std::fixed
#include <cmath>

// Incluimos nuestras clases/plantillas de ejemplo (asumimos que ya existen)
// #include "Vector2D.h"
// #include "Matrix.h"
// #include "Polynomial.h"

// ******************************************************************
// Para este ejemplo, voy a definir versiones simplificadas
// de tus plantillas para que el código sea autocontenido.
// En un proyecto real, incluirías tus archivos de cabecera.
// ******************************************************************

// Simplificación de Vector2D<T> para el ejemplo
template <typename T>
class Vector2D {
public:
    T x, y;
    Vector2D(T _x = T(), T _y = T()) : x(_x), y(_y) {}
};

// Simplificación de Matrix<T> para el ejemplo (matriz 2x2)
template <typename T>
class Matrix {
public:
    T data[2][2]; // Matriz 2x2 fija para simplificar
    Matrix(T v00=T(), T v01=T(), T v10=T(), T v11=T()) {
        data[0][0] = v00; data[0][1] = v01;
        data[1][0] = v10; data[1][1] = v11;
    }
};

// Simplificación de Polynomial<T> para el ejemplo
template <typename T>
class Polynomial {
public:
    std::vector<T> coefficients; // Coeficientes: [a0, a1, a2, ...] para a0 + a1*x + a2*x^2 + ...
    Polynomial(const std::vector<T>& coeffs) : coefficients(coeffs) {}

    // Evalúa el polinomio en un punto x
    T evaluate(T x_val) const {
        T result = T();
        T x_power = T(1);
        for (const T& coeff : coefficients) {
            result += coeff * x_power;
            x_power *= x_val;
        }
        return result;
    }
};

// ******************************************************************
// Fin de las definiciones simplificadas
// ******************************************************************


// Plantilla de clase DataExporter
template <typename T>
class DataExporter {
public:
    // Método para exportar una serie de puntos (x, y)
    // Es útil para graficar funciones o curvas.
    void exportPoints(std::ostream& os, const std::vector<std::pair<T, T>>& points,
                      const std::string& header = "") const {
        if (!header.empty()) {
            os << "# " << header << std::endl;
        }
        os << std::fixed << std::setprecision(10); // Precisión para los números
        for (const auto& p : points) {
            os << p.first << "\t" << p.second << std::endl;
        }
        os << std::endl; // Línea en blanco para separar bloques de datos en Gnuplot
    }

    // Método para exportar una lista de vectores 2D
    // Cada vector en una fila, con sus componentes en columnas
    void exportVector2DList(std::ostream& os, const std::vector<Vector2D<T>>& vectors,
                            const std::string& header = "") const {
        if (!header.empty()) {
            os << "# " << header << std::endl;
        }
        os << std::fixed << std::setprecision(10);
        for (const auto& v : vectors) {
            os << v.x << "\t" << v.y << std::endl;
        }
        os << std::endl;
    }

    // Método para exportar una matriz (ej. 2x2 para simplificar)
    // Cada fila de la matriz se exporta en una línea.
    void exportMatrix(std::ostream& os, const Matrix<T>& mat,
                      const std::string& header = "") const {
        if (!header.empty()) {
            os << "# " << header << std::endl;
        }
        os << std::fixed << std::setprecision(10);
        // Suponiendo una matriz 2x2 de nuestro ejemplo simplificado
        for (int i = 0; i < 2; ++i) {
            os << mat.data[i][0] << "\t" << mat.data[i][1] << std::endl;
        }
        os << std::endl;
    }

    // Método para exportar valores de un polinomio evaluado en un rango
    void exportPolynomial(std::ostream& os, const Polynomial<T>& poly, T start_x, T end_x, int num_points,
                          const std::string& header = "") const {
        if (num_points <= 0) return;
        if (!header.empty()) {
            os << "# " << header << std::endl;
        }
        os << std::fixed << std::setprecision(10);

        T step = (end_x - start_x) / (num_points - 1);
        for (int i = 0; i < num_points; ++i) {
            T x_val = start_x + i * step;
            T y_val = poly.evaluate(x_val);
            os << x_val << "\t" << y_val << std::endl;
        }
        os << std::endl;
    }

    // Puedes agregar más métodos para otros tipos:
    // exportComplexList, exportDualList, etc.
};

int main() {
    // 1. Crear un objeto exportador para tipo double
    DataExporter<double> exporter_double;

    // 2. Abrir un archivo para escritura
    std::ofstream outputFile("simulation_data.dat");
    if (!outputFile.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return 1;
    }

    // ----------------------------------------------------
    // Ejemplos de uso con las clases simplificadas
    // ----------------------------------------------------

    // --- Exportar Vectores 2D ---
    std::vector<Vector2D<double>> vec_data;
    vec_data.push_back(Vector2D<double>(1.0, 2.5));
    vec_data.push_back(Vector2D<double>(2.0, 3.0));
    vec_data.push_back(Vector2D<double>(3.0, 1.5));
    exporter_double.exportVector2DList(outputFile, vec_data, "Vectores de Posicion");

    // --- Exportar Matriz ---
    Matrix<double> myMatrix(1.1, 2.2, 3.3, 4.4);
    exporter_double.exportMatrix(outputFile, myMatrix, "Matriz de Transformacion");

    // --- Exportar Polinomio ---
    Polynomial<double> myPoly({1.0, -0.5, 0.2}); // 1 - 0.5x + 0.2x^2
    exporter_double.exportPolynomial(outputFile, myPoly, -5.0, 5.0, 100, "Polinomio de Prueba");

    // --- Exportar Puntos arbitrarios (ej. resultados de una simulacion) ---
    std::vector<std::pair<double, double>> simulation_points;
    for (int i = 0; i < 10; ++i) {
        double x_val = i * 0.1;
        double y_val = std::sin(x_val * 2 * M_PI); // Asumiendo M_PI definido o <cmath>
        simulation_points.push_back({x_val, y_val});
    }
    exporter_double.exportPoints(outputFile, simulation_points, "Curva Senoidal Simulada");


    // 3. Cerrar el archivo
    outputFile.close();
    std::cout << "Datos exportados a 'simulation_data.dat'." << std::endl;

    // Puedes abrir 'simulation_data.dat' con un editor de texto
    // y usarlo con Gnuplot:
    // gnuplot> plot 'simulation_data.dat' index 0 using 1:2 with lines title "Vectores X Y"
    // gnuplot> plot 'simulation_data.dat' index 2 using 1:2 with lines title "Polinomio"
    // (Ajusta los indices según el orden en que exportes los datos y las líneas en blanco)

    return 0;
}
