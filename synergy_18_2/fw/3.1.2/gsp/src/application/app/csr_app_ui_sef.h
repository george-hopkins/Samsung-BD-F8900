#ifndef CSR_APP_UI_SEF_H__
#define CSR_APP_UI_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_app_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Handler functions for the CSR_UI upstream messages */
void CsrAppCsrUiUieCreateCfmHandler(CsrAppInstData *inst);
void CsrAppCsrUiMenuGetitemCfmHandler(CsrAppInstData *inst);
void CsrAppCsrUiEventIndHandler(CsrAppInstData *inst);

#ifdef __cplusplus
}
#endif

#endif
