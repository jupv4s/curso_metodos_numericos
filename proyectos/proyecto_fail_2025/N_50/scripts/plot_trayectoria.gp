# --- Configuración del Gráfico para N=50 ---
reset
set terminal pngcairo size 800,800 enhanced font 'Verdana,10'
set output 'trayectoria_particula_final.png'

set title "Trayectoria de la Partícula (T=100 a T=104, N=50)"
set xlabel "Coordenada X"
set ylabel "Coordenada Y"
set zlabel "Coordenada Z"
set grid

# --- Lectura de Datos y Graficación ---
# Correcto, porque el archivo es .csv
set datafile separator ","

# --- ¡CAMBIO CLAVE AQUÍ! ---
# Apuntar al archivo .csv que realmente existe
splot 'particle_trajectory.csv' using 1500002:1500003:1500004 with lines title 'Trayectoria', \
      '' using 1500002:1500003:1500004 every ::0::0 with points pointtype 7 pointsize 2 linecolor rgb "green" title 'Inicio', \
      '' using 1500002:1500003:1500004 every ::-1::-1 with points pointtype 7 pointsize 2 linecolor rgb "red" title 'Final'