#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cfloat>
#include "rk_4.h"
#include "eqns.h"
#include "spline.h"

using namespace std;

// Genera archivo con paso fijo usando splines
void generarArchivoFijo(const vector<double>& t, const vector<double>& x, const vector<double>& vx, 
                       const vector<double>& y, const vector<double>& vy, const vector<double>& xo, 
                       const vector<double>& vox, const vector<double>& yo, const vector<double>& voy,
                       const vector<double>& phi, const vector<double>& vphi, double t0, double tf) {
    Spline sx(t, x), svx(t, vx), sy(t, y), svy(t, vy), sxo(t, xo);
    Spline svox(t, vox), syo(t, yo), svoy(t, voy), sphi(t, phi), svphi(t, vphi);
    
    ofstream out("trayectoria_final_dt0.1.txt");
    out << "# t\tx\tvx\ty\tvy\txo\tvox\tyo\tvoy\tphi\tvphi\n" << scientific << setprecision(10);
    for (double time = t0; time <= tf; time += 0.1) {
        out << time << "\t" << sx.evaluar(time) << "\t" << svx.evaluar(time) << "\t"
            << sy.evaluar(time) << "\t" << svy.evaluar(time) << "\t" << sxo.evaluar(time) << "\t"
            << svox.evaluar(time) << "\t" << syo.evaluar(time) << "\t" << svoy.evaluar(time) << "\t"
            << sphi.evaluar(time) << "\t" << svphi.evaluar(time) << "\n";
    }
    out.close();
    cout << "Archivo generado: trayectoria_final_dt0.1.txt" << endl;
}

// Análisis de colisión con splines
void analizarColision() {
    cout << "Iniciando análisis de colisión..." << endl;
    
    // Leer datos de la simulación (solo t, x, y)
    vector<double> t_all, x_all, y_all;
    double T, X, temp, Y;
    
    ifstream archivo("trayectoria_adaptativa.txt"); string header; getline(archivo, header);
    while (archivo >> T >> X >> temp >> Y) {
        t_all.push_back(T); x_all.push_back(X); y_all.push_back(Y);
        string line; getline(archivo, line); // Descartar resto de la línea
    }
    archivo.close();
    
    if (t_all.empty()) { cerr << "Error: no se leyeron datos" << endl; return; }
    cout << "Datos leídos: " << t_all.size() << endl;
    
    // Segmento de colisión [35, 37]
    vector<double> t_seg, x_seg, y_seg;
    ofstream orig("original_segment.txt"); orig << "# t\tx\ty\n";
    for (size_t i = 0; i < t_all.size(); ++i) {
        if (t_all[i] >= 35.0 && t_all[i] <= 37.0) {
            t_seg.push_back(t_all[i]); x_seg.push_back(x_all[i]); y_seg.push_back(y_all[i]);
            orig << t_all[i] << " " << x_all[i] << " " << y_all[i] << "\n";
        }
    }
    orig.close();
    
    // Datos gruesos (dt ~ 0.2)
    vector<double> t_coarse, x_coarse, y_coarse;
    double last_t = -1.0;
    ofstream coarse("coarse_data.txt"); coarse << "# t\tx\ty\n";
    for (size_t i = 0; i < t_seg.size(); ++i) {
        if (i == 0 || t_seg[i] - last_t >= 0.2) {
            t_coarse.push_back(t_seg[i]); x_coarse.push_back(x_seg[i]); y_coarse.push_back(y_seg[i]);
            coarse << t_seg[i] << " " << x_seg[i] << " " << y_seg[i] << "\n";
            last_t = t_seg[i];
        }
    }
    coarse.close();
    
    // Interpolación con splines naturales
    Spline spline_x(t_coarse, x_coarse, Spline::Natural), spline_y(t_coarse, y_coarse, Spline::Natural);
    
    ofstream interp("spline_interpolated.txt"); interp << "# t\tx_interp\ty_interp\n";
    double dt = (t_coarse.back() - t_coarse.front()) / 200.0;
    for (double time = t_coarse.front(); time <= t_coarse.back(); time += dt)
        interp << time << " " << spline_x.evaluar(time) << " " << spline_y.evaluar(time) << "\n";
    interp.close();
    
    cout << "Archivos generados:" << endl;
    cout << " - original_segment.txt: Datos de alta resolución" << endl;
    cout << " - coarse_data.txt: Puntos para interpolación" << endl;
    cout << " - spline_interpolated.txt: Trayectoria reconstruida" << endl;
}

int main()
{
    try {
        // Parte 1: Simulación de la dinámica
        cout << "Iniciando simulación de dinámica..." << endl;
        
        // Condiciones iniciales: {x, vx, y, vy, xo, vox, yo, voy, phi, vphi}
        vector<double> estado_inicial = {0.0, 0.01, 0.3, 0.0, 0.0, -0.03, 0.0, 0.0, 0.0, 0.0};
        double tiempo_inicial = 0.0;
        double tiempo_final = 1000.0;
        double paso_inicial = 1e-5;
        double tolerancia = 10.0 * DBL_EPSILON;
        string archivo_salida = "trayectoria_adaptativa.txt";
        
        // Integrar con paso adaptativo
        rk4 integrador(f);
        integrador.integrar_adaptativo(estado_inicial, tiempo_inicial, tiempo_final, paso_inicial, tolerancia, archivo_salida);
        
        // Leer datos generados por la integración
        vector<double> t, x, vx, y, vy, xo, vox, yo, voy, phi, vphi;
        double T, X, VX, Y, VY, XO, VOX, YO, VOY, PHI, VPHI;
        
        ifstream archivo("trayectoria_adaptativa.txt"); string header; getline(archivo, header);
        while (archivo >> T >> X >> VX >> Y >> VY >> XO >> VOX >> YO >> VOY >> PHI >> VPHI) {
            t.push_back(T); x.push_back(X); vx.push_back(VX); y.push_back(Y); vy.push_back(VY);
            xo.push_back(XO); vox.push_back(VOX); yo.push_back(YO); voy.push_back(VOY);
            phi.push_back(PHI); vphi.push_back(VPHI);
        }
        archivo.close();
        
        if (t.empty()) { cerr << "Error: no se leyeron datos" << endl; return 1; }
        cout << "Datos leídos: " << t.size() << endl;
        
        // Generar archivo con paso fijo
        generarArchivoFijo(t, x, vx, y, vy, xo, vox, yo, voy, phi, vphi, tiempo_inicial, tiempo_final);
        
        // Parte 2: Análisis de colisión
        analizarColision();
        
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nProceso finalizado con éxito." << endl;
    return 0;
}