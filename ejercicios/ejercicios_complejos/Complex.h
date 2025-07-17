#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>
#include <sstream>
#include <stdexcept> // Necesario para std::invalid_argument

// Clase plantilla para números complejos
template <class T>
class Complex
{
private:
    T re; // Parte real
    T im; // Parte imaginaria

public:
    // Constructores
    Complex() : re(T(0)), im(T(0)) {}
    Complex(T real, T imag) : re(real), im(imag) {}
    explicit Complex(T real) : re(real), im(T(0)) {}
    Complex(const Complex<T>& other) = default;
    ~Complex() = default;

    // Operador de asignación
    Complex<T>& operator=(const Complex<T>& other) = default;

    // Operadores de asignación compuesta
    Complex<T>& operator+=(const Complex<T>& other) {
        this->re += other.re;
        this->im += other.im;
        return *this;
    }
    Complex<T>& operator*=(const Complex<T>& other) {
        const T old_re = this->re;
        this->re = old_re * other.re - this->im * other.im;
        this->im = old_re * other.im + this->im * other.re;
        return *this;
    }

    // Accesores
    T real() const { return re; }
    T imag() const { return im; }
    void real(T val) { re = val; }
    void imag(T val) { im = val; }

    // Operaciones básicas
    T norm() const { return std::sqrt(re * re + im * im); }
    Complex<T> conj() const { return Complex<T>(re, -im); }
};

// Operadores de flujo
template <class T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& c)
{
    os << "(" << c.real() << ", " << c.imag() << ")";
    return os;
}

template <class T>
std::istream& operator>>(std::istream& is, Complex<T>& c)
{
    T re, im;
    is >> re >> im;
    c.real(re);
    c.imag(im);
    return is;
}

// Operador unario de negación
template <class T>
Complex<T> operator-(const Complex<T>& c)
{
    return Complex<T>(-c.real(), -c.imag());
}

// Operadores aritméticos
template <class T>
Complex<T> operator+(const Complex<T>& a, const Complex<T>& b)
{
    return Complex<T>(a.real() + b.real(), a.imag() + b.imag());
}

template <class T>
Complex<T> operator-(const Complex<T>& a, const Complex<T>& b)
{
    return Complex<T>(a.real() - b.real(), a.imag() - b.imag());
}

template <class T>
Complex<T> operator*(const Complex<T>& a, const Complex<T>& b)
{
    T re = a.real() * b.real() - a.imag() * b.imag();
    T im = a.real() * b.imag() + a.imag() * b.real();
    return Complex<T>(re, im);
}

template <class T>
Complex<T> operator*(const T& scalar, const Complex<T>& c)
{
    return Complex<T>(scalar * c.real(), scalar * c.imag());
}

template <class T>
Complex<T> operator*(const Complex<T>& c, const T& scalar)
{
    return scalar * c;
}

// --- INICIO DE LA SECCIÓN CORREGIDA ---
template <class T>
Complex<T> operator/(const Complex<T>& a, const Complex<T>& b)
{
    T denom = b.real() * b.real() + b.imag() * b.imag();
    // Se compara con T(0) para que funcione con Rational y con tipos nativos
    if (denom == T(0)) {
        // Se lanza una excepción en lugar de retornar NAN,
        // lo cual es más genérico y robusto.
        throw std::invalid_argument("Division por un numero complejo cero.");
    }
    T re = (a.real() * b.real() + a.imag() * b.imag()) / denom;
    T im = (a.imag() * b.real() - a.real() * b.imag()) / denom;
    return Complex<T>(re, im);
}
// --- FIN DE LA SECCIÓN CORREGIDA ---

// Operadores de comparación
template <class T>
bool operator==(const Complex<T>& a, const Complex<T>& b)
{
    return a.real() == b.real() && a.imag() == b.imag();
}

template <class T>
bool operator!=(const Complex<T>& a, const Complex<T>& b)
{
    return !(a == b);
}

// Funciones matemáticas
template <class T>
T abs(const Complex<T>& c)
{
    return c.norm();
}

template <class T>
T arg(const Complex<T>& c)
{
    return std::atan2(c.imag(), c.real());
}

template <class T>
Complex<T> conj(const Complex<T>& c)
{
    return c.conj();
}

template <class T>
Complex<T> polar(T magnitude, T angle)
{
    return Complex<T>(
        magnitude * std::cos(angle),
        magnitude * std::sin(angle)
    );
}

template <class T>
Complex<T> exp(const Complex<T>& c)
{
    T e = std::exp(c.real());
    return Complex<T>(e * std::cos(c.imag()), e * std::sin(c.imag()));
}

template <class T>
Complex<T> log(const Complex<T>& c)
{
    return Complex<T>(std::log(c.norm()), arg(c));
}

template <class T>
Complex<T> pow(const Complex<T>& base, const Complex<T>& exponent)
{
    return exp(exponent * log(base));
}

template <class T>
Complex<T> sqrt(const Complex<T>& c)
{
    T r = std::sqrt(c.norm());
    T theta = arg(c) / 2;
    return polar<T>(r, theta);
}

template <class T>
Complex<T> sin(const Complex<T>& c)
{
    return Complex<T>(
        std::sin(c.real()) * std::cosh(c.imag()),
        std::cos(c.real()) * std::sinh(c.imag())
    );
}

template <class T>
Complex<T> cos(const Complex<T>& c)
{
    return Complex<T>(
        std::cos(c.real()) * std::cosh(c.imag()),
        -std::sin(c.real()) * std::sinh(c.imag())
    );
}

template <class T>
std::string to_polar(const Complex<T>& c)
{
    std::ostringstream oss;
    oss << c.norm() << " * (cos(" << arg(c) << ") + i*sin(" << arg(c) << "))";
    return oss.str();
}

#endif