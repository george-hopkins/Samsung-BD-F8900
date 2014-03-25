#ifndef CSR_BT_TPT_PRIM_H__
#define CSR_BT_TPT_PRIM_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* profile manager include files    */
#include "csr_pmem.h"
#include "csr_bt_spp_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_av_lib.h"
#include "csr_sbc_api.h"


/*************************************************************************************
  Function to schedule another TPT run: Send TPT prim with no body.
*************************************************************************************/

#define tptScheduleRun(TptData, newState) do { \
    (TptData)->state = (newState); \
    CsrSchedMessagePut(TESTQUEUE, TPT_PRIM, NULL); \
    } while(0)


/*************************************************************************************
  Event types:
*************************************************************************************/
#define TPT_PRIM     (0xCAFE)

/*************************************************************************************
  Command status:
*************************************************************************************/
#define TPT_TASK_DONE    (0x00)

/* XXX: Remove */
#define TPT_DEFAULT_MAX_FRAME_SIZE (1012)

/*************************************************************************************
  Result status:
*************************************************************************************/
#define NO_STREAM_HOLES                 (0x00)  /* There are NO stream holes in AV stream */
#define STREAM_HOLES                    (0x01)  /* There are stream holes in AV stream */
#define STREAM_HOLES_UNDERRUN           (0x02)  /* one complete run as not been performed */


/*************************************************************************************
  Test setup defines:
*************************************************************************************/
#define MAX_ACTIVATE_TIME               (0)
#define DELAY_BEFORE_GETTING_INSTANCES  (1000000)
#define MAX_DISCOVERY_RESULTS_IN_LIST   (100)
#define TIME_WITH_NO_DATA               (3000000) /* In micro seconds */

#define MAX_NUMBER_OF_TIMER_PACKETS     (10)    /* The CSRMAX - number of timer ticks send to app before reporting error */

#define AV_DATA_DELAY_DEC               (1000)  /* The decrement factor when no holes */
#define DATA_DELAY_START                (15000) /* The startup delay between each interrupt.       */
#define DATA_PACK_FOR_EACH_INT          (2)     /* Number of packets to send for each interrupt    */
#define SINGLE_DATA_PACK_FOR_EACH_INT   (1)     /* Number of packets to send for each interrupt    */
#define INTER_DELAY_PACKET_NUMBERS2SEND ((TPT_DEFAULT_MAX_FRAME_SIZE - 30) / sizeof(CsrTime))
#define INTER_DELAY_PACKET_TO_RETURN    (MAX_NUMBER_TO_SEND)    /* Number of packets to send return*/
#define INTER_DELAY_NUM_GROUPS_NUMBERS  ((INTER_DELAY_PACKET_TO_RETURN / (INTER_DELAY_PACKET_NUMBERS2SEND) + 2))
#define CONNECT_RETRY                   (10)   /* Number of connect retries before failure         */
#define TPT_DELAY_BETWEEN_AV_TESTS      (5000000) /* Delay in micro seconds */
#define TPT_AV_SERVER_SEND_START_DELAY  (100000)  /* Delay in micro seconds */
#define TPT_SBC_ENCODING_START_DELAY    (10000)   /* Delay in micro seconds */
#define TPT_AV_STREAM_ABORT_DELAY       (1000000) /* Delay in micro seconds */
#define TPT_AV_SUSPEND_RESEND_DELAY     (1000000) /* Delay in micro seconds */

#define ASSIGN_TLABEL ((CsrUint8) (TptData->tLabel = (TptData->tLabel + 1) % 16)) /* AV transaction label */

/* AV SBC defines */
#define SBC_MEDIA_CODEC_SC_SIZE         (8)
#define SBC_IE_SAMPLE_FREQ_MASK         ((CsrUint8) 0xF0)
#define SBC_IE_CHANNEL_MODE_MASK        ((CsrUint8) 0x0F)
#define SBC_IE_BLOCK_MASK               ((CsrUint8) 0xF0)
#define SBC_IE_SUBBAND_MASK             ((CsrUint8) 0x0C)
#define SBC_IE_ALLOC_METHOD_MASK        ((CsrUint8) 0x03)
#define SBC_FRAME_TIMING_DELAY          (22000)             /* In micro seconds */
#define SBC_DISPLAY_COUNTDOWN           (20)


