#include "montecarlo_estimator.h"

// Constructor por defecto
MonteCarloEstimator::MonteCarloEstimator() 
    : x_min(0.0), x_max(1.0), y_min(0.0), y_max(1.0), z_min(0.0), z_max(0.0), // Init z_min/max
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
    : x_min(a), x_max(b), y_min(0.0), y_max(0.0), z_min(0.0), z_max(0.0), // Y/Z limits not strictly used for 1D func integration
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
    : x_min(x0), x_max(x1), y_min(y0), y_max(y1), z_min(0.0), z_max(0.0), // Z limits not used for 2D
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


// Predicado específico para la figura polar r(theta) = sin^a(theta) + cos^b(theta)
bool MonteCarloEstimator::isInsidePolarFigure(double x, double y) const {
    double r_prime = std::sqrt(x * x + y * y);
    if (r_prime == 0.0) {
        return true; 
    }

    double theta_prime = std::atan2(y, x);
    
    // Potencias impares pueden dar resultados negativos, lo que no tiene sentido para un radio.
    // Esto es una simplificación de cómo se trata r(theta) para valores donde sería negativo.
    // Una opción más robusta sería r_figure = std::max(0.0, ...).
    double r_figure = std::pow(std::sin(theta_prime), param_a_polar) + std::pow(std::cos(theta_prime), param_b_polar);

    if (r_figure < 0) { // Si el radio de la figura en esa dirección es negativo, el punto no puede estar dentro
        return false;
    }
    return r_prime <= r_figure;
}

// Constructor específico para el Problema 3a del PDF (figura polar)
MonteCarloEstimator::MonteCarloEstimator(int n_samples, int a_param, int b_param, const std::string& desc)
    : x_min(-1.0), x_max(1.0), y_min(-1.0), y_max(1.0), z_min(0.0), z_max(0.0), // Z limits not used for 2D
      random_engine(std::random_device{}()),
      param_a_polar(a_param), param_b_polar(b_param),
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
        
        if (isInsidePolarFigure(x_rand, y_rand)) { // Llama al predicado interno
            points_inside++;
        }
    }
    estimated_result = bounding_box_area * (static_cast<double>(points_inside) / n_samples);
}

// Predicado específico para el toroide (ecuación)
bool MonteCarloEstimator::isInsideTorus(double x, double y, double z) const {
    double eq = std::pow(R_toroide - std::sqrt(x * x + y * y), 2) + z * z;
    return eq <= std::pow(r_toroide, 2);
}

// Método genérico para estimar volumen en 3D
double MonteCarloEstimator::estimateVolume3D(Predicate3DType is_inside_func, int n_samples)
{
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras para Montecarlo debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    // Calcular el volumen del recuadro
    double bounding_box_volume = (x_max - x_min) * (y_max - y_min) * (z_max - z_min);
    if (bounding_box_volume <= 0) {
        std::cerr << "Error: El volumen del recuadro debe ser positivo." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    int points_inside = 0;
    for (int i = 0; i < n_samples; ++i) {
        double x_rand = getRandomNumber(x_min, x_max);
        double y_rand = getRandomNumber(y_min, y_max);
        double z_rand = getRandomNumber(z_min, z_max);
        
        if (is_inside_func(x_rand, y_rand, z_rand)) {
            points_inside++;
        }
    }
    return bounding_box_volume * (static_cast<double>(points_inside) / n_samples);
}

// Constructor específico para el problema del Toroide (volumen 3D)
MonteCarloEstimator::MonteCarloEstimator(double R_major, double r_minor, int n_samples, const std::string& desc)
    : R_toroide(R_major), r_toroide(r_minor),
      random_engine(std::random_device{}()),
      description(desc)
{
    if (n_samples <= 0) {
        std::cerr << "Error: El número de muestras para Montecarlo debe ser positivo." << std::endl;
        estimated_result = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    // Definir los límites del cubo que encierra el toroide
    // Basado en tu ejemplo, L = 4 * (r + R) y el cubo va de -L/2 a L/2
    double L = 2 * (r_toroide + R_toroide); // Un cubo de lado 2*(R+r) es suficiente
                                            // Si usas 4*(R+r) como en tu ejemplo, es más grande de lo necesario
                                            // Un toroide con radios R y r cabe en un cubo de lado 2*(R+r)
                                            // que va de -(R+r) a +(R+r) en X, Y, Z.
    x_min = -L; x_max = L; // Ajuste para que L sea el lado, no 2L.
    y_min = -L; y_max = L;
    z_min = -L; z_max = L;

    // Lado L = 4*(r+R) es bastante generoso, lo usaremos.
    // El centro del toroide está en el origen.
    // Un toroide con radio mayor R y radio menor r, se extiende
    // desde -(R+r) hasta +(R+r) en X e Y, y desde -r hasta +r en Z.
    // Por lo tanto, un cubo de lado 2*(R+r) sería lo mínimo necesario.
    // Si tu ejemplo usa 4*(R+r), lo respetaremos.
    L = 4*(r_toroide + R_toroide); // Usando L de tu ejemplo.
    x_min = -L/2.0; x_max = L/2.0;
    y_min = -L/2.0; y_max = L/2.0;
    z_min = -L/2.0; z_max = L/2.0;


    // Ahora calculamos el volumen usando el método genérico 3D y el predicado específico
    estimated_result = estimateVolume3D(
        [this](double x, double y, double z){ return this->isInsideTorus(x, y, z); }, // Lambda para usar isInsideTorus
        n_samples
    );
}

// Método público para mostrar el resultado y la descripción
void MonteCarloEstimator::show() const {
    if (std::isnan(estimated_result)) {
        std::cout << "No se pudo calcular la estimacion para: " << description << std::endl;
    } else {
        std::cout << "\n--- Resultado de Monte Carlo ---" << std::endl;
        std::cout << "Descripcion: " << description << std::endl;
        std::cout << "Volumen/Area/Integral estimada: " << estimated_result << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
}
