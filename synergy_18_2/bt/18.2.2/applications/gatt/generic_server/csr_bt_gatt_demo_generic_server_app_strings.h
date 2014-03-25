#ifndef CSR_BT_GENERICSRV_STRING_H__
#define CSR_BT_GENERICSRV_STRING_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #1 $
 ****************************************************************************/

#include "csr_bt_ui_strings.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Key defines */
#define CSR_BT_GENERIC_SRV_APP_INVALID_KEY               0x00
#define CSR_BT_GENERIC_SRV_APP_ACTIVATE_KEY              0x01
#define CSR_BT_GENERIC_SRV_APP_DEACTIVATE_KEY            0x02
#define CSR_BT_GENERIC_SRV_APP_TOGGLE_TEMP_TYPE_KEY      0x03
#define CSR_BT_GENERIC_SRV_APP_OK_KEY                    0x04
#define CSR_BT_GENERIC_SRV_APP_UNREGISTER_KEY            0x05
#define CSR_BT_GENERIC_SRV_APP_REGISTER_KEY              0x06
#define CSR_BT_GENERIC_SRV_APP_NOTIFY_KEY                0x07
#define CSR_BT_GENERIC_SRV_APP_INDICATE_KEY              0x08

/* Main Menu TEXT defines */
#define TEXT_GENERIC_SRV_APP_ACTIVATE_UCS2              CONVERT_TEXT_STRING_2_UCS2("Activate")
#define TEXT_GENERIC_SRV_APP_TOGGLE_TEMP_TYPE_UCS2      CONVERT_TEXT_STRING_2_UCS2("Toggle Temp. Type")
#define TEXT_GENERIC_SRV_APP_DEACTIVATE_UCS2            CONVERT_TEXT_STRING_2_UCS2("Deactivate")
#define TEXT_GENERIC_SRV_APP_OK_UCS2                    CONVERT_TEXT_STRING_2_UCS2("OK")
#define TEXT_GENERIC_SRV_APP_UNREGISTER_UCS2            CONVERT_TEXT_STRING_2_UCS2("Unregister")
#define TEXT_GENERIC_SRV_APP_REGISTER_UCS2              CONVERT_TEXT_STRING_2_UCS2("Register")
#define TEXT_GENERIC_SRV_APP_INDICATE_UCS2            CONVERT_TEXT_STRING_2_UCS2("Indicate Client")
#define TEXT_GENERIC_SRV_APP_NOTIFY_UCS2                CONVERT_TEXT_STRING_2_UCS2("Notify Client")

#endif /* CSR_BT_GENERICSRV_STRING_H__ */


