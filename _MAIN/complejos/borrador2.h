#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

template <class T>
class Complex
{
private:
    T re; // Parte real
    T im; // Parte imaginaria

public:
    // Constructores y destructor
    Complex(): re(T(0)), im(T(0)) {}
    ~Complex() = default;
    Complex(T real, T imag): re(real), im(imag) {}
    Complex(const Complex<T>& other) = default;
    explicit Complex(T real): re(real), im(T(0)) {}

    // Operador de asignación
    Complex<T>& operator=(const Complex<T>& other) = default;

    // Accesores
    T get_re() const { return re; }
    T get_im() const { return im; }
    void set_re(T val) { re = val; }
    void set_im(T val) { im = val; }

    // Operaciones
    T norma() const
    {
        return std::sqrt(re * re + im * im);
    }

    Complex<T> conjugado() const
    {
        return Complex<T>(re, -im);
    }

    // Métodos tipo STL
    T real() const { return get_re(); }
    T imag() const { return get_im(); }
    void real(T r) { set_re(r); }
    void imag(T i) { set_im(i); }
};

// Operadores de flujo
template <class T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& c)
{
    os << "(" << c.get_re() << ", " << c.get_im() << ")";
    return os;
}

// Operadores aritméticos
template <class T>
Complex<T> operator+(const Complex<T>& a, const Complex<T>& b)
{
    return Complex<T>(a.get_re() + b.get_re(), a.get_im() + b.get_im());
}

template <class T>
Complex<T> operator-(const Complex<T>& a, const Complex<T>& b)
{
    return Complex<T>(a.get_re() - b.get_re(), a.get_im() - b.get_im());
}

template <class T>
Complex<T> operator*(const Complex<T>& a, const Complex<T>& b)
{
    T real = a.get_re() * b.get_re() - a.get_im() * b.get_im();
    T imag = a.get_re() * b.get_im() + a.get_im() * b.get_re();
    return Complex<T>(real, imag);
}

template <class T>
Complex<T> operator/(const Complex<T>& a, const Complex<T>& b)
{
    T denom = b.norma() * b.norma();
    T real = (a.get_re() * b.get_re() + a.get_im() * b.get_im()) / denom;
    T imag = (a.get_im() * b.get_re() - a.get_re() * b.get_im()) / denom;
    return Complex<T>(real, imag);
}

// Funciones trigonométricas
template <class T>
Complex<T> sin(const Complex<T>& c)
{
    return Complex<T>(
        std::sin(c.get_re()) * std::cosh(c.get_im()),
        std::cos(c.get_re()) * std::sinh(c.get_im())
    );
}

template <class T>
Complex<T> cos(const Complex<T>& c)
{
    return Complex<T>(
        std::cos(c.get_re()) * std::cosh(c.get_im()),
        -std::sin(c.get_re()) * std::sinh(c.get_im())
    );
}

#endif