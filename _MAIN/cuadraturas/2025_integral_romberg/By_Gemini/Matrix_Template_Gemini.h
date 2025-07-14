#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cassert>
#include <limits>
#include <type_traits>
#include <algorithm> // Para std::swap

// Variable global para el conteo de intercambios de filas en Gauss.
// DEBE ser 'extern' aquí y definida en UN SOLO .cpp (e.g., Matrix.cpp o un globals.cpp).
extern int contador_gauss; [cite: 1]

template <class T>
class Matrix
{
private:
    std::vector<std::vector<T>> data;

public:
    Matrix();
    ~Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix<T>& other);
    Matrix(std::vector<std::vector<T>> elements);
    Matrix(int n); // Matriz identidad

    Matrix<T> operator=(const Matrix<T>& other);
    T operator()(int r, int c) const;
    T& operator()(int r, int c);

    std::vector<std::vector<T>> get();
    std::size_t nrow() const;
    std::size_t ncol() const;
    std::vector<T> row(int r);
    std::vector<T> col(int c);

    void save(std::string filePath);

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& a)
    {
        T epsilon = T{};
        if constexpr (std::is_floating_point_v<T>) {
            epsilon = std::numeric_limits<T>::epsilon() * 100;
        }

        const auto& x = a.data;

        os << "[";
        for (std::size_t i = 0; i < x.size(); ++i) {
            if (i > 0) os << ",\n ";
            os << "[";
            for (std::size_t j = 0; j < x[i].size(); ++j) {
                if constexpr (std::is_floating_point_v<T>) {
                    os << (std::abs(x[i][j]) < epsilon ? T{} : x[i][j]);
                } else {
                    os << x[i][j];
                }
                if (j < x[i].size() - 1) os << ", ";
            }
            os << "]";
        }
        os << "]";
        return os;
    }
};

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for(std::size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if(i + 1 < v.size()) os << ", ";
    }
    os << "]";
    return os;
}

// Implementación de la clase Matrix (dentro del .h para plantillas)
template <class T> Matrix<T>::Matrix() = default;
template <class T> Matrix<T>::~Matrix() = default;

template <class T>
Matrix<T>::Matrix(int rows, int cols) : data(rows, std::vector<T>(cols, T{})) {}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& other) : data(other.data) {}

template <class T>
Matrix<T>::Matrix(std::vector<std::vector<T>> elements) : data(std::move(elements)) {}

template <class T>
Matrix<T>::Matrix(int n) : data(n, std::vector<T>(n, T{})) {
    for (int i = 0; i < n; ++i) data[i][i] = T{1};
}

template <class T>
Matrix<T> Matrix<T>::operator=(const Matrix<T>& x) {
    if (this != &x) data = x.data;
    return *this;
}

template <class T> T Matrix<T>::operator()(int r, int c) const { return data[r][c]; }
template <class T> T& Matrix<T>::operator()(int r, int c) { return data[r][c]; }

template <class T> std::vector<std::vector<T>> Matrix<T>::get() { return data; }
template <class T> std::size_t Matrix<T>::nrow() const { return data.size(); }
template <class T> std::size_t Matrix<T>::ncol() const { return data.empty() ? 0 : data[0].size(); }
template <class T> std::vector<T> Matrix<T>::row(int r) { return data[r]; }

template <class T>
std::vector<T> Matrix<T>::col(int c) {
    std::vector<T> res(data.size());
    for(std::size_t i = 0; i < data.size(); i++) res[i] = data[i][c];
    return res;
}

template <class T>
void Matrix<T>::save(std::string filePath) {
    std::ofstream myfile(filePath);
    if (!myfile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << filePath << "' para guardar la matriz." << std::endl;
        return;
    }
    for(const auto& row_vec : data) {
        for (std::size_t j = 0; j < row_vec.size(); j++) {
            if constexpr (std::is_floating_point_v<T>) {
                myfile << std::fixed << std::setw(11) << std::setprecision(6) << row_vec[j];
            } else {
                myfile << row_vec[j];
            }
            if(j < row_vec.size() - 1) myfile << ",";
        }
        myfile << "\n";
    }
}

template <class T>
Matrix<T> operator+(const Matrix<T>& a1, const Matrix<T>& b1) {
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    Matrix<T> suma(a1.nrow(), a1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i)
        for (std::size_t j = 0; j < a1.ncol(); ++j)
            suma(i,j) = a1(i,j) + b1(i,j);
    return suma;
}

template <class T>
Matrix<T> operator-(const Matrix<T>& a1, const Matrix<T>& b1) {
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    Matrix<T> resta(a1.nrow(), a1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i)
        for (std::size_t j = 0; j < a1.ncol(); ++j)
            resta(i,j) = a1(i,j) - b1(i,j);
    return resta;
}

template <class T>
Matrix<T> operator*(T scalar, const Matrix<T>& m) {
    Matrix<T> producto(m.nrow(), m.ncol());
    for (std::size_t i = 0; i < m.nrow(); ++i)
        for (std::size_t j = 0; j < m.ncol(); ++j)
            producto(i,j) = scalar * m(i,j);
    return producto;
}

template <class T>
std::vector<T> operator*(const Matrix<T>& a, const std::vector<T>& x) {
    assert(a.ncol() == x.size());
    std::vector<T> resultado(a.nrow(), T{});
    for(std::size_t i = 0; i < a.nrow(); ++i)
        for(std::size_t j = 0; j < a.ncol(); ++j)
            resultado[i] += a(i, j) * x[j];
    return resultado;
}

