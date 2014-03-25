/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_callback_q.h"

/* Is queue blocked by this class/type */
static CsrBool csrBtCmCallbackIsBlocked(cmInstanceData_t *cmInst,
                                        CmCallbackObj *self)
{
    CmCallbackObj *obj;
    CmCallbackFctMatch *selfmat;
    CsrUintFast8 i;
    CsrUintFast8 j;

    /* Killswitch engaged? */
    if(cmInst->globalState == notReady_s)
    {
        return TRUE;
    }

    /* Self callbacks */
    for(i=0; i<CM_CALLBACK_MAX_FCTS; i++)
    {
        selfmat = &(self->callbacks[i]);
        if(selfmat->func != NULL)
        {
            /* Objects on queue */
            for(obj = (CmCallbackObj*)cmInst->callback;
                obj != NULL;
                obj = obj->next)
            {
                /* Can elements be compared? */
                if(obj != self)
                {
                    /* Object callbacks */
                    for(j=0; j<CM_CALLBACK_MAX_FCTS; j++)
                    {
                        CmCallbackFctMatch *objmat;
                        objmat = &(obj->callbacks[j]);
                        if(objmat->func != NULL)
                        {
                            /* Can objects be compared? */
                            if(selfmat->match == objmat->match)
                            {
                                switch(selfmat->match)
                                {
                                    case CM_CALLBACK_MATCH_CONTEXT:
                                        /* context stored in 'class' */
                                        if(selfmat->class == objmat->class)
                                        {
                                            return TRUE;
                                        }
                                        break;
                                        
                                    case CM_CALLBACK_MATCH_CLASSTYPE:
                                        if((selfmat->class == objmat->class)
                                           && (selfmat->type == objmat->type))
                                        {
                                            return TRUE;
                                        }
                                        break;
                                        
                                    case CM_CALLBACK_MATCH_CLASSTYPEADDR:
                                        if((selfmat->class == objmat->class)
                                           && (selfmat->type == objmat->type)
                                           && CsrBtBdAddrEq(&(selfmat->addr), &(objmat->addr)))
                                        {
                                            return TRUE;
                                        }
                                        break;
                                } /* Match type */
                            } /* Objects comparable */
                        } /* Object match table valid */
                    } /* Object callback list */
                } /* Self and object are not the same */
            } /* Object iteration */
        } /* Self match table valid */
    } /* Self callback list */
    return FALSE;
}

/* Generic queue unblocker */
void CsrBtCmCallbackUnblock(cmInstanceData_t *cmInst)
{
    CmCallbackObj *obj;

    /* Killswitch engaged? */
    if(cmInst->globalState == notReady_s)
    {
        return;
    }

    for(obj = (CmCallbackObj*)cmInst->callback;
        obj != NULL;
        obj = obj->next)
    {
        if((obj->sendMsg != NULL) &&
           !csrBtCmCallbackIsBlocked(cmInst, obj))
        {
            CsrSchedMessagePut(obj->sendQid, obj->sendClass, obj->sendMsg);
            obj->sendMsg = NULL;
        }
    }
}

/* Add callback object to system */
CmCallbackObj *CsrBtCmCallbackNew(cmInstanceData_t *cmInst,
                                  CsrSchedQid sendQid,
                                  CsrPrim sendClass,
                                  CsrUint16 sendType,
                                  void *sendMsg,
                                  void *context)
{
    CmCallbackObj **pp_tail;

    /* Indirect search for tail */
    for(pp_tail = (CmCallbackObj**)&cmInst->callback;
        *pp_tail != NULL;
        pp_tail = &((*pp_tail)->next))
    {
        ;/* No nothing */
    }   
    *pp_tail = CsrPmemZalloc(sizeof(CmCallbackObj));

    (*pp_tail)->sendQid = sendQid;
    (*pp_tail)->sendClass = sendClass;
    (*pp_tail)->sendType = sendType;
    (*pp_tail)->sendMsg = sendMsg;
    (*pp_tail)->context = context;

    return *pp_tail;
}

