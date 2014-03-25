#ifndef CSR_BT_BIPS_HANDLER_H__
#define CSR_BT_BIPS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_bips_prim.h"
#include "csr_bt_obex_util.h"
#include "csr_bt_cmn_sdp_lib.h"

#define CSR_BT_BIPS_INSTANCES_POOL_SIZE     0x02
#define CSR_BT_BIPS_INVALID_CONNECT_TYPE    0xFF

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID_LENGTH      0x10
#define CSR_BT_BIPS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX  0x18

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define CSR_BT_BIPS_L2CAP_PSM_SDP_RECORD_INDEX                  0x3B
#define CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX         0x46
#define CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX             0x4C
#define CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX               0x54
#else
#define CSR_BT_BIPS_L2CAP_PSM_SDP_RECORD_INDEX                  CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
#define CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX         0x40
#define CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX             0x46
#define CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX               0x4E
#endif
#define CSR_BT_NUMBER_OF_BASIC_IMAGING_SESSION                  0x03
#define CSR_BT_BIPS_TARGET_LENGTH                               0x10
#define CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID             0x09
#define CSR_BT_BIPS_APPLICATION_PARAMETER_STOREFLAG             0x0A
#define CSR_BT_BIPS_APPLICATION_PARAMETER_NBRETURNEDHANDLES     0x01
#define CSR_BT_BIPS_RC_GET_MONITORING_BODY_HEADER_INDEX         0x01
#define CSR_BT_BIPS_RC_GET_IMAGE_BODY_HEADER_INDEX              0x01
#define CSR_BT_BIPS_PRISERV_IDLE_STATE                          0x00
#define CSR_BT_BIPS_PRISERV_ACTIVATED_STATE                     0x01
#define CSR_BT_BIPS_PRISERV_OBEX_CONNECTED_STATE                0x02
#define CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE            0x03
#define CSR_BT_BIPS_HEADER_SIZE                                 0x10

typedef struct BipsInstancesPool
{
    CsrUint8                    numberInPool;
    CsrSchedQid                      phandles[CSR_BT_BIPS_INSTANCES_POOL_SIZE];
    struct BipsInstancesPool    *next;
} BipsInstancesPool;


typedef struct
{
    CsrUint32                   lengthOfObject;
    CsrUint16                   typeOffset;
    CsrUint16                   nameOffset;
    CsrUint16                   imgDescriptorOffset;
    CsrUint16                   imgDescriptorLength;
    CsrUint16                   imgHandleOffset;
    CsrUint16                   bodyOffset;
    CsrUint16                   bodyLength;
    CsrUint16                   obexPacketLength;
    CsrUint16                   obexResponsePacketLength;
    CsrBool                     serviceIdFound;
    CsrUint16                   userDefinedHeaderLength;
    CsrUint8                    *userDefinedHeader;
    CsrUint8                  *obexPacket;
    CsrUint8                  *cloneObexPacket;
} BipsCollectedInfo;

typedef struct
{
    CsrUint8                  *aaObexPacket;
    CsrUint16                   aaObexPacketLength;
    CsrBtObexResponseCode       responseCode;
    CsrUint16                   bodyLength;
    CsrUint16                   bodyOffset;
} BipsAaCollectedInfo;

typedef struct
{
    CsrUint32                   upperDataCapacity;
    CsrUint32                   lowerDataCapacity;
    CsrBool                     localSrmEnable;
    CsrUint16                   windowSize;
    CsrUint16                   maxLocalObexPacketSize;
    CsrUint16                   maxPeerObexPacketSize;
    CsrUint8                    featureSelection;
    CsrSchedQid                 appHandle;
    CsrSchedQid                 myAppHandle;
    dm_security_level_t         secIncoming;
    CsrUint8                    localServerCh;
    psm_t                       localPsm;
    CsrUint8                    connectType;
    CsrUint8                    serviceId[CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID_LENGTH];
    CsrBool                     finalFlag;
    CsrBtDeviceAddr             deviceAddr;
    CsrBool                     clientConnected;
    CsrBool                     primaryPending;
    CsrBool                     aAFoundAllHeaders;
    CsrUint16                   nbReturnedHandles;
    CsrUint8                    priServState;
    CsrBtObexResponseCode       getStatusResponseCode;
    CsrBool                     authenticated;
    CsrBool                     digestChallenge;
    CsrUint32                   length;
    CsrUint32                   count;
    void                        *obexCliInst;
    void                        *obexServInst;
    CsrUint8                    numberOfBipsInstances;
    BipsInstancesPool           *bipsInstances;
    BipsCollectedInfo           *collectedInfo;
    BipsAaCollectedInfo         *aACollectedInfo;
    CsrBtConnId                 btConnId;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId                 masterBtConnId;
    CsrBtConnId                 slaveBtConnId;
#endif /* CSR_AMP_ENABLE */
} BipsInstanceData;

typedef CsrUint8 (*BipsStateHandlerType)(BipsInstanceData *pInst, void *msg);

#ifdef __cplusplus
}
#endif

#endif
