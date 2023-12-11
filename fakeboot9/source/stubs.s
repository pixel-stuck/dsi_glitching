.section .text, "x"
.arm
.global ExitBootrom
ExitBootrom:
	LDR R3, =0xFFFF4B30+1
	BX R3
	
.global PXI_NotifyID
PXI_NotifyID:
	LDR R1, =0xFFFF4BA4+1
	BX R1
	
.global PXI_WaitBootResponse
PXI_WaitBootResponse:
	LDR R1, =0xFFFF4BB0+1
	BX R1
	
.global PXIi_GetFromFifoSecure
PXIi_GetFromFifoSecure:
	LDR R2, =0xFFFF4BC4+1
	BX R2
	
.global PXIi_GetFromFifo
PXIi_GetFromFifo:
	LDR R2, =0xFFFF4C5C+1
	BX R2
	
.global PXIi_SetToFifo
PXIi_SetToFifo:
	LDR R2, =0xFFFF4CA0+1
	BX R2

.pool
	