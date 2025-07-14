#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

using namespace std;

/*Declaracion de la clase Complex*/

template <class T>
class Complex
{
private:  // variables miembros
    T re; // parte real del complejo
    T im; // parte imaginaria del complejo

public: // metodos, constructores y destructores
    Complex();                                     //  constructor por defecto
    Complex(T a, T b = T(0));                      //  constructor parametrizado
    ~Complex();                                    //  destructor
    Complex(const Complex<T> &);              //  constructor de copia (copia superficial)
    Complex<T> operator=(const Complex<T> &); //  operador de asignacion (copia profunda)
    T operator()(int) const;                            //  operador "()"
    T &operator()(int);                                 //  lectura-escritura
    T get_re() const;                                   //  parte real de un complejo
    T get_im() const;                                   //  parte imaginaria de un complejo
    T norma() const;                                    //  norma de un complejo
    Complex<T> conjugado() const;                                //  conjugado de un complejo
};

/*Implementacion de la clase Complex*/

template <class T>
Complex<T>::Complex() //...constructor por defecto
{
    re = T(0); // la parte real del numero complejo se inicializa en cero
    im = T(0); // la parte imaginaria del numero complejo se inicializa en cero
}

template <class T>
Complex<T>::Complex(T a, T b) //...constructor parametrizado
{
    re = a; // asignamos el valor del parametro "a" a la variable miembro 're'
    im = b; // asignamos el valor del parametro "b" a la variable miembro 'im'
}

template <class T>
Complex<T>::~Complex() //...destructor
{
    // bloque de instrucciones
}

template <class T>
Complex<T>::Complex(const Complex<T> &r) //...constructor de copia
{
    re = r.re; // se asigna la parte real del objeto pasado como argumento a la parte real del objeto actual
    im = r.im; // se asigna la parte imaginaria del objeto pasado como argumento a la parte imaginaria del objeto actual
}

template <class T>
Complex<T> Complex<T>::operator=(const Complex<T> &r) //...operador de asignacion "="
{
    if (this != &r)
    {
        re = r.re; // se asigna la parte real del objeto pasado como argumento a la parte real del objeto actual
        im = r.im; // se asigna la parte imaginaria del objeto pasado como argumento a la parte imaginaria del objeto actual
    }
    return *this;
}

template <class T>
T Complex<T>::operator()(int i) const //...sobrecarga del operador "()"
{
    if (i == 0)
    {
        return re;
    }
    else if (i == 1)
    {
        return im;
    }
    else
    {
        cerr << "Error: Indice incorrecto en Complex::operator()" << endl;
        return T(0); // Devuelve 0 si el índice es incorrecto
    }
}

template <class T>
T Complex<T>::get_re() const //...funcion que retorna la parte real de un Complex
{
    return re;
}
template <class T>
T Complex<T>::get_im() const //...funcion que retorna la parte imaginaria de un Complex
{
    return im;
}

template <class T>
T Complex<T>::norma() const // norma de un complejo
{
    return sqrt(re * re + im * im);
}

template <class T>
Complex<T> Complex<T>::conjugado() const
{
    return Complex<T>(re, -im);
}

template <class T> // sobrecarga del operador <<
ostream &operator<<(ostream &os, const Complex<T> &r)
{
    os << "(" << r(0) << ", " << r(1) << ")";
    return os;
}

// Operaciones algebraicas y funciones

template <class T> //...suma de complejos
Complex<T> operator+(const Complex<T> &r1, const Complex<T> &r2)
{
    return Complex<T>(r1(0) + r2(0), r1(1) + r2(1));
}

template <class T> //...resta de complejos
Complex<T> operator-(const Complex<T> &r1, const Complex<T> &r2)
{
    return Complex<T>(r1(0) - r2(0), r1(1) - r2(1));
}

template <class T> //...producto de complejos
Complex<T> operator*(const Complex<T> &r1, const Complex<T> &r2)
{
    return Complex<T>(r1(0) * r2(0) - r1(1) * r2(1), r2(0) * r1(1) + r1(0) * r2(1));
}

template <class T> //...cociente de complejos
Complex<T> operator/(const Complex<T> &r1, const Complex<T> &r2)
{
    double norma = r2.norma() * r2.norma();
    return Complex<T>((r1(1) * r2(1) + r1(0) * r2(0)) / norma, (-r1(0) * r2(1) + r1(1) * r2(0)) / norma);
}

template <class T> //...seno de un complejo
Complex<T> sin(const Complex<T> &r)
{
    return Complex<T>(sin(r(0)) * cosh(r(1)), cos(r(0)) * sinh(r(1)));
}

template <class T> //...coseno de un complejo
Complex<T> cos(const Complex<T> &r)
{
    return Complex<T>(cos(r(0)) * cosh(r(1)), -sin(r(0)) * sinh(r(1)));
}

#endif