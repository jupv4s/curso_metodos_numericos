SHELL = /bin/bash

# Compilador y banderas (sin -fopenmp)
CXX = g++
CXXFLAGS = -std=c++17 -O3
LDFLAGS = -lm

# Directorios
SRCDIR = src
INCDIR = include
DATADIR = data
PLOTDIR = plots

# Lista de todos los archivos fuente
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/rk_4.cpp $(SRCDIR)/diferencias_finitas.cpp $(SRCDIR)/eqns.cpp $(SRCDIR)/metodo_de_lineas.cpp

# Ejecutable
EXECUTABLE = main

# --- Reglas ---
all:
	@echo "Compiling..."
	# Compila todo en un solo comando
	@time $(CXX) $(CXXFLAGS) -I$(INCDIR) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

run: all
	@echo "Creating output directories..."
	@mkdir -p $(DATADIR) $(PLOTDIR)
	@echo "Running..."
	@time ./$(EXECUTABLE)

plot:
	@echo "Plotting density map..."
	@time gnuplot scripts/plot_mapa.gp
	@echo "Plotting trajectory..."
	@time gnuplot scripts/plot_trayectoria.gp

clean:
	@echo "Cleaning up..."
	@rm -f $(EXECUTABLE)
	@rm -rf $(DATADIR) $(PLOTDIR)