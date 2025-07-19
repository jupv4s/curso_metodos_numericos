#ifndef DIFERENCIAS_FINITAS_H
#define DIFERENCIAS_FINITAS_H

#include <vector>

class DiferenciasFinitas
{
public:
    DiferenciasFinitas(int N, double L);
    void apply(const std::vector<double>& u, std::vector<double>& Lu) const;

private:
    const int N;
    const double h;
    const double h2_inv;
    int index(int i, int j, int k) const;
};

#endif