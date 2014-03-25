#ifndef CSR_HCI_HANDLER_H__
#define CSR_HCI_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_mblk.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrHciLto);

typedef struct csrHciListStructType
{
    CsrUint16                    handle;
    CsrSchedQid                  queueId;
    CsrMblk                     *data;
    struct csrHciListStructType *next;
} csrHciListType;

typedef CsrUint8 CsrHciState;
#define CSR_HCI_STATE_IDLE          ((CsrHciState) 0)
#define CSR_HCI_STATE_ACTIVATED     ((CsrHciState) 1)
#define CSR_HCI_STATE_DEACTIVATING  ((CsrHciState) 2)

typedef struct
{
    CsrHciState state;

    CsrSchedQid     hciEventHandler;
    CsrSchedQid     hciAclMainHandler;
    csrHciListType *hciScoHandler;
    csrHciListType *hciAclHandler;
    csrHciListType *hciVendorEventHandler;

#ifndef CSR_BLUECORE_ONOFF
    CsrMessageQueueType *saveQueue;
    CsrBool              restoreFlag;
#endif

    void *recvMsgP;
} CsrHciInstData;


typedef void (*CsrHciEventType)(CsrHciInstData *instData);

#ifdef __cplusplus
}
#endif


#endif
