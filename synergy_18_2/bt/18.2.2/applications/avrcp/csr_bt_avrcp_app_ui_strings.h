#ifndef CSR_BT_AVRCP_APP_UI_STRINGS_H__
#define CSR_BT_AVRCP_APP_UI_STRINGS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_ui_strings.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_MAIN_MENU_STRING                             "AVRCP Main Menu"
#define TEXT_AVRCP_ACTIVATE_MENU_STRING                   "AVRCP_ACTIVATE"
#define TEXT_AVRCP_DEACTIVATE_STRING                      "Deactivate AVRCP"
#define TEXT_AVRCP_CONNECT_STRING                         "Connect AVRCP"
#define TEXT_AVRCP_ESTABLISH_CON_STRING                   "Establish an AVRCP connection"
#define TEXT_AVRCP_MODE_STRING                            "AVRCP mode"
#define TEXT_AVRCP_CONNECTING_STRING                      "AVRCP connecting..."
#define TEXT_AVRCP_DISOCONNECT_STRING                     "Disconnect the AVRCP Connection"
#define TEXT_AVRCP_DISCONNECT_CFM_STRING                  "AVRCP Disconnected"
#define TEXT_AVRCP_SETTING_FRAGMENTATION_ABORT_STRING     "Fragmentation Abort flag set\nThe flag will be set to CONTINUE automatically after use\n"  
#define TEXT_AVRCP_VERSION_STRING                         "AVRCP Version"
                                                                        



/* UI text defines */
#define TEXT_MAIN_MENU_UCS2                        CONVERT_TEXT_STRING_2_UCS2(TEXT_MAIN_MENU_STRING)
#define TEXT_AVRCP_ACTIVATE_MENU_UCS2              CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_ACTIVATE_MENU_STRING)
#define TEXT_AVRCP_DEACTIVATE_UCS2                 CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_DEACTIVATE_STRING)
#define TEXT_AVRCP_CONNECT_UCS2                    CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_CONNECT_STRING)
#define TEXT_AVRCP_ESTABLISH_CON_UCS2              CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_ESTABLISH_CON_STRING)
#define TEXT_AVRCP_MODE_UCS2                       CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_MODE_STRING)
#define TEXT_AVRCP_CONNECTING_UCS2                 CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_CONNECTING_STRING)
#define TEXT_AVRCP_DISOCONNECT_UCS2                CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_DISOCONNECT_STRING)
#define TEXT_AVRCP_DISCONNECT_CFM_UCS2             CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_DISCONNECT_CFM_STRING)                      
#define TEXT_AVRCP_SETTING_FRAGMENTATION_ABORT_UCS2 CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_SETTING_FRAGMENTATION_ABORT_STRING)
#define TEXT_AVRCP_VERSION_UCS2                    CONVERT_TEXT_STRING_2_UCS2(TEXT_AVRCP_VERSION_STRING)







#ifdef __cplusplus
}
#endif



#endif /* CSR_BT_AV_APP_UI_STRINGS_H__ */
