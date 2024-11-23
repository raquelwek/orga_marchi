/**********  FUNCIONES PARA LA CACHE *********/
#include "funciones_cache.h"
#define ADDRESS_SIZE 32     // Tamaño de dirección en bits
#define CONTADORES_CANT 10
#define SIMBOLO_WRITE 'W'
#define SIMBOLO_READ 'R'

Cache* crear_cache(uint32_t C, uint32_t E, uint32_t S, uint32_t B){
    Cache* cache = malloc(sizeof(Cache));
    cache->tamanio_cache = C;
    cache->tamanio_bloque = B;
    cache->num_conjuntos = S;
    cache->num_lineas = E;
    cache->contador = inicializar_contador();
    cache->sets = inicializar_sets(S, E);
    return cache;
}

void destruir_cache(Cache* cache){
    destruir_sets(cache, cache->sets);
    hash_destruir(cache->contador);
    free(cache);
}

bool hit_case(Cache* cache, uint32_t set_index, char tag, char operacion){
    hash_t* set = cache->sets[set_index];
    hash_t* contador = cache->contador;  
    if (!hash_pertenece(set, &tag)) return false;//El tag no se encuentra en el set
    line_t* linea = hash_obtener(set, &tag);
    if (es_linea_valida(linea)){
        if (operacion == SIMBOLO_READ ) {
            hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
            hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1);
        } else if (operacion == SIMBOLO_WRITE) {
            linea->dirty = 1;
            hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
            hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1);
        }
        linea->orden++;
        cache->last_used++;
        return true;
        
    } 
    return false;
}

// miss_case contabiliza los contadores en el caso de miss
// post: se actualizan los contadores de la cache
void miss_case(char operacion, uint32_t tam_block, hash_t* contador) {
     
    if (operacion == SIMBOLO_READ ) {
        hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
        hash_guardar(contador , "rmiss", hash_obtener(contador,"rmiss") + 1);
        hash_guardar(contador , "bytes-read", hash_obtener(contador,"bytes-read") + tam_block);
        hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1 + PENALTY);
    } else if (operacion == SIMBOLO_WRITE) {
        hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
        hash_guardar(contador , "wmiss", hash_obtener(contador,"wmiss") + 1);
        hash_guardar(contador , "bytes-written", hash_obtener(contador,"bytes-written") + tam_block);
        hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1 + PENALTY);
    }
        

}
void dirty_miss_case(char operacion, uint32_t tam_block, hash_t* contador) {
   
    if (operacion == SIMBOLO_READ ) {
        hash_guardar(contador , "loads", hash_obtener(contador,"loads") + 1);
        hash_guardar(contador , "dirty-rmiss", hash_obtener(contador,"dirty-rmiss") + 1);
        hash_guardar(contador , "bytes-read", hash_obtener(contador,"bytes-read") + tam_block);
        hash_guardar(contador , "time-r", hash_obtener(contador,"time-r") + 1 + (2 * PENALTY));
    } else if (operacion == SIMBOLO_WRITE) {
        hash_guardar(contador , "stores", hash_obtener(contador,"stores") + 1);
        hash_guardar(contador , "dirty-wmiss", hash_obtener(contador,"dirty-wmiss") + 1);
        hash_guardar(contador , "bytes-written", hash_obtener(contador,"bytes-written") + tam_block);
        hash_guardar(contador , "time-w", hash_obtener(contador,"time-w") + 1 + (2 * PENALTY));
    }

    
}
bool es_linea_valida(line_t* linea){
    return linea->valido == 1;
}
bool set_tiene_espacio(hash_t* set, uint32_t lineas_por_set){
    return hash_cantidad(set) < lineas_por_set;
}


bool agg_tag(Cache* cache, uint32_t set_index, char tag, char OP) {

    hash_t* set = cache->sets[set_index];
    line_t* linea = malloc(sizeof(line_t));
    bool es_dirty_miss = false;
    if (!set_tiene_espacio(set, cache->num_lineas)){
        tag = obtener_tag_a_desalojar(cache, set_index);

        line_t* linea_target = hash_obtener(set, &tag);
        linea -> numero_linea = linea_target->numero_linea;
        es_dirty_miss = (linea->dirty) == 1;
        destruir_linea(hash_borrar(set, &tag));
    }
    
    linea->tag = tag;
    linea->valido = 1;
    linea->dirty = 0;
    linea->orden = cache->last_used +1;
    cache->last_used++;
    if (OP == SIMBOLO_WRITE) linea->dirty = 1;
    hash_guardar(set, &tag, linea);
    return es_dirty_miss;
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