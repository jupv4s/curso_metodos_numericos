#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>    // Para std::ostream
#include <vector>      // Para std::vector
#include <string>      // Para std::string
#include <fstream>     // Para std::ofstream
#include <cmath>       // Para std::abs, std::sqrt
#include <iomanip>     // Para std::fixed, std::setw, std::setprecision
#include <limits>      // Para std::numeric_limits
#include <type_traits> // Para std::is_floating_point_v
#include <stdexcept>   // Para excepciones estándar como std::out_of_range, std::invalid_argument
#include <utility>     // Para std::swap

#include "Vector.h"

inline int gauss_swap_count = 0;

// Declaraciones adelantadas de las funciones globales para poder declararlas como friend
// (Necesario si se definen después de la clase Matrix)
template <class T> class Matrix; // Declaración adelantada de la clase Matrix

template <class U> Matrix<U> gauss_elimination(const Matrix<U>& x);
template <class U> U determinant(const Matrix<U>& x);
template <class U> Matrix<U> inverse(const Matrix<U>& x);


/* --- Matrix Class Declaration --- */
template <class T>
class Matrix
{
private:
    std::vector<std::vector<T>> data; // Almacenamiento interno de los elementos de la matriz.
    std::size_t rows_ = 0;             // Número de filas.
    std::size_t cols_ = 0;             // Número de columnas.

    // --- Métodos auxiliares privados para operaciones de fila (modificaciones in-place) ---
    // Estos métodos modifican la matriz directamente.
    void swap_rows_internal(std::size_t r1, std::size_t r2) {
        if (r1 >= rows_ || r2 >= rows_) {
            throw std::out_of_range("Error: Índice de fila fuera de rango en swap_rows_internal.");
        }
        std::swap(data[r1], data[r2]);
    }

    void multiply_row_by_scalar_internal(std::size_t r, T scalar) {
        if (r >= rows_) {
            throw std::out_of_range("Error: Índice de fila fuera de rango en multiply_row_by_scalar_internal.");
        }
        for (std::size_t j = 0; j < cols_; ++j) {
            data[r][j] *= scalar;
        }
    }

    // Operación: fila_a_modificar_idx -= factor * fila_a_restar_multiplicada_idx
    void subtract_multiple_of_row_internal(std::size_t row_to_modify_idx, std::size_t row_to_subtract_multiplied_idx, T factor) {
        if (row_to_modify_idx >= rows_ || row_to_subtract_multiplied_idx >= rows_) {
            throw std::out_of_range("Error: Índice de fila fuera de rango en subtract_multiple_of_row_internal.");
        }
        for (std::size_t j = 0; j < cols_; ++j) {
            data[row_to_modify_idx][j] -= factor * data[row_to_subtract_multiplied_idx][j];
        }
    }

    // Método auxiliar para encontrar la fila pivote para una columna específica, comenzando desde una fila dada.
    std::size_t find_pivot_row_internal(std::size_t col_idx, std::size_t start_row) {
        if (col_idx >= cols_ || start_row >= rows_) {
            throw std::out_of_range("Error: Índice de columna o fila de inicio fuera de rango en find_pivot_row_internal.");
        }

        std::size_t pivot_row = start_row;
        T pivot_val = data[start_row][col_idx];

        if constexpr (std::is_floating_point_v<T>) {
            for (std::size_t i = start_row + 1; i < rows_; ++i) {
                if (std::abs(data[i][col_idx]) > std::abs(pivot_val)) {
                    pivot_val = data[i][col_idx];
                    pivot_row = i;
                }
            }
        } else {
            // Para tipos no flotantes, encuentra el primer pivote no nulo.
            for (std::size_t i = start_row + 1; i < rows_; ++i) {
                if (data[i][col_idx] != T()) {
                    pivot_val = data[i][col_idx];
                    pivot_row = i;
                    break;
                }
            }
        }
        return pivot_row;
    }


public:
    // --- Declaraciones de funciones amigas para acceso a miembros privados ---
    template <class U> friend Matrix<U> gauss_elimination(const Matrix<U>& x);
    template <class U> friend U determinant(const Matrix<U>& x);
    template <class U> friend Matrix<U> inverse(const Matrix<U>& x);

