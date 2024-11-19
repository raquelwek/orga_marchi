#ifndef PROCESAR_ARCHIVOS_H
#define PROCESAR_ARCHIVOS_H
#include <stdint.h> // Dependencia para usar uint32_t
#include "funciones_cache.h" // Dependencia para manejar la estructura de la caché

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
    //char politica_remplazo; // Política de reemplazo ('L' para LRU, 'F' para FIFO, etc.)
    int tiempo_acceso;      // Tiempo acumulado de acceso
    // Otras estructuras como estadísticas, conjuntos, etc.
} Cache;


typedef struct line{
    int valido;             // 0: invalido, 1: valido
    int dirty;              // 0: no modificado, 1: modificado
    int tag;
} line_t;

typedef struct acceso{
    uint32_t address;
    int operacion;          // 0: lectura, 1: escritura
} acceso_t;
/**
 * @brief Crea e inicializa una caché.
 * 
 * @param tamanio_cache Tamaño total de la caché (en bytes).
 * @param tamanio_bloque Tamaño de cada bloque (en bytes).
 * @param num_conjuntos Número de conjuntos en la caché.
 * @param politica_remplazo Política de reemplazo ('L' para LRU, 'F' para FIFO, etc.).
 * @return Un puntero a la estructura Cache creada.
 */
Cache* crear_cache(int tamanio_cache, int lineas, int num_sets);
/**
 * @brief Procesa una línea de traza y actualiza la caché.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param operacion Operación a realizar ('R' para lectura, 'W' para escritura).
 * @param direccion Dirección de memoria a acceder.
 */
void procesar_linea_traza(Cache* cache, char operacion, unsigned int direccion);

/**
 * @brief Libera toda la memoria asociada a la caché.
 * 
 * @param cache Puntero a la estructura de la caché que se va a liberar.
 */
void liberar_cache(Cache* cache);

#endif // PROCESAR_ARCHIVOS_H
