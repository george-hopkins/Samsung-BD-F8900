#ifndef CSR_BT_OBEX_FTP_STRING_H__
#define CSR_BT_OBEX_FTP_STRING_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_ui_strings.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Key defines */
/* Main menu */
#define CSR_BT_FTP_SERVER_APP_INVALID_KEY 0x00
#define CSR_BT_FTP_SERVER_APP_ACTIVATE_KEY 0x01
#define CSR_BT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_KEY 0x02
#define CSR_BT_FTP_SERVER_APP_DISABLE_SRM_KEY 0x03
#define CSR_BT_FTP_SERVER_APP_ENABLE_SRM_KEY 0x04
#define CSR_BT_FTP_SERVER_APP_DEACTIVATE_KEY 0x05
#define CSR_BT_FTP_SERVER_APP_SWITCH_TO_MASTER_KEY 0x06

/* Allow object menu*/
#define CSR_BT_FTP_SERVER_APP_YES_KEY 0x10
#define CSR_BT_FTP_SERVER_APP_NO_KEY 0x11
#define CSR_BT_FTP_SERVER_APP_AUTHENTICATE_KEY 0x12

/* Main Menu TEXT defines */
#define TEXT_FTP_SERVER_APP_MAIN_MENU_STRING                        "OBEX FTP Server"
#define TEXT_FTP_SERVER_APP_ACTIVATE_STRING                         "Activate"
#define TEXT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_STRING     "Activate w. OBEX Authentication"
#define TEXT_FTP_SERVER_APP_DISABLE_SRM_STRING                      "Disable SRM"
#define TEXT_FTP_SERVER_APP_ENABLE_SRM_STRING                       "Enable SRM"
#define TEXT_FTP_SERVER_APP_DEACTIVATE_STRING                       "Deactivate"

#define TEXT_FTP_SERVER_APP_SWITCH_TO_MASTER_STRING                 "Switch to MASTER Role"
#define TEXT_FTP_SERVER_APP_ALLOW_STRING                            "Allow operation?"
#define TEXT_FTP_SERVER_APP_AUTHENTICATE_STRING                     "Authenticate"
#define TEXT_FTP_SERVER_APP_REJECT_STRING                           "Reject current operation"

#define TEXT_FTP_SERVER_APP_MAIN_MENU_UCS2                      CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_MAIN_MENU_STRING)
#define TEXT_FTP_SERVER_APP_ACTIVATE_UCS2                       CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_ACTIVATE_STRING)
#define TEXT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_UCS2   CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_ACTIVATE_WITH_AUTHENTICATION_STRING)
#define TEXT_FTP_SERVER_APP_DISABLE_SRM_UCS2                    CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_DISABLE_SRM_STRING)
#define TEXT_FTP_SERVER_APP_ENABLE_SRM_UCS2                     CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_ENABLE_SRM_STRING)
#define TEXT_FTP_SERVER_APP_DEACTIVATE_UCS2                     CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_DEACTIVATE_STRING)
#define TEXT_FTP_SERVER_APP_SWITCH_TO_MASTER_UCS2               CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_SWITCH_TO_MASTER_STRING)
#define TEXT_FTP_SERVER_APP_ALLOW_UCS2                          CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_ALLOW_STRING)
#define TEXT_FTP_SERVER_APP_AUTHENTICATE_UCS2                   CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_AUTHENTICATE_STRING)
#define TEXT_FTP_SERVER_APP_REJECT_UCS2                         CONVERT_TEXT_STRING_2_UCS2(TEXT_FTP_SERVER_APP_REJECT_STRING)

#ifdef __cplusplus
}
#endif

#endif
