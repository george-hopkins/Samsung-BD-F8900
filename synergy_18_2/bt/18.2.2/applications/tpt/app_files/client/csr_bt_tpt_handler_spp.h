#ifndef CSR_BT_TPT_HANDLER_SPP_H__
#define CSR_BT_TPT_HANDLER_SPP_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* SPP */
void tptInitHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConnectingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConnectedHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);
void tptConfiguringHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);
void tptTestingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);
void tptReportingHandleSppPrim(CsrBtTptClientInstanceDataType *TptData);


#ifdef __cplusplus
}
#endif

#endif /* !TPT_HANDLER_SPP_H__ */
