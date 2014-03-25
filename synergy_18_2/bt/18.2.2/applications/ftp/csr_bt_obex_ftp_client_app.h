#ifndef CSR_BT_OBEX_FTP_CLIENT_APP_H__
#define CSR_BT_OBEX_FTP_CLIENT_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "csr_file.h"
#include "csr_pmem.h"
#include "csr_unicode.h"
#include "csr_message_queue.h"
#include "csr_sched_init.h"

#include "csr_ui_lib.h"
#include "csr_app_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_util.h"
#include "csr_bt_ftc_prim.h"
#include "csr_bt_ftc_lib.h"
#include "csr_bt_ftc_app_task.h"

#define KEY_CHECK_TIMEOUT               100000
#define MAX_DISCOVERY_IN_LIST           10
#define MAX_PACKET_SIZE                 (0xFFFE)
#define MAX_DIRLEN                      40
#define MAX_FOLDER_BUFFER               4096

#define MOVE_TO_BREDR                   0
#define MOVE_TO_AMP                     1

/* Authentication states */
#define AUTH_USERID   0
#define AUTH_PASS     1


typedef struct
{
    CsrUieHandle    hMainMenu;
    CsrUieHandle    hSelectedOptionsMenu;
    CsrUieHandle    hFileBrowser;
    CsrUieHandle    hObexPasskeyInputDialog;
    CsrUieHandle    hDirectoryInputDialog;
    CsrUieHandle    hCopyInputDialog;
    CsrUieHandle    hMoveInputDialog;
    CsrUieHandle    hSetPermInputDialog;
    CsrUieHandle    hAllowMenu;
    CsrUieHandle    hCommonDialog; /* Connecting... and Trasfering... */

    CsrUieHandle    hBackEvent;
    CsrUieHandle    hSk1Event;
    CsrUieHandle    hSk1AbortEvent;
    CsrUieHandle    hSk1CancelEvent;
    CsrUieHandle    hSk2Event;
    CsrUieHandle    hSk2AmpEvent;
    CsrUieHandle    hSk2BrEdrEvent;

    CsrBool         mainMenuUi;
    CsrBool         selectedOptionsMenuUi;
    CsrBool         fileBrowserUi;
    CsrBool         commonDialogUi;
    CsrBool         obexPasskeyUi;
    CsrBool         allowMenuUi;
    CsrBool         directoryInputDialogUi;
    CsrBool         copyInputDialogUi;
    CsrBool         moveInputDialogUi;
    CsrBool         setPermInputDialogUi;
} CsrBtObexFtpClientAppUiInstData;

/* Instance data for application */
typedef struct
{
    CsrSchedQid         ftcSchedQid;                                /* My application/queue handle id */
    CsrBtObexFtpClientAppUiInstData csrUiVar;
    CsrBtDeviceAddr     search_list[MAX_DISCOVERY_IN_LIST];         /* Search list */
    CsrUint8            search_idx;                                 /* Search list index */
    CsrBtDeviceAddr     address;                                    /* Remote active address */
    CsrUint16           event;                                      /* Message id */
    void                *msg;                                       /* Message body */
    char                *targ_name;                                 /* Target name */
    CsrBool             targ_remote;                                /* Is target remote? */
    CsrBool             targ_dir;                                   /* Is target a directory? */
    struct FtcInfo_t    *dir_head;                                  /* Browser head */
    struct FtcInfo_t    *dir_tail;                                  /* Browser tail */
    CsrUint16           dir_count;                                  /* Number of entries */
    CsrUint16           dir_index;                                  /* Current index */
    char                *dir_local;                                 /* Local directory */
    char                *dir_remote;                                /* Remote directory */
    char                *dir_newrem;                                /* New remote directory */
    char                *mkdir_name;                                /* New dir name */
    CsrBool             mkdir_remote;                               /* New dir local/remote */
    CsrUint32           mkdir_idx;                                  /* New dir index */

    char                *copy_name;                                 /* dest copy name */
    CsrBool             copy_remote;                                /* copy local/remote */

    char                *move_name;                                 /* dest copy name */
    CsrBool             move_remote;                                /* copy local/remote */

    char                *setPerm_name;                              /* setPerm name */
    CsrBool             setPerm_remote;                             /* setPerm local/remote */

    void                *folder;                                    /* Remote folder listing data */
    CsrUint32           folder_idx;                                 /* Remote folder listing index */
    CsrUint32           folder_size;                                /* Remote tolder listing size */
    CsrFileHandle       *file;                                      /* File transfer */
    CsrUint32           file_idx;                                   /* Transfer index */
    CsrUint32           file_size;                                  /* Transfer total size */
    CsrUint32           file_progress;                              /* Transfer progress UI update threshold */
    CsrTime             file_time;                                  /* Transfer start time */
    CsrBool             auth_local;                                 /* Local or remote request */
    CsrUint16           auth_state;                                 /* Authentication state */
    CsrCharString*      auth_user;                                  /* Authentication userid */
    CsrBtConnId         btConnId;                                   /* Valid id for connections */
    CsrBool             srmSupported;                               /* Single response mode support */
    CsrUint16           channelInUse;                               /* L2cap over AMP or BR/EDR */
} CsrBtObexFtpClientAppGlobalInstData;


/* Prototypes */
void cleanInstance(CsrBtObexFtpClientAppGlobalInstData *inst);

#ifdef __cplusplus
extern "C" }
#endif

#endif /* CSR_BT_OBEX_FTP_CLIENT_APP_H__ */

