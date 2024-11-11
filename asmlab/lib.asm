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

; Strings literales
STR_PLACEHOLDER    db "%s", 0
NULL_str db "NULL", 0       
         

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
extern malloc
extern listAddLast
extern intPrint
extern fputc
extern free
extern getCloneFunction
extern getDeleteFunction
extern getPrintFunction
extern intDelete
extern listDelete
extern listNew

; ** String **
;char* strClone(char* a);
;   int length = strlen(a) + 1   ; obtener longitud de la cadena incluyendo el carácter nulo
;   char* copia = malloc(length) 
;   for (int i = 0; i < length; i++)
;       copia[i] = a[i]          ; copiar cada carácter, incluyendo el carácter nulo
;   return copia

strClone:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    
    mov r12, rdi            ;preservar puntero a char
    xor r13, r13            ;registro para alamacenar longitud del char

    call strLen              ; como recibe mismo parametro no cambio rdi
    mov r13, rax             ; alamceno longitud obtenida
    inc r13

    mov rdi, r13           ; movemos longitud a almacenar como param de malloc
    call malloc              ; dejo en rax el puntero a devolver
    
    ;inicializar valores de la copia
    xor r9, r9             ; registro volatil como contador del for

    .loop:
        cmp r9, r13
        jge .fin
        
        mov r10, [r12+ r9] ;uso como auxiliar registro no volatil
        mov [rax + r9], r10

        inc r9
        jmp .loop


    .fin:
    mov BYTE[rax + r9], 0          ; caracter nulo al final
    pop r13
    pop r12
    pop rbp
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
        je .printNull      ; si es '\0', saltar a la impresión final

        ; imprimir el carácter actual
        mov edi, edx        ; poner el carácter en edi
        mov rsi, r13        ; pasar pFile en rsi desde r13
        call fputc

        inc r12             ; acum++
        jmp .while

    .printNull:
        cmp r12,0
        jne .finalPrint

        lea rdi, [rel NULL_str] ; Cargar la dirección de "NULL" en rdi
        mov rsi, r13
        call strPrint

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
    
    push r12         
    mov r12, 0       ; Inicializar contador en 0 

    .while:
        mov dl, [rdi + r12] ; Leer el caracter en la posición rdi + r12
        cmp dl, 0           ; Comparar si es el carácter nulo (\0)
        je .fuera_de_loop   ; Si es \0, salimos del loop
        inc r12             ; Si no, incrementamos el contador y seguimos
        jmp .while

    .fuera_de_loop:
    ; En este punto, r12 contiene la longitud de la cadena (sin el \0)

    mov eax, r12d    ; Guardar el resultado en eax para retornarlo
    pop r12          ; Restaurr el valor original de r12
    ret              ; Retornar el valor en eax


;int32_t strCmp(char* a, char* b);
strCmp:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    
    xor rax, rax    ;inicializar contador

    .while:
        mov dl, [rdi + rax] ;caracter del char a
        mov al, [rsi + rax] ;caracter del char b
        cmp dl, 0           ; chequear si es nulo
        je .finA
        cmp al, 0
        je .bMenorQueA      ;pues A no termino aun
        cmp dl, al
        jne .noIguales
        inc rcx
        jmp .while
    .finA: 
        cmp al, 0           ; terminan al mismo tiempo?
        je .iguales
        jmp .aMenorQueB
    .noIguales:
        cmp dl,al
        jl .aMenorQueB
        jmp .bMenorQueA
    .iguales:
        mov rax, 0
        jmp .fin
    .aMenorQueB:
        mov rax, 1
        jmp .fin
    .bMenorQueA:
        mov rax, -1
        jmp .fin
    .fin:
        add rsp, 8
        pop rbp
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
    push rbp                      ; Guardar el valor original de rbp
    mov rbp, rsp                  ; Establecer el nuevo marco de pila

    mov al, [rdi + ARRAY_SIZE_OFFSET] ; Obtener el tamaño actual (a->size) en al

    pop rbp                       ; Restaurar el valor de rbp
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
    mov r13, rsi                           ;preservar el puntero al dato a agregar en r13
    xor r14, r14
    mov r14b, BYTE [r12 + ARRAY_SIZE_OFFSET]      ;preservar SIZE en r14
    
    ;comprobar que tengo espacio libre
    cmp r14b, BYTE [r12 + ARRAY_CAPACITY_OFFSET]
    jge .fin

    ;obtener la función de clonado
    mov edi, dword[r12 + ARRAY_TYPE_OFFSET]
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
    push rbp
    mov rbp, rsp

    mov dl, [rdi + ARRAY_SIZE_OFFSET]   ;size
    cmp sil, dl
    jl .obtener    ;Si i esta dentro del size, obtiene dato sino fin
    mov rax, 0
    jmp .fin


    .obtener:
    mov rdx, [rdi + ARRAY_SIZE_OFFSET]  ;size
    imul rsi, rsi, 8              ;shl rsi, 3
    mov rax, [rdx + rsi]

    .fin:
    pop rbp
    ret

