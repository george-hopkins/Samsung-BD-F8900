/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_dspm_converter_init.h"
#ifndef EXCLUDE_CSR_DSPM_MODULE
#include "csr_dspm_serialize.h"
#include "csr_dspm_prim.h"

static CsrMsgConvMsgEntry csrdspm_conv_lut[] =
{
    {CSR_DSPM_ACTIVATE_REQ, CsrDspmActivateReqSizeof, CsrDspmActivateReqSer, CsrDspmActivateReqDes, CsrDspmActivateReqSerFree},
    {CSR_DSPM_DEACTIVATE_REQ, CsrDspmDeactivateReqSizeof, CsrDspmDeactivateReqSer, CsrDspmDeactivateReqDes, CsrDspmDeactivateReqSerFree},
    {CSR_DSPM_STREAM_CP_OPEN_REQ, CsrDspmStreamCpOpenReqSizeof, CsrDspmStreamCpOpenReqSer, CsrDspmStreamCpOpenReqDes, CsrDspmStreamCpOpenReqSerFree},
    {CSR_DSPM_STREAM_CP_CLOSE_REQ, CsrDspmStreamCpCloseReqSizeof, CsrDspmStreamCpCloseReqSer, CsrDspmStreamCpCloseReqDes, CsrDspmStreamCpCloseReqSerFree},
    {CSR_DSPM_STREAM_SCO_ENABLE_REQ, CsrDspmStreamScoEnableReqSizeof, CsrDspmStreamScoEnableReqSer, CsrDspmStreamScoEnableReqDes, CsrDspmStreamScoEnableReqSerFree},
    {CSR_DSPM_STREAM_CONFIGURE_REQ, CsrDspmStreamConfigureReqSizeof, CsrDspmStreamConfigureReqSer, CsrDspmStreamConfigureReqDes, CsrDspmStreamConfigureReqSerFree},
    {CSR_DSPM_STREAM_SYNC_REQ, CsrDspmStreamSyncReqSizeof, CsrDspmStreamSyncReqSer, CsrDspmStreamSyncReqDes, CsrDspmStreamSyncReqSerFree},
    {CSR_DSPM_OPERATOR_CREATE_REQ, CsrDspmOperatorCreateReqSizeof, CsrDspmOperatorCreateReqSer, CsrDspmOperatorCreateReqDes, CsrDspmOperatorCreateReqSerFree},
    {CSR_DSPM_OPERATOR_DESTROY_REQ, CsrDspmOperatorDestroyReqSizeof, CsrDspmOperatorDestroyReqSer, CsrDspmOperatorDestroyReqDes, CsrDspmOperatorDestroyReqSerFree},
    {CSR_DSPM_OPERATOR_MESSAGE_REQ, CsrDspmOperatorMessageReqSizeof, CsrDspmOperatorMessageReqSer, CsrDspmOperatorMessageReqDes, CsrDspmOperatorMessageReqSerFree},
    {CSR_DSPM_OPERATOR_START_REQ, CsrDspmOperatorStartReqSizeof, CsrDspmOperatorStartReqSer, CsrDspmOperatorStartReqDes, CsrDspmOperatorStartReqSerFree},
    {CSR_DSPM_OPERATOR_STOP_REQ, CsrDspmOperatorStopReqSizeof, CsrDspmOperatorStopReqSer, CsrDspmOperatorStopReqDes, CsrDspmOperatorStopReqSerFree},
    {CSR_DSPM_OPERATOR_RESET_REQ, CsrDspmOperatorResetReqSizeof, CsrDspmOperatorResetReqSer, CsrDspmOperatorResetReqDes, CsrDspmOperatorResetReqSerFree},
    {CSR_DSPM_CONNECTION_CREATE_REQ, CsrDspmConnectionCreateReqSizeof, CsrDspmConnectionCreateReqSer, CsrDspmConnectionCreateReqDes, CsrDspmConnectionCreateReqSerFree},
    {CSR_DSPM_CONNECTION_DESTROY_REQ, CsrDspmConnectionDestroyReqSizeof, CsrDspmConnectionDestroyReqSer, CsrDspmConnectionDestroyReqDes, CsrDspmConnectionDestroyReqSerFree},
    {CSR_DSPM_CAPABILITY_DOWNLOAD_REQ, CsrDspmCapabilityDownloadReqSizeof, CsrDspmCapabilityDownloadReqSer, CsrDspmCapabilityDownloadReqDes, CsrDspmCapabilityDownloadReqSerFree},
    {CSR_DSPM_CAPABILITY_REMOVE_REQ, CsrDspmCapabilityRemoveReqSizeof, CsrDspmCapabilityRemoveReqSer, CsrDspmCapabilityRemoveReqDes, CsrDspmCapabilityRemoveReqSerFree},
    {CSR_DSPM_ACTIVATE_CFM, CsrDspmActivateCfmSizeof, CsrDspmActivateCfmSer, CsrDspmActivateCfmDes, CsrDspmActivateCfmSerFree},
    {CSR_DSPM_DEACTIVATE_CFM, CsrDspmDeactivateCfmSizeof, CsrDspmDeactivateCfmSer, CsrDspmDeactivateCfmDes, CsrDspmDeactivateCfmSerFree},
    {CSR_DSPM_DEACTIVATE_IND, CsrDspmDeactivateIndSizeof, CsrDspmDeactivateIndSer, CsrDspmDeactivateIndDes, CsrDspmDeactivateIndSerFree},
    {CSR_DSPM_STREAM_CP_OPEN_CFM, CsrDspmStreamCpOpenCfmSizeof, CsrDspmStreamCpOpenCfmSer, CsrDspmStreamCpOpenCfmDes, CsrDspmStreamCpOpenCfmSerFree},
    {CSR_DSPM_STREAM_CP_CLOSE_CFM, CsrDspmStreamCpCloseCfmSizeof, CsrDspmStreamCpCloseCfmSer, CsrDspmStreamCpCloseCfmDes, CsrDspmStreamCpCloseCfmSerFree},
    {CSR_DSPM_STREAM_SCO_ENABLE_CFM, CsrDspmStreamScoEnableCfmSizeof, CsrDspmStreamScoEnableCfmSer, CsrDspmStreamScoEnableCfmDes, CsrDspmStreamScoEnableCfmSerFree},
    {CSR_DSPM_STREAM_CONFIGURE_CFM, CsrDspmStreamConfigureCfmSizeof, CsrDspmStreamConfigureCfmSer, CsrDspmStreamConfigureCfmDes, CsrDspmStreamConfigureCfmSerFree},
    {CSR_DSPM_STREAM_SYNC_CFM, CsrDspmStreamSyncCfmSizeof, CsrDspmStreamSyncCfmSer, CsrDspmStreamSyncCfmDes, CsrDspmStreamSyncCfmSerFree},
    {CSR_DSPM_OPERATOR_CREATE_CFM, CsrDspmOperatorCreateCfmSizeof, CsrDspmOperatorCreateCfmSer, CsrDspmOperatorCreateCfmDes, CsrDspmOperatorCreateCfmSerFree},
    {CSR_DSPM_OPERATOR_DESTROY_CFM, CsrDspmOperatorDestroyCfmSizeof, CsrDspmOperatorDestroyCfmSer, CsrDspmOperatorDestroyCfmDes, CsrDspmOperatorDestroyCfmSerFree},
    {CSR_DSPM_OPERATOR_MESSAGE_CFM, CsrDspmOperatorMessageCfmSizeof, CsrDspmOperatorMessageCfmSer, CsrDspmOperatorMessageCfmDes, CsrDspmOperatorMessageCfmSerFree},
    {CSR_DSPM_OPERATOR_MESSAGE_IND, CsrDspmOperatorMessageIndSizeof, CsrDspmOperatorMessageIndSer, CsrDspmOperatorMessageIndDes, CsrDspmOperatorMessageIndSerFree},
    {CSR_DSPM_OPERATOR_START_CFM, CsrDspmOperatorStartCfmSizeof, CsrDspmOperatorStartCfmSer, CsrDspmOperatorStartCfmDes, CsrDspmOperatorStartCfmSerFree},
    {CSR_DSPM_OPERATOR_STOP_CFM, CsrDspmOperatorStopCfmSizeof, CsrDspmOperatorStopCfmSer, CsrDspmOperatorStopCfmDes, CsrDspmOperatorStopCfmSerFree},
    {CSR_DSPM_OPERATOR_RESET_CFM, CsrDspmOperatorResetCfmSizeof, CsrDspmOperatorResetCfmSer, CsrDspmOperatorResetCfmDes, CsrDspmOperatorResetCfmSerFree},
    {CSR_DSPM_CONNECTION_CREATE_CFM, CsrDspmConnectionCreateCfmSizeof, CsrDspmConnectionCreateCfmSer, CsrDspmConnectionCreateCfmDes, CsrDspmConnectionCreateCfmSerFree},
    {CSR_DSPM_CONNECTION_DESTROY_CFM, CsrDspmConnectionDestroyCfmSizeof, CsrDspmConnectionDestroyCfmSer, CsrDspmConnectionDestroyCfmDes, CsrDspmConnectionDestroyCfmSerFree},
    {CSR_DSPM_CAPABILITY_DOWNLOAD_CFM, CsrDspmCapabilityDownloadCfmSizeof, CsrDspmCapabilityDownloadCfmSer, CsrDspmCapabilityDownloadCfmDes, CsrDspmCapabilityDownloadCfmSerFree},
    {CSR_DSPM_CAPABILITY_REMOVE_CFM, CsrDspmCapabilityRemoveCfmSizeof, CsrDspmCapabilityRemoveCfmSer, CsrDspmCapabilityRemoveCfmDes, CsrDspmCapabilityRemoveCfmSerFree},

    {0, NULL, NULL, NULL, NULL}
};

void CsrDspmConverterInit(void)
{
    CsrMsgConvInsert(CSR_DSPM_PRIM, csrdspm_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csrdspm_conv_info =
{
    CSR_DSPM_PRIM,
    "CSR_DSPM_PRIM",
    csrdspm_conv_lut
};
CsrLogPrimitiveInformation *CsrDspmTechInfoGet(void)
{
    return &csrdspm_conv_info;
}

#endif
#endif
