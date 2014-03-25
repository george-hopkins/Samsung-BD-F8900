#ifndef CSR_ABCSP_CONFIG_TIMER_H__
#define CSR_ABCSP_CONFIG_TIMER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAMES
        ABCSP_START_BCSP_TIMER  -  start the bcsp ack timeout timer
        ABCSP_START_TSHY_TIMER  -  start the bcsp-le tshy timer
        ABCSP_START_TCONF_TIMER  -  start the bcsp-le tconf timer

SYNOPSES
        void ABCSP_START_BCSP_TIMER(void);
        void ABCSP_START_TSHY_TIMER(void);
        void ABCSP_START_TCONF_TIMER(void);

FUNCTIONS
        These three functions each schedule a timed event.  Each
        event catcher is "void fn(void)".

            The BCSP timer requires a call to CsrAbcspBcspTimedEvent();

            The TSHY timer requires a call to CsrAbcspTshyTimedEvent();

            The TCONF timer requires a call to CsrAbcspTconfTimedEvent();

        The timers' periods are configurable items, and are set in the
        external environment:

            The BCSP (Ttimeout) timer is normally set to 0.25s.

            The BCSP Link Establishment Tshy timer is normally set to 0.25s.

            The BCSP Link Establishment Tconf timer is normally set to 0.25s.

        The ABCSP_CANCEL_*_TIMER() functions each cancel the corresponding
        timer.

    The ABCSP_START_TSHY_TIMER() and ABCSP_START_TCONF_TIMER() timers are
    not used at the same time.
*/
void CsrAbcspStartBcspTimer(void);
void CsrAbcspStartTshyTimer(void);
void CsrAbcspStartTconfTimer(void);
#define ABCSP_START_BCSP_TIMER()    CsrAbcspStartBcspTimer()
#define ABCSP_START_TSHY_TIMER()    CsrAbcspStartTshyTimer()
#define ABCSP_START_TCONF_TIMER()    CsrAbcspStartTconfTimer()

/****************************************************************************
NAMES
        ABCSP_CANCEL_BCSP_TIMER  -  cancel the bcsp ack timeout timer
        ABCSP_CANCEL_TSHY_TIMER  -  cancel the bcsp-le tshy timer
        ABCSP_CANCEL_TCONF_TIMER  -  cancel the bcsp-le tconf timer

SYNOPSES
        void ABCSP_CANCEL_BCSP_TIMER(void);
        void ABCSP_CANCEL_TSHY_TIMER(void);
        void ABCSP_CANCEL_TCONF_TIMER(void);

FUNCTIONS
        Each function prevents its timed event from occurring, if possible.

        It is acceptable to call one of these cancel functions if the
        corresponding timer isn't running.
*/
void CsrAbcspCancelBcspTimer(void);
void CsrAbcspCancelTshyTimer(void);
void CsrAbcspCancelTconfTimer(void);
#define ABCSP_CANCEL_BCSP_TIMER()    CsrAbcspCancelBcspTimer()
#define ABCSP_CANCEL_TSHY_TIMER()    CsrAbcspCancelTshyTimer()
#define ABCSP_CANCEL_TCONF_TIMER()    CsrAbcspCancelTconfTimer()

#ifdef __cplusplus
}
#endif

#endif
