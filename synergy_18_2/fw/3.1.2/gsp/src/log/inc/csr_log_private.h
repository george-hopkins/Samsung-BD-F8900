#ifndef CSR_LOG_PRIVATE_H__
#define CSR_LOG_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_prim_defs.h"
#include "csr_log_formats.h"
#include "csr_transport.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------*/
/*  Misc common structures */
/*---------------------------------*/

typedef struct CsrLogLevelTextPending
{
    const CsrCharString           *name;
    CsrLogLevelText                logLevel;
    struct CsrLogLevelTextPending *next;
} CsrLogLevelTextPending;

struct CsrLogTextHandle
{
    CsrLogTextTaskId          taskId;
    const CsrCharString      *originName;
    CsrLogLevelText           logLevel;
    CsrUint16                 subOriginsCount;
    CsrLogSubOrigin          *subOriginsLegacy;
    CsrLogLevelText          *subOriginsLogLevel;
    CsrLogLevelTextPending   *subOriginsLogLevelPending;
    struct CsrLogTextHandle  *next;
    struct CsrLogTextHandle **self;
};

typedef struct csrLogLevelFilterTextTaskSubOrigin
{
    CsrUint16                                  subOrigin;
    CsrLogLevelText                            warningLevel;
    struct csrLogLevelFilterTextTaskSubOrigin *next;
} CsrLogLevelFilterTextTaskSubOrigin;

typedef struct csrLogLevelFilterTextTask
{
    CsrLogTextTaskId                    taskId;
    CsrLogLevelFilterTextTaskSubOrigin *subOrigins;
    CsrLogLevelText                     taskWarningLevel;
    struct csrLogLevelFilterTextTask   *next;
} CsrLogLevelFilterTextTask;

typedef struct
{
    CsrLogLevelFilterTextTask *tasks;
    CsrLogLevelText            allTaskWarningLevel;
} CsrLogLevelFilterText;

typedef struct csrLogLevelFilterTask
{
    CsrSchedQid                   taskId;
    CsrLogLevelTask               taskLogLevel;
    struct csrLogLevelFilterTask *next;
} CsrLogLevelFilterTask;

typedef struct
{
    CsrLogLevelFilterTask *tasks;
    CsrLogLevelTask        allTaskLogLevel;
} CsrLogLevelFilterTasks;


typedef struct
{
    CsrLog *logList;

    CsrLogLevelEnvironment logLevelFilterEnvironment;
    CsrLogLevelFilterTasks logLevelFilterTask;
    CsrLogLevelFilterText  logLevelFilterText;

    CsrLogTextHandle *logTextHandles;
    CsrLogTextTaskId  dynamicTaskId;
} CsrLogDispatchCtx;

extern CsrLogDispatchCtx CsrLogCtx;


/*---------------------------------*/
/*  CSR Synergy logging */
/*---------------------------------*/

typedef struct
{
    CsrUint8       type;                  /* <! Type */
    CsrUint16      prim_type;
    CsrUint8       prim_version;
    CsrPrim        msg_type;
    CsrUint16      state;
    CsrUint32      line;
    CsrCharString *file;
} SynergyExceptionStateEvent;

typedef struct
{
    CsrUint8       type;                  /* <! Type */
    CsrUint16      prim_type;
    CsrUint8       prim_version;
    CsrUint16      state;
    CsrCharString *text;
    CsrUint32      line;
    CsrCharString *file;
} SynergyExceptionGeneral;

typedef struct
{
    CsrUint8       type;                  /* <! Type */
    CsrUint16      prim_type;
    CsrUint8       prim_version;
    CsrUint16      state;
    CsrCharString *text;
    CsrUint32      line;
    CsrCharString *file;
} SynergyExceptionWarning;

typedef struct
{
    CsrUint8  type;               /* <! Type */
    void     *orig_ptr;
    CsrUint16 prim_type;
    CsrUint8  serializer_type;
    void     *msg;                /* <! Payload */
} SynergyCommonSavePopMsg;

