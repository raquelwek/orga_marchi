#ifndef FUNCIONES_CACHE_H
#define FUNCIONES_CACHE_H
#include <stdint.h> // Dependencia para usar uint32_t
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define PENALTY 100

typedef enum acceso {
	WRITTING,
	READING,
} op_t;

typedef struct{
    uint32_t numero_linea;
    int8_t valido;             // 0: invalido, 1: valido
    uint8_t dirty;              // 0: no modificado, 1: modificado
    int32_t tag;
    uint32_t last_used;              // para LRU
} line_t;

typedef struct {
    uint32_t indice_op;
    char* case_identifier;
    uint32_t cache_index;
    char* cache_tag;
    int32_t cache_line;
    int32_t line_tag;
    uint8_t valid_bit;
    uint32_t dirty_bit;
    uint32_t last_used;
}verboso_t;

typedef struct {
    uint32_t loads;
    uint32_t stores;
    uint32_t rmiss;
    uint32_t wmiss;
    uint32_t dirty_rmiss;
    uint32_t dirty_wmiss;
    uint32_t bytes_read;
    uint32_t bytes_written;
    uint32_t time_w;
    uint32_t time_r;
}contador_t;

typedef struct cache{
    uint32_t tamanio_cache;      // Tamaño total de la caché (en bytes)
    uint32_t tamanio_bloque;     // Tamaño de cada bloque (en bytes)
    uint32_t num_conjuntos;      // Número de conjuntos en la caché
    uint32_t num_lineas;
    uint32_t indice_op;
    line_t** sets; 
    contador_t* contador;
}Cache;

// crear_cache crea una caché de tamaño C, con E líneas por set, S sets y B tamaño de bloque
Cache* crear_cache(uint32_t C, uint32_t E, uint32_t S, uint32_t B);

// destruir_cache libera la memoria asociada a la caché
void destruir_cache(Cache* cache);

// inicializar_sets inicializa los sets de la caché
line_t** inicializar_sets(uint32_t S, uint32_t E);

// inicializar_contador inicializa el contador de la caché
contador_t* inicializar_contador();

// hit_case simula un hit en la caché
bool hit_case(Cache* cache, uint32_t set_index, int32_t tag, op_t* operacion, verboso_t* info);

// miss_case simula un miss en la caché
void miss_case(op_t* operacion, uint32_t tam_block, contador_t* contador);

// dirty_miss_case simula un dirty miss en la caché
void dirty_miss_case(op_t* operacion, uint32_t tam_block, contador_t* contador);

// campos_verboso actualiza los campos de un struct verboso
void campos_verboso(verboso_t* info, line_t* linea, char* caso);

// agg_tag agrega un tag a la caché
void agg_tag(Cache* cache, uint32_t set_index, int32_t tag, op_t* operacion, verboso_t* info);

// obtener_linea_a_desalojar obtiene la línea a desalojar
line_t* obtener_linea_a_desalojar(Cache* cache, uint32_t set_index);

// valida_menos_usada valida la línea menos usada
line_t* valida_menos_usada(Cache* cache, uint32_t set_index);

// es_linea_valida verifica si una línea es válida
line_t* linea_invalida_menor_index(line_t* set, uint32_t lineas_por_set);

#endif // FUNCIONES_CACHE_H