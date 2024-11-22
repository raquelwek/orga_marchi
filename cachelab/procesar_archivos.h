//#ifndef FUNCIONES_CACHE_H
//#define FUNCIONES_CACHE_H
#ifndef PROCESAR_ARCHIVOS_H
#define PROCESAR_ARCHIVOS_H
#include "funciones_cache.h"// Implementación de las funciones de la caché


void procesar_linea(Cache* cache, char* linea);

uint32_t obtener_tag(uint32_t offset_set, uint32_t offset_block, uint32_t direccion);

uint32_t obtenter_set(uint32_t direccion, uint32_t offset_set_index, uint32_t offset_block);

char* obtener_campos(char* linea);

uint32_t calcular_offset(uint32_t n);

uint32_t obtenerTag(uint32_t direccion);

uint32_t obtenerSet(uint32_t direccion);

uint32_t obtenerBloqueOffset(uint32_t direccion);

//#endif // FUNCIONES_CACHE_H
#endif //PROCESAR_ARCHIVOS_H
