#include "langevin2D.h"

LangevinSystem2D::LangevinSystem2D(NoiseType type, double gamma)
    : noise_type_(type),
      gamma_(gamma),
      gen_(std::random_device{}()), // inicializa con una semilla aleatoria
      uniform_dist_(-0.1, 0.1),
      gaussian_dist_(0.0, 0.05)
{
}

std::vector<double> LangevinSystem2D::operator()(double t, const std::vector<double>& y)
{
    double eta_x, eta_y;

    if (noise_type_ == UNIFORM)
    {
        eta_x = uniform_dist_(gen_);
        eta_y = uniform_dist_(gen_);
    }
    else
    {
        eta_x = gaussian_dist_(gen_);
        eta_y = gaussian_dist_(gen_);
    }

    // y[0]=x, y[1]=vx, y[2]=y, y[3]=vy
    double vx = y[1];
    double vy = y[3];
    double dvxdt = -gamma_ * vx + eta_x;
    double dvydt = -gamma_ * vy + eta_y;

    return {vx, dvxdt, vy, dvydt};
}