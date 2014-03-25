#ifndef CSR_BT_TPT_SRV_H__
#define CSR_BT_TPT_SRV_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_tpt_cmd.h"

/* Applications states */
typedef CsrUint8  CsrBtTptAppStateType;
enum
{
    ENUM_CSR_BT_TPT_STATE_INIT = CSR_BT_TPT_STATUS_BASE,    /* Initializing */
    ENUM_CSR_BT_TPT_STATE_CONNECTABLE,                      /* Attempting to connect to server (or preparing to, i.e. inquiry) */
    ENUM_CSR_BT_TPT_STATE_CONNECTED,                        /* Connected to server */
    ENUM_CSR_BT_TPT_STATE_CONFIGURING,                      /* Server accepted test parameters */
    ENUM_CSR_BT_TPT_STATE_TESTING,                          /* Test is underway */
    ENUM_CSR_BT_TPT_STATE_REPORTING                         /* Test finished and result is received from server */
};

#define CSR_BT_TPT_STATE_INIT                  ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_INIT)
#define CSR_BT_TPT_STATE_CONNECTABLE           ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONNECTABLE)
#define CSR_BT_TPT_STATE_CONNECTED             ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONNECTED)
#define CSR_BT_TPT_STATE_CONFIGURING           ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONFIGURING)
#define CSR_BT_TPT_STATE_TESTING               ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_TESTING)
#define CSR_BT_TPT_STATE_REPORTING             ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_REPORTING)


typedef struct CsrBtTptAvResultTypeag
{
    CsrTime        beginTime;
    CsrTime        endTime;

    CsrUint32    packetCount;
    CsrUint32    incomingBytes;

    CsrUint32    lowRateBufSz;
    CsrUint32    midRateBufSz;
    CsrUint32    highRateBufSz;
} CsrBtTptAvResultType;

typedef struct CsrBtTptSppResultTypeag
{
    CsrTime        beginTime;
    CsrTime        endTime;

    CsrUint32    incomingBytes;
    CsrUint32    packetCount;
    CsrUint32    expectedPacketCount;
} CsrBtTptSppResultType;

typedef struct
{
    CsrUint16    state;
    CsrSchedQid   phandle;
    void        *recvMsgP;

    /* Peer device information. */
    CsrBtDeviceAddr peerAddr;

    /* SPP stuff. */
    CsrSchedQid       spp_qid;    /* SPP service handle */
    CsrUint8   spp_sc;     /* SPP server channel */

    CsrBtTptSppResultType    sppResult;

    /* Expecting an CSR_BT_SPP_DATA_CFM from a previously sent command? */
    CsrBool          cmd_pendingSppAck;

    /* AV stuff. */
    CsrUint8 av_cid;     /* Connection id */
    CsrUint8 av_tlabel;  /* Transaction label ([0; 15]) */
    CsrUint8 av_mysep;   /* Local and remote stream endpoint */
    CsrUint8 av_peersep;
    CsrUint8 av_shandle; /* Stream handle */
    CsrBool  avDeactivated;

    /* SBC encoding */
    CsrBool      runSbcCoding;
    void        *sbcHdl;        /* SBC handle */
    CsrUint16    sbcFramesize;   /* The framesize for the bit pool */
    CsrUint8     *sbcFramePtr;   /* Pointer to the SBC encoded data */
    CsrSchedTid         timerId;


    /* Current test */
    tptTest     curTest;
    CsrBool      test_started;
    CsrUint32    dataReported;
    CsrUint32    remainData;
    CsrBool      sendingResults;
    CsrBool      avResultsSent;
    CsrBool      sppResultsSent;

    CsrTime        *timestamps;    /* Array of timestamps */
    CsrUint32    *seqnos;


    CsrTime            lastTime;
    CsrBtTptSystemInfoType systemInfoClient;     /* System information     */
    CsrBool          closingProgram;
    CsrBool          reActivateOnDisconnect;

} CsrBtTptServerInstanceDataType;


#define CSR_BT_TPT_AV_CLIENT_SEND_START_DELAY   (100000)  /* Delay in micro seconds */
#define CSR_BT_TPT_SBC_ENCODE_DELAY             (22000)   /* Delay in micro seconds */

#define CSR_BT_SBC_CODING_SAMPLE_DATA_SIZE_X    (256)
#define CSR_BT_SBC_CODING_SAMPLE_DATA_SIZE_Y    (2)

#define CSR_BT_TPT_REACTIVATE_SPP_ON_DISCONNECT (0)       /* Set to 0 if the program should exit on test finish */
                                                  /* Otherwise set to 1 */

void csrBtTptAvSbcCoding(CsrUint16 dummy, CsrBtTptServerInstanceDataType *TptData);
void csrBtTptPrepareSbc(CsrBtTptServerInstanceDataType *TptData);

#ifdef __cplusplus
}
#endif
#endif