/*************************************************************************************
  TPT command defines:
*************************************************************************************/
#define TPT_START_CLIENT_TX                 (0x01)
#define TPT_START_SERVER_TX                 (0x02)
#define TPT_START_STATUS_READ               (0x03)
#define TPT_DATA_PACKET                     (0x0a) /* 0x09, sequence number                                 */
#define TPT_AV_INIT_PROCESS                 (0x0b) /* Set the server into AV ACCEPT for AV connection       */
#define TPT_AV_SERVER_SEND_START_CMD        (0x0d) /* The server sends the test data to the client          */
#define TPT_AV_CLIENT_SEND_START_CMD        (0x0f) /* Setup the client send                                 */
#define TPT_SERVER_SYSTEM_READ              (0x10) /* Reading system information                            */
#define TPT_END_TEST                        (0x12) /* disconnect all connections and put SPP into active    */
#define TPT_AV_STOP_SENDING                 (0x13) /* av stop sending because of received errors            */

#define TPT_TEST_STATUS_INDEX               (12)
#define TPT_AV_AUDIO_DATA_START_INDEX       (13)   /* Index for first "real" audio data byte i.e. first byte after the SBC header */

/*************************************************************************************
  TPT command response defines:
*************************************************************************************/
#define TPT_SPP_ACK_SIGNAL                  (0x80)
#define TPT_SPP_STATUS_RETURN               (0x82)
#define TPT_SERVER_SYSTEM_RES               (0x83) /* response for system read                              */
#define TPT_AV_SERVER_MEASURE_STATUS_RETURN (0x84) /* response for AV server measure                        */


/*************************************************************************************
  TPT status defines:
*************************************************************************************/
#define TPT_ACC_STATUS_PAGE_SCAN_ON         (0x01) /* Enable inq while testing.                             */
#define TPT_ACC_STATUS_SERVER_SBC_ON        (0x02) /* Enable SBC encoder on target                          */
#define TPT_ACC_STATUS_SERVER_SBC_RUN       (0x04) /*                                                       */

#define TPT_STATUS_SPP_CONNECTED            (0x01) /* When SPP is connected                                 */
#define TPT_STATUS_AV_CONNECTED             (0x02) /* When AV is connected                                  */
#define TPT_SERVER_MIN_TIME_MULTIPLY        (2)    /* The CSRMIN time is always min_time_inteval meaured * this value */


/*************************************************************************************
  TPT type defines:
*************************************************************************************/
typedef CsrPrim    TptStatus_t;
#define CSR_BT_TPT_STATUS_BASE (0x0001)

enum
{
    ENUM_TPT_INIT_DONE = CSR_BT_TPT_STATUS_BASE,
    ENUM_TPT_SEARCH_DONE,
    ENUM_TPT_SPP_CONNECT_DONE,

    ENUM_TPT_SERVER_INFO_RD,
    ENUM_TPT_SERVER_INFO_CFM,

    ENUM_TPT_SPP_SERVER_SEND_START,
    ENUM_TPT_SPP_SERVER_SEND,
    ENUM_TPT_SPP_CLIENT_SEND,
    ENUM_TPT_SPP_CLIENT_SEND_START,
    ENUM_TPT_SPP_CLIENT_SEND_SETUP,
    ENUM_TPT_SPP_CLIENT_SEND_PROCESS,

    ENUM_TPT_AV_CONNECTING_INIT,
    ENUM_TPT_SERVER_INFO_CFM_AV,