typedef SynergyCommonSavePopMsg SynergySaveMsg;

typedef SynergyCommonSavePopMsg SynergyPopMsg;

/*---------------------------------*/
/*  State transition logging */
/*---------------------------------*/
typedef struct
{
    CsrUint8       type;             /* <! Type */
    bitmask16_t    mask;             /* <! Mask indicating which members are present */
    CsrUint32      identifier;       /* <! Application specific identifier, e.g. connection id, handles etc. */
    CsrCharString *fsm_name;         /* <! FSM name */
    CsrUint32      prev_state;       /* <! Previous state */
    CsrCharString *prev_state_str;   /* <! Next state */
    CsrUint32      in_event;         /* <! event causing this transition */
    CsrCharString *in_event_str;     /* <! Next state */
    CsrUint32      next_state;       /* <! Next state */
    CsrCharString *next_state_str;   /* <! Next state */
    CsrUint32      line;             /* <! line where the transition tooks place */
    CsrCharString *file;             /* <! file where the transition tooks place */
} SynergyStateTransition;

/*---------------------------------*/
/*  Text logging */
/*---------------------------------*/
typedef struct
{
    CsrUint8       type;     /* <! Type */
    CsrCharString *text;     /* <! Text */
    CsrUint32      line;     /* <! Optional: line where the transition tooks place */
    CsrCharString *file;     /* <! Optional: file where the transition tooks place */
} SynergyTaskText;

typedef struct
{
    CsrUint8         event;      /* <! Event */
    CsrLogTextTaskId taskId;     /* <! taskId */
    CsrCharString   *taskName;   /* <! TaskName */
    CsrUint16        subOriginsLen; /* <! Number of Sub origins */
    CsrLogSubOrigin *subOrigins; /* <! Optional: name and identifier of each subOrigin */
} SynergyTextRegister;

typedef struct
{
    CsrUint8         event;      /* <! Event */
    CsrLogTextTaskId taskId;     /* <! TaskId */
    CsrUint16        subOrigin;  /* <! SubOrigin */
    CsrLogLevelText  level;      /* <! warningLevel */
    CsrSize          textLength; /* <! Length of text */
    CsrCharString   *text;       /* <! Text */
    CsrSize          bufferLength; /* <! Length of buffer */
    const CsrUint8  *buffer;     /* <! Buffer */
} SynergyTextPrint;

/*---------------------------------*/
/*  Test logging */
/*---------------------------------*/
typedef struct
{
    CsrUint8       type;     /* <! Type */
    CsrUint8       verdict;  /* <! Verdict */
    CsrCharString *text;         /* <! Text */
    CsrUint32      line;     /* <! Optional: line where the transition tooks place */
    CsrCharString *file;         /* <! Optional: file where the transition tooks place */
} SynergyTestVerdict;

typedef struct
{
    CsrUint8 channel;            /* <! Channel type */
    CsrBool  received;
    /* CsrSize payload_length; */
    void *payload;           /* <! Payload */
} SynergyBtiTrans;

typedef struct
{
    CsrUint8 channel;            /* <! Channel type */
    /* CsrSize payload_length; */
    void *payload;           /* <! Payload */
} SynergyHci;

typedef struct
{
    CsrUint8  transport;
    CsrUint8  direction;
    CsrUint32 consumed_length;
    CsrUint32 payload_length;
    void     *payload;       /* <! Payload */
} SynergyTrans;

typedef struct
{
    CsrUint8       protocol;      /* <! Protocol */
    CsrUint8       event;
    CsrUint32      line;
    CsrCharString *file;
} SynergyProtocol;