; array_t* arrayNew(type_t t, uint8_t capacity){
;    array_t* a = malloc(capacity);
;    a -> type = t;
;    a -> size = 0;
;    a -> capacity = capacity;
;}
arrayNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    mov r12d, edi ; t
    mov r13b, sil  ; capacity

    mov rdi, ARRAY_SIZE
    call malloc ;Devuelve en rax el puntero

    mov dword [rax + ARRAY_TYPE_OFFSET], r12d
    mov BYTE [rax + ARRAY_CAPACITY_OFFSET], r13b
    mov BYTE [rax + ARRAY_SIZE_OFFSET], BYTE 0

    pop r13
    pop r12
    pop rbp
    ret

; void* arrayRemove(array_t* a, uint8_t i)
void* arrayRemove(array_t* a, uint8_t i) {
    // Verificar si el índice está fuera de rango
    if (i >= a->size) {
        return NULL; // Retorna 0 si está fuera de rango
    }

    // Obtener el elemento a eliminar
    void* removedData = a->data[i];

    // Mover los elementos a la izquierda para llenar el espacio vacío
    for (uint8_t j = i; j < a->size - 1; j++) {
        a->data[j] = a->data[j + 1];
    }

    // Disminuir el tamaño del arreglo
    a->size--;

    // Retornar el elemento eliminado
;   return removedData;
;}
arrayRemove:
   push rbp
   mov rbp, rsp
   push r12
   push r13
   push r14
   push r15
   mov r14, rdi
   mov r12b, BYTE [rdi + ARRAY_SIZE_OFFSET] ;Preservo el size
   mov r13b, sil
   cmp r13b, r12b
   jge .fin
   mov r15, [rdi + ARRAY_DATA_OFFSET + r13b * 8] ;rdi + OFFSET => PRIMER ELEMENTO. => ELEMENTO A eliminar
   dec r12b 
   .loop:
   cmp r13b, r12b 
   jge .fin
   mov rdi, r14
   mov sil, r13b
   inc r13b
   mov dl, r13b 

   call arraySwap
   jmp .loop

   .fin:
   mov rax, r15
   pop r15
   pop r14
   pop r13
   pop r12
   pop rbp
   ret

; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
   push rbp
   mov rbp, rsp
   mov r9, [rdi + ARRAY_DATA_OFFSET] ;primer dato
   mov r8, [r9 + sil * 8]; Array[i]
   mov r10, [r9 + dl * 8]; Array[j]

   mov [r9 + sil * 8], r10
   mov [r9 + dl * 8], r8

   pop rbp
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
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15
    mov r12, rdi; Puntero a la estructura del array
    mov r13, rsi; Archivo donde quiero imprimir
    
    mov edi, [r12 + ARRAY_TYPE_OFFSET] ;
    call getPrintFunction
    mov r14, rax ; Guardo la funcion

    mov dil, CHAR_OPENING_BRACKET
    mov rsi, r13
    call fputc

    xor r15, r15 ;contador


    .loop:
    
    cmp r15b, BYTE [r12 + ARRAY_SIZE_OFFSET]
    jge .fin

    mov rdi, [r12 + ARRAY_DATA_OFFSET + r15 * 8]
    mov rsi, r13
    call r14

    mov r8b,BYTE [r12 + ARRAY_SIZE_OFFSET]
    dec r8b
    cmp r15b, r8b
    je .fin

    mov dil, CHAR_COMA
    mov rsi, r13
    call fputc


    .fin

    mov dil, CHAR_CLOSING_BRACKET
    mov rsi, r13
    call fputc

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

; ** Card **
 push rbp
    mov rbp, rsp
    push r12
    push r13
    mov r12d, edi ; t
    mov r13b, sil  ; capacity

    mov rdi, ARRAY_SIZE
    call malloc ;Devuelve en rax el puntero

    mov dword [rax + ARRAY_TYPE_OFFSET], r12d
    mov BYTE [rax + ARRAY_CAPACITY_OFFSET], r13b
    mov BYTE [rax + ARRAY_SIZE_OFFSET], BYTE 0

    pop r13
    pop r12
    pop rbp
    ret

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    mov r12, rdi ; suit
    mov r13, rsi  ; number

    mov rsi, 3 ;=>type_card
    call listNew

    mov r14, rax
    mov rdi, CARD_SIZE
    call malloc ;Devuelve en rax el puntero
    mov [rax + CARD_NUMBER_OFFSET], r13
    mov [rax + CARD_SUIT_OFFSET], r12
    mov [rax + CARD_STACKED_OFFSET], r14

    pop r14
    pop r13
    pop r12
    pop rbp   
    ret

