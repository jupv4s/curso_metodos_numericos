SHELL = /bin/bash

SRC_DIR = src
BUILD_DIR = build
PLOTS_DIR = plots
DATA_DIR = data

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/rk_4.cpp $(SRC_DIR)/langevin3D.cpp
OBJS = $(BUILD_DIR)/main

all: $(BUILD_DIR) $(DATA_DIR) plot_dir $(OBJS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(DATA_DIR):
	@mkdir -p $(DATA_DIR)

plot_dir:
	@mkdir -p plot

$(OBJS): $(SRCS)
	@echo "Compiling..."
	@time g++ -I include -o $(BUILD_DIR)/main $(SRCS)

run: all
	@echo "Running..."
	@time $(BUILD_DIR)/main

plot: plot_dir
	@echo "Plotting..."
	@time gnuplot scritps/plot3D.gp

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(DATA_DIR) plot

.PHONY: all run plot clean plot_dir