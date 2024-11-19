#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "procesar_archivos.h"
#include "funciones_cache.h"
#include "analisis_resultados.h"


/********* SIMULADOR DE CACHÉ *********/
int main (void){    
    return 0;
}


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