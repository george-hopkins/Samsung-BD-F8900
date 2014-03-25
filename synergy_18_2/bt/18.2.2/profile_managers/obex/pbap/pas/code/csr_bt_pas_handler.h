#ifndef CSR_BT_PAS_HANDLER_H__
#define CSR_BT_PAS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_pas_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PAS_IDLE_STATE                      (0)
#define PAS_REGISTERED_STATE                (1)
#define PAS_ACTIVATION_PENDING_STATE        (2)
#define PAS_ACTIVE_STATE                    (3)
#define PAS_CONNECTION_PENDING_STATE        (4)
#define PAS_CONNECTED_STATE                 (5)
#define PAS_DEACTIVATION_PENDING_STATE      (6)
#define PAS_NUMBER_OF_STATES                (7)

typedef CsrUint32                           PasFolderId;
#define PAS_ROOT_ID                         (1)
#define PAS_TELECOM_ID                      (2)
#define PAS_PB_ID                           (3)
#define PAS_ICH_ID                          (4)
#define PAS_OCH_ID                          (5)
#define PAS_MCH_ID                          (6)
#define PAS_CCH_ID                          (7)

struct _PasFolder
{
    PasFolderId          id;
    CsrUint8            *pwzName;
    struct _PasFolder   *pParent;
    struct _PasFolder   *pChild;
    struct _PasFolder   *pNext;
};
typedef struct _PasFolder PasFolder;

typedef struct
{
    CsrUint8             localServerCh;
    CsrUint16            maxObexPacketSize;
    CsrUint16            windowSize;
    CsrBool              localSrmEnable;
    void                *obexInst;
    CsrSchedQid          appHandle;
    CsrUint8             nSdpFeature;
    CsrUint8             nSdpRepository;
    CsrUint8             obexCommand;
    PasFolder           *pFolder; /* Virtual folder use check the folder set using SetPath */
    dm_security_level_t  secIncoming; 
} PasInst;


typedef CsrUint8 (* PasStateHandler)(PasInst *pInst, void *msg);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PAS_HANDLER_H__ */

