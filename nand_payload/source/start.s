.section .text._start

.arm

.global _start
_start:
  /* Disable interrupts */
  ldr r0, =0x04000000
  mov r1, #0
  str r1, [r0, #0x208]
  str r1, [r0, #0x210]
  str r1, [r0, #0x218]

  /* Clear BSS */
  ldr r0, =__bss_start
  ldr r1, =__bss_end
  mov r2, #0
clear_bss_loop:
  str r2, [r0], #4
  cmp r0, r1
  bne clear_bss_loop

  /* Jump to C main() */
  ldr lr, =_start
  ldr r0, =exploit_main
  bx r0

.pool

.thumb

.global jumpToBinary
jumpToBinary:
  bx r0

.global emmcReadExt
emmcReadExt:
  ldr r3, =emmcRead
  bx r3

.global emmcInitExt
emmcInitExt:
  ldr r3, =emmcInit
  bx r3

.global PXI_SendDataByFifoExt 
PXI_SendDataByFifoExt:
  ldr r3, =PXI_SendDataByFifo
  bx r3

.global PXI_WaitBootResponseExt
PXI_WaitBootResponseExt:
  ldr r0, =PXI_WaitBootResponse
  bx r0

.global PXI_RecvDataByFifoExt
PXI_RecvDataByFifoExt:
  ldr r3, =PXI_RecvDataByFifo
  bx r3

.global SetArm7MBKsExt
SetArm7MBKsExt:
  ldr r1, =SetArm7MBKs
  bx r1

.global loadBinarySectionExt
loadBinarySectionExt:
  ldr r3, =loadBinarySection
  bx r3

.global PXI_NotifyIDExt
PXI_NotifyIDExt:
  ldr r3, =PXI_NotifyID
  bx r3

.global SetArm9MBKsExt
SetArm9MBKsExt:
  ldr r3, =SetArm9MBKs
  bx r3

.global AES_SetKeyYExt
AES_SetKeyYExt:
  ldr r3, =AES_SetKeyY
  bx r3

.global PXI_GetBootCmdExt
PXI_GetBootCmdExt:
  ldr r3, =PXI_GetBootCmd
  bx r3

.global stopDmaChannelExt
stopDmaChannelExt:
  ldr r3, =stopDmaChannel
  bx r3

.global j_ResetInterrupts
j_ResetInterrupts:
  ldr r3, =ResetInterrupts
  bx r3

.global miLockWram
miLockWram:
  ldr r3, =i_miLockWram
  bx r3

.global sdmcResetExt
sdmcResetExt:
  ldr r3, =sdmcReset
  bx r3

.global initSdCardExt
initSdCardExt:
  push {r3-r7,lr}
  ldr  r0, =0x3FFE6E4
  ldr  r1, =0x400
  ldr  r3, =initSdCard
  bx   r3

.global delay
delay:
  bx pc
.arm
delay_arm:
  subs r0, r0, #2
  bcs delay_arm
  bx lr

.pool