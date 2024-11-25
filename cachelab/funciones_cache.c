/**********  FUNCIONES PARA LA CACHE *********/
#include <stdlib.h>
#include "funciones_cache.h"
#define ADDRESS_SIZE 32     // Tamaño de dirección en bits
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


Cache* crear_cache(uint32_t C, uint32_t E, uint32_t S, uint32_t B){
    Cache* cache = malloc(sizeof(Cache));
    if (cache == NULL) {
        perror("Error al asignar memoria");
        exit(1);
    }
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

bool hit_case(Cache* cache, uint32_t set_index, char* tag, char* operacion, verboso_t* info){
    char caso[] = "1";
    hash_t* set = cache->sets[set_index];
    hash_t* contador = cache->contador;  
    if (!hash_pertenece(set, tag)) return false;//El tag no se encuentra en el set

    line_t* linea = hash_obtener(set, tag);
    if (es_linea_valida(linea)){
        if (strcmp(operacion, "R") == 0) {
            uint32_t* ptr_nuevo_valor = malloc(sizeof(uint32_t));
            if (!ptr_nuevo_valor) exit(EXIT_FAILURE);
            *ptr_nuevo_valor = *(uint32_t*)hash_obtener(contador, strings[0]) + 1;
            hash_guardar(contador, strings[0], ptr_nuevo_valor);

            // Actualizar "time_r"
            uint32_t* ptr_nuevo_time_r = malloc(sizeof(uint32_t));
            if (!ptr_nuevo_time_r) exit(EXIT_FAILURE);
            *ptr_nuevo_time_r = *(uint32_t*)hash_obtener(contador, strings[9]) + 1;
            hash_guardar(contador, strings[9], ptr_nuevo_time_r);

        } else if (strcmp(operacion, "W") == 0) {
            linea->dirty = 1;

            // Actualizar "stores"
            uint32_t* ptr_nuevo_stores = malloc(sizeof(uint32_t));
            if (!ptr_nuevo_stores) exit(EXIT_FAILURE);
            *ptr_nuevo_stores = *(uint32_t*)hash_obtener(contador, strings[1]) + 1;
            hash_guardar(contador, strings[1], ptr_nuevo_stores);

            // Actualizar "time_w"
            uint32_t* ptr_nuevo_time_w = malloc(sizeof(uint32_t));
            if (!ptr_nuevo_time_w) exit(EXIT_FAILURE);
            *ptr_nuevo_time_w = *(uint32_t*)hash_obtener(contador, strings[8]) + 1;
            hash_guardar(contador, strings[8], ptr_nuevo_time_w);

        }
        linea->last_used = cache -> indice_op;
        campos_verboso(info, linea, &caso);
        return true;
        
    } 
    return false;
}
void campos_verboso(verboso_t* v, line_t*  linea, char* caso){
    v -> case_identifier = caso;
    v -> cache_line = linea -> numero_linea;
    v -> line_tag = linea -> tag;
    v -> valid_bit = linea -> valido;
    v -> dirty_bit = linea ->dirty;
    v -> indice_op = linea -> last_used;
}

// miss_case contabiliza los contadores en el caso de miss
// post: se actualizan los contadores de la cache
void miss_case(char* operacion, uint32_t tam_block, hash_t* contador) {
     
    if (strcmp(operacion, "W") == 0) {
        // Operación de lectura
        uint32_t* ptr_nuevo_loads = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_loads) exit(EXIT_FAILURE); // Manejo de errores por malloc
        *ptr_nuevo_loads = *(uint32_t*)hash_obtener(contador, strings[0]) + 1;
        hash_guardar(contador, strings[0], ptr_nuevo_loads);

        // Guardar nuevo valor de "rmiss"
        uint32_t* ptr_nuevo_rmiss = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_rmiss) exit(EXIT_FAILURE);
        *ptr_nuevo_rmiss = *(uint32_t*)hash_obtener(contador, strings[2]) + 1;
        hash_guardar(contador, strings[2], ptr_nuevo_rmiss);

        // Guardar nuevo valor de "bytes_read"
        uint32_t* ptr_nuevo_bytes_read = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_bytes_read) exit(EXIT_FAILURE);
        *ptr_nuevo_bytes_read = *(uint32_t*)hash_obtener(contador, strings[6]) + tam_block;
        hash_guardar(contador, strings[6], ptr_nuevo_bytes_read);

        // Guardar nuevo valor de "time_r"
        uint32_t* ptr_nuevo_time_r = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_time_r) exit(EXIT_FAILURE);
        *ptr_nuevo_time_r = *(uint32_t*)hash_obtener(contador, strings[9]) + 1 + PENALTY;
        hash_guardar(contador, strings[9], ptr_nuevo_time_r);

    } else if (strcmp(operacion, "R") == 0) {
        // Operación de escritura
        uint32_t* ptr_nuevo_stores = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_stores) exit(EXIT_FAILURE);
        *ptr_nuevo_stores = *(uint32_t*)hash_obtener(contador, strings[1]) + 1;
        hash_guardar(contador, strings[1], ptr_nuevo_stores);

        uint32_t* ptr_nuevo_wmiss = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_wmiss) exit(EXIT_FAILURE);
        *ptr_nuevo_wmiss = *(uint32_t*)hash_obtener(contador, strings[3]) + 1;
        hash_guardar(contador, strings[3], ptr_nuevo_wmiss);

        uint32_t* ptr_nuevo_bytes_written = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_bytes_written) exit(EXIT_FAILURE);
        *ptr_nuevo_bytes_written = *(uint32_t*)hash_obtener(contador, strings[7]) + tam_block;
        hash_guardar(contador, strings[7], ptr_nuevo_bytes_written);

        uint32_t* ptr_nuevo_time_w = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_time_w) exit(EXIT_FAILURE);
        *ptr_nuevo_time_w = *(uint32_t*)hash_obtener(contador, strings[8]) + 1 + PENALTY;
        hash_guardar(contador, strings[8], ptr_nuevo_time_w);
    }
        

}
void dirty_miss_case(char* operacion, uint32_t tam_block, hash_t* contador) {
   
    if (strcmp(operacion, "R") == 0) {
        uint32_t* ptr_nuevo_loads = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_loads) exit(EXIT_FAILURE);
        *ptr_nuevo_loads = *(uint32_t*)hash_obtener(contador, strings[0]) + 1;
        hash_guardar(contador, strings[0], ptr_nuevo_loads);

        // Actualizar "dirty_rmiss"
        uint32_t* ptr_nuevo_dirty_rmiss = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_dirty_rmiss) exit(EXIT_FAILURE);
        *ptr_nuevo_dirty_rmiss = *(uint32_t*)hash_obtener(contador, strings[4]) + 1;
        hash_guardar(contador, strings[4], ptr_nuevo_dirty_rmiss);

        // Actualizar "bytes_read"
        uint32_t* ptr_nuevo_bytes_read = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_bytes_read) exit(EXIT_FAILURE);
        *ptr_nuevo_bytes_read = *(uint32_t*)hash_obtener(contador, strings[6]) + tam_block;
        hash_guardar(contador, strings[6], ptr_nuevo_bytes_read);

        // Actualizar "time_r"
        uint32_t* ptr_nuevo_time_r = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_time_r) exit(EXIT_FAILURE);
        *ptr_nuevo_time_r = *(uint32_t*)hash_obtener(contador, strings[9]) + 1 + (2 * PENALTY);
        hash_guardar(contador, strings[9], ptr_nuevo_time_r);

    } else if (strcmp(operacion, "W") == 0) {
        // Actualizar "stores"
        uint32_t* ptr_nuevo_stores = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_stores) exit(EXIT_FAILURE);
        *ptr_nuevo_stores = *(uint32_t*)hash_obtener(contador, strings[1]) + 1;
        hash_guardar(contador, strings[1], ptr_nuevo_stores);

        // Actualizar "dirty_wmiss"
        uint32_t* ptr_nuevo_dirty_wmiss = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_dirty_wmiss) exit(EXIT_FAILURE);
        *ptr_nuevo_dirty_wmiss = *(uint32_t*)hash_obtener(contador, strings[5]) + 1;
        hash_guardar(contador, strings[5], ptr_nuevo_dirty_wmiss);

        // Actualizar "bytes_written"
        uint32_t* ptr_nuevo_bytes_written = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_bytes_written) exit(EXIT_FAILURE);
        *ptr_nuevo_bytes_written = *(uint32_t*)hash_obtener(contador, strings[7]) + tam_block;
        hash_guardar(contador, strings[7], ptr_nuevo_bytes_written);

        // Actualizar "time_w"
        uint32_t* ptr_nuevo_time_w = malloc(sizeof(uint32_t));
        if (!ptr_nuevo_time_w) exit(EXIT_FAILURE);
        *ptr_nuevo_time_w = *(uint32_t*)hash_obtener(contador, strings[8]) + 1 + (2 * PENALTY);
        hash_guardar(contador, strings[8], ptr_nuevo_time_w);
    }

    
}
bool es_linea_valida(line_t* linea){
    return linea->valido == 1;
}
bool set_tiene_espacio(hash_t* set, uint32_t lineas_por_set){
    return hash_cantidad(set) < lineas_por_set;
}

