#ifndef CSR_BT_JSR82_L2CAP_H__
#define CSR_BT_JSR82_L2CAP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_jsr82_main.h"

#define JSR82_L2CA_UUID 0x0000

#ifdef __cplusplus
extern "C" {
#endif

#define JSR82_FIND_CID_ELEMENT(func, searchDataPtr) ((jsr82CidElement *)CsrCmnListSearch(&(psmPtr->cidList), func, searchDataPtr))

void CsrBtJsr82L2caGetPsmReqHandler(Jsr82MainInstanceData *instData);
CsrBool CsrBtJsr82CmL2caRegisterCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
void CsrBtJsr82L2caAcceptReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caOpenReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caConnectAcceptCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caConnectCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caDisconnectReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caDisconnectIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caTxDataReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caDataCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caRxDataReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caDataIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caRxReadyReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler(Jsr82MainInstanceData *instData);
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtJsr82CmL2caModeChangeIndHandler(Jsr82MainInstanceData *instData);
#else
#define CsrBtJsr82CmL2caModeChangeIndHandler NULL
#endif
void CsrBtJsr82L2caGetConfigReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82RemoveL2capConnection(CsrBtConnId btConnId, Jsr82MainInstanceData *instData);
void CsrBtJsr82L2caCloseReqHandler(Jsr82MainInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif
