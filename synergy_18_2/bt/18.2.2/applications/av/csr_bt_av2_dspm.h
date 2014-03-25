#ifndef CSR_BT_AV2_DSPM_H__
#define CSR_BT_AV2_DSPM_H__

#ifdef CSR_USE_DSPM
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include <stdio.h>
#include "csr_bt_av2.h"
#include "csr_sbc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Instance data for avrouter filter */
typedef struct
{
    CsrUint8          sbc_capabilities[4];
    CsrSbcChannelMode channel_mode;
    CsrSbcAllocMethod alloc_method;
    void             *sbcHdl;
    CsrUint8          channels;
    CsrUint16         sample_freq;
    CsrUint8          blocks;
    CsrUint8          subbands;
    CsrUint8          bitPool;
    CsrUint8          minBitpool;
    CsrUint8          maxBitpool;
    CsrUint8          remoteMinBitpool;
    CsrUint8          remoteMaxBitpool;
    CsrUint16         framesize;
    av2filter_t      *filter;
    av2instance_t    *avInstance;
    char             *filename;
    FILE             *file;
} avrouter_instance_t;

void CsrAv2DspmInit(av2instance_t *inst);
void CsrAv2DspmActivateRspHandler(av2instance_t *instData);
void CsrAv2DspmStreamConnect(av2instance_t *instData);
void CsrBtAv2DspmStreamOpenRspHandle(av2instance_t *instData);
void CsrBtAv2DspmConfigStreamRspHandler(av2instance_t *instData);
void CsrBtAv2DspmSendCreateOperator(av2instance_t *instData);
void CsrBtAv2DspmCreateOperatorsRspHandler(av2instance_t *instData);
void CsrBtAv2DspmSendConnectStreams(av2instance_t *instData);
void CsrBtAv2DspmConnectionCreateRspHandler(av2instance_t *instData);
void CsrBtAv2DspmOperatorStartRspHandler(av2instance_t *instData);
void CsrBtAv2DspmSendDisconnectStreams(av2instance_t *instData);
void CsrBtAv2DspmOperatorStopRspHandler(av2instance_t *instData);
void CsrBtAv2DspmConnectionDestroyRspHandler(av2instance_t *instData);
void CsrBtAv2DspmOperatorDestroyRspHandler(av2instance_t *instData);
void CsrBtAv2DspmStreamCpCloseRspHandler(av2instance_t *instData);
void CsrBtAv2DspmStreamOpMsgRspHandler(av2instance_t *instData);
void CsrAv2DspmStreamReadyToStart(av2instance_t *instData);

#ifdef __cplusplus
}
#endif

#endif /*CSR_USE_DSPM*/

#endif /* CSR_BT_AV2_DSPM_H__ */
