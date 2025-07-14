#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>     // Para std::ostream, std::cerr, std::endl
#include <vector>       // Para std::vector
#include <string>       // Para std::string
#include <fstream>      // Para std::ofstream
#include <cmath>        // Para std::abs, std::pow, std::sqrt
#include <iomanip>      // Para std::fixed, std::setw, std::setprecision
#include <assert.h>     // Para assert
#include <limits>       // Para std::numeric_limits (útil para epsilon si T es float/double)
#include <type_traits>  // Para std::is_floating_point_v

// No usar 'using namespace std;' en archivos de encabezado.

// Variable global para el conteo de intercambios de filas en Gauss para el determinante.
// Nota: Para una plantilla, una variable global template-dependiente es compleja.
// Una alternativa es pasar el contador como referencia en 'gauss' y 'determinante'.
// Sin embargo, si 'gauss' se llama independientemente, esta es una forma simple.
int contador_gauss = 0;

/* --- Declaración de la clase Matriz --- */
template <class T>
class matrix
{
private:
    std::vector<std::vector<T>> data; // Almacenamiento interno de los elementos de la matriz.

public:
    // --- Constructor y Destructor ---
    matrix();                                          // Constructor por defecto
    ~matrix();                                         // Destructor
    matrix(int rows, int cols);                        // Crea una matriz vacía de dimensiones especificadas
    matrix(const matrix<T> & other);                   // Constructor de copia para duplicar una matriz existente
    matrix(std::vector<std::vector<T>> elements);      // Inicializa la matriz con un vector de vectores
    matrix(int n);                                     // Crea una matriz identidad cuadrada de tamaño n x n

    // --- Operadores de Acceso y Asignación ---
    matrix<T> operator = (const matrix<T> & other);   // Asigna los contenidos de otra matriz
    T operator()(int r, int c) const;                  // Acceso de solo lectura a un elemento (matriz[r][c])
    T & operator()(int r, int c);                      // Acceso de lectura/escritura a un elemento (matriz[r][c])

    // --- Métodos de Información y Extracción de Datos ---
    std::vector<std::vector<T>> get();                 // Retorna una copia de todos los elementos de la matriz
    std::size_t nrow() const;                          // Retorna el número de filas de la matriz
    std::size_t ncol() const;                          // Retorna el número de columnas de la matriz
    std::vector<T> row(int r);                         // Retorna la fila especificada como un vector
    std::vector<T> col(int c);                         // Retorna la columna especificada como un vector

    // --- Métodos de Entrada/Salida ---
    void save(std::string filePath);                   // Guarda el contenido de la matriz en un archivo CSV o similar

