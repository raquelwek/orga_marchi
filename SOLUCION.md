# Taller de Microarquitectura
# Organización del Computador

## Secuenciales
## OrgaSmall
### 1. Análisis
    - Características generales de **OrgaSmall**
    El tamaño de la memoria presente en el Procesador es de 256 palabras como máximo, pues el tamaño de palabra manejado por todo el microprocesador es de 8 bits.
    Por otro lado el * *Program Counter* * es aquel que guarda la dirección de la próxima instrucción a ser ejercutada, y por lo antes dicho para que el mismo pueda generar 256 direcciones diferentes, es necesario que su tamaño sea de 8 bits.
    Dado que el * *opCode* * es un código único representado por 5 bits para cada instrucción implementada, en total nuestra máquina **OrgaSmall** podría tener 2^5 = 32 códigos instrucciones. En nuestro caso ya tenemos 21 instrucciones por ende podríamos implementar 11 instrucciones más.

    -  Funcionamientos particulares de algunas componentes
    En primer lugar, la señal `inc` del circuito PC se encarga de incrementar +1 el valor actual del registro, dicha señal sirve para poder leer las instrucciones completas ya que en este caso de ocupan 2 palabras.

    Por otro lado la ALU, tiene una señal `opW` la cual es últil para indicar si las flags obtenidas de alguna operación deben ser escritas o no, en base a lo que sea necesario por el microprocesador.
