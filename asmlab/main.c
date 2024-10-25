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
    printf("\nFIN PRUEBA TESTSTRPRINT\n");
}
int main (void){ 
    testStrPrint();   
    return 0;
}