/* Remove callback */
void CsrBtCmCallbackDelete(cmInstanceData_t *cmInst,
                           CmCallbackObj *obj)
{
    CsrBool freed;
    CsrUintFast8 i;
    CmCallbackObj *po;
    CmCallbackObj **ppn;

    /* See if object should be removed from main list. Use double
     * indirection */
    if(cmInst != NULL)
    {
        for(ppn = (CmCallbackObj**)&cmInst->callback;
            (po = *ppn) != NULL;
            ppn = &po->next)
        {
            if(obj == po)
            {
                *ppn = obj->next;
                break;
            }
        }
    }

    /* Invoke special free-callback-function */
    freed = FALSE;
    for(i=0; i<CM_CALLBACK_MAX_FCTS; i++)
    {
        if(obj->callbacks[i].match == CM_CALLBACK_MATCH_DELETE)
        {
            obj->callbacks[i].func(cmInst, obj, obj->context, NULL);
            freed = TRUE;
        }
    }

    if(!freed)
    {
        /* No special free-callback. Assume flat context */
        CsrPmemFree(obj->context);
    }
    
    if(obj->sendMsg)
    {
        switch(obj->sendClass)
        {
            case RFCOMM_PRIM :
                rfc_free_primitive(obj->sendMsg);
                obj->sendMsg = NULL;
                break;
            case DM_PRIM:
                dm_free_upstream_primitive(obj->sendMsg);
                obj->sendMsg = NULL;
                break;
            case SDP_PRIM:
                sdp_free_upstream_primitive(obj->sendMsg);
                obj->sendMsg = NULL;
                break;
            case L2CAP_PRIM:
                L2CA_FreePrimitive((L2CA_UPRIM_T*)obj->sendMsg);
                obj->sendMsg = NULL;
                break;
        }
        CsrPmemFree(obj->sendMsg);
    }

    CsrPmemFree(obj);
}

/* Deinit code */
#ifdef ENABLE_SHUTDOWN
void CsrBtCmCallbackDeinit(cmInstanceData_t *cmInst)
{
    while(cmInst->callback)
    {
        CsrBtCmCallbackDelete(cmInst, (CmCallbackObj*)cmInst->callback);
    }
}
#endif

/* Add callback/match to object */
CsrBool CsrBtCmCallbackAddFct(CmCallbackObj *obj,
                              CsrUint8 match,
                              CsrUint16 class,
                              CsrUint16 type,
                              CsrBtDeviceAddr *addr,
                              CmCallbackFct *func)
{
    CsrUintFast8 i;
    for(i=0; i<CM_CALLBACK_MAX_FCTS; i++)
    {
        if(obj->callbacks[i].func == NULL)
        {
            obj->callbacks[i].match = match;
            obj->callbacks[i].class = class;
            obj->callbacks[i].type = type;
            obj->callbacks[i].func = func;
            if(addr)
            {
                bd_addr_copy(&obj->callbacks[i].addr, addr);
            }
            else
            {
                bd_addr_zero(&obj->callbacks[i].addr);
            }
            return TRUE;
        }
    }
    return FALSE;
}

/* Invoke callback */
CsrBool CsrBtCmCallbackInvoke(cmInstanceData_t *cmInst,
                              CsrUint8 match,
                              CsrUint16 matchClass,
                              CsrUint16 matchType,
                              CsrBtDeviceAddr *matchAddr,
                              void *msg)
{
    CmCallbackObj *obj;
    CmCallbackObj **pp_obj;
    CsrUintFast8 i;

    /* Double indirect search for the object */
    for(pp_obj = (CmCallbackObj**)&cmInst->callback;
        (pp_obj != NULL) && ((obj = *pp_obj) != NULL);
        pp_obj = &obj->next)
    {
        /* Scan each callback */
        for(i=0;
            (i<CM_CALLBACK_MAX_FCTS) && (obj->callbacks[i].func != NULL);
            i++)
        {
            CmCallbackFctMatch *m;
            m = &(obj->callbacks[i]);

            if ((   (match == CM_CALLBACK_MATCH_CONTEXT)
                    && (matchClass == m->class) )
                || ((match == CM_CALLBACK_MATCH_CLASSTYPE)
                    && (matchClass == m->class)
                    && (matchType == m->type))
                || ((match == CM_CALLBACK_MATCH_CLASSTYPEADDR)
                    && (matchClass == m->class)
                    && (matchType == m->type)
                    && (matchAddr != NULL)
                    && bd_addr_eq(matchAddr, &(m->addr))))
            {
                /* Match found. Remove object from queue and invoke
                 * the callback. Then delete the object and stop */
                *pp_obj = obj->next;
                m->func(cmInst, obj, obj->context, msg);
                CsrBtCmCallbackDelete(NULL, obj);
                return TRUE;
            }
        }
    }

    /* No callback found */
    return FALSE;
}

