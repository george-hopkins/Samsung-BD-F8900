#ifndef CSR_UI_ICON_H__
#define CSR_UI_ICON_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrUiIconPut(CsrUiInstanceData *instanceData, CsrUint16 icon, CsrInt16 x, CsrInt16 y);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_ICON_H__ */
