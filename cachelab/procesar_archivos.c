/********* MANEJO DE ARCHIVOS  *********/
#include <procesar_archivos.h>
#define LAYAOT_LINE = "0x%x: %c 0x%x %u 0x%x\n";

void procesar_linea(Cache* cache, char* linea) {
    // Variables para almacenar cada campo
    uint32_t direccion;  // Campo 1: Dirección inicial
    char operacion;          // Campo 2: Operación (W o R)
    unsigned int dir_acceso; // Campo 3: Dirección de acceso
    unsigned int tamaño;     // Campo 4: Tamaño
    unsigned int dir_final;  // Campo 5: Dirección final
}