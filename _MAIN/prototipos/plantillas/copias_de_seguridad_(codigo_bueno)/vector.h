// vector.h (Renombrado de vec.h)

#ifndef MLB_VECTOR_H // ¡NUEVO nombre para el include guard!
#define MLB_VECTOR_H

#include <iostream>    // Para std::ostream
#include <vector>      // ¡Importante! Este sigue siendo std::vector (el contenedor dinámico de la STL)
#include <cmath>       // Para std::sqrt, std::abs
#include <limits>      // Para std::numeric_limits
#include <type_traits> // Para std::is_floating_point_v
#include <cassert>     // Para assert

// NO usar 'using namespace std;' en archivos de encabezado.

// Inicio de tu propio espacio de nombres
namespace mlb {

template <class T>
class vector // ¡TU CLASE ahora se llama 'vector'!
{
private:
    // Cambio de nombre de 'elements' a 'data' para consistencia con la clase 'matrix'.
    std::vector<T> data; // Aquí SIEMPRE debes usar std::vector para la de la librería estándar

public:
    // --- Constructores y Destructor ---
    vector(); // El constructor también cambia de nombre
    // Usar std::size_t para el tamaño y valor por defecto de T
    vector(std::size_t n, T x = T()); // El constructor también cambia de nombre
    vector(const std::vector<T>& u); // Aquí el parámetro es un std::vector
    vector(const vector<T>& u);      // Constructor de copia para tu mlb::vector
    ~vector();

    // --- Operadores de Acceso ---
    T& operator[](std::size_t i);
    T operator[](std::size_t i) const;

    // --- Métodos de Información ---
    T modulo() const;
    std::size_t size() const; // Retorna std::size_t

    // --- Operador de Flujo (friend) ---
    // Nota: El friend operator<< debe referirse a tu 'mlb::vector',
    // por eso la declaración dentro del namespace ya sabe que 'vector' es mlb::vector.
    friend std::ostream& operator<<(std::ostream& os, const vector<T>& u)
    {
        // Define una tolerancia solo si T es un tipo de punto flotante
        T epsilon = T(); // Inicializa a 0.
        if constexpr (std::is_floating_point_v<T>) { // C++17 feature
            epsilon = std::numeric_limits<T>::epsilon() * 100; // Un épsilon más generoso
        }

        os << "(";
        // Usar std::size_t para el bucle y acceder a 'data'
        for (std::size_t i = 0; i < u.data.size(); ++i)
        {
            // Aplica la lógica de tolerancia para tipos flotantes
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(u.data[i]) < epsilon) {
                    os << T(); // Imprime el valor cero para T
                } else {
                    os << u.data[i];
                }
            } else { // Para tipos no flotantes, imprime directamente
                os << u.data[i];
            }
            os << (i == u.data.size() - 1 ? "" : ", ");
        }
        os << ")";
        return os;
    }
};

// --- Implementación de Funciones Globales para Operaciones con Vectores ---
// Todas estas funciones también deben estar dentro del namespace 'mlb'
// y referirse a 'vector<T>' como mlb::vector<T>

template <class T>
vector<T>::vector() {}

template <class T>
vector<T>::vector(std::size_t n, T x) : data(n, x) {}

template <class T>
vector<T>::vector(const std::vector<T>& u) : data(u) {} // Recibe un std::vector

template <class T>
vector<T>::vector(const vector<T>& u) : data(u.data) {} // Recibe tu mlb::vector

template <class T>
vector<T>::~vector() {}

template <class T>
T& vector<T>::operator[](std::size_t i) { return data[i]; }

template <class T>
T vector<T>::operator[](std::size_t i) const { return data[i]; }

template <class T>
T vector<T>::modulo() const
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
std::size_t vector<T>::size() const
{
    return data.size();
}

template <class T>
vector<T> operator+(const vector<T>& x, const vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la suma.");
    vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] + y[i];
    }
    return result;
}

template <class T>
vector<T> operator-(const vector<T>& x, const vector<T>& y)
{
    assert(x.size() == y.size() && "Error: Los vectores deben tener el mismo tamaño para la resta.");
    vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] - y[i];
    }
    return result;
}

template <class T>
vector<T> operator*(T a, const vector<T>& x)
{
    vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] * a;
    }
    return result;
}

template <class T>
vector<T> operator*(const vector<T>& x, T a)
{
    return a * x;
}

template <class T>
T operator*(const vector<T>& x, const vector<T>& y) // Producto escalar
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
vector<T> operator/(const vector<T>& x, T a)
{
    if constexpr (std::is_floating_point_v<T>) {
        assert(std::abs(a) > std::numeric_limits<T>::epsilon() * 100 && "Error: Division por cero o por un valor muy cercano a cero.");
    } else {
        assert(a != T() && "Error: Division por cero.");
    }

    vector<T> result(x.size());
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] / a;
    }
    return result;
}

} // Fin del namespace mlb

#endif
