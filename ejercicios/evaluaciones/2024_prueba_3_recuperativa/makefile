SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ main.cpp rk_4.cpp spline.cpp eqns.cpp -o main

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time gnuplot plot.gp

clean:
	@rm -f main *.txt *.dat *.png