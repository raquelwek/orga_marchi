#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "funciones_cache2.h"
#include "procesar_archivos.h"

/*  *********************
    * PROGRAMA CACHESIM *
    *********************
*/

typedef struct {
    char* archivo_traza;
    int tamano_cache;
    int asociatividad;
    int numero_sets;
    int rango_n;
    int rango_m;
    bool modo_verboso;
} Argumentos;

void registrar_procesada(verboso_t* info, uint32_t indice, uint32_t asociatividad){
    printf("%d %s %x %x %d", info->indice_op, info->case_identifier, info->cache_index, info->cache_tag, info->cache_line);
    if (info->line_tag == -1){
        printf(" %d", info->line_tag);
    }else {
        printf(" %x", info->line_tag);
    }
    printf(" %d %d", info->valid_bit, info->dirty_bit);
    if (asociatividad > 1 ){
        printf(" %d", info->last_used);
    }
    printf("\n");
}
bool verificar_argumentos(int argc) {
    return (argc == 5 || argc == 8);   
}

bool verificar_archivo(char* archivo){
    if (archivo == NULL) return false; // Verifico que el puntero no sea nulo
    FILE *file = fopen(archivo, "r");
    if (file) {
        fclose(file); 
        return true;
    }
    return false; 
}

bool verificar_potencia(int numero){
    return numero > 0 && (numero & (numero - 1)) == 0;
}

bool verificar_rango(int n, int m){
    return (n >= 0 && m >= n);
}

bool verificar_condiciones(int argc, char *argv[], bool modo_verboso){
    bool valido = (verificar_argumentos(argc) && verificar_archivo(argv[1]));
    if(modo_verboso && argc == 8){
        int rango_n = atoi(argv[6]);
        int rango_m = atoi(argv[7]);
        bool verboso = (verificar_potencia(rango_n) && verificar_potencia(rango_m) && verificar_rango(rango_n,rango_m));
        return (valido && verboso);
    }

    return valido;
        
     
}
///.trazas/adpcm.xex 2048 2 64 -v 0 1500


void asignar_argumentos(int argc, char *argv[], Argumentos* args){
    // Asignar valores comunes
    args->archivo_traza = argv[1];
    args->tamano_cache = atoi(argv[2]);
    args->asociatividad = atoi(argv[3]);
    args->numero_sets = atoi(argv[4]);
    args->modo_verboso = false;
    args->rango_n = 0; // Valores por defecto
    args->rango_m = 0;

    // Verificar modo verboso y rangos
    if (argc >= 6 && (strcmp(argv[5], "-v") == 0)) {
        args->modo_verboso = true;
        args->rango_n = atoi(argv[6]);
        args->rango_m = atoi(argv[7]);
    }

}

// Función para procesar el archivo de traza
void procesar_archivo(char* archivo_entrada, Cache* cache, bool modo_verboso, int n, int m) {
    FILE *file = fopen(archivo_entrada, "r");
    if (!file) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", archivo_entrada);
        return;
    }

    char linea[256];
    int indice = -1; // Indice para contar las líneas
    
    verboso_t* info = malloc(sizeof(verboso_t));

    while (fgets(linea, sizeof(linea), file)) {
        indice++; // Incrementar el índice al leer una nueva línea

        procesar_linea(cache, linea, info);
        cache -> indice_op++;
        
        // En modo verboso, procesar solo registrar las líneas dentro del rango [n, m]
        if (modo_verboso && (indice >= n && indice <= m)) {
            registrar_procesada(info, cache->indice_op, cache -> num_lineas);
        }
        
    }
    fclose(file);
    free(info);
    
}
   
uint32_t calcular_tambloque(int tamano_cache,int numero_sets,int asociatividad){
    return (tamano_cache / (numero_sets * asociatividad));
}

void imprimir_metricas(Cache* cache) {
    
    contador_t* contador = cache->contador;
    uint32_t total_accesses = contador -> loads + contador -> stores;
    uint32_t total_misses = contador->rmiss+ contador->wmiss;
    uint32_t total_dirty_misses = contador->dirty_rmiss + contador->dirty_wmiss;

    uint32_t tamanio_cache = cache->tamanio_cache/1000;//tam cache en KB
    double dirty_miss_rate = (double)(total_misses) / (double)total_accesses;

    // Imprimimos las métricas
    if (cache->num_lineas == 1) {
        printf("direct-mapped, %d sets, size = %dKB\n",cache->num_conjuntos, tamanio_cache);
    } else{
        printf("%d-way, %d sets, size = %dKB\n", cache->num_lineas, cache->num_conjuntos, tamanio_cache);
    }
    printf("loads %d stores %d total %d\n", contador->loads,contador->stores, total_accesses);
    printf("rmiss %d wmiss %d total %d\n", contador->rmiss, contador->wmiss, total_misses);
    printf("dirty rmiss %d dirty wmiss %d\n", contador->dirty_rmiss,contador->dirty_wmiss);
    printf("bytes read %d bytes written %d\n", contador->bytes_read, contador->bytes_written);
    printf("read time %d write time %d\n", contador->time_r, contador->time_w);
    printf("miss rate %.6f\n", dirty_miss_rate);
}

// Función principal
int main(int argc, char *argv[]) {
      // Estructura para almacenar los argumentos procesados
    Argumentos* args = malloc(sizeof(Argumentos));
    args->archivo_traza = NULL;
    args->tamano_cache = 0;
    args->asociatividad = 0;
    args->numero_sets = 0;
    args->modo_verboso = false;
    args->rango_n = 0;
    args->rango_m = 0;

    

    // Verificar condiciones
    if (!verificar_condiciones(argc, argv, false)) {
        fprintf(stderr, "Error: condiciones inválidas para los argumentos.\n");
        return 1;
    }

    // Asignar argumentos
    asignar_argumentos(argc, argv, args);

    // Crear la caché con los parámetros predefinidos
    uint32_t bloque = calcular_tambloque(args->tamano_cache,args->numero_sets,args->asociatividad);
    Cache* cache = crear_cache(args->tamano_cache, args->asociatividad, args->numero_sets, bloque);

    // Procesar el archivo de traza
    procesar_archivo(args->archivo_traza, cache, args->modo_verboso, args->rango_n, args->rango_m);

    free(args);

    // Imprimir las métricas de la simulación
    imprimir_metricas(cache);

    // Liberar la memoria de la caché
    destruir_cache(cache);

    return 0;
}

