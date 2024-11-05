#include "lib.h"

funcCmp_t *getCompareFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcCmp_t *)&intCmp;
        break;
    case TypeString:
        return (funcCmp_t *)&strCmp;
        break;
    case TypeCard:
        return (funcCmp_t *)&cardCmp;
        break;
    default:
        break;
    }
    return 0;
}
funcClone_t *getCloneFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcClone_t *)&intClone;
        break;
    case TypeString:
        return (funcClone_t *)&strClone;
        break;
    case TypeCard:
        return (funcClone_t *)&cardClone;
        break;
    default:
        break;
    }
    return 0;
}
funcDelete_t *getDeleteFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcDelete_t *)&intDelete;
        break;
    case TypeString:
        return (funcDelete_t *)&strDelete;
        break;
    case TypeCard:
        return (funcDelete_t *)&cardDelete;
        break;
    default:
        break;
    }
    return 0;
}
funcPrint_t *getPrintFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcPrint_t *)&intPrint;
        break;
    case TypeString:
        return (funcPrint_t *)&strPrint;
        break;
    case TypeCard:
        return (funcPrint_t *)&cardPrint;
        break;
    default:
        break;
    }
    return 0;
}

/** Int **/

int32_t intCmp(int32_t *a, int32_t *b)
{   
   if (*a > *b){
    return -1;
   }else if (*a < *b){
    return 1;
   }else {
    return 0;
   }
}

void intDelete(int32_t *a)
{
    free(a);
}

void intPrint(int32_t *a, FILE *pFile)
{
    fprintf(pFile,"%d", *a );
}

int32_t *intClone(int32_t *a)
{
    int32_t* copia = malloc(sizeof(int32_t));
    *copia = *a;
    return copia;
    
}

/** Lista **/

list_t *listNew(type_t t)
{
    list_t* l  =  malloc(sizeof(list_t));
    l -> type = t;
    l -> size = 0;
    l -> first = NULL;
    l -> last = NULL;
    return l;
}

uint8_t listGetSize(list_t *l)
{
   return l->size;
}

void *listGet(list_t *l, uint8_t i)
{
    listElem_t*  n = l->first;
    for (uint8_t j = 0; j < i; j++){
        n = n -> next;
    }
    return n -> data;
}

void listAddFirst(list_t *l, void *data)
{
    listElem_t* n = malloc(sizeof(listElem_t));
    funcClone_t* clonar = getCloneFunction(l->type);
    n -> data = clonar(data);
    n -> prev = NULL;
    n->next = l->first;
    

    // Si la lista estaba vacía, también actualizamos el último elemento
    if (l->last == NULL) {
        l->last = n;
    }else {
        l->first->prev = n; 
    }
    l->first = n;
    l->size++;
}
    

void listAddLast(list_t *l, void *data)
{
    listElem_t* n = malloc(sizeof(listElem_t));
    funcClone_t* clonar = getCloneFunction(l->type);
    n -> data = clonar(data);
    n -> next = NULL;
    n -> prev = l -> last;

    if (l->last == NULL) { // La lista está vacía
        l->first = n;
    } else {
        l->last->next = n;
    }
    l -> last = n;
    l -> size++;
}

list_t *listClone(list_t *l)
{   

     list_t* copia = listNew(l->type);
    listElem_t* actual = l->first;

    while (actual != NULL) {
        listAddLast(copia, actual->data); 
        actual = actual->next;
    }

    return copia;
}

void *listRemove(list_t *l, uint8_t i)
{
    if (l->size == 0 || i >= l->size) {
        return NULL; // Lista vacía o si el índice es inválido
    }

    listElem_t *tmp = l->first;
    void *data = NULL;

    if (i == 0) {
        data = tmp->data;
        l->first = tmp->next;
        if (l->first != NULL) {
            l->first->prev = NULL;
        } else {
            l->last = NULL; // La lista queda vacía
        }
    } else {
        for (uint8_t j = 0; j < i; j++) {
            tmp = tmp->next;
        }
        data = tmp->data;
        tmp->prev->next = tmp->next;
        if (tmp->next != NULL) {
            tmp->next->prev = tmp->prev;
        } else {
            l->last = tmp->prev; // Actualizar el último elemento
        }
    }

    free(tmp);
    l->size--;
    return data;
}

void listSwap(list_t *l, uint8_t i, uint8_t j)
{
    if (i >= l->size || j >= l->size || i == j) {
        return; //índices son inválidos o son iguales
    }

    listElem_t *nodeI = l->first;
    listElem_t *nodeJ = l->first;

    for (uint8_t index = 0; index < i; index++) {
        nodeI = nodeI->next;
    }

    for (uint8_t index = 0; index < j; index++) {
        nodeJ = nodeJ->next;
    }

    void *tmp = nodeI->data;
    nodeI->data = nodeJ->data;
    nodeJ->data = tmp;
}

void listDelete(list_t *l)
{
    funcDelete_t* borrar = getDeleteFunction(l->type);
    listElem_t* actual = l->first;

    // Eliminar cada nodo de la lista antes de hacer el free del structs
    while (actual != NULL) {
        listElem_t* proximo = actual->next;

        borrar(actual->data);
        
        free(actual);
        actual = proximo;
    }
    l->first = NULL;
    l->last = NULL;
    l->size = 0;

    free(l);
}


void listPrint(list_t *l, FILE *pFile)
{
    
    if (!l || !pFile) return; // Verifico si la lista o el archivo es NULL
    
    funcPrint_t *printFunction = getPrintFunction(l->type);
    if (!printFunction) return; // Verifico si se obtuvo la función de impresión adecuada

    fprintf(pFile, "[");
    
    listElem_t *current = l->first;
    while (current != NULL)
    {
        printFunction(current->data, pFile);
        current = current->next;
        if (current != NULL)
        {
            fprintf(pFile, ", ");
        }
    }
    
    fprintf(pFile, "]");
}

/** Game **/

game_t *gameNew(void *cardDeck, funcGet_t *funcGet, funcRemove_t *funcRemove, funcSize_t *funcSize, funcPrint_t *funcPrint, funcDelete_t *funcDelete)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t *g)
{
    int applied = 0;
    uint8_t i = 0;
    while (applied == 0 && i + 2 < g->funcSize(g->cardDeck))
    {
        card_t *a = g->funcGet(g->cardDeck, i);
        card_t *b = g->funcGet(g->cardDeck, i + 1);
        card_t *c = g->funcGet(g->cardDeck, i + 2);
        if (strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0)
        {
            card_t *removed = g->funcRemove(g->cardDeck, i);
            cardAddStacked(b, removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t *g)
{
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t *g)
{
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t *g, FILE *pFile)
{
    g->funcPrint(g->cardDeck, pFile);
}
