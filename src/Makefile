# Makefile for Process Scheduling Simulator
# Author: Faber Navia y Ricardo Delgado
# Optimized for Debian 12 (GNU/Linux)

CC = gcc
CFLAGS =  -std=c99 -g -D_GNU_SOURCE
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

# === TESTS ===

# old tests (caso_1_rr.txt y caso_2_multicolas.txt)
test_rr_old: $(TARGET)
	@echo "=== Ejecutando prueba caso_1_rr ==="
	./$(TARGET) < $(TESTDIR)/caso_1_rr.txt

test_multi_old: $(TARGET)
	@echo "=== Ejecutando prueba caso_2_multicolas ==="
	./$(TARGET) < $(TESTDIR)/caso_2_multicolas.txt

# FIFO
test_fifo1: $(TARGET)
	@echo "=== Ejecutando prueba FIFO 1 (escalonado) ==="
	./$(TARGET) < $(TESTDIR)/fifo_1_escalonado.txt

test_fifo2: $(TARGET)
	@echo "=== Ejecutando prueba FIFO 2 (mismo tiempo) ==="
	./$(TARGET) < $(TESTDIR)/fifo_2_mismo_tiempo.txt

test_fifo3: $(TARGET)
	@echo "=== Ejecutando prueba FIFO 3 (proceso largo) ==="
	./$(TARGET) < $(TESTDIR)/fifo_3_proceso_largo.txt

test_fifo4: $(TARGET)
	@echo "=== Ejecutando prueba FIFO 4 (mezcla) ==="
	./$(TARGET) < $(TESTDIR)/fifo_4_mezcla.txt

# Round Robin
test_rr1: $(TARGET)
	@echo "=== Ejecutando prueba RR 1 (quantum igual) ==="
	./$(TARGET) < $(TESTDIR)/rr_1_quantum_igual.txt

test_rr2: $(TARGET)
	@echo "=== Ejecutando prueba RR 2 (quantum pequeño) ==="
	./$(TARGET) < $(TESTDIR)/rr_2_quantum_pequeno.txt

test_rr3: $(TARGET)
	@echo "=== Ejecutando prueba RR 3 (variado) ==="
	./$(TARGET) < $(TESTDIR)/rr_3_variado.txt

# Multicolas
test_multi1: $(TARGET)
	@echo "=== Ejecutando prueba Multicolas 1 (FIFO + RR) ==="
	./$(TARGET) < $(TESTDIR)/multicolas_1_fifo_rr.txt

test_multi2: $(TARGET)
	@echo "=== Ejecutando prueba Multicolas 2 (tres colas) ==="
	./$(TARGET) < $(TESTDIR)/multicolas_2_trescolas.txt

test_multi3: $(TARGET)
	@echo "=== Ejecutando prueba Multicolas 3 (FIFO bloqueo) ==="
	./$(TARGET) < $(TESTDIR)/multicolas_3_fifo_bloqueo.txt


# Help
help:
	@echo "Targets disponibles:"
	@echo "  all              - Compilar el programa"
	@echo "  clean            - Limpiar archivos compilados"
	@echo "  install-deps     - Instalar dependencias necesarias"
	@echo ""
	@echo "=== PRUEBAS ==="
	@echo ""
	@echo "  test_fifo1       - Ejecutar FIFO 1 (escalonado)"
	@echo "  test_fifo2       - Ejecutar FIFO 2 (mismo tiempo)"
	@echo "  test_fifo3       - Ejecutar FIFO 3 (proceso largo)"
	@echo "  test_fifo4       - Ejecutar FIFO 4 (mezcla)"
	@echo ""
	@echo "  test_rr1         - Ejecutar RR 1 (quantum igual)"
	@echo "  test_rr2         - Ejecutar RR 2 (quantum pequeño)"
	@echo "  test_rr3         - Ejecutar RR 3 (variado)"
	@echo ""
	@echo "  test_multi1      - Ejecutar Multicolas 1 (FIFO + RR)"
	@echo "  test_multi2      - Ejecutar Multicolas 2 (tres colas)"
	@echo "  test_multi3      - Ejecutar Multicolas 3 (FIFO bloqueo)"
	@echo ""
	@echo "  test_all         - Ejecutar TODOS los tests"
	@echo "  help             - Mostrar esta ayuda"

.PHONY: all clean test_rr_old test_multi_old \
        test_fifo1 test_fifo2 test_fifo3 test_fifo4 \
        test_rr1 test_rr2 test_rr3 \
        test_multi1 test_multi2 test_multi3 \
        install-deps help
