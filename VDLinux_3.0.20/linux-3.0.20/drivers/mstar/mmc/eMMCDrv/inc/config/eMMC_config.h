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

#ifndef __eMMC_CONFIG_H__
#define __eMMC_CONFIG_H__

#define UNIFIED_eMMC_DRIVER             1

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//   - search and check all [FIXME] if need modify or not
//=====================================================
#ifdef CONFIG_MSTAR_AMBER3
#define eMMC_DRV_X10P_LINUX             1
#endif
#ifdef CONFIG_MSTAR_EDISON
#define eMMC_DRV_X12_LINUX              1
#endif


//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
  #include "eMMC_X10P_linux.h"
#elif defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
  #include "eMMC_X12_linux.h"
#else
  #error "Error! no platform selected."
#endif

#if (defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX) || (defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX)
#define eMMC_DRV_TV_LINUX               1
#endif

//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define eMMC_DMA_RACING_PATCH           1
#define eMMC_DMA_PATCH_WAIT_TIME        DELAY_10ms_in_us
#define eMMC_DMA_RACING_PATTERN0        (((U32)'M'<<24)|((U32)0<<16)|((U32)'S'<<8)|(U32)1)
#define eMMC_DMA_RACING_PATTERN1        (((U32)'T'<<24)|((U32)6<<16)|((U32)'A'<<8)|(U32)8)


//===========================================================
// Time Dalay, do NOT edit the following content
//===========================================================
#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
#define TIME_WAIT_DAT0_HIGH             (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT   (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FIFOCLK_RDY           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_CMDRSP_END            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_1_BLK_END             (HW_TIMER_DELAY_1s*5)
#define TIME_WAIT_n_BLK_END             (HW_TIMER_DELAY_1s*10) // safe for 512 blocks
#else
#define TIME_WAIT_DAT0_HIGH             HW_TIMER_DELAY_1s
#define TIME_WAIT_FCIE_RESET            HW_TIMER_DELAY_10ms
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT   HW_TIMER_DELAY_1us
#define TIME_WAIT_FIFOCLK_RDY           HW_TIMER_DELAY_10ms
#define TIME_WAIT_CMDRSP_END            HW_TIMER_DELAY_10ms
#define TIME_WAIT_1_BLK_END             HW_TIMER_DELAY_1s
#define TIME_WAIT_n_BLK_END             HW_TIMER_DELAY_1s // safe for 512 blocks
#endif


#endif /* __eMMC_CONFIG_H__ */
