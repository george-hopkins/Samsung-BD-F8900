/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_phdc_mgr_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE
#include "csr_bt_phdc_mgr_serialize.h"
#include "csr_bt_phdc_mgr_prim.h"

static CsrMsgConvMsgEntry csr_bt_phdc_mgr_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND
    { CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND, CsrBtPhdcMgrAcceptAssociateIndSizeof, CsrBtPhdcMgrAcceptAssociateIndSer, CsrBtPhdcMgrAcceptAssociateIndDes, CsrBtPhdcMgrAcceptAssociateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES
    { CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES, CsrBtPhdcMgrAcceptAssociateResSizeof, CsrBtPhdcMgrAcceptAssociateResSer, CsrBtPhdcMgrAcceptAssociateResDes, CsrBtPhdcMgrAcceptAssociateResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND
    { CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND, CsrBtPhdcMgrAcceptBtLinkIndSizeof, CsrBtPhdcMgrAcceptBtLinkIndSer, CsrBtPhdcMgrAcceptBtLinkIndDes, CsrBtPhdcMgrAcceptBtLinkIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES
    { CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES, CsrBtPhdcMgrAcceptBtLinkResSizeof, CsrBtPhdcMgrAcceptBtLinkResSer, CsrBtPhdcMgrAcceptBtLinkResDes, CsrBtPhdcMgrAcceptBtLinkResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND
    { CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND, CsrBtPhdcMgrAssociateCompleteIndSizeof, CsrBtPhdcMgrAssociateCompleteIndSer, CsrBtPhdcMgrAssociateCompleteIndDes, CsrBtPhdcMgrAssociateCompleteIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND
    { CSR_BT_PHDC_MGR_BPM_BP_IND, CsrBtPhdcMgrBpmBpIndSizeof, CsrBtPhdcMgrBpmBpIndSer, CsrBtPhdcMgrBpmBpIndDes, CsrBtPhdcMgrBpmBpIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND
    { CSR_BT_PHDC_MGR_BPM_PULSE_IND, CsrBtPhdcMgrBpmPulseIndSizeof, CsrBtPhdcMgrBpmPulseIndSer, CsrBtPhdcMgrBpmPulseIndDes, CsrBtPhdcMgrBpmPulseIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND
    { CSR_BT_PHDC_MGR_BT_LINK_REL_IND, CsrBtPhdcMgrBtLinkRelIndSizeof, CsrBtPhdcMgrBtLinkRelIndSer, CsrBtPhdcMgrBtLinkRelIndDes, CsrBtPhdcMgrBtLinkRelIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM
    { CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM, CsrBtPhdcMgrConfigureEndpointCfmSizeof, CsrBtPhdcMgrConfigureEndpointCfmSer, CsrBtPhdcMgrConfigureEndpointCfmDes, CsrBtPhdcMgrConfigureEndpointCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND
    { CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND, CsrBtPhdcMgrConfigureEndpointIndSizeof, CsrBtPhdcMgrConfigureEndpointIndSer, CsrBtPhdcMgrConfigureEndpointIndDes, CsrBtPhdcMgrConfigureEndpointIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
    { CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ, CsrBtPhdcMgrConfigureEndpointReqSizeof, CsrBtPhdcMgrConfigureEndpointReqSer, CsrBtPhdcMgrConfigureEndpointReqDes, CsrBtPhdcMgrConfigureEndpointReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
    { CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES, CsrBtPhdcMgrConfigureEndpointResSizeof, CsrBtPhdcMgrConfigureEndpointResSer, CsrBtPhdcMgrConfigureEndpointResDes, CsrBtPhdcMgrConfigureEndpointResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM
    { CSR_BT_PHDC_MGR_DISASSOCIATE_CFM, CsrBtPhdcMgrDisassociateCfmSizeof, CsrBtPhdcMgrDisassociateCfmSer, CsrBtPhdcMgrDisassociateCfmDes, CsrBtPhdcMgrDisassociateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND
    { CSR_BT_PHDC_MGR_DISASSOCIATE_IND, CsrBtPhdcMgrDisassociateIndSizeof, CsrBtPhdcMgrDisassociateIndSer, CsrBtPhdcMgrDisassociateIndDes, CsrBtPhdcMgrDisassociateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ
    { CSR_BT_PHDC_MGR_DISASSOCIATE_REQ, CsrBtPhdcMgrDisassociateReqSizeof, CsrBtPhdcMgrDisassociateReqSer, CsrBtPhdcMgrDisassociateReqDes, CsrBtPhdcMgrDisassociateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
    { CSR_BT_PHDC_MGR_DS_IND, CsrBtPhdcMgrDsIndSizeof, CsrBtPhdcMgrDsIndSer, CsrBtPhdcMgrDsIndDes, CsrBtPhdcMgrDsIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
    { CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND, CsrBtPhdcMgrExtConfigObjIndSizeof, CsrBtPhdcMgrExtConfigObjIndSer, CsrBtPhdcMgrExtConfigObjIndDes, CsrBtPhdcMgrExtConfigObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM
    { CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM, CsrBtPhdcMgrGetDeviceCapabCfmSizeof, CsrBtPhdcMgrGetDeviceCapabCfmSer, CsrBtPhdcMgrGetDeviceCapabCfmDes, CsrBtPhdcMgrGetDeviceCapabCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
    { CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND, CsrBtPhdcMgrGetDeviceCapabIndSizeof, CsrBtPhdcMgrGetDeviceCapabIndSer, CsrBtPhdcMgrGetDeviceCapabIndDes, CsrBtPhdcMgrGetDeviceCapabIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ
    { CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ, CsrBtPhdcMgrGetDeviceCapabReqSizeof, CsrBtPhdcMgrGetDeviceCapabReqSer, CsrBtPhdcMgrGetDeviceCapabReqDes, CsrBtPhdcMgrGetDeviceCapabReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM
    { CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM, CsrBtPhdcMgrSetupBtLinkCfmSizeof, CsrBtPhdcMgrSetupBtLinkCfmSer, CsrBtPhdcMgrSetupBtLinkCfmDes, CsrBtPhdcMgrSetupBtLinkCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ
    { CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ, CsrBtPhdcMgrSetupBtLinkReqSizeof, CsrBtPhdcMgrSetupBtLinkReqSer, CsrBtPhdcMgrSetupBtLinkReqDes, CsrBtPhdcMgrSetupBtLinkReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM
    { CSR_BT_PHDC_MGR_UNREGISTER_CFM, CsrBtPhdcMgrUnregisterCfmSizeof, CsrBtPhdcMgrUnregisterCfmSer, CsrBtPhdcMgrUnregisterCfmDes, CsrBtPhdcMgrUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ
    { CSR_BT_PHDC_MGR_UNREGISTER_REQ, CsrBtPhdcMgrUnregisterReqSizeof, CsrBtPhdcMgrUnregisterReqSer, CsrBtPhdcMgrUnregisterReqDes, CsrBtPhdcMgrUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND
    { CSR_BT_PHDC_MGR_WS_BMI_IND, CsrBtPhdcMgrWsBmiIndSizeof, CsrBtPhdcMgrWsBmiIndSer, CsrBtPhdcMgrWsBmiIndDes, CsrBtPhdcMgrWsBmiIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND
    { CSR_BT_PHDC_MGR_WS_HEIGHT_IND, CsrBtPhdcMgrWsHeightIndSizeof, CsrBtPhdcMgrWsHeightIndSer, CsrBtPhdcMgrWsHeightIndDes, CsrBtPhdcMgrWsHeightIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND
    { CSR_BT_PHDC_MGR_WS_WEIGHT_IND, CsrBtPhdcMgrWsWeightIndSizeof, CsrBtPhdcMgrWsWeightIndSer, CsrBtPhdcMgrWsWeightIndDes, CsrBtPhdcMgrWsWeightIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtPhdcMgrConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PHDC_MGR_PRIM, csr_bt_phdc_mgr_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_phdc_mgr_conv_info = {
    CSR_BT_PHDC_MGR_PRIM,
    "CSR_BT_PHDC_MGR_PRIM",
    csr_bt_phdc_mgr_conv_lut
};

CsrLogPrimitiveInformation* CsrBtPhdcMgrTechInfoGet(void)
{
    return &csr_bt_phdc_mgr_conv_info;
}
#endif
#endif

