reset session
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'grafico_integral.png'

set title "Evaluación Numérica de la Integral de Whittaker" font ",14"
set xlabel "Variable x"
set ylabel "Valor de la Integral I(x)"

set grid
set key top leftq

plot 'resultados.csv' skip 2 using 1:2 with lines lw 2 title 'I(x)'