void agg_tag(Cache* cache, uint32_t set_index, char* tag, char* OP, verboso_t* info){
    hash_t* set = cache->sets[set_index];
    line_t* linea = malloc(sizeof(line_t));
    char casoA[] = "2a";
    char casoB[] = "2b";
    char* caso = casoA;

    if (!set_tiene_espacio(set, cache->num_lineas)){
        line_t* linea_target = obtener_linea_a_desalojar(cache, set_index);
        linea -> numero_linea = linea_target->numero_linea;
        linea->last_used = linea_target ->last_used;
        // preparar info:
        
        if (linea_target -> dirty == 1) caso = casoB;
        campos_verboso(info,linea_target, *caso);        

        destruir_linea(hash_borrar(set, tag));

    }else {
        // seleccionar línea  no válida cuyo índice sea  el menor
        //si la cache tenía espacio por llenar, se asigna el número de línea siguiente
        linea->numero_linea = hash_cantidad(set);
        linea->last_used = cache ->indice_op; // el bloque se usa por primera vez

        info -> case_identifier = *caso;
        info -> cache_line = linea -> numero_linea;
        info -> valid_bit = 0;
        info -> dirty_bit = 0;
        info -> line_tag = -1;
        info -> indice_op = linea -> last_used;
    }
    
    linea->tag = tag;
    linea->valido = 1;
    linea->dirty = 0;
    if (strcmp(OP, "W")==0) linea->dirty = 1;
    hash_guardar(set, tag, linea);
    return;
}

