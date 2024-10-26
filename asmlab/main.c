#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"

void testStrPrint() {
    printf("INICIO PRUEBA TESTSTRPRINT\n");
    char* a = "elfo magico";
    strPrint(a, stdout);  // Llama a la función ensamblador
    char* b = "pato enojado";
    strPrint(b, stdout);  // Llama a la función ensamblador
    printf("\nFIN PRUEBA TESTSTRPRINT\n");
}

void testStrDelete() {
    char* str = (char*)malloc(20 * sizeof(char));
    if (str == NULL) {
        perror("Error en malloc");
        return;
    }

    snprintf(str, 20, "Hola, mundo!");
    printf("Cadena antes de strDelete: %s\n", str);

    strDelete(str);
    printf("Los datos fueron liberados%s\n");
}
int main (void){ 
    //testStrPrint();
    testStrDelete();
    return 0;
}


