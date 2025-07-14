#include "montecarlo_estimator.h"

// Constructor por defecto y Destructor han sido eliminados de aquí
// porque no tienen una declaración en el .h simplificado.
// Si deseas tener un constructor por defecto, debes declararlo explícitamente en el .h

double MonteCarloEstimator::getRandomNumber(double min_val, double max_val) {
    std::uniform_real_distribution<> dis(min_val, max_val);
    return dis(random_engine);
}

// Constructor para ÁREA (2D)
// NOTA: Los nombres de los parámetros (x0, x1, etc.) son OBLIGATORIOS aquí en la definición.
MonteCarloEstimator::MonteCarloEstimator(double x0, double x1, double y0, double y1, int N, Predicate2D is_inside_func, const std::string& desc)
    : x_min(x0), x_max(x1), y_min(y0), y_max(y1), z_min(0.0), z_max(0.0), // z_min/max se inicializan aunque no se usen
      random_engine(std::random_device{}()),
      description(desc), n_samples(N)
{
    if (x_min >= x_max || y_min >= y_max || n_samples <= 0) {
        std::cerr << "Error: Limites o muestras invalidas para Area 2D." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double bounding_box_area = (x_max - x_min) * (y_max - y_min);
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

// Constructor para VOLUMEN (3D)
// NOTA: Los nombres de los parámetros (x0, x1, etc.) son OBLIGATORIOS aquí en la definición.
MonteCarloEstimator::MonteCarloEstimator(double x0, double x1, double y0, double y1, double z0, double z1, int N, Predicate3D is_inside_func, const std::string& desc)
    : x_min(x0), x_max(x1), y_min(y0), y_max(y1), z_min(z0), z_max(z1),
      random_engine(std::random_device{}()),
      description(desc), n_samples(N)
{
    if (x_min >= x_max || y_min >= y_max || z_min >= z_max || n_samples <= 0) {
        std::cerr << "Error: Limites o muestras invalidas para Volumen 3D." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double bounding_box_volume = (x_max - x_min) * (y_max - y_min) * (z_max - z_min);
    int points_inside = 0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = getRandomNumber(x_min, x_max);
        double y_rand = getRandomNumber(y_min, y_max);
        double z_rand = getRandomNumber(z_min, z_max);
        if (is_inside_func(x_rand, y_rand, z_rand)) {
            points_inside++;
        }
    }
    estimated_result = bounding_box_volume * (static_cast<double>(points_inside) / n_samples);
}

void MonteCarloEstimator::show() const {
    if (std::isnan(estimated_result)) {
        std::cout << "No se pudo calcular la estimacion para: " << description << std::endl;
    } else {
        std::cout << "\n--- Resultado de Monte Carlo ---" << std::endl;
        std::cout << "Descripcion: " << description << std::endl;
        std::cout << "Numero de muestras: " << n_samples << std::endl;
        std::cout << "Estimacion: " << estimated_result << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
}
