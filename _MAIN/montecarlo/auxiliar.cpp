#include "auxiliar.h"
#include <iostream>
#include <iomanip>
#include <cmath> // Para std::sin, std::cos, std::pow, std::sqrt, std::atan2

// Definir PI si no está en cmath (a veces no está M_PI por defecto)
#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// Puedes definir la precisión de salida aquí también si quieres que sea consistente
const int OUTPUT_PRECISION = 12;

void solveProblem3a()
{
    std::cout << std::fixed << std::setprecision(OUTPUT_PRECISION);

    // --- Problema 3a: Área de la figura polar r(theta) = sin^a(theta) + cos^b(theta) ---
    // Parámetros específicos del Problema 3a según el PDF 
    const int A_POLAR = 1;
    const int B_POLAR = 99; // Corregido según el PDF: b=99, no 9 
    const int N_SAMPLES_POLAR = 10000000; // 10^7 puntos 

    // Límites del recuadro según el PDF 
    double x_min_polar = -0.51;
    double x_max_polar = 1.01;
    double y_min_polar = -0.065;
    double y_max_polar = 1.01;

    // 1. Definir el predicado (lambda) para la figura polar
    // Capturamos A_POLAR y B_POLAR en la lambda
    auto is_inside_polar_figure = [A_POLAR, B_POLAR](double x, double y) -> bool
    {
        double r_prime = std::sqrt(x * x + y * y);
        if (r_prime == 0.0) { // El origen (0,0) siempre está dentro de la figura si r(theta) >= 0 para todo theta.
                              // En este caso, sin(theta)^1 + cos(theta)^99 siempre es >= 0.
            return true;
        }
        double theta_prime = std::atan2(y, x); // El ángulo se obtiene de atan2(y,x) 

        // Asegurarse de que theta_prime esté en [0, 2*PI) para consistencia con la definición de r(theta) en el problema
        // std::atan2 devuelve un ángulo en (-PI, PI].
        // Si el ángulo es negativo, le sumamos 2*PI para que quede en [0, 2*PI).
        if (theta_prime < 0) {
            theta_prime += 2 * M_PI;
        }

        // Calcular r(theta) = sin^a(theta) + cos^b(theta) 
        // Se necesita manejar el caso donde sin(theta) o cos(theta) son negativos si 'a' o 'b' son impares
        // y el resultado de pow podría ser negativo si la base es negativa y el exponente es impar.
        // Para este problema a=1, b=99, así que no es un problema para sin(theta) y cos(theta) cuando theta está en [0, 2pi)
        // ya que sin(theta) puede ser negativo pero la potencia es 1, y cos(theta) puede ser negativo y la potencia es 99 (impar)
        // lo que significa que el resultado puede ser negativo.
        // Sin embargo, la función r(theta) se define como una distancia, que debe ser no negativa.
        // Asumiremos que r(theta) en la ecuación es la magnitud, o que la figura está contenida en el primer y cuarto cuadrante
        // por la forma de la gráfica. La figura mostrada está en el primer y cuarto cuadrante predominantemente.
        // Para asegurar que r_figure sea siempre positivo para la comparación de radio, podemos usar std::fabs si r es una distancia.
        // La ecuación $r(\theta)=sin^{a}(\theta)+cos^{b}(\theta)$ por sí misma puede dar radios negativos si el exponente es impar y la base es negativa.
        // Sin embargo, la gráfica sugiere un radio siempre positivo. Se toma el valor absoluto para r_figure para asegurar que sea una distancia.
        double r_figure = std::pow(std::sin(theta_prime), A_POLAR) + std::pow(std::cos(theta_prime), B_POLAR);
        
        // La gráfica muestra que la figura no tiene radios negativos. Por seguridad y consistencia,
        // tomamos el valor absoluto si la interpretación es que 'r' es siempre positivo.
        // Sin embargo, si la ecuación polar permite cruzar el origen con r negativo,
        // la interpretación cambia (se grafica en la dirección opuesta al ángulo).
        // Dada la imagen, parece un radio siempre positivo en el sentido de una distancia.
        // Vamos a asumir que r_figure debe ser >= 0 para la comparación con r_prime.
        // El problema de la prueba dice r(theta), no |r(theta)|. Pero la figura lo sugiere.
        // Si la figura no se renderiza correctamente con r_figure directamente, abs podría ser la solución.
        // Para a=1, b=99, sin(theta)^1 es positivo en (0, PI) y negativo en (PI, 2PI).
        // cos(theta)^99 es positivo en (-PI/2, PI/2) y negativo en (PI/2, 3PI/2).
        // Esto hace que la suma pueda ser negativa. Por ejemplo, en theta = 3*PI/2 (0,-r), sin es -1, cos es 0. r = -1.
        // Si el problema implica que r debe ser una distancia, entonces r_figure debería ser abs(r_figure).
        // PERO, la imagen dada  muestra una figura que *no* se extiende a radios negativos en el sentido polar tradicional.
        // La forma sugiere que la ecuación describe la curva directamente, y el método Monte Carlo debe probar si un punto (x,y)
        // está "detrás" de la curva o no.

        // Una forma robusta de verificar si un punto (x,y) está "dentro" de una curva polar r=f(theta) es
        // comparar su radio r_prime con f(theta_prime).
        // Si r_prime <= f(theta_prime) y f(theta_prime) es positivo, entonces está dentro.
        // Si f(theta_prime) es negativo (lo cual es posible con exponentes impares), el punto no puede estar dentro
        // si su radio r_prime (que es siempre positivo) es comparado con un valor negativo.
        // La forma de la figura en el PDF  sugiere que r(theta) es siempre positivo.

        // Vamos a ajustar la interpretación para que r_figure siempre sea positivo para la comparación de distancia.
        // O más bien, seguir la lógica del problema: r_figure debe ser el valor de la función en ese ángulo.
        // Si r_figure es negativo, significa que el punto (r_figure * cos(theta), r_figure * sin(theta))
        // está en la dirección opuesta al ángulo theta.
        // La condición para estar "dentro" en coordenadas polares es que el radio del punto sea menor o igual al radio de la curva
        // PARA EL MISMO ÁNGULO.

        // Vamos a probar con la interpretación literal de r(theta) y si falla, usamos abs(r_figure).
        // En este caso, r_figure puede ser negativo. Un punto con r_prime > 0 no puede estar "dentro" si r_figure < 0.
        // Por la forma de la figura, si r_figure < 0, el punto no está dentro en ese ángulo.
        if (r_figure < 0) { // Si la función polar da un radio negativo, el punto no está "dentro" en esa dirección.
            return false;
        }

        return r_prime <= r_figure;
    };

    // 3. Crear el estimador usando el constructor genérico 2D
    MonteCarloEstimator problem3a_solver(
        x_min_polar, x_max_polar,
        y_min_polar, y_max_polar,
        N_SAMPLES_POLAR,
        is_inside_polar_figure, // ¡Aquí pasamos la lambda!
        "Area de figura polar: r(theta) = sin^1(theta) + cos^99(theta) (Problema 3a)"
    );
    problem3a_solver.show();
}

// Puedes añadir aquí otras funciones de solución de problemas (por ejemplo, para el Toroide si lo necesitas)
/*
void solveTorusProblem() {
    // Lógica para el toroide, similar a como estaba en tu main original
    // ...
}
*/
