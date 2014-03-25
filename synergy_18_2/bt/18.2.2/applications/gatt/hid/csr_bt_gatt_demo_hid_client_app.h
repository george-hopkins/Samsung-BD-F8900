#ifndef CSR_BT_HIDC_APP_H__
#define CSR_BT_HIDC_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "csr_list.h"
#include "csr_types.h"
#include "csr_ui_lib.h"
#include "csr_bt_profiles.h"
#include "csr_message_queue.h"
#include "csr_file.h"
#include "csr_bt_ui_strings.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_platform.h"
#include "csr_bt_gatt_demo_db_utils.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_HIDC_APP_CSR_UI_PRI 1


#define CSR_BT_HIDC_APP_RSSI_TIMER_INTERVAL 1000000

typedef struct
{
    CsrUieHandle hMainMenu;
    CsrUieHandle hBackEvent;
    CsrUieHandle hSk1Event;
    CsrUieHandle hSk2Event;
    CsrUieHandle hInputSk1Event;
    CsrUieHandle hSelectComInstanceMenu;
    CsrUieHandle hInputDialog;
    CsrUieHandle hCommonDialog;
    CsrBool      localInitiatedAction;
} CsrBtHidcAppUiInstData;


/* CSR_BT_HIDC instance data */
typedef struct
{
    char                           *profileName;

    CsrSchedQid                     hidcAppHandle;
    CsrSchedQid                     hidcProfileHandle;
    CsrMessageQueueType            *saveQueue;
    CsrBool                         restoreFlag;
    CsrBool                         initialized;

    CsrSchedQid                     phandle;                  /* proximity client demo apps own phandle                   */
    CsrBtGattId                     gattId;                   /* gattId that this demo app identifies with                */

    CsrBtHidcAppUiInstData         csrUiVar;
    void                           *recvMsgP;
    CsrBtTypedAddr                  selectedDeviceAddr;
    CsrBtTypedAddr                  connectedDeviceAddr;
    CsrBtConnId                     btConnId;
    
    /*database values and handles (local copy) */
    CsrCmnList_t                    dbPrimServices;
    CsrUint16                       dbPrimServicesCount;
    CsrBtGattAppDbPrimServiceElement  *currentPrimService;  /* used for when we treverse the database during connect */
    CsrBtGattAppDbCharacElement       *currentCharac;  /* used for when we treverse the database during connect */
    CsrBool                         interuptNotif; 
    CsrBool                         reportNotif; 
    CsrBool                         traversingDb;
    CsrBool                         whiteListConnect;
    /* NOTE!! tmporary workaround to be able to work with devices without security implemented */
    CsrUint16                       interruptValueHandle;
} CsrBtHidcAppInstData;

void CsrBtHidcAppStartRegister(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartUnregister(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartConnecting(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartDisconnect(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartConnectWhitelist(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartAddWhitelist(CsrBtHidcAppInstData *hidcData, CsrBtTypedAddr addr);
void CsrBtHidcAppStartReadWhitelist(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartDeactivate(CsrBtHidcAppInstData *hidcData);
void CsrBtHidcAppStartReadUuid(CsrBtHidcAppInstData *hidcData, CsrUint16 uuid);
void CsrBtHidcAppStartReadValue(CsrBtHidcAppInstData *hidcData, CsrUint16 pUuid, CsrUint16 cUuid);
void CsrBtHidcAppStartWriteValue(CsrBtHidcAppInstData *hidcData, CsrUint16 pUuid, CsrUint16 cUuid);
void CsrBtHidcAppStartWriteClientConfig(CsrBtHidcAppInstData *hidcData, CsrBtUuid *pUuid, CsrBtUuid *cUuid, CsrUint16 value);
void CsrBtHidcAppStartReadCharacUuid(CsrBtHidcAppInstData *hidcData, CsrUint16 uuid);

CsrBtUuid *CsrBtHidcAppUuid16ToUuid128(CsrUint16 uuid);

#define PROFILE_NAME(name) CsrUtf8StrDup((CsrUtf8String *) name)

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_HIDC_APP_H__ */

