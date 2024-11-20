#ifndef FUNCIONES_CACHE_H
#define FUNCIONES_CACHE_H
#include <stdint.h> // Dependencia para usar uint32_t
#include "funciones_cache.h" // Dependencia para manejar la estructura de la caché
#include "hash.h"
/**
 * @brief Estructura que representa una caché.
 * 
 * Contiene los datos necesarios para la simulación, como estadísticas, 
 * configuraciones de tamaño, políticas de reemplazo, y los conjuntos.
 */
typedef struct {
    uint32_t tamanio_cache;      // Tamaño total de la caché (en bytes)
    uint32_t tamanio_bloque;     // Tamaño de cada bloque (en bytes)
    uint32_t num_conjuntos;      // Número de conjuntos en la caché
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
void destruir_cache(Cache* cache);
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

bool es_hit(Cache* cache, uint32_t set_index, char tag, uint32_t b_off,  uint32_t address, char operacion);
bool es_dirty_miss(Cache* cache, uint32_t set_index, char tag, uint32_t b_off,  uint32_t address, char operacion);
void agg_tag(Cache* cache, uint32_t set_index, char tag, char OP);
char obtener_tag_a_desalojar(Cache* cache, uint32_t set_index);
hash_t** inicializar_sets(uint32_t num_sets, uint32_t lineas);
void destruir_sets(Cache* cache,hash_t** sets);
hash_t* inicializar_contador();
void destruir_linea(void* linea);
void destruir_int(void* num);

#endif // FUNCIONES_CACHE_H
