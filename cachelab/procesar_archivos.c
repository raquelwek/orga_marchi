/********* MANEJO DE ARCHIVOS  *********/
//#include "funciones_cache.h"
#include "procesar_archivos.h"
#include <stdlib.h>
#include <string.h>

#define NUM_CAMPOS 5 // Número de campos en el archivo de traza

void procesar_linea(Cache* cache, char* linea, verboso_t* info) {
    // Variables para almacenar cada campo
    uint32_t pc;      
    char operacion;          
    uint32_t dir_acceso; 
    uint32_t cant_bytes;     
    uint32_t data_readed;  
    char** campos = obtener_campos(linea);
    pc = campos[0];
    operacion = campos[1];
    dir_acceso = (uint32_t)atoi(campos[2]);;
    cant_bytes = campos[3];
    data_readed = campos[4];

    uint32_t offset_set= calcular_offset(cache->num_conjuntos);
    uint32_t offset_block = calcular_offset(cache->tamanio_bloque);    

    uint32_t set_index = obtener_set(dir_acceso, offset_set, offset_block);
    uint32_t tag = obtener_tag(offset_set, offset_block, dir_acceso);
    char* tagC = (char*)tag;

    info -> indice_op = cache->indice_op;
    info -> cache_index= set_index;
    info ->cache_tag = tagC;

    bool hit = hit_case(cache, set_index, tagC, operacion, info);
    if (!hit) {
        agg_tag(cache, set_index, tagC, operacion, info);
        bool es_dirty_miss = (info -> dirty_bit) == 1;

        if (es_dirty_miss) {
            dirty_miss_case(operacion, cache->tamanio_bloque, cache->contador);
        }else {
            miss_case(operacion, cache->tamanio_bloque, cache->contador);
        }
    }

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
char** obtener_campos(char* linea) {
    // Crear un arreglo estático para guardar los campos
    static char* arrayCampos[NUM_CAMPOS]; // Usa `static` para devolverlo sin problemas
    char* fragmento = strtok(linea, " ");
    int i = 0;

    while (fragmento != NULL && i < NUM_CAMPOS) {
        arrayCampos[i++] = fragmento; // Guardar puntero al fragmento
        fragmento = strtok(NULL, " ");
    }
    return arrayCampos;
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