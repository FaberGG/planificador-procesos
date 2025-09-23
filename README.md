# Simulador de Planificación de Procesos

## Descripción

Este proyecto implementa un simulador de planificación de procesos en un sistema operativo utilizando colas de prioridad. El simulador soporta los algoritmos de planificación **FIFO** (First In First Out) y **Round Robin (RR)**.

## Estructura del Proyecto

```
lsoA-proy01-g0X/
├── src/
│   ├── main.c          # Programa principal
│   ├── sched.c         # Implementación de la planificación
│   ├── sched.h         # Definiciones de la planificación
│   ├── list.c          # Implementación de listas enlazadas
│   ├── list.h          # Definiciones de listas enlazadas
│   ├── split.c         # Funciones para dividir cadenas
│   ├── split.h         # Definiciones de split
│   ├── util.c          # Funciones de utilidad
│   ├── util.h          # Definiciones de utilidades
│   ├── plot.c          # Generación de diagramas de Gantt
│   ├── plot.h          # Definiciones de plot
│   ├── Makefile        # Archivo de compilación
│   └── test/           # Archivos de prueba
│       ├── test1.txt
│       ├── test2.txt
│       └── test3.txt
```

## Compilación

Para compilar el proyecto:

```bash
cd src
make
```

Para limpiar archivos compilados:

```bash
make clean
```

## Uso

El simulador lee comandos desde la entrada estándar. Los comandos disponibles son:

### Comando DEFINE

Define parámetros para la simulación:

```
# Definir número de colas de prioridad
DEFINE queues <número>

# Definir estrategia para una cola
DEFINE scheduling <cola> <estrategia>

# Definir quantum para una cola
DEFINE quantum <cola> <quantum>
```

**Estrategias soportadas:**

- `RR`: Round Robin
- `FIFO`: First In First Out

### Comando PROCESS

Anuncia la llegada de un proceso:

```
PROCESS <nombre> <tiempo_llegada> <tiempo_ejecución> <prioridad>
```

### Comando START

Inicia la simulación:

```
START
```

## Ejemplo de Entrada

```
# Configurar 3 colas de prioridad
DEFINE queues 3
DEFINE scheduling 1 RR
DEFINE scheduling 2 RR
DEFINE scheduling 3 FIFO
DEFINE quantum 1 5
DEFINE quantum 2 4
DEFINE quantum 3 2

# Definir procesos
PROCESS process1 10 30 1
PROCESS process2 0 21 2
PROCESS process3 7 25 3

# Iniciar simulación
START
```

## Ejecución

### Desde archivo:

```bash
./scheduler < test/test1.txt
```

### Entrada interactiva:

```bash
./scheduler
```

## Pruebas

El proyecto incluye archivos de prueba preconfigurados:

```bash
# Crear archivos de prueba
make setup-tests

# Ejecutar pruebas
make test1
make test2
make test3
```

## Generación de Diagramas de Gantt

El simulador puede generar diagramas de Gantt usando gnuplot. Para instalar gnuplot:

### Ubuntu/Debian:

```bash
sudo apt-get install gnuplot
```

### CentOS/RHEL:

```bash
sudo yum install gnuplot
```

### macOS:

```bash
brew install gnuplot
```

## Salida del Programa

El programa genera una tabla con los resultados de la simulación:

```
=== RESULTADOS DE LA SIMULACION ===
Colas de prioridad: 3
Tiempo total de la simulacion: 76 unidades de tiempo
Tiempo promedio de espera: 15.67 unidades de tiempo

    #        Proceso  T. Llegada   Tamaño T. Espera T. Finalizacion
-------------------------------------------------------------------------
    1       process1          10       30        25              65
    2       process2           0       21        13              55
    3       process3           7       25         5              57

Secuencia de ejecucion:
process2(4) - process3(2) - process2(4) - process1(5) - process2(4) - ...
```

## Algoritmos Implementados

### FIFO (First In First Out)

- No expropiativo
- Los procesos se ejecutan hasta completarse
- El orden de ejecución es por llegada

### Round Robin (RR)

- Expropiativo por tiempo
- Cada proceso recibe un quantum de tiempo
- Los procesos regresan al final de la cola si no terminan

## Características Técnicas

- **Colas de prioridad**: Soporte para múltiples niveles de prioridad
- **Gestión de memoria**: Uso de listas enlazadas dinámicas
- **Tiempo de espera**: Cálculo automático del tiempo de espera por proceso
- **Visualización**: Generación de diagramas de Gantt (opcional)
- **Modular**: Código organizado en módulos reutilizables

## Limitaciones

- Solo soporta algoritmos FIFO y Round Robin
- No incluye algoritmos SJF o SRT
- Requiere gnuplot para generar gráficos
- Entrada limitada a comandos predefinidos

## Autor

Erwin Meza Vega <emezav@unicauca.edu.co>

## Licencia

MIT License
