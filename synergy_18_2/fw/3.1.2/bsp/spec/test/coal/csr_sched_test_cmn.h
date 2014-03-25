#ifndef CSR_SCHED_TEST_CMN_H__
#define CSR_SCHED_TEST_CMN_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void srv_start_tc_put_message1(void **);
int srv_handle_tc_put_message1(void **);

void srv_start_tc_put_message2(void **);
int srv_handle_tc_put_message2(void **);
int cli_handle_tc_put_message2(void **);

void srv_start_tc_timed_event1(void **);
int srv_handle_tc_timed_event1(void **);

void srv_start_tc_timed_event2(void **);
int srv_handle_tc_timed_event2(void **);

void srv_start_tc_timed_event3(void **);
int srv_handle_tc_timed_event3(void **);

void srv_start_tc_timed_event4(void **);
int srv_handle_tc_timed_event4(void **);

void srv_start_tc_cancel_timed_event(void **);
int srv_handle_tc_cancel_timed_event(void **);

void srv_start_stop_scheduler(void **);
int srv_handle_stop_scheduler(void **);

void srv_start_broadcast1(void **);
int srv_handle_broadcast1(void **);
int cli_handle_broadcast1(void **);

void srv_start_broadcast2(void **);
int srv_handle_broadcast2(void **);
int cli_handle_broadcast2(void **);

void srv_start_tc_timer_impl1(void **);
int srv_handle_tc_timer_impl1(void **);

void srv_start_tc_bgint_none(void **);
int srv_handle_tc_bgint_none(void **);

void srv_start_tc_bgint_nomsg_notimer(void **);
int srv_handle_tc_bgint_nomsg_notimer(void **);

void srv_start_tc_bgint_nomsg_timer(void **);
int srv_handle_tc_bgint_nomsg_timer(void **);

void srv_start_tc_bgint_msg_notimer(void **);
int srv_handle_tc_bgint_msg_notimer(void **);

void srv_start_tc_bgint_msg_timer(void **);
int srv_handle_tc_bgint_msg_timer(void **);

void srv_start_tc_bgint_all(void **);
int srv_handle_tc_bgint_all(void **);


void srv_start_tc_timer_perf(void **);
int srv_handle_tc_timer_perf(void **);

void srv_start_tc_put_message_perf(void **);
int srv_handle_tc_put_message_perf(void **);
int cli_handle_tc_put_message_perf(void **);

void srv_start_tc_put_message_external(void **);
int srv_handle_tc_put_message_external(void **);
int cli_handle_tc_put_message_external(void **);
/* The entries in "ts" below contain 6 entries:
 * - Test case number
 * - Init function for task 1
 * - Handler function for task 1
 * - Init function for task 2
 * - Handler function for task 2
 * - Testcase name
 */
CsrCoalTcDefs ts[] =
{
    {0,
     srv_start_tc_put_message1,
     srv_handle_tc_put_message1,
     NULL,
     NULL,
     "put_message1"},

    {1,
     srv_start_tc_put_message2,
     srv_handle_tc_put_message2,
     NULL,
     cli_handle_tc_put_message2,
     "put_message2"},

    {2,
     srv_start_tc_timed_event1,
     srv_handle_tc_timed_event1,
     NULL,
     NULL,
     "time_event1"},

    {3,
     srv_start_tc_timed_event2,
     srv_handle_tc_timed_event2,
     NULL,
     NULL,
     "time_event2"},

    {4,
     srv_start_tc_cancel_timed_event,
     srv_handle_tc_cancel_timed_event,
     NULL,
     NULL,
     "CsrSchedTimerCancel"},

    {5,
     srv_start_broadcast1,
     srv_handle_broadcast1,
     NULL,
     cli_handle_broadcast1,
     "broadcast1"},

    {6,
     srv_start_broadcast2,
     srv_handle_broadcast2,
     NULL,
     cli_handle_broadcast2,
     "broadcast2"},

#ifdef INCLUDE_ELABORATE_TESTCASES
    /* This testcase should only be enabled when timer-wrapping
     * is set low enough to actually test it, otherwise it may
     * fail */
    {7,
     srv_start_tc_timer_impl1,
     srv_handle_tc_timer_impl1,
     NULL,
     NULL,
     "timer_impl1"},
#endif

    {8,
     srv_start_tc_bgint_nomsg_notimer,
     srv_handle_tc_bgint_nomsg_notimer,
     NULL,
     NULL,
     "bgint_nomsg_notimer"},

    {9,
     srv_start_tc_bgint_nomsg_timer,
     srv_handle_tc_bgint_nomsg_timer,
     NULL,
     NULL,
     "bgint_nomsg_timer"},

    {10,
     srv_start_tc_bgint_msg_notimer,
     srv_handle_tc_bgint_msg_notimer,
     NULL,
     NULL,
     "bgint_msg_notimer"},

    {11,
     srv_start_tc_bgint_msg_timer,
     srv_handle_tc_bgint_msg_timer,
     NULL,
     NULL,
     "bgint_msg_timer"},

    {12,
     srv_start_tc_bgint_all,
     srv_handle_tc_bgint_all,
     NULL,
     NULL,
     "bgint_all"},

#ifdef INCLUDE_ELABORATE_TESTCASES
    {13,
     srv_start_tc_timed_event3,
     srv_handle_tc_timed_event3,
     NULL,
     NULL,
     "time_event3"},

#ifdef _WIN32
    {14,
     srv_start_tc_timed_event4,
     srv_handle_tc_timed_event4,
     NULL,
     NULL,
     "time_event4"},
#endif
#endif

    {15,
     srv_start_tc_timer_perf,
     srv_handle_tc_timer_perf,
     NULL,
     NULL,
     "timer_perf"},

    {16,
     srv_start_tc_put_message_perf,
     srv_handle_tc_put_message_perf,
     NULL,
     cli_handle_tc_put_message_perf,
     "put_message_perf"},

    {17,
     srv_start_tc_put_message_external,
     srv_handle_tc_put_message_external,
     NULL,
     cli_handle_tc_put_message_external,
     "put_message_external"},

    {0xff, NULL, NULL, NULL, NULL, ""}
};

#ifdef __cplusplus
}
#endif

#endif
