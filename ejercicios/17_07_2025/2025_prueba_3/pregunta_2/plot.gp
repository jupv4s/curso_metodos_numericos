# Script para graficar la trayectoria y variables de estado desde un archivo.
# Genera 5 archivos .png diferentes.

# --- Definiciones Comunes ---
# Define el nombre del archivo de datos para no repetirlo.
datafile = 'trayectoria_corazon.dat'

# --- 1. Trayectoria en el plano xy (x vs y) ---
print "Generando: trayectoria_xy.png"
set terminal png enhanced font "Verdana,10" size 600,600
set output 'trayectoria_xy.png'

set title 'Trayectoria de la partícula'
set xlabel 'x'
set ylabel 'y'
set grid
# 'set size ratio -1' es el equivalente a 'plt.axis("equal")'
set size ratio -1
plot datafile every ::1 using 2:4 with lines notitle


# --- 2. Posición x vs tiempo (x vs t) ---
print "Generando: x_vs_t.png"
set terminal png enhanced font "Verdana,10" size 800,600
set output 'x_vs_t.png'

set title 'x vs t'
set xlabel 't'
set ylabel 'x'
set grid
# 'set size noratio' devuelve el aspect ratio a su valor por defecto.
set size noratio
plot datafile every ::1 using 1:2 with lines notitle


# --- 3. Posición y vs tiempo (y vs t) ---
print "Generando: y_vs_t.png"
set terminal png enhanced font "Verdana,10" size 800,600
set output 'y_vs_t.png'

set title 'y vs t'
set xlabel 't'
set ylabel 'y'
set grid
plot datafile every ::1 using 1:4 with lines notitle


# --- 4. Ángulo theta vs tiempo (theta vs t) ---
print "Generando: theta_vs_t.png"
set terminal png enhanced font "Verdana,10" size 800,600
set output 'theta_vs_t.png'

set title 'theta vs t'
set xlabel 't'
set ylabel 'theta'
set grid
plot datafile every ::1 using 1:6 with lines notitle


# --- 5. Velocidad angular omega vs tiempo (omega vs t) ---
print "Generando: omega_vs_t.png"
set terminal png enhanced font "Verdana,10" size 800,600
set output 'omega_vs_t.png'

set title 'omega vs t'
set xlabel 't'
set ylabel 'omega'
set grid
plot datafile every ::1 using 1:7 with lines notitle


print "\nProceso completado. Se generaron 5 gráficos."