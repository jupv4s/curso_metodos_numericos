#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std; // Añadido para evitar prefijos std::

// No se necesitan <fstream> y <string> para la funcionalidad principal del vector en este archivo.

template <class T>
class vec
{
private:
    vector<T> elements;

public:
    vec();
    vec(int n, T x = 0);
    vec(const vector<T>& u);
    vec(const vec<T>& u);
    ~vec();

    T& operator()(int i);
    T operator()(int i) const;
    T& operator[](int i);
    T operator[](int i) const;

    T modulo() const; // Agregado const para indicar que no modifica el objeto
    int size() const; // Agregado const para indicar que no modifica el objeto
};

template <class T>
vec<T>::vec() {}

template <class T>
vec<T>::vec(int n, T x) : elements(n, x) {} // Lista de inicialización para mayor limpieza

template <class T>
vec<T>::vec(const vector<T>& u) : elements(u) {}

template <class T>
vec<T>::vec(const vec<T>& u) : elements(u.elements) {}

template <class T>
vec<T>::~vec() {}

template <class T>
T& vec<T>::operator()(int i) { return elements[i]; }

template <class T>
T vec<T>::operator()(int i) const { return elements[i]; }

template <class T>
T& vec<T>::operator[](int i) { return elements[i]; }

template <class T>
T vec<T>::operator[](int i) const { return elements[i]; }

template <class T>
T vec<T>::modulo() const
{
    T sum_sq = T(0);
    for (const T& val : elements) // Uso de range-based for loop
    {
        sum_sq += val * val;
    }
    return sqrt(sum_sq);
}

template <class T>
int vec<T>::size() const
{
    return elements.size();
}

template <class T>
ostream& operator<<(ostream& os, const vec<T>& u) // Pasar 'u' por const reference
{
    os << "(";
    for (int i = 0; i < u.size(); ++i) // Uso de pre-incremento
    {
        os << u[i] << (i == u.size() - 1 ? "" : ", ");
    }
    os << ")";
    return os;
}

template <class T>
vec<T> operator+(const vec<T>& x, const vec<T>& y)
{
    // Asumimos que x.size() == y.size() o se añadiría manejo de errores.
    vec<T> result(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] + y[i];
    }
    return result;
}

template <class T>
vec<T> operator-(const vec<T>& x, const vec<T>& y)
{
    vec<T> result(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] - y[i];
    }
    return result;
}

template <class T>
vec<T> operator*(T a, const vec<T>& x)
{
    vec<T> result(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] * a;
    }
    return result;
}

template <class T>
vec<T> operator*(const vec<T>& x, T a)
{
    return a * x; // Reutiliza el operador anterior
}

template <class T>
T operator*(const vec<T>& x, const vec<T>& y) // Producto escalar
{
    T S = T(0);
    for (int i = 0; i < x.size(); ++i)
    {
        S += x[i] * y[i];
    }
    return S;
}

template <class T>
vec<T> operator/(const vec<T>& x, T a)
{
    // Se debería añadir una comprobación para a == 0
    vec<T> result(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        result[i] = x[i] / a; // Cambiado para que sea x[i] / a, no a / x[i]
    }
    return result;
}

#endif
