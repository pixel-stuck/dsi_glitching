#include "types.h"
#include "code.h"
#include "rt.h"

#include <string.h>
#include <stdlib.h>

#define IPCSYNC (*((vu16*)0x04000180))
#define IPCFIFOCNT (*((vu16*)0x04000184))

#define HEADER_BUF 0x2FF8100

const u8 gNandStage2KeyY[16] = { 0 }; // Bring your own key
const u8 gUnk[20] = { 0 };

void PXI_InitFifoFIRM(void);

void PXI_SendIDByIntf(u16 a1)
{
  IPCSYNC = (a1 << 8) & 0xF00;
}

u32  PXI_RecvIDByIntf( void )
{
  return (u32)(IPCSYNC & 0xF);
}

void  PXI_WaitIDByIntf( u32 id )
{
  while (PXI_RecvIDByIntf() != id)
    {
    }
}

void PXI_SendDataByFifo(u32 tag, u32 *data, u32 len)
{
  u32 i; // r4

  len /= 4;
  *((u32 *)0x2FF8024) = (tag & 0x1F) | (((*((u32 *)0x2FF8024))>>5) & 0x3F) | (len << 6);
  while ( PXIi_SetToFifo(*((u32 *)0x2FF8024)) )
  {
  }
  for ( i = 0; i < len; ++i )
  {
    while ( PXIi_SetToFifo(data[i]) );
  }
}

s32 PXI_RecvDataByFifo(u32 tag, u32 *buf, u32 max_size)
{
  s32 result; // r0
  u32 v7; // r5
  u32 i; // r4

  while ( PXIi_GetFromFifo((u32 *)0x2FF8028) )
    ;
  result = -1;
  if ( (*(u32 *)0x2FF8028 & 0x1F) == tag )
  {
    v7 = *(u32 *)0x2FF8028 >> 6;
    if ( (max_size / 4) >= v7 )
    {
      for ( i = 0; i < v7; ++i )
      {
        while ( PXIi_GetFromFifo(&buf[i]))
        {
        }
      }
      result = 0;
    }
  }
  return result;
}

void PXI_InitFifoFIRM()
{
  u32 enabled = OS_DisableInterrupts();

  IPCFIFOCNT = 0xC008u;

  PXI_SendIDByIntf( 9 );
  PXI_WaitIDByIntf( 7 );
  
  (void)OS_RestoreInterrupts(enabled);
}

void InitMBKsForArm7()
{
  *((vu8*)0x4000247) = 3;
  *((vu8*)0x4004040) = 0x81u;
  *((vu8*)0x4004041) = 0x85u;
  *((vu8*)0x4004042) = 0x89u;
  *((vu8*)0x4004043) = 0x8Du;
  *((vu8*)0x4004044) = 0x81u;
  *((vu8*)0x4004045) = 0x85u;
  *((vu8*)0x4004046) = 0x89u;
  *((vu8*)0x4004047) = 0x8Du;
  *((vu8*)0x4004048) = 0x91u;
  *((vu8*)0x4004049) = 0x95u;
  *((vu8*)0x400404A) = 0x99u;
  *((vu8*)0x400404B) = 0x9Du;
  *((vu8*)0x400404C) = 0x81u;
  *((vu8*)0x400404D) = 0x85u;
  *((vu8*)0x400404E) = 0x89u;
  *((vu8*)0x400404F) = 0x8Du;
  *((vu8*)0x4004050) = 0x91u;
  *((vu8*)0x4004051) = 0x95u;
  *((vu8*)0x4004052) = 0x99u;
  *((vu8*)0x4004053) = 0x9Du;
}

void SetMBKsForArm9()
{
  //180h 14h Global MBK1..MBK5 Slot Settings
  *(vu32*)0x4004040 = *(u32*)(HEADER_BUF+0x380);
  *(vu32*)0x4004044 = *(u32*)(HEADER_BUF+0x384);
  *(vu32*)0x4004048 = *(u32*)(HEADER_BUF+0x388);
  *(vu32*)0x400404C = *(u32*)(HEADER_BUF+0x38C);
  *(vu32*)0x4004050 = *(u32*)(HEADER_BUF+0x390);
  
  //194h 0Ch Local ARM9 MBK6..MBK8 Settings
  *(vu32*)0x4004054 = *(u32*)(HEADER_BUF+0x394);
  *(vu32*)0x4004058 = *(u32*)(HEADER_BUF+0x398);
  *(vu32*)0x400405C = *(u32*)(HEADER_BUF+0x39C);
}

__attribute__ ((section ("entry"))) int SecureMain(void)
{
  InitMBKsForArm7();
  PXI_InitFifoFIRM();
  
  if(PXI_WaitBootResponse() == 9)
  {
    //GCD Boot
  }
  else if(PXI_WaitBootResponse() == 10)
  {
    //NAND Boot
    PXI_RecvDataByFifo(0x12, (u32*)HEADER_BUF, 0x400);
    PXI_NotifyID(8); //Validate Header?
    PXI_SendDataByFifo(0x12, (u32*)gNandStage2KeyY, 16);
    PXI_SendDataByFifo(0x12, (u32*)gUnk, 20);
    
    if(PXI_WaitBootResponse() == 6 && PXI_WaitBootResponse() == 5)
    {
      SetMBKsForArm9();
      PXI_NotifyID(5); //Validate ARM7+ARM9 Bins
      ExitBootrom(*((u32*)(HEADER_BUF+0x228)),0,1);
    }
  }
  else if(PXI_WaitBootResponse() == 8)
  {
    //SPI Boot
  }
  
  //If we get here then we screwed up
  while(1);
}