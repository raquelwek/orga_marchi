#ifndef FUNCIONES_CACHE_H
#define FUNCIONES_CACHE_H

#include "procesar_archivos.h" // Dependencia para acceder a la estructura Cache
#include "funciones_cache.h" // Implementación de las funciones de la caché
/**
 * @brief Realiza una lectura desde la caché.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param direccion Dirección de memoria a leer.
 * @return 1 si es un hit, 0 si es un miss.
 */
int leer_cache(Cache* cache, unsigned int direccion);

/**
 * @brief Realiza una escritura en la caché.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param direccion Dirección de memoria a escribir.
 * @return 1 si es un hit, 0 si es un miss.
 */
int escribir_cache(Cache* cache, unsigned int direccion);

/**
 * @brief Reemplaza un bloque en un conjunto de la caché.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param direccion Dirección de memoria para el reemplazo.
 * @return void
 */
void reemplazar_bloque(Cache* cache, unsigned int direccion);

/**
 * @brief Limpia la caché escribiendo todos los bloques sucios a memoria principal.
 * 
 * @param cache Puntero a la estructura de la caché.
 */
void limpiar_cache(Cache* cache);

#endif // FUNCIONES_CACHE_H
