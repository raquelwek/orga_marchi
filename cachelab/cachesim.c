#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones_cache.h"
#include "analisis_resultados.h"
#include "procesar_archivos.h"

#define PENALTY 100       // Penalty para los misses
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


void verificar_argumentos(int argc, char *argv[], Argumentos* args) {
    if (argc < 5 || argc > 8) {
        fprintf(stderr, "Uso incorrecto. Se esperan 4 o 7 argumentos.\n");
        exit(1); // Finaliza con error
    }

    
    // Asignar valores comunes
    args->archivo_traza = argv[1];
    args->tamano_cache = atoi(argv[2]);
    args->asociatividad = atoi(argv[3]);
    args->numero_sets = atoi(argv[4]);
    args->modo_verboso = false;
    args->rango_n = 0; // Valores por defecto
    args->rango_m = 0;

    // Verificar modo verboso y rangos
    if (argc >= 6 && strcmp(argv[5], "-v")) {
        args->rango_n = atoi(argv[6]);
        args->rango_m = atoi(argv[7]);
    }
    
}

// Función para procesar el archivo de traza
void procesar_archivo(char* archivo_entrada, Cache* cache, bool modo_verboso, int n, int m) {
    FILE *file = fopen(archivo_entrada, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de traza");
        return;
    }
    char linea[256];
    if (!modo_verboso)
    {
        while (fgets(linea, sizeof(linea), file)) {
            procesar_linea(cache, linea);
        }
        fclose(file);
    } else {
        for (int i = n; i < m; i++)
        {
            procesar_linea(cache, linea);
        }
        fclose(file);
        
    }
    
    

}

// Función principal
int main(int argc, char *argv[]) {
    
     // Estructura para almacenar los argumentos procesados
    Argumentos args;

    // Verificar y procesar los argumentos
    verificar_argumentos(argc, argv, &args);
    
    

    // Crear la caché con los parámetros predefinidos
    Cache* cache = crear_cache(args.tamanioo_cache, args.asociatividad, args.numero_sets);


    // Procesar el archivo de traza
    procesar_archivo(args.archivo_traza, cache, args.modo_verboso, args.rango_n, args.rango_m );

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