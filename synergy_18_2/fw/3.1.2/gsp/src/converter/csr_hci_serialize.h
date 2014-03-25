#ifndef CSR_HCI_SERIALIZE_H__
#define CSR_HCI_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrHciPfree(void *ptr);

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM
CsrSize CsrHciRegisterEventHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciRegisterEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterEventHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM
CsrSize CsrHciRegisterAclHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciRegisterAclHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterAclHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterAclHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
CsrSize CsrHciScoDataIndSizeof(void *msg);
CsrUint8 *CsrHciScoDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciScoDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHciScoDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
CsrSize CsrHciVendorSpecificEventIndSizeof(void *msg);
CsrUint8 *CsrHciVendorSpecificEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciVendorSpecificEventIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHciVendorSpecificEventIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrSize CsrHciRegisterVendorSpecificEventHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciRegisterVendorSpecificEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterVendorSpecificEventHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterVendorSpecificEventHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
CsrSize CsrHciScoDataReqSizeof(void *msg);
CsrUint8 *CsrHciScoDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciScoDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrHciScoDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
CsrSize CsrHciAclDataReqSizeof(void *msg);
CsrUint8 *CsrHciAclDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciAclDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrHciAclDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrSize CsrHciUnregisterVendorSpecificEventHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciUnregisterVendorSpecificEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterVendorSpecificEventHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterVendorSpecificEventHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM
CsrSize CsrHciUnregisterAclHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciUnregisterAclHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterAclHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterAclHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
CsrSize CsrHciCommandReqSizeof(void *msg);
CsrUint8 *CsrHciCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciCommandReqDes(CsrUint8 *buffer, CsrSize length);
void CsrHciCommandReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ
CsrSize CsrHciRegisterEventHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciRegisterEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterEventHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterEventHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
CsrSize CsrHciAclDataIndSizeof(void *msg);
CsrUint8 *CsrHciAclDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciAclDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHciAclDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM
CsrSize CsrHciRegisterScoHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciRegisterScoHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterScoHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterScoHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_EVENT_IND
CsrSize CsrHciEventIndSizeof(void *msg);
CsrUint8 *CsrHciEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciEventIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHciEventIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ
CsrSize CsrHciUnregisterScoHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciUnregisterScoHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterScoHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterScoHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrSize CsrHciUnregisterVendorSpecificEventHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciUnregisterVendorSpecificEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterVendorSpecificEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterVendorSpecificEventHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ
CsrSize CsrHciRegisterScoHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciRegisterScoHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterScoHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterScoHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
CsrSize CsrHciVendorSpecificCommandReqSizeof(void *msg);
CsrUint8 *CsrHciVendorSpecificCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciVendorSpecificCommandReqDes(CsrUint8 *buffer, CsrSize length);
void CsrHciVendorSpecificCommandReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM
CsrSize CsrHciUnregisterScoHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciUnregisterScoHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterScoHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterScoHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ
CsrSize CsrHciUnregisterAclHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciUnregisterAclHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciUnregisterAclHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciUnregisterAclHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
CsrSize CsrHciIncomingDataReqSizeof(void *msg);
CsrUint8 *CsrHciIncomingDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciIncomingDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrHciIncomingDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ
CsrSize CsrHciRegisterAclHandlerReqSizeof(void *msg);
CsrUint8 *CsrHciRegisterAclHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterAclHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterAclHandlerReqSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_NOP_IND
CsrSize CsrHciNopIndSizeof(void *msg);
CsrUint8 *CsrHciNopIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciNopIndDes(CsrUint8 *buffer, CsrSize length);
void CsrHciNopIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_HCI_NOP_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrSize CsrHciRegisterVendorSpecificEventHandlerCfmSizeof(void *msg);
CsrUint8 *CsrHciRegisterVendorSpecificEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrHciRegisterVendorSpecificEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrHciRegisterVendorSpecificEventHandlerCfmSerFree CsrHciPfree
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifdef __cplusplus
}
#endif

#endif
