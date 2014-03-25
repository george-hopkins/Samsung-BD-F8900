#ifndef CSR_NAND_R_H__
#define CSR_NAND_R_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c24a0.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/* Operational Modes */
#define MODE_DISABLE()          REG_SET(rNFCONT, 1, 0, 0)
#define MODE_AUTOLOAD()         REG_SET(rNFCONT, 1, 0, 1)
#define MODE_AUTOSTORE()        REG_SET(rNFCONT, 1, 0, 2)
#define MODE_SOFTWARE()         REG_SET(rNFCONT, 1, 0, 3)

/* Ready/nBusy Flag */
#define NAND_READY_CLEAR()      {rNFSTAT = 1 << 13;}
#define NAND_READY_WAIT()       {while (!BIT_CHECK(rNFSTAT, 13)) {}}

/* Command/Address/Data I/O */
#define COMMAND(cmd)            {rNFCMMD = (cmd) & 0xFF;}
#define ADDRESS(addr)           {rNFADDR = (addr) & 0xFF;}
#define DATA_WRITE(data)        {rNFDATA = (data) & 0xFF;}
#define DATA_READ()             (rNFDATA & 0xFF)

/* Error Correction */
#define ECC_RESET()             BIT_SET(rNFCONT, 8)
#define ECC_MAIN_LOCK()         BIT_SET(rNFCONT, 9)
#define ECC_MAIN_UNLOCK()       BIT_CLEAR(rNFCONT, 9)
#define ECC_ERROR_BYTE()        REG_CHECK(rNFESTAT0, 17, 7)
#define ECC_ERROR_BIT()         REG_CHECK(rNFESTAT0, 6, 4)

#define FULL_ADDRESS(_column, _row)     {ADDRESS(_column); \
                                         ADDRESS(_column >> 8); \
                                         ADDRESS(_row); \
                                         ADDRESS(_row >> 8);}


#ifdef __cplusplus
}
#endif

#endif
