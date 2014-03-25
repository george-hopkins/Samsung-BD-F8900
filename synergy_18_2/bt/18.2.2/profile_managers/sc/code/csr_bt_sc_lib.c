/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "hci_prim.h"
#include "csr_bt_profiles.h"
#include "csr_msg_transport.h"

void CsrBtScMessagePutDownstream(void* msg)
{
    CsrSchedMessagePut(CSR_BT_SC_IFACEQUEUE, CSR_BT_SC_PRIM, msg);
}

void CsrBtScMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_SC_IFACEQUEUE, CSR_BT_SC_PRIM, msg);
}

void CsrBtScMapSecInLevel(CsrUint16 secInput, CsrUint16 *secOutput)
{
    *secOutput = SECL4_IN_SSP | /* Always set SSP. This is ignored if we're not in mode 4 */
        ((secInput & CSR_BT_SEC_AUTHENTICATION) ? SECL_IN_AUTHENTICATION : 0) |
        ((secInput & CSR_BT_SEC_AUTHORISATION)  ? SECL_IN_AUTHORISATION : 0) |
        ((secInput & CSR_BT_SEC_ENCRYPTION)     ? SECL_IN_ENCRYPTION: 0) |
        ((secInput & CSR_BT_SEC_MITM)           ? SECL4_IN_MITM : 0);
}

#define SEC_ACTION_MASK     (CSR_BT_SEC_DEFAULT|CSR_BT_SEC_MANDATORY|CSR_BT_SEC_SPECIFY) /* Security Action Mask*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScSetSecInLevel
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *      secInLevel:         the mapped security settings
 *      secLevel:           the security level
 *      secManLevel:        the mandatory security level
 *      secDefLevel:        the default security level
 *      successCode:        the value to return if operation succeeds
 *      errorCode:          the value to return if operation fails
 *----------------------------------------------------------------------------*/
CsrBtResultCode CsrBtScSetSecInLevel(CsrUint16 *secInLevel, CsrUint16 secLevel,
                                     CsrUint16 secManLevel, CsrUint16 secDefLevel,
                                     CsrBtResultCode successCode, CsrBtResultCode errorCode)
{
    CsrBtResultCode rval;
    CsrUint8 action;

    action = secLevel & SEC_ACTION_MASK;

    switch(action)
    {
        case CSR_BT_SEC_DEFAULT:
            CsrBtScMapSecInLevel(secDefLevel, secInLevel);
            rval = successCode;
            break;
        case CSR_BT_SEC_MANDATORY:
            CsrBtScMapSecInLevel(secManLevel, secInLevel);
            rval = successCode;
            break;
        case CSR_BT_SEC_SPECIFY:
            if ((secLevel & secManLevel) == secManLevel)
            {
                CsrBtScMapSecInLevel(secLevel, secInLevel);
                rval = successCode;
                break;
            }
            /* !! FALL THROUGH !! */
        default:
            /* Use mandatory settings if invalid action is requested */
            CsrBtScMapSecInLevel(secManLevel, secInLevel);
            rval = errorCode;
            break;
    }

    return rval;
}

void CsrBtScMapSecOutLevel(CsrUint16 secInput, CsrUint16 *secOutput)
{
    *secOutput = SECL4_OUT_SSP | /* Always set SSP. This is ignored if we're not in mode 4 */
        ((secInput & CSR_BT_SEC_AUTHENTICATION) ? SECL_OUT_AUTHENTICATION : 0) |
        ((secInput & CSR_BT_SEC_AUTHORISATION)  ? SECL_OUT_AUTHORISATION : 0) |
        ((secInput & CSR_BT_SEC_ENCRYPTION)     ? SECL_OUT_ENCRYPTION: 0) |
        ((secInput & CSR_BT_SEC_MITM)           ? SECL4_OUT_MITM : 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScSetSecOutLevel
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *      secOutLevel:        the mapped security settings
 *      secLevel:           the security level
 *      secManLevel:        the mandatory security level
 *      secDefLevel:        the default security level
 *      successCode:        the value to return if operation succeeds
 *      errorCode:          the value to return if operation fails
 *----------------------------------------------------------------------------*/
CsrBtResultCode CsrBtScSetSecOutLevel(CsrUint16 *secOutLevel, CsrUint16 secLevel,
                                      CsrUint16 secManLevel, CsrUint16 secDefLevel,
                                      CsrBtResultCode successCode, CsrBtResultCode errorCode)
{
    CsrBtResultCode rval;
    CsrUint8 action;

    action = secLevel & SEC_ACTION_MASK;

    switch(action)
    {
        case CSR_BT_SEC_DEFAULT:
            CsrBtScMapSecOutLevel(secDefLevel, secOutLevel);
            rval = successCode;
            break;
        case CSR_BT_SEC_MANDATORY:
            CsrBtScMapSecOutLevel(secManLevel, secOutLevel);
            rval = successCode;
            break;
        case CSR_BT_SEC_SPECIFY:
            if ((secLevel & secManLevel) == secManLevel)
            {
                CsrBtScMapSecOutLevel(secLevel, secOutLevel);
                rval = successCode;
                break;
            }
            /* !! FALL THROUGH !! */
        default:
            /* Use mandatory settings if invalid action is requested */
            CsrBtScMapSecOutLevel(secManLevel, secOutLevel);
            rval = errorCode;
            break;
    }

    return rval;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScDeregisterReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SC_DEREGISTER_REQ message to the SC message
 *        queue.
 *
 *    PARAMETERS
 *        protocolId:         Protocol at which security is to be applied
 *        channel:            Channel for that protocol
 *----------------------------------------------------------------------------*/
void CsrBtScDeregisterReqSend(dm_protocol_id_t protocolId, CsrUint16 channel)
{
    CsrBtScDeregisterReq *prim = (CsrBtScDeregisterReq *) CsrPmemAlloc(sizeof(CsrBtScDeregisterReq));
    prim->type = CSR_BT_SC_DEREGISTER_REQ;
    prim->protocolId = protocolId;
    prim->channel = channel;

    CsrBtScMessagePutDownstream(prim);
}
