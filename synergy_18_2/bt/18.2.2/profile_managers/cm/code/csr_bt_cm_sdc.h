#ifndef CSR_BT_CM_SDC_H__
#define CSR_BT_CM_SDC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "sdc_prim.h"
#include "sds_prim.h"
#include "sdclib.h"
#include "sdslib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 These function are found under cmSdpArrivalHandler.c
************************************************************************************/
void CsrBtCmSdcArrivalHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cmSdcHandler.c
************************************************************************************/
void CsrBtCmSdcSearchReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcServiceSearchReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcRfcSearchReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcRfcExtendedSearchReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
void CsrBtCmSdcUuid128RfcSearchReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmSdcCancelSearchReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcAttrReqHandle(cmInstanceData_t *cmData);
void CsrBtCmSdcCloseReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcReleaseResourcesReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdsRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdsUnRegisterReqHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
void cmSdsRegisterUuid128ReqHandler(cmInstanceData_t *cmData);
void cmSdsUnRegisterUuid128ReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcUuid128SearchReqHandler(cmInstanceData_t *cmData);
#endif
#ifdef CSR_BT_INSTALL_CM_PRI_SDC
void CsrBtCmSdcOpenReqHandler(cmInstanceData_t *cmData);
#endif
void CsrBtCmSdcOpenSearchCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcServiceSearchCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcServiceAttributeCfmHandler(cmInstanceData_t * cmData);
void CsrBtCmSdcCloseSearchIndHandler(cmInstanceData_t * cmData);
void CsrBtCmSdsRegisterCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmSdsUnRegisterCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmSdcCloseIndMsgSend(CsrSchedQid appHandle, CsrUint8 server, CsrBtDeviceAddr deviceAddr,
                               CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void removeElementFromSdcList(cmInstanceData_t * cmData);
void CsrBtCmSdcStartHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
#ifdef __cplusplus
}
#endif

#endif /* ndef _CM_SDP_H */