    // --- Constructores y Destructor ---
    Matrix() : rows_(0), cols_(0) {} // Constructor por defecto.
    ~Matrix() = default;             // Destructor por defecto (std::vector gestiona la memoria).

    // Crea una matriz vacía de dimensiones especificadas.
    Matrix(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols) {
        data.resize(rows);
        for (std::size_t i = 0; i < rows; ++i) {
            data[i].resize(cols, T()); // Inicializa con el valor por defecto de T (ej. 0 para tipos numéricos).
        }
    }

    // Constructor de copia.
    Matrix(const Matrix<T>& other) : data(other.data), rows_(other.rows_), cols_(other.cols_) {}

    // Inicializa la matriz con un vector de vectores.
    // (std::initializer_list sería otra opción para inicialización con {}).
    Matrix(std::vector<std::vector<T>> elements) : data(std::move(elements)) {
        rows_ = data.size();
        cols_ = 0;
        if (rows_ > 0) {
            // Asegura que todas las filas tengan el mismo número de columnas.
            cols_ = data[0].size();
            for (std::size_t i = 1; i < rows_; ++i) {
                if (data[i].size() != cols_) {
                    throw std::invalid_argument("Error: Todas las filas en el vector de inicialización deben tener el mismo número de columnas.");
                }
            }
        }
    }

    // Crea una matriz identidad cuadrada de tamaño n x n.
    explicit Matrix(std::size_t n) : rows_(n), cols_(n) {
        data.resize(n);
        for (std::size_t i = 0; i < n; ++i) {
            data[i].resize(n, T());
            data[i][i] = T(1); // Establece los elementos de la diagonal a 1.
        }
    }

