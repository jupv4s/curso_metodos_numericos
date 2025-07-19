#ifndef PEFRL_H
#define PEFRL_H

#include <vector>
#include <string>
#include <functional>

class pefrl {
public:
    // Define un tipo para la función de fuerza, que toma la posición (q) y devuelve la fuerza/aceleración (F).
    using ForceFunction = std::function<std::vector<double>(const std::vector<double>&)>;

private:
    ForceFunction F; // Función de fuerza F(q)
    
    // Coeficientes del método PEFRL
    static constexpr double xi = 0.1786178958448091;
    static constexpr double lambda = -0.2123418310626054;
    static constexpr double chi = -0.06626458266981849;

    // Método privado para realizar un único paso de integración PEFRL.
    void pefrl_step(std::vector<double>& q, std::vector<double>& v, double h);

public:
    // Constructor que toma la función de fuerza.
    pefrl(const ForceFunction& force_func);

    // Método para integrar la trayectoria desde un tiempo inicial hasta un tiempo final.
    void integrar(std::vector<double>& q0, std::vector<double>& v0, double t0, double tf, double h, const std::string& archivo_salida);
};

#endif