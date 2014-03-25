/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#ifndef _WIN32_WCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_ui_lib.h"
#include "csr_app_lib.h"
#include "csr_bt_gap_app_handler.h"
#include "csr_bt_gap_app_cm_event_handler.h"
#include "csr_util.h"

#define PROFILE_CM CsrUtf8StrDup("CM")

static void csrBtCmReadLocalDeviceAddressCfmFunc(
    void                    *instData,              /* the applications own instance data                                           */
    CsrBtDeviceAddr            localDeviceAddr      /* the local Bluetooth device address                      */
    )
{
    char buf[33];
    CsrBtGapInstData *inst = instData;

    inst->cmVar.localDeviceAddr = localDeviceAddr;
#ifndef _WIN32_WCE
    sprintf(buf,"Local DeviceAddr: %04X:%02X:%06X", localDeviceAddr.nap, localDeviceAddr.uap, localDeviceAddr.lap);
#endif
    CsrAppBacklogReqSend(TECH_BT, PROFILE_CM, FALSE, buf);
}

static void csrBtCmReadLocalNameCfmFunc(
    void                    *instData,              /* the applications own instance data                                           */
    CsrUtf8String           *localName              /* the friendly name of the local Bluetooth device in utf8                      */
                                        )
{
    /*
    This message is send to the tasks specified in the parameter "phandle"
    in the CSR_BT_CM_READ_LOCAL_NAME_REQ message.
    This is the confimation of read local name procedure (initiated by a CSR_BT_CM_READ_LOCAL_NAME_REQ).
    */
    CsrBtGapInstData *inst = instData;

    CsrUtf8StrCpy(inst->cmVar.localName, localName);

    CsrAppBacklogReqSend(TECH_BT, PROFILE_CM, FALSE, (CsrCharString *) localName);
    CsrPmemFree(localName);
}

static void csrBtCmWriteCodCfmFunc(
    void                    *instData,              /* the applications own instance data                                           */
    CsrBtResultCode         resultCode,
    CsrBtSupplier           resultSupplier)
{
    CsrAppBacklogReqSend(TECH_BT, PROFILE_CM, FALSE, "COD set to Mobile Phone");
}

static void csrBtCmReadLocalVersionCfmFunc(
    void                    *instData,              /* the applications own instance data                                           */
    CsrUint8                 lmpVersion             /* the local lmpVersion                      */
    )
{
    char buf[19];
    CsrBtGapInstData *inst = instData;

    inst->cmVar.lmpVersion = lmpVersion;
#ifndef _WIN32_WCE
    sprintf(buf,"Local Version: %03d", lmpVersion);
#endif
    CsrAppBacklogReqSend(TECH_BT, PROFILE_CM, FALSE, buf);

    CsrUiStatusBluetoothSetReqSend(TRUE);
}



/*
Setup of the functions.
The table below shall be configured for each application that can receive
messages from the CSR_BT CM.
If some of the messages will never occur in the specific application the function reference
can be set to "NULL" in the table.
*/
static const struct CsrBtCmAppEventHandlerStructType csrBtCmAppEventFunctions =
{
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM            */
        csrBtCmReadLocalDeviceAddressCfmFunc,   /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM        */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        csrBtCmReadLocalNameCfmFunc,            /* CSR_BT_CM_READ_LOCAL_NAME_CFM           */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM       */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM          */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM                 */
        csrBtCmWriteCodCfmFunc,                 /* CSR_BT_CM_WRITE_COD_CFM                 */
        NULL,                                   /* CSR_BT_CM_READ_COD_CFM                  */
        csrBtCmReadLocalVersionCfmFunc,         /* CSR_BT_CM_READ_LOCAL_VERSION_CFM        */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM       */
        NULL,                                   /* CSR_BT_CM_READ_SCAN_ENABLE_CFM          */
        NULL,                                   /* CSR_BT_CM_WRITE_SCAN_ENABLE_CFM         */
};

/* setup the global functional pointer that shall be overloaded to the event function */
const CsrBtCmAppEventHandlerStructType * const CsrBtGapCsrBtCmAppEventFunctionsPtr = &csrBtCmAppEventFunctions;


