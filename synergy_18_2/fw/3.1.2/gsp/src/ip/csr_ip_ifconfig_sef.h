#ifndef CSR_IP_IFCONFIG_SEF_H__
#define CSR_IP_IFCONFIG_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_ip_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIpIfconfigSubscribeReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigUnsubscribeReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigUpReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigDownReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigNatReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigForwardReqHandler(CsrIpInstanceData *instanceData);
void CsrIpIfconfigArpEntryAddReqHandler(CsrIpInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
