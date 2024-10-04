# Taller de Microarquitectura
# Organización del Computador

## Secuenciales
## OrgaSmall
### 1. Análisis
- Características generales de **OrgaSmall**

El tamaño de la memoria presente en el Procesador es de 256 palabras como máximo, pues el tamaño de palabra manejado por todo el microprocesador es de 8 bits (tamaño del bus de datos).

Por otro lado el  *Program Counter*  es aquel que guarda la dirección de la próxima instrucción a ser ejercutada, y por lo antes dicho para que el mismo pueda generar 256 direcciones diferentes, es necesario que su tamaño sea de 8 bits.

Dado que el *opCode* es un código único representado por 5 bits para cada instrucción implementada, en total nuestra máquina **OrgaSmall** podría tener 2<sup>5</sup> = 32 códigos instrucciones. En nuestro caso ya tenemos 21 instrucciones por ende podríamos implementar 11 instrucciones más.

- Funcionamientos particulares de algunas componentes
    1. *Contador del programa*

    En primer lugar, la señal `inc` del circuito **`PC`** se encarga de incrementar +1 el valor actual del *Program Counter*, dicha señal sirve para leer ambas partes de la instrucción que deseemos posteriormente decodificar, esto es así porque el tamaño de palabra manejado es de 1 byte y cada instrucción ocupa 2bytes.

    2.  *Unidad Aritmético Lógica*

    Por otro lado la **`ALU`**, tiene una señal `opW` la cual es últil para indicar si las flags obtenidas de alguna operación deben ser escritas o no, en base a lo que sea necesario por el para cada operación.

    3. *Data Path*

    `DE_enOutImm` es la señal de control que permite que permite activar la salida del valor del bus de datos inmediato, pues todos los componentes deben escuchar al bus  de datos en algún momento para hacer sus operaciones, sin embargo no al mismo tiempo ya que implicaría el mal funcionamiento del procesdor.

    Asímismo las señales `RB_enIn`, `RB_enOut`, `RB_selectIndexIn` y `RB_selectIndexOut` son las que nos permiten almacenar (escribir) u obtener (leer) daros del *Banco de Registros*, en particular cada una:

    - `RB_enIn`: Habilita la entrada del bus de datos del registro seleccionado.
    - `RB_enOut`: Habilita la salida al bus de datos del registro seleccionado.
    - `RB_selectIndexIn`: Selecciona qué registro se escribirá.
    - `RB_selectIndexOut`: Selecciona qué registro se leerá.


    4.  *Unidad de Control*

    Partiendo de la base de que en el funcionamiento general esperado por nuestro procesador es el siguiente:
    ```mermaid
    graph TD;
        Fetch --> Decode;
        Decode --> Execute;
        Execute --> Fetch;
    ```

    Que más a bajo nivel se determina por la unidad de control de la siguiente manera:
    Comúnmente si no se activa ninguna señal interna de la unidad de control para hacer un salto, el `OP_code` se resetea en su valor de inicio 00000.
    Mientras que, la unidad de control usará las señales internas `jc_microOp`,`jz_microOp` y `jn_microOp` para determinar si se quiere hacer un salto condicional; como así también, las flags resultantes de la ALU pues, es necesario verificar que las condiciones de salto sean verdaderas o en otras opalabras que las señales se corresponden a la flag de condición. De esta manera, si no se cumplen las condiciones para saltar a la instrucción que deseamos se incrementa +1 al microPC con el fin de que caiga en la microinstrucción `reset_microOp` y continúe el loop común (vuelve al fetch); caso contrario, le suma +2 para evitar justamente la línea antes mencionada y habilitar el `PC_load` para cargar la línea siguiente que esté mismo debe de ejecutar para que finalmente se continúe con el flujo normal del procesador.

    Diagrama de Flujo de saltos:
    ```mermaid
    graph TD;
        A["¿Quiero hacer un salto?"] -->|Sí| B["¿La señal se corresponde con la flag coondicional?"];
        A -->|No| D["Cargar Opcode con 00000"];
        B -->|Sí| E["Incremento +2 en microOP"];
        B -->|No| F["Incremento +1 en microOP"];
    ```

## 2. Ensamblar y correr
- **Comportamiento esperado antes de correr el programa**:

  Dado el código ASM podemos predecir que el comportamiento del programa será el siguiente.

  Al inicio, por medio de la instrucción **JMP**, el `PC` apuntará hacia la dirección de memoria de la etiqueta `seguir`.

  En líneas generales, se carga en el registro  `R0` el valor `0xFF` por medio de la instrucción **SET**.
  Luego, con la misma instrucción se carga en `R1` el valor `0x11`. 
  
  La idea, entonces es que se sumen los valores almacenados en ambos registros, por medio de la instrucción **ADD**. El resultado se almacenará en `R0`, y según si la operación encendió o no la señal `flag_C`, la instrucción del **ADD** se repetirá o no. 
  
  En caso de que `flag_C = 0` el `PC` apuntará hacia la instrucción `JMP halt`, que cargará en el `PC` la dirección de `halt`.Caso contrario, se vuelve a la instrucción del **ADD** 

  En particular, si `R0 <- 0xFF` y `R1 <- 0x11`, al ejecutarse el **ADD** `R0 <- 0x10` y `flag_C = 1`. Como se encendió la señal de *carry*, el `PC` apuntará hacia la instrucción **ADD** y la misma se ejecutará de nuevo, con `R0 <- 0x10`. Ahora, una vez que se hace la suma, `R0 <- 0x21` y ahora `flag_C = 0`. Esta vez no hay *carry*, entonces el `PC` apunta hacia la siguiente instrucción, que carga en el PC el valor de la etiqueta `halt`.

