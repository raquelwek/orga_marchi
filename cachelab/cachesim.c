#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "funciones_cache2.h"
#include "procesar_archivos.h"


//#define PENALTY 100       // Penalty para los misses
// Estructura para devolver los argumentos procesados
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
    char place_holder[] = "%d %c %x %x %d %d %d %d";
    printf(place_holder, indice, info->case_identifier, info->cache_index, info->cache_tag, info->cache_line, info->line_tag, info->valid_bit, info->dirty_bit);
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
    if (argc >= 6 && strcmp(argv[5], "-v") == 0) {
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

    uint32_t E = cache -> num_lineas;
    while (fgets(linea, sizeof(linea), file)) {
        indice++; // Incrementar el índice al leer una nueva línea

        if (!modo_verboso) {
            // En modo normal, procesar todas las líneas
            procesar_linea(cache, linea, info);
            cache -> indice_op++;
        } else {
            // En modo verboso, procesar solo líneas dentro del rango [n, m]
            if (indice >= n && indice <= m) {
                procesar_linea(cache, linea, info);
                cache -> indice_op++;
                registrar_procesada(info, cache->indice_op, E);
            }
        }
    }
    fclose(file);
    free(info);
    

}
   
uint32_t calcular_tambloque(int tamano_cache,int numero_sets,int asociatividad){
    return (tamano_cache / (numero_sets * asociatividad));
}

void imprimir_metricas(Cache* cache) {
    /*
    // Obtenemos los contadores de la caché
    uint32_t* loads = (uint32_t*)hash_obtener(cache->contador, strings[0]);
    uint32_t* stores = (uint32_t*)hash_obtener(cache->contador, strings[1]);
    uint32_t* rmiss = (uint32_t*)hash_obtener(cache->contador, strings[2]);
    uint32_t* wmiss = (uint32_t*)hash_obtener(cache->contador, strings[3]);
    uint32_t* dirty_rmiss = (uint32_t*)hash_obtener(cache->contador, strings[4]);
    uint32_t* dirty_wmiss = (uint32_t*)hash_obtener(cache->contador, strings[5]);

    uint32_t* bytes_read = (uint32_t*)hash_obtener(cache->contador, strings[6]);
    uint32_t* bytes_written = (uint32_t*)hash_obtener(cache->contador, strings[7]);
    uint32_t* time_w = (uint32_t*)hash_obtener(cache->contador, strings[8]);
    uint32_t* time_r = (uint32_t*)hash_obtener(cache->contador, strings[9]);
    */
    // Calculamos los resultados
    contador_t* contador = cache->contador;
    uint32_t total_accesses = contador -> loads + contador -> stores;
    uint32_t total_misses = contador->rmiss+ contador->wmiss;
    uint32_t total_dirty_misses = contador->dirty_rmiss + contador->dirty_wmiss;
    //uint32_t total_time = contador->time_w + *time_r;

    uint32_t tamanio_cache = cache->tamanio_cache/1000;//tam cache en KB
    double dirty_miss_rate = (total_misses) / total_accesses;

    // Imprimimos las métricas
    printf("%d-way, %d sets, size = %dKB\n", cache->num_lineas, cache->num_conjuntos, tamanio_cache);
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

/********* SIMULADOR DE CACHÉ *********/
/*
    SECCIONES A DESARROLLAR
    - Procesar archivos 
        * Función que cree caché (almacenar mem)
        * Función que procese línea de traza (op a caché)
           * Actualizar datos de caché
           * esHit? -> contabilizar hit de R o W
           * else: esDirtyMiss? contabilizar dirtymiss correspondiente
                * else: contabilizar miss correspodiente
           * en base a cada acción ir sumando los tiempos de accesos
        * Función que libere memoria de caché
    
    - Funciones de caché
        * Leer y escribir en caché
        * Reemplazar bloques
        * Limpiar caché
        * actulizar estructuras de datos para las estadísticas (lista enlazada)

    - Análisis de datos obtenidos: "analisis_resultados.c"
        - Métricas:
        número de lecturas (loads)
        número de escrituras (stores)
        número total de accesos (loads + stores)
        número de misses de lectura (rmiss)
        número de mises de escritura (wmiss)
        número total de misses (rmiss + wmiss)
        número de “dirty read misses” y “dirty write misses”
        cantidad de bytes leídos de memoria (bytes read)
        cantidad de bytes escritos en memoria (bytes written)
        tiempo de acceso acumulado (en ciclos) para todas las lecturas
        tiempo de acceso acumulado (en ciclos) para todas las escrituras
        miss rate total
        - Imprimir métricas:
            * loads "R"
            * stores "W"
            * bytes read - “miss de lectura”  
            * bytes written - "mis de escritura"
            * dirty rmiss - "dirtymiss de lec"
            * dirty wmiss - "dirtymiss de es"
            * miss rate - total misses / accesos a ram
        
        - Contabilización de tiempos de ejecución:
            1. si es hit +1ciclo
            2. si es miss
                clean: penalty+1 
                dirty: 2*penalty + 1

    - main: "cachesim.c"
        * interpretar comando
        * interfaz de línea de comandos
        * liberar memoria
        * 
    Reading symbols from ./cachesim...
(gdb) (gdb) set args ./trazas/adpcm.xex 2048 2 64
Undefined command: "".  Try "help".
(gdb) set args ./trazas/pruebas.xex 2048 2 64
(gdb) (gdb) break main
Undefined command: "".  Try "help".
(gdb) break main
Breakpoint 1 at 0x403605: file cachesim.c, line 155.
*/