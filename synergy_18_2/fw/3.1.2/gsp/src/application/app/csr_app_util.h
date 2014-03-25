#ifndef CSR_APP_UTIL_H__
#define CSR_APP_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_unicode.h"
#include "csr_app_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrAppRestoreSavedMessages(CsrAppInstData *instData);

CsrSchedQid CsrAppGetProfileEntryFromKey(CsrAppInstData *inst, CsrUint32 key);

CsrAppProfileUie *CsrAppGetProfileUieFromMenuName(CsrAppInstData *inst, CsrUtf8String *menuName, CsrUtf8String *subMenuName);

void CsrAppInsertDisplayMenuHandle(CsrAppInstData *inst, CsrUieHandle displayMenuHandle);
CsrAppProfileUie *CsrAppInsertPopupDialogHandle(CsrAppInstData *inst, CsrUieHandle popupDialogHandle);

void CsrAppRegisterProfile(CsrAppInstData *inst, CsrSchedQid phandle, CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrUint16 menuIcon, CsrUint16 subMenuIcon, CsrBool top);

#ifdef __cplusplus
}
#endif

#endif
