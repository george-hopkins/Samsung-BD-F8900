#ifndef CSR_TCP_TRANSCEIVER_HANDLER_H__
#define CSR_TCP_TRANSCEIVER_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_log_text_2.h"

#define STATE_OFF           0
#define STATE_ENABLING      1
#define STATE_DISABLING     2
#define STATE_ON            3

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrTcpTransceiverLto);

typedef struct
{
    CsrUint8 cliState;                          /* Client state */
    CsrUint8 srvState;                          /* Server state */
    void    *message;                           /* Last message received */

    /* Various information */
    CsrUint16 controlId;                        /* The controlId associated with CSR_APP */
    CsrUint16 parameterKey;                     /* The key of the parameter being modified */

    /* Client */
    CsrUint16   cliSocketHandle;                /* Client connection socket handle */
    CsrSchedTid cliTimerId;
    CsrUint32   cliFrameSize;
    CsrTime     cliStartTime;

    /* Server */
    CsrUint16   listenSocketHandle;             /* Server listening socket handle */
    CsrUint16   srvSocketHandle;                /* Server connection socket handle */
    CsrSchedTid srvTimerId;
    CsrUint32   srvFrameSize;
    CsrTime     srvStartTime;

    /* UI */
    CsrUieHandle hSelectEvent;                  /* Event triggered when selecting a menu item */
    CsrUieHandle hBackEvent;                    /* Event that will hide the currently shown UIE */
    CsrUieHandle hMenu;                         /* The TCP Transceiver main menu */
    CsrUieHandle hOptionsMenu;                  /* The Options menu */
    CsrUieHandle hInputDialog;                  /* Options input dialog */
} CsrTcpTransceiverInstanceData;

#ifdef __cplusplus
}
#endif

#endif
