;### DEFINICIÓN DE OFFSETS ###

;## ARRAY ##
%define ARRAY_TYPE_OFFSET 0
%define ARRAY_SIZE_OFFSET 4
%define ARRAY_CAPACITY_OFFSET 5
%define ARRAY_DATA_OFFSET 8
%define ARRAY_SIZE 16

;## CARD ##
%define CARD_SUIT_OFFSET 0
%define CARD_NUMBER_OFFSET 8
%define CARD_STACKED_OFFSET 16
%define CARD_SIZE 24

;## LIST ##
%define LIST_TYPE_OFFSET 0
%define LIST_SIZE_OFFSET 4
%define LIST_FIRST_OFFSET 8
%define LIST_LAST_OFFSET 16
%define ARRAY_SIZE 24

;## NODO LIST ##
%define NODE_DATA_OFFSET 0
%define NODE_NEXT_OFFSET 8
%define NODE_PREV_OFFSET 16
%define NODE_SIZE_OFFSET 24


;### CONSTANTES PARA LOS PRINTS ###
section .data
; Símbolos ASCII
CHAR_OPENING_BRACE    db '{'        
CHAR_CLOSING_BRACE    db '}'        
CHAR_OPENING_BRACKET  db '['        
CHAR_CLOSING_BRACKET  db ']'       
CHAR_GUION            db '-' 
CHAR_COMA             db','      

; Strings literales para los nombres de los símbolos
STR_PLACEHOLDER    db "%s", 0        
         

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
extern intPrint
extern fputc
extern free
extern getCloneFunction
extern getDeleteFunction

; ** String **
;char* strClone(char* a);
strClone:
ret

;void strPrint(char* a, FILE* pFile) LOGICA A REPLICAR:
;   size_t acum = 0;
;   while (char[acum] != '\0' )
;       fputc(char[acum], pFile)
;       acum++

strPrint:
    push rbp
    mov rbp, rsp           ; establecer el marco de pila
    push r12               ; preservar r12
    push r13               ; preservar r13 (para pFile)
    push r14               ; preservar r14 (para char* a)

    sub rsp, 8
    mov r13, rsi           ; guardar pFile en r13
    mov r14, rdi           ; guardar el puntero a la cadena en r14
    mov r12, 0             ; inicializar contador (acum)

    .while:
        mov dl, [r14 + r12] ; obtener el carácter actual de la cadena
        cmp dl, 0
        je .finalPrint      ; si es '\0', saltar a la impresión final

        ; imprimir el carácter actual
        mov edi, edx        ; poner el carácter en edi
        mov rsi, r13        ; pasar pFile en rsi desde r13
        call fputc

        inc r12             ; acum++
        jmp .while

    .finalPrint:
        add rsp, 8
        ; restaurar los registros no volátiles
        pop r14
        pop r13
        pop r12
        pop rbp
        ret


;uint32_t strLen(char* a);
strLen:
ret

;int32_t strCmp(char* a, char* b);
strCmp:
ret

;void strDelete(char* a);
;   free(a);

strDelete:
    push rbp                
    mov rbp, rsp            ; establecer el nuevo marco de pila

    mov rdi, rdi            ; `a` ya debe estar en rdi, solo aseguramos
    call free               ; llamar a `free` para liberar `a`

    pop rbp                
    ret


; ** Array **

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
ret

; void arrayAddLast(array_t* a, void* data)
;   tam = a->size
;   
;   if (a->size == a->capacity) 
;       return
;   funcCopia = getCloneFunction(a->type)
;   a->data[tam] = funcCopia(data)
;   a->size++
arrayAddLast:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    sub rsp, 8                              ;alinear pila
    mov r12, rdi                            ;preservar el puntero al struct array_t en r12
    mov r13, rsi                            ;preservar el puntero al dato a agregar en r13
    mov r14, [r12 + ARRAY_SIZE_OFFSET]      ;preservar SIZE en r14
    
    ;comprobar que tengo espacio libre
    cmp r14, [r12 + ARRAY_CAPACITY_OFFSET]
    jge .fin

    ;obtener la función de clonado
    mov rdi, [r12 + ARRAY_TYPE_OFFSET]
    call getCloneFunction

    ;clonar el dato
    mov rdi, r13
    call rax

    ;guardar el dato clonado en la última posición
    mov [r12 + ARRAY_DATA_OFFSET + r14 * 8], rax
    inc BYTE[r12 + ARRAY_SIZE_OFFSET]

    .fin:
    add rsp, 8                          ;restaurar pila
    pop r14
    pop r13
    pop r12
    pop rbp
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

