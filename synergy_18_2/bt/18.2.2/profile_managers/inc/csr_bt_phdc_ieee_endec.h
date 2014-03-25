#ifndef CSR_BT_PHDC_IEEE_ENDEC_H__
#define CSR_BT_PHDC_IEEE_ENDEC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*Libraries headers*/
#include "csr_types.h"

/* IEEE Headers */
#include "csr_bt_phdc_ieee_asn.h"

/* Function Prototypes */
CsrUint16 CsrBtPhdcMderCalculateApduEncodedLength(const APDU *const apduData);

void CsrBtPhdcMderEncodeApdu(const APDU *const apduData, CsrUint8 *msg_ptr);
CsrUint16 CsrBtPhdcMderDecodeApdu(const CsrUint8 *msg_ptr, APDU **apdu);

#ifdef __cplusplus
extern "C" }
#endif

#endif

