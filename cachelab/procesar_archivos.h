//#ifndef FUNCIONES_CACHE_H
//#define FUNCIONES_CACHE_H
#ifndef PROCESAR_ARCHIVOS_H
#define PROCESAR_ARCHIVOS_H
#include "funciones_cache.h"// Implementación de las funciones de la caché

/* procesar_linea recibe una línea de un archivo de traza 
 * actualizando los atributos de la caché según la operación
 */
void procesar_linea(Cache* cache, char* linea,  verboso_t* info);

/* obtener_tag recibe un offset de set, un offset de bloque y una dirección
 * y devuelve el tag correspondiente a la dirección
 */
uint32_t obtener_tag(uint32_t offset_set, uint32_t offset_block, uint32_t direccion);

/* obtener_set recibe una dirección, un offset de set y un offset de bloque
 * y devuelve el set correspondiente a la dirección
 */
uint32_t obtener_set(uint32_t direccion, uint32_t offset_set_index, uint32_t offset_block);

/* obtener_campos recibe una línea de un archivo de traza y devuelve un arreglo
 * con los campos de la línea
 */
char** obtener_campos(char* linea);

/* calcular_offset recibe un cantidad n y devuelve el offset correspondiente
    para acceder al comienzo de un bloque de tamaño n
 */
uint32_t calcular_offset(uint32_t n);


uint32_t obtenerTag(uint32_t direccion);

uint32_t obtenerSet(uint32_t direccion);

uint32_t obtenerBloqueOffset(uint32_t direccion);

//#endif // FUNCIONES_CACHE_H
#endif //PROCESAR_ARCHIVOS_H
