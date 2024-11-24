#ifndef ANALISIS_RESULTADOS_H
#define ANALISIS_RESULTADOS_H

#include "funciones_cache.h"


/**
 * @brief Imprime las métricas de la simulación.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param penalty Ciclos de penalización por miss.
 */
void imprimir_metricas(const Cache* cache);

#endif  //ANALISIS_RESULTADOS_H
