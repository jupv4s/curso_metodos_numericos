#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>    // Para std::ostream, std::cerr, std::endl
#include <vector>      // Para std::vector
#include <string>      // Para std::string
#include <fstream>     // Para std::ofstream
#include <cmath>       // Para std::abs, std::pow, std::sqrt
#include <iomanip>     // Para std::fixed, std::setw, std::setprecision
#include <cassert>     // Para assert (se recomienda <cassert> en lugar de <assert.h>)
#include <limits>      // Para std::numeric_limits (útil para epsilon si T es float/double)
#include <type_traits> // Para std::is_floating_point_v

int contador_gauss = 0;

/* --- Declaración de la clase Matrix --- */
template <class T>
class Matrix
{
private:
    std::vector<std::vector<T>> data; // Almacenamiento interno de los elementos de la matriz.

public:
    // --- Constructor y Destructor ---
    Matrix();                                          // Constructor por defecto
    ~Matrix();                                         // Destructor
    Matrix(int rows, int cols);                        // Crea una matriz vacía de dimensiones especificadas
    Matrix(const Matrix<T> & other);                   // Constructor de copia para duplicar una matriz existente
    Matrix(std::vector<std::vector<T>> elements);      // Inicializa la matriz con un vector de vectores
    Matrix(int n);                                     // Crea una matriz identidad cuadrada de tamaño n x n

    // --- Operadores de Acceso y Asignación ---
    Matrix<T> operator = (const Matrix<T> & other);    // Asigna los contenidos de otra matriz
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
    friend std::ostream & operator << (std::ostream & os, const Matrix<T> & a) // Referencia a 'Matrix<T>'
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
                }
                else
                { // Para tipos no flotantes, imprime directamente
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
// Estas funciones operan con la clase 'Matrix<T>'.

// Operador de Flujo para vector<T> (sigue siendo global, no necesita ser friend de Matrix)
template <class T>
std::ostream & operator << (std::ostream & os, const std::vector<T> & v)
{
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

/* --- Implementación de la clase Matrix --- */

template <class T>
Matrix<T>::Matrix(){} // Referencia a 'Matrix<T>'

template <class T>
Matrix<T>::~Matrix(){} // Referencia a 'Matrix<T>'

template <class T>
Matrix<T>::Matrix(int rows, int cols) // Referencia a 'Matrix<T>'
{
    data.resize(rows);
    for (int i = 0; i < rows; ++i)
    {
        data[i].resize(cols, T()); // Inicializa con el valor por defecto de T (0 para numéricos)
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> & other) // Referencia a 'Matrix<T>'
{
    data = other.data;
}

template <class T>
Matrix<T>::Matrix(std::vector<std::vector<T>> elements) // Referencia a 'Matrix<T>'
{
    data = elements;
}

template <class T>
Matrix<T>::Matrix(int n) // Referencia a 'Matrix<T>'
{
    data.resize(n);
    for (int i = 0; i < n; ++i)
    {
        data[i].resize(n, T());
        data[i][i] = T(1); // El valor 1 para el tipo T
    }
}

template <class T>
Matrix<T> Matrix<T>::operator = (const Matrix<T> & x) // Referencia a 'Matrix<T>'
{
    if (this != & x)
    {
        data = x.data;
    }
    return *this;
}

template <class T>
T Matrix<T>::operator()(int r, int c) const // Referencia a 'Matrix<T>'
{
    return data[r][c];
}

template <class T>
T & Matrix<T>::operator()(int r, int c) // Referencia a 'Matrix<T>'
{
    return data[r][c];
}

template <class T>
std::vector<std::vector<T>> Matrix<T>::get() // Referencia a 'Matrix<T>'
{
    return data;
}

template <class T>
std::size_t Matrix<T>::nrow() const // Referencia a 'Matrix<T>'
{
    return data.size();
}

template <class T>
std::size_t Matrix<T>::ncol() const // Referencia a 'Matrix<T>'
{
    if (data.empty()) {
        return 0;
    }
    return data[0].size();
}

template <class T>
std::vector<T> Matrix<T>::row(int r) { // Referencia a 'Matrix<T>'
    return data[r];
}

template <class T>
std::vector<T> Matrix<T>::col(int c) { // Referencia a 'Matrix<T>'
    std::vector<T> res(data.size());
    for(std::size_t i = 0; i < data.size(); i++) { // Usar std::size_t
        res[i] = data[i][c];
    }
    return res;
}

template <class T>
void Matrix<T>::save(std::string filePath) // Referencia a 'Matrix<T>'
{
    std::ofstream myfile;
    myfile.open(filePath);

    if (!myfile.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo '" << filePath << "' para guardar la matriz." << std::endl;
        return;
    }

    for(std::size_t i = 0; i < data.size(); i++) // Usar std::size_t
    {
        for (std::size_t j = 0; j < data[i].size(); j++) // Usar std::size_t
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                myfile << std::fixed << std::setw(11) << std::setprecision(6) << data[i][j];
            }
            else
            {
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
Matrix<T> operator + (const Matrix<T> & a1, const Matrix<T> & b1) // Referencia a 'Matrix<T>'
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    Matrix<T> suma(a1.nrow(), a1.ncol()); // Referencia a 'Matrix<T>'
    for (std::size_t i = 0; i < a1.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < a1.ncol(); ++j) { // Usar std::size_t
            suma(i,j) = a1(i,j) + b1(i,j);
        }
    }
    return suma;
}

template <class T>
Matrix<T> operator - (const Matrix<T> & a1, const Matrix<T> & b1) // Referencia a 'Matrix<T>'
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    Matrix<T> resta(a1.nrow(), a1.ncol()); // Referencia a 'Matrix<T>'
    for (std::size_t i = 0; i < a1.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < a1.ncol(); ++j) { // Usar std::size_t
            resta(i,j) = a1(i,j) - b1(i,j);
        }
    }
    return resta;
}

template <class T>
Matrix<T> operator * (T scalar, const Matrix<T> & m) // Referencia a 'Matrix<T>'
{
    Matrix<T> producto(m.nrow(), m.ncol()); // Referencia a 'Matrix<T>'
    for (std::size_t i = 0; i < m.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < m.ncol(); ++j) { // Usar std::size_t
            producto(i,j) = scalar * m(i,j);
        }
    }
    return producto;
}

