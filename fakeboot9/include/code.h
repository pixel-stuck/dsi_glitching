#ifndef CODE_H
#define CODE_H

extern void InitCP15();
extern void MIi_InitMainMemCR();
extern void ExitBootrom(u32 arm9EntryPoint, u32 mbkSettings, u32 loadKeys);
extern u32 OS_DisableInterrupts();
extern u32 OS_RestoreInterrupts(u32 old);
extern s32 PXIi_GetFromFifoSecure(u32 fifotag, u8 *data_buf);
extern s32 PXIi_GetFromFifo(u32 *data);
extern s32 PXIi_SetToFifo(u32 data);
extern u8 PXI_WaitBootResponse();
extern void PXI_NotifyID(u32 id);
extern u8 eng_font_widths_bin[];

#endif