COUNTER RN R0 ;Rename R0 to COUNTER
SUM RN R1 ;Rename R1 to SUM
I RN R2 ;Rename R2 to I
	AREA RESET, CODE, READONLY
	ENTRY
	MOV SUM, #0x0 ;sum = 0
	MOV I, #0x01 ; I = 1
	MOV COUNTER, #0x08 ;COUNTER = 8
	
LOOP ; Start of loop
	CMP I, COUNTER  ;Compare I and COUNTER
	BGT HERE ; Jump to HERE ( end of programm ) if I > COUNTER
	ADD SUM, I ; SUM = SUM + I
	ADD I, #0x01 ; I = I + 1 
	B LOOP
HERE ; The value of the result of the addition operation is stored in SUM ( R1 )  
	B HERE ;stay here forever
END