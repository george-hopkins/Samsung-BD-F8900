#ifndef CSR_BT_AMPM_APP_H
#define CSR_BT_AMPM_APP_H

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CSR_BT_AMPM_APP_STATE_IDLE = 0,        /* Idle, nominal */
    CSR_BT_AMPM_APP_STATE_MOVING_INIT,     /* Moving as initiator */
    CSR_BT_AMPM_APP_STATE_MOVING_RESP,     /* Moving as responder */
    CSR_BT_AMPM_APP_STATE_MOVING_RESP_CMP  /* Moving as responder, waiting for complete */
} CsrBtAmpmAppState;

typedef struct
{
    CsrCmnListDataElm_t *next; /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t *prev; /* Do not move - used for common linked list handling */

    CsrSchedQid qid;
    CsrUint32 flags;
    CsrUint32 eventMask;
    CsrBtConnId btConnId;
    CsrBtDeviceAddr addr;
    CsrSchedTid moveIndTimer;
    CsrBtAmpmAppState state;
    CsrBtAmpController localId;
    CsrBool autoMove;
    CsrUint8 lastAmpsCount;
} CsrBtAmpmAppSubscriberElem;

typedef struct
{
    CsrCmnListDataElm_t *next; /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t *prev; /* Do not move - used for common linked list handling */

    CsrSchedQid qid;
    CsrBtConnId btConnId;
    CsrBtDeviceAddr addr;
} CsrBtAmpmAppDiscoveryElem;

typedef struct
{
    CsrCmnListDataElm_t *next; /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t *prev; /* Do not move - used for common linked list handling */

    CsrBtConnId master;
    CsrUint8 numSlaves;
    CsrBtConnId *slaves;
} CsrBtAmpmPrfElem;

/* Register application */
extern void CsrBtAmpmAppRegisterReq(CsrBtAmpmInstance *inst,
                                    CsrBtAmpmRegisterReq *req);

/* Unregister application */
extern void CsrBtAmpmAppDeregisterReq(CsrBtAmpmInstance *inst,
                                      CsrBtAmpmDeregisterReq *req);

/* Register application to handle power on indications */
extern void CsrBtAmpmAppRegisterPowerOnReq(CsrBtAmpmInstance *inst,
                             CsrBtAmpmRegisterPowerOnReq *req);

/* Respond to AMP power on indications */
void CsrBtAmpmAppPowerOnRes(CsrBtAmpmInstance *inst,
                             CsrBtAmpmPowerOnRes *res);

/* Explicit controller listing request from application */
extern void CsrBtAmpmAppControllerReq(CsrBtAmpmInstance *inst,
                                      CsrBtAmpmControllerReq *req);

/* Discovery complete callback for btdev */
extern void CsrBtAmpmAppCheckNotification(CsrBtAmpmInstance *inst);

/* Discovery failure callback for btdev */
extern void CsrBtAmpmAppFailNotification(CsrBtAmpmInstance *inst,
                                         CsrBtDeviceAddr *addr,
                                         CsrBtAmpmResult result);

/* Has application(s) registered? */
extern CsrBool CsrBtAmpmAppIsEnabled(CsrBtAmpmInstance *inst);

/* Return application subscriber given btConnId */
extern CsrBtAmpmAppSubscriberElem *CsrBtAmpmAppGetSubscriber(CsrBtAmpmInstance *inst,
                                                             CsrBtConnId btConnId);

/* Return application subscriber with test mode bit set */
extern CsrBtAmpmAppSubscriberElem *CsrBtAmpmAppGetTestSubscriber(CsrBtAmpmInstance *inst);

/* General initialisation/shutdown */
extern void CsrBtAmpmAppInit(CsrBtAmpmInstance *inst);
extern void CsrBtAmpmAppDeinit(CsrBtAmpmInstance *inst);

/* Main handler for CM primitives */
extern void CsrBtAmpmAppCmHandler(CsrBtAmpmInstance *inst);

/* Redirection handler for move request */
extern void CsrBtAmpmAppCmMoveChanReq(CsrBtAmpmInstance *inst,
                                      CsrBtAmpmMoveReq *prim);

/* Redirection handler for move response */
extern void CsrBtAmpmAppCmMoveChanRes(CsrBtAmpmInstance *inst,
                                      CsrBtAmpmMoveRes *prim);

/* Automatic move channel initiator */
extern void CsrBtAmpmAppCmAutoMoveChan(CsrBtAmpmInstance *inst,
                                       CsrBtAmpmAppSubscriberElem *sub,
                                       CsrBtAmpController localId,
                                       CsrBtAmpController remoteId);

/* Profile helper general initialisation/shutdown */
extern void CsrBtAmpmPrfInit(CsrBtAmpmInstance *inst);
extern void CsrBtAmpmPrfDeinit(CsrBtAmpmInstance *inst);

/* Profile hint handler: Multi connection information */
extern void CsrBtAmpmPrfMultiConnReqHandler(CsrBtAmpmInstance *inst,
                                            CsrBtAmpmPrfMultiConnReq *prim);

/* Profile hint: Is btConnId a slave we allow to be moved? */
extern CsrBool CsrBtAmpmPrfSlaveAllow(CsrBtAmpmInstance *inst,
                                      CsrBtConnId slave);

/* Profile hint: Is btConnId a master that triggers slave moves? */
extern void CsrBtAmpmPrfMasterTrigger(CsrBtAmpmInstance *inst,
                                      CsrBtConnId master);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_AMPM_APP_H */
