// mlb_vector.h

#ifndef MLB_VECTOR_H
#define MLB_VECTOR_H

#include <iostream>    // Para std::ostream
#include <vector>      // Para std::vector (el contenedor dinámico de la STL)
#include <cmath>       // Para std::sqrt, std::abs
#include <limits>      // Para std::numeric_limits
#include <type_traits> // Para std::is_floating_point_v
#include <cassert>     // Para assert

// NO usar 'using namespace std;' en archivos de encabezado.

// La clase mlb_vector ahora está en el ámbito global.
template <class T>
class mlb_vector // ¡TU CLASE ahora se llama 'mlb_vector' y no está en un namespace!
{
private:
    std::vector<T> data; // Aquí SIEMPRE debes usar std::vector para la de la librería estándar

public:
    // --- Constructores y Destructor ---
    mlb_vector();
    mlb_vector(std::size_t n, T x = T());
    mlb_vector(const std::vector<T>& u); // Constructor desde un std::vector
    mlb_vector(const mlb_vector<T>& u);    // Constructor de copia para tu mlb_vector
    ~mlb_vector();

    // --- Operadores de Acceso ---
    T& operator[](std::size_t i);
    T operator[](std::size_t i) const;

    // --- Métodos de Información ---
    T modulo() const;
    std::size_t size() const; // Retorna std::size_t

    // --- Operador de Flujo (friend) ---
    // El friend operator<< se refiere a 'mlb_vector' que ahora está en el ámbito global.
    friend std::ostream& operator<<(std::ostream& os, const mlb_vector<T>& u)
    {
        T epsilon = T();
        if constexpr (std::is_floating_point_v<T>) {
            epsilon = std::numeric_limits<T>::epsilon() * 100;
        }

        os << "(";
        for (std::size_t i = 0; i < u.data.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(u.data[i]) < epsilon) {
                    os << T();
                } else {
                    os << u.data[i];
                }
            } else {
                os << u.data[i];
            }
            os << (i == u.data.size() - 1 ? "" : ", ");
        }
        os << ")";
        return os;
    }
};

// --- Implementación de Funciones Globales para Operaciones con Vectores ---
// Todas estas funciones ahora están en el ámbito global y se refieren a 'mlb_vector<T>'.

template <class T>
mlb_vector<T>::mlb_vector() {}

template <class T>
mlb_vector<T>::mlb_vector(std::size_t n, T x) : data(n, x) {}

template <class T>
mlb_vector<T>::mlb_vector(const std::vector<T>& u) : data(u) {}

template <class T>
mlb_vector<T>::mlb_vector(const mlb_vector<T>& u) : data(u.data) {}

template <class T>
mlb_vector<T>::~mlb_vector() {}

template <class T>
T& mlb_vector<T>::operator[](std::size_t i) { return data[i]; }

template <class T>
T mlb_vector<T>::operator[](std::size_t i) const { return data[i]; }

template <class T>
T mlb_vector<T>::modulo() const
{
    T sum_sq = T();
    for (const T& val : data)
    {
        sum_sq += val * val;
    }
    if constexpr (std::is_floating_point_v<T>) {
        return std::sqrt(sum_sq);
    } else {
        return static_cast<T>(std::sqrt(static_cast<double>(sum_sq)));
    }
}

template <class T>
std::size_t mlb_vector<T>::size() const
{
    return data.size();
}

template <class T>
mlb_vector<T> operator+(const mlb_vector<T>& x, const mlb_vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la suma.");
    mlb_vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] + y[i];
    }
    return result;
}

template <class T>
mlb_vector<T> operator-(const mlb_vector<T>& x, const mlb_vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la resta.");
    mlb_vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] - y[i];
    }
    return result;
}

template <class T>
mlb_vector<T> operator*(T a, const mlb_vector<T>& x)
{
    mlb_vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] * a;
    }
    return result;
}

template <class T>
mlb_vector<T> operator*(const mlb_vector<T>& x, T a)
{
    return a * x;
}

template <class T>
T operator*(const mlb_vector<T>& x, const mlb_vector<T>& y) // Producto escalar
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para el producto escalar.");
    T S = T();
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        S += x[i] * y[i];
    }
    return S;
}

template <class T>
mlb_vector<T> operator/(const mlb_vector<T>& x, T a)
{
    if constexpr (std::is_floating_point_v<T>) {
        assert(std::abs(a) > std::numeric_limits<T>::epsilon() * 100 && "Error: Division por cero o por un valor muy cercano a cero.");
    } else {
        assert(a != T() && "Error: Division por cero.");
    }

    mlb_vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] / a;
    }
    return result;
}

#endif
