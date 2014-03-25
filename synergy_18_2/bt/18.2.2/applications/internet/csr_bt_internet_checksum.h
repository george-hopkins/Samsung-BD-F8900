#ifndef CSR_BT_INTERNET_CHECKSUM_H__
#define CSR_BT_INTERNET_CHECKSUM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CsrUint16 CalculateCheckSum(CsrUint16 length, CsrUint8 * data);
extern CsrUint32 CalculateCheckSumFirstFragment(CsrUint16 length, CsrUint8 * data);
extern CsrUint32 CalculateCheckSumFragment(CsrUint32 preliminaryCheckSum, CsrUint16 length, CsrUint8 * data);
extern CsrUint16 CalculateCheckSumLastFragment(CsrUint32 preliminaryCheckSum, CsrUint16 length, CsrUint8 * data);

#ifdef __cplusplus
}
#endif

#endif    /* CSR_BT_INTERNET_CHECKSUM_H__ */
