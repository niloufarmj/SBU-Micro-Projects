	AREA RESET, DATA, READONLY
		DCD          0       ;initial_sp
		DCD          MAIN    ;reset_vector
			
	AREA StackArea,DATA,READWRITE
SS SPACE 0x18                ;Allocating 6 words for Stack

COUNTER RN R3 ;Rename R3 to COUNTER
NUMBER RN R0  ;Rename R0 to NUMBER
ONES RN R1	  ;Rename R1 ro ONES
ZEROES RN R2  ;Rename R2 to ZEROES
TEMP RN R6 

	AREA MyCode, CODE,   READONLY
	
MAIN 
   LDR R5, =SS
   ADD R5, R5, #24
   MOV SP, R5
   MOV NUMBER, #1   ;Register Rd
   MOV COUNTER, #0  ;Counter
   
WHILE 
   MOVS NUMBER, NUMBER, LSR #1    ;Logical right shift
   ADDCS ONES, ONES, #1      	  ;ONES = number of ones
   ADDCC ZEROES, ZEROES, #1       ;ZEROES = number of zeros
   ADD COUNTER, COUNTER, #1       ;counter++
   CMP COUNTER, #32
   BNE WHILE
   BL SUBROUTINE
   
HERE
   B HERE ;stay here forever
   
SUBROUTINE 
   PUSH {ONES, ZEROES, LR}
   MOV TEMP, #3
   MUL ZEROES, ONES, TEMP
   MOV TEMP, #100
   SUB TEMP, ZEROES
   MOV COUNTER, TEMP
   POP {ONES, ZEROES, PC}
   