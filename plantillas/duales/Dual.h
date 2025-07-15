#ifndef DUAL_H
#define DUAL_H

#include <iostream>
#include <cmath>
#include <cassert>

template <class T>
class Dual
{
private:
    T re; // Parte real
    T du; // Parte dual (coeficiente de ε)

public:
    // --- Constructores y destructor ---
    Dual() : re(T()), du(T()) {}
    Dual(T real, T dual = T()) : re(real), du(dual) {}
    Dual(const Dual<T>& other) = default;
    ~Dual() = default;

    // --- Asignación ---
    Dual<T>& operator=(const Dual<T>& other) = default;

    // --- Accesores ---
    T real() const { return re; }
    T dual() const { return du; }
    void real(T r) { re = r; }
    void dual(T d) { du = d; }

    // --- Operadores aritméticos internos ---
    Dual<T> operator+(const Dual<T>& other) const
    {
        return Dual<T>(re + other.re, du + other.du);
    }

    Dual<T> operator-(const Dual<T>& other) const
    {
        return Dual<T>(re - other.re, du - other.du);
    }

    Dual<T> operator*(const Dual<T>& other) const
    {
        return Dual<T>(
            re * other.re,
            re * other.du + du * other.re
        );
    }

    Dual<T> operator/(const Dual<T>& other) const
    {
        assert(other.re != T() && "Error: división por parte real nula.");
        T denom = other.re * other.re;
        return Dual<T>(
            re / other.re,
            (du * other.re - re * other.du) / denom
        );
    }

    Dual<T> operator-() const
    {
        return Dual<T>(-re, -du);
    }

    // --- Funciones útiles ---
    Dual<T> conj() const { return Dual<T>(re, -du); }
};

// --- Operadores aritméticos externos ---

template <class T>
Dual<T> operator+(const Dual<T>& a, const Dual<T>& b) { return a + b; }

template <class T>
Dual<T> operator-(const Dual<T>& a, const Dual<T>& b) { return a - b; }

template <class T>
Dual<T> operator*(const Dual<T>& a, const Dual<T>& b) { return a * b; }

template <class T>
Dual<T> operator*(T scalar, const Dual<T>& d) { return Dual<T>(scalar * d.real(), scalar * d.dual()); }

template <class T>
Dual<T> operator*(const Dual<T>& d, T scalar) { return scalar * d; }

template <class T>
Dual<T> operator/(const Dual<T>& a, const Dual<T>& b) { return a / b; }

// --- Comparación ---

template <class T>
bool operator==(const Dual<T>& a, const Dual<T>& b)
{
    return a.real() == b.real() && a.dual() == b.dual();
}

template <class T>
bool operator!=(const Dual<T>& a, const Dual<T>& b)
{
    return !(a == b);
}

// --- Funciones matemáticas comunes ---

template <class T>
Dual<T> sin(const Dual<T>& d)
{
    return Dual<T>(std::sin(d.real()), std::cos(d.real()) * d.dual());
}

template <class T>
Dual<T> cos(const Dual<T>& d)
{
    return Dual<T>(std::cos(d.real()), -std::sin(d.real()) * d.dual());
}

template <class T>
Dual<T> tan(const Dual<T>& d)
{
    T cos_r = std::cos(d.real());
    return Dual<T>(std::tan(d.real()), d.dual() / (cos_r * cos_r));
}

template <class T>
Dual<T> exp(const Dual<T>& d)
{
    T exp_r = std::exp(d.real());
    return Dual<T>(exp_r, exp_r * d.dual());
}

template <class T>
Dual<T> log(const Dual<T>& d)
{
    assert(d.real() > T() && "Error: log indefinido para valores no positivos.");
    return Dual<T>(std::log(d.real()), d.dual() / d.real());
}

template <class T>
Dual<T> sqrt(const Dual<T>& d)
{
    assert(d.real() >= T() && "Error: raíz cuadrada indefinida para valores negativos.");
    T root = std::sqrt(d.real());
    return Dual<T>(root, d.dual() / (2 * root));
}

// --- Impresión ---

template <class T>
std::ostream& operator<<(std::ostream& os, const Dual<T>& d)
{
    os << d.real() << " + " << d.dual() << "ε";
    return os;
}

// --- Funciones libres estilo STL ---

template <class T>
T real(const Dual<T>& d) { return d.real(); }

template <class T>
T dual(const Dual<T>& d) { return d.dual(); }

template <class T>
Dual<T> conj(const Dual<T>& d) { return d.conj(); }

#endif