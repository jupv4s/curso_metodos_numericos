// DataManager.tpp
#include "DataManager.h" // Asegurarse de que incluye su propio .h

// Implementación de exportVector (para std::vector)
template <class T>
bool DataManager::exportVector(const std::vector<T>& data, const std::string& filename, int precision) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo '" + filename + "'.");
        }

        // Formato de escritura
        file << std::fixed << std::setprecision(precision);
        //file << std::scientific << std::setprecision(precision);

        // Modificación: Exportar elementos separados por tabulaciones, uno por línea
        for (const auto& val : data) {
            file << val << std::endl;
        }
        file.close();
        std::cout << "std::vector<" << typeid(T).name() << "> exportado con éxito a '" << filename << "'." << std::endl;
        return true; // Éxito
    } catch (const std::runtime_error& e) {
        std::cerr << "Error al exportar std::vector<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    } catch (const std::exception& e) {
        std::cerr << "Error desconocido al exportar std::vector<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    }
}

// Implementación de exportCustomVector (tu clase Vector)
template <class T>
bool DataManager::exportCustomVector(const Vector<T>& vec, const std::string& filename, int precision) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo '" + filename + "'.");
        }

        // Formato de escritura
        file << std::fixed << std::setprecision(precision);
        //file << std::scientific << std::setprecision(precision);
        
        // MODIFICACIÓN PRINCIPAL AQUÍ: Iterar y exportar con tabulaciones
        for (std::size_t i = 0; i < vec.size(); ++i) {
            file << vec[i] << (i == vec.size() - 1 ? "" : "\t"); // Tabulación entre elementos, no al final
        }
        file << std::endl; // Nueva línea al final del vector
        file.close();
        std::cout << "Vector<" << typeid(T).name() << "> exportado con éxito a '" << filename << "'." << std::endl;
        return true; // Éxito
    } catch (const std::runtime_error& e) {
        std::cerr << "Error al exportar Vector<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    } catch (const std::exception& e) {
        std::cerr << "Error desconocido al exportar Vector<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    }
}

// Implementación de exportMatrix (tu clase Matrix)
template <class T>
bool DataManager::exportMatrix(const Matrix<T>& matrix, const std::string& filename, int precision)
{
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo '" + filename + "'.");
        }
        
        // Formato de escritura
        file << std::fixed << std::setprecision(precision);
        //file << std::scientific << std::setprecision(precision);

        // MODIFICACIÓN PRINCIPAL AQUÍ: Iterar filas y columnas con tabulaciones
        for (std::size_t i = 0; i < matrix.rows(); ++i) { // Asumiendo un método 'rows()' en tu clase Matrix
            for (std::size_t j = 0; j < matrix.cols(); ++j) { // Asumiendo un método 'cols()' en tu clase Matrix
                file << matrix(i, j) << (j == matrix.cols() - 1 ? "" : "\t"); // Tabulación entre elementos, no al final de la fila
            }
            file << std::endl; // Nueva línea al final de cada fila
        }
        file.close();
        std::cout << "Matrix<" << typeid(T).name() << "> exportada con éxito a '" << filename << "'." << std::endl;
        return true; // Éxito
    } catch (const std::runtime_error& e) {
        std::cerr << "Error al exportar Matrix<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    } catch (const std::exception& e) {
        std::cerr << "Error desconocido al exportar Matrix<" << typeid(T).name() << "> a '" << filename << "': " << e.what() << std::endl;
        return false; // Fallo
    }
}

// Implementación de exportOdeSolution
bool DataManager::exportOdeSolution(const OdeSolution& result, const std::string& filename, int precision) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo '" + filename + "'.");
        }

        // Formato de escritura
        file << std::fixed << std::setprecision(precision);
        //file << std::scientific << std::setprecision(precision);

        for (std::size_t i = 0; i < result.num_points(); ++i) {
            file << result.times[i]; // Tiempo en la primera columna
            // Iterar sobre los componentes del Vector<double> del estado
            for (std::size_t j = 0; j < result.states[i].size(); ++j) {
                file << "\t" << result.states[i][j]; // Cada componente del estado en una columna separada por tabulación
            }
            file << std::endl; // Nueva línea para el siguiente punto de tiempo
        }
        file.close();
        std::cout << "OdeSolution exportada con éxito a '" << filename << "'." << std::endl;
        return true;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error al exportar OdeSolution a '" << filename << "': " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error desconocido al exportar OdeSolution a '" << filename << "': " << e.what() << std::endl;
        return false;
    }
}