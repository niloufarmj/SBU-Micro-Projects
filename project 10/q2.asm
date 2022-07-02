.MODEL SMALL 
.386 
.STACK 64 
.DATA 

	DATA1 DD 00000001H 
	DATA2 DD 00000007H 

	Result DD 0,0       ; Multiplication res 
	
	OUT_NUMBER DD 10000000H
	
	
	OUT_STR DB 11 DUP ('$')

	TEST_STRING DB 'AMIRR HOSSEIN $'
	BUFF DB 8
		 DB ? 
		 DB 8 DUP(0),'$'
		 
	NEW_LINE DB 13,10, '$' 
	
	IN_NUMBER DD 00000000H
.CODE 

; -----------------------------------------


Print_NUM PROC
for_first_16_bit:
	MOV CX,0
    push CX
	PUSH BX
	ADD BX, 2 
	JMP third
for_second_16_bit:
	pop BX
	pop cx
	ADD CX,1
	CMP CX,2
	JE END_OF
	push cx
	push bx
	JMP third
END_OF:
  RET
third:
	   MOV AX, [BX] 
	   MOV CX,4
	   MOV BX, 16
first_loop:
	   MOV DX,0
       DIV BX
	   CMP DL, 09H
	   JG zero_nine
       ADD DL,30H
back_from_A_F:
	   PUSH DX
	   LOOP first_loop
	   JMP CC
zero_nine:
	   ADD DL, 37H
	   JMP back_from_A_F
CC:       
	   MOV CX, 4
LOOP2: 
	   POP AX
	   JMP handle_put_value_in_buffer
back_from_handle:
       LOOP LOOP2	
	   JMP for_second_16_bit
handle_put_value_in_buffer:
	MOV [SI],AL
	 INC SI
	 JMP back_from_handle	
Print_NUM ENDP

Prepare_for_print PROC
   ;save result of multiply in 64 bit data holder with name : result
	MOV SI, OFFSET Result
	MOV [SI], EDX 
	ADD SI,4 
	MOV [SI], EAX 	
        RET
Prepare_for_print ENDP


div_NUMS PROC
MOV EAX,DATA1
MOV EBX,DATA2
DIV EBX
div_Nums ENDP

MULTIPLE_NUMS PROC
	MOV EAX, DATA1
        IMUL DATA2
	CALL Prepare_for_print
	LEA SI, OUT_STR
	LEA BX, Result
	CALL Print_NUM
	LEA DX,OUT_STR
    MOV AH,9
    INT 21H 
LEA SI, OUT_STR
	LEA BX, Result+4
	CALL Print_NUM
	LEA DX,OUT_STR
    MOV AH,9
    INT 21H 
	
	RET
MULTIPLE_NUMS ENDP
	

; ----------------------------------------
MAIN 	PROC FAR 
	
	;Init Data Segment : 
    MOV AX, @DATA 
    MOV DS, AX 
    CALL MULTIPLE_NUMS
	
	JMP Dos_instruction_for_End 
	
MAIN ENDP 

Dos_instruction_for_End:
	MOV AH,4CH
	INT 21H

END MAIN