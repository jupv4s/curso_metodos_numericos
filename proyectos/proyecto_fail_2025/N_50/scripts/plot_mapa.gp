# --- Configuración del Gráfico ---
reset
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'mapa_campo_E_final.png'

set title "Magnitud del Campo Eléctrico |E| en T=100 (N=50)"
set xlabel "Coordenada X"
set ylabel "Coordenada Y"

# ... (otras configuraciones)
set pm3d map
set palette defined ( 0 "black", 1 "purple", 2 "red", 3 "yellow" )
set cblabel "|E|"
set view map
set size square

# --- ¡LÍNEA CLAVE AQUÍ! ---
# Especificar que el separador de columnas es una tabulación.
#set datafile separator "\t"
set datafile separator ","

# Rejilla de 50x50 para la simulación final
set dgrid3d 50,50

# Apuntar al archivo .dat correcto
splot 'fields_T100.csv' using 1:2:6 with pm3d title ''