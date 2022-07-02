.MODEL SMALL
.STACK 100H
.DATA

; The string to be printed
TEST_STRING DB 'tryyourbestsebruoyyrt', '$'
STRING1 DB 'String is palindrome', '$'
STRING2 DB 'String is not palindrome', '$'
Y DB 'y', '$'

.CODE
MAIN PROC FAR
MOV AX, @DATA
MOV DS, AX


CALL Palindrome
CALL CountY

;interrupt to exit
MOV AH, 4CH
INT 21H
MAIN ENDP
Palindrome PROC

; load the starting address
; of the string
MOV SI,OFFSET TEST_STRING

; traverse to the end of;
;the string
LOOP1 :
	MOV AX, [SI]
	CMP AL, '$'
	JE LABEL1
	INC SI
	JMP LOOP1

;load the starting address;
;of the string
LABEL1 :
	MOV DI,OFFSET TEST_STRING
	DEC SI

	; check if the string is palindrome;
	;or not
	LOOP2 :
	CMP SI, DI
	JL OUTPUT1
	MOV AX,[SI]
	MOV BX, [DI]
	CMP AL, BL
	JNE OUTPUT2

	DEC SI
	INC DI
	JMP LOOP2

OUTPUT1:
	;load address of the string
	LEA DX,STRING1

	; output the string;
	;loaded in dx
	MOV AH, 09H
	INT 21H
	RET

OUTPUT2:
	;load address of the string
	LEA DX,STRING2

	; output the string
	; loaded in dx
	MOV AH,09H
	INT 21H
	RET

Palindrome ENDP


CountY PROC
    ; load the starting address
    ; of the string and y
    MOV SI,OFFSET TEST_STRING    
    MOV DI,OFFSET Y
    MOV DX, 0H    ;Set DX to 0
LOOP3 :
	MOV AX, [SI]
	CMP AL, '$' ;Check end of string, if yes, jump to finish
	JE FINISH
    MOV AX,[SI]
	MOV BX, [DI]
	CMP AL, BL ; Check if AL = Bl (y)
    JE EQUAL ; If equal, jump to EQUAL
	INC SI
	JMP LOOP3
FINISH:
	RET
EQUAL:
    INC DL ; Increase the number of 'y'
    INC SI
	JMP LOOP3
CountY ENDP

END MAIN
