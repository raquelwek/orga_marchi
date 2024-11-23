#include "hash.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern char* strdup(const char*); // para que el compilador no marque error al usar strden
const size_t CAPACIDAD_INICIAL_TABLA = 115;
const double FACTOR_DE_CARGA_REDIMENSION_AUMENTAR = 0.7;
const double FACTOR_DE_CARGA_REDIMENSION_DISMINUIR = 0.1;
typedef enum estado {VACIO, BORRADO, OCUPADO} estado_t;
const size_t NUMERO_PARA_REDIMENSION_AUMENTAR = 2;
typedef struct campo campo_t;

struct campo{
	char* clave;
	void* dato;
	estado_t estado;
};

struct hash{
	campo_t* tabla;
	size_t cantidad;
	size_t borrados;
	size_t capacidad;
	hash_destruir_dato_t funcion_destruccion;
	
};

typedef struct indice indice_t;
struct indice{
	bool encontrado;
	size_t indice;
};

uint32_t funcion_hash(const uint8_t* key, size_t length){
  //https://en.wikipedia.org/wiki/Jenkins_hash_function
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}


campo_t* hash_crear_tabla(size_t capacidad){
	campo_t* tabla = malloc(capacidad*sizeof(campo_t));	
	if (!tabla) return NULL;
	for (size_t i = 0; i < capacidad; i++) tabla[i].estado = VACIO;
	return tabla;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash_nuevo = malloc(sizeof(hash_t));
	if (!hash_nuevo) return NULL;
	campo_t* tabla = hash_crear_tabla(CAPACIDAD_INICIAL_TABLA);
	if (!tabla){
		free(hash_nuevo);
		return NULL;
	}
	hash_nuevo->tabla = tabla;
	hash_nuevo->cantidad = 0;
	hash_nuevo->borrados = 0;
	hash_nuevo->capacidad = CAPACIDAD_INICIAL_TABLA;
	hash_nuevo->funcion_destruccion = destruir_dato;
	return hash_nuevo;
}

// Busca la clave una tabla pasada por parámetro, devuelve un struct con dos miembros, encontrado que es un bool que marca si fue encontrada
// la clave y un indice que es un lugar vacío de la tabla donde podría insertarse correctamente un elemento.
indice_t* hash_buscar_clave(campo_t* tabla, size_t capacidad ,const char* clave){
	indice_t* indice_buscado = malloc(sizeof(indice_t));
	if (!indice_buscado) return NULL;
	size_t indice = (funcion_hash((const uint8_t*)clave, strlen(clave)))% capacidad;
	while (tabla[indice].estado != VACIO){
		if ( tabla[indice].estado == OCUPADO && strcmp(clave, tabla[indice].clave) == 0){
			indice_buscado->encontrado = true;
			indice_buscado->indice = indice;
			return indice_buscado;
		}
		if (indice == capacidad -1){
			indice = 0;
			continue;
		}
		indice++;
	}
	indice_buscado->encontrado = false;
	indice_buscado->indice = indice;
	return indice_buscado;
}



// Redimensiona la tabla aumentado su capacidad a la capacidad deseada y pasando los elementos a una tabla nueva.
bool hash_redimensionar_tabla(hash_t* hash, size_t capacidad_nueva){
	indice_t* indice_buscado;
	campo_t* tabla_nueva = hash_crear_tabla(capacidad_nueva);
	if (!tabla_nueva) return false;
	for (size_t i = 0; i < hash->capacidad; i++){ 
		if (hash->tabla[i].estado ==  OCUPADO){
			indice_buscado = hash_buscar_clave(tabla_nueva, capacidad_nueva, hash->tabla[i].clave);
			if (!indice_buscado){
				free(tabla_nueva);
				free(indice_buscado);
			}
			tabla_nueva[indice_buscado->indice].estado = OCUPADO;
			tabla_nueva[indice_buscado->indice].dato = hash->tabla[i].dato;
			tabla_nueva[indice_buscado->indice].clave = hash->tabla[i].clave;
			free(indice_buscado);
		}
	}
	hash->capacidad =  capacidad_nueva;
	hash->borrados =  0;
	free(hash->tabla);	
	hash->tabla = tabla_nueva;
	return true;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){ 
	if ((double)(hash->cantidad+hash->borrados)/(double)hash->capacidad >= FACTOR_DE_CARGA_REDIMENSION_AUMENTAR){
		bool nueva_tabla = hash_redimensionar_tabla(hash, hash->capacidad*NUMERO_PARA_REDIMENSION_AUMENTAR);
		if (!nueva_tabla) return false;
	}	
	indice_t* indice_buscado = hash_buscar_clave(hash->tabla, hash->capacidad, clave);
	if(!indice_buscado) return false;
	
	if (indice_buscado->encontrado){
		hash_destruir_dato_t funcion_destruccion = hash->funcion_destruccion;
		if (funcion_destruccion) funcion_destruccion(hash->tabla[indice_buscado->indice].dato);

	}
	else{
		char* clave_copia =  strdup(clave);          
		if (!clave_copia){
			free(indice_buscado);
			return false;
		}
		hash->tabla[indice_buscado->indice].clave = clave_copia;
		hash->tabla[indice_buscado->indice].estado = OCUPADO;			
		hash->cantidad++;
	}
	hash->tabla[indice_buscado->indice].dato = dato;
	free(indice_buscado);	
	return true;
}


