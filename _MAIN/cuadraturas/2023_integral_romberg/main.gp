# Configuración de la terminal de salida
set terminal pngcairo enhanced font 'Arial,12'
set output 'g_x_cuadratura.png'

# Título y etiquetas de los ejes
set title 'Función g(x) calculada por Cuadratura de Romberg'
set xlabel 'x'
set ylabel 'g(x)'

# Configuración de la cuadrícula
set grid

# Configuración de la leyenda (opcional, pero buena práctica)
set key autotitle columnhead # Usa la primera fila del archivo como título de columna para la leyenda

# Configuración de los rangos de los ejes (opcional, si quieres un control específico)
# set xrange [0:10]
# set yrange [-0.5:1.1]

# Cargar los datos y graficar
# 'cuadratura.dat' es tu archivo de datos
# using 1:2 indica usar la primera columna para x y la segunda para y
# with linespoints dibuja líneas y puntos. Puedes usar 'with lines' para solo líneas.
# lt 1 lc rgb "blue" establece el tipo de línea (1), color (azul)
# pt 7 establece el tipo de punto
plot 'cuadratura.dat' using 1:2 with linespoints title 'g(x)' lt 1 lc rgb "blue" pt 7
