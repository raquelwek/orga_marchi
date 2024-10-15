start:
SET R0, 0x00
SET R1, 0x00
SET R2, 0x08
SET R3, prueba
ADD R0, R1
JRZ R3 

halt: JMP halt

prueba:
ADD R0,R2
