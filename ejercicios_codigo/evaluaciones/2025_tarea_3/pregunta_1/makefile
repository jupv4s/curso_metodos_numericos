SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -o main main.cpp rk_4.cpp ilagrange.cpp spline.cpp

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time gnuplot plot.gp

clean:
	@rm -f main *.txt *.dat *.png