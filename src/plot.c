/**
 * @file
 * @brief Modulo para generar el diagrama de Gantt de la planificacion.
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @copyright MIT License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "plot.h"
#include "sched.h"
#include "split.h"
#include "util.h"

// Include headers for Linux/Unix systems
#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#endif

int create_plot(char *path, list *processes)
{
    int max_time;
    int xticks;
    int nprocesses;

    node_iterator it;
    node_iterator slice_it;
    process *proc;
    slice *s;
    int line_style;

    char *image_path;
    char *plot_cmd;

    image_path = concat(remove_extension(path), ".png");
    plot_cmd = concat("gnuplot \"", concat(path, "\""));

    int line_cnt;
    FILE *stream;

    if (processes == 0 || processes->count == 0)
    {
        return 0;
    }

    //Obtener la cantidad de procesos
    nprocesses = processes->count;

    // Obtener el tiempo maximo de planificacion
    max_time = max_scheduling_time(processes);

    if (max_time == 0)
    {
        // Dibujar un grafico vacio con 10 unidades de tiempo
        max_time = 10;
    }

    // Open file to plot command
    stream = fopen(path, "w");

    if (stream == NULL)
    {
        return 0;
    }

    xticks = 5;

    if (max_time > 100)
    {
        xticks = 10;
    }

    if (max_time < 20)
    {
        xticks = 1;
    }

    fprintf(stream, "set term pngcairo dashed size 1024,768\n");
    fprintf(stream, "set output '%s'\n", image_path);
    fprintf(stream, "set style fill solid\n");
    fprintf(stream, "set xrange [0:%d]\n", max_time + (max_time % xticks) + xticks);
    fprintf(stream, "set yrange [0:%d]\n", nprocesses + 1);
    fprintf(stream, "unset ytics\n");
    fprintf(stream, "set title 'Planificacion de Procesos'\n");
    
    // Configurar etiquetas del eje Y con nombres de procesos
    fprintf(stream, "set ytics(");
    for (it = head(processes); it != 0; it = next(it))
    {
        proc = (process *)it->data;
        fprintf(stream, "'%s' %d", proc->name, proc->pid);
        if (it->next != 0)
        {
            fprintf(stream, ",");
        }
    }
    fprintf(stream, ")\n");
    
    fprintf(stream, "set xtics %d\n", xticks);
    fprintf(stream, "unset key\n");
    fprintf(stream, "set xlabel 'Tiempo'\n");
    fprintf(stream, "set ylabel 'Proceso'\n");

    // Configurar estilos de linea
    fprintf(stream, "set style line 1 lt 1 lw 2 lc rgb '#00ff00'\n"); // Verde - Tiempo de ejecucion
    fprintf(stream, "set style line 2 lt 1 lw 2 lc rgb '#00ff00'\n"); // Verde - Tiempo de ejecucion
    fprintf(stream, "set style line 3 lt 1 lw 1 lc rgb '#ff0000'\n"); // Rojo - Tiempo de espera

    fprintf(stream, "set style arrow 1 heads size screen 0.008,90 ls 1\n");
    fprintf(stream, "set style arrow 2 heads size screen 0.008,100 ls 2\n");
    fprintf(stream, "set style arrow 3 heads size screen 0.008,100 ls 3\n");

    // Generar flechas para cada slice de tiempo
    line_cnt = 1;
    for (it = head(processes); it != 0; it = next(it))
    {
        proc = (process *)it->data;
        for (slice_it = head(proc->slices); slice_it != 0; slice_it = next(slice_it))
        {
            s = (slice *)slice_it->data;
            line_style = 1;
            if (s->type == WAIT)
            {
                line_style = 3; // Estilo rojo para tiempo de espera
            }
            fprintf(stream, "set arrow %d from %d,%d to %d,%d as %d\n",
                    line_cnt++, s->from, proc->pid,
                    s->to, proc->pid, line_style);
        }
    }

    // Plot no data (NaN) - solo para generar el grafico con las flechas
    fprintf(stream, "plot NaN\n");
    fclose(stream);

// Execute gnuplot command based on operating system
#ifdef __linux__
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Warning! unable to execute plot program!\n");
        free(image_path);
        return 0;
    }

    if (pid == 0)
    {
        // Child process
        execlp("gnuplot", "gnuplot", path, (char *)NULL);
        fprintf(stderr, "Warning! error executing plot program! Make sure gnuplot is installed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status = 0;
        if (waitpid(pid, &status, 0) != pid)
        {
            fprintf(stderr, "Warning! unable to plot chart!\n");
            free(image_path);
            return 0;
        }
        
        if (WEXITSTATUS(status) != 0)
        {
            fprintf(stderr, "Warning! gnuplot execution failed!\n");
            free(image_path);
            return 0;
        }
    }
#else
    // Fallback for other systems - use system() call
    char *full_cmd = concat("gnuplot \"", concat(path, "\""));
    int result = system(full_cmd);
    free(full_cmd);
    
    if (result != 0)
    {
        fprintf(stderr, "Warning! gnuplot execution failed!\n");
        free(image_path);
        return 0;
    }
#endif

    printf("Diagram generated: %s\n", image_path);
    free(image_path);
    return 1;
}