#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Incluimos las cabeceras de las clases que vamos a utilizar.
#include "rk_4.h"
#include "spline.h"
#include "ilagrange.h"

// Definición del sistema de EDOs de Van der Pol modificado.
// El vector de estado es y_vec = {x, y}
vector<double> van_der_pol(double t, const vector<double>& y_vec) {
    // Parámetros del problema
    const double mu = 3.0;
    const double A = 0.05;
    const double omega = 1.0;

    // Descomponemos el vector de estado para mayor claridad
    double x = y_vec[0];
    double y = y_vec[1];

    // Calculamos las derivadas
    double dxdt = y;
    double dydt = mu * (1.0 - x * x) * y - x + A * cos(omega * t);

    // Retornamos el vector de derivadas {dx/dt, dy/dt}
    return {dxdt, dydt};
};

// Función para cargar los datos generados por RK4 desde un archivo.
void cargar_datos(const string& archivo, vector<double>& t, vector<double>& x, vector<double>& y) {
    ifstream data(archivo);
    string header;
    getline(data, header); // Ignorar la cabecera

    double t_val, x_val, y_val;
    while (data >> t_val >> x_val >> y_val) {
        t.push_back(t_val);
        x.push_back(x_val);
        y.push_back(y_val);
    }
    data.close();
}


int main() {
    cout << "Resolviendo Problema 1: Oscilador de Van der Pol" << endl;

    // === Parte 1: Integración con Runge-Kutta 4 ===
    cout << "Paso 1: Integrando el sistema con RK4 de paso fijo..." << endl;

    // Condiciones iniciales y parámetros de integración
    const vector<double> y0 = {1.44043, -0.394709};
    const double t0 = 0.0;
    const double tf = 8.94963;
    const double h = 0.01;
    const string rk4_output_file = "van_der_pol_data.txt";

    // Creamos el objeto solver y resolvemos el sistema.
    rk4 solver(van_der_pol);
    solver.integrar_paso_fijo(y0, t0, tf, h, rk4_output_file);

    cout << "-> Datos de RK4 guardados en '" << rk4_output_file << "'." << endl;
    cout << "-> Para graficar x(t) use la columna 1 vs 2." << endl;
    cout << "-> Para graficar el espacio de fases y(x) use la columna 2 vs 3." << endl;

    // === Parte 2: Interpolación ===
    cout << "\nPaso 2: Realizando interpolaciones..." << endl;

    // Cargamos los datos que acabamos de generar.
    vector<double> t_vals, x_vals, y_vals_rk4;
    cargar_datos(rk4_output_file, t_vals, x_vals, y_vals_rk4);

    // Descartamos el último 10% de los datos.
    int total_points = t_vals.size();
    int points_to_keep = static_cast<int>(total_points * 0.90);
    
    t_vals.resize(points_to_keep);
    x_vals.resize(points_to_keep);
    
    cout << "-> Se descarta el 10% de los datos. Se conservan " << points_to_keep << " de " << total_points << " puntos." << endl;

    // Creamos los objetos de interpolación para x(t).
    Lagrange lagrange_interp(t_vals, x_vals);
    Spline spline_natural(t_vals, x_vals, Spline::Natural);
    Spline spline_periodico(t_vals, x_vals, Spline::Periodico);
    
    // Generamos un archivo para comparar las interpolaciones.
    const string interp_output_file = "interpolation_comparison.txt";
    ofstream out(interp_output_file);
    out << scientific << setprecision(10);
    out << "# t_original\tx_original\tx_lagrange\tx_spline_nat\tx_spline_per\n";

    // Evaluamos cada interpolador en los mismos puntos de tiempo originales para comparar.
    for (size_t i = 0; i < t_vals.size(); ++i) {
        double t_actual = t_vals[i];
        
        out << t_actual << "\t"
            << x_vals[i] << "\t"
            << lagrange_interp.evaluar(t_actual) << "\t"
            << spline_natural.evaluar(t_actual) << "\t"
            << spline_periodico.evaluar(t_actual) << "\n";
    }
    out.close();

    cout << "-> Datos para la comparación de interpolaciones guardados en '" << interp_output_file << "'." << endl;
    cout << "-> Puede usar un programa como Gnuplot para graficar los resultados." << endl;
    
    cout << "\nProceso completado." << endl;

    return 0;
}