template <class T>
std::vector<T> operator*(const std::vector<T>& v, const Matrix<T>& m) {
    assert(v.size() == m.nrow());
    std::vector<T> resultado(m.ncol(), T{});
    for (std::size_t j = 0; j < m.ncol(); ++j)
        for (std::size_t k = 0; k < v.size(); ++k)
            resultado[j] += v[k] * m(k, j);
    return resultado;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& a1, const Matrix<T>& b1) {
    assert(a1.ncol() == b1.nrow());
    Matrix<T> producto(a1.nrow(), b1.ncol());
    for (std::size_t i = 0; i < a1.nrow(); ++i)
        for (std::size_t j = 0; j < b1.ncol(); ++j)
            for (std::size_t k = 0; k < a1.ncol(); ++k)
                producto(i,j) += a1(i,k) * b1(k,j);
    return producto;
}

template <class T>
std::vector<T> operator+(const std::vector<T>& v1, const std::vector<T>& v2) {
    assert(v1.size() == v2.size());
    std::vector<T> res(v1.size());
    for(std::size_t i = 0; i < v1.size(); ++i) res[i] = v1[i] + v2[i];
    return res;
}

template <class T>
std::vector<T> operator-(const std::vector<T>& v1, const std::vector<T>& v2) {
    assert(v1.size() == v2.size());
    std::vector<T> res(v1.size());
    for(std::size_t i = 0; i < v1.size(); ++i) res[i] = v1[i] - v2[i];
    return res;
}

template <class T>
T norm2(const std::vector<T>& v) {
    T res = T{};
    for(T val : v) res += (val * val);
    if constexpr (std::is_floating_point_v<T>) return std::sqrt(res);
    return static_cast<T>(std::sqrt(static_cast<double>(res)));
}

template <class T>
Matrix<T> transpuesta(const Matrix<T>& M) {
    Matrix<T> m_transpuesta(M.ncol(), M.nrow());
    for (std::size_t i = 0; i < M.nrow(); ++i)
        for (std::size_t j = 0; j < M.ncol(); ++j)
            m_transpuesta(j,i) = M(i,j);
    return m_transpuesta;
}

template <class T>
Matrix<T> gauss(const Matrix<T>& x) {
    Matrix<T> m_gauss = x;
    contador_gauss = 0; // Reinicio de la variable global 

    for (std::size_t i = 0; i < m_gauss.nrow(); ++i) {
        std::size_t fila_pivote_maximo = i;
        if constexpr (std::is_floating_point_v<T>) {
            for (std::size_t j = i + 1; j < m_gauss.nrow(); ++j) {
                if (std::abs(m_gauss(j, i)) > std::abs(m_gauss(fila_pivote_maximo, i))) {
                    fila_pivote_maximo = j;
                }
            }
        } else {
            for (std::size_t j = i + 1; j < m_gauss.nrow(); ++j) {
                if (m_gauss(j, i) != T{}) {
                    fila_pivote_maximo = j;
                    break;
                }
            }
        }

        if (fila_pivote_maximo != i) {
            for (std::size_t j = 0; j < m_gauss.ncol(); ++j) {
                std::swap(m_gauss(i, j), m_gauss(fila_pivote_maximo, j));
            }
            contador_gauss++;
        }

        if (m_gauss(i, i) == T{}) {
            std::cerr << "Error: Pivote cero o muy cercano a cero. La matriz puede ser singular." << std::endl;
            return Matrix<T>(0,0);
        }

        for (std::size_t j = 0; j < m_gauss.nrow(); ++j) {
            if (i != j) {
                if (m_gauss(i,i) == T{}) continue;
                T factor = m_gauss(j, i) / m_gauss(i, i);
                for (std::size_t k = 0; k < m_gauss.ncol(); ++k) {
                    m_gauss(j, k) -= (factor * m_gauss(i, k));
                }
            }
        }
    }
    return m_gauss;
}

template <class T>
T determinante(const Matrix<T>& x) {
    assert(x.nrow() == x.ncol());
    Matrix<T> m_reducida = gauss(x);
    T det_val = T{1};

    if ((contador_gauss % 2) != 0) det_val *= T{-1};

    for (std::size_t i = 0; i < x.nrow(); ++i) det_val *= m_reducida(i, i);
    return det_val;
}

template <class T>
Matrix<T> inversa(const Matrix<T>& x) {
    assert(x.nrow() == x.ncol());

    Matrix<T> m_extendida(x.nrow(), x.ncol() * 2);
    for (std::size_t i = 0; i < x.nrow(); ++i) {
        for (std::size_t j = 0; j < x.ncol(); ++j) m_extendida(i, j) = x(i, j);
        m_extendida(i, i + x.ncol()) = T{1};
    }

    m_extendida = gauss(m_extendida);

    for (std::size_t i = 0; i < x.nrow(); ++i) {
        T factor = m_extendida(i, i);
        if (factor == T{} || (std::is_floating_point_v<T> && std::abs(factor) < std::numeric_limits<T>::epsilon() * 100)) {
            std::cerr << "Error: Matriz singular. No se puede calcular la inversa." << std::endl;
            return Matrix<T>(0,0);
        }
        for (std::size_t j = 0; j < m_extendida.ncol(); ++j) {
            m_extendida(i, j) /= factor;
        }
    }

    Matrix<T> m_inversa(x.nrow(), x.ncol());
    for (std::size_t i = 0; i < x.nrow(); ++i)
        for (std::size_t j = 0; j < x.ncol(); ++j)
            m_inversa(i, j) = m_extendida(i, j + x.ncol());
    return m_inversa;
}
