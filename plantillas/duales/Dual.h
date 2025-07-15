#ifndef DUAL_H
#define DUAL_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <array>

// Plantilla para números duales generalizados (ε^n = 0)
template <class T, size_t N = 5> // Por defecto para ε^5 = 0
class Dual
{
private:
    std::array<T, N> parts; // Coeficientes para ε^0, ε^1, ..., ε^(N-1)

public:
    // --- Constructores y destructor ---
    Dual() { parts.fill(T(0)); }
    Dual(const T& scalar) {
        parts.fill(T(0));
        parts[0] = scalar;
    }
    // Constructor para inicializar las primeras partes (útil para 1ra derivada)
    Dual(const T& c0, const T& c1) {
        parts.fill(T(0));
        parts[0] = c0;
        parts[1] = c1;
    }
    Dual(const Dual<T, N>& other) = default;
    ~Dual() = default;

    // --- Asignación ---
    Dual<T, N>& operator=(const Dual<T, N>& other) = default;

    // --- Accesores ---
    // Acceso general a cualquier coeficiente
    const T& operator[](size_t i) const { return parts[i]; }
    T& operator[](size_t i) { return parts[i]; }

    // Accesores convenientes para compatibilidad (1er orden)
    T real() const { return parts[0]; }
    T dual() const { return parts[1]; }
    void real(T r) { parts[0] = r; }
    void dual(T d) { parts[1] = d; }

    // --- Operador unario ---
    Dual<T, N> operator-() const {
        Dual<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = -parts[i];
        }
        return result;
    }
};

// --- Operadores aritméticos externos ---

template <class T, size_t N>
Dual<T, N> operator+(const Dual<T, N>& a, const Dual<T, N>& b) {
    Dual<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <class T, size_t N>
Dual<T, N> operator-(const Dual<T, N>& a, const Dual<T, N>& b) {
    Dual<T, N> result;
    for (size_t i = 0; i < N; ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template <class T, size_t N>
Dual<T, N> operator*(const Dual<T, N>& a, const Dual<T, N>& b) {
    Dual<T, N> result;
    for (size_t k = 0; k < N; ++k) {
        T sum = T(0);
        for (size_t i = 0; i <= k; ++i) {
            sum += a[i] * b[k - i];
        }
        result[k] = sum;
    }
    return result;
}

template <class T, size_t N>
Dual<T, N> operator/(const Dual<T, N>& a, const Dual<T, N>& b) {
    assert(b[0] != T(0) && "Error: división por parte real nula.");
    Dual<T, N> result;
    result[0] = a[0] / b[0];
    for (size_t k = 1; k < N; ++k) {
        T sum = T(0);
        for (size_t i = 0; i < k; ++i) {
            sum += result[i] * b[k - i];
        }
        result[k] = (a[k] - sum) / b[0];
    }
    return result;
}

// --- Operaciones con escalares ---
template <class T, size_t N>
Dual<T, N> operator*(const T& scalar, const Dual<T, N>& d) {
    return Dual<T, N>(scalar) * d;
}

template <class T, size_t N>
Dual<T, N> operator*(const Dual<T, N>& d, const T& scalar) {
    return d * Dual<T, N>(scalar);
}

template <class T, size_t N>
Dual<T, N> operator+(const T& scalar, const Dual<T, N>& d) {
    return Dual<T, N>(scalar) + d;
}

template <class T, size_t N>
Dual<T, N> operator+(const Dual<T, N>& d, const T& scalar) {
    return d + Dual<T, N>(scalar);
}

// --- Impresión ---
template <class T, size_t N>
std::ostream& operator<<(std::ostream& os, const Dual<T, N>& d) {
    os << d[0];
    for (size_t i = 1; i < N; ++i) {
        os << " + " << d[i] << "e^" << i;
    }
    return os;
}

#endif