#ifndef CSR_BT_CM_CALLBACK_Q_H__
#define CSR_BT_CM_CALLBACK_Q_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

****************************************************************************/

#include "csr_bt_cm_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CM_CALLBACK_MAX_FCTS             (4)               /* max callbacks per object */

#define CM_CALLBACK_MATCH_CONTEXT        (0)               /* match on context */
#define CM_CALLBACK_MATCH_CLASSTYPE      (1)               /* match on class+type */
#define CM_CALLBACK_MATCH_CLASSTYPEADDR  (2)               /* match on class+type with address */
#define CM_CALLBACK_MATCH_DELETE         (3)               /* special free function */

struct cmCallbackObjTag;
typedef struct cmCallbackObjTag CmCallbackObj;

/* Callback function prototype */
typedef void (CmCallbackFct)(cmInstanceData_t *cmInst,     /* Main CM instance */
                             CmCallbackObj *object,        /* Callback object */
                             void *context,                /* Callback instance (obj->context) */
                             void *event);                 /* Event to handle (NULL only for delete callbacks) */

/* Internal callback function/match storage */
typedef struct
{
    CsrUint8                 match;                        /* how to match */
    CsrUint16                class;                        /* primitive class / context */
    CsrUint16                type;                         /* primitive number */
    CsrBtDeviceAddr          addr;                         /* primitive class+type with address */
    CmCallbackFct            *func;                        /* callback function */
} CmCallbackFctMatch;

/* Callback object */
struct cmCallbackObjTag
{
    CmCallbackObj            *next;                        /* next */
    CmCallbackFctMatch        callbacks[CM_CALLBACK_MAX_FCTS]; /* table of callbacks and match parameters */
    CsrSchedQid               sendQid;                     /* target qid for blocked message */
    CsrPrim                   sendClass;                   /* blocked message class */
    CsrUint16                 sendType;                    /* blocked message type */
    void                     *sendMsg;                     /* blocked message (may be NULL) */
    void                     *context;                     /* callback private context */
};

/* Allocate and add callback object to system */
CmCallbackObj *CsrBtCmCallbackNew(cmInstanceData_t *cmInst,
                                  CsrSchedQid sendQid,
                                  CsrPrim sendClass,
                                  CsrUint16 sendType,
                                  void *sendMsg,
                                  void *context);

/* Remove callback */
void CsrBtCmCallbackDelete(cmInstanceData_t *cmInst,
                           CmCallbackObj *obj);

/* Add callback/match to object */
CsrBool CsrBtCmCallbackAddFct(CmCallbackObj *obj,
                              CsrUint8 match,
                              CsrUint16 class,
                              CsrUint16 type,
                              CsrBtDeviceAddr *addr,
                              CmCallbackFct *func);

/* Search for and invoke callback function */
CsrBool CsrBtCmCallbackInvoke(cmInstanceData_t *cmInst,
                              CsrUint8 match,
                              CsrUint16 matchClass,
                              CsrUint16 matchType,
                              CsrBtDeviceAddr *matchAddr,
                              void *msg);

/* Shutdown code */
#ifdef ENABLE_SHUTDOWN
void CsrBtCmCallbackDeinit(cmInstanceData_t *cmInst);
#endif

/* Generic queue unblock */
void CsrBtCmCallbackUnblock(cmInstanceData_t *cmInst);

/* Simple dispatcher for class/type blocks */
void CsrBtCmCallbackDispatchSimple(cmInstanceData_t *cmInst,
                                   DM_UPRIM_T *dmPrim);

/* Send simple blocking class+type primitive */
void CsrBtCmCallbackSendSimpleBlock(cmInstanceData_t *cmInst,
                                    CsrSchedQid target,
                                    CsrUint16 class,
                                    CsrUint16 rspType,
                                    void *context,
                                    void *msg,
                                    CmCallbackFct *func);
    
#define CsrBtCmCallbackSendSimpleBlockDm(inst, rsp, ctx, msg, func) \
    CsrBtCmCallbackSendSimpleBlock((inst), DM_IFACEQUEUE, DM_PRIM,\
                                   (rsp), (ctx), (msg), (func))

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_CM_CALLBACK_Q_H__ */
