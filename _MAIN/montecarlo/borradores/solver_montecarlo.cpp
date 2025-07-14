#include "solver_montecarlo.h"

// Constructor por defecto
MonteCarloEstimator::MonteCarloEstimator() 
    : x_min(0.0), x_max(1.0), y_min(0.0), y_max(1.0), 
      random_engine(std::random_device{}()), 
      estimated_result(std::numeric_limits<double>::quiet_NaN()), 
      description("Estimador por defecto") {}

// Destructor
MonteCarloEstimator::~MonteCarloEstimator() {}

// Genera un número aleatorio uniforme dentro de un rango
double MonteCarloEstimator::getRandomNumber(double min_val, double max_val) {
    std::uniform_real_distribution<> dis(min_val, max_val);
    return dis(random_engine);
}

// Constructor genérico para integración 1D
MonteCarloEstimator::MonteCarloEstimator(double a, double b, int n_samples, Function1DType f, const std::string& desc)
    : x_min(a), x_max(b), y_min(0.0), y_max(0.0), // Y limits not strictly used for 1D func integration
      random_engine(std::random_device{}()),
      description(desc)
{
    if (x_min >= x_max) {
        std::cerr << "Advertencia: x_min debe ser menor que x_max. Intercambiando valores." << std::endl;
        std::swap(x_min, x_max);
    }
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras para Montecarlo debe ser positivo." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double sum_fx = 0.0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = getRandomNumber(x_min, x_max);
        sum_fx += f(x_rand);
    }
    estimated_result = (x_max - x_min) * (sum_fx / n_samples);
}

// Constructor genérico para estimación de área 2D
MonteCarloEstimator::MonteCarloEstimator(double x0, double x1, double y0, double y1, int n_samples, Predicate2DType is_inside_func, const std::string& desc)
    : x_min(x0), x_max(x1), y_min(y0), y_max(y1), 
      random_engine(std::random_device{}()),
      description(desc)
{
    if (x_min >= x_max || y_min >= y_max) {
        std::cerr << "Advertencia: Los límites del recuadro no son válidos." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras para Montecarlo debe ser positivo." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double bounding_box_area = (x_max - x_min) * (y_max - y_min);
    if (bounding_box_area <= 0) {
        std::cerr << "Error: El área del recuadro debe ser positiva." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    int points_inside = 0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = getRandomNumber(x_min, x_max);
        double y_rand = getRandomNumber(y_min, y_max);
        
        if (is_inside_func(x_rand, y_rand)) {
            points_inside++;
        }
    }
    estimated_result = bounding_box_area * (static_cast<double>(points_inside) / n_samples);
}

//------------------------------------------------------------------------------------------

// Predicado específico para la figura polar r(theta) = sin^a(theta) + cos^b(theta)
bool MonteCarloEstimator::isInsidePolarFigure(double x, double y) const {
    double r_prime = std::sqrt(x * x + y * y);
    // Para evitar atan2(0,0) si el punto es el origen
    if (r_prime == 0.0) {
        return true; // El origen siempre está dentro si r(theta) >= 0
    }

    double theta_prime = std::atan2(y, x);

    // Asegurarse de que theta_prime esté en [0, 2*PI) si es necesario, aunque atan2 da en [-PI, PI]
    // La función r(theta) con sen^a y cos^b suele tener simetría o no cambia mucho al sumar 2*PI
    // Sin embargo, para potencias impares de cos(theta) puede ser importante.
    // atan2(y,x) da un resultado en [-PI, PI].
    // Si la función r(theta) se comporta igual para theta y theta+2PI, no es un problema.
    // Para sin(theta) y cos(theta), sí se repite cada 2PI.
    // Sin embargo, para asegurar, podemos normalizar theta_prime si r(theta) fuera muy complejo.
    // Por ejemplo: if (theta_prime < 0) theta_prime += 2 * M_PI;

    // Calcular el r(theta) de la figura para ese ángulo
    double r_figure = std::pow(std::sin(theta_prime), param_a) + std::pow(std::cos(theta_prime), param_b);

    // Si r_figure es negativo, significa que en esa dirección no hay figura,
    // o que el radio debería ser 0. Para una figura polar, el radio es siempre >= 0.
    // Si la potencia b es impar, cos^b(theta) puede ser negativo.
    // Si r_figure calculado es negativo, el punto no puede estar dentro en esa dirección.
    // Si r_figure es muy pequeño y negativo, podría ser un artefacto de la función.
    // Para el contexto del problema, asumimos que r(theta) debe ser >= 0
    if (r_figure < 0) {
        return false; // El punto no está dentro si el radio de la figura es negativo en esa dirección.
                      // O bien, podríamos interpretarlo como 0 en esa dirección.
    }

    return r_prime <= r_figure;
}


// Constructor específico para el Problema 3 del PDF (figura polar)
MonteCarloEstimator::MonteCarloEstimator(int n_samples, int a_param, int b_param, const std::string& desc)
    : x_min(-1.0), x_max(1.0), y_min(-1.0), y_max(1.0), // Recuadro para la figura polar, basado en la imagen y r_max=1
      random_engine(std::random_device{}()),
      param_a(a_param), param_b(b_param),
      description(desc)
{
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras para Montecarlo debe ser positivo." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double bounding_box_area = (x_max - x_min) * (y_max - y_min); // Area del cuadrado 2x2 = 4
    if (bounding_box_area <= 0) {
        std::cerr << "Error: El área del recuadro debe ser positiva." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    int points_inside = 0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = getRandomNumber(x_min, x_max);
        double y_rand = getRandomNumber(y_min, y_max);
        
        // Usar el predicado interno que conoce los parámetros 'a' y 'b'
        if (isInsidePolarFigure(x_rand, y_rand)) {
            points_inside++;
        }
    }
    estimated_result = bounding_box_area * (static_cast<double>(points_inside) / n_samples);
}

// Método público para mostrar el resultado y la descripción
void MonteCarloEstimator::show() const {
    if (std::isnan(estimated_result)) {
        std::cout << "No se pudo calcular la estimacion para: " << description << std::endl;
    } else {
        std::cout << "\n--- Resultado de MonteCarlo ---" << std::endl;
        std::cout << "Descripcion: " << description << std::endl;
        std::cout << "Area/Integral estimada: " << estimated_result << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
}
