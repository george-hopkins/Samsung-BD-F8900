#ifndef CSR_NAND_R_H__
#define CSR_NAND_R_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c6410.h"
#include "platform/csr_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Parameters */
#define ECC_MAIN_8BIT_MSG_LEN   512
#define ECC_MAIN_8BIT_ECC_LEN   (13)

/* Commands */
#define CMD_READ            ((0x00) | (0x30 << 8))
#define CMD_READ_COPYBACK   ((0x00) | (0x35 << 8))
#define CMD_READ_RANDOM     ((0x05) | (0xE0 << 8))
#define CMD_READ_STATUS      (0x70)
#define CMD_WRITE           ((0x80) | (0x10 << 8))
#define CMD_WRITE_CACHE     ((0x80) | (0x15 << 8))
#define CMD_WRITE_COPYBACK  ((0x85) | (0x10 << 8))
#define CMD_WRITE_RANDOM     (0x85)
#define CMD_ERASE_BLOCK     ((0x60) | (0xD0 << 8))
#define CMD_READ_ID          (0x90)
#define CMD_RESET            (0xFF)

/* NFCONF */
#define NFCONF_HCLK_BELOW_66MHZ (1 << 30)
#define NFCONF_MSG_LEN_512      (0 << 25)
#define NFCONF_MSG_LEN_24       (1 << 25)
#define NFCONF_ECC_TYPE_1BIT    (0 << 23)
#define NFCONF_ECC_TYPE_4BIT    (2 << 23)
#define NFCONF_ECC_TYPE_8BIT    (1 << 23)
#define NFCONF_TACLS            ((7 - 0) << 12) /* Not optimised */
#define NFCONF_TWRPH0           ((8 - 1) << 8)  /* Not optimised */
#define NFCONF_TWRPH1           ((8 - 1) << 4)  /* Not optimised */
#define NFCONF_RESERVED         (6 << 0)
#define NFCONF_DEFAULT          (NFCONF_MSG_LEN_512 | NFCONF_ECC_TYPE_1BIT)
#define NFCONF_SET(value)       {rNFCONF = NFCONF_TACLS | NFCONF_TWRPH0 | NFCONF_TWRPH1 | NFCONF_RESERVED | value;}

/* ECC Type */
#define ECC_TYPE_1BIT()         REG_SET(rNFCONF, 24, 23, 0)
#define ECC_TYPE_4BIT()         REG_SET(rNFCONF, 24, 23, 2)
#define ECC_TYPE_8BIT()         REG_SET(rNFCONF, 24, 23, 1)

/* NFCONT */
#define NFCONT_ECC_DIRECTION_DECODE     (0 << 18)
#define NFCONT_ECC_DIRECTION_ENCODE     (1 << 18)
#define NFCONT_LOCK_TIGHT               (1 << 17)
#define NFCONT_LOCK_SOFT                (1 << 16)
#define NFCONT_ECC_INTERRUPT            (1 << 12)
#define NFCONT_ECC_STOP                 (1 << 11)
#define NFCONT_ILLEGAL_INTERRUPT        (1 << 10)
#define NFCONT_RNB_INTERRUPT            (1 << 9)
#define NFCONT_RNB_DETECT_RISING        (0 << 8)
#define NFCONT_RNB_DETECT_FALLING       (1 << 8)
#define NFCONT_MECC_UNLOCK              (0 << 7)
#define NFCONT_MECC_LOCK                (1 << 7)
#define NFCONT_SECC_UNLOCK              (0 << 6)
#define NFCONT_SECC_LOCK                (1 << 6)
#define NFCONT_MECC_INIT                (1 << 5)
#define NFCONT_SECC_INIT                (1 << 4)
#define NFCONT_NCS3                     (1 << 2)
#define NFCONT_NCS2                     (1 << 1)
#define NFCONT_ENABLE                   (1 << 0)
#define NFCONT_DEFAULT                  (NFCONT_ECC_DIRECTION_DECODE | \
                                         NFCONT_RNB_DETECT_RISING | \
                                         NFCONT_MECC_LOCK | \
                                         NFCONT_SECC_LOCK | \
                                         NFCONT_NCS3 | \
                                         NFCONT_NCS2)
#define NFCONT_SET(value)               {rNFCONT = value;}

/* Operational Modes */
#define MODE_DISABLE()          BIT_CLEAR(rNFCONT, 0)
#define MODE_ENABLE()           BIT_SET(rNFCONT, 0)

/* Manual Chip Enable Control */
#define CHIP_DISABLE()          BIT_SET(rNFCONT, 1)
#define CHIP_ENABLE()           BIT_CLEAR(rNFCONT, 1)

/* Protected Manual Chip Enable Control */
#define CHIP_ENABLE_ATOMIC()    {INT_ATOMIC_EXECUTE_HEAD(); \
                                 CHIP_ENABLE()
#define CHIP_DISABLE_ATOMIC()   CHIP_DISABLE(); \
    INT_ATOMIC_EXECUTE_TAIL();}

/* ECC Control */
#define ECC_MAIN_RESET()        BIT_SET(rNFCONT, 5)
#define ECC_MAIN_LOCK()         BIT_SET(rNFCONT, 7)
#define ECC_MAIN_UNLOCK()       BIT_CLEAR(rNFCONT, 7)
#define ECC_STOP()              BIT_SET(rNFCONT, 11)
#define ECC_DIRECTION_DECODE()  BIT_CLEAR(rNFCONT, 18)
#define ECC_DIRECTION_ENCODE()  BIT_SET(rNFCONT, 18)

/* ECC Status */
#define ECC_ENCODE_DONE()       BIT_CHECK(rNFSTAT, 7)
#define ECC_ENCODE_DONE_CLEAR() BIT_SET(rNFSTAT, 7)

/* Ready/nBusy Flag */
#define READY()                 BIT_CHECK(rNFSTAT, 0)
#define RNB_DETECT()            BIT_CHECK(rNFSTAT, 4)
#define RNB_DETECT_CLEAR()      BIT_SET(rNFSTAT, 4)

/* Command/Address/Data I/O */
#define COMMAND(cmd)                {rNFCMMD = (CsrUint8) ((cmd) & 0xFF);}
#define ADDRESS(addr)               {rNFADDR = (CsrUint8) ((addr) & 0xFF);}
#define FULL_ADDRESS(column, row)   {ADDRESS(column); \
                                     ADDRESS((column) >> 8); \
                                     ADDRESS(row); \
                                     ADDRESS((row) >> 8); \
                                     ADDRESS((row) >> 16);}
#define DATA_WRITE8(data)           {rNFDATA8 = data;}
#define DATA_READ8()                (rNFDATA8)
#define DATA_WRITE16(data)          {rNFDATA16 = data;}
#define DATA_READ16()               (rNFDATA16)
#define DATA_WRITE32(data)          {rNFDATA32 = data;}
#define DATA_READ32()               (rNFDATA32)

/* Error Correction */
#define ECC_ERROR_BYTE()        REG_CHECK(rNFECCERR0, 17, 7)
#define ECC_ERROR_BIT()         REG_CHECK(rNFECCERR0, 6, 4)

#ifdef __cplusplus
}
#endif

#endif
