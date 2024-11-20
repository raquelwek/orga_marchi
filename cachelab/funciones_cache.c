/**********  FUNCIONES PARA LA CACHE *********/
#include "funciones_cache.h"
#define ADDRESS_SIZE 32     // Tamaño de dirección en bits
#define CONTADORES_CANT 10

Cache* crear_cache(uint32_t tamanio_cache, uint32_t lineas, uint32_t num_sets){
    Cache* cache = malloc(sizeof(Cache));
    cache->tamanio_cache = tamanio_cache;
    cache->tamanio_bloque = lineas;
    cache->num_conjuntos = num_sets;
    cache->contador = inicializar_contador();
    cache->sets = inicializar_sets(num_sets, lineas);
    return cache;
}

void destruir_cache(Cache* cache){
    destruir_sets(cache, cache->sets);
    hash_destruir(cache->contador);
    free(cache);
}

bool es_hit(Cache* cache, uint32_t set_index, char tag, uint32_t b_off,  uint32_t address, char operacion){
    hash_t* set = cache->sets[set_index];
    hash_t* contador = cache->contador;  
    if (!hash_pertenece(set, &tag)) return false;//El tag no se encuentra en el set
    line_t* linea = hash_obtener(set, &tag);
    if (linea->valido == 1 ){
        if (operacion == 'R' )
        {
            hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
            hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1);
        } else if (operacion == 'W')
        {
            hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
            hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1);
        }
        return true;
        
    } 
    return false;
}
bool es_dirty_miss(Cache* cache, uint32_t set_index, char tag, uint32_t b_off,  uint32_t address){
    hash_t* set = cache->sets[set_index];
    if (!hash_pertenece(set, &tag)) return false;//El tag no se encuentra en el set
    line_t* linea = hash_obtener(set, &tag);
    if (linea->valido == 1 && linea->dirty == 1) 
    {
        if (operacion == 'R' )
        {
            hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
            hash_guardar(contador , "dirty-rmiss", hash_obtener(contador,"dirty-rmiss") + 1);
            hash_guardar(contador , "bytes-read", hash_obtener(contador,"bytes-read") + tamañio_bloque);
            hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1 + (2 * penalty));
        } else if (operacion == 'W')
        {
            hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
            hash_guardar(contador , "dirty-wmiss", hash_obtener(contador,"dirty-wmiss") + 1);
            hash_guardar(contador , "bytes-written", hash_obtener(contador,"bytes-written") + tamañio_bloque);
            hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1 + (2 * penalty));
        }
        return true;
    }
    else{
        if (operacion == 'R' )
        {
            hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
            hash_guardar(contador , "bytes-read", hash_obtener(contador,"bytes-read") + tamañio_bloque);
            hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1 + penalty);
        } else if (operacion == 'W')
        {
            hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
            hash_guardar(contador , "bytes-written", hash_obtener(contador,"bytes-written") + tamañio_bloque);
            hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1 + penalty);
        }
        return false;
    }
    
}
void añadir_tag(Cache* cache, uint32_t set_index, char tag, char OP){
    
    hash_t* set = cache->sets[set_index];
    if (hash_cantidad(set) == cache->tamanio_bloque){
        char tag_a_desalojar = obtener_tag_a_desalojar(cache, set_index);
        destruir_linea(hash_borrar(set, &tag_a_desalojar));
    }
    line_t* linea = malloc(sizeof(line_t));
    linea->tag = tag;
    linea->valido = 1;
    linea->dirty = 0;
    if (OP == 'W') linea->dirty = 1;
    hash_guardar(set, &tag, linea);
}

char obtener_tag_a_desalojar(Cache* cache, uint32_t set_index) {
    hash_t* set = cache->sets[set_index];
    char tag_a_desalojar = 0;  // Por defecto, inicializamos con 0 (reemplazable si se encuentra algo).
    uint32_t orden_minimo = UINT32_MAX;  // Máximo valor posible para encontrar el mínimo.
    
    hash_iter_t* iter = hash_iter_crear(set);
    while (!hash_iter_al_final(iter)) {
        const char* tag_actual = hash_iter_ver_actual(iter);
        line_t* linea = hash_obtener(set, tag_actual);
        if (linea->orden < orden_minimo) {
            orden_minimo = linea->orden;
            tag_a_desalojar = *tag_actual;
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return tag_a_desalojar;
}



/*
mi idea es que cada set esté asociado a un hash de lineas las claves son los tags y los valores son las lineas
donde cada linea tiene un tag, un bit de validez y un bit de dirty. Entonces
por cada set tengo un hash, y los sets los almaceno en un arreglo de sets.
Para contar cada hit, miss, dirty miss, dirty hit, etc. uso tmb un hash donde
las claves serían los contadores y los valores arrancan en 0 y se incrementan luego.
*/

// FUNCIONES AUXILIARES

hash_t** inicializar_sets(uint32_t num_sets, uint32_t lineas){
    hash_t** sets = malloc(num_sets * sizeof(hash_t*));
    for(int i = 0; i < num_sets; i++){
        sets[i] = hash_crear(destruir_linea);
    }
    return sets;
}
void destruir_sets(Cache* cache,hash_t** sets){
    for(int i = 0; i < cache->num_conjuntos; i++){
        hash_t* set  = sets[i];
        while (hash_cantidad(set) > 0){
            destruir_linea(hash_borrar(set, hash_iter_ver_actual(hash_iter_crear(set))));
        }
        hash_destruir(sets[i]);
    }
    free(sets);
}
hash_t* inicializar_contador() {
    hash_t* contador = hash_crear(destruir_int);
    const char* strings[CONTADORES_CANT] = {
        "loads",
        "stores",
        "rmiss",
        "wmiss",
        "dirty-rmiss",
        "dirty-wmiss",
        "bytes-read",
        "bytes-written",
        "time-w",
        "time-r"
    };
    int32_t num = 0;
    for(int i = 0; i < CONTADORES_CANT; i++){
        hash_guardar(contador, strings[i], &num);
    }

}
void destruir_linea(void* linea){
    free(linea);
}

void destruir_int(void* num){
    free(num);
}