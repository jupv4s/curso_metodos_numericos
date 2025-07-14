# Configuración inicial para el gráfico
set title "Resultados de Simulación y Funciones"
set xlabel "Eje X"
set ylabel "Eje Y"
set grid # Muestra una cuadrícula para mejor referencia

# Configuración de la leyenda
set key top left # Posición de la leyenda (arriba a la izquierda)

# Graficar los datos usando 'index' para cada bloque
# Verifica que cada 'index' corresponda al bloque de datos deseado en 'simulation_data.dat'
plot 'simulation_data.dat' index 0 using 1:2 with points title "Puntos de Vectores" pt 7 ps 1.5 lc rgb "blue", \
     '' index 1 using 1:2 with linespoints title "Matriz (Filas)" lc rgb "green", \
     '' index 2 using 1:2 with lines title "Polinomio de Prueba" lc rgb "red", \
     '' index 3 using 1:2 with lines title "Curva Senoidal Simulada" lc rgb "purple"
