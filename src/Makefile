# Makefile for Process Scheduling Simulator
# Author: Erwin Meza Vega <emezav@unicauca.edu.co>
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

# Test with files already created
test_fifo: $(TARGET)
	./$(TARGET) test/caso_1_fifo.txt

test_rr: $(TARGET)
	./$(TARGET) test/caso_1_rr.txt

test_multicolas: $(TARGET)
	./$(TARGET) test/caso_2_multicolas.txt

# Create test directory and sample files
setup-tests: $(TESTDIR)
	@echo "# Test 1: Round Robin básico" > $(TESTDIR)/test1.txt
	@echo "DEFINE queues 2" >> $(TESTDIR)/test1.txt
	@echo "DEFINE scheduling 1 RR" >> $(TESTDIR)/test1.txt
	@echo "DEFINE scheduling 2 FIFO" >> $(TESTDIR)/test1.txt
	@echo "DEFINE quantum 1 3" >> $(TESTDIR)/test1.txt
	@echo "DEFINE quantum 2 5" >> $(TESTDIR)/test1.txt
	@echo "PROCESS P1 0 10 1" >> $(TESTDIR)/test1.txt
	@echo "PROCESS P2 2 5 1" >> $(TESTDIR)/test1.txt
	@echo "PROCESS P3 4 8 2" >> $(TESTDIR)/test1.txt
	@echo "START" >> $(TESTDIR)/test1.txt
	@echo ""
	@echo "# Test 2: FIFO con diferentes prioridades" > $(TESTDIR)/test2.txt
	@echo "DEFINE queues 3" >> $(TESTDIR)/test2.txt
	@echo "DEFINE scheduling 1 FIFO" >> $(TESTDIR)/test2.txt
	@echo "DEFINE scheduling 2 FIFO" >> $(TESTDIR)/test2.txt
	@echo "DEFINE scheduling 3 RR" >> $(TESTDIR)/test2.txt
	@echo "DEFINE quantum 1 1" >> $(TESTDIR)/test2.txt
	@echo "DEFINE quantum 2 1" >> $(TESTDIR)/test2.txt
	@echo "DEFINE quantum 3 2" >> $(TESTDIR)/test2.txt
	@echo "PROCESS A 0 6 1" >> $(TESTDIR)/test2.txt
	@echo "PROCESS B 1 4 2" >> $(TESTDIR)/test2.txt
	@echo "PROCESS C 2 3 3" >> $(TESTDIR)/test2.txt
	@echo "PROCESS D 3 2 1" >> $(TESTDIR)/test2.txt
	@echo "START" >> $(TESTDIR)/test2.txt
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

$(TESTDIR):
	mkdir -p $(TESTDIR)

# Install dependencies (gnuplot for plotting)
install-deps:
	@echo "Para generar gráficos de Gantt, instale gnuplot:"
	@echo "  Ubuntu/Debian: sudo apt-get install gnuplot"
	@echo "  CentOS/RHEL: sudo yum install gnuplot"
	@echo "  macOS: brew install gnuplot"
	@echo "  Windows: Descargue desde http://www.gnuplot.info/"

# Help
help:
	@echo "Targets disponibles:"
	@echo "  all          - Compilar el programa"
	@echo "  clean        - Limpiar archivos compilados"
	@echo "  setup-tests  - Crear archivos de prueba"
	@echo "  test1        - Ejecutar prueba 1"
	@echo "  test2        - Ejecutar prueba 2"
	@echo "  test3        - Ejecutar prueba 3"
	@echo "  install-deps - Información sobre dependencias"
	@echo "  help         - Mostrar esta ayuda"

.PHONY: all clean test1 test2 test3 setup-tests install-deps help