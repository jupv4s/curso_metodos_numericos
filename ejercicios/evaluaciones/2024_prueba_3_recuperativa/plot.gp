# --- Configuración de salida ---
set terminal png enhanced font 'Verdana,10' size 800,800
set output 'comparacion_spline.png'

# --- Títulos y aspecto del gráfico ---
set title "Comparación de Trayectoria: Original vs. Spline"
set xlabel "Posición x"
set ylabel "Posición y"
set grid dashtype 2 linecolor rgb "#cccccc" # Rejilla punteada (compatible con gnuplot antiguo).

# Esencial: asegura que las escalas X/Y sean iguales para no distorsionar la forma.
set size ratio -1

# Mueve la leyenda afuera para no tapar los datos.
set key outside bottom center

# --- Comando para graficar las tres series de datos ---
plot 'original_segment.txt' using 2:3 with linespoints lc rgb "#3366CC" pt 7 ps 0.3 title 'Original', \
     'coarse_data.txt' using 2:3 with points lc rgb "#DC3912" pt 7 ps 1.3 title 'Muestreo', \
     'spline_interpolated.txt' using 2:3 with lines lc rgb "#FF9900" lw 2.5 title 'Spline'