- **Memoria e instrucciones. Valores de las etiquetas:**

  Teniendo en cuenta que en el micro **OrgaSmall** cada instrucción se codifica con 16 bits, podemos saber qué lugar ocupará cada instrucción en la memoria:

  | **Dirección** |**Instrucción**          | 
  | ------------- | ------------------------|
  | 0x00          | JMP `seguir`            |
  | 0x02          | `seguir`: SET R0, 0XFF  |
  | 0x04          | set R1, 0X11            |
  | 0X06          | `siguiente`: ADD R0,R1  |
  | 0x08          | JC `siguiente`          |
  | 0x0A          | `halt`: JMP `halt`      |

  Ahora que ya definimos qué lugar ocupa en memoria cada instrucción, podemos reemplazar las etiquetas con sus respectivos valores:
  - `seguir` <-- 0x02
  - `siguiente` <-- 0x06
  - `halt` <-- 0x0A

  Entonces,por ejemplo, la instrucción `JMP seguir` se puede "leer" como: `PC <- 0x02`. Es decir, la próxima instrucción a ejecutarse será aquella asociada con la dirección de memoria 0x02, en este caso es: `SET R0, 0XFF`.
  De manera similar, la instrucción `JC siguiente` sería: `PC <- 0x06`. Esto implica que si corresponde, la próxima instrucción a ejecutar será aquella asociada con la dirección 0x06. 

- **Ejecutar y controlar. Ciclos de clock**

  Podemos determinar cuántos ciclos de clock son necesarios para que el código llegue a la instrucción `JMP halt` analizando el microcódigo y el flujo del programa.

  Observando el archivo *microCode.ops* sabemos que cada linea de microcódigo se corresponde con un clock.
  Como se mencionó anteriormente, el funcionamiento del **OrgaSmall** viene dado por  el ciclo de instrucción *fetch, decode y execute*. Por lo tanto, para toda instrucción de nuestro micro se llevará a cabo el ciclo de *fetch y decode* el cual tarda lo mismo en cualquier caso.

  Dicho lo anterior, analicemos cuántos ciclos de clock se necesitan para el *fetch y decode*:

  `PC_enOut MM_enAddr` -> habilita la salida del PC y permite que se lea del componente de Memory (*Fetch*)

  `MM_enOut DE_loadH PC_inc` -> carga la parte alta de la instrucción y se incrementa el PC (*Decode*)

  `PC_enOut MM_enAddr` -> habilita que se cargue el valor del PC en el path y carga la direccion en Memory (*Fetch*)

  `MM_enOut DE_loadL PC_inc` -> carga la parte baja de la instrucción y se incrementa el PC (*Decode*)

  `load_microOp` -> se redirige el flujo del programa para ejecutar la instrucción

  `reset_microOp`

  Entonces, vemos que el ciclo de *fetch y decode* requiere de 5 clocks, pues la microinstrucción `load_microOp` redirige el flujo del programa para realizar el `execute` de la instrucción correspondiente. Como cada instrucción en el *microCode.ops* finaliza con la línea `reset_microOp`, dicha línea no llegará a ejecutarse en el *fetch y decode*.

  Entonces las intrucciones a ejecutarse con sus respectivos clocks son:
    - **JMP**: 2 clocks + 5 clocks = 7 clocks
    - **SET**: 2 clocks + 5 = 7 clocks
    - **ADD**: 5 clocks + 5 = 10 clocks -> *se ejecuta 2 veces* = 20 clocks
    - **JC**:  4 clocks + 5 = 9 clocks -> *se ejecuta 2 veces* = 18 clocks

  Entonces, el flujo del programa sería: 

 
  
    ```mermaid
      stateDiagram-v2
        direction LR
        [*] --> JMP_seguir: Fetch-Decode
        JMP_seguir --> SET_R0,0xFF: F-D

        note right of SET_R0,0xFF
             Asociada con la etiqueta seguir
        end note

        SET_R0,0xFF--> SET_R1,0x11 : F-D
        SET_R1,0x11 --> ADD_R0,R1 : F-D

        note right of ADD_R0,R1
             Asociada a la etiqueta siguiente
        end note

        ADD_R0,R1 --> JC_siguiente : F-D

        note right of JC_siguiente
            En este punto, flac_C = 1
        end note

    ```

    ```mermaid
      stateDiagram-v2
        direction LR
        JC_siguiente --> ADD_R0,R1: F-D

        note left of JC_siguiente
            Recordar que: flag_C = 1
        end note

        ADD_R0,R1 --> JC_siguiente_: F-D
        JC_siguiente_ --> JMP_halt: F-D

        note left of JC_siguiente_
            En este punto flag_C = 0
        end note

        JMP_halt --> [*]

    ```
  Por lo tanto, para llegar a la instrucción `JMP halt` son necesarios:
  *7 + 7 + 20 + 18* -> 52 clocks

- **Microinstrucciones necesarias para realizar el ADD y para realizar el salto**

  Sabemos que en el micro **OrgaSmall** el ciclo de instrucción es *fetch, decode y execute* para ejecutar cualquier instrucción. A partir de la información del *microCode.ops* vemos que el ciclo de *fetch y decode* requiere 5 microinstrucciones. Entonces, para realizar el **ADD** serán necesarias: 5 microinstrucciones para el *fetch y decode* + 5 microinstrucciones para el *execute*, en total 10 microinstrucciones.
  Luego, para realizar el **JMP** se necesitan 5 microinstrucciones (*fetch y decode*) y otras 2 microinstrucciones (*execute*), es decir 7 en total.



