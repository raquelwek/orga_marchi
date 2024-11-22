/********* ANALISIS DE RESULTADOS  *********/
#include "analisis_resultados.h"
#include <stdio.h>
#define penalty 100

void imprimir_metricas(const Cache* cache) {
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

    uint32_t tamanio_cache = cache->tamanio_cache/1000;//tam cache en KB
    float dirty_miss_rate = (total_misses) / (float)total_misses * 100;

    // Imprimimos las métricas
    printf("%d-way , %d sets, size=%d\n", cache->num_lineas, cache->num_conjuntos, tamanio_cache);
    printf("loads %d stores %d total %d\n", loads, stores, total_accesses);
    printf("rmiss %d wmiss %d total %d\n", rmiss, wmiss, total_misses);
    printf("dirty rmiss %d dirty wmiss %d\n", dirty_rmiss, dirty_wmiss);
    printf("bytes read %d bytes written %d\n", bytes_read, bytes_written);
    printf("read time %d write time %d\n", time_r, time_w);
    printf("miss rate %f\n", dirty_miss_rate);
}