    ENUM_TPT_AV_CONNECTING,
    ENUM_TPT_AV_CONNECT_DONE,
    ENUM_TPT_AV_SERVER_SEND_START,
    ENUM_TPT_AV_SERVER_SEND_REPORTING_AFTER_SUSPEND,
    ENUM_TPT_AV_SERVER_SEND_SERVER_SETUP,
    ENUM_TPT_AV_SERVER_SEND_STREAM_OPEN,

    ENUM_TPT_AV_SERVER_SEND_SUSPEND,
    ENUM_TPT_AV_SERVER_SEND_SUSPEND2,

    ENUM_TPT_AV_SERVER_SEND_STREAM_START,
    ENUM_TPT_AV_SERVER_SEND_DONE,
    ENUM_TPT_AV_SERVER_SEND_DONE_DELAY,
    ENUM_TPT_AV_CLIENT_SEND_START,
    ENUM_TPT_AV_CLIENT_SEND_SERVER_SETUP,
    ENUM_TPT_AV_CLIENT_SEND_STREAM_START,
    ENUM_TPT_AV_CLIENT_SEND_SUSPENDING,
    ENUM_TPT_AV_CLIENT_SEND_STREAM_OPEN,
    ENUM_TPT_AV_CLIENT_SEND_REPORTING_AGAIN,
    ENUM_TPT_AV_CLIENT_SEND_DONE,
    ENUM_TPT_TEST_REPORTING,
    ENUM_TPT_TEST_FINISHED,
    ENUM_TPT_TEST_FAILURE,
    ENUM_TPT_SEND_AV_DATA
};

#define TPT_INIT_DONE                               ((TptStatus_t)ENUM_TPT_INIT_DONE)
#define TPT_SEARCH_DONE                             ((TptStatus_t)ENUM_TPT_SEARCH_DONE)
#define TPT_SPP_CONNECT_DONE                        ((TptStatus_t)ENUM_TPT_SPP_CONNECT_DONE)

#define TPT_SERVER_INFO_RD                          ((TptStatus_t)ENUM_TPT_SERVER_INFO_RD)
#define TPT_SERVER_INFO_CFM                         ((TptStatus_t)ENUM_TPT_SERVER_INFO_CFM)

#define TPT_SPP_SERVER_SEND_START                   ((TptStatus_t)ENUM_TPT_SPP_SERVER_SEND_START)
#define TPT_SPP_SERVER_SEND                         ((TptStatus_t)ENUM_TPT_SPP_SERVER_SEND)
#define TPT_SPP_CLIENT_SEND                         ((TptStatus_t)ENUM_TPT_SPP_CLIENT_SEND)
#define TPT_SPP_CLIENT_SEND_START                   ((TptStatus_t)ENUM_TPT_SPP_CLIENT_SEND_START)
#define TPT_SPP_CLIENT_SEND_SETUP                   ((TptStatus_t)ENUM_TPT_SPP_CLIENT_SEND_SETUP)
#define TPT_SPP_CLIENT_SEND_PROCESS                 ((TptStatus_t)ENUM_TPT_SPP_CLIENT_SEND_PROCESS)

#define TPT_AV_CONNECTING_INIT                      ((TptStatus_t)ENUM_TPT_AV_CONNECTING_INIT)
#define TPT_SERVER_INFO_CFM_AV                      ((TptStatus_t)ENUM_TPT_SERVER_INFO_CFM_AV)

#define TPT_AV_CONNECTING                           ((TptStatus_t)ENUM_TPT_AV_CONNECTING)
#define TPT_AV_CONNECT_DONE                         ((TptStatus_t)ENUM_TPT_AV_CONNECT_DONE)
#define TPT_AV_SERVER_SEND_START                    ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_START)
#define TPT_AV_SERVER_SEND_REPORTING_AFTER_SUSPEND  ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_REPORTING_AFTER_SUSPEND)
#define TPT_AV_SERVER_SEND_SERVER_SETUP             ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_SERVER_SETUP)
#define TPT_AV_SERVER_SEND_STREAM_OPEN              ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_STREAM_OPEN)

