# --- Configuración General del Gráfico ---
reset
set terminal pngcairo size 800,800 enhanced font 'Verdana,10'
set xlabel "Coordenada X"
set ylabel "Coordenada Y"
set zlabel "Coordenada Z"
set grid
set datafile separator "\t" # <--- Importante: Separador por tabulaciones

# --- Para la prueba con N=20 (Columnas 96002, 96003, 96004) ---
set output 'data/trayectoria_particula.png'
set title "Trayectoria de la Partícula (T=100 a T=104, N=20)"
splot 'data/particle_trajectory.dat' using 96002:96003:96004 with lines title 'Trayectoria', \
       '' using 96002:96003:96004 every ::0::0 with points pointtype 7 pointsize 2 linecolor rgb "green" title 'Inicio', \
       '' using 96002:96003:96004 every ::-1::-1 with points pointtype 7 pointsize 2 linecolor rgb "red" title 'Final'

# --- Para la simulación final con N=50 (Columnas 1500002, 1500003, 1500004) ---
#set output 'data/trayectoria_particula_final.png'
#set title "Trayectoria de la Partícula (T=100 a T=104, N=50)"
#splot 'data/particle_trajectory.dat' using 1500002:1500003:1500004 with lines title 'Trayectoria', \
#      '' using 1500002:1500003:1500004 every ::0::0 with points pointtype 7 pointsize 2 linecolor rgb "green" title 'Inicio', \
#      '' using 1500002:1500003:1500004 every ::-1::-1 with points pointtype 7 pointsize 2 linecolor rgb "red" title 'Final'