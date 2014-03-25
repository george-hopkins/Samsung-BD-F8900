#ifndef CSR_AM_SERIALIZE_H__
#define CSR_AM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrAmPfree(void *ptr);

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM
CsrSize CsrAmAudioPathConfigCfmSizeof(void *msg);
CsrUint8 *CsrAmAudioPathConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathConfigCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM
CsrSize CsrAmAudioPathConnectCfmSizeof(void *msg);
CsrUint8 *CsrAmAudioPathConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathConnectCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM
CsrSize CsrAmAudioPathRemoveSinkCfmSizeof(void *msg);
CsrUint8 *CsrAmAudioPathRemoveSinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathRemoveSinkCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathRemoveSinkCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM
CsrSize CsrAmAudioPathAddSinkCfmSizeof(void *msg);
CsrUint8 *CsrAmAudioPathAddSinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathAddSinkCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathAddSinkCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
CsrSize CsrAmAudioPathConfigReqSizeof(void *msg);
CsrUint8 *CsrAmAudioPathConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathConfigReqDes(CsrUint8 *buffer, CsrSize length);
void CsrAmAudioPathConfigReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ
CsrSize CsrAmAudioPathReleaseReqSizeof(void *msg);
CsrUint8 *CsrAmAudioPathReleaseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathReleaseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathReleaseReqSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM
CsrSize CsrAmAudioPathReleaseCfmSizeof(void *msg);
CsrUint8 *CsrAmAudioPathReleaseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathReleaseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathReleaseCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ
CsrSize CsrAmAudioPathRemoveSinkReqSizeof(void *msg);
CsrUint8 *CsrAmAudioPathRemoveSinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathRemoveSinkReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathRemoveSinkReqSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
CsrSize CsrAmAudioPathConnectReqSizeof(void *msg);
CsrUint8 *CsrAmAudioPathConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrAmAudioPathConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
CsrSize CsrAmAudioPathAddSinkReqSizeof(void *msg);
CsrUint8 *CsrAmAudioPathAddSinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathAddSinkReqDes(CsrUint8 *buffer, CsrSize length);
void CsrAmAudioPathAddSinkReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_REQ
CsrSize CsrAmInitReqSizeof(void *msg);
CsrUint8 *CsrAmInitReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmInitReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmInitReqSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_INIT_REQ */

#ifndef EXCLUDE_CSR_AM_INIT_CFM
CsrSize CsrAmInitCfmSizeof(void *msg);
CsrUint8 *CsrAmInitCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmInitCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmInitCfmSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_INIT_CFM */

#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND
CsrSize CsrAmAudioPathReleaseIndSizeof(void *msg);
CsrUint8 *CsrAmAudioPathReleaseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrAmAudioPathReleaseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrAmAudioPathReleaseIndSerFree CsrAmPfree
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_RELEASE_IND */

#ifdef __cplusplus
}
#endif

#endif
