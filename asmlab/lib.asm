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
%define LIST_SIZE 24

;## NODO LIST ##
%define NODE_DATA_OFFSET 0
%define NODE_NEXT_OFFSET 8
%define NODE_PREV_OFFSET 16
%define NODE_SIZE_OFFSET 24

;## TIPOS DE DATOS ###
%define TYPE_INT 1
%define TYPE_STRING 2
%define TYPE_CARD 3

;## SIZE INICIAL ##
%define INITIAL_SIZE 0
%define FIN_CHAR 0
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
extern listClone
extern listAddLast
extern listAddFirst
extern intPrint
extern intClone
extern intCmp
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

    call strLen               ; como recibe mismo parametro no cambio rdi
    mov r13d, eax             ; alamceno longitud obtenida
    inc r13d

    xor rdi, rdi
    mov edi, r13d          ; movemos longitud a almacenar como param de malloc
    call malloc              ; dejo en rax el puntero a devolver
    
    ;inicializar valores de la copia
    xor r9, r9             ; registro volatil como contador del for
    xor r10,r10

    .loop:
    mov r10b, byte [r12 + r9] ; Copiar carácter desde el original
    mov byte [rax + r9], r10b ; Escribir carácter en la copia
    inc r9                   ; Incrementar el contador
    cmp r9d, r13d            ; Comparar con la longitud total (incluye '\0')
    jl .loop                 ; Continuar si aún no hemos alcanzado el final


    .fin:
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
    xor r12,r12             ; inicializar contador (acum)

    .while:
        mov dl, [r14 + r12] ; obtener el carácter actual de la cadena
        cmp dl, FIN_CHAR
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
        pop r14
        pop r13
        pop r12
        pop rbp
        ret
    
        

;uint32_t strLen(char* a);
strLen:
    push rbp
    mov rbp, rsp
    xor rcx, rcx       ; Inicializar contador en 0 
    xor rdx, rdx

    .while:
        mov dl, [rdi + rcx] ; Leer el caracter en la posición rdi + r12
        cmp dl, 0           ; Comparar si es el carácter nulo (\0)
        je .fuera_de_loop   ; Si es \0, salimos del loop
        inc rcx             ; Si no, incrementamos el contador y seguimos
        jmp .while

    .fuera_de_loop:
    xor rax, rax
    mov eax, ecx    ; Guardar el resultado en eax para retornarlo
    pop rbp          ; Restaurar el valor original de rbp
    ret              ; Retornar el valor en eax


;int32_t strCmp(char* a, char* b);
strCmp:
    push rbp
    mov rbp, rsp
    
    
    xor rax, rax    ;inicializar contador
    xor rcx, rcx    ;placeholder para char a
    xor rdx, rdx    ;idem para char b

    .while:
        mov cl, [rdi + rax] ; Cargar carácter de la cadena a
        mov dl, [rsi + rax] ; Cargar carácter de la cadena b
        cmp cl, dl          
        jne .noIguales      
        test cl, cl          ; Son nulos?
        je .iguales          
        inc rax              
        jmp .while           
    .noIguales:
        jl .aMenorQueB       ; Si cl < dl, A es menor que B
        jmp .bMenorQueA      ; De lo contrario, B es menor que A

    .iguales:
        xor rax, rax         ; Retornar 0 (iguales)
        jmp .fin

    .aMenorQueB:
        mov rax, 1           ; Retornar 1 (A menor que B)
        jmp .fin

    .bMenorQueA:
        mov rax, -1          ; Retornar -1 (B menor que A)
    .fin:
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
    sub rsp, 8                                      ;alinear pila

    mov r12, rdi                                    ;preservar el puntero al struct array_t en r12
    mov r13, rsi                                    ;preservar el puntero al dato a agregar en r13
    xor r14, r14
    mov r14b, BYTE [r12 + ARRAY_SIZE_OFFSET]        ;preservar SIZE en r14

    ;comprobar que tengo espacio libre
    mov cl, [r12 + ARRAY_CAPACITY_OFFSET]
    cmp r14b, cl
    jz .fin

    ;obtener la función de clonado
    mov edi, [r12 + ARRAY_TYPE_OFFSET]
    call getCloneFunction

    ;clonar el dato 
    mov rdi, r13
    call rax

    ;guardar el dato clonado en la última posición
    xor rcx, rcx
    mov cl, [r12 + ARRAY_SIZE_OFFSET]
    mov rdx, [r12 + ARRAY_DATA_OFFSET]
    shl rcx, 3
    mov [rdx + rcx], rax                       ; a->data[a->size] = clon;

    xor rcx, rcx
    mov cl, [r12 + ARRAY_SIZE_OFFSET]
    inc cl
    mov [r12 + ARRAY_SIZE_OFFSET], cl          ; a->size++;

    .fin:
    add rsp, 8                          
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    sub rsp, 8

    xor r14, r14        
    mov r14b, sil       ;guardo indice

    xor r13, r13
    mov r13b, BYTE [rdi + ARRAY_SIZE_OFFSET]   ;size

    cmp r14b, r13b      ;indice >= size => fueraRango
    jge .fueraRango
    cmp r14b, 0         ;indice < 0 => fueraRango
    jl .fueraRango
    
    mov r12, [rdi + ARRAY_DATA_OFFSET] ;Primer elemento                
    shl r14, 3
    mov rax, [r12 + r14]
    jmp .fin

    .fueraRango:
    xor rax,rax

    .fin:
    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

