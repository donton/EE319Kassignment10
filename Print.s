; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
	PRESERVE8
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R0,R4-R7, LR}
;binding phase
DIGIT EQU 0
;allocation phase	
	SUB SP, #4
;access phase- implement the modulus function	
	MOV R1, #0x30
	LDR R6, [SP, #DIGIT]
	MOV R6, #0
	STR R6, [SP, #DIGIT]
	;LDR R6, [SP, #DIGIT]
	;ADD R6, R6, R1
	;STR R6, [SP, #DIGIT]
	MOV R2, R0
	MOV R6, R0
	MOV R0, #10
	MOV R4, #0
LOOP
	ADD R4, #1
	MOV R7, R2
	UDIV R2, R2, R0		; divide our given number by 10	
	MUL R3, R2, R0		; multiply back by 10
	SUB R3, R7, R3		; find the remainder	
	MUL R2, R0
	CMP R2, #9
	BLS DONE
	UDIV R2, R0
	;CMP R1, #1
	B NOTCOUNT
	;ADD SP, #-4
	;STR R3, [SP, #DIGIT]
	;B LOOP
NOTCOUNT
	ADD SP, #-4
	STR R3, [SP]			; push that digit on the stack
	B LOOP				
DONE 
	ADD SP, #-4
	STR R3, [SP, #DIGIT]
	;LDR R1, [SP, #DIGIT]
OVER 
	LDR R0, [SP]
	ADD SP, #4
ADJUST	
	ADD R0, #0x30
	;ADD R0, R1
	PUSH {R0-R11}
	BL ST7735_OutChar
	POP {R0-R11}
	ADDS R4, #-1
	BEQ DEAL
	CMP R4, #1
	;BEQ POPLOCAL	
	B OVER
;POPLOCAL
	;LDR R0, [SP, #DIGIT]
	;B ADJUST
;deallocation phase
DEAL 
	ADD SP, #4
	POP {R0,R4-R7, LR}
	BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R4-R8, LR}
	MOV R8, LR
;binding phase- 
PER EQU 0
;allocation phase	
	SUB SP, #4
;access phase- implement the modulus function	
	LDR R6, [SP, #PER]
	MOV R6, #0
	STR R6, [SP, #PER]
	;LDR R6, [SP, #PER]
	;ADD R6, #0x2E
	;STR R6, [SP, #PER]
	MOV R2, R0
	MOV R6, #10000
	CMP R0, R6
	BHS HUGENUM
	MOV R6, R0
	MOV R0, #10
	MOV	R4, #4
LOOP1
	ADD R4, #-1
	MOV R7, R2
	UDIV R2, R2, R0		; divide our given number by 10	
	MUL R3, R2, R0		; multiply back by 10
	SUB R3, R7, R3		; find the remainder	
	MUL R2, R0
	UDIV R2, R0
	ADD SP, #-4
	ADD R3,#0x30
	STR R3, [SP]			; push that digit on the stack
	CMP R4, #0
	BNE LOOP1
	B OUT
HUGENUM MOV R3, #0x2A
	MOV R0, #4
STARLOOP
	ADD SP, #-4
	STR R3, [SP]
	ADDS R0, #-1
	BNE STARLOOP 
OUT 
	MOV R4, #5
OVER1 
	CMP R4, #4
	BEQ PERIOD
	LDR R0, [SP]
	ADD SP, #4
ADJUST1
	PUSH {R0-R11,LR}
	BL ST7735_OutChar
	POP {R0-R11,LR}
	ADDS R4, #-1
	BEQ DEAL1
	B OVER1
PERIOD 
	;LDR R0, [SP, #PER]
	MOV R0, #0x2E
	B ADJUST1
;deallocation phase
DEAL1
	ADD SP, #4
	MOV R0, R8
	POP {R4-R8, LR}
	MOV LR, R0
	BX   LR
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file