/*---------------------------------*/
/*  Scheduler logging */
/*---------------------------------*/
#define SYNERGY_SCHEDULER_TYPE_INIT                      (0x0000)
#define SYNERGY_SCHEDULER_TYPE_DEINIT                    (0x0001)
#define SYNERGY_SCHEDULER_TYPE_START                     (0x0002)
#define SYNERGY_SCHEDULER_TYPE_STOP                      (0x0003)
#define SYNERGY_SCHEDULER_TYPE_INIT_TASK                 (0x0004)
#define SYNERGY_SCHEDULER_TYPE_DEINIT_TASK               (0x0005)
#define SYNERGY_SCHEDULER_TYPE_ACTIVATE_TASK             (0x0006)
#define SYNERGY_SCHEDULER_TYPE_DEACTIVATE_TASK           (0x0007)
#define SYNERGY_SCHEDULER_TYPE_PUT_MESSAGE               (0x0008)
#define SYNERGY_SCHEDULER_TYPE_GET_MESSAGE               (0x0009)
#define SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_IN            (0x000A)
#define SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_FIRE          (0x000B)
#define SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_DONE          (0x000C)
#define SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_CANCEL        (0x000D)
#define SYNERGY_SCHEDULER_TYPE_BGINT_REGISTER            (0x000E)
#define SYNERGY_SCHEDULER_TYPE_BGINT_UNREGISTER          (0x000F)
#define SYNERGY_SCHEDULER_TYPE_BGINT_SET                 (0x0010)
#define SYNERGY_SCHEDULER_TYPE_BGINT_SERVICE_START       (0x0011)
#define SYNERGY_SCHEDULER_TYPE_BGINT_SERVICE_DONE        (0x0012)
#define SYNERGY_SCHEDULER_TYPE_TECHINFO                  (0x0013)
#define SYNERGY_SCHEDULER_TYPE_PLATFORMINFO              (0x0014)

/*---------------------------------*/
/*  Exception */
/*---------------------------------*/
#define SYNERGY_EXCEPTION_TYPE_STATE_EVENT               (0x0015)
#define SYNERGY_EXCEPTION_TYPE_GENERAL                   (0x0016)
#define SYNERGY_EXCEPTION_TYPE_WARNING                   (0x0017)

/*---------------------------------*/
/*  Save message */
/*---------------------------------*/
#define SYNERGY_SAVE_POP_MSG_TYPE_SAVE                   (0x0018)
#define SYNERGY_SAVE_POP_MSG_TYPE_POP                    (0x0019)

/*---------------------------------*/
/*  BlueCore Transport Interface logging */
/*---------------------------------*/
#define SYNERGY_BTI_TRANS_TYPE_BCCMD                     (0x002c)
#define SYNERGY_BTI_TRANS_TYPE_HQ                        (0x002d)
#define SYNERGY_BTI_TRANS_TYPE_DM                        (0x002e)
#define SYNERGY_BTI_TRANS_TYPE_HCI                       (0x002f)
#define SYNERGY_BTI_TRANS_TYPE_ACL                       (0x0030)
#define SYNERGY_BTI_TRANS_TYPE_SCO                       (0x0031)
#define SYNERGY_BTI_TRANS_TYPE_L2CAP                     (0x0032)
#define SYNERGY_BTI_TRANS_TYPE_RFCOMM                    (0x0033)
#define SYNERGY_BTI_TRANS_TYPE_SDP                       (0x0034)
#define SYNERGY_BTI_TRANS_TYPE_DFU                       (0x0035)
#define SYNERGY_BTI_TRANS_TYPE_VM                        (0x0036)

/*---------------------------------*/
/*  BlueCore Transport logging */
/*---------------------------------*/
#define SYNERGY_TRANSPORT_TYPE_BCSP                      (0x0037)
#define SYNERGY_TRANSPORT_TYPE_USB                       (0x0038)
#define SYNERGY_TRANSPORT_TYPE_H4_UART                   (0x0039)
#define SYNERGY_TRANSPORT_TYPE_H5_THREE_WIRE             (0x003a)
#define SYNERGY_TRANSPORT_TYPE_H4_DS                     (0x003b)
#define SYNERGY_TRANSPORT_TYPE_TYPE_A                    (0x003c)
#define SYNERGY_TRANSPORT_TYPE_PIPE                      (0x003d)