; void arrayDelete(array_t* a) 
;   funcBorrar = getDeleteFunction(a->type)
;   for i=0;i< a->size, i++ 
;       funcBorrar(a->data[i])
;   free(a->data)
;   free(a)
arrayDelete:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    sub rsp, 8      ;alinear pila

    xor r13,r13  ;inicializar contador en 0
    mov rbx, rdi ;preservo puntero a struct

    mov rdi, [rbx + ARRAY_TYPE_OFFSET]  ;preparo param para obtner func de borrar
    call getDeleteFunction
    mov r12, rax                        ;preservo puntero a funcion de borrar

    .for:
        cmp r13b, BYTE[rbx + ARRAY_SIZE]
        jge .ultimoPaso                        ;si el contador es mayor o igual termino iteracion

        mov rdi, [rbx +ARRAY_DATA_OFFSET + r13*8]   ;colocar params para eliminar el dato
        call r12
        inc r13
        jmp .for 

    .ultimoPaso:
        mov rdi, [rbx+ARRAY_DATA_OFFSET]
        call free

        mov rdi, rbx
        call free

    .fin:
        add rsp, 8
        pop r12
        pop rbx
        pop rbp
        ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
ret

; ** Card **

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
ret

;char* cardGetSuit(card_t* c)
;   return c->suit;
cardGetSuit:
    mov rax, [rdi + CARD_SUIT_OFFSET]
    ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
ret


;void cardPrint(card_t* c, FILE* pFile) 
;RES->{suit-number-[cartas en el stack]}:  ejemplo {espada-7-[{oro-2-[]}, {basto-7-[]}]}
;   fputc('{', pFile)
;   strPrint(c->suit, pfile)
;   fputc('-', pFile)
;   intPrint(c->number, pFile)
;   fputc('-', pFile)
;   fputc('[', pFile)
;   actual = (c->stacked) -> first
;   while actual != 0 
;       cardPrint(actual -> dato, pFile)
;       actual = actual -> next
;       if actual != 0 
;            fputc(',',pFile)
;   fputc(']', pFile)
;   fputc('}', pFile)

cardPrint:
    push rbp
    push r12
    push r13
    push r14
    mov rbp, rsp
    sub rsp, 8                  ; Alinear pila

    ; Preservar parámetros
    mov r12, rdi                ; `c` (card)
    mov r13, rsi                ; `pFile`

    ; Imprimir '{'
    mov rsi, r13
    mov dil, CHAR_OPENING_BRACE
    call fputc

    ; Imprimir `suit`
    mov rdi, [r12 + CARD_SUIT_OFFSET]
    mov rsi, r13
    call strPrint

    ; Imprimir '-'
    mov rsi, r13
    mov dil, CHAR_GUION
    call fputc

    ; Imprimir `number`
    mov rdi, [r12 + CARD_NUMBER_OFFSET]
    mov rsi, r13
    call intPrint

    ; Imprimir '-'
    mov rsi, r13
    mov dil, CHAR_GUION
    call fputc

    ; Imprimir '['
    mov rsi, r13
    mov dil, CHAR_OPENING_BRACKET
    call fputc

    ; Obtener el primer nodo de `stacked`
    mov r14, [r12 + CARD_STACKED_OFFSET]
    mov r14, [r14 + LIST_FIRST_OFFSET]  ; `actual` = primer nodo en la lista

    ; Iterar sobre la lista de cartas en `stacked`
    .while:
        cmp r14, 0                      ; ¿actual == NULL?
        je .end_while                   ; Si no hay más nodos, salir

        ; Imprimir la carta actual
        mov rdi, [r14 + NODE_DATA_OFFSET]
        mov rsi, r13
        call cardPrint

        ; Avanzar al siguiente nodo
        mov r14, [r14 + NODE_NEXT_OFFSET]
        cmp r14, 0                      ; ¿actual != NULL?
        je .ultimoNodo                  ; Si es el último nodo, omitir coma

        ; Imprimir ','
        mov rsi, r13
        mov dil, CHAR_COMA
        call fputc

    .ultimoNodo:
        jmp .while                      ; Volver al inicio del bucle

    .end_while:
    ; Imprimir ']'
    mov rsi, r13
    mov dil, CHAR_CLOSING_BRACKET
    call fputc

    ; Imprimir '}'
    mov rsi, r13
    mov dil, CHAR_CLOSING_BRACE
    call fputc

    ; Restaurar registros y finalizar
    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbp
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

