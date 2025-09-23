/**
 * @file
 * @brief Planificación de procesos
 * @author Erwin Meza Vega [emezav@unicauca.edu.co]
 *
 */

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sched.h"

/* Rutina para la planificacion - Simplificada solo para FIFO y RR */
void schedule(list *processes, priority_queue *queues, int nqueues)
{
    int tiempo_actual = 0;
    int procesos_restantes = processes->count;
    int cola_actual = 0;
    process *proceso_actual = NULL;
    int tiempo_asignado = 0;
    int siguiente_llegada = 0;
    int i;

    list *sequence = create_list(); // Secuencia de ejecucion
    sequence_item *si; // Item de secuencia de CPU

    node_iterator it;
    process *p;
    int total_waiting = 0;
    int tiempo_total_simulacion = 0;

    printf("=== INICIANDO SIMULACION ===\n");
    printf("Procesos a simular: %d\n", procesos_restantes);
    printf("Colas de prioridad: %d\n", nqueues);

    // Obtener el tiempo minimo de llegada
    tiempo_actual = get_next_arrival(queues, nqueues);
    if (tiempo_actual == -1) tiempo_actual = 0;

    // Procesar llegadas iniciales
    process_arrival(tiempo_actual, queues, nqueues);

    // Algoritmo principal de planificacion - Simplificado
    while (procesos_restantes > 0) {

        // Buscar la cola de mayor prioridad que tenga procesos listos
        int cola_encontrada = 0;
        for (i = 0; i < nqueues; i++) {
            int cola_idx = (cola_actual + i) % nqueues;
            if (!empty(queues[cola_idx].ready)) {
                cola_actual = cola_idx;
                cola_encontrada = 1;
                break;
            }
        }

        if (!cola_encontrada) {
            // No hay procesos listos, avanzar al siguiente evento de llegada
            siguiente_llegada = get_next_arrival(queues, nqueues);
            if (siguiente_llegada == -1) {
                // No hay mas llegadas, terminar
                break;
            }
            tiempo_actual = siguiente_llegada;
            process_arrival(tiempo_actual, queues, nqueues);
            continue;
        }

        // Obtener el primer proceso de la cola (FIFO simple)
        proceso_actual = front(queues[cola_actual].ready);
        pop_front(queues[cola_actual].ready);

        // Cambiar estado del proceso a RUNNING
        proceso_actual->state = RUNNING;
        printf("[T=%d] Ejecutando %s (prioridad %d, tiempo restante: %d)\n",
               tiempo_actual, proceso_actual->name, cola_actual + 1, proceso_actual->remaining_time);

        // Calcular tiempo a asignar
        if (queues[cola_actual].strategy == RR) {
            // Round Robin: usar quantum
            tiempo_asignado = min(queues[cola_actual].quantum, proceso_actual->remaining_time);
        } else {
            // FIFO: dar todo el tiempo restante (no expropiativo)
            tiempo_asignado = proceso_actual->remaining_time;
        }

        // Actualizar tiempos del proceso actual
        proceso_actual->cpu_time += tiempo_asignado;
        proceso_actual->remaining_time -= tiempo_asignado;

        // Agregar slice de CPU al proceso
        push_back(proceso_actual->slices,
                  create_slice(CPU, tiempo_actual, tiempo_actual + tiempo_asignado));

        // Agregar a la secuencia de ejecucion
        si = (sequence_item *)malloc(sizeof(sequence_item));
        si->name = proceso_actual->name;
        si->time = tiempo_asignado;
        push_back(sequence, si);

        // Aumentar tiempo de espera de los demas procesos listos
        add_waiting_time(processes, proceso_actual, tiempo_actual, tiempo_asignado);

        // Avanzar el tiempo
        tiempo_actual += tiempo_asignado;

        // Procesar llegadas durante este intervalo
        process_arrival(tiempo_actual, queues, nqueues);

        // Verificar si el proceso ha finalizado
        if (proceso_actual->remaining_time <= 0) {
            proceso_actual->state = FINISHED;
            proceso_actual->finished_time = tiempo_actual;
            push_back(queues[cola_actual].finished, proceso_actual);
            procesos_restantes--;
            printf("[T=%d] Proceso %s FINALIZADO\n", tiempo_actual, proceso_actual->name);
        } else {
            // El proceso no finalizo, volver a la cola de listos
            proceso_actual->state = READY;

            if (queues[cola_actual].strategy == RR) {
                // Round Robin: insertar al final de la cola
                push_back(queues[cola_actual].ready, proceso_actual);
            } else {
                // FIFO: insertar al inicio (continua su ejecucion)
                push_front(queues[cola_actual].ready, proceso_actual);
            }
        }

        // Cambiar a la siguiente cola de prioridad
        cola_actual = (cola_actual + 1) % nqueues;
    }

    // CALCULAR ESTADISTICAS FINALES
    tiempo_total_simulacion = tiempo_actual;

    // IMPRIMIR RESULTADOS DE LA SIMULACION
    printf("\n=== RESULTADOS DE LA SIMULACION ===\n");
    printf("Colas de prioridad: %d\n", nqueues);
    printf("Tiempo total de la simulacion: %d unidades de tiempo\n", tiempo_total_simulacion);

    // Calcular tiempo promedio de espera
    float tiempo_promedio_espera = 0;
    for (it = head(processes); it != 0; it = next(it)) {
        p = (process *)it->data;
        total_waiting += p->waiting_time;
    }
    tiempo_promedio_espera = (float)total_waiting / processes->count;
    printf("Tiempo promedio de espera: %.2f unidades de tiempo\n", tiempo_promedio_espera);

    // TABLA DE PROCESOS
    printf("\n%5s%15s%12s%8s%10s%15s\n",
           "#", "Proceso", "T. Llegada", "Tamaño", "T. Espera", "T. Finalizacion");
    printf("-------------------------------------------------------------------------\n");

    i = 1;
    for (it = head(processes); it != 0; it = next(it)) {
        p = (process *)it->data;
        printf("%5d%15s%12d%8d%10d%15d\n",
               i++, p->name, p->arrival_time, p->execution_time,
               p->waiting_time, p->finished_time);
    }

    // SECUENCIA DE EJECUCION
    printf("\nSecuencia de ejecucion:\n");
    for (it = head(sequence); it != 0; it = next(it)) {
        si = (sequence_item *)it->data;
        printf("%s(%d) ", si->name, si->time);
        if (next(it) != NULL) printf("- ");
    }
    printf("\n");

    // Limpiar memoria de la secuencia
    clear_list(sequence, 1);
    destroy_list(sequence, 0);
}

