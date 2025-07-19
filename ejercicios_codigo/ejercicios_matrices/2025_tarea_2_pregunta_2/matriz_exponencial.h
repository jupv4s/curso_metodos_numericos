#ifndef MATRIX_EXPONENTIAL_H
#define MATRIX_EXPONENTIAL_H

#include "Matrix.h"
#include "Complex.h"

// Alias para tu clase Complex<double> para mayor comodidad.
using CustomComplex = Complex<double>;

/**
 * @brief Calcula la exponencial de una matriz usando la relación de Euler.
 * Fórmula: exp(i*theta*sigma) = cos(theta)*I + i*sin(theta)*sigma
 */
Matrix<CustomComplex> exponencial_euler(const Matrix<CustomComplex>& sigma, double theta);

/**
 * @brief Calcula la exponencial de una matriz usando su serie de Taylor.
 * La serie se detiene cuando la norma del término actual es menor a la tolerancia.
 */
Matrix<CustomComplex> exponencial_taylor(const Matrix<CustomComplex>& A, double tolerance = 1e-12);

/**
 * @brief Compara si dos matrices son numéricamente cercanas dentro de una tolerancia.
 */
bool son_matrices_cercanas(const Matrix<CustomComplex>& A, const Matrix<CustomComplex>& B, double tolerance = 1e-9);

#endif