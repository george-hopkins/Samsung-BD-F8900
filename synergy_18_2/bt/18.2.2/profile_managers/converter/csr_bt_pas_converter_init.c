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
#include "csr_bt_pas_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PAS_MODULE
#include "csr_bt_pas_serialize.h"
#include "csr_bt_pas_prim.h"

static CsrMsgConvMsgEntry csr_bt_pas_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PAS_ABORT_IND
    { CSR_BT_PAS_ABORT_IND, CsrBtPasAbortIndSizeof, CsrBtPasAbortIndSer, CsrBtPasAbortIndDes, CsrBtPasAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ
    { CSR_BT_PAS_ACTIVATE_REQ, CsrBtPasActivateReqSizeof, CsrBtPasActivateReqSer, CsrBtPasActivateReqDes, CsrBtPasActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM
    { CSR_BT_PAS_AUTHENTICATE_CFM, CsrBtPasAuthenticateCfmSizeof, CsrBtPasAuthenticateCfmSer, CsrBtPasAuthenticateCfmDes, CsrBtPasAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND
    { CSR_BT_PAS_AUTHENTICATE_IND, CsrBtPasAuthenticateIndSizeof, CsrBtPasAuthenticateIndSer, CsrBtPasAuthenticateIndDes, CsrBtPasAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
    { CSR_BT_PAS_AUTHENTICATE_REQ, CsrBtPasAuthenticateReqSizeof, CsrBtPasAuthenticateReqSer, CsrBtPasAuthenticateReqDes, CsrBtPasAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
    { CSR_BT_PAS_AUTHENTICATE_RES, CsrBtPasAuthenticateResSizeof, CsrBtPasAuthenticateResSer, CsrBtPasAuthenticateResDes, CsrBtPasAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_IND
    { CSR_BT_PAS_CONNECT_IND, CsrBtPasConnectIndSizeof, CsrBtPasConnectIndSer, CsrBtPasConnectIndDes, CsrBtPasConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PAS_CONNECT_RES
    { CSR_BT_PAS_CONNECT_RES, CsrBtPasConnectResSizeof, CsrBtPasConnectResSer, CsrBtPasConnectResDes, CsrBtPasConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM
    { CSR_BT_PAS_DEACTIVATE_CFM, CsrBtPasDeactivateCfmSizeof, CsrBtPasDeactivateCfmSer, CsrBtPasDeactivateCfmDes, CsrBtPasDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ
    { CSR_BT_PAS_DEACTIVATE_REQ, CsrBtPasDeactivateReqSizeof, CsrBtPasDeactivateReqSer, CsrBtPasDeactivateReqDes, CsrBtPasDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_PAS_DISCONNECT_IND
    { CSR_BT_PAS_DISCONNECT_IND, CsrBtPasDisconnectIndSizeof, CsrBtPasDisconnectIndSer, CsrBtPasDisconnectIndDes, CsrBtPasDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PAS_NEXT_IND
    { CSR_BT_PAS_NEXT_IND, CsrBtPasNextIndSizeof, CsrBtPasNextIndSer, CsrBtPasNextIndDes, CsrBtPasNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
    { CSR_BT_PAS_NEXT_RES, CsrBtPasNextResSizeof, CsrBtPasNextResSer, CsrBtPasNextResDes, CsrBtPasNextResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_IND
    { CSR_BT_PAS_PULL_MCH_IND, CsrBtPasPullMchIndSizeof, CsrBtPasPullMchIndSer, CsrBtPasPullMchIndDes, CsrBtPasPullMchIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_RES
    { CSR_BT_PAS_PULL_MCH_RES, CsrBtPasPullMchResSizeof, CsrBtPasPullMchResSer, CsrBtPasPullMchResDes, CsrBtPasPullMchResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND
    { CSR_BT_PAS_PULL_MCH_SIZE_IND, CsrBtPasPullMchSizeIndSizeof, CsrBtPasPullMchSizeIndSer, CsrBtPasPullMchSizeIndDes, CsrBtPasPullMchSizeIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES
    { CSR_BT_PAS_PULL_MCH_SIZE_RES, CsrBtPasPullMchSizeResSizeof, CsrBtPasPullMchSizeResSer, CsrBtPasPullMchSizeResDes, CsrBtPasPullMchSizeResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_MCH_SIZE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_IND
    { CSR_BT_PAS_PULL_PB_IND, CsrBtPasPullPbIndSizeof, CsrBtPasPullPbIndSer, CsrBtPasPullPbIndDes, CsrBtPasPullPbIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_RES
    { CSR_BT_PAS_PULL_PB_RES, CsrBtPasPullPbResSizeof, CsrBtPasPullPbResSer, CsrBtPasPullPbResDes, CsrBtPasPullPbResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND
    { CSR_BT_PAS_PULL_PB_SIZE_IND, CsrBtPasPullPbSizeIndSizeof, CsrBtPasPullPbSizeIndSer, CsrBtPasPullPbSizeIndDes, CsrBtPasPullPbSizeIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES
    { CSR_BT_PAS_PULL_PB_SIZE_RES, CsrBtPasPullPbSizeResSizeof, CsrBtPasPullPbSizeResSer, CsrBtPasPullPbSizeResDes, CsrBtPasPullPbSizeResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_PB_SIZE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND
    { CSR_BT_PAS_PULL_VCARD_ENTRY_IND, CsrBtPasPullVcardEntryIndSizeof, CsrBtPasPullVcardEntryIndSer, CsrBtPasPullVcardEntryIndDes, CsrBtPasPullVcardEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES
    { CSR_BT_PAS_PULL_VCARD_ENTRY_RES, CsrBtPasPullVcardEntryResSizeof, CsrBtPasPullVcardEntryResSer, CsrBtPasPullVcardEntryResDes, CsrBtPasPullVcardEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND
    { CSR_BT_PAS_PULL_VCARD_LIST_IND, CsrBtPasPullVcardListIndSizeof, CsrBtPasPullVcardListIndSer, CsrBtPasPullVcardListIndDes, CsrBtPasPullVcardListIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES
    { CSR_BT_PAS_PULL_VCARD_LIST_RES, CsrBtPasPullVcardListResSizeof, CsrBtPasPullVcardListResSer, CsrBtPasPullVcardListResDes, CsrBtPasPullVcardListResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND
    { CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND, CsrBtPasPullVcardListSizeIndSizeof, CsrBtPasPullVcardListSizeIndSer, CsrBtPasPullVcardListSizeIndDes, CsrBtPasPullVcardListSizeIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES
    { CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES, CsrBtPasPullVcardListSizeResSizeof, CsrBtPasPullVcardListSizeResSer, CsrBtPasPullVcardListSizeResDes, CsrBtPasPullVcardListSizeResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND
    { CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND, CsrBtPasPullVcardMchListIndSizeof, CsrBtPasPullVcardMchListIndSer, CsrBtPasPullVcardMchListIndDes, CsrBtPasPullVcardMchListIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES
    { CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES, CsrBtPasPullVcardMchListResSizeof, CsrBtPasPullVcardMchListResSer, CsrBtPasPullVcardMchListResDes, CsrBtPasPullVcardMchListResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND
    { CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND, CsrBtPasPullVcardMchListSizeIndSizeof, CsrBtPasPullVcardMchListSizeIndSer, CsrBtPasPullVcardMchListSizeIndDes, CsrBtPasPullVcardMchListSizeIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES
    { CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES, CsrBtPasPullVcardMchListSizeResSizeof, CsrBtPasPullVcardMchListSizeResSer, CsrBtPasPullVcardMchListSizeResDes, CsrBtPasPullVcardMchListSizeResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM
    { CSR_BT_PAS_SECURITY_IN_CFM, CsrBtPasSecurityInCfmSizeof, CsrBtPasSecurityInCfmSer, CsrBtPasSecurityInCfmDes, CsrBtPasSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ
    { CSR_BT_PAS_SECURITY_IN_REQ, CsrBtPasSecurityInReqSizeof, CsrBtPasSecurityInReqSer, CsrBtPasSecurityInReqDes, CsrBtPasSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND
    { CSR_BT_PAS_SET_BACK_FOLDER_IND, CsrBtPasSetBackFolderIndSizeof, CsrBtPasSetBackFolderIndSer, CsrBtPasSetBackFolderIndDes, CsrBtPasSetBackFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SET_BACK_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND
    { CSR_BT_PAS_SET_FOLDER_IND, CsrBtPasSetFolderIndSizeof, CsrBtPasSetFolderIndSer, CsrBtPasSetFolderIndDes, CsrBtPasSetFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES
    { CSR_BT_PAS_SET_FOLDER_RES, CsrBtPasSetFolderResSizeof, CsrBtPasSetFolderResSer, CsrBtPasSetFolderResDes, CsrBtPasSetFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SET_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND
    { CSR_BT_PAS_SET_ROOT_FOLDER_IND, CsrBtPasSetRootFolderIndSizeof, CsrBtPasSetRootFolderIndSer, CsrBtPasSetRootFolderIndDes, CsrBtPasSetRootFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_PAS_SET_ROOT_FOLDER_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtPasConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PAS_PRIM, csr_bt_pas_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_pas_conv_info = {
    CSR_BT_PAS_PRIM,
    "CSR_BT_PAS_PRIM",
    csr_bt_pas_conv_lut
};

CsrLogPrimitiveInformation* CsrBtPasTechInfoGet(void)
{
    return &csr_bt_pas_conv_info;
}
#endif
#endif

