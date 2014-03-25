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

#ifndef __eMMC_ERR_CODES_H__
#define __eMMC_ERR_CODES_H__
//===========================================================
// device status (R1, R1b)
//===========================================================
#define eMMC_R1_ADDRESS_OUT_OF_RANGE    BIT31
#define eMMC_R1_ADDRESS_MISALIGN        BIT30
#define eMMC_R1_BLOCK_LEN_ERROR         BIT29
#define eMMC_R1_ERASE_SEQ_ERROR         BIT28
#define eMMC_R1_ERASE_PARAM             BIT27
#define eMMC_R1_WP_VIOLATION            BIT26
#define eMMC_R1_DEVICE_IS_LOCKED        BIT25
#define eMMC_R1_LOCK_UNLOCK_FAILED      BIT24
#define eMMC_R1_COM_CRC_ERROR           BIT23
#define eMMC_R1_ILLEGAL_COMMAND         BIT22
#define eMMC_R1_DEVICE_ECC_FAILED       BIT21
#define eMMC_R1_CC_ERROR                BIT20
#define eMMC_R1_ERROR                   BIT19
#define eMMC_R1_CID_CSD_OVERWRITE       BIT16
#define eMMC_R1_WP_ERASE_SKIP           BIT15
#define eMMC_R1_ERASE_RESET             BIT13
#define eMMC_R1_CURRENT_STATE           (BIT12|BIT11|BIT10|BIT9)
#define eMMC_R1_READY_FOR_DATA          BIT8
#define eMMC_R1_SWITCH_ERROR            BIT7
#define eMMC_R1_EXCEPTION_EVENT         BIT6
#define eMMC_R1_APP_CMD                 BIT5

#define eMMC_ERR_R1_31_24               (eMMC_R1_ADDRESS_OUT_OF_RANGE| \
                                         eMMC_R1_ADDRESS_MISALIGN| \
                                         eMMC_R1_BLOCK_LEN_ERROR| \
                                         eMMC_R1_ERASE_SEQ_ERROR| \
                                         eMMC_R1_ERASE_PARAM| \
                                         eMMC_R1_WP_VIOLATION| \
                                         eMMC_R1_LOCK_UNLOCK_FAILED)
#define eMMC_ERR_R1_23_16               (eMMC_R1_COM_CRC_ERROR| \
                                         eMMC_R1_ILLEGAL_COMMAND| \
                                         eMMC_R1_DEVICE_ECC_FAILED| \
                                         eMMC_R1_CC_ERROR| \
                                         eMMC_R1_ERROR| \
                                         eMMC_R1_CID_CSD_OVERWRITE)
#define eMMC_ERR_R1_15_8                (eMMC_R1_WP_ERASE_SKIP| \
                                         eMMC_R1_ERASE_RESET)
#define eMMC_ERR_R1_7_0                 (eMMC_R1_SWITCH_ERROR)

//===========================================================
// driver error codes
//===========================================================
#define eMMC_ST_SUCCESS                 0

#define eMMC_ST_ERR_MEM_CORRUPT         (0x0001 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NOT_ALIGN           (0x0002 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NOT_PACKED          (0x0003 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_DATA_MISMATCH       (0x0004 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_TIMEOUT_WAIT_REG0   (0x0005 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_TIMEOUT_FIFOCLKRDY  (0x0006 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_TIMEOUT_MIULASTDONE (0x0007 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_TIMEOUT_WAITD0HIGH  (0x0008 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_TIMEOUT_STARTBIT    (0x0009 | eMMC_ST_PLAT)

#define eMMC_ST_ERR_BIST_FAIL           (0x0010 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_DEBUG_MODE          (0x0011 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_FCIE_NO_CLK         (0x0012 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_PARAMETER           (0x0013 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NOT_INIT            (0x0014 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_INVALID_PARAM       (0x0015 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_PARTITION_CHKSUM    (0x0016 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NO_PART_INFO        (0x0017 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NO_PARTITION        (0x0018 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NO_OK_DDR_PARAM     (0x0019 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_SAVE_DDRT_FAIL      (0x001A | eMMC_ST_PLAT)
#define eMMC_ST_ERR_DDRT_CHKSUM         (0x001B | eMMC_ST_PLAT)
#define eMMC_ST_ERR_DDRT_NONA           (0x001C | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CIS_NNI             (0x001D | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CIS_PNI             (0x001E | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CIS_NNI_NONA        (0x001F | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CIS_PNI_NONA        (0x0020 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_SDR_DETECT_DDR      (0x0021 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NO_CIS              (0x0022 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_NOT_eMMC_PLATFROM   (0x0023 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_FCIE_NO_RIU         (0x0024 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_INT_TO              (0x0025 | eMMC_ST_PLAT)

#define eMMC_ST_ERR_CMD1                (0x0A00 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD1_DEV_NOT_RDY    (0x0A01 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD2                (0x0A02 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD3_CMD7           (0x0A03 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_R1_31_24            (0x0A04 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_R1_23_16            (0x0A05 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_R1_15_8             (0x0A06 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_R1_7_0              (0x0A07 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD8_CIFD           (0x0A08 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD8_MIU            (0x0A09 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD17_CIFD          (0x0A0A | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD17_MIU           (0x0A0B | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD18               (0x0A0C | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD6                (0x0A0D | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD13               (0x0A0E | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD12               (0x0A0F | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_CIFD          (0x0A10 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_CIFD_WAIT_D0H (0x0A11 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_CIFD_CHK_R1   (0x0A12 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_MIU           (0x0A13 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_MIU_WAIT_D0H  (0x0A14 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD24_MIU_CHK_R1    (0x0A15 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD25               (0x0A16 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD25_WAIT_D0H      (0x0A17 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD25_CHK_R1        (0x0A18 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_CMD9                (0x0A19 | eMMC_ST_PLAT)
#define eMMC_ST_ERR_SEC_UPFW_TO         (0x0A20 | eMMC_ST_PLAT)

#endif /* __eMMC_ERR_CODES_H__ */
