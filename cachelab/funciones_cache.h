#ifndef PROCESAR_ARCHIVOS_H
#define PROCESAR_ARCHIVOS_H
#include <stdint.h> // Dependencia para usar uint32_t
#include "funciones_cache.h" // Dependencia para manejar la estructura de la caché
#include <hash.h>
/**
 * @brief Estructura que representa una caché.
 * 
 * Contiene los datos necesarios para la simulación, como estadísticas, 
 * configuraciones de tamaño, políticas de reemplazo, y los conjuntos.
 */
typedef struct {
    int tamanio_cache;      // Tamaño total de la caché (en bytes)
    int tamanio_bloque;     // Tamaño de cada bloque (en bytes)
    int num_conjuntos;      // Número de conjuntos en la caché
    hash_t* contador;
    hash_t** sets;
} Cache;


typedef struct line{
    uint32_t valido;             // 0: invalido, 1: valido
    uint32_t dirty;              // 0: no modificado, 1: modificado
    char tag;
    uint32_t orden;              // para LRU
    //uint32_t* bloque;
} line_t;
/*
typedef struct acceso{
    uint32_t address;
    uint32_t operacion;          // 0: lectura, 1: escritura
} acceso_t;
*/
/**
 * @brief Crea e inicializa una caché.
 * 
 * @param tamanio_cache Tamaño total de la caché (en bytes).
 * @param tamanio_bloque Tamaño de cada bloque (en bytes).
 * @param num_conjuntos Número de conjuntos en la caché.
 * @return Un puntero a la estructura Cache creada alocada en el heap.
 */

Cache* crear_cache(uint32_t tamanio_cache, uint32_t lineas, uint32_t num_sets);
/**
 * @brief Procesa una línea de traza y actualiza la caché.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param linea a procesar
 * @param direccion Dirección de memoria a acceder.
 */
void procesar_linea_traza(Cache* cache, char linea, uint32_t direccion);

/**
 * @brief Libera toda la memoria asociada a la caché.
 * 
 * @param cache Puntero a la estructura de la caché que se va a liberar.
 */
void liberar_cache(Cache* cache);

#endif // PROCESAR_ARCHIVOS_H
