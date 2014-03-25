#ifndef CSR_TFTP_SEF_H__
#define CSR_TFTP_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_tftp_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrTftpActivateHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpDeactivateHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpConnectHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpReadHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpWriteHandler(CsrTftpInstanceData *instanceData);

extern void CsrTftpIpSocketUdpNewHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpBindHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpDataIndHandler(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpDataCfmHandler(CsrTftpInstanceData *instanceData);

extern void CsrTftpIpSocketUdpNewHandlerDeactivating(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpBindHandlerDeactivating(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpDataIndHandlerDeactivating(CsrTftpInstanceData *instanceData);
extern void CsrTftpIpSocketUdpDataCfmHandlerDeactivating(CsrTftpInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
