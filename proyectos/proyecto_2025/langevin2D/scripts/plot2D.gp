# configuracion general
set terminal png enhanced font "Verdana,10" size 1200,900

# graficos para el ruido UNIFORME

# grafico 1: evolucion temporal (Uniforme)
set output 'plot/uniform_evolution.png'
set title "Evolución Temporal con Ruido Uniforme"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'data/langevin2d_uniforme.dat' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)'

# grafico 2: trayectoria en el plano XY (Uniforme)
set output 'plot/uniform_trajectory.png'
set title "Trayectoria de la Partícula con Ruido Uniforme"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set size square
set grid
plot 'data/langevin2d_uniforme.dat' using 2:4 with lines title 'Trayectoria (y vs x)'


# graficos para el ruido GAUSSIANO

# grafico 3: evolucion temporal (Gaussiano)
set output 'plot/gaussian_evolution.png'
set title "Evolución Temporal con Ruido Gaussiano"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'data/langevin2d_gaussiano.dat' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)'

# grafico 4: trayectoria en el plano XY (Gaussiano)
set output 'plot/gaussian_trajectory.png'
set title "Trayectoria de la Partícula con Ruido Gaussiano"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set size square
set grid
plot 'data/langevin2d_gaussiano.dat' using 2:4 with lines title 'Trayectoria (y vs x)'

set terminal pop
print "Gráficos generados: plot/uniform_evolution.png, plot/uniform_trajectory.png, plot/gaussian_evolution.png, plot/gaussian_trajectory.png"