    // --- Implementación de operator << como función amiga inline ---
    // Esto resuelve el problema de acceso a miembros privados y la declaración de friend.
    friend std::ostream & operator << (std::ostream & os, const matrix<T> & a)
    {
        // Define una tolerancia solo si T es un tipo de punto flotante
        T epsilon = T(); // Inicializa a 0.
        if constexpr (std::is_floating_point_v<T>) { // C++17 feature
            epsilon = std::numeric_limits<T>::epsilon() * 100; // Un épsilon más generoso
        }

        const auto& x = a.data;

        os << "[";
        for (std::size_t i = 0; i < x.size(); ++i) // Usar std::size_t para los bucles
        {
            if (i > 0) {
                os << "," << std::endl << " ";
            }
            os << "[";
            for (std::size_t j = 0; j < x[i].size(); ++j) // Usar std::size_t para los bucles
            {
                // Aplica la lógica de tolerancia para tipos flotantes
                if constexpr (std::is_floating_point_v<T>) {
                    if (std::abs(x[i][j]) < epsilon) {
                        os << T(); // Imprime el valor cero para T
                    } else {
                        os << x[i][j];
                    }
                } else { // Para tipos no flotantes, imprime directamente
                    os << x[i][j];
                }

                if (j < x[i].size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
        }
        os << "]";
        return os;
    }
};

/* --- Declaraciones e Implementaciones de Funciones Globales para Operaciones con Matrices y Vectores --- */
// Estas funciones operan con la clase 'matrix<T>'.

// Operador de Flujo para vector<T> (sigue siendo global, no necesita ser friend de matrix)
template <class T>
std::ostream & operator << (std::ostream & os, const std::vector<T> & v) {
    os << "[";
    for(std::size_t i = 0; i < v.size(); ++i) { // Usar std::size_t
        os << v[i];
        if(i + 1 < v.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

/* --- Implementación de la clase matrix --- */

template <class T>
matrix<T>::matrix(){}

template <class T>
matrix<T>::~matrix(){}

template <class T>
matrix<T>::matrix(int rows, int cols)
{
    data.resize(rows);
    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols, T()); // Inicializa con el valor por defecto de T (0 para numéricos)
    }
}

template <class T>
matrix<T>::matrix(const matrix<T> & other)
{
    data = other.data;
}

template <class T>
matrix<T>::matrix(std::vector<std::vector<T>> elements)
{
    data = elements;
}

template <class T>
matrix<T>::matrix(int n)
{
    data.resize(n);
    for (int i = 0; i < n; ++i)
    {
        data[i].resize(n, T());
        data[i][i] = T(1); // El valor 1 para el tipo T
    }
}

template <class T>
matrix<T> matrix<T>::operator = (const matrix<T> & x)
{
    if (this != & x)
    {
        data = x.data;
    }
    return *this;
}

template <class T>
T matrix<T>::operator()(int r, int c) const
{
    return data[r][c];
}

template <class T>
T & matrix<T>::operator()(int r, int c)
{
    return data[r][c];
}

template <class T>
std::vector<std::vector<T>> matrix<T>::get()
{
    return data;
}

template <class T>
std::size_t matrix<T>::nrow() const
{
    return data.size();
}

template <class T>
std::size_t matrix<T>::ncol() const
{
    if (data.empty()) {
        return 0;
    }
    return data[0].size();
}

template <class T>
std::vector<T> matrix<T>::row(int r) {
    return data[r];
}

template <class T>
std::vector<T> matrix<T>::col(int c) {
    std::vector<T> res(data.size());
    for(std::size_t i = 0; i < data.size(); i++) { // Usar std::size_t
        res[i] = data[i][c];
    }
    return res;
}

template <class T>
void matrix<T>::save(std::string filePath)
{
    std::ofstream myfile;
    myfile.open(filePath);
    if (!myfile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << filePath << "' para guardar la matriz." << std::endl;
        return;
    }
    for(std::size_t i = 0; i < data.size(); i++) // Usar std::size_t
    {
        for (std::size_t j = 0; j < data[i].size(); j++) // Usar std::size_t
        {
            if constexpr (std::is_floating_point_v<T>) {
                myfile << std::fixed << std::setw(11) << std::setprecision(6) << data[i][j];
            } else {
                myfile << data[i][j];
            }

            if(j < data[i].size() - 1)
            {
                myfile << ",";
            }
        }
        myfile << std::endl;
    }
    myfile.close();
}

/* --- Implementación de funciones globales (operadores y utilidades) --- */

template <class T>
matrix<T> operator + (const matrix<T> & a1, const matrix<T> & b1)
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    matrix<T> suma(a1.nrow(), a1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < a1.ncol(); ++j) { // Usar std::size_t
            suma(i,j) = a1(i,j) + b1(i,j);
        }
    }
    return suma;
}

template <class T>
matrix<T> operator - (const matrix<T> & a1, const matrix<T> & b1)
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    matrix<T> resta(a1.nrow(), a1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < a1.ncol(); ++j) { // Usar std::size_t
            resta(i,j) = a1(i,j) - b1(i,j);
        }
    }
    return resta;
}

template <class T>
matrix<T> operator * (T scalar, const matrix<T> & m)
{
    matrix<T> producto(m.nrow(), m.ncol());
    for (std::size_t i = 0; i < m.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < m.ncol(); ++j) { // Usar std::size_t
            producto(i,j) = scalar * m(i,j);
        }
    }
    return producto;
}

template <class T>
std::vector<T> operator * (const matrix<T> & a, const std::vector<T> &x)
{
    assert(a.ncol() == x.size());

    std::vector<T> resultado(a.nrow(), T());

    for(std::size_t i = 0; i < a.nrow(); ++i) { // Usar std::size_t
        for(std::size_t j = 0; j < a.ncol(); ++j) { // Usar std::size_t
            resultado[i] = resultado[i] + (a(i, j) * x[j]);
        }
    }
    return resultado;
}

template <class T>
std::vector<T> operator * (const std::vector<T> & v, const matrix<T> & m)
{
    assert(v.size() == m.nrow());

    std::vector<T> resultado(m.ncol(), T());

    for (std::size_t j = 0; j < m.ncol(); ++j) { // Usar std::size_t
        for (std::size_t k = 0; k < v.size(); ++k) { // Usar std::size_t
            resultado[j] = resultado[j] + (v[k] * m(k, j));
        }
    }
    return resultado;
}

template <class T>
matrix<T> operator * (const matrix<T> & a1, const matrix<T> & b1)
{
    assert(a1.ncol() == b1.nrow());
    matrix<T> producto(a1.nrow(), b1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < b1.ncol(); ++j) { // Usar std::size_t
            for (std::size_t k = 0; k < a1.ncol(); ++k) { // Usar std::size_t
                producto(i,j) = producto(i,j) + (a1(i,k) * b1(k,j));
            }
        }
    }
    return producto;
}

template <class T>
std::vector<T> operator + (const std::vector<T> & v1, const std::vector<T> & v2) {
    assert(v1.size() == v2.size());
    std::vector<T> res(v1.size());
    for(std::size_t i = 0; i < v1.size(); ++i) { // Usar std::size_t
        res[i] = v1[i] + v2[i];
    }
    return res;
}

template <class T>
std::vector<T> operator - (const std::vector<T> & v1, const std::vector<T> & v2) {
    assert(v1.size() == v2.size());
    std::vector<T> res(v1.size());
    for(std::size_t i = 0; i < v1.size(); ++i) { // Usar std::size_t
        res[i] = v1[i] - v2[i];
    }
    return res;
}

