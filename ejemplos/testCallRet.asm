start:
SET R0, 0x08
SET R1, 0x08
CALL R7, suma

halt: JMP halt

suma:
ADD R0, R1
RET R7
