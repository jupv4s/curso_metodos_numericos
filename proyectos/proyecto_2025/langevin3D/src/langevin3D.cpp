#include "langevin3D.h"

LangevinSystem3D::LangevinSystem3D(NoiseType type, double gamma)
    : noise_type_(type),
      gamma_(gamma),
      gen_(std::random_device{}()),
      uniform_dist_(-0.1, 0.1),
      gaussian_dist_(0.0, 0.05)
{
}

std::vector<double> LangevinSystem3D::operator()(double t, const std::vector<double>& y)
{
    double eta_x, eta_y, eta_z;

    if (noise_type_ == UNIFORM)
    {
        eta_x = uniform_dist_(gen_);
        eta_y = uniform_dist_(gen_);
        eta_z = uniform_dist_(gen_);
    }
    else
    {
        eta_x = gaussian_dist_(gen_);
        eta_y = gaussian_dist_(gen_);
        eta_z = gaussian_dist_(gen_);
    }

    // y[0]=x, y[1]=vx, y[2]=y, y[3]=vy, y[4]=z, y[5]=vz
    double vx = y[1];
    double vy = y[3];
    double vz = y[5];
    double dvxdt = -gamma_ * vx + eta_x;
    double dvydt = -gamma_ * vy + eta_y;
    double dvzdt = -gamma_ * vz + eta_z;

    return {vx, dvxdt, vy, dvydt, vz, dvzdt};
}