static CmCallbackObj * csrBtCmCallbackFindMatch(cmInstanceData_t *cmInst,
                                                CsrUint8         match,
                                                CsrUint16        matchClass,
                                                CsrUint16        matchType,
                                                CsrBtDeviceAddr  *matchAddr)
{
    CmCallbackObj *obj;
    CmCallbackObj **pp_obj;
    CsrUint8 i;

    /* Double indirect search for the object */
    for(pp_obj = (CmCallbackObj**)&cmInst->callback;
        (pp_obj != NULL) && ((obj = *pp_obj) != NULL);
        pp_obj = &obj->next)
    {
        /* Scan each callback */
        for(i=0;
            (i<CM_CALLBACK_MAX_FCTS) && (obj->callbacks[i].func != NULL);
            i++)
        {
            CmCallbackFctMatch *m;
            m = &(obj->callbacks[i]);

            if ((   (match == CM_CALLBACK_MATCH_CONTEXT)
                    && (matchClass == m->class) )
                || ((match == CM_CALLBACK_MATCH_CLASSTYPE)
                    && (matchClass == m->class)
                    && (matchType == m->type))
                || ((match == CM_CALLBACK_MATCH_CLASSTYPEADDR)
                    && (matchClass == m->class)
                    && (matchType == m->type)
                    && (matchAddr != NULL)
                    && bd_addr_eq(matchAddr, &(m->addr))))
            {
                /* Match found. */
                if (obj->sendMsg != NULL) 
                {
                    return obj;
                }
            }
        }
    }
    /* No callback found */
    return NULL;
}

/* Helper for simple callback */
void CsrBtCmCallbackSendSimpleBlock(cmInstanceData_t *cmInst,
                                    CsrSchedQid target,
                                    CsrUint16 class,
                                    CsrUint16 rspType,
                                    void *context,
                                    void *msg,
                                    CmCallbackFct *func)
{
    CmCallbackObj *obj;

    obj = CsrBtCmCallbackNew(cmInst,
                             target,
                             class,
                             *(CsrUint16*)msg,
                             msg, /* sendMsg */
                             context);
    (void)CsrBtCmCallbackAddFct(obj,
                                CM_CALLBACK_MATCH_CLASSTYPE,
                                class,
                                rspType,
                                NULL,
                                func);

    /* Send if not blocked */
    if(!csrBtCmCallbackIsBlocked(cmInst, obj))
    {
        CsrSchedMessagePut(target,
                           class,
                           msg);
        obj->sendMsg = NULL;
    }
}

/* Simple dispatcher for class/type blocks */
void CsrBtCmCallbackDispatchSimple(cmInstanceData_t *cmInst,
                                   DM_UPRIM_T *dmPrim)
{
    CmCallbackObj *obj;
    CsrBtCmCallbackInvoke(cmInst,
                          CM_CALLBACK_MATCH_CLASSTYPE,
                          DM_PRIM,
                          dmPrim->type,
                          NULL, /*matchAddr*/
                          dmPrim);

    obj = csrBtCmCallbackFindMatch(cmInst,
                                   CM_CALLBACK_MATCH_CLASSTYPE,
                                   DM_PRIM,
                                   dmPrim->type,
                                   NULL);

    if (obj)
    {
        CsrSchedMessagePut(obj->sendQid, obj->sendClass, obj->sendMsg);
        obj->sendMsg = NULL;
    }
}
