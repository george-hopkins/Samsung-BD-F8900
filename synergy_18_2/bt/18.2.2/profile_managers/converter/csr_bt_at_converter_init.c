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
#include "csr_bt_at_converter_init.h"
#ifndef EXCLUDE_CSR_BT_AT_MODULE
#include "csr_bt_at_serialize.h"
#include "csr_bt_at_prim.h"

static CsrMsgConvMsgEntry csr_bt_at_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ
    { CSR_BT_AT_DG_ACTIVATE_REQ, CsrBtAtDgActivateReqSizeof, CsrBtAtDgActivateReqSer, CsrBtAtDgActivateReqDes, CsrBtAtDgActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_A_IND
    { CSR_BT_AT_DG_ATC_A_IND, CsrBtAtDgAtcAIndSizeof, CsrBtAtDgAtcAIndSer, CsrBtAtDgAtcAIndDes, CsrBtAtDgAtcAIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_A_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND
    { CSR_BT_AT_DG_ATC_ABORT_IND, CsrBtAtDgAtcAbortIndSizeof, CsrBtAtDgAtcAbortIndSer, CsrBtAtDgAtcAbortIndDes, CsrBtAtDgAtcAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND
    { CSR_BT_AT_DG_ATC_AND_C_IND, CsrBtAtDgAtcAndCIndSizeof, CsrBtAtDgAtcAndCIndSer, CsrBtAtDgAtcAndCIndDes, CsrBtAtDgAtcAndCIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND
    { CSR_BT_AT_DG_ATC_AND_D_IND, CsrBtAtDgAtcAndDIndSizeof, CsrBtAtDgAtcAndDIndSer, CsrBtAtDgAtcAndDIndDes, CsrBtAtDgAtcAndDIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND
    { CSR_BT_AT_DG_ATC_AND_F_IND, CsrBtAtDgAtcAndFIndSizeof, CsrBtAtDgAtcAndFIndSer, CsrBtAtDgAtcAndFIndDes, CsrBtAtDgAtcAndFIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
    { CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES, CsrBtAtDgAtcConnectTextResSizeof, CsrBtAtDgAtcConnectTextResSer, CsrBtAtDgAtcConnectTextResDes, CsrBtAtDgAtcConnectTextResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
    { CSR_BT_AT_DG_ATC_D_IND, CsrBtAtDgAtcDIndSizeof, CsrBtAtDgAtcDIndSer, CsrBtAtDgAtcDIndDes, CsrBtAtDgAtcDIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES
    { CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES, CsrBtAtDgAtcDefaultSettingResSizeof, CsrBtAtDgAtcDefaultSettingResSer, CsrBtAtDgAtcDefaultSettingResDes, CsrBtAtDgAtcDefaultSettingResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_E_IND
    { CSR_BT_AT_DG_ATC_E_IND, CsrBtAtDgAtcEIndSizeof, CsrBtAtDgAtcEIndSer, CsrBtAtDgAtcEIndDes, CsrBtAtDgAtcEIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_E_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES
    { CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES, CsrBtAtDgAtcFactorySettingResSizeof, CsrBtAtDgAtcFactorySettingResSer, CsrBtAtDgAtcFactorySettingResDes, CsrBtAtDgAtcFactorySettingResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND
    { CSR_BT_AT_DG_ATC_GCAP_IND, CsrBtAtDgAtcGcapIndSizeof, CsrBtAtDgAtcGcapIndSer, CsrBtAtDgAtcGcapIndDes, CsrBtAtDgAtcGcapIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
    { CSR_BT_AT_DG_ATC_GCAP_RES, CsrBtAtDgAtcGcapResSizeof, CsrBtAtDgAtcGcapResSer, CsrBtAtDgAtcGcapResDes, CsrBtAtDgAtcGcapResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND
    { CSR_BT_AT_DG_ATC_GMI_IND, CsrBtAtDgAtcGmiIndSizeof, CsrBtAtDgAtcGmiIndSer, CsrBtAtDgAtcGmiIndDes, CsrBtAtDgAtcGmiIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
    { CSR_BT_AT_DG_ATC_GMI_RES, CsrBtAtDgAtcGmiResSizeof, CsrBtAtDgAtcGmiResSer, CsrBtAtDgAtcGmiResDes, CsrBtAtDgAtcGmiResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND
    { CSR_BT_AT_DG_ATC_GMM_IND, CsrBtAtDgAtcGmmIndSizeof, CsrBtAtDgAtcGmmIndSer, CsrBtAtDgAtcGmmIndDes, CsrBtAtDgAtcGmmIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
    { CSR_BT_AT_DG_ATC_GMM_RES, CsrBtAtDgAtcGmmResSizeof, CsrBtAtDgAtcGmmResSer, CsrBtAtDgAtcGmmResDes, CsrBtAtDgAtcGmmResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND
    { CSR_BT_AT_DG_ATC_GMR_IND, CsrBtAtDgAtcGmrIndSizeof, CsrBtAtDgAtcGmrIndSer, CsrBtAtDgAtcGmrIndDes, CsrBtAtDgAtcGmrIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
    { CSR_BT_AT_DG_ATC_GMR_RES, CsrBtAtDgAtcGmrResSizeof, CsrBtAtDgAtcGmrResSer, CsrBtAtDgAtcGmrResDes, CsrBtAtDgAtcGmrResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_H_IND
    { CSR_BT_AT_DG_ATC_H_IND, CsrBtAtDgAtcHIndSizeof, CsrBtAtDgAtcHIndSer, CsrBtAtDgAtcHIndDes, CsrBtAtDgAtcHIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_H_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_L_IND
    { CSR_BT_AT_DG_ATC_L_IND, CsrBtAtDgAtcLIndSizeof, CsrBtAtDgAtcLIndSer, CsrBtAtDgAtcLIndDes, CsrBtAtDgAtcLIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_L_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_M_IND
    { CSR_BT_AT_DG_ATC_M_IND, CsrBtAtDgAtcMIndSizeof, CsrBtAtDgAtcMIndSer, CsrBtAtDgAtcMIndDes, CsrBtAtDgAtcMIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_M_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_O_IND
    { CSR_BT_AT_DG_ATC_O_IND, CsrBtAtDgAtcOIndSizeof, CsrBtAtDgAtcOIndSer, CsrBtAtDgAtcOIndDes, CsrBtAtDgAtcOIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_O_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_P_IND
    { CSR_BT_AT_DG_ATC_P_IND, CsrBtAtDgAtcPIndSizeof, CsrBtAtDgAtcPIndSer, CsrBtAtDgAtcPIndDes, CsrBtAtDgAtcPIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_P_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND
    { CSR_BT_AT_DG_ATC_Q_IND, CsrBtAtDgAtcQIndSizeof, CsrBtAtDgAtcQIndSer, CsrBtAtDgAtcQIndDes, CsrBtAtDgAtcQIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES
    { CSR_BT_AT_DG_ATC_RESULT_CODE_RES, CsrBtAtDgAtcResultCodeResSizeof, CsrBtAtDgAtcResultCodeResSer, CsrBtAtDgAtcResultCodeResDes, CsrBtAtDgAtcResultCodeResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND
    { CSR_BT_AT_DG_ATC_S0_IND, CsrBtAtDgAtcS0IndSizeof, CsrBtAtDgAtcS0IndSer, CsrBtAtDgAtcS0IndDes, CsrBtAtDgAtcS0IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND
    { CSR_BT_AT_DG_ATC_S0Q_IND, CsrBtAtDgAtcS0qIndSizeof, CsrBtAtDgAtcS0qIndSer, CsrBtAtDgAtcS0qIndDes, CsrBtAtDgAtcS0qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES
    { CSR_BT_AT_DG_ATC_S0Q_RES, CsrBtAtDgAtcS0qResSizeof, CsrBtAtDgAtcS0qResSer, CsrBtAtDgAtcS0qResDes, CsrBtAtDgAtcS0qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND
    { CSR_BT_AT_DG_ATC_S10_IND, CsrBtAtDgAtcS10IndSizeof, CsrBtAtDgAtcS10IndSer, CsrBtAtDgAtcS10IndDes, CsrBtAtDgAtcS10IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND
    { CSR_BT_AT_DG_ATC_S10Q_IND, CsrBtAtDgAtcS10qIndSizeof, CsrBtAtDgAtcS10qIndSer, CsrBtAtDgAtcS10qIndDes, CsrBtAtDgAtcS10qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES
    { CSR_BT_AT_DG_ATC_S10Q_RES, CsrBtAtDgAtcS10qResSizeof, CsrBtAtDgAtcS10qResSer, CsrBtAtDgAtcS10qResDes, CsrBtAtDgAtcS10qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND
    { CSR_BT_AT_DG_ATC_S3_IND, CsrBtAtDgAtcS3IndSizeof, CsrBtAtDgAtcS3IndSer, CsrBtAtDgAtcS3IndDes, CsrBtAtDgAtcS3IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND
    { CSR_BT_AT_DG_ATC_S3Q_IND, CsrBtAtDgAtcS3qIndSizeof, CsrBtAtDgAtcS3qIndSer, CsrBtAtDgAtcS3qIndDes, CsrBtAtDgAtcS3qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES
    { CSR_BT_AT_DG_ATC_S3Q_RES, CsrBtAtDgAtcS3qResSizeof, CsrBtAtDgAtcS3qResSer, CsrBtAtDgAtcS3qResDes, CsrBtAtDgAtcS3qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND
    { CSR_BT_AT_DG_ATC_S4_IND, CsrBtAtDgAtcS4IndSizeof, CsrBtAtDgAtcS4IndSer, CsrBtAtDgAtcS4IndDes, CsrBtAtDgAtcS4IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND
    { CSR_BT_AT_DG_ATC_S4Q_IND, CsrBtAtDgAtcS4qIndSizeof, CsrBtAtDgAtcS4qIndSer, CsrBtAtDgAtcS4qIndDes, CsrBtAtDgAtcS4qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES
    { CSR_BT_AT_DG_ATC_S4Q_RES, CsrBtAtDgAtcS4qResSizeof, CsrBtAtDgAtcS4qResSer, CsrBtAtDgAtcS4qResDes, CsrBtAtDgAtcS4qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND
    { CSR_BT_AT_DG_ATC_S5_IND, CsrBtAtDgAtcS5IndSizeof, CsrBtAtDgAtcS5IndSer, CsrBtAtDgAtcS5IndDes, CsrBtAtDgAtcS5IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND
    { CSR_BT_AT_DG_ATC_S5Q_IND, CsrBtAtDgAtcS5qIndSizeof, CsrBtAtDgAtcS5qIndSer, CsrBtAtDgAtcS5qIndDes, CsrBtAtDgAtcS5qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES
    { CSR_BT_AT_DG_ATC_S5Q_RES, CsrBtAtDgAtcS5qResSizeof, CsrBtAtDgAtcS5qResSer, CsrBtAtDgAtcS5qResDes, CsrBtAtDgAtcS5qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND
    { CSR_BT_AT_DG_ATC_S6_IND, CsrBtAtDgAtcS6IndSizeof, CsrBtAtDgAtcS6IndSer, CsrBtAtDgAtcS6IndDes, CsrBtAtDgAtcS6IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND
    { CSR_BT_AT_DG_ATC_S6Q_IND, CsrBtAtDgAtcS6qIndSizeof, CsrBtAtDgAtcS6qIndSer, CsrBtAtDgAtcS6qIndDes, CsrBtAtDgAtcS6qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES
    { CSR_BT_AT_DG_ATC_S6Q_RES, CsrBtAtDgAtcS6qResSizeof, CsrBtAtDgAtcS6qResSer, CsrBtAtDgAtcS6qResDes, CsrBtAtDgAtcS6qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND
    { CSR_BT_AT_DG_ATC_S7_IND, CsrBtAtDgAtcS7IndSizeof, CsrBtAtDgAtcS7IndSer, CsrBtAtDgAtcS7IndDes, CsrBtAtDgAtcS7IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND
    { CSR_BT_AT_DG_ATC_S7Q_IND, CsrBtAtDgAtcS7qIndSizeof, CsrBtAtDgAtcS7qIndSer, CsrBtAtDgAtcS7qIndDes, CsrBtAtDgAtcS7qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES
    { CSR_BT_AT_DG_ATC_S7Q_RES, CsrBtAtDgAtcS7qResSizeof, CsrBtAtDgAtcS7qResSer, CsrBtAtDgAtcS7qResDes, CsrBtAtDgAtcS7qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND
    { CSR_BT_AT_DG_ATC_S8_IND, CsrBtAtDgAtcS8IndSizeof, CsrBtAtDgAtcS8IndSer, CsrBtAtDgAtcS8IndDes, CsrBtAtDgAtcS8IndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND
    { CSR_BT_AT_DG_ATC_S8Q_IND, CsrBtAtDgAtcS8qIndSizeof, CsrBtAtDgAtcS8qIndSer, CsrBtAtDgAtcS8qIndDes, CsrBtAtDgAtcS8qIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES
    { CSR_BT_AT_DG_ATC_S8Q_RES, CsrBtAtDgAtcS8qResSizeof, CsrBtAtDgAtcS8qResSer, CsrBtAtDgAtcS8qResDes, CsrBtAtDgAtcS8qResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_T_IND
    { CSR_BT_AT_DG_ATC_T_IND, CsrBtAtDgAtcTIndSizeof, CsrBtAtDgAtcTIndSer, CsrBtAtDgAtcTIndDes, CsrBtAtDgAtcTIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_T_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
    { CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND, CsrBtAtDgAtcUnknownExtendedCmdIndSizeof, CsrBtAtDgAtcUnknownExtendedCmdIndSer, CsrBtAtDgAtcUnknownExtendedCmdIndDes, CsrBtAtDgAtcUnknownExtendedCmdIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
    { CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES, CsrBtAtDgAtcUnknownExtendedCmdResSizeof, CsrBtAtDgAtcUnknownExtendedCmdResSer, CsrBtAtDgAtcUnknownExtendedCmdResDes, CsrBtAtDgAtcUnknownExtendedCmdResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_V_IND
    { CSR_BT_AT_DG_ATC_V_IND, CsrBtAtDgAtcVIndSizeof, CsrBtAtDgAtcVIndSer, CsrBtAtDgAtcVIndDes, CsrBtAtDgAtcVIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_V_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_X_IND
    { CSR_BT_AT_DG_ATC_X_IND, CsrBtAtDgAtcXIndSizeof, CsrBtAtDgAtcXIndSer, CsrBtAtDgAtcXIndDes, CsrBtAtDgAtcXIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_X_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND
    { CSR_BT_AT_DG_ATC_Z_IND, CsrBtAtDgAtcZIndSizeof, CsrBtAtDgAtcZIndSer, CsrBtAtDgAtcZIndDes, CsrBtAtDgAtcZIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_CONNECT_IND
    { CSR_BT_AT_DG_CONNECT_IND, CsrBtAtDgConnectIndSizeof, CsrBtAtDgConnectIndSer, CsrBtAtDgConnectIndDes, CsrBtAtDgConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_IND
    { CSR_BT_AT_DG_CONTROL_IND, CsrBtAtDgControlIndSizeof, CsrBtAtDgControlIndSer, CsrBtAtDgControlIndDes, CsrBtAtDgControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ
    { CSR_BT_AT_DG_CONTROL_REQ, CsrBtAtDgControlReqSizeof, CsrBtAtDgControlReqSer, CsrBtAtDgControlReqDes, CsrBtAtDgControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_CFM
    { CSR_BT_AT_DG_DATA_CFM, CsrBtAtDgDataCfmSizeof, CsrBtAtDgDataCfmSer, CsrBtAtDgDataCfmDes, CsrBtAtDgDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
    { CSR_BT_AT_DG_DATA_IND, CsrBtAtDgDataIndSizeof, CsrBtAtDgDataIndSer, CsrBtAtDgDataIndDes, CsrBtAtDgDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND
    { CSR_BT_AT_DG_DATA_PATH_STATUS_IND, CsrBtAtDgDataPathStatusIndSizeof, CsrBtAtDgDataPathStatusIndSer, CsrBtAtDgDataPathStatusIndDes, CsrBtAtDgDataPathStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
    { CSR_BT_AT_DG_DATA_REQ, CsrBtAtDgDataReqSizeof, CsrBtAtDgDataReqSer, CsrBtAtDgDataReqDes, CsrBtAtDgDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_RES
    { CSR_BT_AT_DG_DATA_RES, CsrBtAtDgDataResSizeof, CsrBtAtDgDataResSer, CsrBtAtDgDataResDes, CsrBtAtDgDataResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_RES */
#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM
    { CSR_BT_AT_DG_DEACTIVATE_CFM, CsrBtAtDgDeactivateCfmSizeof, CsrBtAtDgDeactivateCfmSer, CsrBtAtDgDeactivateCfmDes, CsrBtAtDgDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ
    { CSR_BT_AT_DG_DEACTIVATE_REQ, CsrBtAtDgDeactivateReqSizeof, CsrBtAtDgDeactivateReqSer, CsrBtAtDgDeactivateReqDes, CsrBtAtDgDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND
    { CSR_BT_AT_DG_DISCONNECT_IND, CsrBtAtDgDisconnectIndSizeof, CsrBtAtDgDisconnectIndSer, CsrBtAtDgDisconnectIndDes, CsrBtAtDgDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ
    { CSR_BT_AT_DG_DISCONNECT_REQ, CsrBtAtDgDisconnectReqSizeof, CsrBtAtDgDisconnectReqSer, CsrBtAtDgDisconnectReqDes, CsrBtAtDgDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND
    { CSR_BT_AT_DG_PORTNEG_IND, CsrBtAtDgPortnegIndSizeof, CsrBtAtDgPortnegIndSer, CsrBtAtDgPortnegIndDes, CsrBtAtDgPortnegIndSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES
    { CSR_BT_AT_DG_PORTNEG_RES, CsrBtAtDgPortnegResSizeof, CsrBtAtDgPortnegResSer, CsrBtAtDgPortnegResDes, CsrBtAtDgPortnegResSerFree },
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtAtConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_AT_PRIM, csr_bt_at_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_at_conv_info = {
    CSR_BT_AT_PRIM,
    "CSR_BT_AT_PRIM",
    csr_bt_at_conv_lut
};

CsrLogPrimitiveInformation* CsrBtAtTechInfoGet(void)
{
    return &csr_bt_at_conv_info;
}
#endif
#endif

