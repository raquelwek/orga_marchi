#ifndef ANALISIS_RESULTADOS_H
#define ANALISIS_RESULTADOS_H

#include "funciones_cache.h"
/**
 * @brief Calcula y devuelve el número total de accesos (loads + stores).
 * 
 * @param cache Puntero a la estructura de la caché.
 * @return Número total de accesos realizados.
 */
int calcular_total_accesos(const Cache* cache);

/**
 * @brief Calcula y devuelve el número total de misses (lectura y escritura).
 * 
 * @param cache Puntero a la estructura de la caché.
 * @return Número total de misses.
 */
int calcular_total_misses(const Cache* cache);

/**
 * @brief Calcula y devuelve la tasa de misses (miss rate).
 * 
 * @param cache Puntero a la estructura de la caché.
 * @return Tasa de misses como un valor flotante (0.0 - 1.0).
 */
float calcular_miss_rate(const Cache* cache);

/**
 * @brief Imprime las métricas de la simulación.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param penalty Ciclos de penalización por miss.
 */
void imprimir_metricas(const Cache* cache, int penalty);

/**
 * @brief Calcula el tiempo total acumulado (en ciclos) para lecturas y escrituras.
 * 
 * @param cache Puntero a la estructura de la caché.
 * @param penalty Ciclos de penalización por miss.
 * @param tiempo_lecturas Puntero para almacenar el tiempo acumulado en lecturas.
 * @param tiempo_escrituras Puntero para almacenar el tiempo acumulado en escrituras.
 */
void calcular_tiempos_acumulados(const Cache* cache, int penalty, int* tiempo_lecturas, int* tiempo_escrituras);

#endif // ANALISIS_RESULTADOS_H
