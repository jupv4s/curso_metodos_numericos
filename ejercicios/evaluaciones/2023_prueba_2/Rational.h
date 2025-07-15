#ifndef RATIONAL_H
#define RATIONAL_H

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <numeric>

// Clase plantilla para números racionales
template <class T>
class Rational
{
private:
    T num; // Numerador
    T den; // Denominador

    // Funciones de ayuda internas
    static T gcd(T a, T b)
    {
        return b == T(0) ? a : gcd(b, a % b);
    }

    void simplify() {
        if (den < T(0))
        {
            num = -num;
            den = -den;
        }
        if (num != T(0))
        {
            T common_divisor = gcd(std::abs(num), std::abs(den));
            num /= common_divisor;
            den /= common_divisor;
        }
    }

public:
    // Constructores y Destructor
    Rational() : num(T(0)), den(T(1)) {}
    Rational(T numerator, T denominator) : num(numerator), den(denominator)
    {
        if (den == T(0))
        {
            throw std::invalid_argument("El denominador no puede ser cero.");
        }
        simplify();
    }
    explicit Rational(T numerator) : num(numerator), den(T(1)) {}
    Rational(const Rational<T>& other) = default;
    ~Rational() = default;

    // Operador de Asignación
    Rational<T>& operator=(const Rational<T>& other) = default;

    // Accesores
    T numerator() const { return num; }
    T denominator() const { return den; }
    void set_numerator(T val)
    {
        num = val;
        simplify();
    }
    void set_denominator(T val)
    {
        if (val == T(0))
        {
            throw std::invalid_argument("El denominador no puede ser cero.");
        }
        den = val;
        simplify();
    }

    Rational<T>& operator+=(const Rational<T>& other)
    {
    // La fórmula para a/b += c/d es (ad+bc)/bd
    T new_num = num * other.den + other.num * den;
    T new_den = den * other.den;

    num = new_num;
    den = new_den;

    simplify(); // Simplificar el resultado
    return *this;
}

    // Utilidades de Conversión
    double to_double() const
    {
        return static_cast<double>(num) / static_cast<double>(den);
    }
};

// Operadores de Flujo
template <class T>
std::ostream& operator<<(std::ostream& os, const Rational<T>& r)
{
    os << r.numerator();
    if (r.denominator() != T(1))
    {
        os << "/" << r.denominator();
    }
    return os;
}

template <class T>
std::istream& operator>>(std::istream& is, Rational<T>& r)
{
    T num, den;
    is >> num >> den;
    r = Rational<T>(num, den);
    return is;
}

// Operadores Aritméticos
template <class T>
Rational<T> operator+(const Rational<T>& a, const Rational<T>& b)
{
    T new_num = a.numerator() * b.denominator() + b.numerator() * a.denominator();
    T new_den = a.denominator() * b.denominator();
    return Rational<T>(new_num, new_den);
}

template <class T>
Rational<T> operator-(const Rational<T>& a, const Rational<T>& b)
{
    T new_num = a.numerator() * b.denominator() - b.numerator() * a.denominator();
    T new_den = a.denominator() * b.denominator();
    return Rational<T>(new_num, new_den);
}

template <class T>
Rational<T> operator*(const Rational<T>& a, const Rational<T>& b)
{
    T new_num = a.numerator() * b.numerator();
    T new_den = a.denominator() * b.denominator();
    return Rational<T>(new_num, new_den);
}

template <class T>
Rational<T> operator/(const Rational<T>& a, const Rational<T>& b)
{
    if (b.numerator() == T(0))
    {
        throw std::invalid_argument("División por un racional cero.");
    }
    T new_num = a.numerator() * b.denominator();
    T new_den = a.denominator() * b.numerator();
    return Rational<T>(new_num, new_den);
}

// Operador unario de negación
template <class T>
Rational<T> operator-(const Rational<T>& r)
{
    return Rational<T>(-r.numerator(), r.denominator());
}

// Operadores de comparación
template <class T>
bool operator<(const Rational<T>& a, const Rational<T>& b)
{
    // a/b < c/d  <=>  ad < cb  (asumiendo denominadores positivos)
    return a.numerator() * b.denominator() < b.numerator() * a.denominator();
}

template <class T>
bool operator>(const Rational<T>& a, const Rational<T>& b)
{
    // a/b > c/d  <=>  ad > cb  (asumiendo denominadores positivos)
    return a.numerator() * b.denominator() > b.numerator() * a.denominator();
}

// Operadores de Comparación
template <class T>
bool operator==(const Rational<T>& a, const Rational<T>& b)
{
    return a.numerator() == b.numerator() && a.denominator() == b.denominator();
}

template <class T>
bool operator!=(const Rational<T>& a, const Rational<T>& b)
{
    return !(a == b);
}

// Funciones Matemáticas
template <class T>
double sin(const Rational<T>& r)
{
    return std::sin(r.to_double());
}

template <class T>
double cos(const Rational<T>& r)
{
    return std::cos(r.to_double());
}

template <class T>
double tan(const Rational<T>& r)
{
    return std::tan(r.to_double());
}

#endif