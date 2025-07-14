#ifndef SOLVER_LINEAR_REGRESSION_H
#define SOLVER_LINEAR_REGRESSION_H

#include <iostream>  // Para std::cerr, std::endl
#include <vector>    // Para std::vector
#include <string>    // Para std::string (rutas de archivo, tipo de regresion)
#include <fstream>   // Para std::ifstream (lectura de archivos)
#include <sstream>   // Para std::istringstream (parsing de líneas)
#include "Matrix.h"

// Clase para realizar regresión lineal múltiple usando Mínimos Cuadrados Ordinarios (OLS).
// Carga datos desde un archivo, calcula los coeficientes y puede mostrarlos.
class linear_regression
{
private:
    std::vector<std::vector<double>> v_dataset; // Datos de entrada: { {x1, x2, ..., y}, ... }
    std::vector<double> m_coefficients;         // Coeficientes calculados (beta_0, beta_1, ...)
    std::string m_regression_type;              // Tipo de regresión ("lineal", "cuadratica", etc.)

    // Método privado para cargar datos desde un archivo.
    // Retorna true si la carga fue exitosa, false en caso contrario.
    bool load_data_from_file(const std::string& file_path);

    // Método privado que contiene la lógica para calcular la regresión lineal.
    // Retorna un vector de coeficientes, o vacío en caso de error.
    std::vector<double> calculate_linear_regression_coefficients();

    // Método privado que orquesta el cálculo de la regresión según el tipo.
    // Se llama desde el constructor.
    void perform_regression_calculation();

public:
    // Constructor por defecto. No recomendado para uso directo en este modelo.
    linear_regression();
    // Destructor.
    ~linear_regression();
    
    // Nuevo constructor principal:
    // Carga los datos del archivo y realiza el cálculo de la regresión.
    // 'file_path': Ruta al archivo de datos.
    // 'regression_type': Cadena que indica el tipo de regresión (ej. "linear").
    explicit linear_regression(const std::string& file_path, const std::string& regression_type = "linear");

    // Muestra los coeficientes calculados de la regresión.
    void show() const; // 'const' porque no modifica el estado del objeto
};

// --- Implementaciones de los métodos de la clase linear_regression ---

linear_regression::linear_regression() {} // Constructor por defecto vacío

linear_regression::~linear_regression() {} // Destructor vacío

// Nuevo constructor principal
linear_regression::linear_regression(const std::string& file_path, const std::string& regression_type)
    : m_regression_type(regression_type) // Inicializa el tipo de regresión
{
    // Carga los datos desde el archivo. Si falla, el dataset estará vacío.
    if (!load_data_from_file(file_path)) {
        std::cerr << "Error: No se pudo cargar el archivo de datos '" << file_path << "'." << std::endl;
        // Podrías lanzar una excepción aquí si es un error fatal.
        return; // Sale si la carga de datos falló
    }

    // Si los datos se cargaron correctamente, realiza el cálculo de la regresión.
    if (!v_dataset.empty()) {
        perform_regression_calculation();
    } else {
        std::cerr << "Advertencia: Dataset vacío, no se realizará la regresión." << std::endl;
    }
}

// Método privado para cargar datos desde un archivo
bool linear_regression::load_data_from_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        // std::cerr ya se maneja en el constructor que llama a esta función.
        return false; // Fallo al abrir el archivo
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ' ')) { // Asumiendo espacio como delimitador
            try {
                row.push_back(std::stod(cell)); // Convierte string a double
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error al parsear el número: " << e.what() << " en línea: " << line << std::endl;
                v_dataset.clear(); // Limpia el dataset para indicar error
                return false;
            } catch (const std::out_of_range& e) {
                std::cerr << "Número fuera de rango: " << e.what() << " en línea: " << line << std::endl;
                v_dataset.clear();
                return false;
            }
        }
        if (!row.empty()) {
            v_dataset.push_back(row);
        }
    }
    file.close();
    return !v_dataset.empty(); // Retorna true si el dataset no está vacío después de la carga
}


// Método privado que contiene la lógica de regresión lineal
std::vector<double> linear_regression::calculate_linear_regression_coefficients()
{
    Matrix<double> m_dataset(v_dataset);
    int num_predictores = m_dataset.ncol() - 1;

    Matrix<double> X(m_dataset.nrow(), num_predictores + 1);
    for (std::size_t i = 0; i < m_dataset.nrow(); ++i)
    {
        X(i, 0) = 1.0; // Intercepto
        for (int j = 0; j < num_predictores; ++j)
        {
            X(i, j + 1) = m_dataset(i, j); // Variables predictoras
        }
    }

    Matrix<double> y(m_dataset.nrow(), 1);
    for (std::size_t i = 0; i < m_dataset.nrow(); ++i)
    {
        y(i, 0) = m_dataset(i, num_predictores); // Variable dependiente (Y)
    }

    Matrix<double> Xt = transpuesta(X);
    Matrix<double> XtX = Xt * X;
    Matrix<double> XtX_inv = inversa(XtX);
    
    if (XtX_inv.nrow() == 0 && XtX_inv.ncol() == 0) {
        std::cerr << "Error en linear_regression::calculate_linear_regression_coefficients(): La matriz (X^T * X) es singular, no se puede calcular la inversa." << std::endl;
        return std::vector<double>(); // Retorna vector vacío si hay error
    }

    Matrix<double> coeficientes_matriz = (XtX_inv * Xt) * y;
    
    std::vector<double> coeficientes;
    for (std::size_t i = 0; i < coeficientes_matriz.nrow(); ++i)
    {
        coeficientes.push_back(coeficientes_matriz(i, 0));
    }
    
    return coeficientes;
}

// Método privado para orquestar el cálculo (permite añadir otros tipos de regresión en el futuro)
void linear_regression::perform_regression_calculation() {
    if (m_regression_type == "linear") {
        m_coefficients = calculate_linear_regression_coefficients();
    } 
    // else if (m_regression_type == "quadratic") {
    //     m_coefficients = calculate_quadratic_regression_coefficients();
    // }
    else {
        std::cerr << "Error: Tipo de regresión '" << m_regression_type << "' no soportado." << std::endl;
        m_coefficients.clear(); // Asegura que los coeficientes estén vacíos en caso de tipo no válido
    }
}


// Método público para mostrar los resultados
void linear_regression::show() const
{
    if (m_coefficients.empty()) {
        std::cout << "No hay coeficientes para mostrar. La regresión no se pudo calcular o el dataset está vacío." << std::endl;
        return;
    }

    std::cout << "\n--- Resultados de la Regresión (" << m_regression_type << ") ---" << std::endl;
    std::cout << "Coeficientes calculados:" << std::endl;
    std::cout << "  Intercepto (beta_0): " << m_coefficients[0] << std::endl;

    for (std::size_t i = 1; i < m_coefficients.size(); ++i) {
        std::cout << "  Coeficiente de X" << i << " (beta_" << i << "): " << m_coefficients[i] << std::endl;
    }

    // Si es una regresión lineal simple (un solo predictor), podemos mostrar la ecuación de la recta.
    if (m_coefficients.size() == 2 && m_regression_type == "linear") {
        std::cout << "\nEcuación del modelo: Y = " 
                  << m_coefficients[1] << "X + " << m_coefficients[0] << std::endl;
    }
    std::cout << "------------------------------------------\n" << std::endl;
}

#endif
