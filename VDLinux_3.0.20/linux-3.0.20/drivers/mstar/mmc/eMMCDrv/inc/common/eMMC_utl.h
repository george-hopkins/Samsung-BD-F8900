////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __eMMC_UTL_H__
#define __eMMC_UTL_H__

#include "eMMC.h"

typedef eMMC_PACK0 struct _eMMC_TEST_ALIGN_PACK {

	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;

} eMMC_PACK1 eMMC_TEST_ALIGN_PACK_t;

extern  U32  eMMC_CheckAlignPack(U8 u8_AlignByteCnt);
extern void  eMMC_dump_mem(unsigned char *buf, int cnt);
extern  U32  eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern  U32  eMMC_ChkSum(U8 *pu8_Data, U32 u32_ByteCnt);
extern  U32  eMMC_PrintDeviceInfo(void);

#endif // __eMMC_UTL_H__
