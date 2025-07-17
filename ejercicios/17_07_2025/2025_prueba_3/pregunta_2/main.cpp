#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>
#include <sstream>
#include "rk_4.h"

using namespace std;

// parametros del recinto y particula
const double Tmax = 100.0; // tiempo final de la simulación
const double dt_max = 1e-6; // paso de tiempo máximo
const double tol = 3e-16;   // tolerancia para el error

// ecuaciones de movimiento para la particula y el recinto rotante
// y = [x, vx, y, vy, theta, omega]
vector<double> sistema(double t, const vector<double>& y)
{
    // parametros fisicos, con |I| = |m| = 1
    double m = 1.0; // masa de la particula
    double I = 1.0; // momento de inercia del recinto
    const double pi = 3.14159265358979323846;
    // epsilon para evitar la singularidad cuando X=0
    const double epsilon = 1e-12;

    // variables (x, vx, y, vy, theta, omega)
    double xp = y[0];      // x
    double vxp = y[1];     // vx
    double yp = y[2];      // y
    double vyp = y[3];     // vy
    double tho = y[4];     // theta
    double omega = y[5];   // omega

    // expresiones auxiliares
    double X = xp * cos(tho) - yp * sin(tho);
    double Y = yp * cos(tho) + xp * sin(tho);
    double absX = abs(X);

    // agregamos epsilon para evitar la division por cero
    double powX_0_333 = pow(absX + epsilon, 1.0 / 3.0);
    double powX_0_666 = pow(absX + epsilon, 2.0 / 3.0);
    
    double powY_2 = pow(Y, 2);
    double powX_2 = pow(X, 2);

    // implementacion explicita de la funcion signo para mayor claridad
    double sign_X = (X > 0.0) ? 1.0 : -1.0;

    // denominadores
    double den1 = 1.0 + 1e12 * pow(0.9 + powX_0_666 * Y - powY_2 - powX_2, 2);
    double den2 = 1.0 + 1e12 * pow(1.1 + powX_0_666 * Y - powY_2 - powX_2, 2);

    // fuerzas
    double fx1_num = powX_0_666 * sin(tho) - 2.0 * sin(tho) * Y - 2.0 * cos(tho) * X + (2.0 * cos(tho) * Y * sign_X) / (3.0 * powX_0_333);
    double fx1 = -1000000.0 * fx1_num;
    double fx2 =  1000000.0 * fx1_num;
    // fuerza en x sobre la particula
    double f_x = -10.0 * (fx1 / den1 + fx2 / den2) / pi;

    double fy1_num = powX_0_666 * cos(tho) - 2.0 * cos(tho) * Y + 2.0 * sin(tho) * X - (2.0 * sin(tho) * Y * sign_X) / (3.0 * powX_0_333);
    double fy1 = -1000000.0 * fy1_num;
    double fy2 =  1000000.0 * fy1_num;
    // fuerza en y sobre la particula
    double f_y = -10.0 * (fy1 / den1 + fy2 / den2) / pi;
    
    double ft1_num = powX_0_666 * X - 2.0 * (-Y) * X - 2.0 * Y * X + (2.0 * (-Y) * Y * sign_X) / (3.0 * powX_0_333);
    double ft1 = -1000000.0 * ft1_num;
    double ft2 =  1000000.0 * ft1_num;
    // torque sobre el recinto
    double f_theta = -10.0 * (ft1 / den1 + ft2 / den2) / pi;

    // ecuaciones de movimiento (x, vx, y, vy, theta, omega)
    vector<double> dydt(6);
    dydt[0] = vxp;          // dx/dt = vx
    dydt[1] = f_x / m;      // dvx/dt = fx/m con m=1
    dydt[2] = vyp;          // dy/dt = vy
    dydt[3] = f_y / m;      // dvy/dt = fy/m con m=1
    dydt[4] = omega;        // dtheta/dt = omega
    dydt[5] = f_theta / I;  // domega/dt = torque/I con I=1
    return dydt;
}

// leemos el ultimo estado disponible del archivo de trayectoria
void leer_ultimo_estado(const string& archivo, double& t, double& x, double& y, double& theta)
{
    ifstream fin(archivo);
    if (!fin) // comprobamos si el archivo se abrio correctamente
    {
        cerr << "Error: No se pudo abrir el archivo de resultados para leer el estado final." << endl;
        return;
    }
    string encabezado;
    getline(fin, encabezado);
    
    double t_, x_, vx_, y_, vy_, theta_, omega_;
    // inicializamos variables por si el archivo está vacío
    t = 0.0; x = 0.0; y = 0.0; theta = 0.0;

    // leemos la ultima linea del archivo
    string ultima_linea;
    string linea_actual;
    while (getline(fin, linea_actual))
    {
        if (!linea_actual.empty())
        {
            ultima_linea = linea_actual;
        }
    }

    if (!ultima_linea.empty())
    {
        stringstream ss(ultima_linea);
        ss >> t_ >> x_ >> vx_ >> y_ >> vy_ >> theta_ >> omega_;
        t = t_;
        x = x_;
        y = y_;
        theta = theta_;
    }

    fin.close();
}

int main()
{
    // integramos el sistema y guardamos los resultados en trayectoria_corazon.dat
    // condiciones iniciales: [x, vx, y, vy, theta, omega]
    vector<double> condiciones_iniciales = {0.2, 0.01, -0.1, 0.0, 0.0, 0.0};
    double t_inicial = 0.0;
    double t_final = Tmax;
    double paso_inicial = dt_max;
    double tolerancia = tol;
    string archivo_salida = "trayectoria_corazon.dat";

    rk4 integrador(sistema);
    integrador.integrar_adaptativo(condiciones_iniciales, t_inicial, t_final, paso_inicial, tolerancia, archivo_salida);
    cout << "Simulación completada. Resultados guardados en " << archivo_salida << endl;

    // leemos el ultimo estado disponible usando la funcion
    double t_ultimo, x_final, y_final, theta_final;
    leer_ultimo_estado(archivo_salida, t_ultimo, x_final, y_final, theta_final);
    cout << "\nEstado final registrado:\n";
    cout << "t = " << t_ultimo << "\nx = " << x_final << "\ny = " << y_final << "\ntheta = " << theta_final << endl;
    
    return 0;
}