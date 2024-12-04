#include "funciones_cache.h"
/*
    FUNCIONES PARA INICIALIZAR ESTRUCTURAS DE DATOS
*/
const char* HIT_IDENTIFIER = "1";
const char* MISS_IDENTIFIER = "2a";
const char* MISS_DIRTY_IDENTIFIER = "2b";

Cache* crear_cache(uint32_t C, uint32_t E, uint32_t S, uint32_t B){
    Cache* cache = malloc(sizeof(Cache));
    cache->tamanio_cache = C;
    cache->tamanio_bloque = B;
    cache->num_conjuntos = S;
    cache->num_lineas = E;
    cache->indice_op = 0;
    cache->sets = inicializar_sets(S, E);
    cache -> contador = inicializar_contador();
    return cache;
}

line_t** inicializar_sets(uint32_t S, uint32_t E){
    line_t** sets = malloc(S * sizeof(line_t*));
    for (uint32_t i = 0; i < S; i++){
        sets[i] = malloc(E * sizeof(line_t));
        for (uint32_t j = 0; j < E; j++){
            sets[i][j].numero_linea = j;
            sets[i][j].valido = 0;
            sets[i][j].dirty = 0;
            sets[i][j].tag = -1;
            sets[i][j].last_used = 0;
        }
    }
    return sets;
}
contador_t* inicializar_contador() {
    contador_t* contador = malloc(sizeof(contador_t));
    contador->loads = 0;
    contador->stores = 0;
    contador->rmiss = 0;
    contador->wmiss = 0;
    contador->dirty_rmiss = 0;
    contador->dirty_wmiss = 0;
    contador->bytes_read = 0;
    contador->bytes_written = 0;
    contador->time_w = 0;
    contador->time_r = 0;
    return contador;
}

void destruir_cache(Cache* cache){
    for (uint32_t i = 0; i < cache->num_conjuntos; i++){
        free(cache->sets[i]);
    }
    free(cache->sets);
    free(cache->contador);
    free(cache);
}

/*
    FUNCIONES PARA SIMULAR LA CACHE
*/
bool hit_case(Cache* cache, uint32_t set_index, int32_t tag, op_t* operacion, verboso_t* info){
    bool hit = false;
    line_t* set = cache->sets[set_index];
    for (uint32_t i = 0; i < cache->num_lineas; i++){
        line_t* linea = &set[i];
        if (linea->valido == 1 && linea->tag == tag){
            hit = true;
            
            campos_verboso(info, linea, HIT_IDENTIFIER);
            if (*operacion == WRITTING){
                linea->dirty = 1;
                cache->contador->stores ++;
                cache->contador->time_w ++;
            }else if (*operacion == READING){
                cache->contador->loads ++;
                cache->contador->time_r ++;
            }
            linea->last_used = cache->indice_op;
        }
    }
    return hit;
}
void miss_case(op_t* operacion, uint32_t tam_block, contador_t* contador){
    if (*operacion == READING){
        contador -> loads ++;
        contador->rmiss ++;
        contador->time_r += 1 + (PENALTY);
        
    }else if (*operacion == WRITTING){
        contador -> stores ++;
        contador->wmiss ++;
        contador->time_w += 1 + (PENALTY);
    }
    contador->bytes_read += tam_block;
}
void dirty_miss_case(op_t* operacion, uint32_t tam_block, contador_t* contador){
    if (*operacion == READING){
        contador -> loads ++;
        contador->rmiss ++;
        contador->dirty_rmiss ++;
        contador->time_r += 1 + (2 * PENALTY);
        
    }else if (*operacion == WRITTING){
        contador -> stores ++;
        contador->wmiss ++;
        contador->dirty_wmiss ++;
        contador->time_w += 1 + (2 * PENALTY);
    }
    contador->bytes_read += tam_block;
    contador->bytes_written += tam_block;
}

void campos_verboso(verboso_t* info, line_t* linea, char* caso){
    info->case_identifier = caso;
    info->line_tag = linea->tag;
    info -> cache_line = linea -> numero_linea; 
    info -> valid_bit = linea -> valido;
    info -> dirty_bit = linea -> dirty;
    info -> last_used = linea -> last_used;
   
}

void agg_tag(Cache* cache, uint32_t set_index, int32_t tag, op_t* operacion, verboso_t* info){
    line_t* set = cache->sets[set_index];
    line_t* linea_a_desalojar = obtener_linea_a_desalojar(cache, set_index);
   
        
        if (linea_a_desalojar->dirty == 1){
            campos_verboso(info, linea_a_desalojar, MISS_DIRTY_IDENTIFIER);
            dirty_miss_case(operacion, cache->tamanio_bloque, cache->contador);
        }else {
            campos_verboso(info, linea_a_desalojar, MISS_IDENTIFIER);
            miss_case(operacion, cache->tamanio_bloque, cache->contador);
        }
        linea_a_desalojar->tag = tag;
        linea_a_desalojar->valido = 1;
        linea_a_desalojar->dirty = 0;
        if (*operacion == WRITTING) linea_a_desalojar->dirty = 1;
        linea_a_desalojar->last_used = cache -> indice_op;
}

line_t* obtener_linea_a_desalojar(Cache* cache, uint32_t set_index) {
    line_t* set = cache->sets[set_index];
    line_t* linea_a_desalojar = linea_invalida_menor_index(set, cache->num_lineas);
    
    if (linea_a_desalojar == NULL){
        linea_a_desalojar = valida_menos_usada(cache, set_index);
    }
    return linea_a_desalojar;
}

line_t* linea_invalida_menor_index(line_t* set, uint32_t num_lineas){
    line_t* linea_a_desalojar = NULL;
    for (uint32_t i = 0; i < num_lineas; i++){
        line_t* linea = &set[i];
        if (linea->valido == 0){
            linea_a_desalojar = linea;
            break;
        }
    }
    return linea_a_desalojar;
}

line_t* valida_menos_usada(Cache* cache, uint32_t set_index){
    line_t* set = cache->sets[set_index];
    line_t* linea_a_desalojar = NULL;
    uint32_t orden_minimo = cache->indice_op;
    for (uint32_t i = 0; i < cache->num_lineas; i++){
        line_t* linea = &set[i];
        if (linea->last_used < orden_minimo && linea->valido == 1){
            orden_minimo = linea->last_used;
            linea_a_desalojar = linea;
        }
    }
    return linea_a_desalojar;
}