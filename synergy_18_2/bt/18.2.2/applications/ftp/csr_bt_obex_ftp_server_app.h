#ifndef CSR_BT_OBEX_FTP_SERVER_APP_H__
#define CSR_BT_OBEX_FTP_SERVER_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "csr_file.h"
#include "csr_pmem.h"
#include "csr_unicode.h"
#include "csr_message_queue.h"

#include "csr_ui_lib.h"
#include "csr_app_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_util.h"
#include "csr_bt_fts_prim.h"
#include "csr_bt_fts_lib.h"
#include "csr_bt_fts_app_task.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    CsrUieHandle    hMainMenu;
    CsrUieHandle    hBackEvent;
    CsrUieHandle    hSk1Event;
    CsrUieHandle    hSk2Event;
    CsrUieHandle    hPasskeySk1Event;
    CsrUieHandle    hObexPasskeyInputDialog;
    CsrUieHandle    hAllowMenu;
    CsrUieHandle    hCommonDialog;
    CsrBool         commonDialogUi;
    CsrBool         allowMenuUi;
    CsrUint8        obexPasskeyUi;
} CsrBtObexFtpServerAppUiInstData;


/* FTS inst data */
typedef struct
{
    CsrUint8                        *lastfileSaved;
    CsrBool                         obexAuthentication;
    char                            ftsPassword[20];
    CsrUint8                        ftsPasswordLength;
    CsrUint8                        modeSelect;
    CsrUint16                       channelInUse;
    CsrUint16                       obexPeerMaxPacketSize;
    char                            currentPath[CSR_BT_MAX_PATH_LENGTH];
    char                            startupPath[CSR_BT_MAX_PATH_LENGTH];
    FILE                            *file;
    CsrUint32                       currentFileSize;
    CsrUint32                       currentFilePosition;
    char                            *folderListingString;
    CsrBtFtsPutObjInd               *ftsPutObjIndMsg;
    CsrBtFtsSetAddFolderInd         *ftsSetAddFolderIndMsg;
    CsrBtFtsGetObjInd               *ftsGetObjIndMsg;
    CsrUint32                       bytesReceived;
    CsrUint32                       totalBytes;
    CsrBtObexResponseCode           obexResult;
    CsrBool                         transferObject;
    CsrBtConnId                     btConnId;
    CsrMessageQueueType            *saveQueue;
    CsrBool                         restoreFlag;

    CsrBool                         initialized;

    CsrUint16                       event;
    void                            *message;
    CsrBool                         srmSupported;
    CsrBtDeviceAddr                 connectingDevice;

    CsrSchedQid                     queueId; /* profile queueId that this demo app instance talks to */

    CsrBtObexFtpServerAppUiInstData csrUiVar;


    CsrUint16                       backLogEntries;
    CsrUint16                       maxBackLogEntries;

    /* for AMP throughput measurement */
    CsrUint32                       dataCount;
    CsrTime                         moveTime;

} CsrBtObexFtpServerAppGlobalInstData;


#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_OBEX_FTP_SERVER_APP_H__ */
