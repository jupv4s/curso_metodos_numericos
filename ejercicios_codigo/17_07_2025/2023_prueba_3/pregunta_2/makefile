SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -o main main.cpp pefrl.cpp

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time python3 plot.py

clean:
	@rm -f main *.txt *.dat *.png