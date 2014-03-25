#ifndef CSR_BT_JSR82_DISCOVERY_H__
#define CSR_BT_JSR82_DISCOVERY_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#define ATTR_NUM_OF_RETRIES 2

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtJsr82DaStartInquiry(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaContinueInquiry(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaCancelInquiry(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaInquiryGetFriendlyName(Jsr82MainInstanceData *instData);
void CsrBtJsr82DlDeviceDiscovered(Jsr82MainInstanceData *instData);
void CsrBtJsr82DlDeviceDiscoveredAndIgnored(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaSdCloseIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaSearchServices(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaCancelServiceSearch(Jsr82MainInstanceData *instData);
void CsrBtJsr82GetAttrSet(Jsr82MainInstanceData *instData);
void CsrBtJsr82StartGetAttrSet(Jsr82MainInstanceData *instData,
                          CsrBtUuid32 *serviceRecHandles, CsrUint16 serviceRecHandlesLength,
                          CsrUint16 *attrSet, CsrUint16 attrSetLength);
void CsrBtJsr82DlServicesDiscovered(Jsr82MainInstanceData *instData);
void CsrBtJsr82ClosedServiceSearch(Jsr82MainInstanceData *instData);
void CsrBtJsr82AddSearchReq(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaRetrieveDevicesReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82SdReadDeviceListIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82SdReadDeviceListCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82DaSelectServiceReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceDiscTimeout(CsrUint16 dummy, void *data);
void CsrBtJsr82CmSelServiceDiscCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceDiscResIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceSdc128searchIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceSdcSearchIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceSdcSearchCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceSdcAttributeCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSelServiceSdcCloseIndHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82SrPopulateRecordReqHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82CmSdcOpenCfmHandler(Jsr82MainInstanceData *instData);
void CsrBtJsr82PopulateCloseHandler(Jsr82MainInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif
