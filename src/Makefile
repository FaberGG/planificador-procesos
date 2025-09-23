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

# Test targets with better error handling
test1: $(TARGET)
	@echo "=== Ejecutando prueba 1 ==="
	@if [ -f $(TESTDIR)/test1.txt ]; then \
		./$(TARGET) < $(TESTDIR)/test1.txt; \
	else \
		echo "Error: Ejecute 'make setup-tests' primero"; \
	fi

test2: $(TARGET)
	@echo "=== Ejecutando prueba 2 ==="
	@if [ -f $(TESTDIR)/test2.txt ]; then \
		./$(TARGET) < $(TESTDIR)/test2.txt; \
	else \
		echo "Error: Ejecute 'make setup-tests' primero"; \
	fi

test3: $(TARGET)
	@echo "=== Ejecutando prueba 3 ==="
	@if [ -f $(TESTDIR)/test3.txt ]; then \
		./$(TARGET) < $(TESTDIR)/test3.txt; \
	else \
		echo "Error: Ejecute 'make setup-tests' primero"; \
	fi

# Create test directory and sample files
setup-tests: $(TESTDIR)
	@echo "# Test FIFO" > $(TESTDIR)/caso_1_fifo.txt
	@echo "DEFINE queues 1" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "DEFINE scheduling 1 FIFO" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "DEFINE quantum 1 10" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "PROCESS p1 0 5 1" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "PROCESS p2 2 4 1" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "PROCESS p3 1 5 1" >> $(TESTDIR)/caso_1_fifo.txt
	@echo "START" >> $(TESTDIR)/caso_1_fifo.txt
	@echo ""
	@echo "# Test RR" > $(TESTDIR)/caso_1_rr.txt
	@echo "DEFINE queues 1" >> $(TESTDIR)/caso_1_rr.txt
	@echo "DEFINE scheduling 1 RR" >> $(TESTDIR)/caso_1_rr.txt
	@echo "DEFINE quantum 1 2" >> $(TESTDIR)/caso_1_rr.txt
	@echo "PROCESS p1 0 5 1" >> $(TESTDIR)/caso_1_rr.txt
	@echo "PROCESS p2 2 4 1" >> $(TESTDIR)/caso_1_rr.txt
	@echo "PROCESS p3 1 5 1" >> $(TESTDIR)/caso_1_rr.txt
	@echo "START" >> $(TESTDIR)/caso_1_rr.txt
	@echo ""
	@echo "# Test 3: Ejemplo del enunciado" > $(TESTDIR)/test3.txt
	@echo "DEFINE queues 3" >> $(TESTDIR)/test3.txt
	@echo "DEFINE scheduling 1 RR" >> $(TESTDIR)/test3.txt
	@echo "DEFINE scheduling 2 RR" >> $(TESTDIR)/test3.txt
	@echo "DEFINE scheduling 3 FIFO" >> $(TESTDIR)/test3.txt
	@echo "DEFINE quantum 1 5" >> $(TESTDIR)/test3.txt
	@echo "DEFINE quantum 2 4" >> $(TESTDIR)/test3.txt
	@echo "DEFINE quantum 3 2" >> $(TESTDIR)/test3.txt
	@echo "PROCESS process1 10 30 1" >> $(TESTDIR)/test3.txt
	@echo "PROCESS process2 0 21 2" >> $(TESTDIR)/test3.txt
	@echo "PROCESS process3 7 25 3" >> $(TESTDIR)/test3.txt
	@echo "START" >> $(TESTDIR)/test3.txt
	@echo "Archivos de prueba creados en $(TESTDIR)/"

test_fifo: $(TARGET) setup-tests
	@echo "=== Ejecutando prueba FIFO ==="
	./$(TARGET) < $(TESTDIR)/caso_1_fifo.txt

test_rr: $(TARGET) setup-tests
	@echo "=== Ejecutando prueba RR ==="
	./$(TARGET) < $(TESTDIR)/caso_1_rr.txt

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
	@echo "  test_fifo        - Ejecutar prueba fifo"
	@echo "  test_rr        - Ejecutar prueba rr"
	@echo "  test_multicolas        - Ejecutar prueba multicolas"
	@echo "  install-deps - Información sobre dependencias"
	@echo "  help         - Mostrar esta ayuda"

.PHONY: all clean test1 test2 test3 setup-tests install-deps help