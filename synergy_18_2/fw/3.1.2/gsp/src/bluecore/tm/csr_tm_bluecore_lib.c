/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_pmem.h"

CsrTmBluecoreActivateTransportReq *CsrTmBlueCoreActivateTransportReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreActivateTransportReq *prim;

    prim = (CsrTmBluecoreActivateTransportReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreActivateTransportReq));
    prim->type = CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreDeactivateTransportReq *CsrTmBlueCoreDeactivateTransportReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreDeactivateTransportReq *prim;
    prim = (CsrTmBluecoreDeactivateTransportReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreDeactivateTransportReq));
    prim->type = CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ;
    prim->phandle = phandle;
    return prim;
}

#ifdef CSR_CHIP_MANAGER_ENABLE
CsrTmBluecoreCmStatusSubscribeReq *CsrTmBlueCoreCmStatusSubscribeReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusSubscribeReq *prim;
    prim = (CsrTmBluecoreCmStatusSubscribeReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusSubscribeReq));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreCmStatusUnsubscribeReq *CsrTmBlueCoreCmStatusUnsubscribeReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusUnsubscribeReq *prim;
    prim = (CsrTmBluecoreCmStatusUnsubscribeReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusUnsubscribeReq));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreCmPingIntervalSetReq *CsrTmBlueCoreCmPingIntervalSetReq_struct(CsrSchedQid phandle, CsrTime interval)
{
    CsrTmBluecoreCmPingIntervalSetReq *prim;
    prim = (CsrTmBluecoreCmPingIntervalSetReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmPingIntervalSetReq));
    prim->type = CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ;
    prim->phandle = phandle;
    prim->interval = interval;
    return prim;
}

CsrTmBluecoreCmReplayRegisterReq *CsrTmBlueCoreCmReplayRegisterReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreCmReplayRegisterReq *prim;
    prim = (CsrTmBluecoreCmReplayRegisterReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmReplayRegisterReq));
    prim->type = CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreCmReplayStartRes *CsrTmBlueCoreCmReplayStartRes_struct(CsrSchedQid phandle, CsrResult result)
{
    CsrTmBluecoreCmReplayStartRes *prim;
    prim = (CsrTmBluecoreCmReplayStartRes *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmReplayStartRes));
    prim->type = CSR_TM_BLUECORE_CM_REPLAY_START_RES;
    prim->phandle = phandle;
    prim->result = result;
    return prim;
}

CsrTmBluecoreCmReplayErrorReq *CsrTmBlueCoreCmReplayErrorReq_struct(CsrUint16 hciOpcode)
{
    CsrTmBluecoreCmReplayErrorReq *prim;
    prim = (CsrTmBluecoreCmReplayErrorReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmReplayErrorReq));
    prim->type = CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ;
    prim->hciOpcode = hciOpcode;
    return prim;
}

#endif /* CSR_CHIP_MANAGER_ENABLE */

#ifdef CSR_BLUECORE_ONOFF
CsrTmBluecoreRegisterReq *CsrTmBlueCoreRegisterReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreRegisterReq *prim;
    prim = (CsrTmBluecoreRegisterReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreRegisterReq));
    prim->type = CSR_TM_BLUECORE_REGISTER_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreUnregisterReq *CsrTmBlueCoreUnregisterReq_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreUnregisterReq *prim;
    prim = (CsrTmBluecoreUnregisterReq *) CsrPmemAlloc(sizeof(CsrTmBluecoreUnregisterReq));
    prim->type = CSR_TM_BLUECORE_UNREGISTER_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreActivateTransportRes *CsrTmBlueCoreActivateTransportRes_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreActivateTransportRes *prim;
    prim = (CsrTmBluecoreActivateTransportRes *) CsrPmemAlloc(sizeof(CsrTmBluecoreActivateTransportRes));
    prim->type = CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES;
    prim->phandle = phandle;
    return prim;
}

CsrTmBluecoreDeactivateTransportRes *CsrTmBlueCoreDeactivateTransportRes_struct(CsrSchedQid phandle)
{
    CsrTmBluecoreDeactivateTransportRes *prim;
    prim = (CsrTmBluecoreDeactivateTransportRes *) CsrPmemAlloc(sizeof(CsrTmBluecoreDeactivateTransportRes));
    prim->type = CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES;
    prim->phandle = phandle;
    return prim;
}

#endif /* CSR_BLUECORE_ONOFF */
