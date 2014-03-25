#ifndef CSR_BT_HIDH_DEMO_APP_H__
#define CSR_BT_HIDH_DEMO_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_sc_demo_app.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The time to wait for bonding and connection in number of seconds */
#define MAX_BONDING_TIME                 (60)
#define KEYB_CHECK_TIMEOUT               (100000)
#define MAX_DISCOVERY_RESULTS_IN_LIST    (10)

#define CSR_BT_UNDEFINED                   (0xFF)

#define NULL_ACTION                      (0)
#define GET_PROTOCOL_ACTION              (1)
#define GET_IDLE_ACTION                  (2)

/* Demo app states */
typedef CsrUint8 hidhStates_t;
#define HIDH_STATE_IDLE (hidhStates_t) 0x00
#define HIDH_STATE_SEARCH (hidhStates_t) 0x01
#define HIDH_STATE_SELECT_DEVICE (hidhStates_t) 0x02
#define HIDH_STATE_PAIRING (hidhStates_t) 0x03
#define HIDH_STATE_CONNECTED (hidhStates_t) 0x04
#define HIDH_STATE_AWAIT_CONNECT (hidhStates_t) 0x05
#define HIDH_STATE_UNPLUG (hidhStates_t) 0x06
#define HIDH_STATE_RECONNECTING (hidhStates_t) 0x07
#define HIDH_STATE_AWAIT_USER_REG (hidhStates_t) 0x08
#define HIDH_NOF_STATES (hidhStates_t) 0x09

/* instance data */
typedef struct
{
    hidhStates_t           state;
    hidhStates_t           prevState;
    void                   *recvMsgP;
    CsrUint8                selectedDevice;
    CsrUint8                numInList;
    CsrBool                 isSearch;
    CsrBool                 nonZeroIdleRate;
    CsrBool                 isEncrypted;
    CsrUint8                action;
    CsrBtHidhProtocol        protocol;
    struct CsrBtHidDevice     *device;
    CsrBtDeviceAddr           bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];
    CsrSchedQid              ctrl_app_hdl;
    CsrBool                 showPtsExtras;
    CsrBool                 scMsgPending;
    ScAppInstData_t        scInstData;
    CsrBtUuid32            hidServiceRecordHandle;
    CsrUint16               sdpAttrib;
} hidhInstData_t;

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HIDH_DEMO_APP_H__ */
