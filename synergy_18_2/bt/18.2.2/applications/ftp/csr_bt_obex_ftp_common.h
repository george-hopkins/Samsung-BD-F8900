#ifndef CSR_BT_OBEX_FTP_COMMON_H__
#define CSR_BT_OBEX_FTP_COMMON_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "csr_file.h"
#include "csr_pmem.h"
#include "csr_unicode.h"
#include "csr_bt_util.h"
#include "csr_bt_platform.h"
#include "csr_bt_file.h"

#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_prim.h"
#include "csr_bt_ampm_lib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PROFILE_FTP_CLIENT                      CsrUtf8StrDup("FTP CLIENT")
#define PROFILE_FTP_SERVER                      CsrUtf8StrDup("FTP SERVER")
#define CSR_BT_FTP_PRIORITY                     1

#define WITHOUT_AUTHENTICATION                  0
#define WITH_AUTHENTICATION                     1
#define AUTHENTICATION_SUCCESS                  2

#define KEYB_CHECK_TIMEOUT                      100000
#define FTP_SERVER_ROOT_PATH                    "."
#define MAX_FILENAMELEN                         40
#define MAX_STRING_LENGTH                       256

#define COMMON_DIALOG_NONE                      0
#define COMMON_DIALOG_ALLOW                     1

#define OBEX_PASSKEY_RESPONSE                   0
#define OBEX_PASSKEY_INITIATE                   1

/* Delete a directory */
int recursiveRmDir(char* targetDir);
CsrUint8 *unicode2String(CsrUcs2String* unicode);
CsrFilePerms mapObexPermToFilePerm(CsrUint32 obexPermission,CsrBool *res);

#ifdef __cplusplus
}
#endif

#endif
