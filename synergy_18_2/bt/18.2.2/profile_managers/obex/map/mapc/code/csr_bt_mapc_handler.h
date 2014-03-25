#ifndef CSR_BT_MAPC_HANDLER_H__
#define CSR_BT_MAPC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_mapc_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_MAPC_CLIENT_INST_ID     0x01
#define CSR_BT_MAPC_SERVER_INST_ID     0x02

/* Number of MAPC instances that is allocated in one pool. */
#define MAPC_INSTANCES_POOL_SIZE         (10)

typedef struct CsrBtMapcInstancePool
{
    CsrUint8                        numberInPool;
    CsrSchedQid                     phandles[MAPC_INSTANCES_POOL_SIZE];
    struct CsrBtMapcInstancePool   *next;
} CsrBtMapcInstancePool;

typedef struct
{
    CsrUint16               serviceIdx;
    CsrUint8                masInstanceId;
} CsrBtMapcMasInstIdMap;

typedef struct
{
    CsrBtMapcInstancePool  *mapcInstances;
    CsrSchedQid             mapcInstanceId;
    CsrUint8                numberOfMapcInstances;
    CsrUint16               maxFrameSize;
    CsrUint16               windowSize;
    CsrBool                 localSrmEnable;

    /* OBEX Client part of MAPC Client */
    CsrSchedQid             appHandle;
    CsrBtMapcMasInstIdMap  *masInstToServiceMap;
    CsrUint16               masInstToServiceMapLength;
    CsrBool                 isMasConnected; /* Used to determine if we should send CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND */
    CsrBool                 notificationRegistrationOn;
    dm_security_level_t     secIncoming;
    dm_security_level_t     secOutgoing;
    void                   *obexClientInst;
    CsrUint16               fullSize;
    CsrUint16               partialSize;
    CsrUint8                newMessage;
    CsrBtMapFracDel         fractionDeliver;
    CsrUtf8String          *mseTime;
    CsrUint8                selectedMasInstanceId;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId             masterBtConnId;
#endif /* CSR_AMP_ENABLE */

    /* OBEX Server part of MAPC Client */
    void                   *obexServerInst;
    CsrBool                 isServerObexConnected;
    CsrUint8                serverChannel;
    CsrBool                 activated;
    CsrBool                 putFinalFlag;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId             slaveBtConnId;
#endif /* CSR_AMP_ENABLE */
} MapcInstanceData;

typedef CsrUint8 (* MapcStateHandlerType)(MapcInstanceData * mapcInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MAPC_HANDLER_H__ */