#define TPT_AV_SERVER_SEND_SUSPEND                  ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_SUSPEND)
#define TPT_AV_SERVER_SEND_SUSPEND2                 ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_SUSPEND2)

#define TPT_AV_SERVER_SEND_STREAM_START             ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_STREAM_START)
#define TPT_AV_SERVER_SEND_DONE                     ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_DONE)
#define TPT_AV_SERVER_SEND_DONE_DELAY               ((TptStatus_t)ENUM_TPT_AV_SERVER_SEND_DONE_DELAY)
#define TPT_AV_CLIENT_SEND_START                    ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_START)
#define TPT_AV_CLIENT_SEND_SERVER_SETUP             ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_SERVER_SETUP)
#define TPT_AV_CLIENT_SEND_STREAM_START             ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_STREAM_START)
#define TPT_AV_CLIENT_SEND_SUSPENDING               ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_SUSPENDING)
#define TPT_AV_CLIENT_SEND_STREAM_OPEN              ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_STREAM_OPEN)
#define TPT_AV_CLIENT_SEND_REPORTING_AGAIN          ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_REPORTING_AGAIN)
#define TPT_AV_CLIENT_SEND_DONE                     ((TptStatus_t)ENUM_TPT_AV_CLIENT_SEND_DONE)
#define TPT_TEST_REPORTING                          ((TptStatus_t)ENUM_TPT_TEST_REPORTING)
#define TPT_TEST_FINISHED                           ((TptStatus_t)ENUM_TPT_TEST_FINISHED)
#define TPT_TEST_FAILURE                            ((TptStatus_t)ENUM_TPT_TEST_FAILURE)
#define TPT_SEND_AV_DATA                            ((TptStatus_t)ENUM_TPT_SEND_AV_DATA)


typedef CsrPrim    TptStates_t;
#define TPT_STATES_BASE (0x0001)

enum
{
    ENUM_TPT_WAITING_BEFORE_GETTING_INSTANCES_STATE = TPT_STATES_BASE,
    ENUM_TPT_IDLE_STATE,
    ENUM_TPT_CONNECTING_STATE,
    ENUM_TPT_CONNECTED_STATE,
    ENUM_TPT_PAIRING_STATE,
    ENUM_TPT_DISCONNECTING_STATE,
    ENUM_TPT_INQUIRY_STATE,             /* Start searching for a remote device          */
    ENUM_TPT_CANCEL_SEARCH_STATE,       /* Send stop search command                     */
    ENUM_TPT_SEARCH_DONE_STATE,         /* The server device is found and stop search   */
    ENUM_TPT_NUM_OF_CLIENTS_STATE,
    ENUM_TPT_ENTRY_NUM_STATE,
    ENUM_TPT_SELECT_TPT_INSTANCE_STATE,
    ENUM_TPT_ACTIVATED_STATE,
    ENUM_TPT_DEACTIVATE_STATE,
    ENUM_TPT_INSERT_FILE_NAME_STATE,
    ENUM_TPT_BUSY_STATE,
    ENUM_TPT_SDC_SEARCH_STATE,
    ENUM_TPT_AV_SERVER_SEND_PHASE_STATE,
    ENUM_TPT_AV_CLIENT_SEND_PHASE_STATE
};

