#ifndef CSR_BT_CONVERTER_UTILS_H__
#define CSR_BT_CONVERTER_UTILS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtConverterSerialiseBtTypedAddr(CsrUint8 *ptr, CsrSize *length, CsrBtTypedAddr *addr);
void CsrBtConverterDeserialiseBtTypedAddr(CsrUint8 *buffer, CsrSize *offset, CsrBtTypedAddr *target);

#ifdef __cplusplus
}
#endif

#endif

