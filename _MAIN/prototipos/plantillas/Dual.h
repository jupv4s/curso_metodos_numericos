#ifndef DUAL_H
#define DUAL_H

#include <iostream>
#include <cmath>

using namespace std;

/**
 * Implementa números duales para diferenciación automática.
 *
 * Un número dual tiene la forma a + b*epsilon, donde 'a' es la parte 
 * real y 'b' es el coeficiente de la parte dual infinitesimal 'epsilon' (epsilon^2 = 0).
 */

template <class T>
class dual
{
private:
    T re; // Parte real.
    T du; // Coeficiente de la parte dual.

public:
    dual();
    dual(T a, T b = T(0));
    ~dual();
    
    dual(const dual<T> &);
    dual<T> operator=(const dual<T> &);
    
    T operator()(int) const;
    T &operator()(int);

    T real_part() const;
    T dual_part() const;
};

/* --- Implementación --- */

template <class T>
dual<T>::dual() : re(T(0)), du(T(0)) {}

template <class T>
dual<T>::dual(T a, T b) : re(a), du(b) {}

template <class T>
dual<T>::~dual() {}

template <class T>
dual<T>::dual(const dual<T> &r) : re(r.re), du(r.du) {}

template <class T>
dual<T> dual<T>::operator=(const dual<T> &r)
{
    if (this != &r)
    {
        re = r.re;
        du = r.du;
    }
    return *this;
}

template <class T>
T dual<T>::operator()(int i) const
{
    switch (i)
    {
    case 0: return re;
    case 1: return du;
    default:
        cerr << "Error: Indice de acceso a dual incorrecto. Se esperaba 0 o 1." << endl;
        return T(0);
    }
}

template <class T>
T &dual<T>::operator()(int i)
{
    switch (i)
    {
    case 0: return re;
    case 1: return du;
    default:
        cerr << "Error: Indice de acceso a dual incorrecto. Se esperaba 0 o 1." << endl;
        // Podrías considerar lanzar una excepción en un entorno de producción.
        return re; // Fallback
    }
}

template <class T>
T dual<T>::real_part() const
{
    return re;
}

template <class T>
T dual<T>::dual_part() const
{
    return du;
}

template <class T>
ostream &operator<<(ostream &os, const dual<T> &r)
{
    os << r(0) << " + " << r(1) << "e";
    return os;
}

/* --- Operaciones y Funciones --- */

template <class T>
dual<T> operator+(const dual<T> &r1, const dual<T> &r2)
{
    return dual<T>(r1.re + r2.re, r1.du + r2.du);
}

template <class T>
dual<T> operator-(const dual<T> &r1, const dual<T> &r2)
{
    return dual<T>(r1.re - r2.re, r1.du - r2.du);
}

template <class T>
dual<T> operator*(const T &c, const dual<T> &r)
{
    return dual<T>(c * r.re, c * r.du);
}

template <class T>
dual<T> operator*(const dual<T> &r1, const dual<T> &r2)
{
    return dual<T>(r1.re * r2.re, r1.du * r2.re + r1.re * r2.du);
}

template <class T>
dual<T> operator/(const dual<T> &r1, const dual<T> &r2)
{
    return dual<T>(r1.re / r2.re, (r1.du * r2.re - r1.re * r2.du) / (r2.re * r2.re));
}

template <class T>
dual<T> sin(const dual<T> &r)
{
    return dual<T>(sin(r.re), cos(r.re) * r.du);
}

template <class T>
dual<T> cos(const dual<T> &r)
{
    return dual<T>(cos(r.re), -sin(r.re) * r.du);
}

template <class T>
dual<T> tan(const dual<T> &r)
{
    return dual<T>(tan(r.re), (1.0 / (cos(r.re) * cos(r.re))) * r.du);
}

template <class T>
dual<T> ln(const dual<T> &r)
{
    return dual<T>(log(r.re), (r.du / r.re));
}

template <class T>
dual<T> sqrt(const dual<T> &r)
{
    return dual<T>(sqrt(r.re), r.du / (2 * sqrt(r.re)));
}

template <class T>
dual<T> exp(const dual<T> &r)
{
    T exp_re = exp(r.re);
    return dual<T>(exp_re, exp_re * r.du);
}

#endif
