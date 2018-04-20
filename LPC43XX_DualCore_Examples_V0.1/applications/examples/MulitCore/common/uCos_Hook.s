		EXPORT  M0_PendSV_Handler
		EXPORT  PendSV_Handler
		IMPORT  OS_CPU_PendSVHandler

;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************

    PRESERVE8
    THUMB

    AREA CODE, CODE, READONLY

M0_PendSV_Handler
PendSV_Handler
	LDR R0, =OS_CPU_PendSVHandler
	BX  R0
    END