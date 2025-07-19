#ifndef LANGEVIN_SYSTEM_2D_H
#define LANGEVIN_SYSTEM_2D_H

#include <vector>
#include <random>

class LangevinSystem2D
{
public:
    enum NoiseType { UNIFORM, GAUSSIAN };

private:
    NoiseType noise_type_;
    std::mt19937 gen_; // cada sistema tiene su propio generador de numeros aleatorios
    std::uniform_real_distribution<> uniform_dist_;
    std::normal_distribution<> gaussian_dist_;
    double gamma_; // coeficiente de friccion

public:
    // constructor que inicializa el tipo de ruido y los generadores
    LangevinSystem2D(NoiseType type, double gamma = 0.5);
    
    // operator() permite que los objetos de esta clase sean llamados como si fueran funciones
    std::vector<double> operator()(double t, const std::vector<double>& y);
    
    // metodos para acceder/modificar parametros
    void setGamma(double gamma) { gamma_ = gamma; }
    double getGamma() const { return gamma_; }
    NoiseType getNoiseType() const { return noise_type_; }
};

#endif