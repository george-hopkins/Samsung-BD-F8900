#ifndef CSR_IP_ETHER_SEF_H__
#define CSR_IP_ETHER_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void csrIpEtherHandleEthers(CsrIpInstanceData *, int *, fd_set *);
void csrIpEtherRemove(CsrIpInstanceData *, CsrUint32);
void csrIpEtherFree(CsrIpInstanceData *, CsrUint32);

void CsrIpEtherIfAddReqHandler(CsrIpInstanceData *instanceData);
void CsrIpEtherIfRemoveReqHandler(CsrIpInstanceData *instanceData);
void CsrIpEtherIfLinkUpReqHandler(CsrIpInstanceData *instanceData);
void CsrIpEtherIfLinkDownReqHandler(CsrIpInstanceData *instanceData);
void CsrIpEtherIfFlowControlPauseReqHandler(CsrIpInstanceData *instanceData);
void CsrIpEtherIfFlowControlResumeReqHandler(CsrIpInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
