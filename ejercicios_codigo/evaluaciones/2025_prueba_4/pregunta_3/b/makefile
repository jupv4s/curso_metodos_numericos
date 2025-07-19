SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -o main main.cpp montecarlo.cpp eqns_cat.cpp

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time gnuplot plot.gp

clean:
	@rm -f main *.csv *.png