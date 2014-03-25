/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm.h"

CsrBool CsrBtCmBnepRoleSwitchAllowed(cmInstanceData_t *cmData)
{
    CsrUintFast8 i;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if(!CsrBtBdAddrEqZero(&cmData->bnepVar.connectVar[i].deviceAddr))
        {
            return FALSE;
        }
    }
    return TRUE;
}

CsrUint8 returnEmptyBnepIdIndex(cmInstanceData_t *cmData)
{ /* Try to find a free index in the bnep connection table. If not it return ERROR */
    CsrUintFast8 i;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if(cmData->bnepVar.connectVar[i].id == ID_EMPTY)
        {
            return (CsrUint8)i;
        }
    }
    return CM_ERROR;
}

CsrUint8 returnReserveBnepIdIndex(cmInstanceData_t *cmData)
{ /* Try to find a reserve index in the bnep connection table. If not it return ERROR */
    CsrUintFast8 i;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if(cmData->bnepVar.connectVar[i].id == ID_RESERVED)
        {
            return (CsrUint8)i;
        }
    }
    return CM_ERROR;
}

CsrUint8 returnBnepIdIndex(cmInstanceData_t *cmData, CsrUint16 id)
{ /* Try to find a free index in the l2cap connection table. If not it return ERROR */
    CsrUintFast8 i;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if(cmData->bnepVar.connectVar[i].id == id)
        {
            return (CsrUint8)i;
        }
    }
    return CM_ERROR;
}

CsrUint8 returnIndexToThisBdAddr(cmInstanceData_t *cmData, CsrBtDeviceAddr    deviceAddr)
{ /* Try to find a index to that bdAddr in the the bnep connection table with the. If not it return ERROR */
    CsrUintFast8 i;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if ( bd_addr_eq( &(cmData->bnepVar.connectVar[i].deviceAddr), &(deviceAddr) ) == TRUE )
        {
            return (CsrUint8)i;
        }
    }
    return CM_ERROR;
}

void CsrBtCmBnepClearBnepTableIndex(bnepTable *theLink)
{
    bd_addr_zero(&theLink->deviceAddr);
    theLink->actualMode           = CSR_BT_ACTIVE_MODE;
    theLink->id                   = ID_EMPTY;
    theLink->requestedMode        = CSR_BT_ACTIVE_MODE;
    CSR_BT_CM_STATE_CHANGE(theLink->state, idle_s);
}
#endif


