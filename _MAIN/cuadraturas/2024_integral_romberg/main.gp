# Configuración de la terminal de salida
set terminal pngcairo enhanced font 'Arial,12'
set output 'beta_functions_3d.png'

# Título y etiquetas de los ejes
set title 'Comparación de Funciones Beta F(x,y) y G(x,y) en 3D'
set xlabel 'x(s)'
set ylabel 'y(s)'
set zlabel 'Valor de la Función'

# Configuración del estilo de la cuadrícula
set ticslevel 0
set border 4095
set grid x y z

# Rotación de la vista para una mejor perspectiva
# Puedes ajustar estos valores (rot_x, rot_z) para ver el gráfico desde diferentes ángulos
set view 60, 30, 1, 1 # view <rot_x>, <rot_z>, <scale>, <scale_z>

# Configuración de la leyenda
set key autotitle columnhead top right

# Plotting en 3D (splot)
# Usaremos las columnas (x_s, y_s, F_val) y (x_s, y_s, G_val)
# La columna 's' es tu índice de tiempo, pero para la visualización en el plano X-Y,
# es más natural ver F y G como "alturas" sobre el círculo (x(s), y(s)).

splot 'beta_functions.dat' using 2:3:4 with linespoints title 'F(x(s),y(s))' lc rgb "blue" lw 2 pt 7 ps 0.5, \
      'beta_functions.dat' using 2:3:5 with linespoints title 'G(x(s),y(s))' lc rgb "red" lw 2 pt 7 ps 0.5