template <class T>
T norm2(const std::vector<T> & v) {
    T res = T(); // Inicializa a 0 para el tipo T
    for(std::size_t i = 0; i < v.size(); ++i) { res = res + (v[i] * v[i]); } // Usar std::size_t
    if constexpr (std::is_floating_point_v<T>) {
        return std::sqrt(res);
    } else {
        return static_cast<T>(std::sqrt(static_cast<double>(res)));
    }
}


/* --- Implementación de Funciones Utilitarias de Álgebra Lineal --- */

template <class T>
matrix<T> transpuesta(const matrix<T> & M)
{
    std::size_t filas = M.nrow(); // Usar std::size_t
    std::size_t columnas = M.ncol(); // Usar std::size_t

    matrix<T> m_transpuesta(columnas, filas);

    for (std::size_t i = 0; i < filas; ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < columnas; ++j) { // Usar std::size_t
            m_transpuesta(j,i) = M(i,j);
        }
    }
    return m_transpuesta;
}

template <class T>
matrix<T> gauss(const matrix<T> & x)
{
    matrix<T> m_gauss = x;

    contador_gauss = 0; // Reinicio de la variable global

    for (std::size_t i = 0; i < m_gauss.nrow(); ++i) { // Usar std::size_t
        T pivote_val = m_gauss(i, i);
        std::size_t fila_pivote_maximo = i; // Usar std::size_t

        if constexpr (std::is_floating_point_v<T>) {
            for (std::size_t j = i + 1; j < m_gauss.nrow(); ++j) { // Usar std::size_t
                if (std::abs(m_gauss(j, i)) > std::abs(pivote_val)) {
                    pivote_val = m_gauss(j, i);
                    fila_pivote_maximo = j;
                }
            }
        } else {
            for (std::size_t j = i + 1; j < m_gauss.nrow(); ++j) { // Usar std::size_t
                if (m_gauss(j, i) != T()) {
                    pivote_val = m_gauss(j, i);
                    fila_pivote_maximo = j;
                    break;
                }
            }
        }

        if (fila_pivote_maximo != i) {
            for (std::size_t j = 0; j < m_gauss.ncol(); ++j) { // Usar std::size_t
                T temp = m_gauss(i, j);
                m_gauss(i, j) = m_gauss(fila_pivote_maximo, j);
                m_gauss(fila_pivote_maximo, j) = temp;
            }
            contador_gauss += 1;
        }

        if (m_gauss(i, i) == T()) {
            std::cerr << "Error: Pivote cero o muy cercano a cero durante la eliminación gaussiana. La matriz puede ser singular." << std::endl;
        }

        for (std::size_t j = 0; j < m_gauss.nrow(); ++j) { // Usar std::size_t
            if (i != j) {
                if (m_gauss(i,i) == T()) continue;

                T factor = m_gauss(j, i) / m_gauss(i, i);
                for (std::size_t k = 0; k < m_gauss.ncol(); ++k) { // Usar std::size_t
                    m_gauss(j, k) = m_gauss(j, k) - (factor * m_gauss(i, k));
                }
            }
        }
    }
    return m_gauss;
}

template <class T>
T determinante(const matrix<T> & x)
{
    assert(x.nrow() == x.ncol());

    matrix<T> m_reducida = gauss(x);
    T det_val = T(1);

    if ((contador_gauss % 2) != 0) {
        det_val = det_val * T(-1);
    }

    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        det_val = det_val * m_reducida(i, i);
    }
    return det_val;
}

template <class T>
matrix<T> inversa(const matrix<T> & x)
{
    assert(x.nrow() == x.ncol());

    matrix<T> m_extendida(x.nrow(), x.ncol() * 2);

    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < x.ncol(); ++j) { // Usar std::size_t
            m_extendida(i, j) = x(i, j);
        }
    }

    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        m_extendida(i, i + x.ncol()) = T(1);
    }

    m_extendida = gauss(m_extendida);

    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        T factor = m_extendida(i, i);
        if constexpr (std::is_floating_point_v<T>) {
            if (std::abs(factor) < std::numeric_limits<T>::epsilon() * 100) {
                 std::cerr << "Error: Matriz singular (pivote cero o muy cercano). No se puede calcular la inversa." << std::endl;
                 return matrix<T>(0,0);
            }
        } else {
            if (factor == T()) {
                std::cerr << "Error: Matriz singular. No se puede calcular la inversa." << std::endl;
                return matrix<T>(0,0);
            }
        }

        for (std::size_t j = 0; j < m_extendida.ncol(); ++j) { // Usar std::size_t
            m_extendida(i, j) = m_extendida(i, j) / factor;
        }
    }

    matrix<T> m_inversa(x.nrow(), x.ncol());
    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < x.ncol(); ++j) { // Usar std::size_t
            m_inversa(i, j) = m_extendida(i, j + x.ncol());
        }
    }
    return m_inversa;
}

#endif
