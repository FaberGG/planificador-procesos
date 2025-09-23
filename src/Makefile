# Makefile for Process Scheduling Simulator
# Author: Faber Navia y Ricardo Delgado
# Optimized for Debian 12 (GNU/Linux)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_GNU_SOURCE
LDFLAGS = -lm
TARGET = scheduler
SRCDIR = .
OBJDIR = obj
TESTDIR = test

# Source files
SOURCES = main.c sched.c list.c split.c util.c plot.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Default target
all: $(OBJDIR) $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Build target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET) *.plt *.png

# Install required dependencies for Debian 12
install-deps:
	sudo apt update
	sudo apt install -y build-essential gcc make gnuplot-x11

test_fifo: $(TARGET) 
	@echo "=== Ejecutando prueba FIFO ==="
	./$(TARGET) < $(TESTDIR)/caso_1_fifo.txt

test_rr: $(TARGET) 
	@echo "=== Ejecutando prueba RR ==="
	./$(TARGET) < $(TESTDIR)/caso_1_rr.txt

test_multi: $(TARGET) 
	@echo "=== Ejecutando prueba multicolas ==="
	./$(TARGET) < $(TESTDIR)/caso_2_multicolas.txt


$(TESTDIR):
	mkdir -p $(TESTDIR)


# Help
help:
	@echo "Targets disponibles:"
	@echo "  all          - Compilar el programa"
	@echo "  clean        - Limpiar archivos compilados"
	@echo "  setup-tests  - Crear archivos de prueba"
	@echo "  test1        - Ejecutar prueba 1"
	@echo "  test2        - Ejecutar prueba 2"
	@echo "  test3        - Ejecutar prueba 3"
	@echo "  test_fifo    - Ejecutar prueba fifo"
	@echo "  test_rr      - Ejecutar prueba rr"
	@echo "  test_multi    - Ejecutar prueba multicolas"
	@echo "  install-deps 	- Información sobre dependencias"
	@echo "  help        	- Mostrar esta ayuda"

.PHONY: all clean test1 test2 test3 test_fifo test_rr test_multi setup-tests install-deps help