; array_t* arrayNew(type_t t, uint8_t capacity){
;    array_t* a = malloc(sizeof(array_t));
;    a -> type = t;
;    a -> size = 0;
;    a -> capacity = capacity;
;    a -> data = malloc(capacity * sizeof(void*));
;
arrayNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13

    xor r12, r12
    xor r13, r13

    mov r12d, edi
    mov r13b, sil

    xor rdi, rdi
    mov rdi, ARRAY_SIZE                        ; se reserva memoria para la estructura
    call malloc

    mov dword[rax + ARRAY_TYPE_OFFSET], r12d         ; INICALIZAR ATRIBUTOS
    mov byte[rax + ARRAY_SIZE_OFFSET],  INITIAL_SIZE          
    mov byte[rax + ARRAY_CAPACITY_OFFSET], r13b     
    mov r12, rax
        
    shl r13, 3
    xor rdi, rdi
    mov rdi, r13
    call malloc                               ; se reserva memoria para el array de datos
    mov [r12 + ARRAY_DATA_OFFSET], rax         

    mov rax, r12

    pop r13
    pop r12
    pop rbp
    ret

; void* arrayRemove(array_t* a, uint8_t i)
;  {
;    // Verificar si el índice está fuera de rango
;    if (i >= a->size) {
;        return NULL; // Retorna 0 si está fuera de rango
;    } 
;
;    // Obtener el elemento a eliminar
;    void* removedData = a->data[i];
;
;    // Mover los elementos a la izquierda para llenar el espacio vacío
;    for (uint8_t j = i; j < a->size - 1; j++) {
;        a->data[j] = a->data[j + 1];
;    }
;
;    // Disminuir el tamaño del arreglo
;    a->size--;
;
;   // Retornar el elemento eliminado
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
   xor r12, r12
   mov r12b, BYTE [rdi + ARRAY_SIZE_OFFSET] ;Preservo el size
   xor r13, r13 ;limpio r13
   mov r13b, sil  ;guardo indice
   
   ;verifico si esta fuera de rango
   cmp r13b, r12b  
   jge .fueraRango    ; indice >= size

   
   ;obtengo elemento a eliminar
   xor r9,r9
   mov r8, [r14 + ARRAY_DATA_OFFSET]
   mov r9b,  r13b
   shl r9, 3
   mov r15, [r8 + r9] ; elemento para remover

   dec r12              ;decremento size
   cmp r13b, r12b 
   je .devolverUltimo            ;indice = size -1

   
   .loop:
   cmp r13b, r12b 
   jge .devolverUltimo
   
   mov rdi, r14  ;Puntero al array
   xor rsi, rsi
   mov sil, r13b ;indice i
   inc r13b
   xor rdx, rdx
   mov dl, r13b 

   call arraySwap
   jmp .loop

   .fueraRango:
   xor rax, rax
   jmp .fin
   
   .devolverUltimo:
   mov BYTE [r14 + ARRAY_SIZE_OFFSET], r12b ;guardo size decrementado

   xor rax, rax  ;limpio rax
   mov rax, r15
   
   .fin:
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
   mov r11b, BYTE [rdi + ARRAY_SIZE_OFFSET]

   cmp sil, r11b
   jge .fin
   cmp dl, r11b
   jg .fin

   shl rsi, 3
   shl rdx, 3


   mov r8, [r9 + rsi]; Array[i]
   mov r10, [r9 + rdx]; Array[j]

   mov [r9 + rsi], r10
   mov [r9 + rdx], r8

   .fin:
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
    push r14

    xor r13,r13                         ;inicializar contador en 0
    mov rbx, rdi                        ;preservo puntero a struct
    mov r14, [rbx + ARRAY_DATA_OFFSET]  ;preservo PUNTERO A PRIMER ELEMENTO DE LOS DATOS

    mov edi, [rbx + ARRAY_TYPE_OFFSET]  ;preparo param para obtner func de borrar
    call getDeleteFunction
    mov r12, rax                        ;preservo puntero a funcion de borrar

    .for:
        xor rcx, rcx
        mov cl, [rbx + ARRAY_SIZE_OFFSET]
        cmp r13b, cl
        jge .ultimoPaso                             ;si el contador es mayor o igual termino iteracion

        mov rax, r13
        shl rax, 3
        mov rdi, [r14 +rax]                         ;colocar params para eliminar el dato
        call r12
        inc r13
        jmp .for 

    .ultimoPaso:
        mov rdi, [rbx+ARRAY_DATA_OFFSET]
        call free

        mov rdi, rbx
        call free

    .fin:
        pop r14
        pop r13
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

    xor rdi, rdi
    mov edi, [r12 + ARRAY_TYPE_OFFSET] ;
    call getPrintFunction
    mov r14, rax ; Guardo la funcion

    xor rdi, rdi
    mov dil, BYTE[CHAR_OPENING_BRACKET] 
    mov rsi, r13
    call fputc

    xor r15, r15 ;contador


    .loop:
        xor r9, r9
        mov r9b, BYTE [r12 + ARRAY_SIZE_OFFSET]
        cmp r15b, r9b
        jge .fin


        mov r10, r15
        shl r10, 3

        mov r11, [r12 + ARRAY_DATA_OFFSET]
        mov rdi, [r11 + r10]
        mov rsi, r13
        call r14

        xor r8, r8
        mov r8b,BYTE [r12 + ARRAY_SIZE_OFFSET]
        dec r8b
        cmp r15b, r8b
        je .fin


        xor rdi, rdi
        mov dil,BYTE[CHAR_COMA]
        mov rsi, r13
        call fputc
        inc r15
        jmp .loop


    .fin:
    xor rdi, rdi
    mov dil, BYTE [CHAR_CLOSING_BRACKET]
    mov rsi, r13
    call fputc

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

; ** Card **
; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    sub rsp, 8

    mov r12, rdi ; suit
    mov r13, rsi  ; number

    xor rdi, rdi
    mov edi, CARD_SIZE
    call malloc

    mov r14, rax            ;puntero a la estructura card_t A DEVOLVER

    mov rdi, r12
    call strClone
    mov [r14 + CARD_SUIT_OFFSET], rax

    mov rdi, r13
    call intClone
    mov [r14 + CARD_NUMBER_OFFSET], rax

    xor rdi, rdi
    mov edi, TYPE_CARD 
    call listNew
    mov [r14 + CARD_STACKED_OFFSET], rax

    mov rax, r14
    add rsp, 8
    pop r14 
    pop r13
    pop r12
    pop rbp   
    ret

;char* cardGetSuit(card_t* c)
;   return c->suit;
cardGetSuit:
    push rbp
    mov rbp, rsp
    
    mov rax, [rdi + CARD_SUIT_OFFSET]

    pop rbp
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
    push rbp
    mov rbp, rsp
    mov rax, [rdi + CARD_STACKED_OFFSET]
    pop rbp
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
    push rbp
    push r12
    push r13
    push r14
    push r15
    mov rbp, rsp

    xor r12, r12
    xor r13, r13
    mov r12, rdi   ;Guardo A
    mov r13, rsi   ;Guardo B
    mov r14, [r12 + CARD_SUIT_OFFSET] ;suit a
    mov r15, [r13 + CARD_SUIT_OFFSET] ;suit b

    mov rdi, r14
    mov rsi, r15
    call strCmp
    cmp rax,0
    jne .fin

    xor r14, r14
    xor r15, r15
    mov r14, [r12 + CARD_NUMBER_OFFSET] ;number a
    mov r15, [r13 + CARD_NUMBER_OFFSET] ;number b

    mov rdi, r14
    mov rsi, r15
    call intCmp
    
    .fin:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret


;card_t* cardClone(card_t* c)
;   copiaCarta = malloc(sizeof(card_t))
;   copiaSuit = strClone(c->suit)
;   copiaCarta -> suit = copiaSuit
;   copiaNumber = intClone(c->number)
;   copiaCarta -> number = copiaNumber
;   copiaStacked = listClone(c->stacked)
;   copiaCarta->stacked = copiaStacked
;   return copiaCarta
cardClone:
    push rbp
    mov rbp, rsp
    push r12
    push rbx
    
    mov r12, rdi            ;guardar puntero a carta a copiar

    ;reservar memoria para la nueva carta
    xor rdi, rdi
    mov edi, CARD_SIZE
    call malloc
    mov rbx, rax            ;guardar puntero a la nueva carta

    ;copiar suit
    mov rdi,[r12 + CARD_SUIT_OFFSET]
    call strClone
    mov [rbx + CARD_SUIT_OFFSET], rax

    ;copiar num
    mov rdi, [r12+CARD_NUMBER_OFFSET]
    call intClone
    mov [rbx + CARD_NUMBER_OFFSET], rax

    ;copiar stacked
    mov rdi, [r12 + CARD_STACKED_OFFSET]
    call listClone
    mov [rbx + CARD_STACKED_OFFSET], rax

    mov rax, rbx    

    .fin:
    pop rbx
    pop r12
    pop rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
;   listAddFirst(c->stacked, card)
cardAddStacked:
    push rbp
    mov rbp, rsp
    
    mov rcx, [rdi + CARD_STACKED_OFFSET]
    mov rdi, rcx
    call listAddFirst

    pop rbp
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
    sub rsp, 8

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

    add rsp, 8
    pop r12
    pop rbp
    ret