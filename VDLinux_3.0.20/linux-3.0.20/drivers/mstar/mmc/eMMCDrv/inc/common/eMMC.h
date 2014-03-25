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

#ifndef eMMC_DRIVER_H
#define eMMC_DRIVER_H

//===========================================================
// debug macro
//===========================================================


//=====================================================================================
#include "eMMC_config.h" // [CAUTION]: edit eMMC_config.h for your platform
//=====================================================================================
#include "eMMC_err_codes.h"


//===========================================================
// macro for Spec.
//===========================================================
#define ADDRESSING_MODE_BYTE        1 // 1 byte
#define ADDRESSING_MODE_SECTOR      2 // 512 bytes
#define ADDRESSING_MODE_4KB         3 // 4KB bytes

#define eMMC_SPEED_OLD              0
#define eMMC_SPEED_HIGH             1
#define eMMC_SPEED_HS200            2

#define eMMC_FLAG_TRIM              BIT0
#define eMMC_FLAG_HPI_CMD12         BIT1
#define eMMC_FLAG_HPI_CMD13         BIT2

//-------------------------------------------------------
// Devices has to be in 512B block length mode by default
// after power-on, or software reset.
//-------------------------------------------------------
#define eMMC_SECTOR_512BYTE         0x200
#define eMMC_SECTOR_512BYTE_BITS    9
#define eMMC_SECTOR_512BYTE_MASK    (eMMC_SECTOR_512BYTE-1)

#define eMMC_SECTOR_BUF_16KB        (eMMC_SECTOR_512BYTE * 0x20)

#define eMMC_SECTOR_BYTECNT         eMMC_SECTOR_512BYTE
#define eMMC_SECTOR_BYTECNT_BITS    eMMC_SECTOR_512BYTE_BITS
//-------------------------------------------------------

#define eMMC_ExtCSD_SetBit          1
#define eMMC_ExtCSD_ClrBit          2
#define eMMC_ExtCSD_WByte           3

#define eMMC_CMD_BYTE_CNT           5
#define eMMC_R1_BYTE_CNT            5
#define eMMC_R1b_BYTE_CNT           5
#define eMMC_R2_BYTE_CNT            16
#define eMMC_R3_BYTE_CNT            5
#define eMMC_R4_BYTE_CNT            5
#define eMMC_R5_BYTE_CNT            5
#define eMMC_MAX_RSP_BYTE_CNT       eMMC_R2_BYTE_CNT


//===========================================================
// internal data Sector Address
//===========================================================
#define eMMC_ID_BYTE_CNT           15
#define eMMC_ID_FROM_CID_BYTE_CNT  10
#define eMMC_ID_DEFAULT_BYTE_CNT   11 // last byte means n GB


//===========================================================
// driver structures
//===========================================================
#define FCIE_FLAG_GET_PART_INFO     BIT1
#define FCIE_FLAG_RSP_WAIT_D0H      BIT2 // currently only R1b
#define FCIE_FLAG_DDR_MODE          BIT3
#define FCIE_FLAG_DDR_TUNING        BIT4
#define FCIE_FLAG_SPEED_MASK        (BIT5|BIT6)
#define FCIE_FLAG_SPEED_HIGH        BIT5
#define FCIE_FLAG_SPEED_HS200       BIT6
#define FCIE_FLAG_TESTING           BIT7
#define FCIE_FLAG_PADTYPE_MASK      (BIT8|BIT9)
#define FCIE_FLAG_PADTYPE_DDR       BIT8
#define FCIE_FLAG_PADTYPE_SDR       BIT9
#define FCIE_FLAG_PADTYPE_BYPASS    (BIT8|BIT9)

typedef struct _eMMC_DRIVER
{
    // ----------------------------------------
    // FCIE
    // ----------------------------------------
    U32 u32_Flag, u32_LastErrCode;
    U8  au8_Rsp[eMMC_MAX_RSP_BYTE_CNT];
    U8  au8_CSD[eMMC_MAX_RSP_BYTE_CNT];
    U8  au8_CID[eMMC_MAX_RSP_BYTE_CNT];
    U16 u16_RCA;
    U16 u16_Reg10_Mode;
    U32 u32_ClkKHz;
    U16 u16_ClkRegVal; // for SDR mode

    // ----------------------------------------
    // eMMC
    // ----------------------------------------
    // CSD
    U16 u16_C_SIZE;
    U8  u8_SPEC_VERS;
    U8  u8_R_BL_LEN, u8_W_BL_LEN; // supported max blk len
    U8  u8_TAAC, u8_NSAC, u8_Tran_Speed;
    U8  u8_C_SIZE_MULT;
    U8  u8_ERASE_GRP_SIZE, u8_ERASE_GRP_MULT;
    U8  u8_R2W_FACTOR;

    U8  u8_IfSectorMode;
    U8  u8_Padding;

    U32 u32_eMMCFlag;
    U32 u32_EraseUnitSize;

    // ExtCSD
    U32 u32_SEC_COUNT;
    U8  u8_BUS_WIDTH;
    U8  u8_Padding2;
    U16 u16_ReliableWBlkCnt;

    // CRC
    U32 u32_LastBlkNo;
    U16 u16_LastBlkCRC[16];

} eMMC_DRIVER, *P_eMMC_DRIVER;

extern eMMC_DRIVER g_eMMCDrv;

//===========================================================
// exposed APIs
//===========================================================
#include "drv_eMMC.h"

//===========================================================
// internal used functions
//===========================================================
#include "eMMC_utl.h"
#include "eMMC_hal.h"


#endif // eMMC_DRIVER_H

