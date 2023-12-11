.section .text, "x"
.arm
	
.global InitCP15
InitCP15:
	MRC             p15, 0, R0,c1,c0, 0
	LDR             R1, =0xF9005
	BIC             R0, R0, R1
	MCR             p15, 0, R0,c1,c0, 0
	MOV             R0, #0
	MCR             p15, 0, R0,c7,c5, 0
	MCR             p15, 0, R0,c7,c6, 0
	MCR             p15, 0, R0,c7,c10, 4
	MOV             R0, #0x20
	MCR             p15, 0, R0,c9,c1, 1
	LDR             R0, =0x2FF8000
	ORR             R0, R0, #0xA
	MCR             p15, 0, R0,c9,c1, 0
	MRC             p15, 0, R0,c1,c0, 0
	LDR             R1, =0x50078
	ORR             R0, R0, R1
	MCR             p15, 0, R0,c1,c0, 0
	BX              LR
.pool

.arm
.global MIi_InitMainMemCR
MIi_InitMainMemCR:
	MOV             R12, LR
	MOV             R0, #0x8000
	BL              OS_SpinWait
	LDR             R3, =0x4000204
	MOV             R1, #0x2000
	STRH            R1, [R3]
	MOV             R0, #0x8000
	BL              OS_SpinWait
	LDR             R3, =0x2FFFFFE
	LDR             R0, =0xFFDF
	LDR             R1, =0xE732
	LDR             R2, =0x27E57FE
	LDRH            LR, [R3]
	STRH            LR, [R3]
	STRH            LR, [R3]
	STRH            R0, [R3]
	STRH            R1, [R3]
	LDRH            LR, [R2]
	LDR             R3, =0x4000204
	MOV             R1, #0x6000
	STRH            R1, [R3]
	BX              R12
.pool
	