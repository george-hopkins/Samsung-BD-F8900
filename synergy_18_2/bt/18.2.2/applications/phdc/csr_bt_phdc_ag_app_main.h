#ifndef CSR_BT_PHDC_AG_APP_MAIN_H__
#define CSR_BT_PHDC_AG_APP_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

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

#define PROFILE_PHDC_AGENT CsrUtf8StrDup("PHDC AGENT")

#define   NUMBER_OF_CSR_BT_PHDC_AG_END_POINT        (2)
#define   PHDC_APP_AG_END_POINT_BPM                 (0)
#define   PHDC_APP_AG_END_POINT_WS                  (1)

#define   PHDC_APP_AG_UI_MAIN_MENU_OFFSET           (NUMBER_OF_CSR_BT_PHDC_AG_END_POINT*2)

/* first default box indices are for the device pop-ups and sub-menus */

#define   PHDC_APP_AG_UI_MAIN_MENU                  (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+0)
#define   PHDC_APP_AG_UI_DEFAULT_POP_UP             (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+1)
#define   PHDC_APP_AG_UI_ACTIVE_DEVICE_MENU         (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+2)
#define   PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU      (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+3)
#define   PHDC_APP_AG_UI_DEVICE_CAPAB_MENU          (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+4)
#define   PHDC_APP_AG_UI_OPERATING_MENU             (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+5)
#define   PHDC_APP_AG_UI_INPUT_DIALOG_MENU          (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+6)
#define   PHDC_APP_AG_UI_MENU_RESUME                (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+7)
#define   NUMBER_OF_CSR_BT_PHDC_APP_AG_UI           (PHDC_APP_AG_UI_MAIN_MENU_OFFSET+8)

#define PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS           (0)
#define PHDC_APP_AG_MAIN_MENU_ACTIVATE_BPM          (1)
#define PHDC_APP_AG_MAIN_GET_CAPAB                  (2)
#define PHDC_APP_AG_MAIN_OPERATION                  (3)

#define PHDC_APP_AG_OPERATING_MENU_SEND_DATA        (0)
#define PHDC_APP_AG_OPERATING_MENU_DISASSOCIATE     (1)
#define PHDC_APP_AG_OPERATING_MENU_SUSPEND          (2)

#define PHDC_APP_AG_ACTIVE_DEVICE_SUB_MENU_RCV_DATA     (0)
#define PHDC_APP_AG_ACTIVE_DEVICE_SUB_MENU_DISCONNECT   (1)
#define PHDC_APP_AG_ACTIVE_DEVICE_SUB_MENU_STATUS       (2)

#define PHDC_APP_AG_RESUME_MENU_RESUME            (0)

#define PHDC_APP_AG_DEFAULT_POP_UP_INVALID              (0xFF)
#define PHDC_APP_AG_DEFAULT_POP_UP_ACTIVATE             (0)
#define PHDC_APP_AG_DEFAULT_POP_UP_ACCEPT_BT_LINK       (1)
#define PHDC_APP_AG_DEFAULT_POP_UP_NORMAL               (4)
#define PHDC_APP_AG_DEFAULT_POP_UP_OPERATING            (5)
#define PHDC_APP_AG_DEFAULT_POP_UP_MAIN_MENU            (6)
#define PHDC_APP_AG_DEFAULT_POP_UP_RESUME_MENU          (7)
#define PHDC_APP_AG_DEFAULT_POP_UP_DEVICE_CAPAB_MENU    (8)
#define PHDC_APP_AG_DEFAULT_POP_UP_SETUP_BT_LINK        (9)
#define PHDC_APP_AG_DEFAULT_POP_UP_BT_LINK_REL          (10)

#define PHDC_APP_AG_UI_HEADING_1                    "Phdc Agent"
#define PHDC_APP_AG_HEADING_1_ACTIVE_DEVICES        "Phdc Agent : Active Devices"

#define PHDC_APP_AG_EP_ST_ACTIVE                    (0)
#define PHDC_APP_AG_EP_ST_SUSPENDED                 (1)
#define PHDC_APP_AG_EP_ST_DISASSOCIATED             (2)

#define  PHDC_APP_AG_NO_OF_OBS_SCAN                 (22)
#define  PHDC_APP_AG_OBS_SCAN_LEN                   (50)

typedef void (* CsrUiEventHandlerFuncType)(void * pInstance, CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key);

typedef struct
{
    CsrUieHandle                               displayHandle;
    CsrUieHandle                               sk1EventHandle;
    CsrUieHandle                               sk2EventHandle;
    CsrUieHandle                               backEventHandle;
    CsrUieHandle                               deleteEventHandle;
    CsrUiEventHandlerFuncType                sk1EventHandleFunc;
    CsrUiEventHandlerFuncType                sk2EventHandleFunc;
    CsrUiEventHandlerFuncType                deleteEventHandleFunc;
    CsrUiEventHandlerFuncType                backEventHandleFunc;
} PhdcAppAgDisplayHandle;


typedef struct
{
    BD_ADDR_T               bd;
    BD_ADDR_T               prevBd;
    CsrUint32               dataChannelId;
    CsrBtMdepId             mdepId;
    CsrUint16               mdepDataType;
    CsrCharString           *mdepDesc;
    CsrUint8                state;
    CsrBool                 busy;
    CsrUint16               maxPktLen;
    CsrCharString           pSystemId[8];
    PhdcAppAgDisplayHandle  *popUpHdl;
    PhdcAppAgDisplayHandle  *subMenuHdl;
    CsrUint8                obsScanIdx;
    CsrCharString           obsScan[PHDC_APP_AG_NO_OF_OBS_SCAN][PHDC_APP_AG_OBS_SCAN_LEN];
} PhdcAppAgEndPoint;

typedef struct{
    CsrUint32         psmIdentifier;
    CsrBtMdepId       mdepId;
    CsrUint16         mdepDataType;
}PhdcAppAgDeviceCapab;

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
    PhdcAppAgDisplayHandle  displayHandlers[NUMBER_OF_CSR_BT_PHDC_APP_AG_UI];
    PhdcAppAgEndPoint       endPoint[NUMBER_OF_CSR_BT_PHDC_AG_END_POINT];

    CsrUint8                popUpInput;
    CsrCharString           *pScratchString; /* used to form strings for Dialog boxes */
    PhdcAppAgDeviceCapab    deviceCapab[2];
    CsrUint8                numCapab;
    CsrUint32               dataChannelId;
    CsrBtDeviceAddr         deviceAddr;  /* rcvd in CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */
    CsrUint8                endpointActivated;
} PhdcAppAgInstance;

typedef void (* CsrBtPhdcAgEventHandler)(PhdcAppAgInstance *pInstance);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PHDC_AG_APP_MGR_H__ */

