#ifndef CSR_BT_POLYPHASE_H__
#define CSR_BT_POLYPHASE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_sbc.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_FLOAT_PRECISION
#define MULT4   ((double)1)
#define MULT8   ((double)1)
#else
#ifndef ENABLE_32BIT_PRECISION
#define SHIFTS_SB4 1
#define SHIFTS_SB8 2
#else
#define SHIFTS_SB4 0
#define SHIFTS_SB8 0
#endif
#define MULT4   (1<<SHIFTS_SB4)
#define MULT8   (1<<SHIFTS_SB8)
#endif

extern const CsrSbcCoefficientSize C_SB4[40], C_SB8[80];

#ifdef __cplusplus
}
#endif

#endif
