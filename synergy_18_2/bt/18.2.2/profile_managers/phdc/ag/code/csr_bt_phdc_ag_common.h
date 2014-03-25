#ifndef CSR_BT_PHDC_AG_COMMON_H__
#define CSR_BT_PHDC_AG_COMMON_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_phdc_ag_main.h"
#include "csr_bt_phdc_ag_prim.h"
#include "csr_bt_phdc_ieee_asn.h"

#ifdef __cplusplus
extern "C" {
#endif




/* Assertion helpers */
#if !defined(__KERNEL__) && defined(_DEBUG)
#include <assert.h>
#define CSR_BT_PHDC_AG_ASSERT(x) assert(x)
#endif
#ifndef CSR_BT_PHDC_AG_ASSERT
#define CSR_BT_PHDC_AG_ASSERT(x)
#endif

void CsrBtPhdcAgentStoreMdsConfiguration(CsrBtPhdcAgConfigureMdsReq *pCsrBtPhdcConfigureMdsReq,CsrBtPhdcAgentInstance *pAgentInstance);
void CsrBtPhdcAgentFreeMdsConfiguration(CsrBtPhdcAgentInstance *pInstance);
void CsrBtPhdcAgentFormConfigObjectList(CsrBtPhdcAgentInstance *pAgentInstance);
void CsrBtPhdcAgentFormConfigObject(ConfigObject *pConfigObject,CsrBtPhdcAgentObjectInfo *pObjectList);
void CsrBtPhdcFormEventReportArgumentSimpleNotiConfig(CsrBtPhdcAgentInstance *pAgentInstance);
void CsrBtPhdcAgFormRoivCmipConfirmedEventReport(EventReportArgumentSimple *pRoivCmipConfirmedEventReport,
                                    CsrBtPhdcAgentInstance *pInstance,CsrBtPhdcAgDataNode *pData);
void CsrBtPhdcAgFormObservationScanFixed(ObservationScanFixed *pObservationScanFixed,CsrBtPhdcAgDataNode *pData);
void CsrBtPhdcAgFormScanReportPerFixed(ScanReportPerFixed *pScanReportPerFixed,CsrBtPhdcAgDataNode *pData);

CsrBool CsrBtPhdcAgentValidateAare(AARE_apdu *pAare);
void CsrBtPhdcAgFormObservationScan(CsrBtPhdcAgentInstance *pInstance,CsrBtPhdcAgDataNode *dataNode);
void CsrBtPhdcAgFormScanReportPerVar(ScanReportPerVar *pScanReportPerVar,CsrBtPhdcAgDataNode *pData,CsrBtPhdcAgentInstance *pInstance);
void CsrBtPhdcStoreApdu(CsrUint8 *msgPtr,CsrUint16 encodedMsgLen,CsrBtPhdcAgentInstance *pInstance);
void CsrBtPhdcAgFreeMemory(CsrBtPhdcAgentInstance *pInst);
void CsrBtPhdcAgFreeObjectInfo(CsrBtPhdcAgentInstance *pInstance);

void CsrBtPhdc20601AarqEncodeSend(CsrBtPhdcAgentInstance *pInstance);

CsrBool CsrBtPhdcAgIsValidObjectHandle(CsrUint16 objHandle,CsrBtPhdcAgentInstance *pInstance);



#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_AG_COMMON_H*/