#define TPT_WAITING_BEFORE_GETTING_INSTANCES_STATE ((TptStates_t)ENUM_TPT_WAITING_BEFORE_GETTING_INSTANCES_STATE)
#define TPT_IDLE_STATE                             ((TptStates_t)ENUM_TPT_IDLE_STATE)
#define TPT_CONNECTING_STATE                       ((TptStates_t)ENUM_TPT_CONNECTING_STATE)
#define TPT_CONNECTED_STATE                        ((TptStates_t)ENUM_TPT_CONNECTED_STATE)
#define TPT_PAIRING_STATE                          ((TptStates_t)ENUM_TPT_PAIRING_STATE)
#define TPT_DISCONNECTING_STATE                    ((TptStates_t)ENUM_TPT_DISCONNECTING_STATE)
#define TPT_INQUIRY_STATE                          ((TptStates_t)ENUM_TPT_INQUIRY_STATE)
#define TPT_CANCEL_SEARCH_STATE                    ((TptStates_t)ENUM_TPT_CANCEL_SEARCH_STATE)
#define TPT_SEARCH_DONE_STATE                      ((TptStates_t)ENUM_TPT_SEARCH_DONE_STATE)
#define TPT_NUM_OF_CLIENTS_STATE                   ((TptStates_t)ENUM_TPT_NUM_OF_CLIENTS_STATE)
#define TPT_ENTRY_NUM_STATE                        ((TptStates_t)ENUM_TPT_ENTRY_NUM_STATE)
#define TPT_SELECT_TPT_INSTANCE_STATE              ((TptStates_t)ENUM_TPT_SELECT_TPT_INSTANCE_STATE)
#define TPT_ACTIVATED_STATE                        ((TptStates_t)ENUM_TPT_ACTIVATED_STATE)
#define TPT_DEACTIVATE_STATE                       ((TptStates_t)ENUM_TPT_DEACTIVATE_STATE)
#define TPT_INSERT_FILE_NAME_STATE                 ((TptStates_t)ENUM_TPT_INSERT_FILE_NAME_STATE)
#define TPT_BUSY_STATE                             ((TptStates_t)ENUM_TPT_BUSY_STATE)
#define TPT_SDC_SEARCH_STATE                       ((TptStates_t)ENUM_TPT_SDC_SEARCH_STATE)
#define TPT_AV_SERVER_SEND_PHASE_STATE             ((TptStates_t)ENUM_TPT_AV_SERVER_SEND_PHASE_STATE)
#define TPT_AV_CLIENT_SEND_PHASE_STATE             ((TptStates_t)ENUM_TPT_AV_CLIENT_SEND_PHASE_STATE)


typedef CsrPrim          TptGlobalState_t;
#define TPT_GLOBAL_STATE_BASE (0x0001)

enum
{
    ENUM_TPT_IDLE_GLOBAL_STATE = TPT_GLOBAL_STATE_BASE,
    ENUM_TPT_SERVER_SEND_STATE,
    ENUM_TPT_CLIENT_SETUP_STATE,
    ENUM_TPT_CLIENT_SEND_STATE,
    ENUM_TPT_CLIENT_STATUS_READ_STATE,
    ENUM_TPT_AV_INIT_PROCESS_STATE,                 /* State when the AV connection is made.    */
    ENUM_TPT_AV_INIT_CONNECT_STATE,                 /* Connection the AV profile                */
    ENUM_TPT_AV_SERVER_SEND_SERVER_SETUP_STATE,
    ENUM_TPT_AV_SERVER_SEND_AV_STREAM_STATE,
    ENUM_TPT_AV_SERVER_SEND_AV_CONFIG_STATE,
    ENUM_TPT_AV_SERVER_STOP_AV_STREAM_STATE,
    ENUM_TPT_AV_SERVER_SEND_SERVER_SUSPEND_STATE,
    ENUM_TPT_AV_SERVER_SEND_AV_MEASURE_STATE,       /* Data is measured from server             */
    ENUM_TPT_AV_CLIENT_SEND_SERVER_SETUP_STATE,
    ENUM_TPT_AV_CLIENT_SEND_AV_STREAM_STATE,
    ENUM_TPT_AV_CLIENT_SEND_AV_MEASURE_STATE,
    ENUM_TPT_AV_CLIENT_SEND_AV_SETUP_STATE,
    ENUM_TPT_AV_CLIENT_SEND_AV_SUSPEND_STATE,
    ENUM_TPT_TEST_FINISHED_STATE
};

