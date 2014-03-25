#ifndef CSR_SCHED_TEST_H__
#define CSR_SCHED_TEST_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Instance placeholders */
typedef struct
{
    int tc_no;
    void (*srv_tc_start)(void **);
    int (*srv_tc_handle)(void **);
    void (*cli_tc_start)(void **);
    int (*cli_tc_handle)(void **);
    char tc_name[32];
} CsrCoalTcDefs;

extern CsrCoalTcDefs ts[];
extern int number_testcases;

typedef struct
{
    CsrSchedTid   timerID1;
    CsrSchedTid   timerID2;
    CsrSchedTid   timerID3;
    CsrSchedTid   timerID4;
    CsrSchedMsgId msgID1;
    CsrSchedMsgId msgID2;
    CsrSchedMsgId msgID3;
    CsrSchedMsgId msgID4;

    CsrUint32 firstTimerCounter;
    CsrUint32 secondTimerCounter;
    CsrUint32 thirdTimerCounter;
} Test1InstanceDataType;

typedef struct
{
    CsrUint16 bgint_mask;
    CsrUint16 count;
    CsrBool   gotBgint;
    CsrBool   broadcast_received;
    CsrBool   stop;

    CsrUint16 send_task;
    CsrUint16 send_prim;
    void     *send_data;

    CsrSchedBgint testBgint1;
    CsrSchedBgint testBgint2;
    CsrSchedBgint testBgint3;
    CsrSchedBgint testBgint4;
    CsrSchedBgint testBgint5;
    CsrSchedBgint testBgint6;
    CsrSchedBgint testBgint7;
    CsrSchedBgint testBgint8;
} BgintTestInstanceData;

/* Tasks */
extern CsrSchedQid TEST_TASK1_IFACEQUEUE;
extern CsrSchedQid TEST_TASK2_IFACEQUEUE;

void task1_init(void **p);
void task1_handler(void **p);
void task1_deinit(void **p);

void task2_init(void **p);
void task2_handler(void **p);
void task2_deinit(void **p);

/* Misc APIs */
int get_tc_no(void);
void testCaseVerboseInformation(int infoCode, char *msg, int msgLength);
void output(char *txt);

/* Testcase verdicts etc */
#define TC_NOT_RUNNED              0
#define TC_PASSED                  1
#define TC_INCONCLUSIVE            2
#define TC_FAILED                  3

#define TC_NOT_FINISHED            0
#define TC_FINISHED                1

#define SCHED_SUCCESS              0
#define UNKNOWN_EVENT_CLASS        1
#define UNKNOWN_PRIM_TYPE          2
#define GET_MESSAGE_NOT_ALLOWED    3
#define CANCEL_TIMED_EVENT_FAIL    4
#define CANCEL_PUT_MSG_FAIL        5
#define CANCEL_PUT_MSG_IN_FAIL     6
#define SCHED_FAILED               7

#define TIMED_EVENT                0
#define CANCEL_TIMED_EVENT         1
#define CANCEL_MESSAGE             2
#define GET_MSG                    3
#define INFORMATION_EVENT          4

void dump_ts_results(void);
void tc_timer_must_be_before(CsrTime time, void (*fn)(CsrUint16, void *), CsrUint16 fniarg, void *fnvarg);
void tc_log(int result);
void set_tc_verdict(int result);
void init_ts_verdict(void);
void deinit_ts_verdict(void);

void sendTest1Req(void);
void sendTest2Req(void);
void test_bgint_answer(CsrUint16 t, CsrUint16 i, void *p);
CsrBool test_bgint_register(BgintTestInstanceData *inst);
void test_bgint_unregister(BgintTestInstanceData *inst);

/* Test primitives */
#define SCHED_START_REQ   0x0000
#define TEST1_PRIM        0x0100
#define TEST2_PRIM        0x0101

typedef CsrUint16 TestPrimType;
#define TEST_1_REQ ((TestPrimType) 0x0022)
#define TEST_2_REQ ((TestPrimType) 0x0033)

typedef struct
{
    CsrUint16 type;
    int       tc_no;
} SCHED_START_REQ_t;

typedef struct
{
    TestPrimType type;
} Test1Req;

typedef struct
{
    TestPrimType type;
} Test2Req;

/* Common messages */
#define TIMER1_TIMEOUT           "TIMER1_TIMEOUT"
#define TIMER2_TIMEOUT           "TIMER2_TIMEOUT"
#define TIMER3_TIMEOUT           "TIMER3_TIMEOUT"
#define TIMER1_FINISH            "TIMER1_FINISH"
#define TIMER2_FINISH            "TIMER2_FINISH"
#define TIMER3_FINISH            "TIMER3_FINISH"
#define CANCEL_TIMER1            "CANCEL_TIMER1 WITH SCHED_SUCCESS"
#define CANCEL_TIMER2            "CANCEL_TIMER2 WITH SCHED_SUCCESS"
#define CANCEL_TIMER3            "CANCEL_TIMER3 WITH SCHED_SUCCESS"
#define PLEASE_WAIT              "PLEASE_WAIT"

#ifdef __cplusplus
}
#endif

#endif
