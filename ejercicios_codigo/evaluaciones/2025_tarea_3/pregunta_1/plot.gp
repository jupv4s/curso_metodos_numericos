# --- Configuración General ---
# Define el tipo de terminal de salida (PNG de alta calidad) y el tamaño de la fuente.
set terminal png enhanced font "Verdana,10" size 1024,768

# --- Gráfico 1: Evolución de la posición x(t) ---
# Nombre del archivo de salida
set output 'van_der_pol_xt.png'
# Títulos del gráfico y de los ejes
set title "Oscilador de Van der Pol: Posición x(t)"
set xlabel "Tiempo (t)"
set ylabel "Posición (x)"
# Activa una grilla para mejor legibilidad
set grid
# Grafica la columna 1 (t) vs la columna 2 (x) del archivo de datos de RK4
plot 'van_der_pol_data.txt' using 1:2 with lines linewidth 2 title 'x(t) [RK4]'


# --- Gráfico 2: Espacio de Fases y(x) ---
# Nombre del archivo de salida
set output 'van_der_pol_phase_space.png'
# Títulos del gráfico y de los ejes
set title "Oscilador de Van der Pol: Espacio de Fases"
set xlabel "Posición (x)"
set ylabel "Velocidad (y)"
# Hace que la escala de los ejes sea la misma para una mejor visualización de la trayectoria
set size square
# Desactiva la leyenda ya que solo hay una línea
unset key
# Activa la grilla
set grid
# Grafica la columna 2 (x) vs la columna 3 (y)
plot 'van_der_pol_data.txt' using 2:3 with lines linewidth 2 title 'Trayectoria y(x)'


# --- Gráfico 3: Comparación de Interpolaciones ---
# Nombre del archivo de salida
set output 'interpolation_comparison.png'
# Títulos del gráfico y de los ejes
set title "Comparación de Métodos de Interpolación"
set xlabel "Tiempo (t)"
set ylabel "Posición (x)"
# Ubicación de la leyenda
set key top right
# Activa la grilla
set grid
# Grafica múltiples curvas en el mismo plano:
# 1. Los datos originales (el 90% de los puntos de RK4) como puntos.
# 2. Las tres interpolaciones (Lagrange, Spline Natural, Spline Periódico) como líneas.
plot 'interpolation_comparison.txt' using 1:2 with points pointtype 7 pointsize 0.5 title 'Datos Originales', \
     '' using 1:3 with lines linewidth 2 dashtype 2 title 'Lagrange', \
     '' using 1:4 with lines linewidth 2 title 'Spline Cúbico Natural', \
     '' using 1:5 with lines linewidth 2 dashtype 3 title 'Spline Cúbico Periódico'

# Restablece la terminal a su estado por defecto (opcional, buena práctica)
set terminal pop
# Mensaje en la terminal
print "Gráficos generados: van_der_pol_xt.png, van_der_pol_phase_space.png, interpolation_comparison.png"