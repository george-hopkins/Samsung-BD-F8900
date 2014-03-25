#ifndef CSR_AM_HANDLER_H__
#define CSR_AM_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_bccmd_prim.h"
#include "csr_am_prim.h"
#include "csr_list.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrAmLto);

/* General Var ID's */
#define BCCMDVARID_CHIP_VER                 ((CsrUint16) 0x281A)
#define BCCMDVARID_BUILD_ID                 ((CsrUint16) 0x2819)

/* Stream related stuff */
#define BCCMDVARID_STREAM_SOURCE            ((CsrUint16) 0x505A)
#define BCCMDVARID_STREAM_SINK              ((CsrUint16) 0x505B)
#define BCCMDVARID_STREAM_CONFIGURE         ((CsrUint16) 0x505C)
#define BCCMDVARID_STREAM_SINK_ALIAS        ((CsrUint16) 0x505D)
#define BCCMDVARID_STREAM_CONNECT           ((CsrUint16) 0x505E)
#define BCCMDVARID_STREAM_SYNC_SID          ((CsrUint16) 0x5062)

#define BCCMDVARID_STREAM_TRANS_START       ((CsrUint16) 0x4020)
#define BCCMDVARID_STREAM_TRANS_COMPLETE    ((CsrUint16) 0x4021)
#define BCCMDVARID_STREAM_TRANS_CANCEL      ((CsrUint16) 0x4022)
#define BCCMDVARID_STREAM_CLOSE_SOURCE      ((CsrUint16) 0x486B)
#define BCCMDVARID_STREAM_CLOSE_SINK        ((CsrUint16) 0x486C)
#define BCCMDVARID_STREAM_TRANS_DISCONNECT  ((CsrUint16) 0x486D)

#define STREAM_TYPE_NO_ENDPOINT           ((CsrUint16) 0xFFFF)
#define STREAM_ID_NONE                    ((CsrUint16) 0xFFFF)
#define AM_PRIM_ID_NONE                   ((CsrUint16) 0xFFFF)
#define AM_VALUE_UNKNOWN                  ((CsrUint16) 0xFFFF)

typedef CsrUint32 AmState;
typedef CsrUint32 AmStreamState;

/* AM States*/
#define AM_INIT_STATE                     ((AmState) 0x00)
#define AM_IDLE_STATE                     ((AmState) 0x01)
#define AM_BUSY_STATE                     ((AmState) 0x02)
#define AM_DEACTIVATING_STATE             ((AmState) 0x03)

/* AM Stream states */
#define AM_STREAM_INITIALISED_STATE              ((AmStreamState) 0x00)
#define AM_STREAM_CONNECTED_STATE                ((AmStreamState) 0x01)
#define AM_STREAM_ACTIVE_STATE                   ((AmStreamState) 0x02)


/* Jemima A12 is the first build Id supporting audio streams, though only for FM audio */
#define FIRST_AUDIO_STREAM_BUILD_ID                     ((CsrUint16) 5985)


/* Audio manager endpoint configuration type */
typedef struct
{
    CsrUint32 key;
    CsrUint32 value;
} AmEpConfigType;

/* An endpoint consists of at least one endpoint Id and
   at most two endpoint Ids in the case of stereo audio  */
typedef struct EndpointTypeTag
{
    struct EndpointTypeTag *next;
    struct EndpointTypeTag *prev;
    CsrUint16               amStreamAudioEndPointL;
    CsrUint16               amStreamAudioEndPointR;
    CsrAmEndPointDefType    amStreamAudioEndPointType;
    CsrAmEndPointDefType    amStreamAudioEndPointTypeR;
    CsrUint8                amStreamAudioEndPointConfigLen;
    AmEpConfigType         *amStreamAudioEndPointConfig;
    CsrBool                 amStreamAudioEndPointSynced;
    CsrBool                 amStreamAudioEndPointAliased;
    CsrBool                 amStreamAudioEndPointAliasedR;
    CsrBool                 amStreamAudioEndPointLConfigDone;
} EndpointType;


/* A stream is defined by a state, a source endpoint, one or more sink endpoints,
   one or two connection ids towards the chip, one connection Id towards the app
   which will not change throughout the lifetime of the stream and a list of configuration
   values */


typedef void (amCallBackFunctionType)(void *);

