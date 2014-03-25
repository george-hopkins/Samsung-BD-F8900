#ifndef CSR_ABCSP_CONFIG_EVENT_H__
#define CSR_ABCSP_CONFIG_EVENT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp_prototype.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAME
        ABCSP_EVENT  -  report an event from the abcsp code

SYNOPSIS
        void ABCSP_EVENT(CsrUint32 e);

FUNCTION
        Reports the occurrence of the event "e".   Values for "e" are
    given in abcsp_events.h.

NOTE
    It is acceptable to #define ABCSP_EVENT() to be nothing.  This should
    cause all calls to the macro to drop silently from the code.
*/

#define ABCSP_EVENT(n)           CsrAbcspEvent(n)


/****************************************************************************
NAME
        ABCSP_REQ_PUMPTXMSGS  -  request external code call CsrAbcspPumpTxMsgs

SYNOPSIS
        void ABCSP_REQ_PUMPTXMSGS( abcsp *_this );

FUNCTION
    Tells external code that there is work to be done on the abcsp
    library's transmit path, and requests that it make a call to
    CsrAbcspPumpTxMsgs() at its earliest convenience.

NOTE
        This macro must not be wired directly to CsrAbcspPumpTxMsgs() or the
        code will go re-entrant and bomb.  This function should normally set
        a flag that should provoke a call to CsrAbcspPumpTxMsgs() after the
        current call into the abcsp library has returned.
*/

#define ABCSP_REQ_PUMPTXMSGS(t)   CsrAbcspReqPumpTxMsgs()

#ifdef __cplusplus
}
#endif

#endif
