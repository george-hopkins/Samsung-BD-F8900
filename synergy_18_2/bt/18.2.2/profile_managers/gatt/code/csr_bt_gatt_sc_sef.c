/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_gatt_private.h"

/* Check whether a result failed due to security -- and whether we can
 * and should do anything about this */
CsrBool CsrBtGattCheckSecurity(GattMainInst *inst,
                               att_result_t result,
                               CsrBtGattQueueElement *qelm)
{
    /* We only deal with authentication and encryption. Authorisation
     * and key size must be dealt with in the application.
       If msgState == CSR_BT_GATT_MSG_QUEUE_CANCELLED the ungoing
       procedure is cancelled, e.g no need to start the security
       procedure. */
    if( qelm->securityFunc                                &&
        qelm->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED &&
        (result == ATT_RESULT_INSUFFICIENT_AUTHENTICATION || 
         result == ATT_RESULT_INSUFFICIENT_ENCRYPTION) ) 
    {
        CsrBtGattConnElement *conn = NULL;
        
        if (qelm->gattId != inst->privateGattId)
        {
            CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                       &conn,
                                                       qelm->btConnId,
                                                       qelm->gattId);
        }
        else
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst, 
                                                                   &qelm->btConnId);
        }

        /* Is security allowed */
        if(conn && !(conn->flags & CSR_BT_GATT_FLAGS_NO_AUTO_SECURITY))
        { /* Fire up security. */
            qelm->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_SECURITY;

            if (result == ATT_RESULT_INSUFFICIENT_AUTHENTICATION)
            {
                CsrBtScLeSecurityReqSend(CSR_BT_GATT_IFACEQUEUE,
                                         DM_SM_SECURITY_AUTHENTICATED_BONDING,
                                         conn->peerAddr,
                                         conn->l2capFlags);
            }
            else 
            {
                CsrBtScLeSecurityReqSend(CSR_BT_GATT_IFACEQUEUE,
                                         DM_SM_SECURITY_UNAUTHENTICATED_BONDING,
                                         conn->peerAddr,
                                         conn->l2capFlags);
            }
            return TRUE;
        }
    }

    /* Security was not our concern... */
    return FALSE;
}

static void csrBtGattScLeSecurityCfmHandler(GattMainInst *inst, CsrBtScLeSecurityCfm *cfm)
{
    CsrBtGattConnElement *conn = CSR_BT_GATT_CONN_INST_FIND_CONN_ID_FROM_ADDRESS(inst->connInst, 
                                                                                 &(cfm->address)); 
    if (conn)
    {
        CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &(conn->btConnId));

        if (qElem && qElem->gattMsg && qElem->securityFunc)
        {
            if (qElem->securityFunc(inst, qElem, cfm->resultCode, cfm->resultSupplier))
            { /* Set qElem->securityFunc to NULL to avoid retry deadlock */
                qElem->securityFunc = NULL;
            }
            else
            { /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
        }
        else
        {
            CsrStateEventException("CSR_BT_SC_LE_SECURITY_CFM received without GATT handler",
                                    CSR_BT_SC_PRIM, CSR_BT_SC_LE_SECURITY_CFM, 0);
        }
    }
    /* Else - The connection has been release while running security, just ignore */
}

void CsrBtGattDispatchSc(GattMainInst *inst)
{
    CsrPrim type = *(CsrPrim *)inst->msg;
    switch(type)
    {
        case CSR_BT_SC_LE_SECURITY_CFM:
            csrBtGattScLeSecurityCfmHandler(inst,
                                            (CsrBtScLeSecurityCfm*)inst->msg);
            break;

        default:
            CsrStateEventException("GATT handler - unknown SC primitive", CSR_BT_SC_PRIM, type, 0);
            break;
    }
}
