#ifndef CSR_BT_AT_DTE_H__
#define CSR_BT_AT_DTE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_pmem.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ATSetup(CsrUint8 procedureStep);
void ATDial(CsrInt8 *dialNumber);
void ATHangup(void);
void ATAnswer(void);
void SendTestData(void);
void SendHookOn(CsrUint16 m ,void *data);

#ifdef __cplusplus
}
#endif

#endif
