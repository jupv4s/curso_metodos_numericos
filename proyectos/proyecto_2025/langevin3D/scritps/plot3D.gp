# configuracion general
set terminal png enhanced font "Verdana,10" size 1200,900

# graficos para el ruido UNIFORME

# grafico 1: evolucion temporal (Uniforme)
set output 'plot/uniforme_evolution_3d.png'
set title "Evolución Temporal 3D con Ruido Uniforme"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'data/langevin3d_uniforme.dat' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)', \
     '' using 1:6 with lines title 'z(t)'

# grafico 2: trayectoria en el espacio XYZ (Uniforme)
set output 'plot/uniforme_trajectory_3d.png'
set title "Trayectoria 3D de la Partícula con Ruido Uniforme"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set zlabel "Posición (z)"
set grid
set view 60, 120
splot 'data/langevin3d_uniforme.dat' using 2:4:6 with lines title 'Trayectoria (x,y,z)'

# graficos para el ruido GAUSSIANO

# grafico 3: evolucion temporal (Gaussiano)
set output 'plot/gaussiano_evolution_3d.png'
set title "Evolución Temporal 3D con Ruido Gaussiano"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'data/langevin3d_gaussiano.dat' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)', \
     '' using 1:6 with lines title 'z(t)'

# grafico 4: trayectoria en el espacio XYZ (Gaussiano)
set output 'plot/gaussiano_trajectory_3d.png'
set title "Trayectoria 3D de la Partícula con Ruido Gaussiano"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set zlabel "Posición (z)"
set grid
set view 60, 120
splot 'data/langevin3d_gaussiano.dat' using 2:4:6 with lines title 'Trayectoria (x,y,z)'

set terminal pop
print "Gráficos generados: plot/uniforme_evolution_3d.png, plot/uniforme_trajectory_3d.png, plot/gaussiano_evolution_3d.png, plot/gaussiano_trajectory_3d.png"