priority_queue *create_queues(int n)
{
    priority_queue *ret;
    int i;

    ret = (priority_queue *)malloc(sizeof(priority_queue) * n);

    for (i = 0; i < n; i++)
    {
        ret[i].strategy = RR; // Por defecto RR
        ret[i].quantum = 1;   // Quantum por defecto
        ret[i].arrival = create_list();
        ret[i].ready = create_list();
        ret[i].finished = create_list();
    }

    return ret;
}

void print_queue(priority_queue *queue)
{
    node_iterator ptr;

    printf("%s q=", (queue->strategy == RR) ? "RR" : "FIFO");
    printf("%d ", queue->quantum);

    printf("ready (%d): { ", queue->ready->count);
    for (ptr = head(queue->ready); ptr != 0; ptr = next(ptr))
    {
        print_process((process *)ptr->data);
    }
    printf("} \n");

    printf("arrival (%d): { ", queue->arrival->count);
    for (ptr = head(queue->arrival); ptr != 0; ptr = next(ptr))
    {
        print_process((process *)ptr->data);
    }
    printf("} \n");

    printf("finished (%d): { ", queue->finished->count);
    for (ptr = head(queue->finished); ptr != 0; ptr = next(ptr))
    {
        print_process((process *)ptr->data);
    }
    printf("}\n");
}

int compare_arrival(void *const a, void *const b)
{
    process *p1 = (process *)a;
    process *p2 = (process *)b;
    return p2->arrival_time - p1->arrival_time;
}

process *create_process(char *name, int arrival_time, int execution_time)
{
    process *p;

    // Reservar memoria para el proceso
    p = (process *)malloc(sizeof(process));

    memset(p, 0, sizeof(process));
    strcpy(p->name, name);
    p->arrival_time = arrival_time;
    p->execution_time = execution_time;
    p->priority = -1;
    p->waiting_time = 0;
    p->remaining_time = p->execution_time;
    p->finished_time = -1;
    p->cpu_time = 0;
    p->state = LOADED;
    p->slices = create_list();

    return p;
}

void restart_process(process *p)
{
    p->waiting_time = 0;
    p->finished_time = -1;
    p->remaining_time = p->execution_time;
    p->state = LOADED;
    clear_list(p->slices, 1);
}

