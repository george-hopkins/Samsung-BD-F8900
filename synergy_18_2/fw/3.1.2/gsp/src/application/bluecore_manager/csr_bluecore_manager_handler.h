#ifndef CSR_BLUECORE_MANAGER_HANDLER_H__
#define CSR_BLUECORE_MANAGER_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrBluecoreManagerLto);

#define CSR_BLUECORE_MANAGER_MENU_CATEGORY "BlueCore"
#define CSR_BLUECORE_MANAGER_MENU_ITEM_NAME "Manager"

typedef CsrUint8 CsrBluecoreManagerState;
#define CSR_BLUECORE_MANAGER_STATE_INACTIVE        ((CsrBluecoreManagerState) 0)
#define CSR_BLUECORE_MANAGER_STATE_ACTIVATING      ((CsrBluecoreManagerState) 1)
#define CSR_BLUECORE_MANAGER_STATE_DEACTIVATING    ((CsrBluecoreManagerState) 2)
#define CSR_BLUECORE_MANAGER_STATE_ACTIVE          ((CsrBluecoreManagerState) 3)

typedef struct
{
    CsrBluecoreManagerState state;        /* Task state */
    void                   *message;      /* Last message received */

    /* Various information */
    CsrUint16 controlId;            /* The controlId associated with CSR_APP */

    /* UI */
    CsrUieHandle hSelectEvent;      /* Event triggered when selecting a menu item */
    CsrUieHandle hBackEvent;        /* Event that will hide the currently shown UIE */
    CsrUieHandle hMenu;             /* The BlueCore Manager main menu */
    CsrUieHandle hPopup;            /* Popup dialog for notifications */
} CsrBluecoreManagerInstanceData;

#ifdef __cplusplus
}
#endif

#endif
