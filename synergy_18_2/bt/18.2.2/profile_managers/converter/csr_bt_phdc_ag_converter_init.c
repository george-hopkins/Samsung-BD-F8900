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
#include "csr_bt_phdc_ag_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
#include "csr_bt_phdc_ag_serialize.h"
#include "csr_bt_phdc_ag_prim.h"

static CsrMsgConvMsgEntry csr_bt_phdc_ag_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND
    { CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND, CsrBtPhdcAgAcceptBtLinkIndSizeof, CsrBtPhdcAgAcceptBtLinkIndSer, CsrBtPhdcAgAcceptBtLinkIndDes, CsrBtPhdcAgAcceptBtLinkIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES
    { CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES, CsrBtPhdcAgAcceptBtLinkResSizeof, CsrBtPhdcAgAcceptBtLinkResSer, CsrBtPhdcAgAcceptBtLinkResDes, CsrBtPhdcAgAcceptBtLinkResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM
    { CSR_BT_PHDC_AG_ASSOCIATE_CFM, CsrBtPhdcAgAssociateCfmSizeof, CsrBtPhdcAgAssociateCfmSer, CsrBtPhdcAgAssociateCfmDes, CsrBtPhdcAgAssociateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ
    { CSR_BT_PHDC_AG_ASSOCIATE_REQ, CsrBtPhdcAgAssociateReqSizeof, CsrBtPhdcAgAssociateReqSer, CsrBtPhdcAgAssociateReqDes, CsrBtPhdcAgAssociateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND
    { CSR_BT_PHDC_AG_BT_LINK_REL_IND, CsrBtPhdcAgBtLinkRelIndSizeof, CsrBtPhdcAgBtLinkRelIndSer, CsrBtPhdcAgBtLinkRelIndDes, CsrBtPhdcAgBtLinkRelIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM
    { CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM, CsrBtPhdcAgChangeDimUnitCodeCfmSizeof, CsrBtPhdcAgChangeDimUnitCodeCfmSer, CsrBtPhdcAgChangeDimUnitCodeCfmDes, CsrBtPhdcAgChangeDimUnitCodeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ
    { CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ, CsrBtPhdcAgChangeDimUnitCodeReqSizeof, CsrBtPhdcAgChangeDimUnitCodeReqSer, CsrBtPhdcAgChangeDimUnitCodeReqDes, CsrBtPhdcAgChangeDimUnitCodeReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM
    { CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM, CsrBtPhdcAgConfigureEndpointCfmSizeof, CsrBtPhdcAgConfigureEndpointCfmSer, CsrBtPhdcAgConfigureEndpointCfmDes, CsrBtPhdcAgConfigureEndpointCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND
    { CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND, CsrBtPhdcAgConfigureEndpointIndSizeof, CsrBtPhdcAgConfigureEndpointIndSer, CsrBtPhdcAgConfigureEndpointIndDes, CsrBtPhdcAgConfigureEndpointIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
    { CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ, CsrBtPhdcAgConfigureEndpointReqSizeof, CsrBtPhdcAgConfigureEndpointReqSer, CsrBtPhdcAgConfigureEndpointReqDes, CsrBtPhdcAgConfigureEndpointReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
    { CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES, CsrBtPhdcAgConfigureEndpointResSizeof, CsrBtPhdcAgConfigureEndpointResSer, CsrBtPhdcAgConfigureEndpointResDes, CsrBtPhdcAgConfigureEndpointResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
    { CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ, CsrBtPhdcAgConfigureMdsReqSizeof, CsrBtPhdcAgConfigureMdsReqSer, CsrBtPhdcAgConfigureMdsReqDes, CsrBtPhdcAgConfigureMdsReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM
    { CSR_BT_PHDC_AG_DATA_CFM, CsrBtPhdcAgDataCfmSizeof, CsrBtPhdcAgDataCfmSer, CsrBtPhdcAgDataCfmDes, CsrBtPhdcAgDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
    { CSR_BT_PHDC_AG_DATA_REQ, CsrBtPhdcAgDataReqSizeof, CsrBtPhdcAgDataReqSer, CsrBtPhdcAgDataReqDes, CsrBtPhdcAgDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND
    { CSR_BT_PHDC_AG_DIM_ATTRIB_IND, CsrBtPhdcAgDimAttribIndSizeof, CsrBtPhdcAgDimAttribIndSer, CsrBtPhdcAgDimAttribIndDes, CsrBtPhdcAgDimAttribIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
    { CSR_BT_PHDC_AG_DIM_ATTRIB_RES, CsrBtPhdcAgDimAttribResSizeof, CsrBtPhdcAgDimAttribResSer, CsrBtPhdcAgDimAttribResDes, CsrBtPhdcAgDimAttribResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM
    { CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM, CsrBtPhdcAgDimConfigureCfmSizeof, CsrBtPhdcAgDimConfigureCfmSer, CsrBtPhdcAgDimConfigureCfmDes, CsrBtPhdcAgDimConfigureCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ
    { CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ, CsrBtPhdcAgDimConfigureReqSizeof, CsrBtPhdcAgDimConfigureReqSer, CsrBtPhdcAgDimConfigureReqDes, CsrBtPhdcAgDimConfigureReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND
    { CSR_BT_PHDC_AG_DIM_OBJECT_IND, CsrBtPhdcAgDimObjectIndSizeof, CsrBtPhdcAgDimObjectIndSer, CsrBtPhdcAgDimObjectIndDes, CsrBtPhdcAgDimObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
    { CSR_BT_PHDC_AG_DIM_OBJECT_RES, CsrBtPhdcAgDimObjectResSizeof, CsrBtPhdcAgDimObjectResSer, CsrBtPhdcAgDimObjectResDes, CsrBtPhdcAgDimObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM
    { CSR_BT_PHDC_AG_DISASSOCIATE_CFM, CsrBtPhdcAgDisassociateCfmSizeof, CsrBtPhdcAgDisassociateCfmSer, CsrBtPhdcAgDisassociateCfmDes, CsrBtPhdcAgDisassociateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND
    { CSR_BT_PHDC_AG_DISASSOCIATE_IND, CsrBtPhdcAgDisassociateIndSizeof, CsrBtPhdcAgDisassociateIndSer, CsrBtPhdcAgDisassociateIndDes, CsrBtPhdcAgDisassociateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ
    { CSR_BT_PHDC_AG_DISASSOCIATE_REQ, CsrBtPhdcAgDisassociateReqSizeof, CsrBtPhdcAgDisassociateReqSer, CsrBtPhdcAgDisassociateReqDes, CsrBtPhdcAgDisassociateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM
    { CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM, CsrBtPhdcAgGetDeviceCapabCfmSizeof, CsrBtPhdcAgGetDeviceCapabCfmSer, CsrBtPhdcAgGetDeviceCapabCfmDes, CsrBtPhdcAgGetDeviceCapabCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
    { CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND, CsrBtPhdcAgGetDeviceCapabIndSizeof, CsrBtPhdcAgGetDeviceCapabIndSer, CsrBtPhdcAgGetDeviceCapabIndDes, CsrBtPhdcAgGetDeviceCapabIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ
    { CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ, CsrBtPhdcAgGetDeviceCapabReqSizeof, CsrBtPhdcAgGetDeviceCapabReqSer, CsrBtPhdcAgGetDeviceCapabReqDes, CsrBtPhdcAgGetDeviceCapabReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM
    { CSR_BT_PHDC_AG_RESUME_CFM, CsrBtPhdcAgResumeCfmSizeof, CsrBtPhdcAgResumeCfmSer, CsrBtPhdcAgResumeCfmDes, CsrBtPhdcAgResumeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND
    { CSR_BT_PHDC_AG_RESUME_IND, CsrBtPhdcAgResumeIndSizeof, CsrBtPhdcAgResumeIndSer, CsrBtPhdcAgResumeIndDes, CsrBtPhdcAgResumeIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ
    { CSR_BT_PHDC_AG_RESUME_REQ, CsrBtPhdcAgResumeReqSizeof, CsrBtPhdcAgResumeReqSer, CsrBtPhdcAgResumeReqDes, CsrBtPhdcAgResumeReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND
    { CSR_BT_PHDC_AG_SETUP_BT_LINK_IND, CsrBtPhdcAgSetupBtLinkIndSizeof, CsrBtPhdcAgSetupBtLinkIndSer, CsrBtPhdcAgSetupBtLinkIndDes, CsrBtPhdcAgSetupBtLinkIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM
    { CSR_BT_PHDC_AG_SUSPEND_CFM, CsrBtPhdcAgSuspendCfmSizeof, CsrBtPhdcAgSuspendCfmSer, CsrBtPhdcAgSuspendCfmDes, CsrBtPhdcAgSuspendCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND
    { CSR_BT_PHDC_AG_SUSPEND_IND, CsrBtPhdcAgSuspendIndSizeof, CsrBtPhdcAgSuspendIndSer, CsrBtPhdcAgSuspendIndDes, CsrBtPhdcAgSuspendIndSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ
    { CSR_BT_PHDC_AG_SUSPEND_REQ, CsrBtPhdcAgSuspendReqSizeof, CsrBtPhdcAgSuspendReqSer, CsrBtPhdcAgSuspendReqDes, CsrBtPhdcAgSuspendReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM
    { CSR_BT_PHDC_AG_UNREGISTER_CFM, CsrBtPhdcAgUnregisterCfmSizeof, CsrBtPhdcAgUnregisterCfmSer, CsrBtPhdcAgUnregisterCfmDes, CsrBtPhdcAgUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ
    { CSR_BT_PHDC_AG_UNREGISTER_REQ, CsrBtPhdcAgUnregisterReqSizeof, CsrBtPhdcAgUnregisterReqSer, CsrBtPhdcAgUnregisterReqDes, CsrBtPhdcAgUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtPhdcAgConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PHDC_AG_PRIM, csr_bt_phdc_ag_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_phdc_ag_conv_info = {
    CSR_BT_PHDC_AG_PRIM,
    "CSR_BT_PHDC_AG_PRIM",
    csr_bt_phdc_ag_conv_lut
};

CsrLogPrimitiveInformation* CsrBtPhdcAgTechInfoGet(void)
{
    return &csr_bt_phdc_ag_conv_info;
}
#endif
#endif

