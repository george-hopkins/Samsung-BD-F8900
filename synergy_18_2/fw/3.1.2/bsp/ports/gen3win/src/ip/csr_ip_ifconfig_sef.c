/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"

#include "csr_ip_handler.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_ifconfig_lib.h"

void CsrIpIfconfigSubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigSubscribeReq *msg = (CsrIpIfconfigSubscribeReq *) (instanceData->msg);
    CsrIpIfconfigIfDetails *ifDetails = NULL;
    CsrUint16 ifDetailsCount = 0;

    CsrIpIfconfigSubscribeCfmSend(msg->appHandle, ifDetailsCount, ifDetails);
}

void CsrIpIfconfigUnsubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigUnsubscribeReq *msg = (CsrIpIfconfigUnsubscribeReq *) (instanceData->msg);

    CsrIpIfconfigUnsubscribeCfmSend(msg->appHandle);
}

void CsrIpIfconfigUpReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigUpReq *msg = (CsrIpIfconfigUpReq *) (instanceData->msg);

    CsrIpIfconfigUpCfmSend(msg->appHandle, msg->ifHandle, CSR_RESULT_SUCCESS);
}

void CsrIpIfconfigDownReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigDownReq *msg = (CsrIpIfconfigDownReq *) (instanceData->msg);

    CsrIpIfconfigDownCfmSend(msg->appHandle, msg->ifHandle, CSR_RESULT_SUCCESS);
}

void CsrIpIfconfigNatReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigNatReq *msg = instanceData->msg;

    CsrIpIfconfigNatCfmSend(msg->appHandle, msg->ifHandle,
        CSR_IP_IFCONFIG_RESULT_NOT_SUPPORTED);
}

void CsrIpIfconfigForwardReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigForwardReq *msg = instanceData->msg;

    CsrIpIfconfigForwardCfmSend(msg->appHandle,
        CSR_IP_IFCONFIG_RESULT_NOT_SUPPORTED);
}

void CsrIpIfconfigArpEntryAddReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigArpEntryAddReq *request = instanceData->msg;

    CsrIpIfconfigArpEntryAddCfmSend(request->appHandle,
        CSR_IP_IFCONFIG_RESULT_NOT_SUPPORTED);
}
