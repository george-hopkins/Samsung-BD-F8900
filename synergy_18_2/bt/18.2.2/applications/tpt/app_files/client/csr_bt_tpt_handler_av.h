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
void tptInitHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConnectingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConnectedHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConfiguringHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
void tptTestingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
void tptReportingHandleAvPrim(CsrBtTptClientInstanceDataType *TptData);
#ifdef __cplusplus
}
#endif

#endif /* !TPT_HANDLER_AV_H__ */
