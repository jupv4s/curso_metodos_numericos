#ifndef SISTEMA_H
#define SISTEMA_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Complex.h"
#include "Dual.h"

// Usaremos números duales de primer orden (N=2) sobre números complejos.
// Dual<Complex<double>, 2>
// La parte [0] es el valor de la función f(z)
// La parte [1] es el valor de la derivada f'(z)
using DualComplex = Dual<Complex<double>, 2>;

class Sistema {
private:
    // La estructura 'particula' es privada, un detalle de implementación de la clase Sistema.
    struct particula {
        double masa;
        double carga;
        Complex<double> posicion;
        Complex<double> velocidad; // Aunque no se usa en este problema, es parte del diseño.
    };

    // Contenedor para un número arbitrario de partículas.
    std::vector<particula> particulas;

    /**
     * @brief Calcula el potencial complejo W(z) = z^2.
     * Esta función está diseñada para trabajar con números DualComplex para
     * obtener la derivada automáticamente.
     * @param z_dual El punto de evaluación como un número DualComplex.
     * @return El resultado de W(z) y W'(z) empaquetado en un DualComplex.
     */
    DualComplex W(const DualComplex& z_dual) const {
        return z_dual * z_dual;
    }

public:
    // Constructor por defecto.
    Sistema() = default;

    /**
     * @brief Agrega una nueva partícula al sistema.
     * @param m Masa de la partícula.
     * @param q Carga de la partícula.
     * @param pos Posición compleja z = x + iy.
     * @param vel Velocidad compleja.
     */
    void agregarParticula(double m, double q, const Complex<double>& pos, const Complex<double>& vel) {
        particulas.emplace_back(particula{m, q, pos, vel});
    }

    /**
     * @brief Calcula la fuerza sobre cada partícula y muestra/guarda los resultados.
     * La fuerza se calcula como F(z) = -q * conj(W'(z)).
     * @param nombreArchivo El nombre del archivo donde se guardarán los resultados.
     */
    void calcularYGuardarFuerzas(const std::string& nombreArchivo) const {
        std::ofstream archivo_salida(nombreArchivo);
        if (!archivo_salida.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
            return;
        }

        std::cout << "Calculando fuerzas sobre las partículas:" << std::endl;
        archivo_salida << "Resultados del cálculo de fuerzas:" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        archivo_salida << "-----------------------------------------" << std::endl;

        for (const auto& p : particulas) {
            // 1. Crear el número dual-complejo para la diferenciación.
            DualComplex z_dual(p.posicion, Complex<double>(1.0, 0.0));

            // 2. Evaluar W(z) usando el número dual.
            DualComplex w_resultado = W(z_dual);

            // 3. Extraer la derivada W'(z) de la parte dual del resultado.
            Complex<double> w_derivada = w_resultado.dual();

            // 4. Calcular el campo eléctrico E(z) = Ex - iEy = -W'(z).
            Complex<double> campo_electrico_def = -w_derivada;
            
            // 5. Calcular la fuerza F(z) = Fx + iFy.
            //    F = q * (Ex + iEy) = q * conj(E) = q * conj(-W'(z)) = -q * conj(W'(z))
            Complex<double> fuerza = -p.carga * conj(w_derivada);

            // 6. Imprimir y guardar los resultados.
            std::cout << "Partícula en z = " << p.posicion
                      << " con carga q = " << p.carga << std::endl;
            std::cout << "  -> Fuerza F(z) = " << fuerza << std::endl << std::endl;

            archivo_salida << "Partícula en z = " << p.posicion
                           << " (q = " << p.carga << ")" << std::endl;
            archivo_salida << "  -> W'(z) = " << w_derivada << std::endl;
            archivo_salida << "  -> E(z) = Ex-iEy = " << campo_electrico_def << std::endl;
            archivo_salida << "  -> Fuerza F(z) = Fx+iFy = " << fuerza << std::endl << std::endl;
        }

        archivo_salida.close();
        std::cout << "Resultados guardados en el archivo '" << nombreArchivo << "'" << std::endl;
    }
};

#endif // SISTEMA_H