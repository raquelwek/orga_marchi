#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib.h"
/*  CASO LISTA */
list_t* CrearMazoCincoCartas(){

    list_t* mazo = listNew(3); //Crear mazo de cartas enlazadas
    char* palo1 = "espada";
    int32_t* num1 = 7;
    card_t* carta1 = cardNew(palo1, num1);
    listAddLast(carta1);

    char* palo2 = "basto";
    int32_t* num2 = 1;
    card_t* carta2 = cardNew(palo2, num2);
    listAddLast(carta2);

    char* palo3 = "oro";
    int32_t* num3 = 5;
    card_t* carta3 = cardNew(palo3, num3);
    listAddLast(carta3);

    char* palo4 = "copa";
    int32_t* num4 = 4;
    card_t* carta4 = cardNew(palo4, num4);
    listAddLast(carta4);

    char* palo5 = "espada";
    int32_t* num5 = 3;
    card_t* carta5 = cardNew(palo5, num5);
    listAddLast(carta5);

    return mazo;

}
void ImprimirMazoLista(list_t* mazo){
    listElem_t* actual = mazo->first;
    while (actual != 0) {
        cardPrint(actial -> dato)
        actual = actual -> next;
    }
}

void testMazoConLista(){
    printf("----INICIA TESTMAZOCONLISTA----\n")
    list_t* mazo = CrearMazoCincoCartas();
    printf("\n----se  creo mazo con 5 cartas ----\n")
    ImprimirMazoLista(mazo);
    printf("\n");
    
    printf("----se  apila una carta a la primera del mazo ----\n");
    char* palo2 = "basto";
    int32_t* num2 = 1;

    card_t* carta2 = cardNew(palo2, num2);
    listElem_t* primera = listGet(mazo,0)
    cardAddStacked(primera->dato,carta2)

    printf("----se  espera ver la subcarta agregada ----\n");
    ImprimirMazoLista(mazo);

    printf("----se  borra el mazo de cartas ----\n");
    listDelete(mazo);
    printf("TODO OK\n");


}
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
    //testStrDelete();
    return 0;
}


