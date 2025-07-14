// Vector.h

#ifndef VECTOR_H // ¡NUEVO nombre para el include guard!
#define VECTOR_H

#include <iostream>    // Para std::ostream
#include <vector>      // Para std::vector (el contenedor dinámico de la STL)
#include <cmath>       // Para std::sqrt, std::abs
#include <limits>      // Para std::numeric_limits
#include <type_traits> // Para std::is_floating_point_v
#include <cassert>     // Para assert

// NO usar 'using namespace std;' en archivos de encabezado.

// La clase Vector ahora está en el ámbito global.
template <class T>
class Vector // ¡TU CLASE ahora se llama 'Vector'!
{
private:
    std::vector<T> data; // Aquí SIEMPRE debes usar std::vector para la de la librería estándar

public:
    // --- Constructores y Destructor ---
    Vector(); // El constructor también cambia de nombre
    Vector(std::size_t n, T x = T());
    Vector(const std::vector<T>& u); // Constructor desde un std::vector
    Vector(const Vector<T>& u);    // Constructor de copia para tu Vector
    ~Vector();

    // --- Operadores de Acceso ---
    T& operator[](std::size_t i);
    T operator[](std::size_t i) const;

    // --- Métodos de Información ---
    T modulo() const;
    std::size_t size() const; // Retorna std::size_t

    // --- Operador de Flujo (friend) ---
    // El friend operator<< se refiere a 'Vector' que ahora está en el ámbito global.
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& u)
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
// Todas estas funciones ahora están en el ámbito global y se refieren a 'Vector<T>'.

template <class T>
Vector<T>::Vector() {}

template <class T>
Vector<T>::Vector(std::size_t n, T x) : data(n, x) {}

template <class T>
Vector<T>::Vector(const std::vector<T>& u) : data(u) {}

template <class T>
Vector<T>::Vector(const Vector<T>& u) : data(u.data) {}

template <class T>
Vector<T>::~Vector() {}

template <class T>
T& Vector<T>::operator[](std::size_t i) { return data[i]; }

template <class T>
T Vector<T>::operator[](std::size_t i) const { return data[i]; }

template <class T>
T Vector<T>::modulo() const
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
std::size_t Vector<T>::size() const
{
    return data.size();
}

template <class T>
Vector<T> operator+(const Vector<T>& x, const Vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la suma.");
    Vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] + y[i];
    }
    return result;
}

template <class T>
Vector<T> operator-(const Vector<T>& x, const Vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la resta.");
    Vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] - y[i];
    }
    return result;
}

template <class T>
Vector<T> operator*(T a, const Vector<T>& x)
{
    Vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] * a;
    }
    return result;
}

template <class T>
Vector<T> operator*(const Vector<T>& x, T a)
{
    return a * x;
}

template <class T>
T operator*(const Vector<T>& x, const Vector<T>& y) // Producto escalar
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
Vector<T> operator/(const Vector<T>& x, T a)
{
    if constexpr (std::is_floating_point_v<T>) {
        assert(std::abs(a) > std::numeric_limits<T>::epsilon() * 100 && "Error: Division por cero o por un valor muy cercano a cero.");
    } else {
        assert(a != T() && "Error: Division por cero.");
    }

    Vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] / a;
    }
    return result;
}

#endif