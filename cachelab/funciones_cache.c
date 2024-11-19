/**********  FUNCIONES PARA LA CACHE *********/
#include "funciones_cache.h"
#define ADDRESS_SIZE 32     // Tamaño de dirección en bits


/*

typedef struct {
    int tamanio_cache;      // Tamaño total de la caché (en bytes)
    int tamanio_bloque;     // Tamaño de cada bloque (en bytes)
    int num_conjuntos;      // Número de conjuntos en la caché
    hash_t* contador;
    hash_t** sets;
} Cache;


typedef struct line{
    int valido;             // 0: invalido, 1: valido
    int dirty;              // 0: no modificado, 1: modificado
    int tag;
    int orden;              // para LRU
} line_t;

mi idea es que cada set esté asociado a un hash de lineas las claves son los tags y los valores son las lineas
donde cada linea tiene un tag, un bit de validez y un bit de dirty. Entonces
por cada set tengo un hash, y los sets los almaceno en un arreglo de sets.
Para contar cada hit, miss, dirty miss, dirty hit, etc. uso tmb un hash donde
las claves serían los contadores y los valores arrancan en 0 y se incrementan luego.
*/