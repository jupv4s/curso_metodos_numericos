# --- Configuración General del Gráfico ---
reset
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set view map
set size square
set pm3d map
set palette defined ( 0 "black", 1 "purple", 2 "red", 3 "yellow" )
set cblabel "|E|"
set datafile separator "\t" # <--- Importante: Separador por tabulaciones

# --- Para la prueba con N=20 ---
set output 'data/mapa_campo_E.png'
set title "Magnitud del Campo Eléctrico |E| en T=100 (N=20)"
set dgrid3d 20,20
splot 'data/fields_T100.dat' using 1:2:6 with pm3d title ''

# --- Para la simulación final con N=50 ---
#set output 'data/mapa_campo_E_final.png'
#set title "Magnitud del Campo Eléctrico |E| en T=100 (N=50)"
#set dgrid3d 50,50
#splot 'data/fields_T100.dat' using 1:2:6 with pm3d title ''