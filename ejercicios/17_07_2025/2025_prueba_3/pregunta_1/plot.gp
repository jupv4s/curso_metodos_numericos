# Script de Gnuplot para graficar la solución del sistema de Lorenz
# Genera dos archivos: lorenz_vs_t.png y lorenz_orbita_3d.png

print "Generando gráfico 2D (variables vs tiempo)..."

# --- Gráfico 1: Variables vs Tiempo (2D) ---
#set terminal pngcairo enhanced font 'Verdana,10' size 1000,600
set terminal png enhanced size 800,600
set output 'lorenz_vs_t.png'

# Configuración del gráfico
set title "Variables del sistema de Lorenz vs tiempo"
set xlabel "t"
set ylabel "Variables"
set grid
set key top right

# Graficar x(t), y(t), z(t) desde las columnas 2, 3 y 4 vs la columna 1 (t).
# 'every ::2' salta las primeras 2 líneas del archivo, igual que skiprows=2.
plot 'lorenz.dat' every ::2 using 1:2 with lines title "x(t)", \
     ''           every ::2 using 1:3 with lines title "y(t)", \
     ''           every ::2 using 1:4 with lines title "z(t)"


print "Generando gráfico 3D (órbita)..."

# --- Gráfico 2: Órbita en el Espacio de Fases (3D) ---
set terminal png enhanced font 'Verdana,10' size 800,600
set output 'lorenz_orbita_3d.png'

# Configuración del gráfico 3D
set title "Órbita en el espacio de fases (Lorenz)"
set xlabel "x"
set ylabel "y"
set zlabel "z"
unset key # No se necesita leyenda para una sola línea

# 'splot' es el comando para gráficos 3D.
# Se usan las columnas 2(x), 3(y) y 4(z).
splot 'lorenz.dat' every ::2 using 2:3:4 with lines lw 1

print "¡Gráficos generados con éxito!"