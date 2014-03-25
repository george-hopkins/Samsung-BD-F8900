/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#endif

#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_obex.h"
#include "csr_bt_syncs_prim.h"
#include "csr_bt_syncs_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_util.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_bt_demoapp.h"

/* #define VCARD_SUPPORT_DISABLED */
/* #define VCALENDAR_SUPPORT_DISABLED */

#define WITHOUT_AUTHENTICATION        0
#define WITH_AUTHENTICATION           1

#define PHONEBOOK_OBJECT_TYPE         1
#define CALENDAR_OBJECT_TYPE          2

#define MAX_TEMP_BUFFER_LENGTH        50
#define MAX_SIZE_OF_VFILE_NAME        200
#define MAX_SIZE_OF_FILENAME          200
#define MAX_NOF_VENTRIES              200
#define MAX_LINE_LENGTH               512

#define KEYB_CHECK_TIMEOUT            (100000)

char file0log[MAX_SIZE_OF_FILENAME]               = "syncfiles"PATH_SEP"0.log";
char filedevinfo[MAX_SIZE_OF_FILENAME]            = "syncfiles"PATH_SEP"devinfo.txt";
char fileinfolog[MAX_SIZE_OF_FILENAME]            = "syncfiles"PATH_SEP"info.log";
char filecclog[MAX_SIZE_OF_FILENAME]              = "syncfiles"PATH_SEP"cc.log";
char filepbvcf[MAX_SIZE_OF_FILENAME]              = "syncfiles"PATH_SEP"pb.vcf";

char fileCalcclog[MAX_SIZE_OF_FILENAME]           = "syncfiles"PATH_SEP"cal_cc.log";
char fileCalinfolog[MAX_SIZE_OF_FILENAME]         = "syncfiles"PATH_SEP"cal_info.log";
char fileCalFile[MAX_SIZE_OF_FILENAME]            = "syncfiles"PATH_SEP"cal_";
char fileUnknown[MAX_SIZE_OF_FILENAME]            = "syncfiles"PATH_SEP"";

static char localVcfFile[]       = "syncfiles"PATH_SEP"";

static char serielnr[]           = "SN:0001CSR\n";
static char databaseId[]         = "DID:12345678\n";
static char totalRecords[]       = "Total-Records:";
static char maximumRecords[]     = "Maximum-Records:50\n";

#define IDLE_KEYBOARD_STATE                     0
#define PASSKEY_KEYBOARD_STATE                  1
#define MODE_SELECT_KEYBOARD_STATE              2
#define OBEX_PASSKEY_KEYBOARD_STATE             3
#define ENTER_OBEX_PASSKEY_KEYBOARD_STATE       4

void KeyMessageService(CsrUint16 m, void *data)
{
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, m, data);
}

typedef struct
{
    CsrUint16        type;
    char            lastfileSaved[MAX_SIZE_OF_FILENAME];
    CsrUint8         *lastPutIndName;
    CsrUint16        lengthName;
    CsrBool          addNew;
} putNextIndDataType;

typedef struct
{
    char            Name[MAX_SIZE_OF_VFILE_NAME];
} fileNameType;

typedef struct
{
    CsrUint16        type;
    CsrUint8         totalNumberOfFiles;
    CsrUint8         fileNumber;
    fileNameType    file[MAX_NOF_VENTRIES];
} getNextIndDataType;

static getNextIndDataType  *savedGetIndData;
static putNextIndDataType  *savedPutIndData;


typedef struct
{
    CsrSchedQid       CsrSchedQid;
    CsrUint16        event;
    void            *message;
    CsrUint8         keyboardState;
    CsrUint8         prevKeyboardState;
    unsigned int    pbLuid;
    unsigned int    pbChangeCounter;
    char            lastfileSaved[MAX_SIZE_OF_FILENAME];
    unsigned int    numberOfRecords;
    CsrBool          obexAuthentication;
    CsrBool          clientAuthenticatedSuccessful;
    unsigned int    calLuid;
    unsigned int    calChangeCounter;
    unsigned int    calNumberOfRecords;
    CsrUint8         objectType;
    char            syncsPassword[20];
    CsrUint8         syncsPasswordLength;
    CsrUint8         modeSelect;
    CsrUint16        obexPeerMaxPacketSize;
    CsrBtDeviceAddr    deviceAuthenticated;
    CsrUint32        connId;
    ScAppInstData_t scInstData;
} SyncServerInstanceDataType;


