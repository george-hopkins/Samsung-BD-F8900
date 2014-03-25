#ifndef CSR_UI_DISPLAYSTACK_H__
#define CSR_UI_DISPLAYSTACK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_ui_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool CsrUiDisplayUieShow(CsrUiInstanceData *instanceData, CsrUieHandle handle, CsrUint16 priority, CsrUint16 inputMode, CsrSchedQid listener);
CsrBool CsrUiDisplayUieHide(CsrUiInstanceData *instanceData, CsrUieHandle handle);
void CsrUiDisplaySetinputmode(CsrUiInstanceData *instanceData, CsrUieHandle handle, CsrUint16 inputMode);
CsrUieHandle CsrUiDisplayGethandle(CsrUiInstanceData *instanceData);
void CsrUiDisplayDestroyAll(CsrUiInstanceData *instanceData);

/* Display Stack Item */
typedef struct DSitem
{
    CsrUieHandle   handle;
    CsrUint16      priority;
    CsrUint16      inputMode;
    CsrSchedQid    listener;
    struct DSitem *next;
} DSitem;

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_DISPLAYSTACK_H__ */
