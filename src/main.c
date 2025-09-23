/**
 * @file
 * @brief Programa principal para la simulacion de planificacion de procesos
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @copyright MIT License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched.h"
#include "split.h"
#include "util.h"
#include "plot.h"

#define MAX_LINE 1024

/**
 * @brief Procesa los comandos de entrada
 * @param processes Lista de procesos
 * @param queues Colas de prioridad
 * @param nqueues Numero de colas de prioridad
 * @return Numero de colas configuradas
 */
int process_input(list *processes, priority_queue **queues, int *nqueues);

/**
 * @brief Configura las colas de prioridad segun los comandos DEFINE
 * @param queues Colas de prioridad
 * @param queue_num Numero de cola (1-based)
 * @param strategy_str Estrategia como string ("RR" o "FIFO")
 * @param quantum Quantum para la cola
 */
void configure_queue(priority_queue *queues, int queue_num, char *strategy_str, int quantum);

int main()
{
    list *processes = create_list();
    priority_queue *queues = NULL;
    int nqueues = 0;
    
    printf("=== SIMULADOR DE PLANIFICACION DE PROCESOS ===\n");
    printf("Leyendo comandos desde entrada estandar...\n");
    printf("Comandos disponibles: DEFINE, PROCESS, START\n\n");
    
    // Procesar entrada
    nqueues = process_input(processes, &queues, &nqueues);
    
    if (nqueues == 0) {
        printf("Error: No se definieron colas de prioridad\n");
        return 1;
    }
    
    if (processes->count == 0) {
        printf("Error: No se definieron procesos\n");
        return 1;
    }
    
    // Ejecutar simulacion
    schedule(processes, queues, nqueues);
    
    // Opcional: crear diagrama de Gantt
    printf("\n¿Desea generar diagrama de Gantt? (requiere gnuplot) [s/n]: ");
    char response;
    scanf(" %c", &response);
    if (response == 's' || response == 'S') {
        if (create_plot("gantt.plt", processes)) {
            printf("Diagrama de Gantt generado: gantt.png\n");
        } else {
            printf("No se pudo generar el diagrama de Gantt\n");
        }
    }
    
    // Limpiar memoria
    clear_list(processes, 1);
    destroy_list(processes, 0);
    
    for (int i = 0; i < nqueues; i++) {
        clear_list(queues[i].ready, 0);
        clear_list(queues[i].arrival, 0);
        clear_list(queues[i].finished, 0);
        destroy_list(queues[i].ready, 0);
        destroy_list(queues[i].arrival, 0);
        destroy_list(queues[i].finished, 0);
    }
    free(queues);
    
    return 0;
}

int process_input(list *processes, priority_queue **queues, int *nqueues)
{
    char line[MAX_LINE];
    split_list *parts;
    int process_counter = 1;
    int queues_defined = 0;
    
    while (fgets(line, MAX_LINE, stdin)) {
        // Remover salto de linea
        line[strcspn(line, "\n")] = '\0';
        
        // Ignorar lineas vacias y comentarios
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }
        
        parts = split(line, NULL);
        
        if (parts->count == 0) {
            free_split_list(parts);
            continue;
        }
        
        // Convertir comando a minusculas para comparacion
        lcase(parts->parts[0]);
        
        if (equals(parts->parts[0], "define")) {
            if (parts->count < 3) {
                printf("Error: comando DEFINE incompleto\n");
                free_split_list(parts);
                continue;
            }
            
            lcase(parts->parts[1]);
            
            if (equals(parts->parts[1], "queues")) {
                // DEFINE queues n
                *nqueues = atoi(parts->parts[2]);
                *queues = create_queues(*nqueues);
                queues_defined = 1;
                printf("Definidas %d colas de prioridad\n", *nqueues);
            }
            else if (equals(parts->parts[1], "scheduling")) {
                // DEFINE scheduling queue_num strategy
                if (parts->count < 4 || !queues_defined) {
                    printf("Error: debe definir las colas antes de configurar estrategias\n");
                    free_split_list(parts);
                    continue;
                }
                
                int queue_num = atoi(parts->parts[2]);
                char *strategy_str = parts->parts[3];
                lcase(strategy_str);
                
                if (queue_num < 1 || queue_num > *nqueues) {
                    printf("Error: numero de cola invalido: %d\n", queue_num);
                    free_split_list(parts);
                    continue;
                }
                
                if (equals(strategy_str, "rr")) {
                    (*queues)[queue_num-1].strategy = RR;
                    printf("Cola %d configurada con estrategia RR\n", queue_num);
                } else if (equals(strategy_str, "fifo")) {
                    (*queues)[queue_num-1].strategy = FIFO;
                    printf("Cola %d configurada con estrategia FIFO\n", queue_num);
                } else {
                    printf("Error: estrategia no soportada: %s (use RR o FIFO)\n", strategy_str);
                }
            }
            else if (equals(parts->parts[1], "quantum")) {
                // DEFINE quantum queue_num quantum_value
                if (parts->count < 4 || !queues_defined) {
                    printf("Error: debe definir las colas antes de configurar quantum\n");
                    free_split_list(parts);
                    continue;
                }
                
                int queue_num = atoi(parts->parts[2]);
                int quantum_value = atoi(parts->parts[3]);
                
                if (queue_num < 1 || queue_num > *nqueues) {
                    printf("Error: numero de cola invalido: %d\n", queue_num);
                    free_split_list(parts);
                    continue;
                }
                
                (*queues)[queue_num-1].quantum = quantum_value;
                printf("Cola %d configurada con quantum %d\n", queue_num, quantum_value);
            }
        }
        else if (equals(parts->parts[0], "process")) {
            // PROCESS name arrival_time execution_time priority
            if (parts->count < 5) {
                printf("Error: comando PROCESS incompleto\n");
                free_split_list(parts);
                continue;
            }
            
            char *name = parts->parts[1];
            int arrival_time = atoi(parts->parts[2]);
            int execution_time = atoi(parts->parts[3]);
            int priority = atoi(parts->parts[4]);
            
            if (!queues_defined || priority < 1 || priority > *nqueues) {
                printf("Error: prioridad invalida %d (debe estar entre 1 y %d)\n", priority, *nqueues);
                free_split_list(parts);
                continue;
            }
            
            process *p = create_process(name, arrival_time, execution_time);
            p->pid = process_counter++;
            p->priority = priority - 1; // Convertir a 0-based
            
            push_back(processes, p);
            
            printf("Proceso agregado: %s (llegada:%d, ejecucion:%d, prioridad:%d)\n", 
                   name, arrival_time, execution_time, priority);
        }
        else if (equals(parts->parts[0], "start")) {
            printf("Iniciando simulacion...\n\n");
            free_split_list(parts);
            break;
        }
        else {
            printf("Comando desconocido: %s\n", parts->parts[0]);
        }
        
        free_split_list(parts);
    }
    
    return *nqueues;
}