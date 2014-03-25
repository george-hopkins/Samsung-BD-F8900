#ifndef CSR_BT_PHDC_MGR_APP_MAIN_H__
#define CSR_BT_PHDC_MGR_APP_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_util.h"
#include "csr_ui_prim.h"
#include "bluetooth.h"
#include "csr_bt_hdp_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PROFILE_PHDC_MGR CsrUtf8StrDup("PHDC MANAGER")

#define   PHDC_APP_END_POINT_BPM                      (0)
#define   PHDC_APP_END_POINT_WS                       (1)
#define   NUMBER_OF_CSR_BT_PHDC_END_POINT             (2)

#define   PHDC_APP_MGR_UI_MAIN_MENU_OFFSET            (NUMBER_OF_CSR_BT_PHDC_END_POINT*2)

#define PHDC_APP_POPUP_TIMEOUT          3000000 /* 3 seconds before popup dialogs are hidden */ 

/* first default box indices are for the device pop-ups and sub-menus
     i.e  the resrources begin from PHDC_APP_MGR_UI_MAIN_MENU_OFFSET */

#define   PHDC_APP_MGR_UI_MAIN_MENU             (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+0)
#define   PHDC_APP_MGR_UI_DEFAULT_POP_UP        (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+1)
#define   PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU    (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+2)
#define   PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU     (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+3)
#define   PHDC_APP_MGR_UI_CONNECT_AGENT_MENU     (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+4)

/* Add plus to the last resource to get the total number */
#define   NUMBER_OF_CSR_BT_PHDC_APP_UI          (PHDC_APP_MGR_UI_MAIN_MENU_OFFSET+5)

#define PHDC_APP_MGR_MAIN_MENU_ACTIVATE            (0)
#define PHDC_APP_MGR_MAIN_MENU_DEVICES             (1)
#define PHDC_APP_MGR_MAIN_MENU_CAPAB               (2)
#define PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT       (3)
#define PHDC_APP_MGR_MAIN_MENU_PROXIMITY           (4)

#define PHDC_APP_MGR_MAIN_MENU_ACTIVATE_TITLE      CONVERT_TEXT_STRING_2_UCS2("Activate Service")
#define PHDC_APP_MGR_MAIN_MENU_DEVICES_TITLE       CONVERT_TEXT_STRING_2_UCS2("Devices")
#define PHDC_APP_MGR_MAIN_MENU_CAPAB_TITLE         CONVERT_TEXT_STRING_2_UCS2("Get Capab")
#define PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT_TITLE CONVERT_TEXT_STRING_2_UCS2("Connect Agent")
#define PHDC_APP_MGR_MAIN_MENU_PROXIMITY_TITLE     CONVERT_TEXT_STRING_2_UCS2("Use Proximity")

#define PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_RCV_DATA             (0)
#define PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_STATUS               (1)
#define PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_DISCONNECT           (2)
#define PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_RESUME               (3)

#define PHDC_APP_MGR_DEFAULT_POP_UP_INVALID         (0xFF)
#define PHDC_APP_MGR_DEFAULT_POP_UP_ACTIVATE        (0)
#define PHDC_APP_MGR_DEFAULT_POP_UP_ACCEPT_BT_LINK  (1)
#define PHDC_APP_MGR_DEFAULT_POP_UP_ASSOC_COMPLETE  (2)
#define PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL          (3)

#define PHDC_APP_MGR_UI_HEADING_1               "Phdc Mgr"
#define PHDC_APP_MGR_HEADING_1_ACTIVE_DEVICES   "Phdc Mgr : Active Devices"
#define PHDC_APP_MGR_HEADING_1_GET_CAPAB        "Phdc Mgr : Get Capab"

#define PHDC_APP_MGR_EP_ST_DISCONNECTED              (0)
#define PHDC_APP_MGR_EP_ST_DISASSOCIATED             (1)
#define PHDC_APP_MGR_EP_ST_ACTIVE                    (2)

#define  PHDC_APP_MGR_NO_OF_OBS_SCAN                 (22)
#define  PHDC_APP_MGR_OBS_SCAN_LEN                   (100)

#define NUMBER_OF_SERVICE_RECORDS  (2)
#define NUMBER_OF_MDEP             (10)


typedef void (* CsrUiEventHandlerFuncType)(void * pInstance, CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key);

typedef struct
{
    CsrUieHandle                displayHandle;
    CsrUieHandle                sk1EventHandle;
    CsrUieHandle                sk2EventHandle;
    CsrUieHandle                backEventHandle;
    CsrUieHandle                deleteEventHandle;
    CsrUiEventHandlerFuncType   sk1EventHandleFunc;
    CsrUiEventHandlerFuncType   sk2EventHandleFunc;
    CsrUiEventHandlerFuncType   deleteEventHandleFunc;
    CsrUiEventHandlerFuncType   backEventHandleFunc;
} PhdcAppMgrDisplayHandle;

typedef struct
{
    BD_ADDR_T                   bd;
    BD_ADDR_T                   prevBd;
    CsrUint32                   dataChannelId;
    CsrBtMdepId                 mdepId;
    CsrUint16                   mdepDataType;
    CsrUtf8String               *mdepDesc;
    CsrUint8                    state;
    CsrBool                     busy;
    CsrUint16                   maxPktLen;
    CsrCharString               pSystemId[8];
    PhdcAppMgrDisplayHandle     *popUpHdl;
    PhdcAppMgrDisplayHandle     *subMenuHdl;
    CsrUint8                    obsScanIdx;
    CsrCharString               obsScan[PHDC_APP_MGR_NO_OF_OBS_SCAN][PHDC_APP_MGR_OBS_SCAN_LEN];
} PhdcAppMgrEndPoint;

typedef struct
{
    CsrBtMdepId       mdepId;
    CsrUint16         mdepDataType;
    CsrUint16         mdepRole;
    CsrUint8          menuIndex;
}PhdcAppMgrMdepInfo;


typedef struct
{
    CsrUint32          psmIdentifier;
    PhdcAppMgrMdepInfo mdep[NUMBER_OF_MDEP];
    CsrUint8           count;
}PhdcAppDeviceCapab;


/* Application instance data */
typedef struct
{
    void                    *recvMsgP;
    CsrUint16               eventType;

    CsrCharString           *pService;
    CsrCharString           *pProvider;

    CsrUint16               controlId;

    CsrUint8                uiIndex;
    CsrBool                 bRegistered;
    CsrUint8                noActiveDevices;
    PhdcAppMgrDisplayHandle displayHandlers[NUMBER_OF_CSR_BT_PHDC_APP_UI];
    PhdcAppMgrEndPoint      endPoint[NUMBER_OF_CSR_BT_PHDC_END_POINT];

    CsrUint8                popUpInput;
    CsrCharString           *pScratchString; /* used to form strings for Dialog boxes */

    PhdcAppDeviceCapab      getCapab[NUMBER_OF_SERVICE_RECORDS];     /* At most a device may have 2 service records */
    CsrUint16               getCapabIndex;
    CsrUint16               selectedEndpoint;
    CsrBool                 proximityInUse;
    CsrSchedTid             timerId;

} PhdcAppMgrInstance;

typedef void (* CsrBtPhdcEventHandler)(PhdcAppMgrInstance *pInstance);










#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PHDC_MGR_APP_MAIN_H__ */

