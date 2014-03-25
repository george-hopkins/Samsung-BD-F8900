#ifndef CSR_BT_MAPS_HANDLER_H__
#define CSR_BT_MAPS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_maps_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Client state defines */
#define CSR_BT_MAPS_CLI_NULL_STATE          0x00
#define CSR_BT_MAPS_CLI_IDLE_STATE          0x01
#define CSR_BT_MAPS_CLI_PENDING_APP_STATE   0x02
#define CSR_BT_MAPS_CLI_PENDING_PEER_STATE  0x03


#define CSR_BT_MAPS_CLIENT_INST_ID      0x01
#define CSR_BT_MAPS_SERVER_INST_ID      0x02

/* Get defines */
#define TYPE_NONE                       0x00
#define TYPE_GET_FOLDER_LISTING         0x01
#define TYPE_GET_MESSAGE_LISTING        0x02
#define TYPE_GET_MESSAGE                0x03
#define TYPE_SET_MESSAGE_STATUS         0x04
#define TYPE_UPDATE_INBOX               0x05
#define TYPE_NOTIFICATION_REGISTRATION  0x06

#define APP_NONE                        0x00
#define APP_FOUND                       0x01

#define NAME_NONE                       0x00
#define NAME_FOUND                      0x01

/* Number of MAPS instances that is allocated in one pool. */
#define MAPS_INSTANCES_POOL_SIZE         (10)

#define CSR_BT_MAPS_DEFAULT_MAX_LIST_COUNT  1024

typedef struct CsrBtMapsInstancePool
{
    CsrUint8                        numberInPool;
    CsrSchedQid                          phandles[MAPS_INSTANCES_POOL_SIZE];
    struct CsrBtMapsInstancePool    *next;
} CsrBtMapsInstancePool;

typedef struct
{
    CsrUint16     serviceIdx;
    CsrUint8      masInstanceId;
} CsrBtMapsMasInstIdMap;

typedef struct
{
    CsrBtMapsInstancePool  *mapsInstances;
    CsrSchedQid             mapsInstanceId;
    CsrUint8                numberOfMapsInstances;
    CsrUint16               maxObexPacketSize;
    CsrUint16               windowSize;
    CsrBool                 localSrmEnable;


    /* OBEX Client part of MAPS Server */
    void                   *obexClientInst;
    CsrBtMapsMasInstIdMap  *masInstToServiceMap;
    CsrUint16               masInstToServiceMapLength;
    dm_security_level_t     secInLevel;
    dm_security_level_t     secOutLevel;
    CsrBtMapMesSupport      supportedMessages;
    CsrBool                 srvConnected;
    CsrUint8                cliState;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId             slaveBtConnId;
#endif /* CSR_AMP_ENABLE */

    /* OBEX Server part of MAPS Server */
    void                   *obexServerInst;
    CsrSchedQid             appHandle;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                serverChannel;
    CsrUint16               maxObexResponseLength;
    CsrUint8               *responseAppHeader;
    CsrUint16               responseAppHeaderLength;
    CsrBool                 finalBitFromPutInd;
    CsrUint8               *bodyFromPutInd;
    CsrUint16               bodyFromPutIndLength;
    CsrUint8                nameHeader;
    CsrUint8                typeHeader;
    CsrUint8                appHeader;
    CsrUcs2String          *pNameHeader;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId             masterBtConnId;
#endif /* CSR_AMP_ENABLE */
    CsrUint8                masInstId;
    CsrUint16               masInstNameLen;
    CsrUint8                *masInstName;

    /* Application tag values */
    CsrUint16               maxListCount;
    CsrUint16               listStartOffset;
    CsrUint8                filterMessageType;
    CsrUtf8String          *filterPeriodBegin;
    CsrUtf8String          *filterPeriodEnd;
    CsrUint8                filterReadStatus;
    CsrUtf8String          *filterRecipient;
    CsrUtf8String          *filterOriginator;
    CsrUint8                filterPriority;
    CsrUint8                attachment;
    CsrUint8                transparent;
    CsrUint8                retry;
    CsrUint8                newMessage;
    CsrUint8                notificationStatus;
    CsrUint8                masInstanceId;
    CsrUint32               parameterMask;
    CsrUint16               folderListingSize;
    CsrUint16               messageListingSize;
    CsrUint8                subjectLength;
    CsrUint8                charset;
    CsrUint8                fractionRequest;
    CsrUint8                fractionDeliver;
    CsrUint8                statusIndicator;
    CsrUint8                statusValue;
    CsrUtf8String          *mseTime;    

} MapsInstanceData;

typedef CsrUint8 (* MapsStateHandlerType)(MapsInstanceData * mapsInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MAPS_HANDLER_H__ */

