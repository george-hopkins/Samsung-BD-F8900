#ifndef CSR_BT_PHDC_MGR_SERIALIZE_H__
#define CSR_BT_PHDC_MGR_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPhdc_mgrPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES
CsrSize CsrBtPhdcMgrAcceptBtLinkResSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrAcceptBtLinkResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrAcceptBtLinkResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrAcceptBtLinkResSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND
CsrSize CsrBtPhdcMgrBtLinkRelIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrBtLinkRelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrBtLinkRelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrBtLinkRelIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND
CsrSize CsrBtPhdcMgrWsHeightIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrWsHeightIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrWsHeightIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrWsHeightIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
CsrSize CsrBtPhdcMgrDsIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrDsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrDsIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcMgrDsIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
CsrSize CsrBtPhdcMgrConfigureEndpointReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrConfigureEndpointReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrConfigureEndpointReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcMgrConfigureEndpointReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM
CsrSize CsrBtPhdcMgrDisassociateCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrDisassociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrDisassociateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrDisassociateCfmSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
CsrSize CsrBtPhdcMgrConfigureEndpointResSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrConfigureEndpointResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrConfigureEndpointResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcMgrConfigureEndpointResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND
CsrSize CsrBtPhdcMgrConfigureEndpointIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrConfigureEndpointIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrConfigureEndpointIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrConfigureEndpointIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ
CsrSize CsrBtPhdcMgrDisassociateReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrDisassociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrDisassociateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrDisassociateReqSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM
CsrSize CsrBtPhdcMgrConfigureEndpointCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrConfigureEndpointCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrConfigureEndpointCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrConfigureEndpointCfmSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND
CsrSize CsrBtPhdcMgrAcceptBtLinkIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrAcceptBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrAcceptBtLinkIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrAcceptBtLinkIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND
CsrSize CsrBtPhdcMgrDisassociateIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrDisassociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrDisassociateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrDisassociateIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM
CsrSize CsrBtPhdcMgrUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrUnregisterCfmSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND
CsrSize CsrBtPhdcMgrAssociateCompleteIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrAssociateCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrAssociateCompleteIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrAssociateCompleteIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND
CsrSize CsrBtPhdcMgrAcceptAssociateIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrAcceptAssociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrAcceptAssociateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrAcceptAssociateIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
CsrSize CsrBtPhdcMgrGetDeviceCapabIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrGetDeviceCapabIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcMgrGetDeviceCapabIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ
CsrSize CsrBtPhdcMgrUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrUnregisterReqSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND
CsrSize CsrBtPhdcMgrBpmPulseIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrBpmPulseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrBpmPulseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrBpmPulseIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ
CsrSize CsrBtPhdcMgrSetupBtLinkReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrSetupBtLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrSetupBtLinkReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrSetupBtLinkReqSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND
CsrSize CsrBtPhdcMgrBpmBpIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrBpmBpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrBpmBpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrBpmBpIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES
CsrSize CsrBtPhdcMgrAcceptAssociateResSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrAcceptAssociateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrAcceptAssociateResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrAcceptAssociateResSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM
CsrSize CsrBtPhdcMgrGetDeviceCapabCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrGetDeviceCapabCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrGetDeviceCapabCfmSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ
CsrSize CsrBtPhdcMgrGetDeviceCapabReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrGetDeviceCapabReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrGetDeviceCapabReqSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND
CsrSize CsrBtPhdcMgrWsBmiIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrWsBmiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrWsBmiIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrWsBmiIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND
CsrSize CsrBtPhdcMgrWsWeightIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrWsWeightIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrWsWeightIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrWsWeightIndSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
CsrSize CsrBtPhdcMgrExtConfigObjIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrExtConfigObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrExtConfigObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcMgrExtConfigObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM
CsrSize CsrBtPhdcMgrSetupBtLinkCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcMgrSetupBtLinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcMgrSetupBtLinkCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcMgrSetupBtLinkCfmSerFree CsrBtPhdc_mgrPfree
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM */

#ifdef __cplusplus
}
#endif

#endif
