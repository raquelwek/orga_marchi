/********* MANEJO DE ARCHIVOS  *********/
//#include "funciones_cache.h"
#include "procesar_archivos.h"
#include <stdlib.h>
#include <string.h>

#define NUM_CAMPOS 5 // Número de campos en el archivo de traza
const char* SEPARADOR = ": "; // Separador de campos en el archivo de traza
const char* READING_OP = "R";

void procesar_linea(Cache* cache, char* linea, verboso_t* info) {
    // Variables para almacenar cada campo     
    op_t operacion;          
    uint32_t dir_acceso;   
    obtener_campos(linea, &operacion, &dir_acceso);
    
    uint32_t offset_set = calcular_offset(cache->num_conjuntos);
    uint32_t offset_block = calcular_offset(cache->tamanio_bloque);    

    uint32_t set_index = obtener_set(dir_acceso, offset_set, offset_block);
    uint32_t tag = obtener_tag(offset_set, offset_block, dir_acceso);
    
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
void obtener_campos(char* comando, op_t* operacion, uint32_t* direccion) {
	char* campos[5];
	char* campo = strtok(comando, SEPARADOR);

    for (int i = 1; i <= NUM_CAMPOS && campo != NULL; i++){
        campos[i-1] = campo;
        campo = strtok(NULL, SEPARADOR);
    }
	
    if(strcmp(campos[1], READING_OP)== 0){
        *operacion = READING;
    } else{
        *operacion = WRITTING;
    }

	*direccion = strtoul(campos[2], NULL, 0);
}

