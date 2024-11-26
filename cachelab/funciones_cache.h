#ifndef FUNCIONES_CACHE_H
#define FUNCIONES_CACHE_H
#include <stdint.h> // Dependencia para usar uint32_t
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#define PENALTY 100
#define CONTADORES_CANT 10
extern const char* strings[CONTADORES_CANT];

typedef struct {
    uint32_t tamanio_cache;      // Tamaño total de la caché (en bytes)
    uint32_t tamanio_bloque;     // Tamaño de cada bloque (en bytes)
    uint32_t num_conjuntos;      // Número de conjuntos en la caché
    uint32_t num_lineas; 
    uint32_t indice_op;
    hash_t* contador;
    hash_t** sets;

} Cache;


typedef struct line{
    uint32_t numero_linea;
    uint32_t valido;             // 0: invalido, 1: valido
    uint32_t dirty;              // 0: no modificado, 1: modificado
    char* tag;
    uint32_t last_used;              // para LRU
} line_t;

typedef struct verboso {
    uint32_t indice_op;
    char* case_identifier;
    uint32_t cache_index;
    char* cache_tag;
    uint32_t cache_line;
    int32_t line_tag;
    uint8_t valid_bit;
    uint32_t dirty_bit;
    uint32_t last_used;

}verboso_t;

// crear_cache crea una caché de tamaño C, con E líneas por set, S sets y B tamaño de bloque
Cache* crear_cache(uint32_t C, uint32_t E, uint32_t S, uint32_t B);

// destruir_cache libera la memoria asociada a la caché
void destruir_cache(Cache* cache);

// hit_case determina si es hit, en caso de serlo actualiza los contadores correspondientes
bool hit_case(Cache* cache, uint32_t set_index, char* tag, char* operacion, verboso_t* info);

// dirty_miss_case contabiliza los contadores en el caso de dirty miss
void dirty_miss_case(char* operacion, uint32_t tam_block, hash_t* contador);

// miss_case contabiliza los contadores en el caso de miss
void miss_case(char* operacion, uint32_t tam_block, hash_t* contador);

// es_linea_valida determina si la línea es válida
bool es_linea_valida(line_t* linea);

// set_tiene_espacio determina si el set tiene espacio para agregar una nueva línea
bool set_tiene_espacio(hash_t* set, uint32_t lineas_por_set);

// agg_tag agrega un tag no encontrado al set, si el set está lleno,
// desaloja el tag que fue menos usado recientemente
// post: devuelve true si el linea desalojado era dirty, false en caso contrario
void agg_tag(Cache* cache, uint32_t set_index, char* tag, char* OP, verboso_t* info);

// Actualiza los campos a imprimir en caso de modo verboso
void campos_verboso(verboso_t* v, line_t*  linea, char* caso);

// obtener_tag_a_desalojar obtiene el tag a desalojar menos usado recientemente
line_t* obtener_linea_a_desalojar(Cache* cache, uint32_t set_index);

// inicializar_sets inicializa los sets de la caché vacíos
hash_t** inicializar_sets(uint32_t num_sets);

// destruir_sets libera la memoria asociada a los sets, y las líneas de cada set
void destruir_sets(Cache* cache,hash_t** sets);

// inicializar_contador inicializa el hash de contadores con cada métrica en 0
hash_t* inicializar_contador();

// destruir_linea libera la memoria asociada a una línea
void destruir_linea(line_t* linea);

// destruir_int libera la memoria asociada a un entero
void destruir_int(void* num);

#endif //FUNCIONES_CACHE_H