;char* cardGetSuit(card_t* c)
;   return c->suit;
cardGetSuit:
    
    mov rax, [rdi + CARD_SUIT_OFFSET]
    ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    push rbp
    mov rbp, rsp
    mov rax, [rdi + CARD_NUMBER_OFFSET]
    pop rbp
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
    mov al, BYTE[CHAR_OPENING_BRACE] 
    mov rdi, rax
    call fputc

    ; Imprimir `suit`
    mov rdi, [r12 + CARD_SUIT_OFFSET]
    mov rsi, r13
    call strPrint

    ; Imprimir '-'
    mov rsi, r13
    mov al, BYTE[CHAR_GUION] 
    mov rdi, rax
    call fputc

    ; Imprimir `number`
    mov rdi, [r12 + CARD_NUMBER_OFFSET]
    mov rsi, r13
    call intPrint

    ; Imprimir '-'
    mov rsi, r13
    mov al, BYTE[CHAR_GUION] 
    mov rdi, rax
    call fputc

    ; Imprimir '['
    mov rsi, r13
    mov al, byte[CHAR_OPENING_BRACKET]
    mov rdi, rax
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
        mov al, BYTE[CHAR_COMA]
        mov rdi, rax
        call fputc

    .ultimoNodo:
        jmp .while                      ; Volver al inicio del bucle

    .end_while:
    ; Imprimir ']'
    mov rsi, r13
    mov al, BYTE[CHAR_CLOSING_BRACKET]
    mov rdi, rax
    call fputc

    ; Imprimir '}'
    mov rsi, r13
    mov al, BYTE[CHAR_CLOSING_BRACE]
    mov rdi, rax
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
;   card_t* copiaCarta = malloc(sizeof(card_t))
;   list_t* copiaLista = malloc(sizeof((list_t)))
;   
;   listElem_t* actual = (c->stacked) -> primero
;   while actual != 0 
;       listAddLast(copiaLista, actual->data)
;       actual=actual -> next
;   copiaCarta-> stacked = copiaLista
;   copiaCarta -> number = c->number
;   copiaCarta -> suit = c->suit
cardClone:
    push rbp
    push r12
    push rbx
    push r13
    push r14
    mov rbp, rsp
    mov r12, rdi            ;reservo puntero a datos a clonar

    mov rdi, CARD_SIZE
    call malloc
    mov rbx, rax        ; guardarnos la nueva pos de mem -copiaCarta-

    mov rdi, LIST_SIZE_OFFSET
    call malloc
    mov r13, rax        ;guardamos puntero al stacked a copiar

    mov r14, [rbx + CARD_NUMBER_OFFSET]
    mov r14, [r14 + LIST_FIRST_OFFSET] ;Inicializar actual

    .while:
        cmp r14, 0
        je .fin

        mov rdi, r13
        mov rsi, [r14 + NODE_DATA_OFFSET]
        call listAddLast
        
        mov r14, [r14 + NODE_NEXT_OFFSET]
        jmp .while

    .fin:
    mov [rbx + CARD_STACKED_OFFSET], r13

    mov r9, [r12 +CARD_NUMBER_OFFSET]                 ;uso de auxiliar r9 volatil
    mov [rbx + CARD_NUMBER_OFFSET], r9

    mov r9, [r12 + CARD_SUIT_OFFSET]
    mov [rbx + CARD_SUIT_OFFSET], r9

    mov rax, rbx
    
    push r14
    push r13
    pop rbx
    pop r12
    pop rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
ret

;void cardDelete(card_t* c)
;   listDelete(c->stacked)
;   intDelete(c->number)
;   strDelete(c->suit)
;   free(c)
cardDelete:
    push rbp
    push r12
    mov rbp, rsp
    mov r12, rdi            ;guardar puntero a carta

    ;borrar stacked
    mov rdi, [r12 + CARD_STACKED_OFFSET]
    call listDelete

    ;borrar number
    mov rdi,[r12 + CARD_NUMBER_OFFSET]
    call intDelete

    ;borrar suit
    mov rdi, [r12 + CARD_SUIT_OFFSET]
    call strDelete

    ;borrar struct
    mov rdi, r12
    call free

    pop r12
    pop rbp
    ret

