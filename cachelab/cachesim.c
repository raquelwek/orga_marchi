#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "procesar_archivos.h"
#include "funciones_cache.h"
#include "analisis_resultados.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones_cache.h"
#include "analisis_resultados.h"
#include "procesar_archivos.h"

#define PENALTY 100       // Penalty para los misses

// Función para procesar el archivo de traza
void procesar_archivo(char* archivo_entrada, Cache* cache) {
    FILE *file = fopen(archivo_entrada, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de traza");
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), file)) {
        procesar_linea(cache, linea, BLOCK_SIZE);
    }
    fclose(file);
}

// Función principal
int main(int argc, char *argv[]) {
    // Verifica si se pasaron los 4 argumentos esperados
    if (argc != 5) {
        fprintf(stderr, "Uso incorrecto. Se esperan 4 argumentos.\n");
        fprintf(stderr, "Uso: %s <archivo_trace> <tamano_cache> <asociatividad> <numero_sets>\n", argv[0]);
        return 1; // Código de error
    }

    // Asignación de los argumentos a las variables correspondientes
    char *archivo_traza = argv[1];  // El archivo de traza
    int tamano_cache = atoi(argv[2]);  // El tamaño de la caché en bytes
    int asociatividad = atoi(argv[3]);  // La asociatividad de la caché (E)
    int numero_sets = atoi(argv[4]);  // El número de sets de la caché (S)

    // Crear la caché con los parámetros predefinidos
    Cache* cache = crear_cache(tamano_cache, asociatividad, numero_sets);

    // Procesar el archivo de traza
    procesar_archivo(argv[1], cache);

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
*/