#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <cmath>

template <typename T>
class Polynomial
{
private:
    // Coeficientes almacenados del grado más bajo al más alto.
    // p(x) = coeffs[0] + coeffs[1]*x + coeffs[2]*x^2 + ...
    std::vector<T> coeffs;

    // Función de ayuda para eliminar los ceros sobrantes de mayor grado.
    void trim()
    {
        while (coeffs.size() > 1 && coeffs.back() == T(0))
        {
            coeffs.pop_back();
        }
    }

public:
    // --- Constructores ---

    // Polinomio cero por defecto
    Polynomial() : coeffs{T(0)} {}

    // A partir de un vector de coeficientes
    Polynomial(const std::vector<T>& c) : coeffs(c)
    {
        if (coeffs.empty()) {
            coeffs.push_back(T(0));
        }
        trim();
    }

    // A partir de una lista de inicialización (muy conveniente)
    // Ej: Polynomial<int> p = {1, -2, 3}; // 1 - 2x + 3x^2
    Polynomial(std::initializer_list<T> il) : coeffs(il)
    {
        if (coeffs.empty()) {
            coeffs.push_back(T(0));
        }
        trim();
    }

    // --- Acceso y Propiedades ---

    // Grado del polinomio
    int degree() const {
        if (coeffs.size() == 1 && coeffs[0] == T(0))
        {
            return -1; // Grado del polinomio cero
        }
        return coeffs.size() - 1;
    }

    // Acceso a los coeficientes mediante el operador []
    T operator[](size_t i) const
    {
        return i < coeffs.size() ? coeffs[i] : T(0);
    }
    T& operator[](size_t i)
    {
        if (i >= coeffs.size())
        {
            coeffs.resize(i + 1, T(0));
        }
        return coeffs[i];
    }
    
    // --- Evaluación ---

    // Evalúa el polinomio en un punto x usando el Método de Horner.
    // Es una plantilla de método para aceptar tipos diferentes (ej: Dual).
    template <typename U>
    U evaluate(const U& x) const
    {
        //if (degree() == -1) return U(0);
        if (degree() == -1) return U(T(0));
        
        U result = U(coeffs.back()); 
        
        for (int i = degree() - 1; i >= 0; --i)
        {
            result = result * x + coeffs[i];
        }
        return result;
    }
    
    // Alias para la evaluación usando el operador de llamada de función.
    template <typename U>
    U operator()(const U& x) const
    {
        return evaluate(x);
    }

    // --- Cálculo ---

    // Devuelve la derivada del polinomio
    Polynomial<T> derivative() const
    {
        if (degree() <= 0) {
            return Polynomial<T>(); // Derivada de una constante es 0
        }
        std::vector<T> deriv_coeffs(degree());
        for (int i = 0; i < degree(); ++i)
        {
            deriv_coeffs[i] = coeffs[i + 1] * T(i + 1);
        }
        return Polynomial<T>(deriv_coeffs);
    }
    
    // --- Operadores de Asignación Compuesta ---
    Polynomial<T>& operator+=(const Polynomial<T>& other)
    {
        coeffs.resize(std::max(coeffs.size(), other.coeffs.size()), T(0));
        for (size_t i = 0; i < other.coeffs.size(); ++i)
        {
            coeffs[i] += other.coeffs[i];
        }
        trim();
        return *this;
    }
    
    Polynomial<T>& operator-=(const Polynomial<T>& other)
    {
        coeffs.resize(std::max(coeffs.size(), other.coeffs.size()), T(0));
        for (size_t i = 0; i < other.coeffs.size(); ++i)
        {
            coeffs[i] -= other.coeffs[i];
        }
        trim();
        return *this;
    }

    Polynomial<T>& operator*=(const Polynomial<T>& other)
    {
        std::vector<T> result_coeffs(degree() + other.degree() + 1, T(0));
        for (size_t i = 0; i < coeffs.size(); ++i)
        {
            for (size_t j = 0; j < other.coeffs.size(); ++j)
            {
                result_coeffs[i + j] += coeffs[i] * other.coeffs[j];
            }
        }
        coeffs = result_coeffs;
        trim();
        return *this;
    }
};

// --- Operadores Aritméticos Externos ---
template <typename T>
Polynomial<T> operator+(Polynomial<T> a, const Polynomial<T>& b)
{
    return a += b;
}

template <typename T>
Polynomial<T> operator-(Polynomial<T> a, const Polynomial<T>& b)
{
    return a -= b;
}

template <typename T>
Polynomial<T> operator*(Polynomial<T> a, const Polynomial<T>& b)
{
    return a *= b;
}

// --- Comparación ---
template <typename T>
bool operator==(const Polynomial<T>& a, const Polynomial<T>& b)
{
    if (a.degree() != b.degree()) return false;
    for (int i = 0; i <= a.degree(); ++i)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

template <typename T>
bool operator!=(const Polynomial<T>& a, const Polynomial<T>& b)
{
    return !(a == b);
}

// --- Impresión (Stream) ---
template <typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& p)
{
    if (p.degree() == -1) 
    {
        return os << "0";
    }

    bool first_term = true;
    for (int i = p.degree(); i >= 0; --i)
    {
        if (p[i] == T(0)) continue;

        if (!first_term)
        {
            if (p[i] > T(0)) os << " + ";
            else os << " - ";
        } else if (p[i] < T(0))
        {
            os << "-";
        }
        
        first_term = false;

        T abs_coeff = (p[i] > T(0)) ? p[i] : -p[i];
        if (abs_coeff != T(1) || i == 0)
        {
            os << abs_coeff;
        }

        if (i > 0)
        {
            os << "x";
            if (i > 1)
            {
                os << "^" << i;
            }
        }
    }
    if (first_term)
    {
        os << "0";
    }
    return os;
}

#endif