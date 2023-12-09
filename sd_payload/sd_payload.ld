ENTRY(_start)

MEMORY 
{
	ram (rwx) : ORIGIN = 0x037ffe00, LENGTH = 0x4200
}

SECTIONS
{
	PROVIDE(_start = 0x03800000);
	.text : {
		LONG((SIZEOF(.text) - 0x200) + SIZEOF(.rodata) + SIZEOF(.data))
		LONG(0x400)
		LONG(_start)
		. = ALIGN(0x200);
		*(.text._start)
		*(.text*)
		. = ALIGN(4);
	} >ram

	.rodata : {
		main.o(.rodata*)
		. = ALIGN(4);
	} >ram

	.data : {
		*(.data*)
		. = ALIGN(4);
	} >ram

	.bss (NOLOAD) :  {
		__bss_start = .;
		*(.bss* COMMON*)
		. = ALIGN(4);
	} >ram
	__bss_end = .;
}

gArm9HeaderData = 0x03FFC000;
sdmcReset = 0xB4D9;
initSdCard = 0xB403;
stopDmaChannel = 0x9991;
emmcInit = 0xB4B5;
emmcRead = 0xA8F5;
PXI_SendDataByFifo = 0xA1E1;
PXI_WaitBootResponse = 0x5609;
PXI_RecvDataByFifo = 0xA18D;
SetArm7MBKs = 0x9881;
loadBinarySection = 0xAA61;
PXI_NotifyID = 0x55F9;
SetArm9MBKs = 0x9895;
AES_SetKeyY = 0x86BD;
PXI_GetBootCmd = 0x5695;
ClearTemporaryMemory = 0xFB4;
JumpToArm7 = 0xFD8;
ResetInterrupts = 0xA001;
i_miLockWram = 0x5555;
MIi_CpuCopyFast = 0x10E0;