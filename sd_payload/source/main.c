#include <stdint.h>
#include <string.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t vu8;

typedef volatile int32_t vs32;
typedef volatile int16_t vs16;
typedef volatile int8_t vs8;

typedef struct {
  u32 emmc_offset;
  u32 binary_size;
  u32 load_address;
  u32 size_rounded;
} sectionLoadInfo;

typedef struct {
	/* 0x000 */ u8 reserved1[0x20];
	/* 0x020 */ sectionLoadInfo arm9LoadInfo;
	/* 0x030 */ sectionLoadInfo arm7LoadInfo;
	/* 0x040 */ u8 reserved2[0xBF];
	/* 0x0FF */ u8 loadTypeInfo;
	/* 0x100 */ u8 signature[0x80];
	/* 0x180 */ u32 mbkSettings[12];
	/* 0x1B0 */ u8 reserved3[0x50];
} bootloaderHeader;

typedef struct {
	/* 0x000 */ u8 padding[0x200];
	/* 0x200 */ bootloaderHeader header;
} arm9BootloaderInfo;

extern void stopDmaChannelExt(int channel);
extern s32  emmcInitExt(void);
extern void emmcReadExt(u32 nandOfs, void* buf, u32 length);
extern void PXI_SendDataByFifoExt(u32 tag, const void *buf, u32 size);
extern u8   PXI_WaitBootResponseExt(void);
extern s32  PXI_RecvDataByFifoExt(u32 tag, void *buf, u32 size);
extern void SetArm7MBKsExt(const u32 *mbkSettings);
extern void loadBinarySectionExt(const sectionLoadInfo *infoPtr, void *cb, int loadType);
extern void PXI_NotifyIDExt(u32 id);
extern void SetArm9MBKsExt(const u32 *mbkSettings);
extern void miLockWram(const u32 *mbkSettings);
extern void AES_SetKeyYExt(const u32 *key);
extern void PXI_GetBootCmdExt(u32 id);
extern void JumpToArm7(u32 id);
extern void j_ResetInterrupts();
extern void ClearTemporaryMemory();
extern void MIi_CpuCopyFast(void* src, void* dst, u32 len);

static void sendProgress(u32 errcode) {
	u8 temp[0x200];
	emmcReadExt(errcode, &temp, 0x200);
}

#define ASSERT(cond, code) sendProgress(code | ((cond) ? 0x600D0000 : 0x0BAD0000) ); if(!(cond)) { while(1); }

static const sectionLoadInfo gArm9LoadInfo = {
	.emmc_offset =  0x00004400,
	.binary_size =  0x00026410,
	.load_address = 0x037B8000,
	.size_rounded = 0x00026600
};

static const sectionLoadInfo gArm7LoadInfo = {
	.emmc_offset =  0x0002AA00,
	.binary_size =  0x00027588,
	.load_address = 0x037B8000,
	.size_rounded = 0x00027600
};

#define KEYPAD  (*(vu16 *)0x04000130)
#define IPCSYNC (*(vu32 *)0x04000180)

#define SCFG_ROM (*(vu8 *)0x04004000)

extern arm9BootloaderInfo gArm9HeaderData;

void exploit_main(void) {
	/* Clear eMMC port struct */
	memset((void*)0x3FFE6E4,0,68);
	
	/* Reinit so the eMMC is in a good state */
	emmcInitExt();

	/* Stop DMA used for eMMC transfer */
	stopDmaChannelExt(5);

	/* Send arm9 a fake copy of the header */
	PXI_SendDataByFifoExt(0x12, &gArm9HeaderData, 0x400);
	if(PXI_WaitBootResponseExt() != 8) {
		sendProgress(0x0BAD0001);
		while(1);
	}

	/* Recieve bootloader key and some unknown data(SHA1 hash?) */
	u32 nandStage2KeyY[4];
	u32 unkData[5];

	PXI_RecvDataByFifoExt(0x12, nandStage2KeyY, 0x10);
	AES_SetKeyYExt(nandStage2KeyY);
	PXI_RecvDataByFifoExt(0x12, unkData, 0x14);
	
	/* Clear memory necessary for stage2 to boot */
	memset((void*)0x3FFC640,0,0x14);
	memset((void*)0x3FFFFF8,0,8);

	/* Set up arm7 memory regions, load arm7 binary and inform arm9 once done */
	SetArm7MBKsExt(gArm9HeaderData.header.mbkSettings);
	loadBinarySectionExt(&gArm7LoadInfo, NULL, 1);
	PXI_NotifyIDExt(6);

	/* Set up arm9 memory regions, load arm9 binary and inform arm9 once done */
	SetArm9MBKsExt(gArm9HeaderData.header.mbkSettings);
	loadBinarySectionExt(&gArm9LoadInfo, NULL, 1);
	PXI_NotifyIDExt(5);

	/* the arm9 settings overwrote ours. Put them back */
	SetArm7MBKsExt(gArm9HeaderData.header.mbkSettings);

	/* Wait for arm9 to finish */
	if(PXI_WaitBootResponseExt() != 5) {
		sendProgress(0x0BAD0002);
		while(1);
	}

	/* Copy out data for arm7 bootloader */
	MIi_CpuCopyFast((void *)0x8188, (void *)0x3FFC400, 0x200);
	MIi_CpuCopyFast((void *)0xB5D8, (void *)0x3FFC600, 0x40);
	MIi_CpuCopyFast((void *)0xC6D0, (void *)0x3FFC654, 0x1048);
	MIi_CpuCopyFast((void *)0xD718, (void *)0x3FFD69C, 0x1048);
	
	/* Reset Interrupts */
	j_ResetInterrupts();
	
	/* Set MBK9 settings from header */
	miLockWram(gArm9HeaderData.header.mbkSettings);

	/* Set unknown key */
	AES_SetKeyYExt((u32 *)0xB638);

	/* Wait for arm9 to notify us it's time to lock the ROM */
	PXI_GetBootCmdExt(4);
	
	/* Boot7 clears out some extra memory */
	ClearTemporaryMemory();

	/* Tell FPGA we're ready to begin glitching */
	sendProgress(0x600);

	/* Use multiple checks of volatile registers to make sure we don't glitch our own payload */
restart_loop:
	while(1) {
		if((KEYPAD & 1) == 0) {
			if((KEYPAD & 1) == 0) {
				if((KEYPAD & 1) == 0) {
					if((KEYPAD & 1) == 0) {
						/* Lock arm9 bootrom */
						SCFG_ROM |= 1;
					}
				}
			}
		}

		if((IPCSYNC & 0xF) == 3) {
			if((IPCSYNC & 0xF) == 3) {
				if((IPCSYNC & 0xF) == 3) {
					if((IPCSYNC & 0xF) == 3) {
						break;
					}
				}
			}
		}
	}

	/* Just to be extra sure */
	if((IPCSYNC & 0xF) != 3) {
		goto restart_loop;
	}

	/* Tell FPGA we successfully broke out of arm9's loop */
	sendProgress(0x800);

	/* Jump to arm7 bootloader */
	JumpToArm7(gArm9HeaderData.header.arm7LoadInfo.load_address);
}