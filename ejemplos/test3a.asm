JMP seguir

seguir: SET R0, 0XFF

hay_carry: SIG R0 ; SIG no modifica las flags de la ALU, entonces nunca se da la condición de salto 
           JC hay_carry ; 

halt: JMP halt