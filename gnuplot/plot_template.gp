# -----------------------------------------------------------
# plot_template.gp: Plantilla de Gnuplot para graficar datos X, Y
# Uso: load "plot_template.gp" "nombre_archivo.dat" "Titulo del Grafico" "Etiqueta Eje X" "Etiqueta Eje Y"
# -----------------------------------------------------------

# Verificar si se pasaron los argumentos necesarios
if (ARGV < 4) {
    print "Uso: load \"plot_template.gp\" \"<archivo_datos>\" \"<titulo>\" \"<xlabel>\" \"<ylabel>\""
    print "Ejemplo: load \"plot_template.gp\" \"datos.dat\" \"Mi Funcion\" \"Tiempo (s)\" \"Amplitud (m)\""
    exit
}

# Asignar los argumentos a variables
data_file = ARGV[1]
graph_title = ARGV[2]
x_label = ARGV[3]
y_label = ARGV[4]
plot_style = "lines" # Estilo por defecto, se puede cambiar
line_color = "blue"  # Color por defecto

# Configuración general del gráfico
set title graph_title
set xlabel x_label
set ylabel y_label
set grid # Cuadrícula para mejor lectura
set key top left # Posición de la leyenda

# Rango de los ejes (opcional, si quieres que sea fijo o auto)
# set autoscale xy # Esto es lo predeterminado, se ajusta a los datos
# set xrange [0:10]
# set yrange [-1:1]

# Configuración del tipo de terminal (ventana de visualización)
set terminal wxt size 800,600 enhanced font "Arial,10" # Para ventana interactiva
# set terminal pngcairo size 800,600 enhanced font "Arial,10" # Para exportar a PNG
# set output "output_graph.png" # Si se usa terminal PNG

# Comando de ploteo principal
# Usa la primera y segunda columna por defecto
plot data_file using 1:2 with @plot_style title data_file linecolor rgb @line_color

# Nota: Si necesitas más flexibilidad (ej. múltiples series, índices),
# necesitarías más argumentos o una plantilla más compleja.

# Opcional: Pausar si se ejecuta directamente y no como parte de un script más grande
# pause -1 "Presiona Enter para continuar..."
