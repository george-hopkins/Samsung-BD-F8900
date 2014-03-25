#ifndef CSR_BT_BIPC_HANDLER_H__
#define CSR_BT_BIPC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_bipc_prim.h"
#include "csr_bt_obex_util.h"
#include "csr_bt_cmn_sdp_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* States for the primary client    */
#define BIPC_CLIENT_IDLE_STATE                  0x00
#define BIPC_CLIENT_SDP_SEARCH_STATE            0x01
#define BIPC_CLIENT_OBEX_CONNECTING_STATE       0x02
#define BIPC_CLIENT_OBEX_CONNECTED_STATE        0x03
#define BIPC_CLIENT_OBEX_RELEASE_STATE          0x04

/* States for the secondary server  */
#define BIPC_SERVER_IDLE_STATE                  0x00
#define BIPC_SERVER_ACTIVATING_STATE            0x01
#define BIPC_SERVER_ACTIVATED_STATE             0x02
#define BIPC_SERVER_DEACTIVATING_STATE          0x03

typedef struct
{
    void                   *obexCliInst;
    void                   *obexServInst;
    void                   *sdpSearchData;
    CsrSchedQid             appHandle;
    CsrUint8               feature;
    dm_security_level_t    secOutgoing;
    CsrUint8               dataCapacity[CSR_BT_DATA_CAPACITY_LEN];
    CsrUint32              supportedFunctions;
    CsrUint16              maxPeerObexPacketLength;
    CsrUint16              originalMaxPacketSize;
    CsrUint16              windowSize;
    CsrBool                localSrmEnable;
    CsrUint8               servState;
    CsrBtDeviceAddr        deviceAddr;
    CsrUint8               servServerChannel;
    CsrSchedQid            myAppHandle;
    CsrUint8               imgHandle[CSR_BT_IMG_HANDLE_LENGTH + 1];
    CsrBool                headerCfm;
    CsrUint8               *clientMiscInfo;
    CsrUint16              clientMiscInfoLength;
    CsrBtObexResponseCode  clientResponseCode;
    CsrBool                clientAuthRspPending;
    CsrUint8               clientState;
    CsrUint16              servObexPacketRspLength;
    CsrUint32              servLengthOfObject;
    CsrUint8               *servMiscInfo;
    CsrUint16              servMiscInfoLength;
    CsrBool                connectCfmSent;
    CsrBool                storeFlag;
    CsrBtConnId            btConnId;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId            masterBtConnId;
    CsrBtConnId            slaveBtConnId;
#endif /* CSR_AMP_ENABLE */
} BipcInstanceData;

typedef CsrUint8 (* BipcStateHandlerType)(BipcInstanceData * pInst, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BIPC_HANDLER_H__ */
