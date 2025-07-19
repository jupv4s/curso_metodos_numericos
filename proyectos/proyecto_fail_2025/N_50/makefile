SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -std=c++17 -O3 main.cpp rk_4.cpp diferencias_finitas.cpp eqns.cpp metodo_de_lineas.cpp -o main -lm

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time gnuplot plot.gp

clean:
	@rm -f main *.txt *.dat *.csv *.png