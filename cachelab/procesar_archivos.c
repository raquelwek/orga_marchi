/********* MANEJO DE ARCHIVOS  *********/
//#include "funciones_cache.h"
#include "procesar_archivos.h"
#include <stdlib.h>
#include <string.h>

#define NUM_CAMPOS 5 // Número de campos en el archivo de traza

void procesar_linea(Cache* cache, char* linea, verboso_t* info) {
    // Variables para almacenar cada campo     
    char operacion;          
    uint32_t dir_acceso;   
    obtener_campos(linea, &operacion, &dir_acceso);
    
    uint32_t offset_set = calcular_offset(cache->num_conjuntos);
    uint32_t offset_block = calcular_offset(cache->tamanio_bloque);    

    uint32_t set_index = obtener_set(dir_acceso, offset_set, offset_block);
    uint32_t tag = obtener_tag(offset_set, offset_block, dir_acceso);
    /*
    char tagC[20];
    
    sprintf(tagC, "%x", tag);


    info -> indice_op = cache->indice_op;
    info -> cache_index= set_index;
    info ->cache_tag = &tagC;

    bool hit = hit_case(cache, set_index, &tagC, &operacion, info);
    if (!hit) {
        agg_tag(cache, set_index, &tagC, &operacion, info);
        bool es_dirty_miss = (info -> dirty_bit) == 1;
        
        if (es_dirty_miss) {
            dirty_miss_case(&operacion, cache->tamanio_bloque, cache->contador);
            printf("DIRTY MISS %d tag: %s %x\n", cache->indice_op, &tagC, tag);
        }else {
            miss_case(&operacion, cache->tamanio_bloque, cache->contador);
            printf("MISS %d %s %x \n", cache->indice_op, &tagC, tag);
        }
    }
    */
    info->indice_op = cache->indice_op;
    info->cache_index = set_index;
    info->cache_tag = tag;
    bool hit = hit_case(cache, set_index, tag, &operacion, info);
    if (!hit) agg_tag(cache, set_index, tag, &operacion, info);

}
uint32_t obtener_tag(uint32_t offset_set, uint32_t offset_block, uint32_t direccion) {
	uint32_t tag = direccion >> (offset_set + offset_block);
	return tag;
}
uint32_t obtener_set(uint32_t direccion, uint32_t offset_set_index, uint32_t offset_block){
    uint32_t  indexMask = (1 << (offset_block + offset_set_index)) - 1;
	uint32_t  setIndex = (direccion & indexMask) >> offset_block;
	return setIndex;
}
uint32_t calcular_offset(uint32_t n){
    uint32_t offset = 0;
	while (n != 1){
		offset += 1;
		n >>= 1;
	}
	return offset;
}
void obtener_campos(char* comando, char* operacion, uint32_t* direccionAcceso) {
    char simbolo_w = 'W';
    char simbolo_r = 'R';
	char* arrayComando[5];
	char* fragmento = strtok(comando, ": ");
	int i = 0;
	while (fragmento != NULL){
		arrayComando[i++] = fragmento;
		fragmento = strtok(NULL, ": ");
	}

    if(strcmp(arrayComando[1], "R")== 0){
        *operacion = simbolo_r;
    } else{
        *operacion = simbolo_w;
    }

	*direccionAcceso = strtoul(arrayComando[2], NULL, 0);
}

uint32_t obtenerTag(uint32_t direccion){
    return direccion >> 13;
}
uint32_t obtenerSet(uint32_t direccion){
    return (direccion >> 5) & 0x7F;
}
uint32_t obtenerBloqueOffset(uint32_t direccion){
    return direccion & 0x1F;
}