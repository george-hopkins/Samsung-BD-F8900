#ifndef CSR_BT_CSR_BT_BASE64_H__
#define CSR_BT_CSR_BT_BASE64_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern CsrBool CsrBtBase64Decode(const CsrUint8* source, CsrUint32 sourceLength, CsrUint8** destination, CsrUint32* destinationLength);
extern CsrBool CsrBtBase64Encode(const CsrUint8* source, CsrUint32 sourceLength, CsrUint8** destination, CsrUint32* destinationLength);

#ifdef __cplusplus
}
#endif

#endif