#define TPT_IDLE_GLOBAL_STATE                      ((TptGlobalState_t)ENUM_TPT_IDLE_GLOBAL_STATE)
#define TPT_SERVER_SEND_STATE                      ((TptGlobalState_t)ENUM_TPT_SERVER_SEND_STATE)
#define TPT_CLIENT_SETUP_STATE                     ((TptGlobalState_t)ENUM_TPT_CLIENT_SETUP_STATE)
#define TPT_CLIENT_SEND_STATE                      ((TptGlobalState_t)ENUM_TPT_CLIENT_SEND_STATE)
#define TPT_CLIENT_STATUS_READ_STATE               ((TptGlobalState_t)ENUM_TPT_CLIENT_STATUS_READ_STATE)
#define TPT_AV_INIT_PROCESS_STATE                  ((TptGlobalState_t)ENUM_TPT_AV_INIT_PROCESS_STATE)
#define TPT_AV_INIT_CONNECT_STATE                  ((TptGlobalState_t)ENUM_TPT_AV_INIT_CONNECT_STATE)
#define TPT_AV_SERVER_SEND_SERVER_SETUP_STATE      ((TptGlobalState_t)ENUM_TPT_AV_SERVER_SEND_SERVER_SETUP_STATE)
#define TPT_AV_SERVER_SEND_AV_STREAM_STATE         ((TptGlobalState_t)ENUM_TPT_AV_SERVER_SEND_AV_STREAM_STATE)
#define TPT_AV_SERVER_SEND_AV_CONFIG_STATE         ((TptGlobalState_t)ENUM_TPT_AV_SERVER_SEND_AV_CONFIG_STATE)
#define TPT_AV_SERVER_STOP_AV_STREAM_STATE         ((TptGlobalState_t)ENUM_TPT_AV_SERVER_STOP_AV_STREAM_STATE)
#define TPT_AV_SERVER_SEND_SERVER_SUSPEND_STATE    ((TptGlobalState_t)ENUM_TPT_AV_SERVER_SEND_SERVER_SUSPEND_STATE)
#define TPT_AV_SERVER_SEND_AV_MEASURE_STATE        ((TptGlobalState_t)ENUM_TPT_AV_SERVER_SEND_AV_MEASURE_STATE)
#define TPT_AV_CLIENT_SEND_SERVER_SETUP_STATE      ((TptGlobalState_t)ENUM_TPT_AV_CLIENT_SEND_SERVER_SETUP_STATE)
#define TPT_AV_CLIENT_SEND_AV_STREAM_STATE         ((TptGlobalState_t)ENUM_TPT_AV_CLIENT_SEND_AV_STREAM_STATE)
#define TPT_AV_CLIENT_SEND_AV_MEASURE_STATE        ((TptGlobalState_t)ENUM_TPT_AV_CLIENT_SEND_AV_MEASURE_STATE)
#define TPT_AV_CLIENT_SEND_AV_SETUP_STATE          ((TptGlobalState_t)ENUM_TPT_AV_CLIENT_SEND_AV_SETUP_STATE)
#define TPT_AV_CLIENT_SEND_AV_SUSPEND_STATE        ((TptGlobalState_t)ENUM_TPT_AV_CLIENT_SEND_AV_SUSPEND_STATE)
#define TPT_TEST_FINISHED_STATE                    ((TptGlobalState_t)ENUM_TPT_TEST_FINISHED_STATE)



typedef struct
{
    CsrBtAvPrim            type;
    CsrUint16            status;
} TPT_MESSAGE_T;


typedef struct /* system read */
{
    CsrUint16        commandID;      /* Holds the command identifire             */
    CsrUint16        rfcFrameSize;   /* The framesize for the RFCOMM             */
    CsrUint16        avFrameSize;    /* The L2Cap frame size used for test AV    */
    CsrUint32        uartSpeed;      /* The UART speed available                 */
    CsrUint32        timerPrecision; /* The timer precision                      */
} TptServerSystemRead_t;



