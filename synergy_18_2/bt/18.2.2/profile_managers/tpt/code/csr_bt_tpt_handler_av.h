#ifndef CSR_BT_TPT_HANDLER_AV_H__
#define CSR_BT_TPT_HANDLER_AV_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* AV */
void tptInitHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConnectableHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConnectedHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConfiguringHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
void tptTestingHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
void tptReportingHandleAvPrim(CsrBtTptServerInstanceDataType *TptData);
#ifdef __cplusplus
}
#endif

#endif /* !TPT_HANDLER_AV_H__ */
