#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "rk_4.h"
#include "spline.h"

using namespace std;

// Constantes del problema
const double g = -0.0001;

// Función para calcular atan2 de forma segura, evitando el caso (0,0)
double atan2_safe(double y, double x) {
    if (x == 0.0 && y == 0.0) return 0.0;
    return atan2(y, x);
}

// Define las ecuaciones diferenciales del sistema. Vector de estado s: [x, y, vx, vy]
vector<double> ecuaciones_movimiento(double t, const vector<double>& s) {
    double x = s[0], y = s[1], vx = s[2], vy = s[3];
    
    double r2 = x * x + y * y;
    if (r2 < 1e-15) return {vx, vy, 0.0, g}; // Evitar división por cero en el origen
    
    double theta = atan2_safe(y, x);
    
    // Factor de fuerza dependiente de la posición
    double cosh_arg = 50.0 * (r2 - 0.5 * sin(3.0 * theta) - 1.0);
    double force_factor = 1.0 / pow(cosh(cosh_arg), 2);
    
    // Componentes de la fuerza
    double Fx = -2500.0 * (2.0 * x + 1.5 * y * cos(3.0 * theta) / r2) * force_factor;
    double Fy = -2500.0 * (2.0 * y + 1.5 * x * cos(3.0 * theta) / r2) * force_factor + g;
    
    return {vx, vy, Fx, Fy};
}

// Calcula y guarda la energía cinética a partir de la trayectoria
void procesar_energia(const string& in_file, const string& out_file) {
    ifstream input(in_file);
    ofstream output(out_file);
    output << "# t\tEcinetica\n" << scientific << setprecision(10);
    
    string header;
    getline(input, header); // Saltar cabecera
    
    double t, x, y, vx, vy;
    while (input >> t >> x >> y >> vx >> vy) {
        output << t << "\t" << 0.5 * (vx * vx + vy * vy) << "\n";
    }
}

// Realiza submuestreo, filtrado e interpolación con splines
void procesar_submuestreo(const string& in_file, const string& out_file) {
    ifstream input(in_file);
    string header;
    getline(input, header);

    // 1. Leer todos los datos de la trayectoria
    vector<vector<double>> data(5);
    // [0] → tiempo
    // [1] → x
    // [2] → y
    // [3] → vx
    // [4] → vy
    double t, x, y, vx, vy;
    while (input >> t >> x >> y >> vx >> vy)
    {
        data[0].push_back(t); data[1].push_back(x); data[2].push_back(y);
        data[3].push_back(vx); data[4].push_back(vy);
    }

    // 2. Tomar el 1% de los puntos (submuestreo)
    int total_puntos = data[0].size();
    int puntos_sub = max(10, total_puntos / 100);
    int salto = total_puntos / puntos_sub;
    vector<vector<double>> sub_data(5);
    for (int i = 0; i < puntos_sub && i * salto < total_puntos; ++i) {
        for(int j=0; j<5; ++j) sub_data[j].push_back(data[j][i * salto]);
    }

    // 3. Conservar solo puntos con índice par (2k) para la interpolación
    vector<vector<double>> pares_data(5);
    for (int i = 0; i < sub_data[0].size(); i += 2) {
        for(int j=0; j<5; ++j) pares_data[j].push_back(sub_data[j][i]);
    }

    // 4. Crear splines y escribir resultados
    // sx: spline para x(t)
    // sy: spline para y(t)
    // svx: spline para vx(t) [no se usa en la salida]
    // svy: spline para vy(t) [no se usa en la salida]
    Spline sx(pares_data[0], pares_data[1]), sy(pares_data[0], pares_data[2]);
    Spline svx(pares_data[0], pares_data[3]), svy(pares_data[0], pares_data[4]);
    
    ofstream output(out_file);
    output << "# t\tx_orig\ty_orig\tvx_orig\tvy_orig\tx_interp\ty_interp\n";
    for (size_t i = 0; i < sub_data[0].size(); ++i) {
        double t_i = sub_data[0][i];
        output << t_i << "\t" << sub_data[1][i] << "\t" << sub_data[2][i] << "\t" << sub_data[3][i] << "\t" << sub_data[4][i]
               << "\t" << sx.evaluar(t_i) << "\t" << sy.evaluar(t_i) << "\n";
    }
}

int main() {
    // Condiciones iniciales: [x, y, vx, vy]
    vector<double> estado_inicial = {0.1, 0.0, 0.01, 0.01};
    
    // Integrar la trayectoria usando RK4 adaptativo
    rk4 integrador(ecuaciones_movimiento);

    // Parámetros de integración (puedes cambiarlos a tu gusto)
    double t_inicial = 0.0;
    double t_final = 500.0;   // cambiar a 5000.0 para una simulación más larga
    double h_inicial = 0.001;
    double tolerancia = 1e-9; // cambiar a 1e-15 para mayor precisión
    string archivo_trayectoria = "trayectoria.dat";
    integrador.integrar_adaptativo(estado_inicial, t_inicial, t_final, h_inicial, tolerancia, archivo_trayectoria);

    // Procesar los datos generados
    procesar_energia(archivo_trayectoria, "energia_cinetica.dat");
    procesar_submuestreo(archivo_trayectoria, "interpolacion.dat");

    cout << "Simulación completada. Archivos generados: " << archivo_trayectoria << ", energia_cinetica.dat, interpolacion.dat" << endl;
    
    return 0;
}