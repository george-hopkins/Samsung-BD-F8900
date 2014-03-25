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
void tptInitHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConnectableHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConnectedHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);
void tptConfiguringHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);
void tptTestingHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);
void tptReportingHandleSppPrim(CsrBtTptServerInstanceDataType *TptData);


#ifdef __cplusplus
}
#endif

#endif /* !TPT_HANDLER_SPP_H__ */
