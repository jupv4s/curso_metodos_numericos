#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>    // Para std::abs, std::pow, std::sqrt
#include <iomanip>  // Para std::fixed, std::setw, std::setprecision
#include <assert.h> // Para assert

// Usar 'std::' explícitamente en lugar de 'using namespace std;' en el .h
// para evitar posibles conflictos de nombres cuando otros archivos incluyan este encabezado.

/**
 * Clase para la representación y manipulación de matrices con tipo double.
 *
 * Esta clase proporciona funcionalidades para crear, manipular y realizar operaciones
 * matemáticas comunes con matrices, como suma, resta, multiplicación, transpuesta,
 * determinante e inversa, utilizando valores de punto flotante de doble precisión (double).
 */

class matrix
{
private: 
    std::vector<std::vector<double>> data; // Almacenamiento interno de los elementos de la matriz.

public:
    // --- Declaración de funciones 'friend' (amigas) ---
    // Esto permite que el operador << acceda a los miembros privados de la clase.
    friend std::ostream & operator << (std::ostream & os, const matrix & m);

    // --- Constructores y Destructor ---
    matrix();                                          // Constructor por defecto
    ~matrix();                                         // Destructor
    matrix(int rows, int cols);                        // Crea una matriz vacía de dimensiones especificadas
    matrix(const matrix & other);                      // Constructor de copia para duplicar una matriz existente
    matrix(std::vector<std::vector<double>> elements); // Inicializa la matriz con un vector de vectores
    matrix(int n);                                     // Crea una matriz identidad cuadrada de tamaño n x n

    // --- Operadores de Acceso y Asignación ---
    matrix operator = (const matrix & other);         // Asigna los contenidos de otra matriz
    double operator()(int r, int c) const;            // Acceso de solo lectura a un elemento (matriz[r][c])
    double & operator()(int r, int c);                // Acceso de lectura/escritura a un elemento (matriz[r][c])

    // --- Métodos de Información y Extracción de Datos ---
    std::vector<std::vector<double>> get();          // Retorna una copia de todos los elementos de la matriz
    int nrow() const;                                // Retorna el número de filas de la matriz
    int ncol() const;                                // Retorna el número de columnas de la matriz
    std::vector<double> row(int r);                  // Retorna la fila especificada como un vector
    std::vector<double> col(int c);                  // Retorna la columna especificada como un vector

    // --- Métodos de Entrada/Salida ---
    void save(std::string filePath);                 // Guarda el contenido de la matriz en un archivo CSV o similar
};

// --- Declaraciones de Funciones Globales para Operaciones con Matrices ---

// Operadores de Flujo (para imprimir)
std::ostream & operator << (std::ostream & os, const std::vector<double> & v); // Para imprimir vectores de double

// Operaciones Aritméticas de Matrices
matrix operator + (const matrix & a, const matrix & b);                           // Suma de dos matrices
matrix operator - (const matrix & a, const matrix & b);                           // Resta de dos matrices
matrix operator * (double scalar, const matrix & m);                              // Producto escalar*matriz
std::vector<double> operator * (const matrix & m, const std::vector<double> & v); // Producto matriz*vector (vector columna)
std::vector<double> operator * (const std::vector<double> & v, const matrix & m); // Producto vector*matriz (vector fila)
matrix operator * (const matrix & a, const matrix & b);                           // Producto matriz*matriz

// Operaciones Aritméticas de Vectores (que usan double)
std::vector<double> operator + (const std::vector<double> & v1, const std::vector<double> & v2);
std::vector<double> operator - (const std::vector<double> & v1, const std::vector<double> & v2);

// Funciones Utilitarias Matemáticas
double norm2(const std::vector<double> & v);        // Calcula la norma euclidiana de un vector.
matrix transpuesta(const matrix & m);               // Genera la matriz transpuesta.
matrix gauss(const matrix & m);                     // Realiza la eliminación de Gauss-Jordan.
double determinante(const matrix & m);              // Calcula el determinante de la matriz.
matrix inversa(const matrix & m);                   // Calcula la matriz inversa.

#endif
