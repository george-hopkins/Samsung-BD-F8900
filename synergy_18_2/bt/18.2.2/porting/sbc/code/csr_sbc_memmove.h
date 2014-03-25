#ifndef CSR_SBC_MEMMOVE_H__
#define CSR_SBC_MEMMOVE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #1 $
***************************************************************************/
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *CsrSbcMemMove(void *dest, const void *src, CsrSize count);

#ifdef __cplusplus
}
#endif

#endif /* CSR_SBC_MEMMOVE_H__ */

