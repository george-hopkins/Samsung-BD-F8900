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
#include "csr_am_converter_init.h"
#ifndef EXCLUDE_CSR_AM_MODULE
#include "csr_am_serialize.h"
#include "csr_am_prim.h"

static CsrMsgConvMsgEntry csr_am_conv_lut[] = {
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM
    { CSR_AM_AUDIO_PATH_ADD_SINK_CFM, CsrAmAudioPathAddSinkCfmSizeof, CsrAmAudioPathAddSinkCfmSer, CsrAmAudioPathAddSinkCfmDes, CsrAmAudioPathAddSinkCfmSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
    { CSR_AM_AUDIO_PATH_ADD_SINK_REQ, CsrAmAudioPathAddSinkReqSizeof, CsrAmAudioPathAddSinkReqSer, CsrAmAudioPathAddSinkReqDes, CsrAmAudioPathAddSinkReqSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM
    { CSR_AM_AUDIO_PATH_CONFIG_CFM, CsrAmAudioPathConfigCfmSizeof, CsrAmAudioPathConfigCfmSer, CsrAmAudioPathConfigCfmDes, CsrAmAudioPathConfigCfmSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
    { CSR_AM_AUDIO_PATH_CONFIG_REQ, CsrAmAudioPathConfigReqSizeof, CsrAmAudioPathConfigReqSer, CsrAmAudioPathConfigReqDes, CsrAmAudioPathConfigReqSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM
    { CSR_AM_AUDIO_PATH_CONNECT_CFM, CsrAmAudioPathConnectCfmSizeof, CsrAmAudioPathConnectCfmSer, CsrAmAudioPathConnectCfmDes, CsrAmAudioPathConnectCfmSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
    { CSR_AM_AUDIO_PATH_CONNECT_REQ, CsrAmAudioPathConnectReqSizeof, CsrAmAudioPathConnectReqSer, CsrAmAudioPathConnectReqDes, CsrAmAudioPathConnectReqSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM
    { CSR_AM_AUDIO_PATH_RELEASE_CFM, CsrAmAudioPathReleaseCfmSizeof, CsrAmAudioPathReleaseCfmSer, CsrAmAudioPathReleaseCfmDes, CsrAmAudioPathReleaseCfmSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND
    { CSR_AM_AUDIO_PATH_RELEASE_IND, CsrAmAudioPathReleaseIndSizeof, CsrAmAudioPathReleaseIndSer, CsrAmAudioPathReleaseIndDes, CsrAmAudioPathReleaseIndSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ
    { CSR_AM_AUDIO_PATH_RELEASE_REQ, CsrAmAudioPathReleaseReqSizeof, CsrAmAudioPathReleaseReqSer, CsrAmAudioPathReleaseReqDes, CsrAmAudioPathReleaseReqSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM
    { CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM, CsrAmAudioPathRemoveSinkCfmSizeof, CsrAmAudioPathRemoveSinkCfmSer, CsrAmAudioPathRemoveSinkCfmDes, CsrAmAudioPathRemoveSinkCfmSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ
    { CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ, CsrAmAudioPathRemoveSinkReqSizeof, CsrAmAudioPathRemoveSinkReqSer, CsrAmAudioPathRemoveSinkReqDes, CsrAmAudioPathRemoveSinkReqSerFree },
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */
#ifndef EXCLUDE_CSR_AM_INIT_CFM
    { CSR_AM_INIT_CFM, CsrAmInitCfmSizeof, CsrAmInitCfmSer, CsrAmInitCfmDes, CsrAmInitCfmSerFree },
#endif /* EXCLUDE_CSR_AM_INIT_CFM */
#ifndef EXCLUDE_CSR_AM_INIT_REQ
    { CSR_AM_INIT_REQ, CsrAmInitReqSizeof, CsrAmInitReqSer, CsrAmInitReqDes, CsrAmInitReqSerFree },
#endif /* EXCLUDE_CSR_AM_INIT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrAmConverterInit(void)
{
    CsrMsgConvInsert(CSR_AM_PRIM, csr_am_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_am_conv_info = {
    CSR_AM_PRIM,
    "CSR_AM_PRIM",
    csr_am_conv_lut
};

CsrLogPrimitiveInformation* CsrAmTechInfoGet(void)
{
    return &csr_am_conv_info;
}
#endif
#endif