    // --- Operadores de Asignación ---
    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this != &other) { // Comprobación de auto-asignación.
            data = other.data;
            rows_ = other.rows_;
            cols_ = other.cols_;
        }
        return *this;
    }

    // Operador de asignación por movimiento.
    Matrix<T>& operator=(Matrix<T>&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            rows_ = other.rows_;
            cols_ = other.cols_;
            other.rows_ = 0; // Deja la fuente en un estado válido y vacío.
            other.cols_ = 0;
        }
        return *this;
    }

    // --- Operadores de Acceso a Elementos ---
    // Acceso de solo lectura a un elemento (matriz[r][c]).
    const T& operator()(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Error: Índice de matriz fuera de rango.");
        }
        return data[r][c];
    }

    // Acceso de lectura/escritura a un elemento (matriz[r][c]).
    T& operator()(std::size_t r, std::size_t c) {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Error: Índice de matriz fuera de rango.");
        }
        return data[r][c];
    }

    // --- Métodos de Información y Extracción de Datos ---
    // Retorna una copia de todos los elementos de la matriz.
    std::vector<std::vector<T>> get_data() const {
        return data;
    }

    std::size_t rows() const { return rows_; } // Obtiene el número de filas.
    std::size_t cols() const { return cols_; } // Obtiene el número de columnas.

    bool empty() const { return rows_ == 0 || cols_ == 0; } // Comprueba si la matriz está vacía.

    // Retorna la fila especificada como un vector (por copia).
    std::vector<T> get_row(std::size_t r) const {
        if (r >= rows_) {
            throw std::out_of_range("Error: Índice de fila fuera de rango.");
        }
        return data[r];
    }

    // Retorna la columna especificada como un vector (por copia).
    std::vector<T> get_col(std::size_t c) const {
        if (c >= cols_) {
            throw std::out_of_range("Error: Índice de columna fuera de rango.");
        }
        std::vector<T> res(rows_);
        for (std::size_t i = 0; i < rows_; ++i) {
            res[i] = data[i][c];
        }
        return res;
    }

    // --- Métodos de Entrada/Salida ---
    void save(std::string filePath) const {
        std::ofstream myfile;
        myfile.open(filePath);

        if (!myfile.is_open()) {
            throw std::runtime_error("Error: No se pudo abrir el archivo '" + filePath + "' para guardar la matriz.");
        }

        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                if constexpr (std::is_floating_point_v<T>) {
                    myfile << std::fixed << std::setw(11) << std::setprecision(6) << data[i][j];
                } else {
                    myfile << data[i][j];
                }

                if (j < cols_ - 1) {
                    myfile << ",";
                }
            }
            myfile << std::endl;
        }
        myfile.close();
    }

    // --- Operadores de Asignación Compuesta (similar a la STL) ---
    Matrix<T>& operator+=(const Matrix<T>& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Error: Las matrices deben tener las mismas dimensiones para la suma.");
        }
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                data[i][j] += other(i, j);
            }
        }
        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("Error: Las matrices deben tener las mismas dimensiones para la resta.");
        }
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                data[i][j] -= other(i, j);
            }
        }
        return *this;
    }

    Matrix<T>& operator*=(T scalar) {
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }

    // --- Función amiga para el operador de salida << ---
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& a) {
        // Define un épsilon solo si T es un tipo de punto flotante.
        T epsilon = T(); // Inicializa a 0.
        if constexpr (std::is_floating_point_v<T>) { // Característica de C++17.
            epsilon = std::numeric_limits<T>::epsilon() * 100; // Un épsilon más generoso.
        }

        os << "[";
        for (std::size_t i = 0; i < a.rows(); ++i) {
            if (i > 0) {
                os << "," << std::endl << " ";
            }
            os << "[";
            for (std::size_t j = 0; j < a.cols(); ++j) {
                // Aplica la lógica de tolerancia para tipos flotantes.
                if constexpr (std::is_floating_point_v<T>) {
                    if (std::abs(a(i, j)) < epsilon) {
                        os << T(); // Imprime el valor cero para el tipo T.
                    } else {
                        os << a(i, j);
                    }
                } else { // Para tipos no flotantes, imprime directamente.
                    os << a(i, j);
                }

                if (j < a.cols() - 1) {
                    os << ", ";
                }
            }
            os << "]";
        }
        os << "]";
        return os;
    }
};

/* --- Global Functions for Matrix and Vector Operations --- */

// Operador de salida para std::vector<T> (permanece global, no necesita ser amiga de Matrix).
// Este operador es para std::vector, no tu clase Vector. Si tu clase Vector tiene su propio <<,
// este es solo para uso general de std::vector.
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (std::size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i + 1 < v.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

// --- Operadores Binarios (funciones no miembro para permitir conversiones implícitas en ambos lados) ---

template <class T>
Matrix<T> operator+(const Matrix<T>& a1, const Matrix<T>& b1) {
    if (a1.rows() != b1.rows() || a1.cols() != b1.cols()) {
        throw std::invalid_argument("Error: Las matrices deben tener las mismas dimensiones para la suma.");
    }
    Matrix<T> sum_matrix = a1; // Copia para usar +=.
    sum_matrix += b1;
    return sum_matrix;
}

template <class T>
Matrix<T> operator-(const Matrix<T>& a1, const Matrix<T>& b1) {
    if (a1.rows() != b1.rows() || a1.cols() != b1.cols()) {
        throw std::invalid_argument("Error: Las matrices deben tener las mismas dimensiones para la resta.");
    }
    Matrix<T> subtract_matrix = a1; // Copia para usar -=.
    subtract_matrix -= b1;
    return subtract_matrix;
}

template <class T>
Matrix<T> operator*(T scalar, const Matrix<T>& m) {
    Matrix<T> product_matrix = m; // Copia para usar *=.
    product_matrix *= scalar;
    return product_matrix;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& m, T scalar) {
    return scalar * m; // Reutiliza el operador anterior.
}

// ¡Operador de multiplicación Matriz * Vector<T> (tu clase Vector)!
template <class T>
Vector<T> operator*(const Matrix<T>& a, const Vector<T>& x) {
    if (a.cols() != x.size()) {
        throw std::invalid_argument("Error: Las dimensiones de la matriz y el vector no son compatibles para la multiplicación.");
    }
    Vector<T> result_vector(a.rows(), T()); // Construir tu clase Vector
    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < a.cols(); ++j) {
            result_vector[i] += (a(i, j) * x[j]);
        }
    }
    return result_vector;
}

