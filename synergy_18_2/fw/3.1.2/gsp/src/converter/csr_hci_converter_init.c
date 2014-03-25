/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_hci_converter_init.h"
#ifndef EXCLUDE_CSR_HCI_MODULE
#include "csr_hci_serialize.h"
#include "csr_hci_private_prim.h"

static CsrMsgConvMsgEntry csr_hci_conv_lut[] = {
#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
    { CSR_HCI_ACL_DATA_IND, CsrHciAclDataIndSizeof, CsrHciAclDataIndSer, CsrHciAclDataIndDes, CsrHciAclDataIndSerFree },
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */
#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
    { CSR_HCI_ACL_DATA_REQ, CsrHciAclDataReqSizeof, CsrHciAclDataReqSer, CsrHciAclDataReqDes, CsrHciAclDataReqSerFree },
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */
#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
    { CSR_HCI_COMMAND_REQ, CsrHciCommandReqSizeof, CsrHciCommandReqSer, CsrHciCommandReqDes, CsrHciCommandReqSerFree },
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */
#ifndef EXCLUDE_CSR_HCI_EVENT_IND
    { CSR_HCI_EVENT_IND, CsrHciEventIndSizeof, CsrHciEventIndSer, CsrHciEventIndDes, CsrHciEventIndSerFree },
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */
#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
    { CSR_HCI_INCOMING_DATA_REQ, CsrHciIncomingDataReqSizeof, CsrHciIncomingDataReqSer, CsrHciIncomingDataReqDes, CsrHciIncomingDataReqSerFree },
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */
#ifndef EXCLUDE_CSR_HCI_NOP_IND
    { CSR_HCI_NOP_IND, CsrHciNopIndSizeof, CsrHciNopIndSer, CsrHciNopIndDes, CsrHciNopIndSerFree },
#endif /* EXCLUDE_CSR_HCI_NOP_IND */
#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM
    { CSR_HCI_REGISTER_ACL_HANDLER_CFM, CsrHciRegisterAclHandlerCfmSizeof, CsrHciRegisterAclHandlerCfmSer, CsrHciRegisterAclHandlerCfmDes, CsrHciRegisterAclHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ
    { CSR_HCI_REGISTER_ACL_HANDLER_REQ, CsrHciRegisterAclHandlerReqSizeof, CsrHciRegisterAclHandlerReqSer, CsrHciRegisterAclHandlerReqDes, CsrHciRegisterAclHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM
    { CSR_HCI_REGISTER_EVENT_HANDLER_CFM, CsrHciRegisterEventHandlerCfmSizeof, CsrHciRegisterEventHandlerCfmSer, CsrHciRegisterEventHandlerCfmDes, CsrHciRegisterEventHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ
    { CSR_HCI_REGISTER_EVENT_HANDLER_REQ, CsrHciRegisterEventHandlerReqSizeof, CsrHciRegisterEventHandlerReqSer, CsrHciRegisterEventHandlerReqDes, CsrHciRegisterEventHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM
    { CSR_HCI_REGISTER_SCO_HANDLER_CFM, CsrHciRegisterScoHandlerCfmSizeof, CsrHciRegisterScoHandlerCfmSer, CsrHciRegisterScoHandlerCfmDes, CsrHciRegisterScoHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ
    { CSR_HCI_REGISTER_SCO_HANDLER_REQ, CsrHciRegisterScoHandlerReqSizeof, CsrHciRegisterScoHandlerReqSer, CsrHciRegisterScoHandlerReqDes, CsrHciRegisterScoHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
    { CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM, CsrHciRegisterVendorSpecificEventHandlerCfmSizeof, CsrHciRegisterVendorSpecificEventHandlerCfmSer, CsrHciRegisterVendorSpecificEventHandlerCfmDes, CsrHciRegisterVendorSpecificEventHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
    { CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ, CsrHciRegisterVendorSpecificEventHandlerReqSizeof, CsrHciRegisterVendorSpecificEventHandlerReqSer, CsrHciRegisterVendorSpecificEventHandlerReqDes, CsrHciRegisterVendorSpecificEventHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
    { CSR_HCI_SCO_DATA_IND, CsrHciScoDataIndSizeof, CsrHciScoDataIndSer, CsrHciScoDataIndDes, CsrHciScoDataIndSerFree },
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */
#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
    { CSR_HCI_SCO_DATA_REQ, CsrHciScoDataReqSizeof, CsrHciScoDataReqSer, CsrHciScoDataReqDes, CsrHciScoDataReqSerFree },
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM
    { CSR_HCI_UNREGISTER_ACL_HANDLER_CFM, CsrHciUnregisterAclHandlerCfmSizeof, CsrHciUnregisterAclHandlerCfmSer, CsrHciUnregisterAclHandlerCfmDes, CsrHciUnregisterAclHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ
    { CSR_HCI_UNREGISTER_ACL_HANDLER_REQ, CsrHciUnregisterAclHandlerReqSizeof, CsrHciUnregisterAclHandlerReqSer, CsrHciUnregisterAclHandlerReqDes, CsrHciUnregisterAclHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM
    { CSR_HCI_UNREGISTER_SCO_HANDLER_CFM, CsrHciUnregisterScoHandlerCfmSizeof, CsrHciUnregisterScoHandlerCfmSer, CsrHciUnregisterScoHandlerCfmDes, CsrHciUnregisterScoHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ
    { CSR_HCI_UNREGISTER_SCO_HANDLER_REQ, CsrHciUnregisterScoHandlerReqSizeof, CsrHciUnregisterScoHandlerReqSer, CsrHciUnregisterScoHandlerReqDes, CsrHciUnregisterScoHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
    { CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM, CsrHciUnregisterVendorSpecificEventHandlerCfmSizeof, CsrHciUnregisterVendorSpecificEventHandlerCfmSer, CsrHciUnregisterVendorSpecificEventHandlerCfmDes, CsrHciUnregisterVendorSpecificEventHandlerCfmSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */
#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
    { CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ, CsrHciUnregisterVendorSpecificEventHandlerReqSizeof, CsrHciUnregisterVendorSpecificEventHandlerReqSer, CsrHciUnregisterVendorSpecificEventHandlerReqDes, CsrHciUnregisterVendorSpecificEventHandlerReqSerFree },
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
    { CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ, CsrHciVendorSpecificCommandReqSizeof, CsrHciVendorSpecificCommandReqSer, CsrHciVendorSpecificCommandReqDes, CsrHciVendorSpecificCommandReqSerFree },
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */
#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
    { CSR_HCI_VENDOR_SPECIFIC_EVENT_IND, CsrHciVendorSpecificEventIndSizeof, CsrHciVendorSpecificEventIndSer, CsrHciVendorSpecificEventIndDes, CsrHciVendorSpecificEventIndSerFree },
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrHciConverterInit(void)
{
    CsrMsgConvInsert(CSR_HCI_PRIM, csr_hci_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_hci_conv_info = {
    CSR_HCI_PRIM,
    "CSR_HCI_PRIM",
    csr_hci_conv_lut
};

CsrLogPrimitiveInformation* CsrHciTechInfoGet(void)
{
    return &csr_hci_conv_info;
}
#endif
#endif

