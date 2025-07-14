SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -o main main.cpp auxiliar.cpp numerical_integration.cpp

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time gnuplot main.gp

clean:
	@rm -f main *.dat *.png
