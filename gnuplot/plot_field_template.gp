# -------------------------------------------------------------------------
# plot_field_template.gp: Plantilla de Gnuplot para graficar campos vectoriales
# Uso: load "plot_field_template.gp" "archivo_campo.dat" "archivo_cargas.dat" "Titulo Grafico" "X Label" "Y Label"
# -------------------------------------------------------------------------

# Verificar argumentos
if (ARGV < 5) {
    print "Uso: load \"plot_field_template.gp\" \"<archivo_campo>\" \"<archivo_cargas>\" \"<titulo>\" \"<xlabel>\" \"<ylabel>\""
    print "Ejemplo: load \"plot_field_template.gp\" \"electric_field.dat\" \"charge_positions.dat\" \"Campo E Dipolo\" \"X\" \"Y\""
    exit
}

# Asignar argumentos
field_file = ARGV[1]      # Archivo con (x, y, Ex, Ey)
charge_file = ARGV[2]     # Archivo con (x, y) de las cargas
graph_title = ARGV[3]
x_label = ARGV[4]
y_label = ARGV[5]

# Opcional: Rango de los ejes (puedes parametrizarlos o dejarlos en autoscale)
set autoscale xy
# set xrange [-2.2:2.2]
# set yrange [-2.2:2.2]

# Configuración general
set title graph_title
set xlabel x_label
set ylabel y_label
set size square       # Aspecto cuadrado es bueno para campos
set grid
set key top right     # Posición de la leyenda
set style arrow 1 nohead lw 0.5 lc rgb "blue" # Define un estilo de flecha para el campo (sin cabeza)
set style arrow 2 head noborder lw 1 lc rgb "red" # Define un estilo de flecha para los puntos (si se quiere)

# Comando de ploteo
# Usamos 'index 0' para los vectores del campo (x, y, Ex, Ey)
# Usamos 'index 1' para los puntos de las cargas (x, y)
plot field_file index 0 using 1:2:3:4 with vectors title "Campo Eléctrico" ls 1, \
     charge_file index 1 using 1:2 with points pt 7 ps 2 title "Cargas Puntuales" lc rgb "black"

# Notas:
# - 'ls 1' usa el estilo de flecha 'arrowstyle 1' que definimos arriba.
# - Si tus cargas tienen diferentes signos y quieres colores distintos,
#   necesitarías exportar las positivas y negativas en índices separados
#   o añadir una columna a tu archivo de cargas para distinguir el tipo
#   y usar 'pointtype variable' o 'linecolor variable'.
# - Si tu archivo de cargas no tiene un 'index 1' separado (ej. está en el mismo archivo
#   'field_file' como un bloque adicional), simplemente cambia 'charge_file' a 'field_file'
#   y ajusta el índice.
