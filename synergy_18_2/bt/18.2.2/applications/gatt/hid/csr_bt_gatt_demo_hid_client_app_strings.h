#ifndef CSR_BT_HIDC_STRING_H__
#define CSR_BT_HIDC_STRING_H__

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
#define CSR_BT_HIDC_APP_INVALID_KEY                0x00
#define CSR_BT_HIDC_APP_REGISTER_KEY               0x01
#define CSR_BT_HIDC_APP_UNREGISTER_KEY             0x02
#define CSR_BT_HIDC_APP_CONNECT_KEY                0x03
#define CSR_BT_HIDC_APP_CONNECT_WHITELIST_KEY      0x04
#define CSR_BT_HIDC_APP_ADD_WHITELIST_KEY          0x05
#define CSR_BT_HIDC_APP_DISCONNECT_KEY             0x06
#define CSR_BT_HIDC_APP_TOGGLE_CONTROL_REPORT_KEY  0x07
#define CSR_BT_HIDC_APP_TOGGLE_INTERUPT_REPORT_KEY 0x08
#define CSR_BT_HIDC_APP_READ_REPORT_DESCRIPTOR_KEY 0x09
#define CSR_BT_HIDC_APP_SHOW_INPUT_DIALOG_KEY      0x0a
#define CSR_BT_HIDC_APP_OK_KEY                     0x0b
#define CSR_BT_HIDC_APP_BATTERY_INFO_KEY           0x0c

/* Main Menu TEXT defines */
#define TEXT_HIDC_APP_REGISTER_UCS2               CONVERT_TEXT_STRING_2_UCS2("Register client")
#define TEXT_HIDC_APP_UNREGISTER_UCS2             CONVERT_TEXT_STRING_2_UCS2("Unregister client")
#define TEXT_HIDC_APP_CONNECT_UCS2                CONVERT_TEXT_STRING_2_UCS2("Connect")
#define TEXT_HIDC_APP_CONNECT_SUB_UCS2            CONVERT_TEXT_STRING_2_UCS2("to selected device")
#define TEXT_HIDC_APP_ADD_WHITELIST_UCS2          CONVERT_TEXT_STRING_2_UCS2("Add to Whitelist")
#define TEXT_HIDC_APP_ADD_WHITELIST_SUB_UCS2      CONVERT_TEXT_STRING_2_UCS2("")
#define TEXT_HIDC_APP_CONNECT_WHITELIST_UCS2      CONVERT_TEXT_STRING_2_UCS2("Whitelist Connect")
#define TEXT_HIDC_APP_CONNECT_WHITELIST_SUB_UCS2  CONVERT_TEXT_STRING_2_UCS2("Listen for incoming connections")
#define TEXT_HIDC_APP_DISCONNECT_UCS2             CONVERT_TEXT_STRING_2_UCS2("Disconnect")
#define TEXT_HIDC_APP_TOGGLE_CONTROL_REPORT_UCS2  CONVERT_TEXT_STRING_2_UCS2("Toggle Control Report")
#define TEXT_HIDC_APP_SHOW_INPUT_DIALOG_UCS2      CONVERT_TEXT_STRING_2_UCS2("Input dialog")
#define TEXT_HIDC_APP_SHOW_INPUT_DIALOG_SUB_UCS2  CONVERT_TEXT_STRING_2_UCS2("Show test dialog")
#define TEXT_HIDC_APP_TOGGLE_INTERUPT_REPORT_UCS2 CONVERT_TEXT_STRING_2_UCS2("Toggle on/off") /*this is actually used to turn on/off key-press 
                                                                                                notifications from the keyboard */
#define TEXT_HIDC_APP_READ_REPORT_DESCRIPTOR_UCS2 CONVERT_TEXT_STRING_2_UCS2("Report Descriptor")
#define TEXT_HIDC_APP_BATTERY_INFO_UCS2           CONVERT_TEXT_STRING_2_UCS2("Battery Info")
    
#define TEXT_HIDC_APP_OK_UCS2                    CONVERT_TEXT_STRING_2_UCS2("OK")

#endif /* CSR_BT_HIDC_STRING_H__ */


