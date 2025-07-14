# 1. Configuración del archivo de salida (imagen PNG)
reset session
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'grafico_integral.png'

# 2. Títulos y etiquetas del gráfico
set title "Evaluación Numérica de la Integral de Whittaker" font ",14"
set xlabel "Variable x"
set ylabel "Valor de la Integral I(x)"

# 3. Estilo y otras configuraciones
set grid             # Añade una rejilla de fondo
set key top left     # Mueve la leyenda a la esquina superior izquierda

# 4. El comando para graficar
#    'resultados.csv' -> El archivo de datos.
#    skip 2           -> Ignora las primeras 2 líneas (cabecera y separador).
#    using 1:2        -> Usa la columna 1 para el eje X y la 2 para el eje Y.
#    with lines       -> Dibuja el gráfico con líneas suaves.
#    lw 2             -> Line Width (ancho de línea) de 2 para que sea más visible.
#    title 'I(x)'     -> Título de la curva en la leyenda.
plot 'resultados.csv' skip 2 using 1:2 with lines lw 2 title 'I(x)'