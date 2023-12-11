.thumb
.section .reset, "x"
.global ResetInterrupts
ResetInterrupts:
	LDR     R0, =0x4000200
    LDRH    R1, [R0,#8]
    MOV     R1, #0
    STRH    R1, [R0,#8]
    STR     R1, [R0,#0x10]
    SUB     R2, R1, #1
    STR     R2, [R0,#0x14]
    BX      LR
.pool
