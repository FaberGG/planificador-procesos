#ifndef SCHED_H
#define SCHED_H

/**
 * @file
 * @brief Planificacion de procesos.
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @copyright MIT License
 */

#include "list.h"

#ifndef min
/** @brief Encuentra el minimo entre dos valores */
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/** @brief Algoritmo de planificacion - Solo FIFO y RR */
typedef enum
{
    FIFO,
    RR
} strategy;

/** @brief Tipo de slice de tiempo */
typedef enum
{
    CPU,
    WAIT
} slice_type;

/** @brief Tajada de tiempo*/
typedef struct
{
    slice_type type; /*!< Tipo de slice */
    int from;        /*!< Tiempo inicial */
    int to;          /*!< Tiempo final */
} slice;

/** @brief Estados de un proceso */
enum state
{
    UNDEFINED,
    LOADED,
    READY,
    RUNNING,
    FINISHED
};

/** @brief Item de secuencia de ejecucion de un proceso */
typedef struct
{
    char *name; /*!< Proceso de la secuencia (cpu)*/
    int time;   /*!< Tiempo de la secuencia */
} sequence_item;

/** @brief Definicion de un proceso */
typedef struct
{
    int pid;            /*!< PID Del proceso */
    char name[80];      /*!< Nombre del proceso (archivo ejecutable) */
    int priority;       /*!< Prioridad */
    int arrival_time;   /*!< Tiempo de llegada */
    int execution_time; /*!< Tiempo total de ejecucion */
    int remaining_time; /*!< Tiempo restante de ejecucion */
    int waiting_time;   /*!< Tiempo de espera */
    int finished_time;  /*!< Tiempo de finalizacion */
    int cpu_time;       /*!< Tiempo de CPU que ha sido asignado */
    enum state state;   /*!< Estado del proceso */
    list *slices;       /*!< Slices de tiempo */
} process;

/** @brief Cola de prioridad */
typedef struct
{
    int quantum;       /*!< Quantum asignado a la cola de prioridad */
    strategy strategy; /*!< Estrategia de planificacion */
    list *ready;       /*!< Cola de procesos listos */
    list *arrival;     /*!< Cola de llegada de procesos*/
    list *finished;    /*!< Cola de procesos finalizados */
} priority_queue;

/**
 * @brief Rutina para la planificacion
 * @param processes Lista de procesos.
 * @param queues Arreglo de colas de prioridad.
 * @param nqueues Cantidad de colas de prioridad.
 */
void schedule(list *processes, priority_queue *queues, int nqueues);

/**
 * @brief Crea un arreglo de colas de prioridad
 * @param n Numero de colas de prioridad
 * @return Arreglo de colas de prioridad
 */
priority_queue *create_queues(int n);

/**
 * @brief Imprime la informacion de una cola de prioridad
 * @param queue Cola de prioridad a imprimir
 */
void print_queue(priority_queue *queue);

/**
 * @brief Retorna el numero de procesos listos en una cola de prioridad
 * @param queues Colas de prioridad
 * @param nqueues Numero de colas de prioridad
 * @return Cantidad de procesos en estado de listo en todas las colas.
 */
int get_ready_count(priority_queue *queues, int nqueues);

/**
 * @brief Retorna el tiempo en el cual se presenta la nueva llegada a la cola de listos de una cola de prioridad
 * @param queues Arreglo de colas de prioridad
 * @param nqueues Cantidad de colas de prioridad
 * @return Tiempo de llegada del nuevo proceso
 */
int get_next_arrival(priority_queue *queues, int nqueues);

/**
 * @brief Procesa la llegada de procesos  a una cola de prioridad
 * @param now Tiempo actual
 * @param queues Arreglo de colas de prioridad
 * @param nqueues Cantidad de colas de prioridad
 * @return Cantidad de procesos que se pasaron a estado de listo
 */
int process_arrival(int now, priority_queue *queues, int nqueues);

/**
 * @brief  Imprimir la informacion de un proceso
 * @param p Proceso a imprimir
 */
void print_process(process *p);

/**
 * @brief Crea un nuevo proceso.
 * @param name Nombre del proceso
 * @param arrival_time Tiempo de llegada
 * @param execution_time Tiempo de ejecucion
 * @return Nuevo proceso
 */
process *create_process(char *name, int arrival_time, int execution_time);

/**
 * @brief Reinicia un un proceso
 * @param p Proceso
 */
void restart_process(process *p);

/**
 * @brief Compara dos procesos por tiempo de llegada.
 * @param a Proceso A
 * @param b Proceso B
 * @return Resta de los tiempos de llegada de p2 y p1
 */
int compare_arrival(void *const a, void *const b);

/**
 * @brief Prepara las estructuras de datos para la simulacion.
 * @param processes Lista de procesos.
 * @param queues Arreglo de colas de prioridad.
 * @param nqueues Cantidad de colas de prioridad.
 */
void prepare(list *processes, priority_queue *queues, int nqueues);

/**
 * @brief  Calcula el tiempo total de la simulacion
 * @param processes Lista de procesos
 * @return Tiempo en el cual termina el ultimo proceso
 */
int max_scheduling_time(list *processes);

/**
 * @brief Crea una nueva slice de tiempo
 * @param type Tipo de slice de tiempo (CPU, WAIT)
 * @param from Tiempo de inicio
 * @param to Tiempo de finalizacion
 * @return Nueva slice.
 */
slice *create_slice(slice_type type, int from, int to);

/**
 * @brief Adiciona el tiempo de espera a los procesos listos
 * @param processes Lista de procesos
 * @param current_process Proceso que tuvo la CPU
 * @param current_time Tiempo actual
 * @param current_slice Cantidad de tiempo que el proceso tuvo la CPU
 */
void add_waiting_time(list *processes, process *current_process, int current_time, int current_slice);

/**
 * @brief Imprime las asignaciones de CPU y tiempo de espera de un proceso.
 * @param p Proceso a imprimir
 */
void print_slices(process *p);

#endif