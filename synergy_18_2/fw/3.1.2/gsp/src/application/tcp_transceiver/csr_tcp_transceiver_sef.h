#ifndef CSR_TCP_TRANSCEIVER_SEF_H__
#define CSR_TCP_TRANSCEIVER_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_tcp_transceiver_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrTcpTransceiverIpSocketTcpNewCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpBindCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpListenCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpConnectCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpAcceptIndHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpDataCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpDataIndHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketTcpCloseIndHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverIpSocketOptionsCfmHandler(CsrTcpTransceiverInstanceData *instanceData);

void CsrTcpTransceiverAppRegisterCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverAppTakeControlIndHandler(CsrTcpTransceiverInstanceData *instanceData);

void CsrTcpTransceiverUiUieCreateCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverUiInputdialogGetCfmHandler(CsrTcpTransceiverInstanceData *instanceData);
void CsrTcpTransceiverUiEventIndHandler(CsrTcpTransceiverInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
