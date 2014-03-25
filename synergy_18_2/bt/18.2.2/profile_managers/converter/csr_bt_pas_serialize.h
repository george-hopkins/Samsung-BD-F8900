#ifndef CSR_BT_PAS_SERIALIZE_H__
#define CSR_BT_PAS_SERIALIZE_H__
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

void CsrBtPasPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
CsrSize CsrBtPasPullVcardMchListIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardMchListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardMchListIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullVcardMchListIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ
CsrSize CsrBtPasSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtPasSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasSecurityInReqSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
CsrSize CsrBtPasSetFolderIndSizeof(void *msg);
CsrUint8 *CsrBtPasSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSetFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasSetFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_RES
CsrSize CsrBtPasPullPbResSizeof(void *msg);
CsrUint8 *CsrBtPasPullPbResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullPbResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullPbResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_RES */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
CsrSize CsrBtPasAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtPasAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasAuthenticateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ
CsrSize CsrBtPasDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtPasDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasDeactivateReqSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
CsrSize CsrBtPasAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtPasAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
CsrSize CsrBtPasPullPbSizeIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullPbSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullPbSizeIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullPbSizeIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM
CsrSize CsrBtPasSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtPasSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasSecurityInCfmSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_IND
CsrSize CsrBtPasConnectIndSizeof(void *msg);
CsrUint8 *CsrBtPasConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasConnectIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM
CsrSize CsrBtPasAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtPasAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasAuthenticateCfmSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES
CsrSize CsrBtPasPullVcardListSizeResSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardListSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardListSizeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardListSizeResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_RES
CsrSize CsrBtPasConnectResSizeof(void *msg);
CsrUint8 *CsrBtPasConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasConnectResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES
CsrSize CsrBtPasPullVcardEntryResSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardEntryResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardEntryResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES
CsrSize CsrBtPasPullMchSizeResSizeof(void *msg);
CsrUint8 *CsrBtPasPullMchSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullMchSizeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullMchSizeResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND
CsrSize CsrBtPasPullMchSizeIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullMchSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullMchSizeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullMchSizeIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_IND
CsrSize CsrBtPasPullMchIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullMchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullMchIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullMchIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM
CsrSize CsrBtPasDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtPasDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasDeactivateCfmSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
CsrSize CsrBtPasPullVcardListIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardListIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullVcardListIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES
CsrSize CsrBtPasPullPbSizeResSizeof(void *msg);
CsrUint8 *CsrBtPasPullPbSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullPbSizeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullPbSizeResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES
CsrSize CsrBtPasSetFolderResSizeof(void *msg);
CsrUint8 *CsrBtPasSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSetFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasSetFolderResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
CsrSize CsrBtPasPullVcardEntryIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullVcardEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
CsrSize CsrBtPasAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtPasAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_RES
CsrSize CsrBtPasPullMchResSizeof(void *msg);
CsrUint8 *CsrBtPasPullMchResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullMchResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullMchResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES
CsrSize CsrBtPasPullVcardMchListResSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardMchListResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardMchListResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardMchListResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND
CsrSize CsrBtPasSetRootFolderIndSizeof(void *msg);
CsrUint8 *CsrBtPasSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasSetRootFolderIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES
CsrSize CsrBtPasPullVcardListResSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardListResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardListResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardListResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES
CsrSize CsrBtPasPullVcardMchListSizeResSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardMchListSizeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardMchListSizeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardMchListSizeResSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
CsrSize CsrBtPasPullPbIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullPbIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullPbIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullPbIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */

#ifndef EXCLUDE_CSR_BT_PAS_DISCONNECT_IND
CsrSize CsrBtPasDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtPasDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasDisconnectIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_IND
CsrSize CsrBtPasNextIndSizeof(void *msg);
CsrUint8 *CsrBtPasNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasNextIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ
CsrSize CsrBtPasActivateReqSizeof(void *msg);
CsrUint8 *CsrBtPasActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasActivateReqSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
CsrSize CsrBtPasPullVcardListSizeIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardListSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardListSizeIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasPullVcardListSizeIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
CsrSize CsrBtPasAbortIndSizeof(void *msg);
CsrUint8 *CsrBtPasAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
CsrSize CsrBtPasNextResSizeof(void *msg);
CsrUint8 *CsrBtPasNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtPasNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND
CsrSize CsrBtPasPullVcardMchListSizeIndSizeof(void *msg);
CsrUint8 *CsrBtPasPullVcardMchListSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasPullVcardMchListSizeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasPullVcardMchListSizeIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND
CsrSize CsrBtPasSetBackFolderIndSizeof(void *msg);
CsrUint8 *CsrBtPasSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtPasSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtPasSetBackFolderIndSerFree CsrBtPasPfree
#endif /* EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND */

#ifdef __cplusplus
}
#endif

#endif