void *hash_borrar(hash_t *hash, const char *clave){
	
	if ((double)(hash->cantidad+hash->borrados)/(double)(hash->capacidad) <= FACTOR_DE_CARGA_REDIMENSION_DISMINUIR){
		bool nueva_tabla = hash_redimensionar_tabla(hash, hash->capacidad/NUMERO_PARA_REDIMENSION_AUMENTAR);
		if (!nueva_tabla)return false;
	}
	indice_t* indice_buscado = hash_buscar_clave(hash->tabla, hash->capacidad, clave);
	if(!indice_buscado) return NULL;
	if (indice_buscado->encontrado){
		hash->tabla[indice_buscado->indice].estado =  BORRADO;
		free(hash->tabla[indice_buscado->indice].clave); 
		hash->borrados++;
		hash->cantidad--;
		size_t indice = indice_buscado->indice;
		free(indice_buscado);		
		return hash->tabla[indice].dato;
	}
	else{
		 free(indice_buscado); 
		 return NULL;
    }
}


void *hash_obtener(const hash_t *hash, const char *clave){
	indice_t* indice_buscado = hash_buscar_clave(hash->tabla, hash->capacidad, clave);
	void* resultado = indice_buscado->encontrado ? hash->tabla[indice_buscado->indice].dato : NULL;
	free(indice_buscado);
	return resultado;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
	indice_t* indice_buscado = hash_buscar_clave(hash->tabla, hash->capacidad, clave);
	bool resultado = indice_buscado->encontrado ? true : false;
	free(indice_buscado);
	return resultado;
}


size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}


void hash_destruir(hash_t *hash){
	hash_destruir_dato_t  funcion_destruccion = hash->funcion_destruccion;
	for (size_t i = 0; i < hash->capacidad; i++){
		if (hash->tabla[i].estado == OCUPADO){
			if(funcion_destruccion) funcion_destruccion(hash->tabla[i].dato);
			free(hash->tabla[i].clave);	
		}	
	}
	free(hash->tabla);
	free(hash);
}

/* Iterador del hash */
struct hash_iter{
	const hash_t* hash;
	size_t posicion;
};

// Auxiliar para que no quede tan larga la funcion "hash__iter_t *hash_iter_crear()"


// Pre : recibe el iterador que fue creado anteriormente
// Post : Avanza el iterador hasta el siguiente campo, retorna true si
// se logro posicionar en un campo ocupado o el final del hash
bool hash_iter_actualizar_siguiente_pos(hash_iter_t* iter) {
	for ( ;iter->posicion <= iter->hash->capacidad; iter->posicion++) {
		if (iter->posicion == iter->hash->capacidad || iter->hash->tabla[iter->posicion].estado == OCUPADO) {
			return true;
		}
	}
	return false;
}

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t * iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->hash = hash;
	iter->posicion = 0;
	if(!hash_iter_actualizar_siguiente_pos(iter)){
		iter->posicion = iter->hash->capacidad;
	}
	return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return false;

	iter->posicion++;
	return hash_iter_actualizar_siguiente_pos(iter);
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char* hash_iter_ver_actual(const hash_iter_t *iter){
	if(hash_iter_al_final(iter)){
		return NULL;
	}
	return iter->hash->tabla[iter->posicion].clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->hash->capacidad == iter->posicion;
}
// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter){
	free(iter);
}

