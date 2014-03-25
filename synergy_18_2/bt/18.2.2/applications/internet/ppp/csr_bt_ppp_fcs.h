#ifndef CSR_BT_PPP_FCS_H__
#define CSR_BT_PPP_FCS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_internet_primitives.h"

extern CsrBool CheckHdlcFrameFcs16(CsrUint8 * header,
                                  CsrUint16 length,
                                  CsrUint8 * data);

/* returns FCS in network order! */
extern CsrUint16 CalculateHdlcFrameFcs16(CsrUint16 protocol,
                                        CsrUint16 length,
                                        CsrUint8 * data);

#ifdef __cplusplus
}
#endif

#endif

