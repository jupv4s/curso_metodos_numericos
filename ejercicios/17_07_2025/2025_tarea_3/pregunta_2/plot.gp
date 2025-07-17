# --- Configuración General ---
# Define el tipo de terminal de salida (PNG de alta calidad) y el tamaño de la fuente.
set terminal png enhanced font "Verdana,10" size 1024,768

# --- Gráfico 1: Comparación de Soluciones ---
set output 'solutions_comparison.png'
# Títulos del gráfico y de los ejes
set title "Comparación de Métodos Numéricos vs Solución Analítica"
set xlabel "Tiempo (t)"
set ylabel "y(t)"
# Activa una grilla y la leyenda
set grid
set key top right

# Grafica las tres curvas: la analítica como una línea gruesa y las numéricas
# como líneas con puntos para poder distinguirlas claramente.
plot 'ode_comparison.txt' using 1:4 with lines linewidth 3 title 'Solución Analítica', \
     '' using 1:2 with linespoints pt 7 ps 0.5 title 'Método de Euler', \
     '' using 1:3 with linespoints pt 5 ps 0.5 title 'Predictor-Corrector'


# --- Gráfico 2: Comparación de Errores Absolutos ---
set output 'error_comparison.png'
# Títulos del gráfico y de los ejes
set title "Error Absoluto de los Métodos Numéricos"
set xlabel "Tiempo (t)"
set ylabel "Error Absoluto |y_{exacta} - y_{numérico}|"

# Usamos escala logarítmica en el eje Y para apreciar mejor la diferencia
# en órdenes de magnitud entre los errores de ambos métodos.
set logscale y
set grid
set key top right

# Grafica el error absoluto de cada método en función del tiempo.
plot 'ode_comparison.txt' using 1:5 with linespoints title 'Error de Euler', \
     '' using 1:6 with linespoints title 'Error de Predictor-Corrector'

# --- Finalizar ---
# Restablece la terminal a su estado por defecto
set terminal pop
print "Gráficos generados: solutions_comparison.png, error_comparison.png"