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
    switch (l->type){
    case TypeNone:
        n->data = (void*) intClone(data);
        break;
    case TypeInt:
        n->data = (void*) strClone(data);
        break;
    case TypeCard:
        n->data = (void*) cardClone(data);
        break;
    }
    n->next = l->first;
    l->first = n;
    l->size++;
}

void listAddLast(list_t *l, void *data)
{
    listElem_t* n = malloc(sizeof(listElem_t));
    switch(l->type){
        case TypeInt:
            n -> data = (void*) intClone(data);
            break;
        case TypeString:
            n -> data = (void*)strClone(data);
            break;
        case TypeCard:
            n -> data = (void*)cardClone(data);
            break;
    
    }
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
    listElem_t* actual = l -> first;
    while (actual != NULL) {
        listAddFirst(copia, actual->data);
        actual = actual -> next;
    }
    return copia;
}

void *listRemove(list_t *l, uint8_t i)
{
    listElem_t* tmp = NULL;
    void* data = NULL;
    if(i==0){
        data = l->first->data;
        tmp = l->first;
        l->first = l->first->next;
    }else{
        listElem_t* n = l->first;
        for(uint8_t j = 0; j < i - 1; j++)
          n = n->next;
        data = n->next->data;
        tmp = n->next;
        n->next =n->next->next;
    }
    free(tmp);
    l->size--;
    return data;
}

void listSwap(list_t *l, uint8_t i, uint8_t j)
{
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

    free(l);
}


void listPrint(list_t *l, FILE *pFile)
{
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
