#ifndef CSR_BT_TPT_APP_H__
#define CSR_BT_TPT_APP_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_platform.h"
#include "csr_bt_tpt_cmd.h"

/* Applications states */
typedef CsrUint8  CsrBtTptAppStateType;
enum
{
    ENUM_CSR_BT_TPT_STATE_INIT = CSR_BT_TPT_STATUS_BASE,  /* Initializing */
    ENUM_CSR_BT_TPT_STATE_CONNECTING,              /* Attempting to connect to server (or preparing to, i.e. inquiry) */
    ENUM_CSR_BT_TPT_STATE_CONNECTED,               /* Connected to server */
    ENUM_CSR_BT_TPT_STATE_CONFIGURING,             /* Configuring and preparing tests */
    ENUM_CSR_BT_TPT_STATE_TESTING,                 /* Test is underway */
    ENUM_CSR_BT_TPT_STATE_REPORTING                /* Test finished and result is received from server */
};

#define CSR_BT_TPT_STATE_INIT                  ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_INIT)
#define CSR_BT_TPT_STATE_CONNECTING            ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONNECTING)
#define CSR_BT_TPT_STATE_CONNECTED             ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONNECTED)
#define CSR_BT_TPT_STATE_CONFIGURING           ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_CONFIGURING)
#define CSR_BT_TPT_STATE_TESTING               ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_TESTING)
#define CSR_BT_TPT_STATE_REPORTING             ((CsrBtTptAppStateType)ENUM_CSR_BT_TPT_STATE_REPORTING)

#define CSR_BT_TPT_MAX_LINE_LENGTH             (100)
#define CSR_BT_TPT_MAX_FILE_NAME_LENGTH        (CSR_BT_TPT_MAX_LINE_LENGTH - 20)

/*************************************************************************************
  Report setup defines:
*************************************************************************************/
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

typedef struct CsrBtTptConfigValuesTypeag
{
    CsrUint16    maxPacketSize;
    CsrUint16    minPacketSize;
    CsrUint16    packetIncrement;
    CsrUint32    maxNoToSend;
    CsrBool      runSppSC;
    CsrBool      runSppCS;
    CsrBool      runAvSC;
    CsrBool      runAvCS;
    CsrBool      runSbcEncode;
    CsrBool      runInquiry;
} CsrBtTptConfigValuesType;

typedef struct
{
    CsrUint16   state;
    CsrSchedQid      phandle;
    void        *recvMsgP;
    CsrBool     configValid;

    /* Connect retrying */
    CsrUint16    retries;

    /* Peer device information. */
    CsrBtDeviceAddr    tptPeerAddr;

    /* SPP stuff. */
    CsrSchedQid       spp_qid;    /* SPP service handle */
    CsrUint8   spp_sc;     /* SPP server channel */

    /* AV stuff. */
    CsrUint8 av_cid;             /* Connection id */
    CsrUint8 av_tlabel;          /* Transaction label ([0; 15]) */
    CsrUint8 av_mysep;           /* Local and remote stream endpoint */
    CsrUint8 av_peersep;         /* Peer stream end point */
    CsrUint8 av_shandle;         /* Stream handle */
    CsrBool  av_open;            /* Is stream opened? */
    CsrBool  testRunCfmReceived; /* Is TPT_CMD_TESTRUN_CFM received yet? */


    /* Current test and test suite. */
    CsrBool      test_configured; /* Is a test configured? */
    tptTest     *tests;
    CsrUint8     numTest;
    CsrUint8     curTest;

    /* File settings */
    char        sppScResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    char        sppCsResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    char        avScResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    char        avCsResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    char        avScDelayResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    char        avCsDelayResultFileName[CSR_BT_TPT_MAX_FILE_NAME_LENGTH + 1];
    CsrFileHandle        *sppScResultFilePtr;
    CsrFileHandle        *avScResultFilePtr;

    /* Report writing */
    CsrBtTptSppResultType   sppResult;
    CsrTime                previousTimeStamp;

    CsrTime                lastTime;
    CsrBtTptSystemInfoType  systemInfoClient;         /* System information */
    char                    updatePrintChar;

} CsrBtTptClientInstanceDataType;


#define CSR_BT_TPT_AV_CLIENT_SEND_START_DELAY  (100000)  /* Delay in micro seconds */
#define CSR_BT_TPT_CONNECT_RETRIES             (3)       /* Number of times to try to re-connect if a connection fails */

#define csrBtTptClientPrint  printf
#ifdef CSR_BT_TPT_DEBUG
#define csrBtTptDebugPrint   csrBtTptClientPrint
#else
/* Don't generate any code for this. */
#define csrBtTptDebugPrint   if (0) printf
#endif

/* Forward declarations */
char* intToAscii(CsrUint32 integer);
void  csrBtTptUpdatePrint(CsrBtTptClientInstanceDataType *TptData);
CsrTime  csrBtTptHpGetTime(void);


#ifdef __cplusplus
}
#endif
#endif