void writeFile(SyncServerInstanceDataType *syncServerInstanceData,
               CsrUint8 theSupport)
{
    CsrFileHandle * writeFile;
    char fileName[MAX_SIZE_OF_FILENAME];

    char fileInfo[MAX_SIZE_OF_FILENAME];
    CsrFileHandle * logFile;

    DIR *dir;
    struct dirent *dir_entry;
    int tempNumber;
    char *ptr;

    strncpy(fileName, "syncfiles"PATH_SEP"devinfo.txt", MAX_SIZE_OF_FILENAME);
    writeFile = CsrBtFopen(fileName, "w");
    if(writeFile == NULL)
    {
        printf("Could not open file %s for writing!\n", fileName);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    CsrBtFwrite("MANU:CSR\n"           , 1, CsrStrLen("MANU:CSR\n"), writeFile);
    CsrBtFwrite("MOD:TestSync\n"       , 1, CsrStrLen("MOD:TestSync\n"), writeFile);
    CsrBtFwrite("IRMC-VERSION:1.1\n"   , 1, CsrStrLen("IRMC-VERSION:1.1\n"), writeFile);
    CsrBtFwrite("SN:0001CSR\n"         , 1, CsrStrLen("SN:0001CSR\n"), writeFile);
    CsrBtFwrite("PB-TYPE-TX:VCARD2.1\n", 1, CsrStrLen("PB-TYPE-TX:VCARD2.1\n"), writeFile);
    CsrBtFwrite("PB-TYPE-RX:VCARD2.1\n", 1, CsrStrLen("PB-TYPE-RX:VCARD2.1\n"), writeFile);

    if (theSupport == 2 || theSupport == 4)
    {
        CsrBtFwrite("CAL-TYPE-TX:VCAL1.0\n", 1, CsrStrLen("CAL-TYPE-TX:VCAL1.0\n"), writeFile);
        CsrBtFwrite("CAL-TYPE-RX:VCAL1.0\n", 1, CsrStrLen("CAL-TYPE-RX:VCAL1.0\n"), writeFile);

        strncpy(fileInfo, "syncfiles"PATH_SEP"cal_0.log", MAX_SIZE_OF_FILENAME);
        logFile = CsrBtFopen(fileInfo, "r");
        if (logFile == NULL)
        {
            printf("Failed to open the file (%s)!\n", fileInfo);
            printf("Deactivate and try again without Calendar support\n");
        }
        strncpy(fileInfo, "syncfiles"PATH_SEP"cal_info.log", MAX_SIZE_OF_FILENAME);
        logFile = CsrBtFopen(fileInfo, "r");
        if (logFile == NULL)
        {
            printf("Failed to open the file (%s)!\n", fileInfo);
            printf("Deactivate and try again without Calendar support\n");
        }
        strncpy(fileInfo, "syncfiles"PATH_SEP"cal_cc.log", MAX_SIZE_OF_FILENAME);
        logFile = CsrBtFopen(fileInfo, "r");
        if (logFile == NULL)
        {
            printf("Failed to open the file (%s)!\n", fileInfo);
            printf("Deactivate and try again without Calendar support\n");
        }
        else
        {
            char line[10];
            if( CsrBtFgets( line, 10, logFile ) == NULL)
            {
                /*            printf( "fgets error\n" );*/
            }
            else
            {
                syncServerInstanceData->calChangeCounter = atoi( line );
            }
            CsrBtFclose(logFile);
        }

        dir = CsrBtOpenDir("syncfiles/");

        while((dir_entry = CsrBtReadDir(dir)))
        {
            if ((ptr = CsrStrStr(dir_entry->d_name, ".vcs")) != NULL)
            {
                *ptr = '\0';

                sscanf(dir_entry->d_name, "%x", &tempNumber);
                syncServerInstanceData->calNumberOfRecords++;
                if (tempNumber > 0)
                {
                    syncServerInstanceData->calLuid = tempNumber;
                }
            }
            else
            {
                /* Do nothing, entry did not match */
            }
        }

        CsrBtCloseDir(dir);
        syncServerInstanceData->calLuid++;
    }
    else
    {
        CsrBtFwrite("CAL-TYPE-TX:NONE\n", 1, CsrStrLen("CAL-TYPE-TX:NONE\n"), writeFile);
        CsrBtFwrite("CAL-TYPE-RX:NONE\n", 1, CsrStrLen("CAL-TYPE-RX:NONE\n"), writeFile);
    }
    CsrBtFwrite("MSG-TYPE-TX:NONE\n" , 1, CsrStrLen("MSG-TYPE-TX:NONE\n"), writeFile);
    CsrBtFwrite("MSG-TYPE-RX:NONE\n" , 1, CsrStrLen("MSG-TYPE-RX:NONE\n"), writeFile);
    CsrBtFwrite("NOTE-TYPE-TX:NONE\n", 1, CsrStrLen("NOTE-TYPE-TX:NONE\n"), writeFile);
    CsrBtFwrite("NOTE-TYPE-RX:NONE\n", 1, CsrStrLen("NOTE-TYPE-RX:NONE\n"), writeFile);
    CsrBtFwrite("INBOX:SINGLE\n"     , 1, CsrStrLen("INBOX:SINGLE\n"), writeFile);
    CsrBtFwrite("MSG-SENT-BOX:NO\n"  , 1, CsrStrLen("MSG-SENT-BOX:NO\n"), writeFile);
    CsrBtFclose(writeFile);
}

void showMenu(SyncServerInstanceDataType *syncServerInstanceData)
{
    if(syncServerInstanceData->modeSelect == 0)
    {
        printf("\nOBEX SYNC SERVER:\n\n");
        printf("\nProgram options:\n\n");
        printf("    1) Activate Vcard support\n");
        printf("    2) Activate Vcard and Vcalendar support\n");
        printf("    3) Activate Vcard support with Obex authentication\n");
        printf("    4) Activate Vcard and Vcalendar support with Obex authentication\n");
        printf("    5) Security Controller Configuration\n");
        printf("    6) Set OBEX password\n");
        printf("\nHit <ESC> to quit program!\n\n");
    }
    else
    {
        printf("\nOBEX SYNC SERVER:\n\n");
        printf("\nProgram options:\n\n");
        printf("    5) Security Controller Configuration\n");
        printf("    6) Set OBEX password\n");
        printf("    9) Deactivate sync\n");
        printf("\nHit <ESC> to quit program!\n\n");
    }
    syncServerInstanceData->keyboardState = MODE_SELECT_KEYBOARD_STATE;
}

static void syncScAppSetFnFini(void *pFiniData)
{
    SyncServerInstanceDataType *instData = (SyncServerInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
    system(CLS);
    showMenu(instData);
}

static void syncScAppSetFnInit(void *pInitData)
{
    SyncServerInstanceDataType *instData = (SyncServerInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, PASSKEY_KEYBOARD_STATE);
}

static void* syncScAppSetFnGetMessage(void *pRecvMsgPData)
{
    SyncServerInstanceDataType *instData = (SyncServerInstanceDataType *) pRecvMsgPData;
    void *result                         = instData->message;
    instData->message                    = NULL;
    return result;
}

void CsrBtAppInit(void ** gash)
{
    char fileInfo[MAX_SIZE_OF_FILENAME];
    CsrFileHandle *logFile;
    int tempNumber;
    DIR *dir;
    struct dirent *dir_entry;

    SyncServerInstanceDataType * syncServerInstanceData;

    *gash = CsrPmemAlloc(sizeof(SyncServerInstanceDataType));
    savedPutIndData = CsrPmemAlloc(sizeof(putNextIndDataType));
    savedGetIndData = CsrPmemAlloc(sizeof(getNextIndDataType));

    syncServerInstanceData = (SyncServerInstanceDataType *) *gash;
    syncServerInstanceData->keyboardState = MODE_SELECT_KEYBOARD_STATE;
    syncServerInstanceData->pbLuid = 0xFF;
    syncServerInstanceData->pbChangeCounter = 0;
    syncServerInstanceData->numberOfRecords = 0;
    syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
    syncServerInstanceData->calChangeCounter = 0;
    syncServerInstanceData->calLuid = 0x7F;
    syncServerInstanceData->calNumberOfRecords = 0;
    syncServerInstanceData->syncsPasswordLength = 0;
    syncServerInstanceData->modeSelect = 0;

    /* Application handle */
    syncServerInstanceData->CsrSchedQid = TESTQUEUE;

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    showMenu(syncServerInstanceData);

    strncpy(fileInfo, "syncfiles"PATH_SEP"0.log", MAX_SIZE_OF_FILENAME);
    logFile = CsrBtFopen(fileInfo, "r");
    if (logFile == NULL)
    {
        printf("Failed to open the file (%s)!\n", fileInfo);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    strncpy(fileInfo, "syncfiles"PATH_SEP"info.log", MAX_SIZE_OF_FILENAME);
    logFile = CsrBtFopen(fileInfo, "r");
    if (logFile == NULL)
    {
        printf("Failed to open the file (%s)!\n", fileInfo);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    strncpy(fileInfo, "syncfiles"PATH_SEP"cc.log", MAX_SIZE_OF_FILENAME);
    logFile = CsrBtFopen(fileInfo, "r");
    if (logFile == NULL)
    {
        printf("Failed to open the file (%s)!\n", fileInfo);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    else
    {
        char line[10];
        if(CsrBtFgets( line, 10, logFile ) != NULL)
        {
            syncServerInstanceData->pbChangeCounter = atoi( line );
        }
        CsrBtFclose(logFile);
    }

    dir = CsrBtOpenDir("syncfiles/");

    while((dir_entry = CsrBtReadDir(dir)))
    {
        if ((CsrStrStr(dir_entry->d_name, ".vcf")) != NULL)
        {
            sscanf(dir_entry->d_name, "%x", &tempNumber);
            syncServerInstanceData->numberOfRecords++;
            if (tempNumber > 0)
            {
                syncServerInstanceData->pbLuid = tempNumber;
            }
        }
    }

    CsrBtCloseDir(dir);

    syncServerInstanceData->pbLuid++;

    /* Security App Configuration */
    CsrBtScActivateReqSend(syncServerInstanceData->CsrSchedQid);
    scAppInitInstanceData(&syncServerInstanceData->scInstData);

    scAppSetFnFini(&syncServerInstanceData->scInstData, syncScAppSetFnFini, syncServerInstanceData);
    scAppSetFnInit(&syncServerInstanceData->scInstData, syncScAppSetFnInit, syncServerInstanceData);
    scAppSetSecLevelSend(&syncServerInstanceData->scInstData, scAppSendProfileSecurityLevelReq, &syncServerInstanceData->scInstData);

    scAppSetFnGetMessage(&syncServerInstanceData->scInstData, syncScAppSetFnGetMessage, syncServerInstanceData);

    scAppSetSecLevelPrim(&syncServerInstanceData->scInstData,
                         0,     /* Index 0. Single profile */
                         "SYNCS", CSR_BT_SYNCS_PRIM,
                         CSR_BT_SYNCS_IFACEQUEUE,
                         CSR_BT_SYNCS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    msgType=0;
    void        *msgData=NULL;
    SyncServerInstanceDataType * syncServerInstanceData;

    syncServerInstanceData = (SyncServerInstanceDataType *) *gash;

    scAppDeinitInstanceData(&syncServerInstanceData->scInstData);

    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType                                                        */
    while ( CsrSchedMessageGet(&msgType, &msgData) )
    {
        switch (msgType)
        {
            case CSR_BT_SYNCS_PRIM:
                CsrBtSyncsFreeUpstreamMessageContents(msgType, msgData);
                break;
            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(msgType, msgData);
                break;
        }
        CsrPmemFree(msgData);
    }

    CsrPmemFree(savedPutIndData);
    CsrPmemFree(savedGetIndData);
    CsrPmemFree(syncServerInstanceData);
}
#endif

CsrBool buffer_check(CsrUint16 maxLength, CsrUint16 length)
{
    CsrBool retVal = TRUE;
    if (maxLength < length)
    {
#ifdef DEBUG
        printf("BUFFER ERROR (Max=%i, Actual=%i)!!!!\n", maxLength, length);
#endif
        retVal = FALSE;
    }
    return retVal;
}

void CsrBtAppHandler(void ** gash)
{
    DIR *dir;
    struct dirent *dir_entry;

    SyncServerInstanceDataType * syncServerInstanceData;

    syncServerInstanceData = (SyncServerInstanceDataType *) *gash;

    CsrSchedMessageGet(
        &syncServerInstanceData->event,
        &syncServerInstanceData->message);

    switch (syncServerInstanceData->event)
    {
        case CSR_BT_SYNCS_PRIM:
            {
                CsrBtSyncsPrim * syncsPrim;

                syncsPrim = (CsrBtSyncsPrim *) syncServerInstanceData->message;

                switch (*syncsPrim)
                {
                    case CSR_BT_SYNCS_SECURITY_IN_CFM:
                        {
                            handleScSecLevelPrim(&syncServerInstanceData->scInstData);
                            break;
                        }
                    case CSR_BT_SYNCS_CONNECT_IND:
                        {
                            CsrBtSyncsConnectInd * connectInd;
                            connectInd = (CsrBtSyncsConnectInd *) syncServerInstanceData->message;

                            syncServerInstanceData->obexPeerMaxPacketSize = connectInd->obexPeerMaxPacketSize;


                            syncServerInstanceData->connId = connectInd->connectionId;
                            if (syncServerInstanceData->obexAuthentication == WITH_AUTHENTICATION)
                            {
                                if ((syncServerInstanceData->clientAuthenticatedSuccessful) &&
                                    (CsrBtBdAddrEq(&(syncServerInstanceData->deviceAuthenticated), &(connectInd->deviceAddr))) )
                                {

                                    printf("\n\nBluetooth address: %04X %02X %06X is now connected to the server\n",
                                           connectInd->deviceAddr.nap,
                                           connectInd->deviceAddr.uap,
                                           connectInd->deviceAddr.lap);
                                    CsrBtSyncsConnectResSend(syncServerInstanceData->connId,
                                                             CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                                }
                                else
                                {
                                    CsrBtBdAddrCopy(&(syncServerInstanceData->deviceAuthenticated), &(connectInd->deviceAddr));
                                    if(!syncServerInstanceData->syncsPasswordLength)
                                    {
                                        printf("\n\nBluetooth address: %04X %02X %06X is now been authenticated with password 1234\n",
                                               connectInd->deviceAddr.nap,
                                               connectInd->deviceAddr.uap,
                                               connectInd->deviceAddr.lap);
                                        CsrBtSyncsAuthenticateReqSend(15, (CsrUint8 *)CsrStrDup("\0Enter Password"), 4, (CsrUint8 *)CsrStrDup("1234"), CsrStrDup("CSR"));
                                    }
                                    else
                                    {
                                        printf("\n\nBluetooth address: %04X %02X %06X is now been authenticated with password %s\n",
                                               connectInd->deviceAddr.nap,
                                               connectInd->deviceAddr.uap,
                                               connectInd->deviceAddr.lap,
                                               syncServerInstanceData->syncsPassword);
                                        CsrBtSyncsAuthenticateReqSend(0, NULL, syncServerInstanceData->syncsPasswordLength,
                                                                      (CsrUint8 *)CsrStrDup(syncServerInstanceData->syncsPassword), NULL );
                                    }
                                }

                            }
                            else
                            {
                                printf("\n\nBluetooth address: %04X %02X %06X is now connected to the server\n",
                                       connectInd->deviceAddr.nap,
                                       connectInd->deviceAddr.uap,
                                       connectInd->deviceAddr.lap);
                                CsrBtSyncsConnectResSend(connectInd->connectionId,
                                                         CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                            }
                            break;
                        }

                    case CSR_BT_SYNCS_AUTHENTICATE_CFM:
                        {
                            printf("\n\nServer initiated authentication successful.\n");

                            syncServerInstanceData->clientAuthenticatedSuccessful = TRUE;
                            break;
                        }

                    case CSR_BT_SYNCS_AUTHENTICATE_IND:
                        {
                            CsrBtSyncsAuthenticateInd  *prim;

                            prim = (CsrBtSyncsAuthenticateInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_AUTHENTICATE_IND received\n");
                            printf("Enter obex response passkey : ");
                            syncServerInstanceData->syncsPasswordLength = 0;
                            syncServerInstanceData->keyboardState = OBEX_PASSKEY_KEYBOARD_STATE;
                            CsrPmemFree(prim->realm);
                            break;
                        }

                        /*case CSR_BT_SYNCS_GET_IND:
                          {
                          CsrBtSyncsGetInd  *getInd;
                          CsrUint16     length = 0;
                          CsrUint8      *body = NULL;
                          CsrUint8      *name = NULL;

                          getInd = (CsrBtSyncsGetInd *) syncServerInstanceData->message;
                          name = CsrUcs2ByteString2Utf8(getInd->payload + getInd->ucs2nameOffset);
                          printf("CSR_BT_SYNCS_GET_IND received unknown request unsupported : %s\n",name);
                          CsrBtSyncsGetResSend(getInd->connectionId, TRUE, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, length, body, FALSE);
                          CsrPmemFree(getInd->payload);
                          CsrPmemFree(name);
                          break;
                          }
                        */
                    case CSR_BT_SYNCS_GET_NEXT_IND:
                        {
                            CsrBtSyncsGetNextInd * getInd;

                            CsrFileHandle       *logFile = NULL;
                            CsrBtStat_t statBuffer;
                            int        result;
                            char       fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode responseCode;
                            CsrUint8    lineCount = 0;
                            CsrUint8    line[MAX_LINE_LENGTH];
                            CsrUint8    fileError = FALSE;
                            CsrUint8    finalFlag = FALSE;
                            CsrUint16   length = 0;
                            CsrUint8    *body = NULL;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetNextInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_NEXT_IND received\n");
                            printf("Get next local vCard files\n");

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            if ( savedGetIndData->type == CSR_BT_SYNCS_GET_PB_ALL_IND)
                            {
                                if (savedGetIndData->fileNumber < savedGetIndData->totalNumberOfFiles)
                                {
                                    strncpy(fileInfo, localVcfFile, MAX_SIZE_OF_FILENAME);
                                    if (savedGetIndData->fileNumber == savedGetIndData->totalNumberOfFiles - 1)
                                    {
                                        finalFlag = TRUE;
                                        strncat(fileInfo, savedGetIndData->file[savedGetIndData->fileNumber].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                                        printf("Send file %s last file\n", savedGetIndData->file[savedGetIndData->fileNumber].Name);
                                    }
                                    else
                                    {
                                        finalFlag = FALSE;
                                        strncat(fileInfo, savedGetIndData->file[savedGetIndData->fileNumber].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                                        printf("Send file %s\n", savedGetIndData->file[savedGetIndData->fileNumber].Name);
                                    }
                                    savedGetIndData->fileNumber++;
                                }
                            }
                            else if( savedGetIndData->type == CSR_BT_SYNCS_GET_CAL_ALL_IND )
                            {
                                if (savedGetIndData->fileNumber < savedGetIndData->totalNumberOfFiles)
                                {
                                    strncpy(fileInfo, localVcfFile, MAX_SIZE_OF_FILENAME);
                                    if (savedGetIndData->fileNumber == savedGetIndData->totalNumberOfFiles - 1)
                                    {
                                        finalFlag = TRUE;
                                        strncat(fileInfo, savedGetIndData->file[savedGetIndData->fileNumber].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                                        printf("Send file %s last file\n", savedGetIndData->file[savedGetIndData->fileNumber].Name);
                                    }
                                    else
                                    {
                                        finalFlag = FALSE;
                                        strncat(fileInfo, savedGetIndData->file[savedGetIndData->fileNumber].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                                        printf("Send file %s\n", savedGetIndData->file[savedGetIndData->fileNumber].Name);
                                    }
                                    savedGetIndData->fileNumber++;
                                }
                            }
                            else
                            {
                                /* Unknown type */
                                printf("Unknown type\n");
                                finalFlag = TRUE;
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                            }

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if(statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size, syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                                finalFlag = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                                finalFlag = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);

                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            CsrBtSyncsGetNextResSend(getInd->connectionId, finalFlag, responseCode, length, body, FALSE);
                            break;
                        }

                        /*case CSR_BT_SYNCS_PUT_IND:
                          {
                          CsrBtSyncsPutInd    *putInd;
                          CsrUint16       length = 0;
                          CsrUint8        *appParameter = NULL;
                          CsrUint8        *name = NULL;

                          putInd = (CsrBtSyncsPutInd *) syncServerInstanceData->message;
                          name = CsrUcs2ByteString2Utf8(putInd->payload + putInd->ucs2nameOffset);
                          printf("CSR_BT_SYNCS_PUT_IND received unsupported : %s\n",name);

                          CsrBtSyncsPutResSend(putInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, length, appParameter, FALSE);
                          CsrPmemFree(putInd->payload);
                          CsrPmemFree(name);
                          break;
                          }
                        */
                    case CSR_BT_SYNCS_PUT_NEXT_IND:
                        {
                            CsrBtSyncsPutNextInd * putNextInd;
                            CsrFileHandle       *putFile;
                            char       fileName[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            char       buffer[MAX_TEMP_BUFFER_LENGTH];
                            CsrUint16   length = 0;
                            CsrUint8    *appParameter = NULL;

                            putNextInd = (CsrBtSyncsPutNextInd *) syncServerInstanceData->message;
                            printf("CSR_BT_SYNCS_PUT_NEXT_IND received, append to file : %s\n", savedPutIndData->lastfileSaved);
                            putFile = CsrBtFopen(savedPutIndData->lastfileSaved, "ab");

                            if(putFile == NULL)
                            {
                                printf("Could not open putFile '%s'\n", savedPutIndData->lastfileSaved);
                            }
                            else
                            {
                                CsrBtFwrite(putNextInd->payload + putNextInd->bodyOffset,
                                            1,
                                            putNextInd->bodyLength,
                                            putFile);
                                CsrBtFclose(putFile);
                                printf("Saved\n");
                            }

                            if (putNextInd->finalFlag)
                            {
                                CsrUint8 lengthChangeCounter;
                                CsrUint8 lengthServerLuid;
                                if (syncServerInstanceData->objectType == PHONEBOOK_OBJECT_TYPE)
                                {
                                    syncServerInstanceData->pbChangeCounter++;    /* test if it's pb or cal */

                                    putFile = CsrBtFopen("syncfiles"PATH_SEP"cc.log", "w");

                                    if(putFile == NULL)
                                    {
                                        printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cc.log");
                                    }
                                    else
                                    {
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        sprintf(buffer,"%d\n\r", syncServerInstanceData->pbChangeCounter);
                                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                        CsrBtFclose(putFile);
                                    }

                                    if (savedPutIndData->type == CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND ||
                                        savedPutIndData->type == CSR_BT_SYNCS_PUT_IND)
                                    {
                                        char temp[MAX_TEMP_BUFFER_LENGTH];
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        length = sprintf(buffer,"0x%x", syncServerInstanceData->pbLuid);
                                        lengthServerLuid = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);
                                        length = sprintf(buffer,"%d",syncServerInstanceData->pbChangeCounter);
                                        lengthChangeCounter = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        length = sprintf(temp, "%c%c%X%c%c%c%d", 1, lengthServerLuid, syncServerInstanceData->pbLuid,
                                                         0, 2, lengthChangeCounter, syncServerInstanceData->pbChangeCounter);
                                        length++;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);
                                        appParameter = CsrPmemAlloc(length);
                                        CsrMemCpy(appParameter, temp, length);

                                        syncServerInstanceData->pbLuid++;
                                        syncServerInstanceData->numberOfRecords++;
                                    }
                                    else if (savedPutIndData->type == CSR_BT_SYNCS_PUT_PB_ENTRY_IND)
                                    {
                                        char temp[MAX_TEMP_BUFFER_LENGTH];
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        length = sprintf(buffer,"%d",syncServerInstanceData->pbChangeCounter);
                                        lengthChangeCounter = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        savedPutIndData->lengthName++;
                                        length = sprintf(temp, "%c%c%s%c%c%c%d", 1, savedPutIndData->lengthName,
                                                         savedPutIndData->lastPutIndName, 0, 2,
                                                         lengthChangeCounter, syncServerInstanceData->pbChangeCounter);
                                        length++;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        appParameter = CsrPmemAlloc(length);
                                        CsrMemCpy(appParameter, temp, length);
                                    }

                                    strncpy(fileName, "syncfiles"PATH_SEP"", MAX_SIZE_OF_FILENAME);
                                    length = sprintf(buffer,"%d",syncServerInstanceData->pbChangeCounter);
                                    buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                    strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                    strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                    putFile = CsrBtFopen(fileName, "w");

                                    if(putFile == NULL)
                                    {
                                        printf("Could not open putFile '%s'\n", fileName);
                                    }
                                    else
                                    {
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                        CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                        CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                        sprintf(buffer, "%d\n", syncServerInstanceData->numberOfRecords);
                                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                        CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                        CsrBtFclose(putFile);
                                    }
                                }
                                else if (syncServerInstanceData->objectType == CALENDAR_OBJECT_TYPE)
                                {
                                    syncServerInstanceData->calChangeCounter++;    /* test if it's pb or cal */

                                    putFile = CsrBtFopen("syncfiles"PATH_SEP"cal_cc.log", "w");

                                    if(putFile == NULL)
                                    {
                                        printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cal_cc.log");
                                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    }
                                    else
                                    {
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        sprintf(buffer,"%d\n\r", syncServerInstanceData->calChangeCounter);
                                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                        CsrBtFclose(putFile);
                                    }


                                    if (savedPutIndData->type == CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND ||
                                        savedPutIndData->type == CSR_BT_SYNCS_PUT_IND)
                                    {
                                        char temp[MAX_TEMP_BUFFER_LENGTH];
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        length = sprintf(buffer, "0x%x", syncServerInstanceData->calLuid);
                                        lengthServerLuid = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        length = sprintf(buffer, "%d", syncServerInstanceData->calChangeCounter);
                                        lengthChangeCounter = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        length = sprintf(temp, "%c%c%X%c%c%c%d", 1, lengthServerLuid,
                                                         syncServerInstanceData->calLuid, 0, 2,
                                                         lengthChangeCounter,
                                                         syncServerInstanceData->calChangeCounter);
                                        length++;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        appParameter = CsrPmemAlloc(length);
                                        CsrMemCpy(appParameter, temp, length);

                                        syncServerInstanceData->calLuid++;
                                        syncServerInstanceData->calNumberOfRecords++;
                                    }
                                    else if (savedPutIndData->type == CSR_BT_SYNCS_PUT_CAL_ENTRY_IND)
                                    {
                                        char temp[MAX_TEMP_BUFFER_LENGTH];
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        length = sprintf(buffer,"%d",syncServerInstanceData->calChangeCounter);
                                        lengthChangeCounter = strlen(buffer) + 1;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        savedPutIndData->lengthName++;
                                        length = sprintf(temp, "%c%c%s%c%c%c%d", 1, savedPutIndData->lengthName,
                                                         savedPutIndData->lastPutIndName, 0, 2,
                                                         lengthChangeCounter, syncServerInstanceData->calChangeCounter);
                                        length++;
                                        buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                        appParameter = CsrPmemAlloc(length);
                                        CsrMemCpy(appParameter, temp, length);
                                    }

                                    strncpy(fileName, "syncfiles"PATH_SEP"cal_", MAX_SIZE_OF_FILENAME);
                                    length = sprintf(buffer,"%d",syncServerInstanceData->calChangeCounter);
                                    buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                    strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                    strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                    putFile = CsrBtFopen(fileName, "w");

                                    if(putFile == NULL)
                                    {
                                        printf("Could not open putFile '%s'\n", fileName);
                                    }
                                    else
                                    {
                                        char buffer[MAX_TEMP_BUFFER_LENGTH];

                                        CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                        CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                        CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                        sprintf(buffer, "%d\n", syncServerInstanceData->calNumberOfRecords);
                                        CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                        CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                        CsrBtFclose(putFile);
                                    }

                                }
                            }
                            else
                            {
                                length = 0;
                                appParameter = NULL;
                            }
                            if (putNextInd->finalFlag)
                            {
                                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                            }

                            CsrBtSyncsPutNextResSend(putNextInd->connectionId, responseCode, length, appParameter, FALSE);
                            break;
                        }

                    case CSR_BT_SYNCS_GET_DEVICE_INFO_IND:
                        {
                            CsrBtSyncsGetDeviceInfoInd * getInd;

                            CsrFileHandle        *logFile;
                            CsrBtStat_t  statBuffer;
                            int         result;
                            char        fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode responseCode;
                            CsrUint8     fileError = FALSE;
                            CsrUint8     lineCount = 0;
                            CsrUint8     line[MAX_LINE_LENGTH];
                            CsrUint16    length;
                            CsrUint8     *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetDeviceInfoInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_DEVICE_INFO_IND received\n");
                            strncpy(fileInfo, filedevinfo, MAX_SIZE_OF_FILENAME);

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size, syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }

                            printf("Sending sync device information response\n");

                            CsrBtSyncsGetDeviceInfoResSend(getInd->connectionId, TRUE, responseCode, statBuffer.st_size, length, body, FALSE);
                            break;
                        }

/*********************** Phonebook primitives ******************************/

                    case CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND:
                        {
                            CsrBtSyncsGetPbChangeLogInd  *getInd;
                            CsrFileHandle                    *logFile;
                            CsrBtStat_t              statBuffer;
                            int                     result;
                            char                    fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrBool                  fileError = FALSE;
                            CsrUint8                 lineCount = 0;
                            CsrUint8                 line[MAX_LINE_LENGTH];
                            CsrUint8                 *name;
                            CsrUint16                length;
                            CsrUint8                 *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetPbChangeLogInd *) syncServerInstanceData->message;

                            strncpy(fileInfo, fileUnknown, MAX_SIZE_OF_FILENAME);
                            name = CsrUcs2ByteString2Utf8(getInd->payload + getInd->ucs2nameOffset);
                            strncat(fileInfo , (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            printf("CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND received %s\n", fileInfo);
                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size, syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }
                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsGetPbChangeLogResSend(getInd->connectionId, TRUE, responseCode, (unsigned int)statBuffer.st_size, length, body, FALSE);
#else
                            CsrBtSyncsGetPbChangeLogResSend(getInd->connectionId, TRUE, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
#endif
                            CsrPmemFree(name);
                            CsrPmemFree(getInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND:
                        {
                            CsrBtSyncsGetPbCurChangeLogInd * getInd;

                            CsrFileHandle         *logFile;
                            CsrBtStat_t   statBuffer;
                            int          result;
                            char         fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode responseCode;
                            CsrUint8      fileError = FALSE;
                            CsrUint8      lineCount = 0;
                            CsrUint8      line[MAX_LINE_LENGTH];
                            CsrUint16     length;
                            CsrUint8      *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetPbCurChangeLogInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND received\n");
                            strncpy(fileInfo, filecclog, MAX_SIZE_OF_FILENAME);

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);

                                printf("File '%s' read, created PB changelog result\n", fileInfo);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }

                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
                            syncServerInstanceData->pbChangeCounter = atoi((char*)body);
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsGetPbCurChangeLogResSend(getInd->connectionId, responseCode,
                                                               statBuffer.st_size, (CsrUint8) length, body, FALSE);
                            printf("Sent PB changelog\n");
#else
                            CsrBtSyncsGetPbCurChangeLogResSend(getInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            printf("Sent empty PB changelog as support is disabled\n");
                            CsrPmemFree(body);
#endif
                            break;
                        }

                    case CSR_BT_SYNCS_GET_PB_INFO_LOG_IND:
                        {
                            CsrBtSyncsGetPbInfoLogInd * getInd;

                            CsrFileHandle        *logFile;
                            CsrBtStat_t  statBuffer;
                            int         result;
                            char        fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8     fileError = FALSE;
                            CsrUint8     lineCount = 0;
                            CsrUint8     line[MAX_LINE_LENGTH];
                            CsrUint16    length;
                            CsrUint8     *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetPbInfoLogInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_PB_INFO_LOG_IND received\n");
                            strncpy(fileInfo, fileinfolog, MAX_SIZE_OF_FILENAME);

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }

                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsGetPbInfoLogResSend(getInd->connectionId, TRUE, responseCode, statBuffer.st_size, length, body, FALSE);
#else
                            CsrBtSyncsGetPbInfoLogResSend(getInd->connectionId, TRUE, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif

                            break;
                        }

                    case CSR_BT_SYNCS_GET_PB_ENTRY_IND:
                        {
                            CsrBtSyncsGetPbEntryInd * getInd;
                            CsrFileHandle        *logFile;
                            CsrBtStat_t  statBuffer;
                            int         result;
                            char        fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8     fileError = FALSE;
                            CsrUint8     lineCount = 0;
                            CsrUint8     line[MAX_LINE_LENGTH];
                            CsrUint8     *name;
                            CsrUint16    length;
                            CsrUint8     *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetPbEntryInd *) syncServerInstanceData->message;

                            name = CsrUcs2ByteString2Utf8(getInd->payload + getInd->ucs2nameOffset);
                            printf("CSR_BT_SYNCS_GET_PB_ENTRY_IND received : %s\n",name);
                            strncpy(fileInfo, fileUnknown, MAX_SIZE_OF_FILENAME);
                            strncat(fileInfo , (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }

                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsGetPbEntryResSend(getInd->connectionId, TRUE, responseCode,
                                                        statBuffer.st_size, length, body, FALSE);
#else
                            CsrBtSyncsGetPbEntryResSend(getInd->connectionId, TRUE,
                                                        CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif
                            CsrPmemFree(name);
                            CsrPmemFree(getInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_GET_PB_ALL_IND:
                        {
                            CsrBtSyncsGetPbAllInd  *getInd;
                            CsrFileHandle              *logFile = NULL;
                            CsrBtStat_t        statBuffer;
                            int               result;
                            char              fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8           finalFlag;
                            CsrUint8           lineCount = 0;
                            CsrUint8           line[MAX_LINE_LENGTH];
                            CsrUint16          length = 0;
                            CsrUint8           *body = NULL;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetPbAllInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_PB_ALL_IND received\n");

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            savedGetIndData->type = CSR_BT_SYNCS_GET_PB_ALL_IND;
                            savedGetIndData->totalNumberOfFiles = 0;
                            fileInfo[0] = '\0';

                            printf("Get all local vCard files\n");
                            dir = CsrBtOpenDir("syncfiles/");
                            finalFlag = TRUE;

                            while ((dir_entry = CsrBtReadDir(dir)) != NULL)
                            {
                                int i = 0;
                                while(dir_entry->d_name[i] != 0)
                                {
                                    fileInfo[i] = dir_entry->d_name[i];
                                    i++;
                                }
                                fileInfo[i] = '\0';

                                if (CsrStrStr(dir_entry->d_name, ".vcf"))
                                {
                                    if (MAX_SIZE_OF_VFILE_NAME>i)
                                    {
                                        strncpy(savedGetIndData->file[savedGetIndData->totalNumberOfFiles].Name,
                                                fileInfo, MAX_SIZE_OF_VFILE_NAME);
                                        printf("Send file %-12s saved %s\n", fileInfo,
                                               savedGetIndData->file[savedGetIndData->totalNumberOfFiles].Name);
                                        savedGetIndData->totalNumberOfFiles++;
                                    }

                                    else
                                    {
                                        printf("Too long filename. %s is being ignored!!!\n", fileInfo);
                                    }
                                }
                            }
                            CsrBtCloseDir(dir);

                            /* First file */
                            CsrMemSet(fileInfo, '\0', sizeof(fileInfo));
                            strncpy(fileInfo, localVcfFile, MAX_SIZE_OF_FILENAME);
                            strncat(fileInfo, savedGetIndData->file[0].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                            savedGetIndData->fileNumber = 1;
                            finalFlag = TRUE;

                            if( savedGetIndData->totalNumberOfFiles > 1 )
                            {
                                finalFlag = FALSE;
                            }

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                            }
                            else if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(0);
#endif
                                return;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }
                            else
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;

                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsGetPbAllResSend(getInd->connectionId, finalFlag, responseCode, length, body, FALSE);
#else
                            CsrBtSyncsGetPbAllResSend(getInd->connectionId, TRUE, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif

                            break;
                        }

                    case CSR_BT_SYNCS_PUT_PB_ENTRY_IND:
                        {
                            CsrBtSyncsPutPbEntryInd *putInd;

                            CsrFileHandle        *putFile = NULL;
                            char        fileName[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            char        buffer[MAX_TEMP_BUFFER_LENGTH];
                            CsrUint16    length = 0;
                            CsrUint8     *appParameter = NULL;
                            CsrUint8     *name = NULL;
                            CsrUint8     *pos = NULL;

                            putInd = (CsrBtSyncsPutPbEntryInd *) syncServerInstanceData->message;
                            name   = CsrUcs2ByteString2Utf8(putInd->payload + putInd->ucs2nameOffset);
                            printf("CSR_BT_SYNCS_PUT_PB_ENTRY_IND received\n");

                            strncpy(fileName, fileUnknown, MAX_SIZE_OF_FILENAME);
                            strncat(fileName, (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                            if ( (putInd->finalFlag) && (putInd->bodyLength == CSR_BT_NO_BODY_HEADER) )
                            {
                                if(unlink(fileName) == -1)
                                {
                                    printf("Could not delete the %s file\n", fileName);
                                }
                                else
                                {
                                    printf("Delete \"%s\" in SyncFiles.\n", fileName);
                                    syncServerInstanceData->numberOfRecords--;
                                }
                            }
                            else
                            {
                                putFile = CsrBtFopen(fileName, "wb");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    CsrBtFwrite(putInd->payload + putInd->bodyOffset,
                                                1,
                                                putInd->bodyLength,
                                                putFile);
                                    CsrBtFclose(putFile);
                                    printf("Saved \"%s\".vcf in SyncFiles.\n", name);
                                }

                            }
                            pos = (CsrUint8*)CsrBtStrRChr((char*)name, '.');
                            *pos = '\0';

                            if (putInd->finalFlag)
                            {
                                char      *temp = NULL;
                                CsrUint8   lengthChangeCounter = 2;
                                CsrUint16  lengthName = 0;

                                syncServerInstanceData->pbChangeCounter++;

                                putFile = CsrBtFopen("syncfiles"PATH_SEP"cc.log", "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cc.log");
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    sprintf(buffer,"%d\n\r", syncServerInstanceData->pbChangeCounter);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFclose(putFile);
                                }

                                strncpy(fileName, "syncfiles"PATH_SEP"", MAX_SIZE_OF_FILENAME);
                                length = sprintf(buffer,"%d",syncServerInstanceData->pbChangeCounter);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                putFile = CsrBtFopen(fileName, "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                    CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                    CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                    sprintf(buffer, "%d\n", syncServerInstanceData->numberOfRecords);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                    CsrBtFclose(putFile);
                                }

                                lengthChangeCounter = strlen(buffer) + 1;
                                lengthName = CsrStrLen((char*)name) + 1;

                                temp = CsrPmemAlloc(lengthName+MAX_TEMP_BUFFER_LENGTH);

                                length = sprintf(temp, "%c%c%s%c%c%c%d", 1, lengthName, name, 0, 2,
                                                 lengthChangeCounter,
                                                 syncServerInstanceData->pbChangeCounter);
                                length++;
                                buffer_check((CsrUint16)(lengthName+MAX_TEMP_BUFFER_LENGTH), length);

                                appParameter = CsrPmemAlloc(length);
                                CsrMemCpy(appParameter, temp, length);
                                CsrPmemFree(temp);
                                CsrPmemFree(name);
                            }
                            else
                            {
                                length = 0;
                                appParameter = NULL;
                                savedPutIndData->type = CSR_BT_SYNCS_PUT_PB_ENTRY_IND;
                                length = sprintf(savedPutIndData->lastfileSaved, fileName);
                                buffer_check(MAX_SIZE_OF_FILENAME, length);

                                CsrPmemFree(savedPutIndData->lastPutIndName);
                                savedPutIndData->lastPutIndName = name;
                                savedPutIndData->lengthName = CsrStrLen((char*)name) + 1;
                            }
                            if (putInd->finalFlag)
                            {
                                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                            }
                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsPutPbEntryResSend(putInd->connectionId, responseCode, length, appParameter, FALSE);
#else
                            CsrBtSyncsPutPbEntryResSend(putInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(appParameter);
#endif
                            CsrPmemFree(putInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND:
                        {
                            CsrBtSyncsPutPbAddEntryInd * putInd;

                            CsrFileHandle        *putFile;
                            char        fileName[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            char        buffer[MAX_TEMP_BUFFER_LENGTH];
                            CsrUint16    length;
                            CsrUint8     *appParameter;

                            putInd = (CsrBtSyncsPutPbAddEntryInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND received\n");

                            strncpy(fileName, fileUnknown, MAX_SIZE_OF_FILENAME);
                            length = sprintf(buffer,"%04x",syncServerInstanceData->pbLuid);
                            buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                            strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                            strncat(fileName, ".vcf", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                            putFile = CsrBtFopen(fileName, "wb");

                            if(putFile == NULL)
                            {
                                printf("Could not open putFile '%s'\n", fileName);
                            }
                            else
                            {
                                CsrBtFwrite(putInd->payload + putInd->bodyOffset,
                                            1,
                                            putInd->bodyLength,
                                            putFile);

                                CsrBtFclose(putFile);
                                printf("Saved \"%04X.vcf\" in SyncFiles filename %s.\n",
                                       syncServerInstanceData->pbLuid, fileName);
                            }

                            if (putInd->finalFlag)
                            {
                                char temp[MAX_TEMP_BUFFER_LENGTH];
                                char buffer[MAX_TEMP_BUFFER_LENGTH];
                                CsrUint8 lengthChangeCounter = 2;
                                CsrUint8 lengthServerLuid = 4;

                                syncServerInstanceData->pbChangeCounter++;

                                putFile = CsrBtFopen("syncfiles"PATH_SEP"cc.log", "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cc.log");
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    sprintf(buffer,"%d\n\r", syncServerInstanceData->pbChangeCounter);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFclose(putFile);
                                }

                                strncpy(fileName, "syncfiles"PATH_SEP"", MAX_SIZE_OF_FILENAME);
                                length = sprintf(buffer,"%04d",syncServerInstanceData->pbChangeCounter);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                putFile = CsrBtFopen(fileName, "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                    CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                    CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                    sprintf(buffer, "%d\n", syncServerInstanceData->numberOfRecords);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                    CsrBtFclose(putFile);
                                }

                                length = sprintf(buffer,"%04x",syncServerInstanceData->pbLuid);
                                lengthServerLuid = strlen(buffer);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                length = sprintf(buffer,"%04d",syncServerInstanceData->pbChangeCounter);
                                lengthChangeCounter = strlen(buffer);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                length = sprintf(temp, "%c%c%04X%c%c%04d", 1, lengthServerLuid,
                                                 syncServerInstanceData->pbLuid, 2,
                                                 lengthChangeCounter,
                                                 syncServerInstanceData->pbChangeCounter);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                appParameter = CsrPmemAlloc(length);
                                CsrMemCpy(appParameter, temp, length);

                                syncServerInstanceData->pbLuid++;
                                syncServerInstanceData->numberOfRecords++;
                            }
                            else
                            {
                                appParameter = NULL;
                                savedPutIndData->type = CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND;
                                length = sprintf(savedPutIndData->lastfileSaved, fileName);
                                buffer_check(MAX_SIZE_OF_FILENAME, length);
                                length = 0;
                            }

                            if (putInd->finalFlag)
                            {
                                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                            }
                            syncServerInstanceData->objectType = PHONEBOOK_OBJECT_TYPE;
#if !defined(VCARD_SUPPORT_DISABLED)
                            CsrBtSyncsPutPbAddEntryResSend(putInd->connectionId, responseCode, length, appParameter, FALSE);
#else
                            CsrBtSyncsPutPbAddEntryResSend(putInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(appParameter);
#endif

                            CsrPmemFree(putInd->payload);
                            length = sprintf(syncServerInstanceData->lastfileSaved, fileName);
                            buffer_check(MAX_SIZE_OF_FILENAME, length);

                            break;
                        }

/*********************** Calendar primitives *******************************/

                    case CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND:
                        {
                            CsrBtSyncsGetCalChangeLogInd * getInd;
                            CsrFileHandle          *logFile;
                            CsrBtStat_t    statBuffer;
                            int           result;
                            char          fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8       lineCount = 0;
                            CsrUint8       line[MAX_LINE_LENGTH];
                            CsrUint16      length;
                            CsrUint8       *body;
                            CsrUint8       *name;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetCalChangeLogInd *) syncServerInstanceData->message;
                            name = CsrUcs2ByteString2Utf8(getInd->payload + getInd->ucs2nameOffset);

                            strncpy(fileInfo, fileCalFile, MAX_SIZE_OF_FILENAME);
                            strncat(fileInfo , (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            printf("CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND received %s\n", fileInfo);
                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(0);
#endif
                                return;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length = 0;
                                body = NULL;
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;

#if !defined(VCALENDAR_SUPPORT_DISABLED)

                            CsrBtSyncsGetCalChangeLogResSend(getInd->connectionId, TRUE,
                                                             responseCode, statBuffer.st_size, length, body, FALSE);
#else
                            CsrBtSyncsGetCalChangeLogResSend(getInd->connectionId, TRUE,
                                                             CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif
                            CsrPmemFree(name);
                            CsrPmemFree(getInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND:
                        {
                            CsrBtSyncsGetCalCurChangeLogInd * getInd;

                            CsrFileHandle            *logFile;
                            CsrBtStat_t      statBuffer;
                            int             result;
                            char            fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8         fileError = FALSE;
                            CsrUint8         lineCount = 0;
                            CsrUint8         line[MAX_LINE_LENGTH];
                            CsrUint16        length;
                            CsrUint8         *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetCalCurChangeLogInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND received\n");
                            strncpy(fileInfo, fileCalcclog, MAX_SIZE_OF_FILENAME);

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }
                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;
                            syncServerInstanceData->pbChangeCounter = atoi((char*)body);

                            printf("Sending calendar changelog response, length=%i, total=%i\n", length, (unsigned int)statBuffer.st_size);

#if !defined(VCALENDAR_SUPPORT_DISABLED)
                            CsrBtSyncsGetCalCurChangeLogResSend(getInd->connectionId,
                                                                responseCode, statBuffer.st_size, (CsrUint8) length, body, FALSE);
#else
                            CsrBtSyncsGetCalCurChangeLogResSend(getInd->connectionId,
                                                                CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif
                            printf("Done sending cal changelog\n");
                            break;
                        }

                    case CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND:
                        {
                            CsrBtSyncsGetCalInfoLogInd * getInd;
                            CsrFileHandle            *logFile;
                            CsrBtStat_t      statBuffer;
                            int             result;
                            char            fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8         fileError = FALSE;
                            CsrUint8         lineCount = 0;
                            CsrUint8         line[MAX_LINE_LENGTH];
                            CsrUint16        length;
                            CsrUint8         *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetCalInfoLogInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND received\n");
                            strncpy(fileInfo, fileCalinfolog, MAX_SIZE_OF_FILENAME);

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (!fileError)
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }
                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;

#if !defined(VCALENDAR_SUPPORT_DISABLED)

                            CsrBtSyncsGetCalInfoLogResSend(getInd->connectionId, TRUE, responseCode,
                                                           statBuffer.st_size, length, body, FALSE);

#else
                            CsrBtSyncsGetCalInfoLogResSend(getInd->connectionId, TRUE,
                                                           CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif
                            break;
                        }

                    case CSR_BT_SYNCS_GET_CAL_ENTRY_IND:
                        {
                            CsrBtSyncsGetCalEntryInd * getInd;
                            CsrFileHandle            *logFile;
                            CsrBtStat_t      statBuffer;
                            int             result;
                            char            fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8         fileError = FALSE;
                            CsrUint8         lineCount = 0;
                            CsrUint8         line[MAX_LINE_LENGTH];
                            CsrUint16        length;
                            CsrUint8         *body;
                            CsrUint8         *name;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetCalEntryInd *) syncServerInstanceData->message;
                            name = CsrUcs2ByteString2Utf8(getInd->payload + getInd->ucs2nameOffset);
                            printf("CSR_BT_SYNCS_GET_CAL_ENTRY_IND received : %s\n", name);
                            strncpy(fileInfo, fileUnknown, MAX_SIZE_OF_FILENAME);
                            strncat(fileInfo , (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                fileError = TRUE;
                            }
                            if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                fileError = TRUE;
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                printf("Failed to open the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length = 0;
                                body = NULL;
                            }
                            else
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;
                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);

                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;
#if !defined(VCALENDAR_SUPPORT_DISABLED)
                            CsrBtSyncsGetCalEntryResSend(getInd->connectionId, TRUE, responseCode,
                                                         statBuffer.st_size, length, body, FALSE);
#else
                            CsrBtSyncsGetCalEntryResSend(getInd->connectionId, TRUE,
                                                         CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, 0, NULL, FALSE);
#endif
                            CsrPmemFree(name);
                            CsrPmemFree(getInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_GET_IND:
                    case CSR_BT_SYNCS_GET_CAL_ALL_IND:
                        {

                            CsrBtSyncsGetCalAllInd  *getInd;
                            CsrFileHandle              *logFile;
                            CsrBtStat_t        statBuffer;
                            int               result;
                            char              fileInfo[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            CsrUint8           finalFlag;
                            CsrUint8           lineCount = 0;
                            CsrUint8           line[MAX_LINE_LENGTH];
                            CsrUint16          length;
                            CsrUint8           *body;

                            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                            getInd = (CsrBtSyncsGetCalAllInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_GET_CAL_ALL_IND received\n");

                            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            savedGetIndData->type = CSR_BT_SYNCS_GET_CAL_ALL_IND;
                            savedGetIndData->totalNumberOfFiles = 0;
                            fileInfo[0] = '\0';

                            printf("Get all local vCalendar files\n");
                            dir = CsrBtOpenDir("syncfiles/");
                            finalFlag = TRUE;

                            while ((dir_entry = CsrBtReadDir(dir)) != NULL)
                            {
                                int i = 0;
                                while(dir_entry->d_name[i] != 0)
                                {
                                    fileInfo[i] = dir_entry->d_name[i];
                                    i++;
                                }
                                fileInfo[i] = '\0';

                                if (CsrStrStr(dir_entry->d_name, ".vcs"))
                                {
                                    if (MAX_SIZE_OF_VFILE_NAME>i)
                                    {
                                        strncpy(savedGetIndData->file[savedGetIndData->totalNumberOfFiles].Name,
                                                fileInfo, MAX_SIZE_OF_VFILE_NAME);
                                        printf("Send file %-12s saved %s\n", fileInfo, savedGetIndData->file[savedGetIndData->totalNumberOfFiles].Name);
                                        savedGetIndData->totalNumberOfFiles++;
                                    }
                                    else
                                    {
                                        printf("Too long filename. %s is being ignored!!!\n", fileInfo);
                                    }
                                }
                            }
                            CsrBtCloseDir(dir);

                            /* First file */
                            CsrMemSet(fileInfo, '\0', sizeof(fileInfo));
                            strncpy(fileInfo, localVcfFile, MAX_SIZE_OF_FILENAME);
                            strncat(fileInfo, savedGetIndData->file[0].Name, (MAX_SIZE_OF_FILENAME-strlen(fileInfo)));
                            savedGetIndData->fileNumber = 1;

                            if( savedGetIndData->totalNumberOfFiles > 1 )
                            {
                                finalFlag = FALSE;
                            }

                            result = CsrBtStat(fileInfo, &statBuffer);
                            if (result)
                            {
                                printf("Failed to get information about the file (%s)!\n", fileInfo);
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                            }
                            else if (statBuffer.st_size > syncServerInstanceData->obexPeerMaxPacketSize)
                            {
                                printf("0.log file too big (%u > %u)!\n", (unsigned int)statBuffer.st_size,
                                       syncServerInstanceData->obexPeerMaxPacketSize);
                                exit(0);
                            }

                            logFile = CsrBtFopen(fileInfo, "rb");
                            if (logFile == NULL)
                            {
                                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                length=0;
                                body = NULL;
                            }
                            else
                            {
                                while(CsrBtFgets((char*)line, MAX_LINE_LENGTH, logFile) != NULL)
                                {
                                    lineCount++;

                                }
                                CsrBtFseek(logFile,0L,SEEK_SET);
                                length = (CsrUint16)statBuffer.st_size;
                                body = CsrPmemAlloc(length + lineCount + 2) ;
                                CsrBtFread(body, 1, statBuffer.st_size, logFile);
                                CsrBtFclose(logFile);
                            }
                            syncServerInstanceData->objectType =  CALENDAR_OBJECT_TYPE;
#if !defined(VCALENDAR_SUPPORT_DISABLED)
                            CsrBtSyncsGetCalAllResSend(getInd->connectionId, finalFlag, responseCode, length, body, FALSE);
#else
                            CsrBtSyncsGetCalAllResSend(getInd->connectionId, TRUE, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(body);
#endif
                            break;
                        }
                    case CSR_BT_SYNCS_PUT_IND:
                    case CSR_BT_SYNCS_PUT_CAL_ENTRY_IND:
                        {


                            CsrBtSyncsPutCalEntryInd *putInd = NULL;
                            CsrFileHandle         *putFile = NULL;
                            char         fileName[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            char         buffer[MAX_TEMP_BUFFER_LENGTH];
                            CsrUint16     length = 0;
                            CsrUint8      *appParameter = NULL;
                            CsrUint8      *name = NULL;
                            CsrUint8      *pos = NULL;

                            putInd = (CsrBtSyncsPutCalEntryInd *) syncServerInstanceData->message;

                            name = CsrUcs2ByteString2Utf8(putInd->payload + putInd->ucs2nameOffset);

                            printf("CSR_BT_SYNCS_PUT_CAL_ENTRY_IND received\n");

                            strncpy(fileName, fileUnknown, MAX_SIZE_OF_FILENAME);
                            strncat(fileName , (char*)name, (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                            if ( (putInd->finalFlag) && (putInd->bodyLength == CSR_BT_NO_BODY_HEADER) )
                            {
                                if ( unlink(fileName) == -1)
                                {
                                    printf("Could not delete the %s file\n", fileName);
                                }
                                else
                                {
                                    printf("Delete \"%s\" in SyncFiles.\n", fileName);
                                    syncServerInstanceData->calNumberOfRecords--;
                                }

                            }
                            else
                            {
                                putFile = CsrBtFopen(fileName, "wb");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    CsrBtFwrite(putInd->payload + putInd->bodyOffset,
                                                1,
                                                putInd->bodyLength,
                                                putFile);

                                    CsrBtFclose(putFile);
                                    printf("Saved \"%s\" in SyncFiles.\n", fileName);

                                    printf("Saved \"%s\" in SyncFiles.\n", name);
                                }
                            }

                            pos = (CsrUint8*)CsrBtStrRChr((char*)fileName, '.');
                            *pos = '\0';

                            if (putInd->finalFlag)
                            {
                                char *temp;
                                CsrUint8 lengthChangeCounter = 2;
                                CsrUint16 lengthName;

                                syncServerInstanceData->calChangeCounter++;

                                putFile = CsrBtFopen("syncfiles"PATH_SEP"cal_cc.log", "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cal_cc.log");
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    sprintf(buffer,"%d\n\r", syncServerInstanceData->calChangeCounter);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFclose(putFile);
                                }

                                strncpy(fileName, "syncfiles"PATH_SEP"cal_", MAX_SIZE_OF_FILENAME);
                                length = sprintf(buffer,"%d",syncServerInstanceData->calChangeCounter);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                putFile = CsrBtFopen(fileName, "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                    CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                    CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                    sprintf(buffer, "%d\n", syncServerInstanceData->calNumberOfRecords);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                    CsrBtFclose(putFile);
                                }

                                lengthChangeCounter = strlen(buffer) + 1;
                                lengthName = CsrStrLen((char*)name) + 1;

                                temp = CsrPmemAlloc(lengthName+MAX_TEMP_BUFFER_LENGTH);

                                length = sprintf(temp, "%c%c%s%c%c%c%d", 1, lengthName, name, 0, 2,
                                                 lengthChangeCounter, syncServerInstanceData->calChangeCounter);
                                length++;
                                buffer_check((CsrUint16)(lengthName+MAX_TEMP_BUFFER_LENGTH), length);

                                appParameter = CsrPmemAlloc(length);
                                CsrMemCpy(appParameter, temp, length);
                                CsrPmemFree(temp);
                                CsrPmemFree(name);
                            }
                            else
                            {
                                appParameter = NULL;
                                savedPutIndData->type = CSR_BT_SYNCS_PUT_CAL_ENTRY_IND;
                                length = sprintf(savedPutIndData->lastfileSaved, fileName);
                                buffer_check(MAX_SIZE_OF_FILENAME, length);
                                length = 0;
                                CsrPmemFree(savedPutIndData->lastPutIndName);
                                savedPutIndData->lastPutIndName = name;
                                savedPutIndData->lengthName = CsrStrLen((char*)name) + 1;
                            }
                            if (putInd->finalFlag)
                            {
                                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                            }
                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;
#if !defined(VCALENDAR_SUPPORT_DISABLED)
                            CsrBtSyncsPutCalEntryResSend(putInd->connectionId, responseCode, length, appParameter, FALSE);
#else
                            CsrBtSyncsPutCalEntryResSend(putInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(appParameter);
#endif
                            CsrPmemFree(putInd->payload);
                            break;
                        }

                    case CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND:
                        {
                            CsrBtSyncsPutCalAddEntryInd * putInd;
                            CsrFileHandle            *putFile;
                            char            fileName[MAX_SIZE_OF_FILENAME];
                            CsrBtObexResponseCode   responseCode;
                            char            buffer[MAX_TEMP_BUFFER_LENGTH];
                            CsrUint16        length;
                            CsrUint8         *appParameter;

                            putInd = (CsrBtSyncsPutCalAddEntryInd *) syncServerInstanceData->message;

                            printf("CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND received\n");

                            strncpy(fileName, fileUnknown, MAX_SIZE_OF_FILENAME);
                            length = sprintf(buffer,"0x%x",syncServerInstanceData->calLuid);
                            buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                            strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                            strncat(fileName, ".vcs", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                            putFile = CsrBtFopen(fileName, "wb");

                            if(putFile == NULL)
                            {
                                printf("Could not open putFile '%s'\n", fileName);
                            }
                            else
                            {
                                CsrBtFwrite(putInd->payload + putInd->bodyOffset,
                                            1,
                                            putInd->bodyLength,
                                            putFile);

                                CsrBtFclose(putFile);
                                printf("Saved \"%X\".vcs in SyncFiles filename %s.\n", syncServerInstanceData->calLuid, fileName);
                            }

                            if (putInd->finalFlag)
                            {
                                char temp[MAX_TEMP_BUFFER_LENGTH];
                                char buffer[MAX_TEMP_BUFFER_LENGTH];
                                CsrUint8 lengthChangeCounter = 2;
                                CsrUint8 lengthCalLuid = 4;

                                syncServerInstanceData->calChangeCounter++;

                                putFile = CsrBtFopen("syncfiles"PATH_SEP"cal_cc.log", "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", "syncfiles"PATH_SEP"cal_cc.log");
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    sprintf(buffer,"%d\n\r", syncServerInstanceData->calChangeCounter);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFclose(putFile);
                                }

                                strncpy(fileName, "syncfiles"PATH_SEP"cal_", MAX_SIZE_OF_FILENAME);
                                length = sprintf(buffer,"%d",syncServerInstanceData->calChangeCounter);
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                strncat(fileName, buffer, (MAX_SIZE_OF_FILENAME-strlen(fileName)));
                                strncat(fileName, ".log", (MAX_SIZE_OF_FILENAME-strlen(fileName)));

                                putFile = CsrBtFopen(fileName, "w");

                                if(putFile == NULL)
                                {
                                    printf("Could not open putFile '%s'\n", fileName);
                                }
                                else
                                {
                                    char buffer[MAX_TEMP_BUFFER_LENGTH];

                                    CsrBtFwrite(serielnr, 1, CsrStrLen(serielnr), putFile);
                                    CsrBtFwrite(databaseId, 1, CsrStrLen(databaseId), putFile);
                                    CsrBtFwrite(totalRecords, 1, CsrStrLen(totalRecords), putFile);
                                    sprintf(buffer, "%d\n", syncServerInstanceData->calNumberOfRecords);
                                    CsrBtFwrite(buffer, 1, CsrStrLen(buffer), putFile);
                                    CsrBtFwrite(maximumRecords, 1, CsrStrLen(maximumRecords), putFile);
                                    CsrBtFclose(putFile);
                                }

                                length = sprintf(buffer,"0x%x",syncServerInstanceData->calLuid);
                                lengthCalLuid = strlen(buffer) + 1;
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                length = sprintf(buffer,"%d",syncServerInstanceData->calChangeCounter);
                                lengthChangeCounter = strlen(buffer) + 1;
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                length = sprintf(temp, "%c%c%X%c%c%c%d", 1, lengthCalLuid,
                                                 syncServerInstanceData->calLuid, 0, 2,
                                                 lengthChangeCounter, syncServerInstanceData->calChangeCounter);
                                length++;
                                buffer_check(MAX_TEMP_BUFFER_LENGTH, length);

                                appParameter = CsrPmemAlloc(length);
                                CsrMemCpy(appParameter, temp, length);

                                syncServerInstanceData->calLuid++;
                                syncServerInstanceData->calNumberOfRecords++;
                            }
                            else
                            {
                                appParameter = NULL;
                                savedPutIndData->type = CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND;
                                length = sprintf(savedPutIndData->lastfileSaved, fileName);
                                buffer_check(MAX_SIZE_OF_FILENAME, length);
                                length = 0;
                            }

                            if (putInd->finalFlag)
                            {
                                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                            }

                            syncServerInstanceData->objectType = CALENDAR_OBJECT_TYPE;


#if !defined(VCALENDAR_SUPPORT_DISABLED)
                            CsrBtSyncsPutCalAddEntryResSend(putInd->connectionId, responseCode, length, appParameter, FALSE);
#else
                            CsrBtSyncsPutCalAddEntryResSend(putInd->connectionId, CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE, 0, NULL, FALSE);
                            CsrPmemFree(appParameter);
#endif
                            CsrPmemFree(putInd->payload);
                            length = sprintf(syncServerInstanceData->lastfileSaved, fileName);
                            buffer_check(MAX_SIZE_OF_FILENAME, length);
                            break;
                        }

/*********************** General primitives *******************************/

                    case CSR_BT_SYNCS_DISCONNECT_IND:
                        {

                            CsrBtSyncsDisconnectInd * disconnectInd;
                            disconnectInd = (CsrBtSyncsDisconnectInd *) syncServerInstanceData->message;
                            syncServerInstanceData->clientAuthenticatedSuccessful = FALSE;

                            printf("CSR_BT_SYNCS_DISCONNECT_IND received\n");
                            break;
                        }

                    case CSR_BT_SYNCS_ABORT_IND:
                        {
                            CsrBtSyncsAbortInd    *abortInd;
                            CsrUint8          *description;

                            abortInd = (CsrBtSyncsAbortInd *) syncServerInstanceData->message;
                            description = CsrUcs2ByteString2Utf8(abortInd->payload + abortInd->descriptionOffset);
                            printf("CSR_BT_SYNCS_ABORT_IND received description %s\n", description);
                            CsrPmemFree(abortInd->payload);
                            CsrPmemFree(description);
                            break;
                        }

                    case CSR_BT_SYNCS_DEACTIVATE_CFM:
                        {
                            CsrBtSyncsDeactivateCfm * deactivateCfm;
                            deactivateCfm = (CsrBtSyncsDeactivateCfm *) syncServerInstanceData->message;
                            syncServerInstanceData->clientAuthenticatedSuccessful = FALSE;
                            printf("Sync Server Deactivated...\n");
                            showMenu(syncServerInstanceData);
                            break;
                        }

                    default:
                        CsrBtSyncsFreeUpstreamMessageContents(syncServerInstanceData->event,
                                                              syncServerInstanceData->message);
                        break;
                }
                break;
            }

        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;
                key = (CsrBtKeyPress_t*)syncServerInstanceData->message;

                if (key->key == ESCAPE_KEY)
                {
                    printf("\nUser exit...\n");
                    CsrPmemFree(syncServerInstanceData->message);
#ifdef ENABLE_SHUTDOWN
                    CsrSchedStop();
#else
                    exit(0);
#endif
                    return;
                }
                else
                {
                    switch (syncServerInstanceData->keyboardState)
                    {
                        case IDLE_KEYBOARD_STATE:
                            break;

                        case PASSKEY_KEYBOARD_STATE:
                            {
                                handleScKeybPrim(&syncServerInstanceData->scInstData);
                                break;
                            }

                        case OBEX_PASSKEY_KEYBOARD_STATE:
                            {
                                if (key->key == RETURN_KEY)
                                {
                                    if (!syncServerInstanceData->syncsPasswordLength)
                                    {
                                        CsrBtSyncsAuthenticateResSend(4, (CsrUint8*)CsrStrDup("1234"), CsrStrDup("CSR-SYNC"));
                                    }
                                    else
                                    {
                                        CsrBtSyncsAuthenticateResSend(syncServerInstanceData->syncsPasswordLength,
                                                                      (CsrUint8*)CsrStrDup(syncServerInstanceData->syncsPassword),
                                                                      CsrStrDup("CSR-SYNC"));
                                    }
                                    syncServerInstanceData->keyboardState = MODE_SELECT_KEYBOARD_STATE;
                                    printf("\n");
                                }
                                else if (key->key == BACKSPACE_KEY)
                                {
                                    if (syncServerInstanceData->syncsPasswordLength > 0)
                                    {
                                        syncServerInstanceData->syncsPasswordLength--;
                                        printf("\b \b");
                                    }
                                }
                                else if ((key->key >= 32) && (key->key < 127))
                                {
                                    if (syncServerInstanceData->syncsPasswordLength < 16)
                                    {
                                        syncServerInstanceData->syncsPassword[syncServerInstanceData->syncsPasswordLength++] = key->key;
                                        printf("%c", key->key);
                                    }
                                }
                                else
                                {
                                    ;
                                }
                                break;
                            }

                        case ENTER_OBEX_PASSKEY_KEYBOARD_STATE:
                            {
                                if (key->key == RETURN_KEY)
                                {
                                    syncServerInstanceData->keyboardState = MODE_SELECT_KEYBOARD_STATE;
                                    syncServerInstanceData->syncsPassword[syncServerInstanceData->syncsPasswordLength] = 0;
                                    printf("\n");
                                    printf("Password stored!\n");
                                }
                                else if (key->key == BACKSPACE_KEY)
                                {
                                    if (syncServerInstanceData->syncsPasswordLength > 0)
                                    {
                                        syncServerInstanceData->syncsPasswordLength--;
                                        printf("\b \b");
                                    }
                                }
                                else if ((key->key >= 32) && (key->key < 127))
                                {
                                    if (syncServerInstanceData->syncsPasswordLength < 16)
                                    {
                                        syncServerInstanceData->syncsPassword[syncServerInstanceData->syncsPasswordLength++] = key->key;
                                        printf("%c", key->key);
                                    }
                                }
                                else
                                {
                                    ;
                                }
                                break;
                            }

                        case MODE_SELECT_KEYBOARD_STATE:
                            {
                                switch (key->key)
                                {
                                    case '1':
                                        {
                                            syncServerInstanceData->obexAuthentication = WITHOUT_AUTHENTICATION;
                                            syncServerInstanceData->modeSelect = 1;
                                            writeFile(syncServerInstanceData, 1);
                                            CsrBtSyncsActivateReqSend(syncServerInstanceData->CsrSchedQid,
                                                                      CSR_BT_SYNC_PHONEBOOK_SUPPORT,
                                                                      CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                                                      0, /* windowSize */
                                                                      TRUE); /*srmEnable*/
                                            system(CLS);
                                            printf("Sync Server activated...\n");
                                            showMenu(syncServerInstanceData);
                                            break;
                                        }
                                    case '2':
                                        {
                                            syncServerInstanceData->obexAuthentication = WITHOUT_AUTHENTICATION;
                                            syncServerInstanceData->modeSelect = 2;
                                            writeFile(syncServerInstanceData, 2);
                                            CsrBtSyncsActivateReqSend(syncServerInstanceData->CsrSchedQid,
                                                                      (CSR_BT_SYNC_PHONEBOOK_SUPPORT | CSR_BT_SYNC_CALENDAR_SUPPORT),
                                                                      CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                                                      0, /* windowSize */
                                                                      TRUE); /*srmEnable*/

                                            system(CLS);
                                            printf("Sync Server activated...\n");
                                            showMenu(syncServerInstanceData);
                                            break;
                                        }
                                    case '3':
                                        {
                                            syncServerInstanceData->obexAuthentication = WITH_AUTHENTICATION;
                                            syncServerInstanceData->modeSelect = 3;
                                            writeFile(syncServerInstanceData, 3);
                                            CsrBtSyncsActivateReqSend(syncServerInstanceData->CsrSchedQid,
                                                                      CSR_BT_SYNC_PHONEBOOK_SUPPORT,
                                                                      CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                                                      0, /* windowSize */
                                                                      TRUE); /*srmEnable*/

                                            system(CLS);
                                            printf("Sync Server activated...\n");
                                            showMenu(syncServerInstanceData);
                                            break;
                                        }
                                    case '4':
                                        {
                                            syncServerInstanceData->obexAuthentication = WITH_AUTHENTICATION;
                                            syncServerInstanceData->modeSelect = 4;
                                            writeFile(syncServerInstanceData, 4);
                                            CsrBtSyncsActivateReqSend(syncServerInstanceData->CsrSchedQid,
                                                                      (CSR_BT_SYNC_PHONEBOOK_SUPPORT | CSR_BT_SYNC_CALENDAR_SUPPORT),
                                                                      CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                                                      0, /* windowSize */
                                                                      TRUE); /*srmEnable*/

                                            system(CLS);
                                            printf("Sync Server activated...\n");
                                            showMenu(syncServerInstanceData);
                                            break;
                                        }
                                    case '5':
                                        {
                                            handleScKeybPrim(&syncServerInstanceData->scInstData);
                                            break;
                                        }
                                    case '6':
                                        {
                                            syncServerInstanceData->keyboardState = ENTER_OBEX_PASSKEY_KEYBOARD_STATE;
                                            syncServerInstanceData->syncsPasswordLength = 0;
                                            printf("Enter obex initiate passkey : ");
                                            break;
                                        }
                                    case '9':
                                        {
                                            CsrBtSyncsDeactivateReqSend();
                                            syncServerInstanceData->modeSelect = 0;
                                            syncServerInstanceData->keyboardState = MODE_SELECT_KEYBOARD_STATE;
                                            break;
                                        }
                                    default:
                                        {
                                            break;
                                        }
                                }
                                break;
                            }
                    }
                }
                break;
            }

        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&syncServerInstanceData->scInstData);
                break;
            }

        default:
            break;
    }

    CsrPmemFree(syncServerInstanceData->message);
}

void applicationUsage(char *col, char *des, char *opt)
{
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    return NULL;
}

void applicationSetup(void)
{
}

char* applicationExtraOptions(void)
{
    return "";
}