template <class T>
std::vector<T> operator * (const Matrix<T> & a, const std::vector<T> &x) // Referencia a 'Matrix<T>'
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
std::vector<T> operator * (const std::vector<T> & v, const Matrix<T> & m) // Referencia a 'Matrix<T>'
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
Matrix<T> operator * (const Matrix<T> & a1, const Matrix<T> & b1) // Referencia a 'Matrix<T>'
{
    assert(a1.ncol() == b1.nrow());
    Matrix<T> producto(a1.nrow(), b1.ncol()); // Referencia a 'Matrix<T>'
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
std::vector<T> operator + (const std::vector<T> & v1, const std::vector<T> & v2)
{
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
Matrix<T> transpuesta(const Matrix<T> & M) // Referencia a 'Matrix<T>'
{
    std::size_t filas = M.nrow(); // Usar std::size_t
    std::size_t columnas = M.ncol(); // Usar std::size_t

    Matrix<T> m_transpuesta(columnas, filas); // Referencia a 'Matrix<T>'

    for (std::size_t i = 0; i < filas; ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < columnas; ++j) { // Usar std::size_t
            m_transpuesta(j,i) = M(i,j);
        }
    }
    return m_transpuesta;
}

template <class T>
Matrix<T> gauss(const Matrix<T> & x) // Referencia a 'Matrix<T>'
{
    Matrix<T> m_gauss = x; // Referencia a 'Matrix<T>'

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
T determinante(const Matrix<T> & x) // Referencia a 'Matrix<T>'
{
    assert(x.nrow() == x.ncol());

    Matrix<T> m_reducida = gauss(x); // Referencia a 'Matrix<T>'
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
Matrix<T> inversa(const Matrix<T> & x) // Referencia a 'Matrix<T>'
{
    assert(x.nrow() == x.ncol());

    Matrix<T> m_extendida(x.nrow(), x.ncol() * 2); // Referencia a 'Matrix<T>'

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
                return Matrix<T>(0,0); // Retorna una matriz vacía si es singular
            }
        } else {
            if (factor == T()) {
                std::cerr << "Error: Matriz singular. No se puede calcular la inversa." << std::endl;
                return Matrix<T>(0,0); // Retorna una matriz vacía si es singular
            }
        }

        for (std::size_t j = 0; j < m_extendida.ncol(); ++j) { // Usar std::size_t
            m_extendida(i, j) = m_extendida(i, j) / factor;
        }
    }

    Matrix<T> m_inversa(x.nrow(), x.ncol()); // Referencia a 'Matrix<T>'
    for (std::size_t i = 0; i < x.nrow(); ++i) { // Usar std::size_t
        for (std::size_t j = 0; j < x.ncol(); ++j) { // Usar std::size_t
            m_inversa(i, j) = m_extendida(i, j + x.ncol());
        }
    }
    return m_inversa;
}

#endif