void print_slices(process *p)
{
    node_iterator it;
    slice *s;

    for (it = head(p->slices); it != 0; it = next(it))
    {
        s = it->data;
        printf("%s %d -> %d ", (s->type == CPU ? "CPU" : "WAIT"), s->from, s->to);
    }
}

void print_process(process *p)
{
    if (p == 0)
    {
        return;
    }
    printf("(%s arrival:%d execution:%d finished:%d waiting:%d ",
           p->name, p->arrival_time, p->execution_time, p->finished_time, p->waiting_time);
    
    printf("%s )\n", (p->state == READY) ? "ready" : 
                     (p->state == LOADED) ? "loaded" :
                     (p->state == FINISHED) ? "finished" : "unknown");
}

void prepare(list *processes, priority_queue *queues, int nqueues)
{
    int i;
    process *p;
    node_iterator it;

    /* Limpiar las colas de prioridad */
    for (i = 0; i < nqueues; i++)
    {
        if (queues[i].ready != 0)
        {
            clear_list(queues[i].ready, 0);
            queues[i].ready = create_list();
        }
        if (queues[i].arrival != 0)
        {
            clear_list(queues[i].arrival, 0);
            queues[i].arrival = create_list();
        }
        if (queues[i].finished != 0)
        {
            clear_list(queues[i].finished, 0);
            queues[i].finished = create_list();
        }
    }

    /* Inicializar la informacion de los procesos en la lista de procesos */
    for (it = head(processes); it != 0; it = next(it))
    {
        p = (process *)it->data;
        restart_process(p);
        insert_ordered(queues[p->priority].arrival, p, compare_arrival);
    }

    printf("Prepared queues:\n");
    for (i=0; i<nqueues; i++) {
        print_queue(&queues[i]);
    }
}

int process_arrival(int now, priority_queue *queues, int nqueues)
{
    int i;
    process *p;
    int queue_processed;
    int total;

    // Procesar llegadas.
    total = 0;
    for (i = 0; i < nqueues; i++)
    {
        if (empty(queues[i].arrival))
        {
            continue;
        }

        queue_processed = 0;
        do
        {
            p = front(queues[i].arrival);

            if (p == 0)
            {
                queue_processed = 1;
                continue;
            }

            // Ignorar el proceso si no es momento de llevarlo a la cola de listos
            if (p->arrival_time > now)
            {
                queue_processed = 1;
                continue;
            }

            printf("[%d] Process %s arrived at %d.\n", now, p->name, p->arrival_time);
            p->state = READY;
            
            // Dibujar la linea del tiempo de espera si llega tarde
            if (now > p->arrival_time)
            {
                int initial_wait = now - p->arrival_time;
                p->waiting_time += initial_wait;
                push_back(p->slices, create_slice(WAIT, p->arrival_time, now));
            }

            total++;

            // Para FIFO y RR, insertar al final de la cola
            push_back(queues[i].ready, p);

            // Quitar el proceso de la cola de llegadas
            pop_front(queues[i].arrival);

        } while (!queue_processed);
    }
    return total;
}

int get_next_arrival(priority_queue *queues, int nqueues)
{
    int ret = INT_MAX;
    process *p;
    int i;

    for (i = 0; i < nqueues; i++)
    {
        p = front(queues[i].arrival);
        if (p != 0)
        {
            ret = min(ret, p->arrival_time);
        }
    }

    if (ret == INT_MAX)
    {
        ret = -1;
    }

    return ret;
}

int get_ready_count(priority_queue *queues, int nqueues)
{
    int ret = 0;
    int i;

    for (i = 0; i < nqueues; i++)
    {
        ret = ret + queues[i].ready->count;
    }
    return ret;
}

int max_scheduling_time(list *processes)
{
    node_iterator it;
    process *p;
    int max = 0;

    for (it = head(processes); it != 0; it = next(it))
    {
        p = (process *)it->data;
        if (p->finished_time > max)
        {
            max = p->finished_time;
        }
    }

    return max;
}

void add_waiting_time(list *processes, process *current_process, int current_time, int current_slice)
{
    node_iterator it;
    process *p;
    for (it = head(processes); it != 0; it = next(it))
    {
        p = (process *)it->data;
        if (p != current_process && p->state == READY)
        {
            p->waiting_time += current_slice;
            push_back(p->slices, create_slice(WAIT, current_time, current_time + current_slice));
        }
    }
}

slice *create_slice(slice_type type, int from, int to)
{
    slice *s;

    s = (slice *)malloc(sizeof(slice));
    s->type = type;
    s->from = from;
    s->to = to;
    return s;
}