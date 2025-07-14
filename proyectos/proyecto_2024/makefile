# Archivo : makefile
# Autor   : Julio Pérez V.
# Email   : julio.perez.v@ug.uchile.cl
# Fecha   : 17 de julio de 2024

SHELL = /bin/bash

all:
	@echo "Compiling..."
	@time g++ -o main main.cpp dependencias/ajuste.h

run:
	@echo "Running..."
	@time ./main

plot:
	@echo "Plotting..."
	@time python3 plot.py

clean:
	@rm -v main *.pdf