#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include <type_traits>
#include <initializer_list> // ¡Nuevo include para initializer_list!

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
    // ¡NUEVO CONSTRUCTOR para initializer_list!
    Vector(std::initializer_list<T> init_list) : data(init_list) {}
    Vector(const Vector<T>& other) = default;
    ~Vector() = default;

    // --- Asignación ---
    Vector<T>& operator=(const Vector<T>& other) = default;

    // --- Acceso ---
    T& operator[](std::size_t i);
    T operator[](std::size_t i) const;
    std::size_t size() const;

    // --- Propiedades matemáticas ---
    T norm() const;                                 // Norma (módulo)
    Vector<T> normalized() const;                   // Vector unitario
    T dot(const Vector<T>& other) const;            // Producto escalar
    Vector<T> cross(const Vector<T>& other) const;  // Producto vectorial (solo R³)

    // --- Impresión ---
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
    {
        T epsilon = T();
        if constexpr (std::is_floating_point_v<T>)
            epsilon = std::numeric_limits<T>::epsilon() * 100;

        os << "(";
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(v.data[i]) < epsilon) {
                    os << T(); // Imprime el valor cero para el tipo T.
                } else {
                    os << v.data[i];
                }
            } else {
                os << v.data[i];
            }

            if (i + 1 < v.size())
                os << ", ";
        }
        os << ")";
        return os;
    }
};

// Implementaciones de métodos (fuera de la clase para evitar el "inline")
template <class T>
Vector<T>::Vector(std::size_t n, T value) : data(n, value) {}

template <class T>
Vector<T>::Vector(const std::vector<T>& values) : data(values) {}

template <class T>
T& Vector<T>::operator[](std::size_t i)
{
    assert(i < data.size() && "Error: Índice de vector fuera de rango.");
    return data[i];
}

template <class T>
T Vector<T>::operator[](std::size_t i) const
{
    assert(i < data.size() && "Error: Índice de vector fuera de rango.");
    return data[i];
}

template <class T>
std::size_t Vector<T>::size() const
{
    return data.size();
}

template <class T>
T Vector<T>::norm() const
{
    T sum_sq = T(); // Inicializa a 0
    for (const T& val : data)
    {
        sum_sq += val * val;
    }
    if constexpr (std::is_floating_point_v<T>) {
        return std::sqrt(sum_sq);
    } else {
        // Para tipos no flotantes, se necesita un cast explícito a un tipo flotante
        // para std::sqrt y luego de vuelta a T.
        return static_cast<T>(std::sqrt(static_cast<double>(sum_sq)));
    }
}

template <class T>
Vector<T> Vector<T>::normalized() const
{
    T n = norm();
    assert(n != T() && "Error: No se puede normalizar un vector nulo.");
    return (*this) / n;
}

template <class T>
T Vector<T>::dot(const Vector<T>& other) const
{
    assert(size() == other.size() && "Error: Los vectores deben tener el mismo tamaño para el producto escalar.");
    T result = T(); // Inicializa a 0
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        result += data[i] * other[i];
    }
    return result;
}

template <class T>
Vector<T> Vector<T>::cross(const Vector<T>& other) const
{
    assert(size() == 3 && other.size() == 3 && "Error: El producto vectorial solo está definido para vectores 3D.");
    Vector<T> result(3);
    result[0] = data[1] * other[2] - data[2] * other[1];
    result[1] = data[2] * other[0] - data[0] * other[2];
    result[2] = data[0] * other[1] - data[1] * other[0];
    return result;
}

// Operadores globales para Vector (no necesitan ser parte de la clase Vector)
template <class T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b)
{
    assert(a.size() == b.size() && "Error: Los vectores deben tener el mismo tamaño para la suma.");
    Vector<T> result(a.size());
    for (std::size_t i = 0; i < a.size(); ++i)
        result[i] = a[i] + b[i];
    return result;
}

template <class T>
Vector<T> operator-(const Vector<T>& a, const Vector<T>& b)
{
    assert(a.size() == b.size() && "Error: Los vectores deben tener el mismo tamaño para la resta.");
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

#endif // VECTOR_H