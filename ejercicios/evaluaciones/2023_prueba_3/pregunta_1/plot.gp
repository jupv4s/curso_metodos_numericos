# Script de Gnuplot para comparar la interpolación de Spline

set terminal pngcairo enhanced font "Verdana,10" size 900,600
set output 'comparacion_spline_x6.png'

set title "Comparación de Spline Cúbico para x_6(t)"
set xlabel "Tiempo t"
set ylabel "x_6(t)"
set grid
set key top left

plot 'puntos_pares.txt' with points pt 7 ps 1.2 lc rgb "red" title "Puntos Pares (para construir spline)", \
     'puntos_impares.txt' with points pt 5 ps 1.2 lc rgb "blue" title "Puntos Impares (reales)", \
     'spline_recuperado.txt' with lines lw 2 lc rgb "orange" title "Curva del Spline"

print "Gráfico 'comparacion_spline_x6.png' generado."