line_t* valida_menos_usada(Cache* cache, uint32_t set_index) {
    hash_t* set = cache->sets[set_index];
    line_t* linea_a_desalojar = NULL;
    uint32_t orden_minimo = cache->indice_op;  // Máximo valor posible para encontrar el mínimo.
    
    hash_iter_t* iter = hash_iter_crear(set);
    while (!hash_iter_al_final(iter)) {
        const char* tag_actual = hash_iter_ver_actual(iter);
        line_t* linea = hash_obtener(set, tag_actual);
        
        if (linea->last_used < orden_minimo && linea->valido == 1){
            orden_minimo = linea->last_used;
            linea_a_desalojar = linea;
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return linea_a_desalojar;
}
line_t* obtener_linea_a_desalojar(Cache* cache, uint32_t set_index) {
    return valida_menos_usada(cache, set_index);
}
/*
line_t* invalida_de_menor_indice(Cache* cache, uint32_t set_index) {
    hash_t* set = cache->sets[set_index];
    line_t* linea_a_desalojar = NULL;
    uint32_t indice_minimo = cache->num_lineas - 1;  // Máximo valor posible para encontrar el mínimo.
    
    hash_iter_t* iter = hash_iter_crear(set);
    while (!hash_iter_al_final(iter)) {
        const char* tag_actual = hash_iter_ver_actual(iter);
        line_t* linea = hash_obtener(set, tag_actual);
        
        if (linea->numero_linea < indice_minimo && linea->valido == 0){
            indice_minimo = linea->numero_linea;
            linea_a_desalojar = linea;
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return linea_a_desalojar;
}
*/

// FUNCIONES AUXILIARES
hash_t** inicializar_sets(uint32_t num_sets, uint32_t lineas) {
    hash_t** sets = malloc(num_sets * sizeof(hash_t*));
    for (uint32_t i = 0; i < num_sets; i++) {
        sets[i] = hash_crear(destruir_linea);
    }
    return sets;
}

void destruir_sets(Cache* cache,hash_t** sets){
    for(uint32_t i = 0; i < cache->num_conjuntos; i++){
        hash_t* set  = sets[i];
        hash_destruir(sets[i]);
    }
    free(sets);
}
hash_t* inicializar_contador() {
    hash_t* contador = hash_crear(destruir_int);
    
    for(int i = 0; i < CONTADORES_CANT; i++){
        uint32_t* num_ptr = malloc(sizeof(uint32_t));
        if (num_ptr == NULL) {
            hash_destruir(contador);
            return NULL;
        }
        *num_ptr = 0;
        hash_guardar(contador, strings[i], num_ptr);
    }
    return contador;
}
void destruir_linea(line_t* linea){
    free(linea);
}

void destruir_int(void* num){
    free(num);
}