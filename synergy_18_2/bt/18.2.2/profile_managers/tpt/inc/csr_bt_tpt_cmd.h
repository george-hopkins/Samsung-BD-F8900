#ifndef CSR_BT_TPT_CMD_H__
#define CSR_BT_TPT_CMD_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define TPT_CMD_MAX_PAYLOAD_LEN         (500)

struct TptCommand_tag {
    CsrUint32 cmdOpcode; /* TPT_CMD_... */
    CsrUint32 testType;

    CsrUint32 paramLen;  /* Payload length in bytes. */
    void     *paramPtr;
};
typedef struct TptCommand_tag TptCommand_t;

/* Test parameter structures */
struct tptSppTestParams_tag {
    CsrUint32 packetCount;
    CsrUint16 packetSize;
    CsrUint16 sppFlags; /* TPT_TESTFLAG_... */
};
typedef struct tptSppTestParams_tag tptSppTestParams;

struct tptAvTestParams_tag {
    CsrUint32 packetCount;
    CsrUint16 packetSize;
    CsrUint16 avFlags; /* TPT_TESTFLAG_... */
};
typedef struct tptAvTestParams_tag tptAvTestParams;

/* Test run structures not needed:  Will have empty payload. */
struct tptReportResult_tag {
    CsrUint32   resultCount;
    CsrTime       *timeStamp; /* Buffer of resultCount timestamps of deltaT between packets. */
    CsrUint32   *seqno;
};
typedef struct tptReportResult_tag tptReportResult;

/* TPT command opcode definitions */
#define TPT_CMD_SETUP_REQ   0x01    /* Configure test parameters */
#define TPT_CMD_SETUP_CFM   0x02    /* Acknowledge test parameters and ready to test */

#define TPT_CMD_TESTRUN_REQ 0x03    /* Client signals intention to start test/readiness for test run */
#define TPT_CMD_TESTRUN_CFM 0x0a    /* Server confirms successful test run */

#define TPT_CMD_RESULT_REQ  0x0b    /* Client requests server side test results */
#define TPT_CMD_RESULT_IND  0x0d    /* Server returns server side test results */
#define TPT_CMD_RESULT_CFM  0x0f    /* Server finished returning test results */


/* TPT test type definitions */
#define TPT_TEST_SPP    0x1
#define TPT_TEST_AV     0x2

/* TPT test direction */
#define TPT_TESTDIR_MASK    16
#define TPT_TESTDIR_CS      (0x1 << TPT_TESTDIR_MASK)
#define TPT_TESTDIR_SC      (0x2 << TPT_TESTDIR_MASK)

/* Specific test definitions */
#define CSR_BT_TPT_TEST_SPP_CS (TPT_TEST_SPP | (TPT_TESTDIR_CS))
#define CSR_BT_TPT_TEST_SPP_SC (TPT_TEST_SPP | (TPT_TESTDIR_SC))
#define CSR_BT_TPT_TEST_AV_CS  (TPT_TEST_AV | (TPT_TESTDIR_CS))
#define CSR_BT_TPT_TEST_AV_SC  (TPT_TEST_AV | (TPT_TESTDIR_SC))

/* Test flags */
#define TPT_TESTFLAG_NONE       0x0
#define TPT_TESTFLAG_INQUIRY    0x1
#define TPT_TESTFLAG_DOSBC      0x2

struct tptTest_tag {
    CsrUint32 testType;      /* Type of test (TPT_TEST_...) */
    CsrUint16 packetSize;    /* Packet size in bytes */
    CsrUint32 packetCount;   /* Number of packets to be sent/received */
    CsrUint32 packetsDone;   /* Packets sent/received */
    CsrUint16 testFlags;     /* TPT_TESTFLAG_.. */
    char     *logName;      /* Name of test log file -- only used by client */
};
typedef struct tptTest_tag tptTest;

/* TPT abort error codes */
#define TPT_ERR_UNSPEC    0x0 /* Unspecified error */
#define TPT_ERR_INVALID    0x1 /* Invalid parameter encountered */
#define TPT_ERR_NOMEM    0x2 /* Unable to allocate memory */


/* Security settings */
#define PASSKEY         "0000"
#define PASSKEY_LEN     4


#ifdef __cplusplus
}
#endif

#endif /* !TPT_CMD_H__ */
