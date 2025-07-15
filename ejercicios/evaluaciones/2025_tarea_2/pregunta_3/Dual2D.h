#ifndef DUAL2D_H
#define DUAL2D_H

#include <cmath>
#include <iostream>
#include <vector>

template <class T>
class Dual2D {
private:
    // f(x,y), df/dx, df/dy, d2f/dx2, d2f/dy2, d2f/dxdy
    T f, fx, fy, fxx, fyy, fxy;

public:
    // --- Constructores ---
    // Constructor por defecto
    Dual2D() : f(0), fx(0), fy(0), fxx(0), fyy(0), fxy(0) {}
    // Constructor para inicializar una variable (x o y) o una constante
    Dual2D(T val, T dx = 0, T dy = 0) : f(val), fx(dx), fy(dy), fxx(0), fyy(0), fxy(0) {}
    // Constructor completo
    Dual2D(T f, T fx, T fy, T fxx, T fyy, T fxy) : f(f), fx(fx), fy(fy), fxx(fxx), fyy(fyy), fxy(fxy) {}

    // --- Accesores (Getters) ---
    T real() const { return f; }
    T df_dx() const { return fx; }
    T df_dy() const { return fy; }
    T d2f_dx2() const { return fxx; }
    T d2f_dy2() const { return fyy; }
    T d2f_dxdy() const { return fxy; }

    // --- Operadores Aritméticos ---
    Dual2D<T> operator+(const Dual2D<T>& other) const {
        return Dual2D<T>(f + other.f, fx + other.fx, fy + other.fy, fxx + other.fxx, fyy + other.fyy, fxy + other.fxy);
    }

    Dual2D<T> operator-(const Dual2D<T>& other) const {
        return Dual2D<T>(f - other.f, fx - other.fx, fy - other.fy, fxx - other.fxx, fyy - other.fyy, fxy - other.fxy);
    }

    Dual2D<T> operator*(const Dual2D<T>& other) const {
        return Dual2D<T>(
            f * other.f,                                          // f
            f * other.fx + fx * other.f,                          // fx
            f * other.fy + fy * other.f,                          // fy
            f * other.fxx + 2 * fx * other.fx + fxx * other.f,    // fxx
            f * other.fyy + 2 * fy * other.fy + fyy * other.f,    // fyy
            f * other.fxy + fx * other.fy + fy * other.fx + fxy * other.f // fxy
        );
    }

    Dual2D<T> operator/(const Dual2D<T>& other) const {
        T denom = other.f * other.f;
        T term_fx = (fx * other.f - f * other.fx) / denom;
        T term_fy = (fy * other.f - f * other.fy) / denom;
        return Dual2D<T>(
            f / other.f,
            term_fx,
            term_fy,
            (fxx * other.f - f * other.fxx - 2 * term_fx * other.fx) / other.f,
            (fyy * other.f - f * other.fyy - 2 * term_fy * other.fy) / other.f,
            (fxy * other.f - f * other.fxy - term_fx * other.fy - term_fy * other.fx) / other.f
        );
    }
};

// --- Operadores con escalares (double, int, etc.) ---
template <class T>
Dual2D<T> operator+(T scalar, const Dual2D<T>& d) {
    return Dual2D<T>(scalar) + d;
}
template <class T>
Dual2D<T> operator+(const Dual2D<T>& d, T scalar) {
    return d + Dual2D<T>(scalar);
}
template <class T>
Dual2D<T> operator*(T scalar, const Dual2D<T>& d) {
    return Dual2D<T>(scalar) * d;
}
template <class T>
Dual2D<T> operator*(const Dual2D<T>& d, T scalar) {
    return d * Dual2D<T>(scalar);
}


// --- Funciones matemáticas sobrecargadas ---
template <class T>
Dual2D<T> sin(const Dual2D<T>& d) {
    T sin_f = std::sin(d.real());
    T cos_f = std::cos(d.real());
    T fx = d.df_dx();
    T fy = d.df_dy();

    return Dual2D<T>(
        sin_f,
        cos_f * fx,
        cos_f * fy,
        cos_f * d.d2f_dx2() - sin_f * fx * fx,
        cos_f * d.d2f_dy2() - sin_f * fy * fy,
        cos_f * d.d2f_dxdy() - sin_f * fx * fy
    );
}

template <class T>
Dual2D<T> cos(const Dual2D<T>& d) {
    T sin_f = std::sin(d.real());
    T cos_f = std::cos(d.real());
    T fx = d.df_dx();
    T fy = d.df_dy();

    return Dual2D<T>(
        cos_f,
        -sin_f * fx,
        -sin_f * fy,
        -sin_f * d.d2f_dx2() - cos_f * fx * fx,
        -sin_f * d.d2f_dy2() - cos_f * fy * fy,
        -sin_f * d.d2f_dxdy() - cos_f * fx * fy
    );
}

template <class T>
Dual2D<T> pow(const Dual2D<T>& base, double exp) {
    T f = base.real();
    T fx = base.df_dx();
    T fy = base.df_dy();
    T pow_f_1 = std::pow(f, exp - 1);
    T pow_f_2 = std::pow(f, exp - 2);

    return Dual2D<T>(
        std::pow(f, exp),
        exp * pow_f_1 * fx,
        exp * pow_f_1 * fy,
        exp * (pow_f_1 * base.d2f_dx2() + (exp - 1) * pow_f_2 * fx * fx),
        exp * (pow_f_1 * base.d2f_dy2() + (exp - 1) * pow_f_2 * fy * fy),
        exp * (pow_f_1 * base.d2f_dxdy() + (exp - 1) * pow_f_2 * fx * fy)
    );
}

// --- Impresión ---
template <class T>
std::ostream& operator<<(std::ostream& os, const Dual2D<T>& d) {
    os << "f: " << d.real() << ", fx: " << d.df_dx() << ", fy: " << d.df_dy()
       << ", fxx: " << d.d2f_dx2() << ", fyy: " << d.d2f_dy2() << ", fxy: " << d.d2f_dxdy();
    return os;
}

#endif