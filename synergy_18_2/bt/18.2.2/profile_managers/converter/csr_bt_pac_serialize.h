#ifndef CSR_BT_PAC_SERIALIZE_H__
#define CSR_BT_PAC_SERIALIZE_H__
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

void CsrBtPacPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
CsrSize CsrBtPacSetFolderReqSizeof(void *msg);
CsrUint8 *CsrBtPacSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetFolderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacSetFolderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM
CsrSize CsrBtPacSetFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtPacSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSetFolderCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtPacSetRootFolderReqSizeof(void *msg);
CsrUint8 *CsrBtPacSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSetRootFolderReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ
CsrSize CsrBtPacSetBackFolderReqSizeof(void *msg);
CsrUint8 *CsrBtPacSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSetBackFolderReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_RES
CsrSize CsrBtPacPullPbResSizeof(void *msg);
CsrUint8 *CsrBtPacPullPbResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullPbResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullPbResSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM
CsrSize CsrBtPacSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtPacSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSecurityOutCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
CsrSize CsrBtPacAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtPacAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_REQ
CsrSize CsrBtPacAbortReqSizeof(void *msg);
CsrUint8 *CsrBtPacAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacAbortReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
CsrSize CsrBtPacPullPbReqSizeof(void *msg);
CsrUint8 *CsrBtPacPullPbReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullPbReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullPbReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES
CsrSize CsrBtPacPullVcardEntryResSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardEntryResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullVcardEntryResSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ
CsrSize CsrBtPacDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtPacDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacDisconnectReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
CsrSize CsrBtPacPullVcardEntryReqSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardEntryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardEntryReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullVcardEntryReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING
CsrSize CsrBtPacHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtPacHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacHouseCleaningSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
CsrSize CsrBtPacPullVcardListReqSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardListReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullVcardListReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM
CsrSize CsrBtPacPullVcardEntryCfmSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardEntryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardEntryCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullVcardEntryCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
CsrSize CsrBtPacAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtPacAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ
CsrSize CsrBtPacCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtPacCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacCancelConnectReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_CFM
CsrSize CsrBtPacPullPbCfmSizeof(void *msg);
CsrUint8 *CsrBtPacPullPbCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullPbCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullPbCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
CsrSize CsrBtPacPullVcardListIndSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardListIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullVcardListIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtPacSetRootFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtPacSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSetRootFolderCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_ABORT_CFM
CsrSize CsrBtPacAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtPacAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacAbortCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_IND
CsrSize CsrBtPacDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtPacDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacDisconnectIndSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM
CsrSize CsrBtPacPullVcardListCfmSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardListCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullVcardListCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM
CsrSize CsrBtPacSetBackFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtPacSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSetBackFolderCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
CsrSize CsrBtPacPullPbIndSizeof(void *msg);
CsrUint8 *CsrBtPacPullPbIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullPbIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullPbIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES
CsrSize CsrBtPacPullVcardListResSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardListResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardListResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacPullVcardListResSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ
CsrSize CsrBtPacSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtPacSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacSecurityOutReqSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_CFM
CsrSize CsrBtPacConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtPacConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPacConnectCfmSerFree CsrBtPacPfree
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
CsrSize CsrBtPacPullVcardEntryIndSizeof(void *msg);
CsrUint8 *CsrBtPacPullVcardEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacPullVcardEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacPullVcardEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
CsrSize CsrBtPacConnectReqSizeof(void *msg);
CsrUint8 *CsrBtPacConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPacConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPacConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */

#ifdef __cplusplus
}
#endif

#endif
