#ifndef LANGEVIN_SYSTEM_3D_H
#define LANGEVIN_SYSTEM_3D_H

#include <vector>
#include <random>

class LangevinSystem3D
{
public:
    enum NoiseType { UNIFORM, GAUSSIAN };

private:
    NoiseType noise_type_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> uniform_dist_;
    std::normal_distribution<> gaussian_dist_;
    double gamma_;

public:
    LangevinSystem3D(NoiseType type, double gamma = 0.5);

    // y[0]=x, y[1]=vx, y[2]=y, y[3]=vy, y[4]=z, y[5]=vz
    std::vector<double> operator()(double t, const std::vector<double>& y);

    void setGamma(double gamma) { gamma_ = gamma; }
    double getGamma() const { return gamma_; }
    NoiseType getNoiseType() const { return noise_type_; }
};

#endif