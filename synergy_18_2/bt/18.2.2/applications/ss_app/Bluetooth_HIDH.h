#ifndef BLUETOOTH_HIDH_H__
#define BLUETOOTH_HIDH_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
//#include "csr_bt_sc_demo_app.h"
#include "csr_bt_hidh_prim.h"

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



/* instance data */
typedef struct
{
	CsrUint16		qid;
#ifndef EXCLUDE_CSR_BT_HID_DRIVER
    CsrInt32   fd;
#endif
    CsrUint8           state;
//    hidhStates_t           prevState;
  //  void                   *recvMsgP;
    CsrUint8                selectedDevice;
 //   CsrUint8                numInList;
  //  CsrBool                 isSearch;
    CsrBool                 nonZeroIdleRate;
//    CsrBool                 isEncrypted;
    CsrUint8                action;
    CsrBtHidhProtocol        protocol;
//    struct CsrBtHidDevice     *device;
   // CsrBool                 showPtsExtras;
	CsrBtDeviceAddr          connectedAddr;
//    CsrBool                 scMsgPending;
    //ScAppInstData_t        scInstData;
    CsrBtUuid32            hidServiceRecordHandle;
    CsrUint16               sdpAttrib;
} HidhAppInst;
void init_hidh(void **gash);
void deinit_hidh(void * *gash);
void handleHidhPrim(HidhAppInst *instData, CsrPrim    *primType);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HIDH_DEMO_APP_H__ */
