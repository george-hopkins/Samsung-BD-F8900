#ifndef CSR_AC97_DATA_H__
#define CSR_AC97_DATA_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <csr_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AC97_STATE_DISABLE      0
#define AC97_STATE_TRANSFER     1
#define AC97_STATE_PAUSE        2

typedef struct FrameBufferPcm
{
    CsrUint32               length;
    CsrUint32              *data;
    CsrUint32              *current;
    AC97PcmCallbackFunction callback;
    struct FrameBufferPcm  *next;
    CsrUint8                paused;
} FrameBufferPcm;

typedef struct FrameBufferMic
{
    CsrUint32               length;
    CsrUint16              *data;
    CsrUint16              *current;
    AC97MicCallbackFunction callback;
    struct FrameBufferMic  *next;
    CsrUint8                paused;
} FrameBufferMic;

#define AC97_FRAMEBUFFER_SLOTS      4

#ifdef __cplusplus
}
#endif

#endif
