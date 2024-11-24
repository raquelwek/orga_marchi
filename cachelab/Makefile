# Variables
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS = -lm

# Archivos fuente y objeto
SRC = hash.c funciones_cache.c procesar_archivos.c analisis_resultados.c cachesim.c
OBJ = $(SRC:.c=.o)

# Regla principal
all: cachesim

# Regla para compilar el ejecutable
cachesim: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Reglas individuales para cada archivo objeto
hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c $< -o $@

funciones_cache.o: funciones_cache.c funciones_cache.h hash.h
	$(CC) $(CFLAGS) -c $< -o $@

procesar_archivos.o: procesar_archivos.c procesar_archivos.h funciones_cache.h hash.h
	$(CC) $(CFLAGS) -c $< -o $@

analisis_resultados.o: analisis_resultados.c analisis_resultados.h procesar_archivos.h funciones_cache.h hash.h
	$(CC) $(CFLAGS) -c $< -o $@

cachesim.o: cachesim.c funciones_cache.h procesar_archivos.h analisis_resultados.h hash.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f *.o
	rm -f  cachesim
