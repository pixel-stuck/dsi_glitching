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

extern void stopDmaChannelExt(int channel);
extern s32  sdmcResetExt(void);
extern s32  initSdCardExt(void);
extern void emmcReadExt(u32 nandOfs, void* buf, u32 length);

#define ALIGN(x, alignment) ((x + (alignment - 1)) & ~(alignment - 1))

void exploit_main(void) {
  /* Clear eMMC port struct */
  memset((void*)0x3FFE6E4,0,68);

  /* Stop DMA used for eMMC transfer */
  stopDmaChannelExt(5);
  
  /* Reset controller to a fresh state */
  sdmcResetExt();

  /* Initialize SD card */
  initSdCardExt();

  /* Read payload header from SD card */
  u8 headerBuf[0x200];
  emmcReadExt(0x200, headerBuf, 0x200);

  /* Read size and sector to read from */
  u32 size = headerBuf[3] << 24 | headerBuf[2] << 16 | headerBuf[1] << 8 | headerBuf[0];
  u32 sector = headerBuf[7] << 24 | headerBuf[6] << 16 | headerBuf[5] << 8 | headerBuf[4];
  uintptr_t loadAddress = headerBuf[11] << 24 | headerBuf[10] << 16 | headerBuf[9] << 8 | headerBuf[8];

  /* Read payload */
  emmcReadExt(sector, loadAddress, ALIGN(size, 0x200));

  /* Jump to external payload */
  jumpToBinary(loadAddress);
}