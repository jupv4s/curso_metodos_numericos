# --- Configuración General ---
set terminal png enhanced font "Verdana,10" size 1200,900

# --- Gráficos para el Ruido UNIFORME ---

# Gráfico 1: Evolución temporal (Uniforme)
set output 'uniform_evolution.png'
set title "Evolución Temporal con Ruido Uniforme"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'langevin_uniforme.txt' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)'

# Gráfico 2: Trayectoria en el plano XY (Uniforme)
set output 'uniform_trajectory.png'
set title "Trayectoria de la Partícula con Ruido Uniforme"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set size square
set grid
plot 'langevin_uniforme.txt' using 2:4 with lines title 'Trayectoria (y vs x)'


# --- Gráficos para el Ruido GAUSSIANO ---

# Gráfico 3: Evolución temporal (Gaussiano)
set output 'gaussian_evolution.png'
set title "Evolución Temporal con Ruido Gaussiano"
set xlabel "Tiempo (t)"
set ylabel "Posición"
set grid
plot 'langevin_gaussiano.txt' using 1:2 with lines title 'x(t)', \
     '' using 1:4 with lines title 'y(t)'

# Gráfico 4: Trayectoria en el plano XY (Gaussiano)
set output 'gaussian_trajectory.png'
set title "Trayectoria de la Partícula con Ruido Gaussiano"
set xlabel "Posición (x)"
set ylabel "Posición (y)"
set size square
set grid
plot 'langevin_gaussiano.txt' using 2:4 with lines title 'Trayectoria (y vs x)'


# --- Finalizar ---
set terminal pop
print "Gráficos generados: uniform_evolution.png, uniform_trajectory.png, gaussian_evolution.png, gaussian_trajectory.png"