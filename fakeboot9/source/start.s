.section .start, "x"
.global main
main:
 	MOV             R0, #0x12
 	MSR             CPSR_c, R0
 	LDR             SP, =0x2FF9200
 	MOV             R0, #0x11
 	MSR             CPSR_c, R0
 	LDR             SP, =0x2FF9100
 	MOV             R0, #0x17
 	MSR             CPSR_c, R0
 	LDR             SP, =0x2FF9000
 	MOV             R0, #0x1B
 	MSR             CPSR_c, R0
 	LDR             SP, =0x2FF8F00
 	MOV             R0, #0x13
 	MSR             CPSR_c, R0
 	LDR             SP, =0x2FF8E00
 	MOV             R0, #0x1F
 	MSR             CPSR_c, R0
	LDR             SP, =0x2FFC000
	
	BL InitCP15
	BL MIi_InitMainMemCR
	
	LDR             R0, =SecureMain
	BX R0
.pool