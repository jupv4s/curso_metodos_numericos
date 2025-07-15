#include "matriz_exponencial.h"
#include <cmath> // Para std::sqrt, std::cos, std::sin

/**
 * @brief Calcula la norma de Frobenius de una matriz.
 * Es la raíz cuadrada de la suma de los cuadrados de las magnitudes de sus elementos.
 * Adaptado para usar los métodos .real() e .imag() de tu clase Complex.
 */
double frobenius_norm(const Matrix<CustomComplex>& m)
{
    double sum = 0.0;
    for (size_t i = 0; i < m.rows(); ++i)
    {
        for (size_t j = 0; j < m.cols(); ++j)
        {
            // Usamos .real() y .imag() de tu clase para calcular el cuadrado de la magnitud
            double re = m(i, j).real();
            double im = m(i, j).imag();
            sum += (re * re + im * im);
        }
    }
    return std::sqrt(sum);
}

// Implementación de la exponencial con la relación de Euler
Matrix<CustomComplex> exponencial_euler(const Matrix<CustomComplex>& sigma, double theta) {
    const CustomComplex i(0.0, 1.0);
    Matrix<CustomComplex> I(sigma.rows()); // Matriz identidad

    // Usamos los operadores de tu clase Matrix
    // CORRECCIÓN: Convertimos explícitamente los double a CustomComplex antes de multiplicar
    Matrix<CustomComplex> term1 = CustomComplex(std::cos(theta)) * I;
    Matrix<CustomComplex> term2 = (i * CustomComplex(std::sin(theta))) * sigma;

    return term1 + term2;
}

// Implementación de la exponencial con la serie de Taylor
Matrix<CustomComplex> exponencial_taylor(const Matrix<CustomComplex>& A, double tolerance) {
    Matrix<CustomComplex> resultado = Matrix<CustomComplex>(A.rows()); // k=0, Identidad
    Matrix<CustomComplex> termino_actual = Matrix<CustomComplex>(A.rows());
    Matrix<CustomComplex> potencia_A = Matrix<CustomComplex>(A.rows());
    
    double k_factorial = 1.0;
    const int max_iter = 100;

    for (int k = 1; k < max_iter; ++k) {
        potencia_A = potencia_A * A;
        k_factorial *= k;
        
        // CORRECCIÓN: Convertimos explícitamente el escalar double a CustomComplex
        termino_actual = CustomComplex(1.0 / k_factorial) * potencia_A;
        resultado = resultado + termino_actual;

        // Criterio de convergencia usando nuestra norma de Frobenius
        if (frobenius_norm(termino_actual) < tolerance) {
            break;
        }
    }
    return resultado;
}

// Implementación de la comparación de matrices
bool son_matrices_cercanas(const Matrix<CustomComplex>& A, const Matrix<CustomComplex>& B, double tolerance)
{
    if (A.rows() != B.rows() || A.cols() != B.cols())
    {
        return false;
    }

    for (size_t i = 0; i < A.rows(); ++i)
    {
        for (size_t j = 0; j < A.cols(); ++j)
        {
            // Usamos la función global abs(Complex) de tu archivo Complex.h
            if (abs(A(i, j) - B(i, j)) > tolerance)
            {
                return false;
            }
        }
    }
    return true;
}