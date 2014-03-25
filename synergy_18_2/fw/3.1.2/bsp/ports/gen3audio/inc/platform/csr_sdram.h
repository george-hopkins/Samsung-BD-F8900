#ifndef CSR_SDRAM_H__
#define CSR_SDRAM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define SDRAM_BASE              0x10000000
#define SDRAM_LENGTH            0x4000000
#define SDRAM_TOP               (SDRAM_BASE + SDRAM_LENGTH)

#ifdef __cplusplus
}
#endif

#endif
