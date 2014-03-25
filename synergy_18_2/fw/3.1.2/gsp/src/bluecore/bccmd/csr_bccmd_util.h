#ifndef CSR_BCCMD_UTIL_H__
#define CSR_BCCMD_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_bccmd_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 *CsrBccmdBccmdCommandBuild(CsrUint16 type,
    CsrUint16 varId,
    CsrUint16 seqNo,
    CsrUint16 payloadLength,
    CsrUint8 *payload,
    CsrUint16 *bccmdLength);

void CsrBccmdDiscardSavedMessages(CsrBccmdInstanceData *bccmdInst);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_UTIL_H__ */