// ¡Operador de multiplicación Vector<T> (tu clase Vector) * Matriz!
template <class T>
Vector<T> operator*(const Vector<T>& v, const Matrix<T>& m) {
    if (v.size() != m.rows()) {
        throw std::invalid_argument("Error: Las dimensiones del vector y la matriz no son compatibles para la multiplicación.");
    }
    Vector<T> result_vector(m.cols(), T()); // Construir tu clase Vector
    for (std::size_t j = 0; j < m.cols(); ++j) {
        for (std::size_t k = 0; k < v.size(); ++k) {
            result_vector[j] += (v[k] * m(k, j));
        }
    }
    return result_vector;
}

template <class T>
Matrix<T> operator*(const Matrix<T>& a1, const Matrix<T>& b1) {
    if (a1.cols() != b1.rows()) {
        throw std::invalid_argument("Error: Las dimensiones de las matrices no son compatibles para la multiplicación.");
    }
    Matrix<T> product_matrix(a1.rows(), b1.cols());
    for (std::size_t i = 0; i < a1.rows(); ++i) {
        for (std::size_t j = 0; j < b1.cols(); ++j) {
            for (std::size_t k = 0; k < a1.cols(); ++k) {
                product_matrix(i, j) += (a1(i, k) * b1(k, j));
            }
        }
    }
    return product_matrix;
}

/* --- Funciones Utilitarias de Álgebra Lineal --- */

template <class T>
Matrix<T> transpose(const Matrix<T>& M) {
    std::size_t num_rows = M.rows();
    std::size_t num_cols = M.cols();

    Matrix<T> transposed_matrix(num_cols, num_rows);

    for (std::size_t i = 0; i < num_rows; ++i) {
        for (std::size_t j = 0; j < num_cols; ++j) {
            transposed_matrix(j, i) = M(i, j);
        }
    }
    return transposed_matrix;
}

// Refactorización de eliminación Gaussiana: ahora utiliza métodos auxiliares privados.
template <class T>
Matrix<T> gauss_elimination(const Matrix<T>& x) {
    Matrix<T> result_matrix = x; // Trabaja sobre una copia.
    gauss_swap_count = 0;      // Reinicia el contador global de intercambios.

    if (result_matrix.empty()) return result_matrix;

    std::size_t num_rows = result_matrix.rows();
    std::size_t num_cols = result_matrix.cols();

    // Itera sobre cada columna (o posición de pivote).
    for (std::size_t i = 0; i < num_rows; ++i) {
        // 1. Encuentra el pivote e intercambia filas si es necesario.
        std::size_t pivot_row_idx = result_matrix.find_pivot_row_internal(i, i);

        if (pivot_row_idx != i) {
            result_matrix.swap_rows_internal(i, pivot_row_idx);
            gauss_swap_count += 1;
        }

        // Obtiene el valor del pivote después del posible intercambio.
        T pivot_value = result_matrix(i, i);

        // Define épsilon para comparaciones de punto flotante.
        T epsilon = T();
        if constexpr (std::is_floating_point_v<T>) {
            epsilon = std::numeric_limits<T>::epsilon() * 100;
        }

        // Si el pivote es cero o muy cercano a cero, la matriz podría ser singular.
        if (pivot_value == T() || (std::is_floating_point_v<T> && std::abs(pivot_value) < epsilon)) {
            // Para la eliminación Gaussiana (forma escalonada por filas), simplemente
            // pasamos a la siguiente columna/pivote. Una matriz singular resultará en
            // un cero en la diagonal en esta etapa.
            continue;
        }

        // 2. Elimina los elementos debajo del pivote (eliminación hacia adelante).
        for (std::size_t j = i + 1; j < num_rows; ++j) {
            T factor = result_matrix(j, i) / pivot_value;
            result_matrix.subtract_multiple_of_row_internal(j, i, factor);
        }
    }
    return result_matrix;
}

