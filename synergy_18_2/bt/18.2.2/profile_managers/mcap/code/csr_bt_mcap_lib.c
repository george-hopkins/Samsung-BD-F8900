/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MCAP_MODULE

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_mcap_private_lib.h"

void CsrBtMcapMsgTransport(void *msg)
{
    CsrMsgTransport(CSR_BT_MCAP_IFACEQUEUE, CSR_BT_MCAP_PRIM, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapConnectMclReqSend
 *
 *  DESCRIPTION
 *      Create the MCL connection
 *
 *  PARAMETERS
 *      bd_addr,
 *      psm,
 *      phandle,
 *      secLevel;
 *      mtu;
 *----------------------------------------------------------------------------*/
void CsrBtMcapConnectMclReqSend(CsrBtDeviceAddr     bd_addr,
                                psm_t               localPsm,
                                psm_t               remotePsm,
                                CsrSchedQid         phandle,
                                dm_security_level_t secLevel,
                                CsrTime             sniffTimeout)
{
    CsrBtMcapConnectMclReq *prim = (CsrBtMcapConnectMclReq  *) CsrPmemAlloc(sizeof(CsrBtMcapConnectMclReq));
    prim->type              = CSR_BT_MCAP_CONNECT_MCL_REQ;
    prim->bd_addr           = bd_addr;
    prim->localPsm          = localPsm;
    prim->remotePsm         = remotePsm;
    prim->phandle           = phandle;
    prim->secLevel          = secLevel;
    prim->sniffTimeout      = sniffTimeout;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapDisconnectMclReqSend
 *
 *  DESCRIPTION
 *      Disconnect the MCL connection
 *
 *  PARAMETERS
 *      bd_addr,
 *      psm,
 *      phandle,
 *----------------------------------------------------------------------------*/
void CsrBtMcapDisconnectMclReqSend(CsrBtDeviceAddr bd_addr,
                                   psm_t           psm,
                                   CsrSchedQid     phandle)

{
    CsrBtMcapDisconnectMclReq    * prim;
    prim                    = (CsrBtMcapDisconnectMclReq  *) CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMclReq));
    prim->type              = CSR_BT_MCAP_DISCONNECT_MCL_REQ;
    prim->bd_addr           = bd_addr;
    prim->psm               = psm;
    prim->phandle           = phandle;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapCreateMdlResSend
 *
 *  DESCRIPTION
 *      Send an MD_CREATE_MDL_RES to remote device using the communication channel.
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *      mcapResponse;
 *      configResponse;
 *      secLevel;
 *      mtu;
 *      *flow;
 *----------------------------------------------------------------------------*/
void CsrBtMcapCreateMdlResSend(CsrUint16                mclId,
                               CsrUint16                mdlId,
                               McapResponseType         mcapResponse,
                               CsrUint8                 configResponse,
                               psm_t                    psm,
                               dm_security_level_t      secLevel,
                               l2ca_mtu_t               mtu,
                                L2CA_FLOW_T             *flow)
{
    CsrBtMcapCreateMdlRes        * prim;
    prim                    = (CsrBtMcapCreateMdlRes  *) CsrPmemAlloc(sizeof(CsrBtMcapCreateMdlRes));
    prim->type              = CSR_BT_MCAP_CREATE_MDL_RES;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->mcapResponse      = mcapResponse;
    prim->configResponse    = configResponse;
    prim->psm               = psm;
    prim->secLevel          = secLevel;
    prim->mtu               = mtu;
    prim->qos               = NULL;
    prim->qosCount          = 0;
    prim->flow              = flow;
    prim->flowCount         = flow ? 1 : 0;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapReconnectMdlReqSend
 *
 *  DESCRIPTION
 *      Send an MD_RECONNECT_MDL_REQ to remote device using the communication channel.
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *----------------------------------------------------------------------------*/
void CsrBtMcapReconnectMdlReqSend(CsrUint16        mclId,
                                  CsrUint16        mdlId,
                                  psm_t           localPsm,
                                  psm_t           remotePsm)

{
    CsrBtMcapReconnectMdlReq     * prim;
    prim                    = (CsrBtMcapReconnectMdlReq  *) CsrPmemAlloc(sizeof(CsrBtMcapReconnectMdlReq));
    prim->type              = CSR_BT_MCAP_RECONNECT_MDL_REQ;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->localPsm          = localPsm;
    prim->remotePsm         = remotePsm;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapReconnectMdlResSend
 *
 *  DESCRIPTION
 *      Send an MD_RECONNECT_MDL_RES to remote device using the communication channel.
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *      mcapResponse;
 *      secLevel;
 *      mtu;
 *      *flow;
 *      sniffTimeout;
 *----------------------------------------------------------------------------*/
void CsrBtMcapReconnectMdlResSend(CsrUint16                mclId,
                                  CsrUint16                mdlId,
                                  McapResponseType         mcapResponse,
                                  psm_t                    psm,
                                  dm_security_level_t      secLevel,
                                  l2ca_mtu_t               mtu,
                                  L2CA_FLOW_T             *flow)
{
    CsrBtMcapReconnectMdlRes *prim;
    prim                    = (CsrBtMcapReconnectMdlRes  *) CsrPmemAlloc(sizeof(CsrBtMcapReconnectMdlRes));
    prim->type              = CSR_BT_MCAP_RECONNECT_MDL_RES;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->mcapResponse      = mcapResponse;
    prim->psm               = psm;
    prim->secLevel          = secLevel;
    prim->mtu               = mtu;
    prim->qos               = NULL;
    prim->qosCount          = 0;
    prim->flow              = flow;
    prim->flowCount         = flow ? 1 : 0;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapConnectMdlReqSend
 *
 *  DESCRIPTION
 *      Create an MDL connection
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *      psm;
 *      secLevel;
 *      mtu;
 *      *flow;
 *----------------------------------------------------------------------------*/
void CsrBtMcapConnectMdlReqSend(CsrUint16                mclId,
                                CsrUint16                mdlId,
                                CsrUint8                 mdepId,
                                CsrUint8                 config,
                                psm_t                    localPsm,
                                psm_t                    remotePsm,
                                dm_security_level_t      secLevel,
                                l2ca_mtu_t               mtu,
                                L2CA_FLOW_T             *flow)
{
    CsrBtMcapConnectMdlReq       * prim;

    prim                    = (CsrBtMcapConnectMdlReq  *) CsrPmemAlloc(sizeof(CsrBtMcapConnectMdlReq));
    prim->type              = CSR_BT_MCAP_CONNECT_MDL_REQ;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->mdepId            = mdepId;
    prim->configuration     = config;
    prim->localPsm          = localPsm;
    prim->remotePsm         = remotePsm;
    prim->secLevel          = secLevel;
    prim->mtu               = mtu;
    prim->qos               = NULL;
    prim->qosCount          = 0;
    prim->flow              = flow;
    prim->flowCount         = flow ? 1 : 0;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapDisconnectMdlReqSend
 *
 *  DESCRIPTION
 *      Disconnect the MDL connection
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *----------------------------------------------------------------------------*/
void CsrBtMcapDisconnectMdlReqSend(CsrUint16        mclId,
                                   CsrUint16        mdlId)

{
    CsrBtMcapDisconnectMdlReq    * prim;

    prim                    = (CsrBtMcapDisconnectMdlReq  *) CsrPmemAlloc(sizeof(CsrBtMcapDisconnectMdlReq));
    prim->type              = CSR_BT_MCAP_DISCONNECT_MDL_REQ;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapDeleteMdlReqSend
 *
 *  DESCRIPTION
 *      Send an MD_DELETE_MDL_REQ to remote device using the communication channel.
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *----------------------------------------------------------------------------*/
void CsrBtMcapDeleteMdlReqSend(CsrUint16        mclId,
                               CsrUint16        mdlId)
{
    CsrBtMcapDeleteMdlReq *prim;

    prim                    = (CsrBtMcapDeleteMdlReq  *) CsrPmemAlloc(sizeof(CsrBtMcapDeleteMdlReq));
    prim->type              = CSR_BT_MCAP_DELETE_MDL_REQ;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapDeleteMdlResSend
 *
 *  DESCRIPTION
 *      Send an MD_DELETE_MDL_RES to remote device using the communication channel.
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *      mcapResponse;
 *----------------------------------------------------------------------------*/
void CsrBtMcapDeleteMdlResSend(CsrUint16            mclId,
                               CsrUint16            mdlId,
                               McapResponseType     mcapResponse)
{
    CsrBtMcapDeleteMdlRes *prim;

    prim                    = (CsrBtMcapDeleteMdlRes  *) CsrPmemAlloc(sizeof(CsrBtMcapDeleteMdlRes));
    prim->type              = CSR_BT_MCAP_DELETE_MDL_RES;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->mcapResponse      = mcapResponse;
    CsrBtMcapMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMcapDataSendReqSend
 *
 *  DESCRIPTION
 *      Send data on an MDL connection
 *
 *  PARAMETERS
 *      bd_addr,
 *      mdlId;
 *      length;
 *      *payload;
 *----------------------------------------------------------------------------*/
void CsrBtMcapDataSendReqSend(CsrUint16        mclId,
                              CsrUint16        mdlId,
                              CsrUint16        length,
                              void            *payload)
{
    CsrBtMcapDataSendReq *prim = (CsrBtMcapDataSendReq  *) CsrPmemAlloc(sizeof(CsrBtMcapDataSendReq));
    prim->type              = CSR_BT_MCAP_DATA_SEND_REQ;
    prim->mclId             = mclId;
    prim->mdlId             = mdlId;
    prim->length            = length;
    prim->payload           = payload;
    CsrBtMcapMsgTransport(prim);
}

#endif