typedef struct StreamTypeTag
{
    struct StreamTypeTag *next;                    /* pointer to the next stream instance if any */
    struct StreamTypeTag *prev;                    /* pointer to the previous stream instance if any */
    AmStreamState         amStreamState;           /* state of the stream: initialised, connected, active */
    CsrBool               amStereo;                /* if TRUE, the stream demands stereo audio */
    CsrAmAudioType        amStreamType;            /* type of audio stream: FM, WBS, CVSD, Auristream,... */
    EndpointType          amSource;                /* Source Id(s) of the stream */
    EndpointType         *amSinkList;              /* List of sink IDs and sink types of the stream */
    CsrCmnList_t          sinkList;                /* List of actual streams (StreamType) */
    CsrUint16             amStreamTransformIdL;    /* If mono, only this is relevant */
    CsrUint16             amStreamTransformIdR;    /* Only used if stereo */
    CsrUint16             amAppStreamConnectionId; /* Unique connection ID to be used between the application and the AM */
    CsrSchedQid           amConnectionOwner;       /* the handle of the application that created the stream */
} StreamType;


/* AM instance*/
typedef struct
{
    AmState                 myState;                /* the state of the AM task */
    CsrSchedQid             currentAppHandle;       /* handle to send confirmation to */
    CsrUint16               msgClass;               /* Current message class */
    void                   *rxMsgP;                 /* the received message */
    void                   *rxDownstreamMsgP;       /* the downstream message being handled */
    CsrBool                 restoreFlag;            /* message from queue */
    CsrUint16               chipVer;                /* the BlueCore chip version */
    CsrUint16               buildId;                /* the BlueCore chip firmware build Id */
    CsrAmPrim               requestType;            /* type of the current request */
    CsrMessageQueueType    *saveQueue;              /* queue of saved messages */
    StreamType             *currentStream;          /* Stream being handled */
    CsrCmnList_t            streamList;             /* List of actual streams (StreamType) */
    amCallBackFunctionType *amCallback;             /* Call back function to use upon reception of answer from the chip */
    CsrUint16               connId;                 /* connection Id of the stream being handled */
    CsrBool                 streamTransOpen;        /* Stream transaction open if TRUE, closed if FALSE */
    EndpointType           *sinksToAdd;             /* pointer to the sink endpoint(s) to add */
    CsrCmnList_t            newSinkList;            /* List with the sinks to add */
    CsrUint8                sinkAddedcount;         /* Counter to keep track of what sink(s) of the list have been allocated so far */
    CsrBool                 streamTransSupported;   /* Stream transaction supported if TRUE, not supported if FALSE */
} AmInstanceDataType;


/* Prototype for AM handler */
typedef void (*AmHandlerType)(AmInstanceDataType *inst);


void CsrAmIssueAmGetChipVer(void);
void CsrAmIssueGetBuildId(void);
void CsrAmIssueBccmdSetSource(CsrUint16 source, CsrUint16 instance, CsrUint16 channel);
void CsrAmIssueBccmdSetSink(CsrUint16 sink, CsrUint16 instance, CsrUint16 channel);
void CsrAmIssueBccmdCloseSource(CsrUint16 sourceId);
void CsrAmIssueBccmdCloseSink(CsrUint16 sinkId);
void CsrAmIssueBccmdStreamTransStart(void);
void CsrAmIssueBccmdStreamTransComplete(void);
void CsrAmIssueBccmdStreamConnect(CsrUint16 src, CsrUint16 snk);
void CsrAmIssueBccmdStreamSnkAlias(CsrUint16 snk1, CsrUint16 snk2);
void CsrAmIssueBccmdStreamSyncEndpoints(CsrUint16 ep1, CsrUint16 ep2);
void CsrAmIssueBccmdStreamDisconnect(CsrUint16 streamId);
void CsrAmIssueBccmdConfigStream(CsrUint16 sid, CsrUint32 key, CsrUint32 value);

/* Prototype for AM downstream message free */
void CsrAmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);


void CsrAmChangeState(CsrCharString *file, CsrUint32 lineno, CsrUint32 *state, CsrCharString *stateName, CsrUint32 newState, CsrCharString *newStateName);
#define CSR_AM_CHANGE_STATE(state, newState)      CsrAmChangeState(__FILE__, __LINE__, &(state), #state, (newState), #newState)

#ifdef __cplusplus
}
#endif

#endif /* CSR_AM_HANDLER_H__ */
