/********* ANALISIS DE RESULTADOS  *********/
#include "analisis_resultados.h"
#include <stdio.h>

void imprimir_metricas(const Cache* cache, int penalty) {
    // Obtenemos los contadores de la caché
    int* loads = (int*)hash_obtener(cache->contador, "loads");
    int* stores = (int*)hash_obtener(cache->contador, "stores");
    int* rmiss = (int*)hash_obtener(cache->contador, "rmiss");
    int* wmiss = (int*)hash_obtener(cache->contador, "wmiss");
    int* dirty_rmiss = (int*)hash_obtener(cache->contador, "dirty-rmiss");
    int* dirty_wmiss = (int*)hash_obtener(cache->contador, "dirty-wmiss");
    int* bytes_read = (int*)hash_obtener(cache->contador, "bytes-read");
    int* bytes_written = (int*)hash_obtener(cache->contador, "bytes-written");
    int* time_w = (int*)hash_obtener(cache->contador, "time-w");
    int* time_r = (int*)hash_obtener(cache->contador, "time-r");

    // Calculamos los resultados
    int total_accesses = *loads + *stores;
    int total_misses = *rmiss + *wmiss;
    int total_dirty_misses = *dirty_rmiss + *dirty_wmiss;
    int total_time = *time_w + *time_r;
    
    // Imprimimos las métricas
    printf("Métricas de la simulación de caché:\n");
    printf("Accesos totales (loads + stores): %d\n", total_accesses);
    printf("Misses totales (rmiss + wmiss): %d\n", total_misses);
    printf("Misses sucios (dirty-rmiss + dirty-wmiss): %d\n", total_dirty_misses);
    printf("Bytes leídos: %d\n", *bytes_read);
    printf("Bytes escritos: %d\n", *bytes_written);
    printf("Tiempo total (time-w + time-r): %d\n", total_time);
    
    // Tiempo de penalización total
    int total_penalty_time = total_misses * penalty;
    printf("Tiempo de penalización total: %d\n", total_penalty_time);
    
    // Calcular y mostrar el rendimiento
    float hit_rate = (total_accesses - total_misses) / (float)total_accesses * 100;
    float miss_rate = (total_misses) / (float)total_accesses * 100;
    float dirty_miss_rate = (total_dirty_misses) / (float)total_misses * 100;

    printf("Tasa de aciertos: %.2f%%\n", hit_rate);
    printf("Tasa de fallos: %.2f%%\n", miss_rate);
    printf("Tasa de fallos sucios: %.2f%%\n", dirty_miss_rate);
}