/*---------------------------------*/
/*  State logging */
/*---------------------------------*/
#define SYNERGY_TASK_BGINT_EXT_TYPE_STATE                (0x0022)

/*---------------------------------*/
/*  Text logging */
/*---------------------------------*/
#define SYNERGY_TASK_BGINT_EXT_TYPE_TASK_TEXT            (0x0023)
#define SYNERGY_TEXT_LEVEL_EVENT_REGISTER                (0x001A)
#define SYNERGY_TEXT_LEVEL_EVENT_PRINT                   (0x001B)

/*---------------------------------*/
/*  Test logging */
/*---------------------------------*/
#define SYNERGY_TEST_VERDICT                             (0x002b)

typedef struct
{
    CsrUint16            segment;
    CsrSchedQid          queue;
    CsrUint16            task_id;
    const CsrCharString *task_id_ver;
    CsrUint8             task_inst;
    const CsrCharString *task_name;
} SynergySchedulerInitTaskInfo;

typedef struct
{
    CsrUint8 type;           /* <! Type */
    CsrUint8 thread_id;
} SynergySchedulerInit;

typedef struct
{
    CsrUint8 type;           /* <! Type */
    CsrUint8 thread_id;
} SynergySchedulerDeinit;

typedef struct
{
    CsrUint8 type;           /* <! Type */
    CsrUint8 thread_id;
} SynergySchedulerStart;

typedef struct
{
    CsrUint8 type;           /* <! Type */
    CsrUint8 thread_id;
} SynergySchedulerStop;

typedef struct
{
    CsrUint8  type;          /* <! Type */
    CsrUint16 task_id;
    CsrUint8  task_id_version;
    CsrUint8  task_inst;
} SynergySchedulerCommonTask;

typedef SynergySchedulerCommonTask SynergySchedulerDeinitTask;

typedef SynergySchedulerCommonTask SynergySchedulerActivateTask;

typedef struct
{
    CsrUint8             type; /* <! Type */
    CsrUint8             thread_id;
    CsrUint16            task_id;
    const CsrCharString *tskName;
} SynergySchedulerInitTask;

typedef struct
{
    CsrUint8  type;              /* <! Type */
    CsrUint16 task_id;
    CsrUint8  task_id_version;
    CsrUint8  task_inst;
    CsrTime   execute_time;
    void     *instdata;              /* <! Payload */
} SynergySchedulerDeactivateTask;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint32     line;
    CsrCharString file;
    CsrUint16     src_task_id;
    CsrUint8      src_task_id_version;
    CsrUint8      src_task_inst;
    CsrUint16     dst_task_id;
    CsrUint8      dst_task_id_version;
    CsrUint8      dst_task_inst;
    CsrSchedMsgId msg_id;
    CsrUint16     prim_type;
    CsrUint8      prim_version;
    CsrUint8      serializer_type;
    void         *msg;           /* <! Message payload */
} SynergySchedulerPutMessage;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint16     src_task_id;
    CsrUint8      src_task_id_version;
    CsrUint8      src_task_inst;
    CsrUint16     dst_task_id;
    CsrUint8      dst_task_id_version;
    CsrUint8      dst_task_inst;
    CsrBool       get_res;
    CsrSchedMsgId msg_id;
    CsrUint16     prim_type;
    CsrUint8      prim_version;
    CsrUint8      serializer_type;
    void         *msg;         /* <! Message payload */
} SynergySchedulerGetMessage;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint32     line;
    CsrCharString file;
    CsrUint16     task_id;
    CsrUint8      task_id_version;
    CsrUint8      task_inst;
    CsrSchedTid   csrTid;
    CsrTime       requested_delay;
    CsrUint16     fniarg;
    void         *fnvarg;           /* <! Payload */
} SynergySchedulerTimedEventIn;

