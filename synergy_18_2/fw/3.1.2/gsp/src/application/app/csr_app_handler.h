#ifndef CSR_APP_HANDLER_H__
#define CSR_APP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_lib.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrAppLto);

#define CSR_APP_STD_PRIO                                           0x01

typedef struct CsrAppProfileUieTag
{
    CsrSchedQid                 phandle;
    CsrUint16                   key;
    CsrUtf8String              *menuName;
    CsrUtf8String              *subMenuName;
    CsrUint16                   menuIcon;
    CsrUint16                   subMenuIcon;
    CsrBool                     top;
    CsrUieHandle                displayMenuHandle;              /* Shared between multiple CsrAppProfileUie in same menu */
    CsrUieHandle                popupDialogHandle;              /* One unique for each CsrAppProfileUie */
    struct CsrAppProfileUieTag *next;
} CsrAppProfileUie;

typedef struct
{
    CsrBool           initialized;
    CsrUint16         mainMenuEntries;
    CsrAppProfileUie *profileHandleList;

    CsrMessageQueueType *saveQueue;
    CsrBool              restoreFlag;

    CsrUint16 backLogEntries;
    CsrUint16 maxBackLogEntries;
    void     *recvMsgP;

    CsrUieHandle hExitEvent;
    CsrUieHandle hBackEvent;
    CsrUieHandle hTakeControlEvent;
    CsrUieHandle hSelectEvent;
    CsrUieHandle hMainMenu;
    CsrUieHandle hBacklogMenu;
    CsrUieHandle hBacklogDialog;
    CsrUieHandle hIdleScreen;
} CsrAppInstData;

void CsrAppFreeDownstreamMessageContents(CsrUint16, void *);

#ifdef __cplusplus
}
#endif

#endif
