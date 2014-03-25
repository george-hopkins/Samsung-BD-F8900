#ifndef CSR_BT_AMPWIFI_H__
#define CSR_BT_AMPWIFI_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_AMPWIFI_PRIORITY                 (1)
#define CSR_BT_AMPWIFI_KEY_INVALID              (0x00)
#define CSR_BT_AMPWIFI_KEY_TOGGLE_WIFI_ONOFF    (0x01)
#define CSR_BT_AMPWIFI_KEY_TOGGLE_COEX          (0x02)

#define CSR_BT_AMPWIFI_TOGGLE_WIFI_STRING       "Toggle WiFi on/off"
#define CSR_BT_AMPWIFI_TOGGLE_COEX_STRING       "Toggle WiFi co-existence"
#define CSR_BT_AMPWIFI_MAIN_ENTRY_NAME          "AMP Wifi support"
#define CSR_BT_AMPWIFI_MODULE_NAME              "CSR_BT_AMPWIFI_CLIENT_APP"

typedef struct
{
    CsrSchedQid         qid;
    CsrUint16           msgClass;
    CsrUint16           msgType;
    void               *msgData;

    CsrBool             wifiEnabled;
    CsrBool             moveInitiatedWifiOn;
    CsrBool             coexEnable;

    CsrUieHandle        uiMainMenu;
    CsrUieHandle        uiBackEvent;
    CsrUieHandle        uiSk1Event;
    CsrUieHandle        uiSk2Event;

} CsrBtAmpWifiInst;

#ifdef __cplusplus
}
#endif

#endif
