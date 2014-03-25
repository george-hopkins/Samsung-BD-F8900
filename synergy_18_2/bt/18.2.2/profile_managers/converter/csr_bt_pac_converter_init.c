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
#include "csr_bt_pac_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PAC_MODULE
#include "csr_bt_pac_serialize.h"
#include "csr_bt_pac_prim.h"

static CsrMsgConvMsgEntry csr_bt_pac_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PAC_ABORT_CFM
    { CSR_BT_PAC_ABORT_CFM, CsrBtPacAbortCfmSizeof, CsrBtPacAbortCfmSer, CsrBtPacAbortCfmDes, CsrBtPacAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_ABORT_REQ
    { CSR_BT_PAC_ABORT_REQ, CsrBtPacAbortReqSizeof, CsrBtPacAbortReqSer, CsrBtPacAbortReqDes, CsrBtPacAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND
    { CSR_BT_PAC_AUTHENTICATE_IND, CsrBtPacAuthenticateIndSizeof, CsrBtPacAuthenticateIndSer, CsrBtPacAuthenticateIndDes, CsrBtPacAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
    { CSR_BT_PAC_AUTHENTICATE_RES, CsrBtPacAuthenticateResSizeof, CsrBtPacAuthenticateResSer, CsrBtPacAuthenticateResDes, CsrBtPacAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ
    { CSR_BT_PAC_CANCEL_CONNECT_REQ, CsrBtPacCancelConnectReqSizeof, CsrBtPacCancelConnectReqSer, CsrBtPacCancelConnectReqDes, CsrBtPacCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_CFM
    { CSR_BT_PAC_CONNECT_CFM, CsrBtPacConnectCfmSizeof, CsrBtPacConnectCfmSer, CsrBtPacConnectCfmDes, CsrBtPacConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
    { CSR_BT_PAC_CONNECT_REQ, CsrBtPacConnectReqSizeof, CsrBtPacConnectReqSer, CsrBtPacConnectReqDes, CsrBtPacConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_IND
    { CSR_BT_PAC_DISCONNECT_IND, CsrBtPacDisconnectIndSizeof, CsrBtPacDisconnectIndSer, CsrBtPacDisconnectIndDes, CsrBtPacDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ
    { CSR_BT_PAC_DISCONNECT_REQ, CsrBtPacDisconnectReqSizeof, CsrBtPacDisconnectReqSer, CsrBtPacDisconnectReqDes, CsrBtPacDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING
    { CSR_BT_PAC_HOUSE_CLEANING, CsrBtPacHouseCleaningSizeof, CsrBtPacHouseCleaningSer, CsrBtPacHouseCleaningDes, CsrBtPacHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_CFM
    { CSR_BT_PAC_PULL_PB_CFM, CsrBtPacPullPbCfmSizeof, CsrBtPacPullPbCfmSer, CsrBtPacPullPbCfmDes, CsrBtPacPullPbCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_IND
    { CSR_BT_PAC_PULL_PB_IND, CsrBtPacPullPbIndSizeof, CsrBtPacPullPbIndSer, CsrBtPacPullPbIndDes, CsrBtPacPullPbIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_IND */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
    { CSR_BT_PAC_PULL_PB_REQ, CsrBtPacPullPbReqSizeof, CsrBtPacPullPbReqSer, CsrBtPacPullPbReqDes, CsrBtPacPullPbReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_RES
    { CSR_BT_PAC_PULL_PB_RES, CsrBtPacPullPbResSizeof, CsrBtPacPullPbResSer, CsrBtPacPullPbResDes, CsrBtPacPullPbResSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_RES */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM
    { CSR_BT_PAC_PULL_VCARD_ENTRY_CFM, CsrBtPacPullVcardEntryCfmSizeof, CsrBtPacPullVcardEntryCfmSer, CsrBtPacPullVcardEntryCfmDes, CsrBtPacPullVcardEntryCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND
    { CSR_BT_PAC_PULL_VCARD_ENTRY_IND, CsrBtPacPullVcardEntryIndSizeof, CsrBtPacPullVcardEntryIndSer, CsrBtPacPullVcardEntryIndDes, CsrBtPacPullVcardEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
    { CSR_BT_PAC_PULL_VCARD_ENTRY_REQ, CsrBtPacPullVcardEntryReqSizeof, CsrBtPacPullVcardEntryReqSer, CsrBtPacPullVcardEntryReqDes, CsrBtPacPullVcardEntryReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES
    { CSR_BT_PAC_PULL_VCARD_ENTRY_RES, CsrBtPacPullVcardEntryResSizeof, CsrBtPacPullVcardEntryResSer, CsrBtPacPullVcardEntryResDes, CsrBtPacPullVcardEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM
    { CSR_BT_PAC_PULL_VCARD_LIST_CFM, CsrBtPacPullVcardListCfmSizeof, CsrBtPacPullVcardListCfmSer, CsrBtPacPullVcardListCfmDes, CsrBtPacPullVcardListCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND
    { CSR_BT_PAC_PULL_VCARD_LIST_IND, CsrBtPacPullVcardListIndSizeof, CsrBtPacPullVcardListIndSer, CsrBtPacPullVcardListIndDes, CsrBtPacPullVcardListIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_IND */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
    { CSR_BT_PAC_PULL_VCARD_LIST_REQ, CsrBtPacPullVcardListReqSizeof, CsrBtPacPullVcardListReqSer, CsrBtPacPullVcardListReqDes, CsrBtPacPullVcardListReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES
    { CSR_BT_PAC_PULL_VCARD_LIST_RES, CsrBtPacPullVcardListResSizeof, CsrBtPacPullVcardListResSer, CsrBtPacPullVcardListResDes, CsrBtPacPullVcardListResSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_RES */
#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM
    { CSR_BT_PAC_SECURITY_OUT_CFM, CsrBtPacSecurityOutCfmSizeof, CsrBtPacSecurityOutCfmSer, CsrBtPacSecurityOutCfmDes, CsrBtPacSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ
    { CSR_BT_PAC_SECURITY_OUT_REQ, CsrBtPacSecurityOutReqSizeof, CsrBtPacSecurityOutReqSer, CsrBtPacSecurityOutReqDes, CsrBtPacSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM
    { CSR_BT_PAC_SET_BACK_FOLDER_CFM, CsrBtPacSetBackFolderCfmSizeof, CsrBtPacSetBackFolderCfmSer, CsrBtPacSetBackFolderCfmDes, CsrBtPacSetBackFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ
    { CSR_BT_PAC_SET_BACK_FOLDER_REQ, CsrBtPacSetBackFolderReqSizeof, CsrBtPacSetBackFolderReqSer, CsrBtPacSetBackFolderReqDes, CsrBtPacSetBackFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_BACK_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM
    { CSR_BT_PAC_SET_FOLDER_CFM, CsrBtPacSetFolderCfmSizeof, CsrBtPacSetFolderCfmSer, CsrBtPacSetFolderCfmDes, CsrBtPacSetFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
    { CSR_BT_PAC_SET_FOLDER_REQ, CsrBtPacSetFolderReqSizeof, CsrBtPacSetFolderReqSer, CsrBtPacSetFolderReqDes, CsrBtPacSetFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM
    { CSR_BT_PAC_SET_ROOT_FOLDER_CFM, CsrBtPacSetRootFolderCfmSizeof, CsrBtPacSetRootFolderCfmSer, CsrBtPacSetRootFolderCfmDes, CsrBtPacSetRootFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ
    { CSR_BT_PAC_SET_ROOT_FOLDER_REQ, CsrBtPacSetRootFolderReqSizeof, CsrBtPacSetRootFolderReqSer, CsrBtPacSetRootFolderReqDes, CsrBtPacSetRootFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAC_SET_ROOT_FOLDER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtPacConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PAC_PRIM, csr_bt_pac_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_pac_conv_info = {
    CSR_BT_PAC_PRIM,
    "CSR_BT_PAC_PRIM",
    csr_bt_pac_conv_lut
};

CsrLogPrimitiveInformation* CsrBtPacTechInfoGet(void)
{
    return &csr_bt_pac_conv_info;
}
#endif
#endif

