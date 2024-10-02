# Taller de Microarquitectura
# Organización del Computador

## Secuenciales
## OrgaSmall
### 1. Análisis
- Características generales de **OrgaSmall**
El tamaño de la memoria presente en el Procesador es de 256 palabras como máximo, pues el tamaño de palabra manejado por todo el microprocesador es de 8 bits.

Por otro lado el * *Program Counter* * es aquel que guarda la dirección de la próxima instrucción a ser ejercutada, y por lo antes dicho para que el mismo pueda generar 256 direcciones diferentes, es necesario que su tamaño sea de 8 bits.

Dado que el * *opCode* * es un código único representado por 5 bits para cada instrucción implementada, en total nuestra máquina **OrgaSmall** podría tener 2^5 = 32 códigos instrucciones. En nuestro caso ya tenemos 21 instrucciones por ende podríamos implementar 11 instrucciones más.

- Funcionamientos particulares de algunas componentes
En primer lugar, la señal `inc` del circuito PC se encarga de incrementar +1 el valor actual del registro, dicha señal sirve para poder leer las instrucciones completas ya que en este caso de ocupan 2 palabras.

Por otro lado la ALU, tiene una señal `opW` la cual es últil para indicar si las flags obtenidas de alguna operación deben ser escritas o no, en base a lo que sea necesario por el microprocesador.

## 2. Ensamblar y correr
- **Comportamiento esperado antes de correr el programa**:
Dado el código ASM podemos predecir que el comportamiento del programa será el siguiente.
Al inicio, por medio de la instrucción **JMP**, el `PC` apuntará hacia la dirección de memoria de la etiqueta `seguir`. 
En líneas generales, se carga en el registro  `R0` el valor `0xFF` por medio de la instrucción **SET**.
Luego, con la misma operación se carga en `R1` el valor `0x11`. La idea, entonces es que se sumen los valores almacenados en ambos registros, por medio de la instrucción **ADD**. El resultado se almacenará en `R0`, y según si la operación encendió o no la señal `flag_C`, la instrucción del **ADD** se repetirá o no. En caso de que `flag_C = 0` el `PC` apuntará hacia la instrucción `JMP halt`, que cargará en el `PC` la dirección de `halt`.Caso contrario, se vuelve a la instrucción del **ADD** 

En particular, si `R0 <- 0xFF` y `R1 <- 0x11`, al ejecutarse el **ADD** `R0 <- 0x10` y `flag_C = 1`. Como se encendió la señal de *carry*, el `PC` apuntará hacia la instrucción **ADD** y la misma se ejecutará de nuevo, con `R0 <- 0x10`. Ahora, una vez que se hace la suma, `R0 <- 0x22` (considerando el *carry*) y ahora `flag_C = 0`. Esta vez no hay *carry*, entonces el `PC` apunta hacia la siguiente instrucción, que carga en el PC el valor de la etiqueta `halt`.