typedef struct  /* Holds the reporting data */
{
    CsrUint16        avReportPacketSize;         /* The packet size that has been send                               */
    CsrUint16        avReportPacketNumber;       /* Number of packet that has been send                              */
    CsrUint16        avReportNumPackSize;        /* AV only: mumber of packets that has been send for each interrupt */
    CsrUint16        avReportErrors;             /* show how manny erros has been received                           */
    CsrTime            avReportTime;               /* */

} TptAvReportingData_t;



typedef struct    /* Holds the command structure */
{
    CsrUint16        commandID;              /* Holds the command identifire                                     */
    CsrUint16        parameter1;             /* parameter 1                                                      */
    CsrUint16        parameter2;             /* Parameter 2                                                      */
    CsrUint16        acc_server_status;      /* set the behavioure of the server                                 */

    CsrUint16        avReportPacketSize;     /* The packet size that has been send                               */
    CsrUint16        avReportPacketNumber;   /* Number of packet that has been send                              */
    CsrUint16        avReportNumPackSize;    /* AV only: mumber of packets that has been send for each interrupt */
    CsrUint16        streamStatus;           /* hold the value to indicate holes in stream                       */
    CsrUint16        avInterDelayStart;      /* the start index of the avInterDelay structure                    */
    CsrUint16        avInterDelayLength;     /* Number of packets in avInterDelay bufferen                       */
    CsrUint16        errorCounts;            /* return the number of errors measured                             */
    CsrTime            avReportTime;           /* */
} TptCommandStruct_t;



typedef struct    /* Holds the command structure */
{
    CsrUint16        commandID;              /* Holds the command identifire        */
    CsrUint16        parameter1;             /* parameter 1                        */
    CsrUint16        parameter2;             /* Parameter 2                        */

    CsrUint16        avReportPacketSize;     /* The packet size that has been send */
    CsrUint16        avReportPacketNumber;   /* Number of packet that has been send */
    CsrUint16        avReportNumPackSize;    /* AV only: mumber of packets that has been send for each interrupt*/
    CsrUint16        streamStatus;           /* hold the value to indicate holes in stream */
    CsrUint16        avInterDelayStart;      /* the start index of the avInterDelay structure */
    CsrUint16        avInterDelayLength;     /* Number of packets in avInterDelay bufferen */
    CsrUint16        avErrorCounts;          /* number of errors received from client/server */
    CsrTime            avReportTime;           /* */
    CsrTime            avInterDelay [INTER_DELAY_PACKET_NUMBERS2SEND]; /* transfeer needed AV Inter Delay data to client */
} TptResponseStruct_t;



typedef struct /* Used only for measure data structure*/
{
    CsrUint16                commandID;          /* Holds the command identifire             */
    CsrUint16                parameter1;         /* parameter 1                              */
    CsrUint16                numOfBytesToTest;   /* number of bytes in packet to test        */
    CsrUint16                payloadSize;
    CsrUint8                 *payload;           /* Holds the beginning of test data struct  */
}TptTestDataStruct_t;



typedef struct /* Used only for measure data structure*/
{
    CsrUint16                commandID;          /* Holds the command identifire             */
    CsrUint16                parameter1;         /* parameter 1                              */
    CsrUint16                numOfBytesToTest;   /* number of bytes in packet to test        */
    CsrUint8                 testDataStart;      /* Holds the beginning of test data struct  */
}TptAckStruct_t;



typedef struct /* Used only for measure data structure*/
{
    CsrUint16                rfcFrameSize;       /* The framesize for the RFCOMM             */
    CsrUint16                avFrameSize;        /* The L2Cap frame size used for test av    */
    CsrUint32                timerPrecision;     /* The timer precision                      */
    CsrUint32                uartSpeed;          /* The UART speed available                 */
}CsrBtTptSystemInfoType;


typedef struct
{
    CsrUint8            mediaType;                   /* Holds the structure of the Media header */
    CsrUint8            length;                      /* Length of the structure                  */
} TptConfigStruct_t;


#ifdef __cplusplus
}
#endif

#endif