template <class T>
T determinant(const Matrix<T>& x) {
    if (x.rows() != x.cols()) {
        throw std::invalid_argument("Error: El determinante solo se puede calcular para matrices cuadradas.");
    }
    if (x.empty()) return T(0); // El determinante de una matriz vacía es 0 (o indefinido según la convención).

    Matrix<T> reduced_matrix = gauss_elimination(x); // Utiliza la eliminación Gaussiana refactorizada.
    T det_value = T(1);

    // Aplica el cambio de signo basado en los intercambios de filas.
    if ((gauss_swap_count % 2) != 0) {
        det_value = det_value * T(-1);
    }

    // Multiplica los elementos de la diagonal.
    for (std::size_t i = 0; i < x.rows(); ++i) {
        det_value = det_value * reduced_matrix(i, i);
    }
    return det_value;
}

// Refactorización de la inversa (utilizando eliminación de Gauss-Jordan).
template <class T>
Matrix<T> inverse(const Matrix<T>& x) {
    if (x.rows() != x.cols()) {
        throw std::invalid_argument("Error: La inversa solo se puede calcular para matrices cuadradas.");
    }
    if (x.empty()) return Matrix<T>(); // Retorna una matriz vacía si la entrada es vacía.

    std::size_t n = x.rows();
    Matrix<T> augmented_matrix(n, n * 2); // Crea la matriz aumentada [A | I].

    // Copia la matriz original a la parte izquierda y la matriz identidad a la parte derecha.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            augmented_matrix(i, j) = x(i, j);
        }
        augmented_matrix(i, i + n) = T(1); // Establece la parte de la identidad.
    }

    // Aplica la eliminación de Gauss-Jordan.
    T epsilon = T();
    if constexpr (std::is_floating_point_v<T>) {
        epsilon = std::numeric_limits<T>::epsilon() * 100;
    }

    // Itera sobre cada fila para crear pivotes y eliminar elementos.
    for (std::size_t i = 0; i < n; ++i) {
        // 1. Encuentra el pivote e intercambia filas.
        std::size_t pivot_row_idx = augmented_matrix.find_pivot_row_internal(i, i);

        if (pivot_row_idx != i) {
            augmented_matrix.swap_rows_internal(i, pivot_row_idx);
        }

        T pivot_value = augmented_matrix(i, i);

        // Comprueba si la matriz es singular (pivote es cero o muy cercano a cero).
        if (pivot_value == T() || (std::is_floating_point_v<T> && std::abs(pivot_value) < epsilon)) {
            throw std::runtime_error("Error: La matriz es singular o casi singular. No se puede calcular la inversa.");
        }

        // 2. Normaliza la fila del pivote para que el elemento pivote se convierta en 1.
        augmented_matrix.multiply_row_by_scalar_internal(i, T(1) / pivot_value);

        // 3. Elimina los elementos en la columna actual, tanto por encima como por debajo del pivote.
        for (std::size_t j = 0; j < n; ++j) {
            if (i != j) { // No elimina el propio pivote.
                T factor = augmented_matrix(j, i); // El pivote ahora es 1.
                augmented_matrix.subtract_multiple_of_row_internal(j, i, factor);
            }
        }
    }

    // La parte izquierda de la matriz aumentada ahora es la identidad.
    // La parte derecha es la matriz inversa.
    Matrix<T> inverted_matrix(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            inverted_matrix(i, j) = augmented_matrix(i, j + n);
        }
    }
    return inverted_matrix;
}

#endif