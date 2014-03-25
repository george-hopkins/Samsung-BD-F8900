#ifndef CSR_BT_PHDC_AG_SERIALIZE_H__
#define CSR_BT_PHDC_AG_SERIALIZE_H__
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

void CsrBtPhdc_agPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ
CsrSize CsrBtPhdcAgAssociateReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgAssociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgAssociateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgAssociateReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND
CsrSize CsrBtPhdcAgDimAttribIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimAttribIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimAttribIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDimAttribIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND
CsrSize CsrBtPhdcAgDisassociateIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDisassociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDisassociateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDisassociateIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM
CsrSize CsrBtPhdcAgResumeCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgResumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgResumeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgResumeCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ
CsrSize CsrBtPhdcAgGetDeviceCapabReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgGetDeviceCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgGetDeviceCapabReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgGetDeviceCapabReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
CsrSize CsrBtPhdcAgGetDeviceCapabIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgGetDeviceCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgGetDeviceCapabIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgGetDeviceCapabIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
CsrSize CsrBtPhdcAgDimAttribResSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimAttribResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimAttribResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgDimAttribResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM
CsrSize CsrBtPhdcAgConfigureEndpointCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgConfigureEndpointCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgConfigureEndpointCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgConfigureEndpointCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND
CsrSize CsrBtPhdcAgBtLinkRelIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgBtLinkRelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgBtLinkRelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgBtLinkRelIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND
CsrSize CsrBtPhdcAgSuspendIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgSuspendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgSuspendIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgSuspendIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND
CsrSize CsrBtPhdcAgSetupBtLinkIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgSetupBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgSetupBtLinkIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgSetupBtLinkIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM
CsrSize CsrBtPhdcAgSuspendCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgSuspendCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgSuspendCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM
CsrSize CsrBtPhdcAgDataCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDataCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ
CsrSize CsrBtPhdcAgResumeReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgResumeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgResumeReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND
CsrSize CsrBtPhdcAgAcceptBtLinkIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgAcceptBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgAcceptBtLinkIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgAcceptBtLinkIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
CsrSize CsrBtPhdcAgDataReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ
CsrSize CsrBtPhdcAgDisassociateReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDisassociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDisassociateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDisassociateReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND
CsrSize CsrBtPhdcAgResumeIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgResumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgResumeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgResumeIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ
CsrSize CsrBtPhdcAgChangeDimUnitCodeReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgChangeDimUnitCodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgChangeDimUnitCodeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgChangeDimUnitCodeReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
CsrSize CsrBtPhdcAgConfigureEndpointResSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgConfigureEndpointResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgConfigureEndpointResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgConfigureEndpointResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND
CsrSize CsrBtPhdcAgDimObjectIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDimObjectIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM
CsrSize CsrBtPhdcAgDimConfigureCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimConfigureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimConfigureCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDimConfigureCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ
CsrSize CsrBtPhdcAgSuspendReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgSuspendReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgSuspendReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES
CsrSize CsrBtPhdcAgAcceptBtLinkResSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgAcceptBtLinkResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgAcceptBtLinkResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgAcceptBtLinkResSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM
CsrSize CsrBtPhdcAgChangeDimUnitCodeCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgChangeDimUnitCodeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgChangeDimUnitCodeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgChangeDimUnitCodeCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM
CsrSize CsrBtPhdcAgUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgUnregisterCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ
CsrSize CsrBtPhdcAgUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgUnregisterReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM
CsrSize CsrBtPhdcAgGetDeviceCapabCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgGetDeviceCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgGetDeviceCapabCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgGetDeviceCapabCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
CsrSize CsrBtPhdcAgDimObjectResSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgDimObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM
CsrSize CsrBtPhdcAgAssociateCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgAssociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgAssociateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgAssociateCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ
CsrSize CsrBtPhdcAgDimConfigureReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDimConfigureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDimConfigureReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDimConfigureReqSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
CsrSize CsrBtPhdcAgConfigureMdsReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgConfigureMdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgConfigureMdsReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgConfigureMdsReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND
CsrSize CsrBtPhdcAgConfigureEndpointIndSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgConfigureEndpointIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgConfigureEndpointIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgConfigureEndpointIndSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
CsrSize CsrBtPhdcAgConfigureEndpointReqSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgConfigureEndpointReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgConfigureEndpointReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPhdcAgConfigureEndpointReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM
CsrSize CsrBtPhdcAgDisassociateCfmSizeof(void *msg);
CsrUint8 *CsrBtPhdcAgDisassociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPhdcAgDisassociateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPhdcAgDisassociateCfmSerFree CsrBtPhdc_agPfree
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM */

#ifdef __cplusplus
}
#endif

#endif