typedef struct
{
    CsrUint8    type;        /* <! Type */
    CsrUint16   task_id;
    CsrUint8    task_id_version;
    CsrUint8    task_inst;
    CsrSchedTid csrTid;
    CsrTime     requested_delay;
    CsrTime     actual_delay;
} SynergySchedulerTimedEventFire;

typedef struct
{
    CsrUint8    type;        /* <! Type */
    CsrUint16   task_id;
    CsrUint8    task_id_version;
    CsrUint8    task_inst;
    CsrSchedTid csrTid;
    CsrTime     requested_delay;
    CsrTime     actual_delay;
    CsrTime     execute_time;
} SynergySchedulerTimedEventDone;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint32     line;
    CsrCharString file;
    CsrUint16     task_id;
    CsrUint8      task_id_version;
    CsrUint8      task_inst;
    CsrSchedTid   csrTid;
    CsrBool       cancel_res;
    CsrTime       requested_delay;
    CsrTime       actual_delay;
} SynergySchedulerTimedEventCancel;

typedef struct
{
    CsrUint8       type;          /* <! Type */
    CsrUint8       thread_id;
    CsrSchedBgint  irq;
    CsrCharString *callback;
    void          *ptr;           /* <! Pointer */
} SynergySchedulerBgintRegister;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint16     id;
    CsrUint8      id_version;
    CsrSchedBgint irq;
} SynergySchedulerBgintSetUnregisterCommon;

typedef SynergySchedulerBgintSetUnregisterCommon SynergySchedulerBgintSet;

typedef SynergySchedulerBgintSetUnregisterCommon SynergySchedulerBgintUnregister;

typedef struct
{
    CsrUint8      type;           /* <! Type */
    CsrUint16     id;
    CsrUint8      id_version;
    CsrSchedBgint irq;
    /*  CsrSize     ptr_length; */
    void *ptr;                   /* <! Pointer */
} SynergySchedulerBgintServiceCommon;

typedef SynergySchedulerBgintServiceCommon SynergySchedulerBgintServiceStart;

typedef SynergySchedulerBgintServiceCommon SynergySchedulerBgintServiceDone;

typedef struct
{
    CsrUint8 little_endian;
    CsrUint8 pointer_size;
    CsrUint8 alignment;
} SynergyHeader;

typedef struct
{
    CsrUint8             hci_type;  /* <! Use reserved H4 HCI type */
    const CsrCharString *version;   /* <! Version of header */
    SynergyHeader        hdr;
} SynergyVerHeader;

/*---------------------------------*/
/*  PCAP format */
/*---------------------------------*/
typedef struct
{
    CsrUint32 magic_number;   /* magic number */
    CsrUint16 version_major;  /* major version number */
    CsrUint16 version_minor;  /* minor version number */
    CsrInt32  thiszone;       /* GMT to local correction */
    CsrUint32 sigfigs;        /* accuracy of timestamps */
    CsrUint32 snaplen;        /* CSRMAX length of captured packets, in octets */
    CsrUint32 network;        /* data link type */
} PcapHdr;

#define PCAP_HDR_SIZEOF                 (4 + 2 + 2 + 4 + 4 + 4 + 4)

typedef struct
{
    CsrUint32 ts_sec;         /* timestamp seconds */
    CsrUint32 ts_usec;        /* timestamp microseconds */
    CsrUint32 incl_len;       /* number of octets of packet saved in file */
    CsrUint32 orig_len;       /* actual length of packet */
    CsrUint32 received;       /* direction */
} PcapRecHdr;

#define PCAP_REC_HDR_SIZEOF             (4 + 4 + 4 + 4)
#define PCAP_REC_HDR_OFFSET_INCL_LEN    (4 + 4)

#ifdef __cplusplus
}
#endif

#endif
