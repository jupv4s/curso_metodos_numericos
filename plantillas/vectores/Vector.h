#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include <type_traits>

template <class T>
class Vector
{
private:
    std::vector<T> data; // Componentes del vector

public:
    // --- Constructores y destructor ---
    Vector() = default;
    Vector(std::size_t n, T value = T());
    Vector(const std::vector<T>& values);
    Vector(const Vector<T>& other) = default;
    ~Vector() = default;

    // --- Asignación ---
    Vector<T>& operator=(const Vector<T>& other) = default;

    // --- Acceso ---
    T& operator[](std::size_t i);
    T operator[](std::size_t i) const;
    std::size_t size() const;

    // --- Propiedades matemáticas ---
    T norm() const;                                // Norma (módulo)
    Vector<T> normalized() const;                  // Vector unitario
    T dot(const Vector<T>& other) const;           // Producto escalar
    Vector<T> cross(const Vector<T>& other) const; // Producto vectorial (solo R³)

    // --- Impresión ---
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
    {
        T epsilon = T();
        if constexpr (std::is_floating_point_v<T>)
            epsilon = std::numeric_limits<T>::epsilon() * 100;

        os << "(";
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<T>)
                os << (std::abs(v[i]) < epsilon ? T() : v[i]);
            else
                os << v[i];
            if (i != v.size() - 1) os << ", ";
        }
        os << ")";
        return os;
    }
};

// --- Implementaciones de métodos de instancia ---

template <class T>
Vector<T>::Vector(std::size_t n, T value) : data(n, value) {}

template <class T>
Vector<T>::Vector(const std::vector<T>& values) : data(values) {}

template <class T>
T& Vector<T>::operator[](std::size_t i) { return data[i]; }

template <class T>
T Vector<T>::operator[](std::size_t i) const { return data[i]; }

template <class T>
std::size_t Vector<T>::size() const { return data.size(); }

template <class T>
T Vector<T>::norm() const
{
    T sum = T();
    for (const T& val : data)
        sum += val * val;
    return static_cast<T>(std::sqrt(static_cast<double>(sum)));
}

template <class T>
Vector<T> Vector<T>::normalized() const
{
    T magnitude = norm();
    assert(magnitude != T() && "Error: no se puede normalizar un vector nulo.");
    return (*this) / magnitude;
}

template <class T>
T Vector<T>::dot(const Vector<T>& other) const
{
    assert(size() == other.size() && "Error: el producto escalar requiere vectores del mismo tamaño.");
    T result = T();
    for (std::size_t i = 0; i < size(); ++i)
        result += data[i] * other[i];
    return result;
}

template <class T>
Vector<T> Vector<T>::cross(const Vector<T>& other) const
{
    assert(size() == 3 && other.size() == 3 && "Error: el producto vectorial requiere vectores de dimensión 3.");
    return Vector<T>({
        data[1] * other[2] - data[2] * other[1],
        data[2] * other[0] - data[0] * other[2],
        data[0] * other[1] - data[1] * other[0]
    });
}

// --- Operadores externos ---

template <class T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b)
{
    assert(a.size() == b.size() && "Error: los vectores deben tener el mismo tamaño para la suma.");
    Vector<T> result(a.size());
    for (std::size_t i = 0; i < a.size(); ++i)
        result[i] = a[i] + b[i];
    return result;
}

template <class T>
Vector<T> operator-(const Vector<T>& a, const Vector<T>& b)
{
    assert(a.size() == b.size() && "Error: los vectores deben tener el mismo tamaño para la resta.");
    Vector<T> result(a.size());
    for (std::size_t i = 0; i < a.size(); ++i)
        result[i] = a[i] - b[i];
    return result;
}

template <class T>
Vector<T> operator*(T scalar, const Vector<T>& v)
{
    Vector<T> result(v.size());
    for (std::size_t i = 0; i < v.size(); ++i)
        result[i] = scalar * v[i];
    return result;
}

template <class T>
Vector<T> operator*(const Vector<T>& v, T scalar)
{
    return scalar * v;
}

template <class T>
Vector<T> operator/(const Vector<T>& v, T scalar)
{
    assert(scalar != T() && "Error: división por cero.");
    Vector<T> result(v.size());
    for (std::size_t i = 0; i < v.size(); ++i)
        result[i] = v[i] / scalar;
    return result;
}

template <class T>
bool operator==(const Vector<T>& a, const Vector<T>& b)
{
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i] != b[i]) return false;
    return true;
}

template <class T>
bool operator!=(const Vector<T>& a, const Vector<T>& b)
{
    return !(a == b);
}

template <class T>
T operator*(const Vector<T>& a, const Vector<T>& b)
{
    return dot(a, b);
}

// --- Funciones auxiliares estilo STL ---

template <class T>
T norm(const Vector<T>& v) { return v.norm(); }

template <class T>
Vector<T> normalize(const Vector<T>& v) { return v.normalized(); }

template <class T>
T dot(const Vector<T>& a, const Vector<T>& b) { return a.dot(b); }

template <class T>
Vector<T> cross(const Vector<T>& a, const Vector<T>& b) { return a.cross(b); }

#endif