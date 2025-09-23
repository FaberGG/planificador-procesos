/**
 * @file
 * @brief String split library
 * @author Author John Navia, Ricardo Delgado
 * @copyright MIT License
 */
#ifndef SPLIT_H_
#define SPLIT_H_

/** @brief Cantidad maxima de subcadenas */
#define MAX_PARTS 255

/**
 * @brief Lista de subcadenas
 */
typedef struct
{
    char *parts[MAX_PARTS]; /*!< Arreglo de subcadenas */
    int count;              /*!< Cantidad de subcadenas*/
} split_list;

/**
 * @brief Divide una cadena en partes.
 * @param str Cadena a dividir
 * @param delim Delimitadores, o NULL para delimitadores por defecto.
 */
split_list *split(char *str, char *delim);

/**
 * @brief Libera una lista.
 * @param l Lista a liberar
 */
void free_split_list(split_list *l);

#endif