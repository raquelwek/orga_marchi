
global strClone
global strPrint
global strCmp
global strLen
global strDelete

global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew


section .text
extern fputc

; ** String **
;char* strClone(char* a);
strClone:
ret

;void strPrint(char* a, FILE* pFile) LOGICA A REPLICAR:
;   size_t acum = 0;
;   while (char[acum] != '\0' )
;       fprintf(pFile, "%s",char[acum])
;       acum++
;   fprintf(pFile, "\n",char[acum])

strPrint:
    push r12    ;preservar valor de posible funcion anterior
    mov r12, 0  ;inicializar contador

    .while:
        mov dl, [rdi+r12] ; obtener caracter actual
        ; preparar parametros para fputc  char -> rdi y PFILE -> RSI
        cmp dl, 0
        je .fin       ; si llego al final termino de iterar

        ;sino preparo el proximo caracter a ser impreso cambiando rdi
        push rdi
        mov dil, dl       
        call fputc
        pop rdi
        inc r12
        jmp .while
        
    
    .fin:
        push rdi
        mov dil, 10 ; 10 es el cdg ASCII para '\n'       
        call fputc
        pop rdi
        pop r12
        ret

;uint32_t strLen(char* a);
strLen:
ret

;int32_t strCmp(char* a, char* b);
strCmp:
ret

;void strDelete(char* a);
strDelete:
ret


; ** Array **

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
ret

; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
ret

; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
ret

; void arrayDelete(array_t* a) {
arrayDelete:
ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
ret

; ** Card **

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
ret


;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
ret


;int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
ret

;card_t* cardClone(card_t* c)
cardClone:
ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
ret

;void cardDelete(card_t* c)
cardDelete:
ret

