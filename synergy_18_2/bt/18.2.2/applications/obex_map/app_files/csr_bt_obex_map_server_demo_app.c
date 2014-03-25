/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <ctype.h>

#ifdef __linux__
#include <unistd.h>
#endif

#include <time.h>
#include <math.h>
#include <errno.h>
#include "csr_sched.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_maps_prim.h"
#include "csr_bt_maps_lib.h"
#include "csr_bt_xml_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_platform.h"
#include "csr_sched_init.h"

#include "csr_unicode.h"

#define KEY_CHECK_TIMEOUT               100000
#define MAX_PACKET_SIZE                 0x4000

#define MAPS_TIME_FORMAT_LENGTH         (16)
#define MAPS_SIZE_FORMAT_LENGTH         (12)

#define MAPS_FILENAME_LENGTH            (20) /* 16 digits + ".msg" */

#define XML_VALUE_DELIMITER "\"" /* This is correct XML delimiter */
#define MAP_MESSAGE_HANDLE_ATTR                 "<msg"
#define MAP_MESSAGE_HANDLE_BEGIN_ATTR           " handle = "
#define MAP_MESSAGE_SUBJECT_BEGIN_ATTR          " subject = "
#define MAP_MESSAGE_DATETIME_BEGIN_ATTR         " datetime = "
#define MAP_MESSAGE_SENDER_NAME_BEGIN_ATTR      " sender_name = "
#define MAP_MESSAGE_SENDER_ADDR_BEGIN_ATTR      " sender_addressing = "
#define MAP_MESSAGE_REPLYTO_ADDR_BEGIN_ATTR     " replyto_addressing = "
#define MAP_MESSAGE_RECIPIENT_NAME_BEGIN_ATTR   " recipient_name = "
#define MAP_MESSAGE_RECIPIENT_ADDR_BEGIN_ATTR   " recipient_addressing = "
#define MAP_MESSAGE_TYPE_BEGIN_ATTR             " type = "
#define MAP_MESSAGE_SIZE_BEGIN_ATTR             " size = "
#define MAP_MESSAGE_TEXT_BEGIN_ATTR             " text = "
#define MAP_MESSAGE_RECEPTION_STATUS_BEGIN_ATTR " reception_status = "
#define MAP_MESSAGE_ATTACHMENT_SIZE_BEGIN_ATTR  " attachment_size = "
#define MAP_MESSAGE_PRIORITY_BEGIN_ATTR         " priority = "
#define MAP_MESSAGE_READ_BEGIN_ATTR             " read = "
#define MAP_MESSAGE_SENT_BEGIN_ATTR             " sent = "
#define MAP_MESSAGE_PROTECTED_BEGIN_ATTR        " protected = "
#define MAP_MESSAGE_LIST_END_ATTR               "/>\n"

#define XML_HEADER1                     "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define XML_HEADER2                     "<!DOCTYPE folder-listing SYSTEM \"obex-folder-listing.dtd\">\n"
#define XML_HEADER3                     "<folder-listing version=\"1.0\">\n"
#define XML_FILE                        "\t<file name=\"%s\" modified=\"%04d%02d%02dT%02d%02d%02dZ\" size=\"%ld\" user-perm=\"%s\"/>\n"
#define XML_FOLDER                      "\t<folder name=\"%s\" modified=\"%04d%02d%02dT%02d%02d%02dZ\"/>\n"
#define XML_MSG_LISTING_HEADER          "<MAP-msg-listing version = \"1.0\">\n"
#define XML_MSG_LISTING_FOOTER          "</MAP-msg-listing>\n"
#define XML_EVENT_NOTIFICATION_HEADER   "<MAP-event-report version = \"1.0\">\n<event"
#define XML_EVENT_NOTIFICATION_FOOTER   " />\n</MAP-event-report>\n"

#define MAX_LINE_LENGTH     250

#define EVENT_NOTIFICATION_KEY_STATE_NULL               0x00
#define EVENT_NOTIFICATION_KEY_STATE_SELECT_EVENT       0x01
#define EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE      0x02
#define EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER      0x03
#define EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER  0x04
#define EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE    0x05
#define EVENT_NOTIFICATION_KEY_STATE_DONE               0xff


#define EVENT_NOTIFICATION_EVENT_TYPE_NEW_MESSAGE       " type = \"NewMessage\""
#define EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_SUCCESS  " type = \"DeliverySuccess\""
#define EVENT_NOTIFICATION_EVENT_TYPE_SENDING_SUCCESS   " type = \"SendingSuccess\""
#define EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_FAILURE  " type = \"DeliveryFailure\""
#define EVENT_NOTIFICATION_EVENT_TYPE_SENDING_FAILURE   " type = \"SendingFailure\""
#define EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_FULL       " type = \"MemoryFull\""
#define EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_AVAILABLE  " type = \"MemoryAvailable\""
#define EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_DELETED   " type = \"MessageDeleted\""
#define EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_SHIFT     " type = \"MessageShift\""
#define EVENT_NOTIFICATION_HANDLE_INBOX_1               " handle = \"1000000000000001\""
#define EVENT_NOTIFICATION_HANDLE_INBOX_2               " handle = \"1000000000000002\""
#define EVENT_NOTIFICATION_HANDLE_INBOX_3               " handle = \"1000000000000003\""
#define EVENT_NOTIFICATION_HANDLE_OUTBOX_1              " handle = \"2000000000000001\""
#define EVENT_NOTIFICATION_HANDLE_OUTBOX_2              " handle = \"2000000000000002\""
#define EVENT_NOTIFICATION_HANDLE_OUTBOX_3              " handle = \"2000000000000003\""
#define EVENT_NOTIFICATION_HANDLE_SENT_1                " handle = \"3000000000000001\""
#define EVENT_NOTIFICATION_HANDLE_SENT_2                " handle = \"3000000000000002\""
#define EVENT_NOTIFICATION_HANDLE_SENT_3                " handle = \"3000000000000003\""
#define EVENT_NOTIFICATION_HANDLE_DELETED_1             " handle = \"1000000000000001\""
#define EVENT_NOTIFICATION_HANDLE_DELETED_2             " handle = \"1000000000000002\""
#define EVENT_NOTIFICATION_HANDLE_DELETED_3             " handle = \"2000000000000001\""
#define EVENT_NOTIFICATION_FOLDER_INBOX                 " folder = \"telecom/msg/inbox\""
#define EVENT_NOTIFICATION_FOLDER_OUTBOX                " folder = \"telecom/msg/outbox\""
#define EVENT_NOTIFICATION_FOLDER_SENT                  " folder = \"telecom/msg/sent\""
#define EVENT_NOTIFICATION_FOLDER_DELETED               " folder = \"telecom/msg/deleted\""
#define EVENT_NOTIFICATION_OLD_FOLDER_INBOX             " old_folder = \"telecom/msg/inbox\""
#define EVENT_NOTIFICATION_OLD_FOLDER_OUTBOX            " old_folder = \"telecom/msg/outbox\""
#define EVENT_NOTIFICATION_OLD_FOLDER_SENT              " old_folder = \"telecom/msg/sent\""
#define EVENT_NOTIFICATION_OLD_FOLDER_DELETED           " old_folder = \"telecom/msg/deleted\""
#define EVENT_NOTIFICATION_TYPE_EMAIL                   " msg_type = \"EMAIL\""
#define EVENT_NOTIFICATION_TYPE_SMS_GSM                 " msg_type = \"SMS_GSM\""
#define EVENT_NOTIFICATION_TYPE_SMS_CDMA                " msg_type = \"SMS_CDMA\""
#define EVENT_NOTIFICATION_TYPE_MMS                     " msg_type = \"MMS\""



/* Global app states */
typedef enum
{
    STATE_IDLE,
    STATE_ACTIVATING,
    STATE_ACTIVATED,
    STATE_PAIRING,
    STATE_SELECT_MAPS_INSTANCE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_SELECT_MAS_INSTANCE_ID,
    STATE_DEACTIVATING,
} MapsAppState_t;

typedef struct FolderInfoTag
{
    char                  *name;
    unsigned short        mode;
    CsrUint32             size;
    CsrTime              time;
    struct FolderInfoTag  *nextInfoElement;
} FolderInfo;

typedef struct MsgListStrTag
{
    char                  *str;
    CsrUint32             length;
    struct MsgListStrTag  *next;
} MsgListStr;

typedef struct FileAttribType
{
    char *senderName;
    char *senderFormatName;
    char *senderEmail;
    char *senderPhone;
    char *replyToAddr;
    char *recipientName;
    char *recipientFormatName;
    char *recipientEmail;
    char *recipientPhone;
    char *msgType;
    CsrBool msgText;
    char *msgFolder;
    char *msgPart;
    char *msgBodyEncoding;
    char *msgBodyCharset;
    char *msgBodyLanguage;
    char *msgBodyLength;
    char *msgBody;
    char *subject;
    char *receptionStatus;
    char *attachmentSize;
    CsrBool priority;
    CsrBool read;
    CsrBool sent;
    CsrBool protect;
}FileAttribType;

/* Textual representation of states */
static char *MapsAppStateString[] =
{
    "Idle",
    "Activating",
    "Activated",
    "Pairing",
    "Select MAPS instance",
    "Connecting",
    "Connected",
    "Select MAS instance ID",
    "Deactivating",
};

typedef struct MapsMsgInfo_t
{
    char              *subject;
    char              *datetime;
    char              *type;
    char              *read;
    char              *sent;
} MessageListElement;

/* File info container */
struct MapsInfo_t
{
    char               *name; /* Use to store msg_handle in case of message listing */
    CsrUint32          size;
    CsrBool            is_dir;
    MessageListElement msgListElement;
    struct MapsInfo_t  *next;
};

typedef struct
{
    CsrSchedQid              mapsInstanceId;                              /* The MAPS instance Id which this instance data belongs to */
    MapsAppState_t      state;                                       /* Global app state */
    MapsAppState_t      state_old;                                   /* Old app state */
    CsrBtDeviceAddr     address;                                     /* Remote active address */
    char                *dir_local;                                  /* Local directory */
    char                *dir_local_root;                             /* Local root directory */
    char                *getFileName;
    char                *folder;                                     /* Folder listing data */
    CsrUint32           folder_idx;                                  /* Folder listing index */
    CsrUint32           folder_size;                                 /* Folder listing size */
    CsrFileHandle                *file;                                       /* File transfer */
    CsrUint32           file_idx;                                    /* Transfer index */
    CsrUint32           file_size;                                   /* Transfer total size */
    CsrUint16           fileSizeRemain;
    char                *message;                                    /* Remote message listing data */
    CsrUint32           message_idx;                                 /* Remote message listing index */
    CsrUint32           message_size;                                /* Remote message listing size */

    CsrBool             registerNotificationStatus;                  /* Used for keeping track of the current MNS status */
    CsrBool             eventNotificationPending;
    CsrUint8            eventNotifiactionKeyState;
    char                *eventNotificationBody;
    CsrUint16           eventNotificationBytesSent;
    CsrUint16           eventNotificationBytesAllowed;
    CsrSchedQid              eventNotificationInstanceId;
    CsrBool             eventNotificationOldFolderNeeded;

    CsrBtMapsMasInstance *masInstanceList;
    CsrUint16           masInstanceListSize;

    CsrUint8            tempValue[6];
    CsrUint8            tempValueLen;

    MsgListStr          msgListing;

    CsrBool             simulateDirectSend;
} MapsInstance;

/* Instance data for application */
typedef struct
{
    CsrSchedQid              qid;                            /* This demo applications queue id (not to be confused with the mapsInstanceIds*/
    CsrUint16           event;                          /* Message id */
    void                *msg;                           /* Message body */

    CsrBool             mapsInstanceListReceived;
    CsrUint8            numberOfInstances;
    CsrUint8            activeInst;

    char                *cls;                           /* Path to CLS */

    MapsInstance        *mapsInst;
    ScAppInstData_t     scInstData;
    CsrUint8            nextMapsInstanceId;
} MapsAppInst;

/* Macro to get the maps instance data based on the activeInst parameter which is specified by the user */
#define CURRENT_INST (inst->mapsInst[inst->activeInst])

/* Macro to get the maps instance data based on the instanceId parameter received in an event from the profile */
#define INST_DATA (&inst->mapsInst[getMapsInstanceNumber(inst, prim->instanceId)])

/* Macro to get the maps instance Id */
#define INST_ID inst->mapsInstanceId

/* Prototypes */
void handleKeyboardEvents(MapsAppInst *inst);

static CsrUint8 *unicode2String(CsrUcs2String* unicode)
{
    CsrUint8 *str;

    str = CsrUcs2ByteString2Utf8(unicode);
    return str;
}

static CsrUint8 getMapsInstanceNumber(MapsAppInst * inst, CsrSchedQid instanceId)
{
    CsrUint8 i;
    for(i=0;i<inst->numberOfInstances;i++)
    {
        if(inst->mapsInst[i].mapsInstanceId == instanceId)
        {
            return i;
        }
    }
    return 0xFF; /* We should never get here */
}

/* Change state */
void stateChange(MapsInstance *inst, MapsAppState_t s)
{
    inst->state_old = inst->state;
    inst->state = s;
}

/* Clean instance */
void cleanInstance(MapsInstance *inst, CsrBool disconnect)
{
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;
    char            rootSubDir[7] = PATH_SEP"root";

    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        inst->address.lap = defGlobalBdAddr.lap;
        inst->address.nap = defGlobalBdAddr.nap;
        inst->address.uap = defGlobalBdAddr.uap;
    }
    else
    {
        inst->address.nap = 0;
        inst->address.uap = 0;
        inst->address.lap = 0;
    }

    inst->registerNotificationStatus = FALSE;
    inst->eventNotificationPending = FALSE;
    inst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_NULL;
    inst->eventNotificationBytesSent = 0;
    inst->eventNotificationBytesAllowed = 0;
    inst->eventNotificationInstanceId = 0;
    inst->eventNotificationOldFolderNeeded = FALSE;

    /* Setup special instance variables */
    if (disconnect)
    {
        inst->state_old = STATE_ACTIVATED;
        stateChange(inst, STATE_ACTIVATED);
    }
    else
    {
        inst->state_old = STATE_IDLE;
        stateChange(inst, STATE_IDLE);
    }

    if (!disconnect)
    {
        CsrPmemFree(inst->dir_local_root);
        inst->dir_local_root = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
        CsrBtGetCwd(inst->dir_local_root, CSR_BT_MAX_PATH_LENGTH - CsrStrLen(rootSubDir));
        CsrStrCat(inst->dir_local_root, &rootSubDir[0]);
        CsrStrCat(inst->dir_local_root, PATH_SEP);
    }
    CsrPmemFree(inst->dir_local);
    inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    sprintf(inst->dir_local, inst->dir_local_root);

    if(inst->file)
    {
        CsrBtFclose(inst->file);
    }
    inst->file = NULL;
    inst->file_idx = 0;
    inst->file_size = 0;

    inst->simulateDirectSend = FALSE;
}

/* Show menu options */
void playMenu(MapsAppInst *inst)
{
    CsrUint8 i;
    system(inst->cls);
    printf("\nCSR plc -- Synergy BT Message Access Server demo application\n");
    printf("Hit <ESC> to quit. Hit <SPACE> to reprint menu.\n\n");

    if(inst->mapsInstanceListReceived)
    {
        printf("List of MAPS Instances:\n");
        for(i=0;i < inst->numberOfInstances; i++)
        {
            printf("\t MapsInstanceId[%02d] = 0x%04X, MNS registration = %s, %s\n",i,inst->mapsInst[i].mapsInstanceId,(inst->mapsInst[i].registerNotificationStatus ? "ON" : "OFF"), (i == inst->activeInst ? " *Selected*" : "Not selected"));
        }

        printf("\nState...............%s (%d)\n", MapsAppStateString[inst->mapsInst[inst->activeInst].state], inst->mapsInst[inst->activeInst].state);
        printf("Active address......%04X:%02X:%06lX\n",
               inst->mapsInst[inst->activeInst].address.nap,
               inst->mapsInst[inst->activeInst].address.uap,
               inst->mapsInst[inst->activeInst].address.lap);
        printf("Local dir...........'%s'\n", inst->mapsInst[inst->activeInst].dir_local);
        printf("Local root dir......'%s'\n", inst->mapsInst[inst->activeInst].dir_local_root);
        printf("\nMenu selection:\n");

        /* Options for (almost) all states */
        switch(inst->mapsInst[inst->activeInst].state)
        {
            case STATE_IDLE:
                {
                    printf("  a) Activate MAPS instance\n");
                    break;
                }
            case STATE_ACTIVATING:
                {
                    printf("  x) Cancel activation of MAPS instance\n");
                    break;
                }
            case STATE_ACTIVATED:
                {
                    printf("  x) Deactivate MAPS instance\n");
                    break;
                }
            case STATE_CONNECTING:
                {
                    printf("\n\n  Choose OBEX response code:\n");
                    printf("      0)  Success             (0x20)\n");
                    printf("      1)  Bad request         (0x40)\n");
                    printf("      2)  Not implemented     (0x51)\n");
                    printf("      3)  Unauthorized        (0x41)\n");
                    printf("      4)  Precondition failed (0x4C)\n");
                    printf("      5)  Not found           (0x44)\n");
                    printf("      6)  Not acceptable      (0x46)\n");
                    printf("      7)  Service unavailable (0x53)\n");
                    printf("      8)  Forbidden           (0x43)\n");

                    printf("\n  x) Deactivate MAPS instance\n");
                    break;
                }
            case STATE_CONNECTED:
                {
                    printf("  n) Send Event Notification\n");
                    printf("\n  x) Deactivate MAPS instance\n");
                    break;
                }
            default:
                break;
        }
        if (inst->mapsInst[inst->activeInst].simulateDirectSend)
        {
            printf("\n  s) Stop simulating that messages pushed to outbox are sent\n");
        }
        else
        {
            printf("\n  s) Simulate sending messages pushed to outbox - NEEDED FOR PTS TESTING\n");
        }
        printf("\n  z) Change MAPS instance\n");
    }
    else
    {
        printf("Waiting for MAPS registration to complete \n");
    }
    printf("\n");
}

static void printEventNotificationSubMenu(MapsInstance *inst)
{
    switch (inst->eventNotifiactionKeyState)
    {
        case EVENT_NOTIFICATION_KEY_STATE_NULL:
        case EVENT_NOTIFICATION_KEY_STATE_DONE: /* Fall-through */
            {
                /* Print nothing */
                break;
            }
        case EVENT_NOTIFICATION_KEY_STATE_SELECT_EVENT:
            {
                printf("\n\n");
                printf("   Choose Event type:\n");
                printf("      1)   New Message\n");
                printf("      2)   Delivery success\n");
                printf("      3)   Sending success\n");
                printf("      4)   Delivery failure\n");
                printf("      5)   Sending failure\n");
                printf("      6)   Memory full\n");
                printf("      7)   Memory available\n");
                printf("      8)   Message deleted\n");
                printf("      9)   Message shift\n");
                break;
            }
        case EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE:
            {
                printf("\n\n");
                printf("   Choose Handle:\n");
                printf("      1)   1000000000000001 (inbox)\n");
                printf("      2)   1000000000000002 (inbox)\n");
                printf("      3)   1000000000000003 (inbox)\n");
                printf("      4)   2000000000000001 (outbox)\n");
                printf("      5)   2000000000000002 (outbox)\n");
                printf("      6)   2000000000000003 (outbox)\n");
                printf("      7)   3000000000000001 (sent)\n");
                printf("      8)   3000000000000002 (sent)\n");
                printf("      9)   3000000000000003 (sent)\n");
                printf("      a)   1000000000000001 (deleted)\n");
                printf("      b)   1000000000000002 (deleted)\n");
                printf("      c)   2000000000000001 (deleted)\n");
                break;
            }
        case EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER:
            {
                printf("\n\n");
                printf("   Choose new/current folder:\n");
                printf("      1)   telecom/msg/inbox\n");
                printf("      2)   telecom/msg/outbox\n");
                printf("      3)   telecom/msg/sent\n");
                printf("      4)   telecom/msg/deleted\n");
                break;
            }
        case EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER:
            {
                printf("\n\n");
                printf("   Choose old folder:\n");
                printf("      1)   telecom/msg/inbox\n");
                printf("      2)   telecom/msg/outbox\n");
                printf("      3)   telecom/msg/sent\n");
                printf("      4)   telecom/msg/deleted\n");
                break;
            }
        case EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE:
            {
                printf("\n\n");
                printf("   Choose message type:\n");
                printf("      1)   E-mail\n");
                printf("      2)   SMS (GSM)\n");
                printf("      3)   SMS (CDMA)\n");
                printf("      4)   MMS\n");
                break;
            }
        default:
            {
                printf("\n\n  !!! ERROR  !!! UNKNOWN STATE (0x%02X) !!!\n",
                                        inst->eventNotifiactionKeyState);
                break;
            }
    }
}

static void sendRemainingEventNotificationBytes(MapsInstance *inst)
{
    CsrUint8        *body = NULL;
    CsrUint16       bodyLength = 0;
    CsrBool         final = FALSE;

    if (inst->eventNotificationBytesAllowed > (CsrStrLen(inst->eventNotificationBody) - inst->eventNotificationBytesSent))
    {
        bodyLength = CsrStrLen(inst->eventNotificationBody) - inst->eventNotificationBytesSent;
        final = TRUE;
    }
    else
    {
        bodyLength = inst->eventNotificationBytesAllowed;
    }

    body = CsrPmemAlloc(bodyLength);

    CsrMemCpy(body, (&inst->eventNotificationBody[inst->eventNotificationBytesSent]), bodyLength);

    CsrBtMapsEventNotificationResSend(inst->eventNotificationInstanceId,
                                      (CsrBtObexResponseCode)(final ? CSR_BT_OBEX_SUCCESS_RESPONSE_CODE : CSR_BT_OBEX_CONTINUE_RESPONSE_CODE),
                                      bodyLength,
                                      body);
}

/* Free entire collection of message listing strings */
static void freeMsgListingStr(MsgListStr **msgListing)
{
    MsgListStr *tmp;

    if(*msgListing != NULL)
    {
        for(tmp = *msgListing; tmp != NULL; tmp = *msgListing)
        {
            CsrPmemFree(tmp->str);
            *msgListing = (*msgListing)->next;
            CsrPmemFree(tmp);
        }
    }
}

/* Free entire folder application list */
static void freeFolderInformation(FolderInfo ** folderInformation)
{
    FolderInfo *tmp;

    if(*folderInformation != NULL)
    {
        for(tmp = *folderInformation; tmp != NULL; tmp = *folderInformation)
        {
            CsrPmemFree(tmp->name);
            *folderInformation = (*folderInformation)->nextInfoElement;
            CsrPmemFree(tmp);
        }
    }
}

/* Read folder information */
static int getFolderInformation(char *filePattern, FolderInfo **folderInformation, CsrBool filesOnly)
{
    DIR         *dir;
    CsrUint32   noOfFiles = 0;
    CsrUint16   searchMode = S_IFDIR;

    if (filesOnly)
    {
        searchMode = S_IFREG;
    }

    if(*folderInformation != NULL)
    {
        freeFolderInformation(folderInformation);
        *folderInformation = NULL;
    }

    if((dir = CsrBtOpenDir(filePattern)) != NULL)
    {
        struct dirent *entry;
        FolderInfo *cur;
        FolderInfo *prev;

        prev = NULL;

        CsrBtChdir(filePattern);

        while((entry = CsrBtReadDir(dir)) != NULL)
        {
            CsrBtStat_t info;
            int res;

            CsrMemSet(&info, 0, sizeof(CsrBtStat_t));

            res = CsrBtStat(entry->d_name, &info);
            if (res != 0)
            {
                perror("Error");
            }
            if((res == 0) && (CsrStrLen(entry->d_name) > 0) &&
              ((info.st_mode & searchMode) == searchMode))
            {
                noOfFiles++;

                cur = CsrPmemAlloc(sizeof(FolderInfo));
                cur->nextInfoElement = NULL;
                cur->name = CsrPmemAlloc(CsrStrLen(entry->d_name) + 1);
                strcpy(cur->name, entry->d_name);
                if (info.st_ctime)
                {
                    cur->time = info.st_ctime;
                }
                else
                {
                    cur->time = 0;
                }
                cur->size = info.st_size;
                cur->mode = info.st_mode;

                if(*folderInformation == NULL)
                {
                    *folderInformation = cur;
                    prev          = cur;
                }
                else if(prev != NULL)
                {
                    prev->nextInfoElement = cur;
                    prev = cur;
                }
            }

        }
        CsrBtCloseDir(dir);
    }
    else
    {
        printf("Could not get folder information, error code %d\n", errno);
        return 0;
    }

    return noOfFiles;
}

static int getFolderListing(char *filePattern, char **folderListingObjectString)
{
    struct tm fileTime;
    FolderInfo *folderInformation = NULL;
    FolderInfo *currentFileInformation;
    char dummyString[256];
    long resultStringLength;

    folderInformation  = NULL;
    resultStringLength = 0;

    fileTime.tm_year = 0;
    fileTime.tm_mon = 0;
    fileTime.tm_mday = 0;
    fileTime.tm_hour = 0;
    fileTime.tm_min = 0;
    fileTime.tm_sec = 0;

    if(getFolderInformation(filePattern, &folderInformation, FALSE))
    {
        /* Begin counting number of chars in folder */
        resultStringLength += sprintf(dummyString, XML_HEADER1);
        resultStringLength += sprintf(dummyString, XML_HEADER2);
        resultStringLength += sprintf(dummyString, XML_HEADER3);

        if(CsrStrStr(filePattern, PATH_SEP) != NULL)
        {
            resultStringLength += sprintf(dummyString, "\t<parent-folder/>\n");
        }

        /* Directories */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
               (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
               (currentFileInformation->mode & S_IFDIR))
            {
                fileTime = *gmtime((time_t*)&currentFileInformation->time);
                resultStringLength += sprintf(dummyString,
                                              XML_FOLDER,
                                              currentFileInformation->name,
                                              fileTime.tm_year + 1900,
                                              fileTime.tm_mon + 1,
                                              fileTime.tm_mday,
                                              fileTime.tm_hour,
                                              fileTime.tm_min,
                                              fileTime.tm_sec);
            }
        }

        /* Files */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
               (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
               !(currentFileInformation->mode & S_IFDIR))
            {
                fileTime = *gmtime((time_t*)&currentFileInformation->time);
                resultStringLength += sprintf(dummyString,
                                              XML_FILE,
                                              currentFileInformation->name,
                                              fileTime.tm_year + 1900,
                                              fileTime.tm_mon + 1,
                                              fileTime.tm_mday,
                                              fileTime.tm_hour,
                                              fileTime.tm_min,
                                              fileTime.tm_sec,
                                              (long)currentFileInformation->size,
                                              (currentFileInformation->mode & S_IWRITE) ? "RWD" : "R");
            }
        }

        /* End count with newline and terminator */
        resultStringLength += sprintf(dummyString, "</folder-listing>\n");
        resultStringLength += 1;

        /* Start real generation */
        *folderListingObjectString = (char*)CsrPmemAlloc(resultStringLength);
        CsrMemSet(*folderListingObjectString, 0, resultStringLength);
        resultStringLength = 0;
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER1);
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER2);
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], XML_HEADER3);

        if(CsrStrStr(filePattern, PATH_SEP) != NULL)
        {
            resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                                          "\t<parent-folder/>\n");
        }

        /* Directories */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
               (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
               (currentFileInformation->mode & S_IFDIR))
            {
                fileTime = *gmtime((time_t*)&currentFileInformation->time);
                resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                                              XML_FOLDER,
                                              currentFileInformation->name,
                                              fileTime.tm_year + 1900,
                                              fileTime.tm_mon + 1,
                                              fileTime.tm_mday,
                                              fileTime.tm_hour,
                                              fileTime.tm_min,
                                              fileTime.tm_sec);
            }
        }

        /* Files */
        for(currentFileInformation = folderInformation;
            currentFileInformation;
            currentFileInformation = currentFileInformation->nextInfoElement)
        {
            if((CsrStrCmp(currentFileInformation->name, ".") != 0) &&
               (CsrStrCmp(currentFileInformation->name, "..") != 0) &&
               !(currentFileInformation->mode & S_IFDIR))
            {


                fileTime = *gmtime((time_t*)&currentFileInformation->time);
                resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength],
                                              XML_FILE,
                                              currentFileInformation->name,
                                              fileTime.tm_year + 1900,
                                              fileTime.tm_mon + 1,
                                              fileTime.tm_mday,
                                              fileTime.tm_hour,
                                              fileTime.tm_min,
                                              fileTime.tm_sec,
                                              (long)currentFileInformation->size,
                                              (currentFileInformation->mode & S_IWRITE) ? "RWD" : "R");
            }
        }
        resultStringLength += sprintf(&(*folderListingObjectString)[resultStringLength], "</folder-listing>\n");
        freeFolderInformation(&folderInformation);
    }
    else
    {
        *folderListingObjectString = NULL;
        return 0;
    }

    return 1;
}

/* List MAPS instances */
void listMapsInstances(MapsAppInst *inst)
{
    if(inst->mapsInstanceListReceived)
    {
        CsrUint8 i;
        stateChange(&CURRENT_INST, STATE_SELECT_MAPS_INSTANCE);

        printf("\nList of MAPS instances:\n\n");
        for(i=0;i < inst->numberOfInstances; i++)
        {
            printf("\t %d) MapsInstanceId[%02d] = 0x%04X, current status = %s\n",i,i,inst->mapsInst[i].mapsInstanceId, (i == inst->activeInst ? "Selected" : "Not selected"));
        }
    }
    else
    {
        printf("No MAPS instances available!\n");
    }
}

/* Check for keypresses (timed event) */
void keyboardTimer(CsrUint16 m, void *data)
{
    MapsAppInst *inst;
    inst = (MapsAppInst*)data;

    if(_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEY_CHECK_TIMEOUT, keyboardTimer, m, data);
}

static void initFileAttribType(FileAttribType *att)
{
    att->senderName = NULL;
    att->senderFormatName = NULL;
    att->senderEmail = NULL;
    att->senderPhone = NULL;
    att->replyToAddr = NULL;
    att->recipientName = NULL;
    att->recipientFormatName = NULL;
    att->recipientEmail = NULL;
    att->recipientPhone = NULL;
    att->msgType = NULL;
    att->msgText = FALSE;
    att->msgFolder = NULL;
    att->msgPart = NULL;
    att->msgBodyEncoding = NULL;
    att->msgBodyCharset = NULL;
    att->msgBodyLanguage = NULL;
    att->msgBodyLength = NULL;
    att->msgBody = NULL;
    att->subject = NULL;
    att->receptionStatus = NULL;
    att->attachmentSize = NULL;
    att->priority = FALSE;
    att->read = FALSE;
    att->sent = FALSE;
    att->protect = FALSE;
}

static CsrBool readMessageFile(FILE *fp, FileAttribType *attribs)
{
    char        line[MAX_LINE_LENGTH];
    char        *interpretPtr = NULL;
    char        *msgBody = CsrPmemAlloc(MAX_LINE_LENGTH);
    CsrBool     bbodyFound = FALSE;
    CsrBool     optionalLinesEndFound = FALSE;
    CsrBool     vCardEndFound = FALSE;
    CsrUint32   bodyBytesCaptured = 0;

    initFileAttribType(attribs);
    msgBody[0] = '\0';

    /* First line must be BEGIN:BMSG */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) == NULL ||
        CsrStrNCmp(line, "BEGIN:BMSG", CsrStrLen("BEGIN:BMSG")) != 0)
    {
        return FALSE;
    }
    /* This line must be VERSION:1.0 */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) == NULL ||
        CsrStrNCmp(line, "VERSION:1.0", CsrStrLen("VERSION:1.0")) != 0)
    {
        return FALSE;
    }
    /* This line must contain STATUS: */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
        CsrStrStr(line, "STATUS:"))
    {
        if (CsrStrStr(line, "UNREAD"))
        {
            attribs->read = FALSE;
        }
        else if (CsrStrStr(line, "READ"))
        {
            attribs->read = TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    /* This line must contain TYPE: */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
        CsrStrStr(line, "TYPE:"))
    {
        CsrPmemFree(attribs->msgType);
        attribs->msgType = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("TYPE:") + 1);
        sprintf(attribs->msgType, "%s", &(line[CsrStrLen("TYPE:")]));
        if (attribs->msgType[CsrStrLen(attribs->msgType) - 1] == '\n')
        {
            attribs->msgType[CsrStrLen(attribs->msgType) - 1] = '\0';
        }
    }
    else
    {
        return FALSE;
    }
    /* This line must contain FOLDER: */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
        CsrStrStr(line, "FOLDER:"))
    {
        CsrPmemFree(attribs->msgFolder);
        attribs->msgFolder = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("FOLDER:") + 1);
        sprintf(attribs->msgFolder, "%s", &(line[CsrStrLen("FOLDER:")]));
        if (attribs->msgFolder[CsrStrLen(attribs->msgFolder) - 1] == '\n')
        {
            attribs->msgFolder[CsrStrLen(attribs->msgFolder) - 1] = '\0';
        }
    }
    else
    {
        return FALSE;
    }
    /* The following lines may contain a VCARD for the message originator */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
        CsrStrStr(line, "BEGIN:VCARD"))
    {
        CsrBool vCardEndFound = FALSE;

        while (!vCardEndFound)
        {
            if (CsrBtFgets((char*)line, MAX_LINE_LENGTH, fp))
            {
                if(CsrStrStr(line, "VERSION:"))
                {
                    /* Ignore this line */
                }
                else if(CsrStrStr(line, "FN:"))
                {
                    CsrPmemFree(attribs->senderFormatName);
                    attribs->senderFormatName = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("FN:") + 1);
                    sprintf(attribs->senderFormatName, "%s", &(line[CsrStrLen("FN:")]));
                    if (attribs->senderFormatName[CsrStrLen(attribs->senderFormatName) - 1] == '\n')
                    {
                        attribs->senderFormatName[CsrStrLen(attribs->senderFormatName) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "N:"))
                {
                    CsrPmemFree(attribs->senderName);
                    attribs->senderName = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("N:") + 1);
                    sprintf(attribs->senderName, "%s", &(line[CsrStrLen("N:")]));
                    if (attribs->senderName[CsrStrLen(attribs->senderName) - 1] == '\n')
                    {
                        attribs->senderName[CsrStrLen(attribs->senderName) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "TEL:"))
                {
                    CsrPmemFree(attribs->senderPhone);
                    attribs->senderPhone = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("TEL:") + 1);
                    sprintf(attribs->senderPhone, "%s", &(line[CsrStrLen("TEL:")]));
                    if (attribs->senderPhone[CsrStrLen(attribs->senderPhone) - 1] == '\n')
                    {
                        attribs->senderPhone[CsrStrLen(attribs->senderPhone) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "EMAIL:"))
                {
                    CsrPmemFree(attribs->senderEmail);
                    attribs->senderEmail = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("EMAIL:") + 1);
                    sprintf(attribs->senderEmail, "%s", &(line[CsrStrLen("EMAIL:")]));
                    if (attribs->senderEmail[CsrStrLen(attribs->senderEmail) - 1] == '\n')
                    {
                        attribs->senderEmail[CsrStrLen(attribs->senderEmail) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "END:VCARD"))
                {
                    vCardEndFound = TRUE;
                }
                else
                {
                    /* vCard contains an unsupported attribute, which should be ignored */
                }
            }
            else
            {
                vCardEndFound = TRUE;
            }
        }
    }
    /* This line may be BEGIN:BENV */
    if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) == NULL ||
        CsrStrStr(&line[0], "BEGIN:BENV") != NULL)
    {
        /* The following lines may contain a VCARD for the message recipient */
        if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
            CsrStrStr(&line[0], "BEGIN:VCARD"))
        {
            CsrBool vCardEndFound = FALSE;

            while (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
                   !vCardEndFound)
            {
                if(CsrStrStr(line, "VERSION:"))
                {
                    /* Ignore this line */
                }
                else if(CsrStrStr(line, "FN:"))
                {
                    CsrPmemFree(attribs->recipientFormatName);
                    attribs->recipientFormatName = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("FN:") + 1);
                    sprintf(attribs->recipientFormatName, "%s", &(line[CsrStrLen("FN:")]));
                    if (attribs->recipientFormatName[CsrStrLen(attribs->recipientFormatName) - 1] == '\n')
                    {
                        attribs->recipientFormatName[CsrStrLen(attribs->recipientFormatName) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "N:"))
                {
                    CsrPmemFree(attribs->recipientName);
                    attribs->recipientName = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("N:") + 1);
                    sprintf(attribs->recipientName, "%s", &(line[CsrStrLen("N:")]));
                    if (attribs->recipientName[CsrStrLen(attribs->recipientName) - 1] == '\n')
                    {
                        attribs->recipientName[CsrStrLen(attribs->recipientName) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "TEL:"))
                {
                    CsrPmemFree(attribs->recipientPhone);
                    attribs->recipientPhone = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("TEL:") + 1);
                    sprintf(attribs->recipientPhone, "%s", &(line[CsrStrLen("TEL:")]));
                    if (attribs->recipientPhone[CsrStrLen(attribs->recipientPhone) - 1] == '\n')
                    {
                        attribs->recipientPhone[CsrStrLen(attribs->recipientPhone) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "EMAIL:"))
                {
                    CsrPmemFree(attribs->recipientEmail);
                    attribs->recipientEmail = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("EMAIL:") + 1);
                    sprintf(attribs->recipientEmail, "%s", &(line[CsrStrLen("EMAIL:")]));
                    if (attribs->recipientEmail[CsrStrLen(attribs->recipientEmail) - 1] == '\n')
                    {
                        attribs->recipientEmail[CsrStrLen(attribs->recipientEmail) - 1] = '\0';
                    }
                }
                else if(CsrStrStr(line, "END:VCARD"))
                {
                    vCardEndFound = TRUE;
                }
                else
                {
                    /* vCard contains an unsupported attribute, which should be ignored */
                }
            }
        }
        if (!CsrStrStr(&line[0], "BEGIN:BBODY"))
        {
            /* Ignore included message headers. Look for message content */
            while (!bbodyFound)
            {
                CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp);
                if (CsrStrStr(&line[0], "BEGIN:BBODY"))
                {
                    bbodyFound = TRUE;
                }
            }
        }
        /* Some optional lines may occur here. The next mandatory line is LENGTH: */

        while (!optionalLinesEndFound)
        {
            CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp);
            if(CsrStrStr(&line[0], "PARTID:"))
            {
                CsrPmemFree(attribs->msgPart);
                attribs->msgPart = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("PARTID:") + 1);
                sprintf(attribs->msgPart, "%s", &(line[CsrStrLen("PARTID:")]));
                if (attribs->msgPart[CsrStrLen(attribs->msgPart) - 1] == '\n')
                {
                    attribs->msgPart[CsrStrLen(attribs->msgPart) - 1] = '\0';
                }
            }
            else if(CsrStrStr(&line[0], "ENCODING:"))
            {
                CsrPmemFree(attribs->msgBodyEncoding);
                attribs->msgBodyEncoding = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("ENCODING:") + 1);
                sprintf(attribs->msgBodyEncoding, "%s", &(line[CsrStrLen("ENCODING:")]));
                if (attribs->msgBodyEncoding[CsrStrLen(attribs->msgBodyEncoding) - 1] == '\n')
                {
                    attribs->msgBodyEncoding[CsrStrLen(attribs->msgBodyEncoding) - 1] = '\0';
                }
            }
            else if(CsrStrStr(&line[0], "CHARSET:"))
            {
                CsrPmemFree(attribs->msgBodyCharset);
                attribs->msgBodyCharset = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("CHARSET:") + 1);
                sprintf(attribs->msgBodyCharset, "%s", &(line[CsrStrLen("CHARSET:")]));
                if (attribs->msgBodyCharset[CsrStrLen(attribs->msgBodyCharset) - 1] == '\n')
                {
                    attribs->msgBodyCharset[CsrStrLen(attribs->msgBodyCharset) - 1] = '\0';
                }
            }
            else if(CsrStrStr(&line[0], "LANGUAGE:"))
            {
                CsrPmemFree(attribs->msgBodyLanguage);
                attribs->msgBodyLanguage = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("LANGUAGE:") + 1);
                sprintf(attribs->msgBodyLanguage, "%s", &(line[CsrStrLen("LANGUAGE:")]));
                if (attribs->msgBodyLanguage[CsrStrLen(attribs->msgBodyLanguage) - 1] == '\n')
                {
                    attribs->msgBodyLanguage[CsrStrLen(attribs->msgBodyLanguage) - 1] = '\0';
                }
            }
            else if(CsrStrStr(&line[0], "LENGTH:"))
            {
                CsrPmemFree(attribs->msgBodyLength);
                attribs->msgBodyLength = CsrPmemAlloc(CsrStrLen(line) - CsrStrLen("LENGTH:") + 1);
                sprintf(attribs->msgBodyLength, "%s", &(line[CsrStrLen("LENGTH:")]));
                if (attribs->msgBodyLength[CsrStrLen(attribs->msgBodyLength) - 1] == '\n')
                {
                    attribs->msgBodyLength[CsrStrLen(attribs->msgBodyLength) - 1] = '\0';
                }
                optionalLinesEndFound = TRUE;
            }
            else
            {
                optionalLinesEndFound = TRUE;
            }
        }
        /* This line must be BEGIN:MSG */
        if (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) == NULL ||
            CsrStrStr(&line[0], "BEGIN:MSG") == NULL)
        {
            return FALSE;
        }
        /* Capture message body text */
        while (CsrBtFgets(&line[0], MAX_LINE_LENGTH, fp) &&
               CsrStrStr(&line[0], "END:MSG") == NULL)
        {
            char    *tempP = msgBody;
            msgBody = CsrPmemAlloc(CsrStrLen(msgBody) + CsrStrLen(line) + 1);
            msgBody[0] = '\0';
            CsrStrCat(msgBody, tempP);
            CsrStrCat(msgBody, line);
            bodyBytesCaptured += CsrStrLen(&line[0]);
            CsrPmemFree(tempP);
        }
        CsrPmemFree(attribs->msgBody);
        attribs->msgBody = CsrPmemAlloc(CsrStrLen(msgBody) + 1);
        sprintf(attribs->msgBody, "%s", &msgBody[0]);

        /* Look for a subject in the message body */
        interpretPtr = CsrStrStr(attribs->msgBody, "Subject:");
        if(interpretPtr)
        {
            CsrUint16   i = 0;

            CsrPmemFree(attribs->subject);
            attribs->subject = CsrPmemAlloc(MAX_LINE_LENGTH);

            while (interpretPtr[i + CsrStrLen("Subject:")] != '\n' &&
                   interpretPtr[i + CsrStrLen("Subject:")] != '\0' &&
                   i < MAX_LINE_LENGTH)
            {
                CsrMemCpy(&attribs->subject[i], &interpretPtr[i + CsrStrLen("Subject:")], 1);
                i++;
            }
            attribs->subject[i] = '\0';
        }
    }
    /* NOTE: We are not checking if all the required END: statements are present in the message file */
    return TRUE;
}

static char *messageListingAddAttribute(char         *messageP,
                                        CsrUint32    *messageLength,
                                        char         *fixedText,
                                        char         *attributeText)
{
    char    *tempMessageP = NULL;

    *messageLength = *messageLength + CsrStrLen(fixedText) +
                                      CsrStrLen((char *)XML_VALUE_DELIMITER) +
                                      CsrStrLen(attributeText) +
                                      CsrStrLen((char *)XML_VALUE_DELIMITER);
    tempMessageP = CsrPmemAlloc(*messageLength + 1);
    tempMessageP[0] = '\0';

    CsrStrCat(tempMessageP, messageP);
    CsrStrCat(tempMessageP, fixedText);
    CsrStrCat(tempMessageP, (char *)XML_VALUE_DELIMITER);
    CsrStrCat(tempMessageP, attributeText);
    CsrStrCat(tempMessageP, (char *)XML_VALUE_DELIMITER);
    CsrPmemFree(messageP);

    return tempMessageP;
}

static CsrBool buildMessageListing(MsgListStr   *listStr,
                                 char         **messageP,
                                 CsrUint32    *messageLength,
                                 CsrFileHandle         *fileP,
                                 char         *fHandle,
                                 char         *fTime,
                                 char         *fSize,
                                 CsrBool      finalMsg)
{
    CsrUint16       count = 0;
    FileAttribType  fileAttribs;
    char            *tempMessageP = NULL;
    char            *tempP = NULL;
    MsgListStr      *currentStr = listStr;
    MsgListStr      *tempPtr = NULL;

    if (!readMessageFile(fileP, &fileAttribs))
    {
        return FALSE;
    }

    if (*messageLength == 0) /* Header is needed */
    {
        tempP = tempMessageP;
        *messageLength += CsrStrLen(XML_MSG_LISTING_HEADER);
        tempMessageP = CsrPmemAlloc(*messageLength + 1);
        tempMessageP[0] = '\0';
        if (tempP)
        {
            CsrStrCat(tempMessageP, tempP);
        }
        CsrStrCat(tempMessageP, XML_MSG_LISTING_HEADER);
        CsrPmemFree(tempP);
    }

    tempP = tempMessageP;
    *messageLength += CsrStrLen(MAP_MESSAGE_HANDLE_ATTR);
    tempMessageP = CsrPmemAlloc(*messageLength + 1);
    tempMessageP[0] = '\0';
    if (tempP)
    {
        CsrStrCat(tempMessageP, tempP);
    }
    CsrStrCat(tempMessageP, MAP_MESSAGE_HANDLE_ATTR);
    CsrPmemFree(tempP);

    /* Adding file handle */
    fHandle[CsrStrLen(fHandle) - 4] = '\0';
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_HANDLE_BEGIN_ATTR, fHandle);

    /* Adding subject */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SUBJECT_BEGIN_ATTR,
                                                        (char *)(fileAttribs.subject ? fileAttribs.subject : " "));

    /* Adding date and time */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_DATETIME_BEGIN_ATTR, fTime);

    /* Adding sender name */
    if (fileAttribs.senderName)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SENDER_NAME_BEGIN_ATTR, fileAttribs.senderName);
    }

    /* Adding sender addressing */
    if (fileAttribs.senderEmail)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SENDER_ADDR_BEGIN_ATTR, fileAttribs.senderEmail);
    }
    else if (fileAttribs.senderPhone)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SENDER_ADDR_BEGIN_ATTR, fileAttribs.senderPhone);
    }

    /* Adding replyTo addressing */
    if (fileAttribs.replyToAddr)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_REPLYTO_ADDR_BEGIN_ATTR, fileAttribs.replyToAddr);
    }

    /* Adding recipient name */
    if (fileAttribs.recipientName)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_RECIPIENT_NAME_BEGIN_ATTR, fileAttribs.recipientName);
    }

    /* Adding recipinet addressing */
    if (fileAttribs.recipientEmail)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_RECIPIENT_ADDR_BEGIN_ATTR, fileAttribs.recipientEmail);
    }
    else if (fileAttribs.recipientPhone)
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_RECIPIENT_ADDR_BEGIN_ATTR, fileAttribs.recipientPhone);
    }
    else
    {
        tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_RECIPIENT_ADDR_BEGIN_ATTR, "");
    }

    /* Adding message type */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_TYPE_BEGIN_ATTR, fileAttribs.msgType);

    /* Adding message size */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SIZE_BEGIN_ATTR, fSize);

    /* Adding text boolean */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_TEXT_BEGIN_ATTR, (fileAttribs.msgText ? "yes" : "no"));

    /* Adding reception status */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_RECEPTION_STATUS_BEGIN_ATTR,
                             fileAttribs.receptionStatus ? fileAttribs.receptionStatus : "complete");

    /* Adding attachment size */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_ATTACHMENT_SIZE_BEGIN_ATTR,
                                    fileAttribs.attachmentSize ? fileAttribs.attachmentSize : "0");

    /* Adding priority boolean */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_PRIORITY_BEGIN_ATTR, (fileAttribs.priority ? "yes" : "no"));

    /* Adding read boolean */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_READ_BEGIN_ATTR, (fileAttribs.read ? "yes" : "no"));

    /* Adding sent boolean */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_SENT_BEGIN_ATTR, (fileAttribs.sent ? "yes" : "no"));

    /* Adding protected boolean */
    tempMessageP = messageListingAddAttribute(tempMessageP, messageLength, MAP_MESSAGE_PROTECTED_BEGIN_ATTR, (fileAttribs.protect ? "yes" : "no"));

    tempP = tempMessageP;
    *messageLength += CsrStrLen(MAP_MESSAGE_LIST_END_ATTR);
    tempMessageP = CsrPmemAlloc(*messageLength + 1);
    tempMessageP[0] = '\0';
    if (tempP)
    {
        CsrStrCat(tempMessageP, tempP);
    }
    CsrStrCat(tempMessageP, MAP_MESSAGE_LIST_END_ATTR);
    CsrPmemFree(tempP);

    if (finalMsg) /* Footer is needed */
    {
        CsrUint32 totalLength = 0;

        tempP = tempMessageP;
        *messageLength += CsrStrLen(XML_MSG_LISTING_FOOTER);
        tempMessageP = CsrPmemAlloc(*messageLength + 1);
        tempMessageP[0] = '\0';
        if (*messageP)
        {
            CsrStrCat(tempMessageP, *messageP);
        }
        if (tempP)
        {
            CsrStrCat(tempMessageP, tempP);
        }
        CsrStrCat(tempMessageP, XML_MSG_LISTING_FOOTER);
        CsrPmemFree(tempP);

        /* Concatenate all message listing strings into one */
        /*while (currentStr->next != NULL)
        {
            totalLength += currentStr->length;
            currentStr = currentStr->next;
        }
        totalLength += (*messageLength + 1);
        CsrPmemFree(*messageP);
        *messageP = CsrPmemAlloc(totalLength);
        currentStr = listStr;
        while (currentStr->next != NULL)
        {
            CsrStrCat(*messageP, currentStr->str);
            currentStr = currentStr->next;
        }
        CsrStrCat(*messageP, tempMessageP);
        freeMsgListingStr(&listStr);
        */
    }
    else if (*messageP)
    {
        tempP = tempMessageP;
        tempMessageP = CsrPmemAlloc(*messageLength + 1);
        tempMessageP[0] = '\0';
        if (tempP)
        {
            CsrStrCat(tempMessageP, *messageP);
            CsrStrCat(tempMessageP, tempP);
        }
        CsrPmemFree(tempP);

    }
    CsrPmemFree(*messageP);

    *messageP = tempMessageP;
    return TRUE;
}

static char *getNextFileName(char *path)
{
    char        *fileName, *newFileName;
    char        zeroString[MAPS_FILENAME_LENGTH];
    FolderInfo  *folderInformation = NULL, *currentFolderInfo, *mostRecentFolderInfo;
    CsrUint16   i = 0;

    fileName = CsrPmemAlloc(MAPS_FILENAME_LENGTH + 1);
    newFileName = CsrPmemAlloc(MAPS_FILENAME_LENGTH + 1);
    fileName[0] = '\0';
    newFileName[0] = '\0';
    for (i = 0; i < MAPS_FILENAME_LENGTH - CsrStrLen(".msg"); i++)
    {
        zeroString[i] = '0';
    }
    zeroString[i] = '\0';
    getFolderInformation(path, &folderInformation, TRUE);

    if (folderInformation)
    {
        mostRecentFolderInfo = folderInformation;
        currentFolderInfo = folderInformation;

        if (currentFolderInfo->name[0] != '\0')
        {
            CsrUint8    i = 0;
            CsrUint8    j = 1;
            CsrUint8    k = 0;
            CsrUint32   n = 0;

            fileName = currentFolderInfo->name;
            while (currentFolderInfo)
            {
                if(atoi(currentFolderInfo->name + 1) > atoi(mostRecentFolderInfo->name + 1)) /* + 1 to avoid overflow */
                {
                    fileName = currentFolderInfo->name;
                }
                currentFolderInfo = currentFolderInfo->nextInfoElement;
            }

            n = atoi(fileName + 1) + 1;
            newFileName[0] = fileName[0];

            for (i = MAPS_FILENAME_LENGTH - CsrStrLen(".msg") - 1; j < MAPS_FILENAME_LENGTH - CsrStrLen(".msg"); i--)
            {
                CsrUint32 multiplier = 1;

                for (k = 0; k < i - 1; k++)
                {
                    multiplier = (CsrUint32)(multiplier * 10);
                }
                newFileName[j] = (char)((n / multiplier) + '0');
                n -= (CsrUint32)((newFileName[j] - '0') * multiplier);
                j++;
            }
            newFileName[j] = '\0';

            if (sprintf(fileName, "%s%s", newFileName, ".msg"))
            {
                return fileName;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

static CsrBtObexResponseCode mapsMoveFile(MapsAppInst *inst, char *newFileName, char *oldFileName)
{
    CsrFileHandle                  *newFile, *oldFile;
    CsrBtObexResponseCode responseCode;
    CsrBool               loop = TRUE;
    char*                 tmpStr = CsrPmemAlloc(MAX_LINE_LENGTH + 1);

    newFile = CsrBtFopen(newFileName, "w+");
    oldFile = CsrBtFopen(oldFileName, "r");

    if (newFile && oldFile)
    {
        while(loop)
        {
            tmpStr = CsrBtFgets(tmpStr, MAX_LINE_LENGTH, oldFile);
            if (tmpStr)
            {
                CsrBtFwrite(CsrStrCat(tmpStr, "\n"), 1, CsrStrLen(CsrStrCat(tmpStr, "\n")), newFile);
            }
            else
            {
                loop = FALSE;
            }
        }
        CsrBtFclose(newFile);
        CsrBtFclose(oldFile);
        unlink((char *)oldFileName);
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else
    {
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
    }

    CsrPmemFree(tmpStr);

    return responseCode;
}

static char *csrBtMapsGetParentDir(char *currentDir)
{
    char        *parentDir = NULL;
    CsrUint16   i = CsrStrLen(currentDir) - 2; /* -2 to avoid the last backslash and the NULL terminator of the string */
    CsrUint16   parentDirLength = 0;

    for (i = CsrStrLen(currentDir) - 2; i > 1; i--)
    {
        if (currentDir[i] == 0x5C) /* 5C is the ASCII value for backslash */
        {
            parentDirLength = i;
            i = 1; /* This will stop the FOR-loop */
        }
    }

    parentDir = CsrPmemAlloc(parentDirLength + 1);
    CsrMemCpy(parentDir, currentDir, parentDirLength);
    parentDir[parentDirLength] = '\0';

    return parentDir;
}

static char *csrBtMapsGetFileName(char *currentFile)
{
    char        *fileName = NULL;
    CsrUint16   i = CsrStrLen(currentFile);
    CsrUint16   fileNameLength = 0;

    for (i = CsrStrLen(currentFile); i > 1; i--)
    {
        if (currentFile[i] == 0x5C) /* 5C is the ASCII value for backslash */
        {
            fileNameLength = i + 1;
            i = 1; /* This will stop the FOR-loop */
        }
    }

    fileName = CsrPmemAlloc(MAPS_FILENAME_LENGTH + 1);
    CsrMemCpy(fileName, currentFile + fileNameLength, MAPS_FILENAME_LENGTH);
    fileName[MAPS_FILENAME_LENGTH] = '\0';

    return fileName;
}

/* Handle the MAPS messages */
void handleMapsEvents(MapsAppInst *inst)
{
    CsrBtMapsPrim type;

    type = *((CsrBtMapsPrim*)inst->msg);
    switch(type)
    {
        case CSR_BT_MAPS_GET_INSTANCE_IDS_CFM:
            {
                CsrBtMapsGetInstanceIdsCfm * prim = inst->msg;
                CsrUint8 i;

                inst->mapsInstanceListReceived = TRUE;
                inst->numberOfInstances = prim->instanceIdsListSize;
                inst->activeInst = 0;

                inst->mapsInst = CsrPmemAlloc(sizeof(MapsInstance) * prim->instanceIdsListSize);

                for( i=0; i< prim->instanceIdsListSize;i++)
                {
                    CsrMemSet(&inst->mapsInst[i], 0, sizeof(MapsInstance));
                    cleanInstance(&inst->mapsInst[i], FALSE);
                    inst->mapsInst[i].mapsInstanceId = prim->instanceIdsList[i];
                }
                playMenu(inst);
                break;
            }
        case CSR_BT_MAPS_ACTIVATE_CFM:
            {
                CsrBtMapsActivateCfm *prim = inst->msg;

                if (prim->resultCode     == CSR_BT_RESULT_CODE_OBEX_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_OBEX_PROFILES)
                {
                    printf("\nActivation successful\n");
                    stateChange(INST_DATA, STATE_ACTIVATED);
                    playMenu(inst);
                }
                else
                {
                    printf("\nActivation error: 0x%04x (supplier 0x%04x)\n", prim->resultCode, prim->resultSupplier);
                }
                break;
            }
        case CSR_BT_MAPS_DEACTIVATE_CFM:
            {
                CsrBtMapsDeactivateCfm *prim = inst->msg;

                printf("\nDeactivation successful\n");
                stateChange(INST_DATA, STATE_IDLE);
                playMenu(inst);
                break;
            }
        case CSR_BT_MAPS_CONNECT_IND:
            {
                CsrBtMapsConnectInd *prim = inst->msg;

                inst->mapsInst[inst->activeInst].address.nap = prim->deviceAddr.nap;
                inst->mapsInst[inst->activeInst].address.uap = prim->deviceAddr.uap;
                inst->mapsInst[inst->activeInst].address.lap = prim->deviceAddr.lap;

                stateChange(INST_DATA, STATE_CONNECTING);
                playMenu(inst);
                printf("Connect indication to instance 0x%04X from %04X:%02X:%06X received.  Count is: %d.   Length is: %d.\n",
                                                                                                            prim->instanceId,
                                                                                                            prim->deviceAddr.nap,
                                                                                                            prim->deviceAddr.uap,
                                                                                                            prim->deviceAddr.lap,
                                                                                                            prim->count,
                                                                                                            prim->length);
                break;
            }
        case CSR_BT_MAPS_DISCONNECT_IND:
            {
                CsrBtMapsDisconnectInd *prim = inst->msg;

                stateChange(INST_DATA, STATE_ACTIVATED);
                playMenu(inst);
                printf("\nInstance 0x%04X disconnected from %04X:%02X:%06X\n", prim->instanceId,
                                                                             inst->mapsInst[inst->activeInst].address.nap,
                                                                             inst->mapsInst[inst->activeInst].address.uap,
                                                                             inst->mapsInst[inst->activeInst].address.lap);
                CsrBtChdir(inst->mapsInst[inst->activeInst].dir_local_root);
                cleanInstance(INST_DATA, TRUE);
                break;
            }
        case CSR_BT_MAPS_SET_BACK_FOLDER_IND:
            {
                CsrBtMapsSetBackFolderInd *prim = inst->msg;
                char *parentDir;

                printf("CSR_BT_MAPS_BACK_FOLDER_IND received\n");

                if (CsrStrLen(inst->mapsInst[inst->activeInst].dir_local) <= CsrStrLen(inst->mapsInst[inst->activeInst].dir_local_root))
                {   /* We are in the root directory, so don't go back one folder */
                    printf("Current folder is the root folder, so we can not go back\n");
                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE);
                }
                else
                {
                    parentDir = csrBtMapsGetParentDir(inst->mapsInst[inst->activeInst].dir_local);

                    if(CsrBtChdir(parentDir))
                    {
                        printf("Unable to locate the parent directory: %s\n", parentDir);
                        CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    }
                    else
                    {
                        CsrPmemFree(inst->mapsInst[inst->activeInst].dir_local);
                        inst->mapsInst[inst->activeInst].dir_local =
                                    CsrPmemAlloc(CsrStrLen(PATH_SEP) + CsrStrLen(parentDir) + 1);
                        sprintf(inst->mapsInst[inst->activeInst].dir_local, "%s"PATH_SEP, parentDir);

                        if (prim->folderName == NULL)
                        {
                            CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                        }
                        else
                        {
                            CsrUint8 *name = NULL;
                            char     path[CSR_BT_MAX_PATH_LENGTH];

                            /* convert big endian unicode to little unicode */
                            name = unicode2String(prim->folderName);

                            sprintf(path, "%s%s", inst->mapsInst[inst->activeInst].dir_local, name);
                            /* Neeed to check if the path exits, if not send CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE */
                            if(CsrBtChdir(path))
                            {
                                CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                                printf("Unable to locate the directory: %s\n", path);
                            }
                            else
                            {
                                /* Check if the path is read only, send CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE */
                                if((CsrBtAccess(path, 2)) == 0)
                                {
                                    char    *tempP = inst->mapsInst[inst->activeInst].dir_local;

                                    inst->mapsInst[inst->activeInst].dir_local =
                                                CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(name) + CsrStrLen(PATH_SEP) + 1);
                                    sprintf(inst->mapsInst[inst->activeInst].dir_local, "%s%s"PATH_SEP, tempP, name);
                                    CsrPmemFree(tempP);
                                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                                }
                                else
                                {
                                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                                    printf("The specified directory is forbidden, read only\n");
                                }
                            }
                            CsrPmemFree(name);
                        }
                    }
                    CsrPmemFree(parentDir);
                }
                playMenu(inst);
                printf("CSR_BT_MAPS_BACK_FOLDER_RES sent\n");
                break;
            }
        case CSR_BT_MAPS_SET_ROOT_FOLDER_IND:
            {
                CsrBtMapsSetRootFolderInd *prim = inst->msg;

                playMenu(inst);
                printf("CSR_BT_MAPS_SET_ROOT_FOLDER_IND received\n");

                if(CsrBtChdir(inst->mapsInst[inst->activeInst].dir_local_root))
                {
                    printf("Unable to locate the root directory: %s\n", inst->mapsInst[inst->activeInst].dir_local_root);
                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                }
                else
                {
                    CsrUint8    *tempP = inst->mapsInst[inst->activeInst].dir_local;

                    inst->mapsInst[inst->activeInst].dir_local = CsrPmemAlloc(CsrStrLen(inst->mapsInst[inst->activeInst].dir_local_root) + 1);
                    CsrMemCpy(inst->mapsInst[inst->activeInst].dir_local, inst->mapsInst[inst->activeInst].dir_local_root, CsrStrLen(inst->mapsInst[inst->activeInst].dir_local_root) + 1);
                    CsrPmemFree(tempP);
                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                }
                break;
            }
        case CSR_BT_MAPS_SET_FOLDER_IND:
            {
                CsrBtMapsSetFolderInd *prim = inst->msg;
                char path[CSR_BT_MAX_PATH_LENGTH];
                CsrUint8 *name;

                printf("CSR_BT_MAPS_SET_FOLDER_IND received\n");

                /* convert big endian unicode to little unicode */
                name = unicode2String(prim->folderName);
                sprintf(path, "%s%s", inst->mapsInst[inst->activeInst].dir_local, name);

                /* Neeed to check if the path exits, if not send CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE */
                if(CsrBtChdir(path))
                {
                    CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                    printf("Unable to locate the directory: %s\n", path);
                }
                else
                {
                    /* Check if the path is read only, send CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE */
                    if((CsrBtAccess(path, 2)) == 0)
                    {
                        char    *tempP = inst->mapsInst[inst->activeInst].dir_local;

                        inst->mapsInst[inst->activeInst].dir_local =
                                    CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(name) + CsrStrLen(PATH_SEP) + 1);
                        sprintf(inst->mapsInst[inst->activeInst].dir_local, "%s%s"PATH_SEP, tempP, name);
                        CsrPmemFree(tempP);
                        CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
                    }
                    else
                    {
                        CsrBtMapsSetFolderResSend(prim->instanceId, CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
                        printf("The specified directory is forbidden, read only\n");
                    }
                }
                playMenu(inst);
                printf("CSR_BT_MAPS_SET_FOLDER_RES sent\n");
                CsrPmemFree(name);
                break;
            }

        case CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND:
            {
                CsrBtMapsGetFolderListingHeaderInd *prim = inst->msg;
                char path[CSR_BT_MAX_PATH_LENGTH];

                playMenu(inst);
                printf("CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND received\n");

                inst->mapsInst->folder = NULL;
                sprintf(path, "%s", inst->mapsInst->dir_local);
                getFolderListing(path, &inst->mapsInst->folder);
                if(inst->mapsInst->folder != NULL)
                {
                    inst->mapsInst->folder_size = CsrStrLen(inst->mapsInst->folder) + 1;
                    inst->mapsInst->folder_idx = 0;
                }
                else
                {
                    printf("Error path does not exists\n");
                    inst->mapsInst->folder_size = 0;
                }

                CsrBtMapsGetFolderListingHeaderResSend(prim->instanceId, (CsrUint16)inst->mapsInst->folder_size, FALSE);
                printf("CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES sent. Listing size is: %d\n", inst->mapsInst->folder_size);
                break;
            }

        case CSR_BT_MAPS_GET_FOLDER_LISTING_IND:
            {
                CsrBtMapsGetFolderListingInd    *prim = inst->msg;
                CsrUint8                        *body;
                CsrUint16                       bytesToRead;
                CsrBtObexResponseCode           responseCode;

                printf("CSR_BT_MAPS_GET_FOLDER_LISTING_IND received\n");
                if(inst->mapsInst->folder != NULL)
                {
                    if(inst->mapsInst->folder_size - inst->mapsInst->folder_idx <= prim->obexResponsePacketLength)
                    {
                        bytesToRead = (CsrUint16)(inst->mapsInst->folder_size - inst->mapsInst->folder_idx);
                    }
                    else
                    {
                        bytesToRead = prim->obexResponsePacketLength;
                    }

                    body = CsrPmemAlloc(bytesToRead);
                    CsrMemCpy(body, (void*)(inst->mapsInst->folder + inst->mapsInst->folder_idx), bytesToRead);

                    inst->mapsInst->folder_idx += bytesToRead;

                    if(inst->mapsInst->folder_idx >= inst->mapsInst->folder_size)
                    {
                        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        CsrPmemFree(inst->mapsInst->folder);
                        inst->mapsInst->folder = NULL;
                    }
                    else
                    {
                        responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                    }
                    CsrBtMapsGetFolderListingResSend(prim->instanceId, responseCode, bytesToRead, body, FALSE);
                }
                else
                {
                    CsrBtMapsGetFolderListingResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 0, NULL, FALSE);
                }
                printf("CSR_BT_MAPS_GET_FOLDER_LISTING_RES sent\n");
                break;
            }

        case CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND:
            {
                struct tm   fileTime;
                CsrBtMapsGetMessageListingHeaderInd *prim = inst->msg;
                char path[CSR_BT_MAX_PATH_LENGTH];
                CsrUint8    *name = NULL;
                FolderInfo  *folderInformation = NULL;
                FolderInfo  *mostRecentFolderInformation = NULL;
                FolderInfo  *currentFolderInformation = NULL;
                CsrUint32   noOfFiles = 0;
                CsrUint32   count = 0;
                char        *currentTimeStr = CsrPmemAlloc(MAPS_TIME_FORMAT_LENGTH + 1);

                /* Init variables */
                for (count = 0; count < (MAPS_TIME_FORMAT_LENGTH + 1); count++)
                {
                    currentTimeStr[count] = '\0';
                }

                playMenu(inst);
                printf("CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND received\n\n");
                printf("   *** NOTE that filters are not supported in this demo app yet ***\n\n");
                printf("Filter values in the request:\n");
                printf("     Max list count: %d\n", prim->maxListCount);
                printf("     List start offset: %d\n", prim->listStartOffset);
                printf("     Max subject length: %d\n", prim->maxSubjectLength);
                printf("     Parameter mask: 0x%08X\n", prim->maxListCount);
                printf("     Filter message type: 0x%02X\n", prim->filterMessageType);
                printf("     Filter period begin: %s\n", prim->filterPeriodBegin);
                printf("     Filter period end: %s\n", prim->filterPeriodEnd);
                printf("     Filter read status: 0x%02X\n", prim->filterReadStatus);
                printf("     Filter recipient: %s\n", prim->filterRecipient);
                printf("     Filter originator: %s\n", prim->filterOriginator);
                printf("     Filter priority: 0x%02X\n\n", prim->filterPriority);
                printf("     Folder name: %s\n\n", prim->folderName == NULL ? "<root>" : (char *)unicode2String(prim->folderName));

                inst->mapsInst->message = NULL;
                inst->mapsInst->message_size = 0;
                inst->mapsInst->message_idx = 0;

                if (prim->folderName)
                {
                    name = unicode2String(prim->folderName);
                }

                if(name == NULL)
                {
                    sprintf(path, "%s", inst->mapsInst->dir_local);
                }
                else
                {
                    sprintf(path, "%s%s", inst->mapsInst->dir_local, name);
                }

                noOfFiles = getFolderInformation(path, &folderInformation, TRUE);

                if(noOfFiles && folderInformation)
                {
                    CsrFileHandle        *currentFile = NULL;
                    CsrBool     formatOk = FALSE;
                    CsrBool     messageAdded;
                    char        tempTime[MAPS_TIME_FORMAT_LENGTH], tempSize[MAPS_SIZE_FORMAT_LENGTH];

                    /* Init of temp variables */
                    for (count = 0; count < MAPS_TIME_FORMAT_LENGTH; count++)
                    {
                        tempTime[count] = '\0';
                    }
                    for (count = 0; count < MAPS_SIZE_FORMAT_LENGTH; count++)
                    {
                        tempSize[count] = '\0';
                    }

                    for (count = 0; count < noOfFiles; count++)
                    {
                        /* Find the most recent message */
                        mostRecentFolderInformation = folderInformation;
                        currentFolderInformation = folderInformation;
                        while (currentFolderInformation)
                        {
                            if(CsrTimeGt(currentFolderInformation->time, mostRecentFolderInformation->time))
                            {
                                mostRecentFolderInformation = currentFolderInformation;
                            }
                            currentFolderInformation = currentFolderInformation->nextInfoElement;
                        }
                        currentFile = CsrBtFopen(mostRecentFolderInformation->name, "r");

                        if (currentFile)
                        {
                            fileTime.tm_year = 0;
                            fileTime.tm_mon  = 0;
                            fileTime.tm_mday = 0;
                            fileTime.tm_hour = 0;
                            fileTime.tm_min  = 0;
                            fileTime.tm_sec  = 0;

                            fileTime = *gmtime((time_t*)&mostRecentFolderInformation->time);

                            /* Time format must be YYYYMMDDTHHMMSS */
                            sprintf(&tempTime[0], "%04d%02d%02dT%02d%02d%02d",
                                                            fileTime.tm_year + 1900,
                                                            fileTime.tm_mon + 1,
                                                            fileTime.tm_mday,
                                                            fileTime.tm_hour,
                                                            fileTime.tm_min,
                                                            fileTime.tm_sec);
                            /* Convert file size to string */
                            sprintf(&tempSize[0], "%d", mostRecentFolderInformation->size);

                            messageAdded = buildMessageListing(&inst->mapsInst->msgListing,
                                                                  &inst->mapsInst->message,
                                                                  &inst->mapsInst->message_size,
                                                                  currentFile,
                                                                  mostRecentFolderInformation->name,
                                                                  tempTime,
                                                                  tempSize,
                                                                  (CsrBool)((count == (noOfFiles - 1)) ? TRUE : FALSE));
                            if (messageAdded == FALSE)
                            {
                                printf("File %s is not correct bMessage format\n", mostRecentFolderInformation->name);
                            }
                            CsrBtFclose(currentFile);
                            currentFile = NULL;
                        }
                        else
                        {
                            printf("Not able to open file %s for reading\n", mostRecentFolderInformation->name);
                        }
                        /* Set time to zero to make sure this file is not listed again */
                        mostRecentFolderInformation->time = 0;

                    }
                }
                else
                {
                    printf("Error path does not exist\n");
                    inst->mapsInst->message = NULL;
                    inst->mapsInst->message_size = 0;
                }
                fileTime.tm_year = 0;
                fileTime.tm_mon  = 0;
                fileTime.tm_mday = 0;
                fileTime.tm_hour = 0;
                fileTime.tm_min  = 0;
                fileTime.tm_sec  = 0;

                /*fileTime = *gmtime((time_t*)&timeNow);*/

                /* Time format must be YYYYMMDDTHHMMSS */
                sprintf(currentTimeStr, "%04d%02d%02dT%02d%02d%02d",
                                        fileTime.tm_year + 1900,
                                        fileTime.tm_mon + 1,
                                        fileTime.tm_mday,
                                        fileTime.tm_hour,
                                        fileTime.tm_min,
                                        fileTime.tm_sec);

                CsrBtMapsGetMessageListingHeaderResSend(prim->instanceId,
                                                        CSR_BT_MAP_NEW_MESSAGE_OFF,
                                                        (CsrCharString *)currentTimeStr,
                                                        (CsrUint16)inst->mapsInst->message_size,
                                                        FALSE);
                printf("CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES sent. Listing size is: %d\n", inst->mapsInst->message_size);
                break;
            }

        case CSR_BT_MAPS_GET_MESSAGE_LISTING_IND:
            {
                CsrBtMapsGetMessageListingInd   *prim = inst->msg;
                CsrUint8                        *body;
                CsrUint16                       bytesToRead;
                CsrBtObexResponseCode           responseCode;

                printf("CSR_BT_MAPS_GET_MESSAGE_LISTING_IND received\n");
                if(inst->mapsInst->message != NULL)
                {
                    if(inst->mapsInst->message_size - inst->mapsInst->message_idx <= prim->obexResponsePacketLength)
                    {
                        bytesToRead = (CsrUint16)(inst->mapsInst->message_size - inst->mapsInst->message_idx);
                    }
                    else
                    {
                        bytesToRead = prim->obexResponsePacketLength;
                    }

                    body = CsrPmemAlloc(bytesToRead);
                    CsrMemCpy(body, (void*)(inst->mapsInst->message + inst->mapsInst->message_idx), bytesToRead);

                    inst->mapsInst->message_idx += bytesToRead;

                    if(inst->mapsInst->message_idx >= inst->mapsInst->message_size)
                    {
                        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        CsrPmemFree(inst->mapsInst->message);
                        inst->mapsInst->folder = NULL;
                    }
                    else
                    {
                        responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                    }
                    CsrBtMapsGetMessageListingResSend(prim->instanceId, responseCode, bytesToRead, body, FALSE);
                    printf("CSR_BT_MAPS_GET_MESSAGE_LISTING_RES with response code 0x%02X sent\n", responseCode);
                }
                else
                {
                    CsrBtMapsGetMessageListingResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 0, NULL, FALSE);
                    printf("CSR_BT_MAPS_GET_MESSAGE_LISTING_RES with response code 0x%02X sent\n", CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
                }
                break;
            }

        case CSR_BT_MAPS_GET_MESSAGE_HEADER_IND:
            {
                CsrBtMapsGetMessageHeaderInd *prim = inst->msg;
                char *msgHandle;
                FolderInfo  *folderInformation = NULL;

                msgHandle = CsrPmemAlloc(CsrStrLen(unicode2String(prim->messageHandle)) + 1);
                msgHandle[0] = '\0';
                CsrStrCat(msgHandle, unicode2String(prim->messageHandle));

                playMenu(inst);
                printf("CSR_BT_MAPS_GET_MESSAGE_HEADER_IND received\n\n");
                printf("Parameters in the request:\n");
                printf("     Message Handle: %s\n", msgHandle);
                printf("     Attachment: %s\n", prim->attachment == CSR_BT_MAP_ATTACHMENT_OFF ? "OFF" : "ON");
                printf("     Charset: %s\n", prim->charset == CSR_BT_MAP_CHARSET_NATIVE ? "<native>" : "<UTF-8>");

                if (prim->fractionRequest != CSR_BT_MAP_FRACTION_REQ_NOT_USED)
                {
                    printf("     Fraction Request: %s\n", prim->fractionRequest == CSR_BT_MAP_FRACTION_REQ_FIRST ? "<first>" : "<next>");
                }
                else
                {
                    printf("     Fraction Request: Not used\n");
                }

                if (msgHandle)
                {
                    switch (msgHandle[0])
                    {
                        case '1':
                            {
                                inst->mapsInst->getFileName =
                                            CsrPmemAlloc(CsrStrLen("inbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "inbox", PATH_SEP, msgHandle, ".msg");
                                break;
                            }
                        case '2':
                            {
                                inst->mapsInst->getFileName =
                                            CsrPmemAlloc(CsrStrLen("outbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "outbox", PATH_SEP, msgHandle, ".msg");
                                break;
                            }
                        case '3':
                            {
                                inst->mapsInst->getFileName =
                                            CsrPmemAlloc(CsrStrLen("sent") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "sent", PATH_SEP, msgHandle, ".msg");
                                break;
                            }
                        default:
                            {
                                inst->mapsInst->getFileName = NULL;
                                break;
                            }
                    }
                }
                else
                {
                    inst->mapsInst->getFileName = NULL;
                }

                if (inst->mapsInst->getFileName)
                {
                    char    *currentFile = NULL;

                    currentFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                             CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                             CsrStrLen(inst->mapsInst->getFileName) + 1);
                    currentFile[0] = '\0';

                    currentFile = CsrStrCat(currentFile ,inst->mapsInst->dir_local_root);
                    currentFile = CsrStrCat(currentFile ,"telecom");
                    currentFile = CsrStrCat(currentFile ,PATH_SEP);
                    currentFile = CsrStrCat(currentFile ,"msg");
                    currentFile = CsrStrCat(currentFile ,PATH_SEP);
                    currentFile = CsrStrCat(currentFile ,inst->mapsInst->getFileName);

                    inst->mapsInst->file = CsrBtFopen(currentFile, "r");

                    if (inst->mapsInst->file == NULL)
                    { /* The file open may fail because the file is placed in the "Deleted"-folder. Let's try that */
                        CsrPmemFree(inst->mapsInst->getFileName);
                        CsrPmemFree(currentFile);
                        inst->mapsInst->getFileName =
                                            CsrPmemAlloc(CsrStrLen("deleted") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                        sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "deleted", PATH_SEP, msgHandle, ".msg");
                        currentFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                             CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                                 CsrStrLen(inst->mapsInst->getFileName) + 1);
                        currentFile[0] = '\0';
                        currentFile = CsrStrCat(currentFile ,inst->mapsInst->dir_local_root);
                        currentFile = CsrStrCat(currentFile ,"telecom");
                        currentFile = CsrStrCat(currentFile ,PATH_SEP);
                        currentFile = CsrStrCat(currentFile ,"msg");
                        currentFile = CsrStrCat(currentFile ,PATH_SEP);
                        currentFile = CsrStrCat(currentFile ,inst->mapsInst->getFileName);

                        inst->mapsInst->file = CsrBtFopen(currentFile, "r");
                    }

                    if (inst->mapsInst->file)
                    {
                        CsrBtFseek(inst->mapsInst->file, 0, CSR_SEEK_END);
                        inst->mapsInst->fileSizeRemain = (CsrUint16)CsrBtFtell(inst->mapsInst->file);
                        CsrBtFseek(inst->mapsInst->file, 0, CSR_SEEK_SET);
                    }
                    else
                    {
                        inst->mapsInst->file = NULL;
                    }
                }
                else
                {
                    inst->mapsInst->file = NULL;
                }
                CsrBtMapsGetMessageHeaderResSend(prim->instanceId, CSR_BT_MAP_FRACTION_DEL_LAST, FALSE);
                CsrPmemFree(msgHandle);
                break;
            }
        case CSR_BT_MAPS_GET_MESSAGE_IND:
            {
                CsrBtMapsGetMessageInd   *prim = inst->msg;
                CsrUint8                 *body;
                CsrUint16                bytesToRead;
                CsrUint16                lfCnt = 0; /* Number of LF characters found */
                CsrBtObexResponseCode    responseCode;

                printf("CSR_BT_MAPS_GET_MESSAGE_IND received\n");
                if(inst->mapsInst->file != NULL)
                {
                    CsrUint16   i = 0;
                    if(inst->mapsInst->fileSizeRemain <= prim->obexResponsePacketLength)
                    {
                        bytesToRead = inst->mapsInst->fileSizeRemain;
                    }
                    else
                    {
                        bytesToRead = prim->obexResponsePacketLength;
                    }

                    body = CsrPmemAlloc(bytesToRead);
                    for (i = 0; i < bytesToRead; i++)
                    {
                        CsrBtFread(&body[i], 1, 1, inst->mapsInst->file);
                        if (body[i] == 0x0A) /* Substitute LF with LF CR */
                        {
                            body[i] = 0x0D;
                            i++;
                            lfCnt++;
                            if (i < bytesToRead)
                            {
                                body[i] = 0x0A;
                            }
                        }
                    }
                    if (i > bytesToRead)
                    { /* This can happen is the last byte read is 0x0A (LF)     */
                      /* In this case we will send one byte less in the current */
                      /* packet and leave the LF-char to be transmitted in the  */
                      /* next packet.                                           */
                        bytesToRead--;
                    }
                    inst->mapsInst->fileSizeRemain -= bytesToRead;
                    inst->mapsInst->fileSizeRemain += lfCnt;

                    if(inst->mapsInst->fileSizeRemain)
                    {
                        responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                    }
                    else
                    {
                        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        CsrBtFclose(inst->mapsInst->file);
                        inst->mapsInst->file = NULL;
                        inst->mapsInst->file_idx = 0;
                        inst->mapsInst->file_size = 0;
                    }
                    CsrBtMapsGetMessageResSend(prim->instanceId, responseCode, bytesToRead, body, FALSE);
                }
                else
                {
                    CsrBtMapsGetMessageResSend(prim->instanceId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 0, NULL, FALSE);
                }
                break;
            }

        case CSR_BT_MAPS_SET_MESSAGE_STATUS_IND:
            {
                CsrBtMapsSetMessageStatusInd    *prim = inst->msg;
                CsrBtObexResponseCode           responseCode;
                char                            *msgHandle = NULL;
                char                            *currentFile = NULL;
                char                            *tempFile = NULL;

                playMenu(inst);
                printf("CSR_BT_MAPS_SET_MESSAGE_STATUS_IND received\n");
                printf("Parameters in the request:\n");
                printf("     Message Handle: %s\n", unicode2String(prim->messageHandle));
                printf("     Status Indicator to modify: %s\n",
                                    prim->statusIndicator == CSR_BT_MAP_STATUS_IND_READ ? "ReadStatus" : "deletedStatus");
                printf("     Status value to set the indicator to: %s\n",
                                    prim->statusValue == CSR_BT_MAP_STATUS_VAL_NO ? "no" : "yes");

                if (prim->statusIndicator == CSR_BT_MAP_STATUS_IND_READ) /* Read status must be set */
                {
                    if (prim->messageHandle)
                    {
                        msgHandle = unicode2String(prim->messageHandle);
                        CsrPmemFree(inst->mapsInst->getFileName);

                        switch (msgHandle[0])
                        {
                            case '1':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("inbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "inbox", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            case '2':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("outbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "outbox", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            case '3':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("sent") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "sent", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            default:
                                {
                                    inst->mapsInst->getFileName = NULL;
                                    break;
                                }
                        }

                        currentFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                                 CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                                 CsrStrLen(inst->mapsInst->getFileName) + 1);
                        currentFile[0] = '\0';

                        currentFile = CsrStrCat(currentFile ,inst->mapsInst->dir_local_root);
                        currentFile = CsrStrCat(currentFile ,"telecom");
                        currentFile = CsrStrCat(currentFile ,PATH_SEP);
                        currentFile = CsrStrCat(currentFile ,"msg");
                        currentFile = CsrStrCat(currentFile ,PATH_SEP);
                        currentFile = CsrStrCat(currentFile ,inst->mapsInst->getFileName);

                        inst->mapsInst->file = CsrBtFopen(currentFile, "r+");
                        if (inst->mapsInst->file == NULL)
                        { /* The file open may fail because the file is in the Deleted folder - try that */
                            CsrPmemFree(inst->mapsInst->getFileName);
                            CsrPmemFree(currentFile);
                            inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("deleted") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                            sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "deleted", PATH_SEP, msgHandle, ".msg");
                            currentFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                                 CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                                     CsrStrLen(inst->mapsInst->getFileName) + 1);
                            currentFile[0] = '\0';
                            currentFile = CsrStrCat(currentFile ,inst->mapsInst->dir_local_root);
                            currentFile = CsrStrCat(currentFile ,"telecom");
                            currentFile = CsrStrCat(currentFile ,PATH_SEP);
                            currentFile = CsrStrCat(currentFile ,"msg");
                            currentFile = CsrStrCat(currentFile ,PATH_SEP);
                            currentFile = CsrStrCat(currentFile ,inst->mapsInst->getFileName);

                            inst->mapsInst->file = CsrBtFopen(currentFile, "r");
                        }
                        if (inst->mapsInst->file == NULL)
                        {
                            responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                            perror("\n     Error");
                        }
                        else
                        {
                            char    *line = CsrPmemAlloc(MAX_LINE_LENGTH + 1);
                            long    tempPos = 0;
                            CsrBool loop = TRUE;

                            while (loop)
                            {
                                if (!CsrBtFgets(line, MAX_LINE_LENGTH, inst->mapsInst->file))
                                {
                                    loop = FALSE;
                                }
                                if (CsrStrStr(line, "STATUS:"))
                                {
                                    char *statusStr = CsrPmemAlloc(CsrStrLen("STATUS:") + CsrStrLen("UNREAD") + 1);

                                    CsrBtFseek(inst->mapsInst->file, tempPos, CSR_SEEK_SET);
                                    statusStr = "STATUS:";
                                    CsrStrCat(statusStr, (char *)(prim->statusValue == CSR_BT_MAP_STATUS_VAL_NO ? "UNREAD" : "READ  "));

                                    /* Find the corrct line to put the new status */
                                    /* Status line is found after the version line */
                                    while (CsrBtFgets(line, MAX_LINE_LENGTH, inst->mapsInst->file) &&
                                           !CsrStrStr(line, "VERSION:"))
                                    {
                                        /* Do nothing - just read next line */
                                    }
                                    CsrBtFwrite(statusStr, 1, CsrStrLen(statusStr), inst->mapsInst->file);
                                    loop = FALSE;
                                    responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                                }
                            }
                            CsrPmemFree(line);
                        }
                        CsrPmemFree(currentFile);
                    }
                    else /* belongs to the "if (prim->messageHandle)"-statement */
                    {
                        inst->mapsInst->getFileName = NULL;
                    }
                }
                else if (prim->statusIndicator == CSR_BT_MAP_STATUS_IND_DELETE) /* File must be deleted or recovered */
                {
                    if (prim->messageHandle)
                    {
                        char        *newFileName = NULL;
                        char        *oldFileName = NULL;
                        CsrBool     loop = TRUE;
                        CsrBool     fileInDeletedFolder = FALSE;
                        CsrFileHandle        *dummyFp = NULL;

                        msgHandle = unicode2String(prim->messageHandle);
                        CsrPmemFree(inst->mapsInst->getFileName);

                        switch (msgHandle[0])
                        {
                            case '1':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("inbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "inbox", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            case '2':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("outbox") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "outbox", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            case '3':
                                {
                                    inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("sent") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                                    sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "sent", PATH_SEP, msgHandle, ".msg");
                                    break;
                                }
                            default:
                                {
                                    inst->mapsInst->getFileName = NULL;
                                    break;
                                }
                        }

                        if (inst->mapsInst->getFileName)
                        {   /* Check if file exists */
                            CsrPmemFree(currentFile);
                            currentFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                                     CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                                     CsrStrLen(inst->mapsInst->getFileName) + 1);
                            currentFile[0] = '\0';
                            currentFile = CsrStrCat(currentFile ,inst->mapsInst->dir_local_root);
                            currentFile = CsrStrCat(currentFile ,"telecom");
                            currentFile = CsrStrCat(currentFile ,PATH_SEP);
                            currentFile = CsrStrCat(currentFile ,"msg");
                            currentFile = CsrStrCat(currentFile ,PATH_SEP);
                            currentFile = CsrStrCat(currentFile ,inst->mapsInst->getFileName);
                            dummyFp = CsrBtFopen(currentFile, "r");
                        }

                        if (inst->mapsInst->getFileName == NULL || dummyFp == NULL)
                        {   /* The file open may fail because the file is placed in the "Deleted"-folder. Let's try that */
                            inst->mapsInst->getFileName =
                                                CsrPmemAlloc(CsrStrLen("deleted") + CsrStrLen(PATH_SEP) + CsrStrLen(msgHandle) + CsrStrLen(".msg") + 1);
                            sprintf(inst->mapsInst->getFileName, "%s%s%s%s", "deleted", PATH_SEP, msgHandle, ".msg");
                            tempFile = CsrPmemAlloc(CsrStrLen(inst->mapsInst->dir_local_root) +
                                                 CsrStrLen("telecom") + CsrStrLen(PATH_SEP) + CsrStrLen("msg") + CsrStrLen(PATH_SEP) +
                                                 CsrStrLen(inst->mapsInst->getFileName) + 1);
                            tempFile[0] = '\0';
                            tempFile = CsrStrCat(tempFile ,inst->mapsInst->dir_local_root);
                            tempFile = CsrStrCat(tempFile ,"telecom");
                            tempFile = CsrStrCat(tempFile ,PATH_SEP);
                            tempFile = CsrStrCat(tempFile ,"msg");
                            tempFile = CsrStrCat(tempFile ,PATH_SEP);
                            tempFile = CsrStrCat(tempFile ,inst->mapsInst->getFileName);

                            dummyFp = CsrBtFopen(tempFile, "r");
                            fileInDeletedFolder = TRUE;
                            CsrPmemFree(tempFile);
                        }

                        if (inst->mapsInst->getFileName && dummyFp)
                        {
                            CsrBtFclose(dummyFp);
                            dummyFp = NULL;

                            newFileName = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH + 1);
                            newFileName[0] = '\0';
                            oldFileName = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH + 1);
                            oldFileName[0] = '\0';

                            if (prim->statusValue == CSR_BT_MAP_STATUS_VAL_NO)
                            {   /* File must be recovered */
                                oldFileName = CsrStrCat(oldFileName, inst->mapsInst->dir_local_root);
                                oldFileName = CsrStrCat(oldFileName, "telecom");
                                oldFileName = CsrStrCat(oldFileName ,PATH_SEP);
                                oldFileName = CsrStrCat(oldFileName , "msg");
                                oldFileName = CsrStrCat(oldFileName ,PATH_SEP);
                                oldFileName = CsrStrCat(oldFileName, "deleted");
                                oldFileName = CsrStrCat(oldFileName ,PATH_SEP);
                                oldFileName = CsrStrCat(oldFileName, msgHandle);
                                oldFileName = CsrStrCat(oldFileName, ".msg");

                                responseCode = mapsMoveFile(inst, currentFile, oldFileName);
                            }
                            else if (prim->statusValue == CSR_BT_MAP_STATUS_VAL_YES)
                            {
                                newFileName = CsrStrCat(newFileName, inst->mapsInst->dir_local_root);
                                newFileName = CsrStrCat(newFileName, "telecom");
                                newFileName = CsrStrCat(newFileName ,PATH_SEP);
                                newFileName = CsrStrCat(newFileName , "msg");
                                newFileName = CsrStrCat(newFileName ,PATH_SEP);
                                newFileName = CsrStrCat(newFileName, "deleted");
                                newFileName = CsrStrCat(newFileName ,PATH_SEP);
                                newFileName = CsrStrCat(newFileName, msgHandle);
                                newFileName = CsrStrCat(newFileName, ".msg");

                                if (fileInDeletedFolder)
                                {   /* File is currently in the "Deleted"-folder and must be permanently deleted  */
                                    unlink((char *)newFileName);
                                    responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                                }
                                else
                                {   /* File must be deleted, i.e. move file to deleted folder */
                                    responseCode = mapsMoveFile(inst, newFileName, currentFile);
                                }
                            }
                            else
                            {
                                responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                            }
                        }
                        else
                        {
                            responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                        }
                    } /* Belongs to the "if (prim->messageHandle)"-statement */
                    else
                    {
                        responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                    }
                }
                else
                {
                    responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                }
                CsrBtMapsSetMessageStatusResSend(prim->instanceId, responseCode, FALSE);
                break;
            }

        case CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND:
            {
                CsrBtMapsPushMessageHeaderInd *prim = inst->msg;
                char                          *fileName = NULL;
                char                          *messageHandle = NULL;
                char                          *oldDir = NULL;
                char                          path[CSR_BT_MAX_PATH_LENGTH];
                CsrUint8                      *name = NULL;
                CsrUint8                      nameLen = 0;

                /* Store current directory */
                oldDir = inst->mapsInst->dir_local;

                /* Change to root directory */
                CsrBtChdir(inst->mapsInst->dir_local_root);

                /* convert big endian unicode to little unicode */
                name = unicode2String(prim->folderName);

                if (inst->mapsInst->simulateDirectSend)
                {
                    sprintf(path, "%s%s%s", inst->mapsInst->dir_local, "sent", PATH_SEP);
                }
                else if (!name)
                {
                    path[0] = '\0';
                    CsrStrCat(&path[0], inst->mapsInst->dir_local);
                }
                else
                {
                    sprintf(path, "%s%s%s", inst->mapsInst->dir_local, name, PATH_SEP);
                }

                playMenu(inst);
                printf("CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND received\n\n");
                printf("Parameters in the request:\n");
                printf("     Folder name: %s\n", name);
                if (prim->transparent)
                {
                    printf("     Transparent: %s\n", prim->transparent == CSR_BT_MAP_TRANSPARENT_OFF ? "keep message" : "don't keep message");
                }
                if (prim->retry)
                {
                    printf("     Retry: %s\n", prim->retry == CSR_BT_MAP_RETRY_OFF ? "Off" : "On");
                }
                printf("     Charset: %s\n", prim->charset == CSR_BT_MAP_CHARSET_NATIVE ? "<native>" : "<UTF-8>");

                CsrBtChdir(path);
                fileName = getNextFileName(path);
                if (fileName)
                {
                    inst->mapsInst->file = CsrBtFopen(fileName, "w+");
                    printf("\nName for new file is: %s%s\n\n", path, fileName);
                }
                else
                {
                    inst->mapsInst->file = NULL;
                    printf("\nERROR: A new file could not be added to: %s\n\n", path);
                }

                if (inst->mapsInst->file)
                {
                    nameLen = CsrStrLen(fileName) - CsrStrLen(".msg");
                    messageHandle = fileName;
                    messageHandle[nameLen] = '\0';
                }
                else
                {
                    messageHandle = CsrStrDup("0000000000000000");
                }
                /* Set current dir to the directory used before this operation */
                CsrBtChdir(oldDir);
                CsrBtMapsPushMessageHeaderResSend(prim->instanceId, (CsrCharString *)&messageHandle[0], FALSE);
                break;
            }

        case CSR_BT_MAPS_PUSH_MESSAGE_IND:
            {
                CsrBtMapsPushMessageInd  *prim = inst->msg;
                CsrBtObexResponseCode    responseCode;
                char                     *bodyToSend = NULL;

                printf("CSR_BT_MAPS_PUSH_MESSAGE_IND received with bodyLength %d\n", prim->bodyLength);

                if(inst->mapsInst->file != NULL)
                {
                    CsrUint32   i = 0;

                    for (i = 0; i < prim->bodyLength; i++)
                    {
                        if (prim->body[i] == 0x0D && prim->body[i+1] == 0x0A) /* If it is CR LF */
                        {
                            CsrBtFwrite("\n", 1, 1, inst->mapsInst->file);
                            i++;
                        }
                        else if (prim->body[i] == 0x0A) /* If it is LF */
                        {
                            CsrBtFwrite("\n", 1, 1, inst->mapsInst->file);
                        }
                        else if (inst->mapsInst->simulateDirectSend &&
                                 prim->body[i]     == 'O' &&
                                 prim->body[i + 1] == 'U' &&
                                 prim->body[i + 2] == 'T' &&
                                 prim->body[i + 3] == 'B' &&
                                 prim->body[i + 4] == 'O' &&
                                 prim->body[i + 5] == 'X')
                        {
                            CsrBtFwrite("S", 1, 1, inst->mapsInst->file);
                            i++;
                            CsrBtFwrite("E", 1, 1, inst->mapsInst->file);
                            i++;
                            CsrBtFwrite("N", 1, 1, inst->mapsInst->file);
                            i++;
                            CsrBtFwrite("T", 1, 1, inst->mapsInst->file);
                            i++;
                            CsrBtFwrite(" ", 1, 1, inst->mapsInst->file);
                            i++;
                            CsrBtFwrite(" ", 1, 1, inst->mapsInst->file);
                            i++;
                        }
                        else
                        {
                            CsrBtFwrite(&(prim->body[i]), 1, 1, inst->mapsInst->file);
                        }
                    }

                    if(prim->finalFlag)
                    {
                        CsrBtFclose(inst->mapsInst->file);
                        inst->mapsInst->file = NULL;
                        inst->mapsInst->file_idx = 0;
                        inst->mapsInst->file_size = 0;
                        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    }
                    else
                    {
                        responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                    }
                }
                else
                {
                    responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }
                CsrBtMapsPushMessageResSend(prim->instanceId, responseCode, FALSE);
                break;
            }

        case CSR_BT_MAPS_UPDATE_INBOX_IND:
            {
                CsrBtMapsUpdateInboxInd  *prim = inst->msg;

                playMenu(inst);
                printf("CSR_BT_MAPS_UPDATE_INBOX_IND received\n");

                CsrBtMapsUpdateInboxResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
                break;
            }

        case CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND:
            {
                CsrBtMapsNotificationRegistrationInd  *prim = inst->msg;

                inst->mapsInst->registerNotificationStatus = prim->notificationStatus;

                playMenu(inst);
                printf("CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND with status %s received\n",
                                                    prim->notificationStatus ? "TRUE" : "FALSE");

                CsrBtMapsNotificationRegistrationResSend(prim->instanceId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
                break;
            }

        case CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND:
            {
                CsrBtMapsNotificationRegistrationCompleteInd  *prim = inst->msg;

                printf("CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND with result 0x%04X(0x%04X) received\n",
                                                                                    prim->resultCode,
                                                                                    prim->resultSupplier);
                break;
            }

        case CSR_BT_MAPS_EVENT_NOTIFICATION_IND:
            {
                CsrBtMapsEventNotificationInd  *prim = inst->msg;

                playMenu(inst);
                printf("CSR_BT_MAPS_EVENT_NOTIFICATION_IND with allowed packet length = %d received\n",
                                                                         prim->obexResponsePacketLength);
                inst->mapsInst->eventNotificationInstanceId = prim->instanceId;
                inst->mapsInst->eventNotificationBytesAllowed = prim->obexResponsePacketLength;

                if (inst->mapsInst->eventNotifiactionKeyState == EVENT_NOTIFICATION_KEY_STATE_NULL)
                {
                    inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_EVENT;
                    CsrPmemFree(inst->mapsInst->eventNotificationBody);
                    inst->mapsInst->eventNotificationBody = CsrPmemAlloc(CsrStrLen(XML_EVENT_NOTIFICATION_HEADER) + 1);
                    inst->mapsInst->eventNotificationBody[0] = '\0';
                    CsrStrCat(inst->mapsInst->eventNotificationBody, XML_EVENT_NOTIFICATION_HEADER);
                    printEventNotificationSubMenu(inst->mapsInst);
                }
                else if (inst->mapsInst->eventNotifiactionKeyState == EVENT_NOTIFICATION_KEY_STATE_DONE)
                {
                    sendRemainingEventNotificationBytes(inst->mapsInst);
                    playMenu(inst);
                    printf("\nNotification Event Response sent\n");
                }
                else
                {
                    printf("\nWARNING: CSR_BT_MAPS_EVENT_NOTIFICATION_IND received in unexpected state\n");
                    printf("         Primitive is ignored!\n");
                }
                break;
            }

        case CSR_BT_MAPS_EVENT_NOTIFICATION_CFM:
            {
                CsrBtMapsEventNotificationCfm  *prim = inst->msg;

                printf("CSR_BT_MAPS_EVENT_NOTIFICATION_CFM with result 0x%04X(0x%04X) received\n",
                                                                                    prim->resultCode,
                                                                                    prim->resultSupplier);
                inst->mapsInst->eventNotificationPending = FALSE;
                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_NULL;
                break;
            }

        default:
            {
                printf("Unhandled MAPS primitive 0x%04X\n", type);
                break;
            }
    }

    CsrBtMapsFreeUpstreamMessageContents(CSR_BT_MAPS_PRIM, inst->msg);
}

static void mapScAppSetFnFini(void *pFiniData)
{
    MapsAppInst *inst = (MapsAppInst *) pFiniData;
    stateChange(&CURRENT_INST, CURRENT_INST.state_old);
    system(CLS);
    playMenu(inst);
}

static void mapScAppSetFnInit(void *pInitData)
{
    MapsAppInst *inst = (MapsAppInst *) pInitData;

    stateChange(&CURRENT_INST, STATE_PAIRING);
}

static void* mapScAppSetFnGetMessage(void *pRecvMsgPData)
{
    MapsAppInst *inst  = (MapsAppInst *) pRecvMsgPData;
    void *result          = inst->msg;
    inst->msg         = NULL;
    return result;
}

/* Initialize demo app */
void CsrBtAppInit(void **gash)
{
    MapsAppInst *inst;

    inst = CsrPmemAlloc(sizeof(MapsAppInst));
    CsrMemSet(inst, 0, sizeof(MapsAppInst));
    *gash = inst;

    /* Application handle */
    inst->qid = TESTQUEUE;
    inst->mapsInstanceListReceived = FALSE;

#ifdef __linux__
    inst->cls = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    getcwd(inst->cls, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(inst->cls, PATH_SEP);
    CsrStrCat(inst->cls, CLS);
#else
    inst->cls = CsrStrDup("cls");
#endif

    CsrSchedTimerSet(KEY_CHECK_TIMEOUT, keyboardTimer, 0, inst);

    /* Security App Configuration */
    CsrBtScActivateReqSend(inst->qid);
    scAppInitInstanceData(&inst->scInstData);

    scAppSetFnFini(&inst->scInstData, mapScAppSetFnFini, inst);
    scAppSetFnInit(&inst->scInstData, mapScAppSetFnInit, inst);
    scAppSetSecLevelSend(&inst->scInstData, scAppSendProfileSecurityLevelReq, &inst->scInstData);

    scAppSetFnGetMessage(&inst->scInstData, mapScAppSetFnGetMessage, inst);

    scAppSetSecLevelPrim(&inst->scInstData,
                         0,     /* Index 0. Single profile */
                         "MAPS", CSR_BT_MAPS_PRIM,
                         CSR_BT_MAPS_IFACEQUEUE,
                         CSR_BT_MAPS_SECURITY_LEVEL_REQ, SC_APP_SET_SEC_LEVEL_NA);

    CsrBtMapsGetInstanceIdsReqSend(inst->qid);
    playMenu(inst);
}

/* Demo application handler */
void CsrBtAppHandler(void **gash)
{
    MapsAppInst *inst = *gash;

    if(CsrSchedMessageGet(&(inst->event), &(inst->msg)))
    {
        switch (inst->event)
        {
            case KEY_MESSAGE:
                handleKeyboardEvents(inst);
                break;

            case CSR_BT_MAPS_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) inst->msg;
                    switch(*prim)
                    {
                        case CSR_BT_MAPS_SECURITY_LEVEL_CFM:
                            handleScSecLevelPrim(&inst->scInstData);
                            break;

                        default:
                            handleMapsEvents(inst);
                            break;
                    }
                    break;
                }
            case CSR_BT_SC_PRIM:
                handleScPrim(&inst->scInstData);
                break;

            default:
                printf("Unhandled event in default handler: 0x%04X\n", inst->event);
                break;
        }

        if(inst->msg != NULL)
        {
            CsrPmemFree(inst->msg);
            inst->msg = NULL;
        }
    }
}

/* Demo application deinitialise */
#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint8 i;
    CsrUint16 msgType=0;
    void *msgData=NULL;
    MapsAppInst *inst;

    inst = *gash;
    printf("Cleaning up...\n");

    /* Get a message from the demoapplication message queue. The message is returned in prim
     * and the event type in eventType */
    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
            case CSR_BT_MAPS_PRIM:
            {
                CsrBtMapsFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
        }
        CsrPmemFree(msgData);
    }

    for(i=0; i<inst->numberOfInstances;i++)
    {
        /* Free instance */
        if(inst->mapsInst[i].file)
        {
            CsrBtFclose(inst->mapsInst[i].file);
            inst->mapsInst[i].file = NULL;
            inst->mapsInst[i].file_idx = 0;
            inst->mapsInst[i].file_size = 0;
        }
        CsrPmemFree(inst->mapsInst[i].folder);
        CsrPmemFree(inst->mapsInst[i].dir_local);
    }
    CsrPmemFree(inst->cls);
    CsrPmemFree(inst);
}
#endif

static CsrBool getUint8Value(MapsInstance *inst, CsrUint8 *value, CsrUint8 key)
{
    CsrBool res = FALSE;
    if (RETURN_KEY == key)
    {
        *value = (CsrUint8) CsrStrToInt(inst->tempValue);
        CsrMemSet(inst->tempValue, 0, 6);
        inst->tempValueLen = 0;
        res = TRUE;
    }
    else if (BACKSPACE_KEY == key)
    {
        if(inst->tempValueLen)
        {
            inst->tempValue[--inst->tempValueLen] = '\0';
            printf("\b \b");
        }
    }
    else if ((key >= '0') && (key <= '9'))
    {
        inst->tempValue[inst->tempValueLen++] = key;
        if(inst->tempValueLen <= 3 && CsrStrToInt(inst->tempValue) < 256)
        {
            printf("%c",key);
        }
        else
        {
            CsrUint8 i;
            printf("Value too high for CsrUint8. Max allowed = %d!!\n",0xFF);
            inst->tempValue[--inst->tempValueLen] = '\0';

            for(i=0;i<inst->tempValueLen;i++)
            {
                printf("%c",inst->tempValue[i]);
            }
        }
    }
    else
    {
        printf("Invalid char: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
    }
    return res;
}

static CsrBool getUint16Value(MapsInstance *inst, CsrUint16 *value, CsrUint8 key)
{
    CsrBool res = FALSE;
    if (RETURN_KEY == key)
    {
        *value = (CsrUint8) CsrStrToInt(inst->tempValue);
        CsrMemSet(inst->tempValue, 0, 6);
        inst->tempValueLen = 0;
        res = TRUE;
    }
    else if (BACKSPACE_KEY == key)
    {
        if(inst->tempValueLen)
        {
            inst->tempValue[--inst->tempValueLen] = '\0';
            printf("\b \b");
        }
    }
    else if ((key >= '0') && (key <= '9'))
    {
        inst->tempValue[inst->tempValueLen++] = key;
        if(inst->tempValueLen <= 5 && CsrStrToInt(inst->tempValue) < 0xFFFF)
        {
            printf("%c",key);
        }
        else
        {
            CsrUint8 i;
            printf("Value too high for CsrUint16. Max allowed = %d!!\n",0xFFFF);
            inst->tempValue[--inst->tempValueLen] = '\0';

            for(i=0;i<inst->tempValueLen;i++)
            {
                printf("%c",inst->tempValue[i]);
            }
        }
    }
    else
    {
        printf("Invalid char: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
    }
    return res;
}

static CsrBool getString(CsrCharString **string, CsrUint8 key)
{
    CsrBool res = FALSE;
    if (RETURN_KEY == key)
    {
        res = TRUE;
    }
    else if (BACKSPACE_KEY == key)
    {
        if(*string != NULL)
        {
            (*string)[CsrStrLen(*string)-1] = '\0';
            printf("\b \b");
        }
    }
    else if ((key >= 32) && (key < 127))
    {
        CsrCharString *temp = *string;
        CsrUint16 tempLen;

        if(*string == NULL)
        {
            tempLen = 0;
            *string = CsrPmemAlloc(2);
        }
        else
        {
            tempLen = CsrStrLen(*string);
            *string = CsrPmemAlloc(tempLen + 2);
            CsrStrCpy(*string,temp);
            CsrPmemFree(temp);
        }

        (*string)[tempLen] = key;
        (*string)[tempLen+1] = '\0';

        printf("%c", key);
    }
    else
    {
        printf("Invalid char: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
    }
    return res;
}

static CsrBool getUint8Bitmask(CsrUint8 *bitmask, CsrUint8 key, CsrUint8 maxValidSelections)
{
    CsrBool res = FALSE;

    switch(key)
    {
        case '0':
            *bitmask = CSR_BT_MAP_PARAMETERS_ALL;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if((key - '1') < maxValidSelections)
                CSR_BIT_TOGGLE(*bitmask, (key - '1'));
            else
                printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);

            break;
        case RETURN_KEY:
            res = TRUE;
            break;
        default:
            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
            break;
    }
    return res;
}


static CsrBool getUint32Bitmask(CsrUint32 *bitmask, CsrUint8 key)
{
    CsrBool res = FALSE;

    switch(key)
    {
        case '0':
            *bitmask = CSR_BT_MAP_PARAMETERS_ALL;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            CSR_BIT_TOGGLE(*bitmask, (key - '1'));
            break;
        case 'a':
            CSR_BIT_TOGGLE(*bitmask, 9);
            break;
        case 'b':
            CSR_BIT_TOGGLE(*bitmask, 10);
            break;
        case 'c':
            CSR_BIT_TOGGLE(*bitmask, 11);
            break;
        case 'd':
            CSR_BIT_TOGGLE(*bitmask, 12);
            break;
        case 'e':
            CSR_BIT_TOGGLE(*bitmask, 13);
            break;
        case 'f':
            CSR_BIT_TOGGLE(*bitmask, 14);
            break;
        case 'g':
            CSR_BIT_TOGGLE(*bitmask, 15);
            break;
        case RETURN_KEY:
            res = TRUE;
            break;
        default:
            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
            break;
    }
    return res;
}

/* Handle keypresses */
void handleKeyboardEvents(MapsAppInst *inst)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)inst->msg;

    /* Globally catched keys */
    if(key->key == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    if(!inst->mapsInstanceListReceived)
    {
        return;
    }

    if (key->key == 'z')
    {
        listMapsInstances(inst);
        return;
    }
    else if(key->key == ' ')
    {
        playMenu(inst);
    }
    else if(key->key == 's')
    {
        inst->mapsInst->simulateDirectSend = !inst->mapsInst->simulateDirectSend;
        playMenu(inst);
    }
    else
    {
        /* State-based catching of keys */
        switch(CURRENT_INST.state)
        {
            case STATE_IDLE:
                {
                    switch(key->key)
                    {
                        case 'a':
                            {
                                CsrBtMapsActivateExtReqSend(CURRENT_INST.mapsInstanceId, inst->qid, 
                                                         (CSR_BT_MAP_EMAIL_SUPPORT | 
                                                          CSR_BT_MAP_SMS_GSM_SUPPORT |
                                                          CSR_BT_MAP_SMS_CDMA_SUPPORT | 
                                                          CSR_BT_MAP_MMS_SUPPORT),
                                                         MAX_PACKET_SIZE,
                                                         0, /*windowSize*/
                                                         TRUE,
                                                        inst->nextMapsInstanceId ,0,NULL);
                                inst->nextMapsInstanceId++;
                                stateChange(&CURRENT_INST, STATE_ACTIVATING);
                                playMenu(inst);
                                return;
                            }
                        default:
                            {
                                printf("\nInvalid key press. Please try again\n");
                                break;
                            }
                    }
                    break;
                }
            case STATE_ACTIVATING: /* Fall-through */
            case STATE_ACTIVATED:
                {
                    switch(key->key)
                    {
                        case 'x':
                            {
                                CsrBtMapsDeactivateReqSend(CURRENT_INST.mapsInstanceId);
                                stateChange(&CURRENT_INST, STATE_DEACTIVATING);
                                inst->nextMapsInstanceId--;
                                playMenu(inst);
                                return;
                            }
                        default:
                            {
                                printf("\nInvalid key press. Please try again\n");
                                break;
                            }
                    }
                    break;
                }

            case STATE_PAIRING:
                {
                    handleScKeybPrim(&inst->scInstData);
                    break;
                }

            case STATE_CONNECTING:
                {
                    CsrBtObexResponseCode response = 0;

                    switch(key->key)
                    {
                        case '0':
                            {
                                response = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                                break;
                            }
                        case '1':
                            {
                                response = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                                break;
                            }
                        case '2':
                            {
                                response = CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE;
                                break;
                            }
                        case '3':
                            {
                                response = CSR_BT_OBEX_UNAUTHORIZED_CODE;
                                break;
                            }
                        case '4':
                            {
                                response = CSR_BT_OBEX_PRECONDITION_FAILED_RESPONSE_CODE;
                                break;
                            }
                        case '5':
                            {
                                response = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                break;
                            }
                        case '6':
                            {
                                response = CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE;
                                break;
                            }
                        case '7':
                            {
                                response = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
                                break;
                            }
                        case '8':
                            {
                                response = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                                break;
                            }
                        case 'x':
                            {
                                CsrBtMapsDeactivateReqSend(CURRENT_INST.mapsInstanceId);
                                stateChange(&CURRENT_INST, STATE_DEACTIVATING);
                                inst->nextMapsInstanceId--;
                                playMenu(inst);
                                return;
                            }
                        default:
                            {
                                printf("\nInvalid key press. Please try again\n");
                                break;
                            }
                    }
                    if (response)
                    {
                        CsrBtMapsConnectResSend(CURRENT_INST.mapsInstanceId, response);
                        if (response == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                        {
                            stateChange(&CURRENT_INST, STATE_CONNECTED);
                            playMenu(inst);
                            printf("\nConnection successful\n\n");
                        }
                    }
                    break;
                }
            case STATE_CONNECTED:
                {
                    switch (inst->mapsInst->eventNotifiactionKeyState)
                    {
                        case EVENT_NOTIFICATION_KEY_STATE_NULL:
                            {
                                switch(key->key)
                                {
                                    case 'n':
                                        {
                                            if (inst->mapsInst->registerNotificationStatus)
                                            {
                                                if (inst->mapsInst->eventNotificationPending)
                                                {
                                                    printf("The request can not be sent. Another Event Notification is pending\n\n");
                                                }
                                                else
                                                {
                                                    CsrBtMapsEventNotificationReqSend(CURRENT_INST.mapsInstanceId);
                                                    inst->mapsInst->eventNotificationPending = TRUE;
                                                }
                                            }
                                            else
                                            {
                                                printf("The request ca not be sent. Notification Registration Status is OFF\n\n");
                                            }
                                            break;
                                        }
                                    case 'x':
                                        {
                                            CsrBtMapsDeactivateReqSend(CURRENT_INST.mapsInstanceId);
                                            stateChange(&CURRENT_INST, STATE_DEACTIVATING);
                                            inst->nextMapsInstanceId--;
                                            playMenu(inst);
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                break;
                            }
                        case EVENT_NOTIFICATION_KEY_STATE_SELECT_EVENT:
                            {
                                char    *tempP = NULL;

                                tempP = inst->mapsInst->eventNotificationBody;

                                switch(key->key)
                                {
                                    case '1':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_NEW_MESSAGE) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_NEW_MESSAGE);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '2':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_SUCCESS) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_SUCCESS);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '3':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_SENDING_SUCCESS) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_SENDING_SUCCESS);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '4':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_FAILURE) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_DELIVERY_FAILURE);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '5':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_SENDING_FAILURE) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_SENDING_FAILURE);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '6':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_FULL) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_FULL);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    case '7':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_AVAILABLE) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_MEMORY_AVAILABLE);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    case '8':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_DELETED) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_DELETED);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            break;
                                        }
                                    case '9':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_SHIFT) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_EVENT_TYPE_MESSAGE_SHIFT);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE;
                                            inst->mapsInst->eventNotificationOldFolderNeeded = TRUE;
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                CsrPmemFree(tempP);
                                break;
                            }
                        case EVENT_NOTIFICATION_KEY_STATE_SELECT_HANDLE:
                            {
                                char    *tempP = NULL;

                                tempP = inst->mapsInst->eventNotificationBody;
                                /* The length of all handles are the same, so we might as well alloc the memory here */
                                inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_HANDLE_INBOX_1) + 1);
                                inst->mapsInst->eventNotificationBody[0] = '\0';
                                CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);

                                switch(key->key)
                                {
                                    case '1':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_INBOX_1);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '2':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_INBOX_2);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '3':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_INBOX_3);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '4':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_OUTBOX_1);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '5':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_OUTBOX_2);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '6':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_OUTBOX_3);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '7':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_SENT_1);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '8':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_SENT_2);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case '9':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_SENT_3);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case 'A':
                                    case 'a':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_DELETED_1);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case 'B':
                                    case 'b':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_DELETED_2);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    case 'C':
                                    case 'c':
                                        {
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_HANDLE_DELETED_3);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER;
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                CsrPmemFree(tempP);
                                break;
                            }

                        case EVENT_NOTIFICATION_KEY_STATE_SELECT_FOLDER:
                            {
                                char    *tempP = NULL;

                                tempP = inst->mapsInst->eventNotificationBody;

                                switch(key->key)
                                {
                                    case '1':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_FOLDER_INBOX) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_FOLDER_INBOX);
                                            if (inst->mapsInst->eventNotificationOldFolderNeeded)
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER;
                                            }
                                            else
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            }
                                            break;
                                        }
                                    case '2':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_OUTBOX) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_OUTBOX);
                                            if (inst->mapsInst->eventNotificationOldFolderNeeded)
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER;
                                            }
                                            else
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            }
                                            break;
                                        }
                                    case '3':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_SENT) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_SENT);
                                            if (inst->mapsInst->eventNotificationOldFolderNeeded)
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER;
                                            }
                                            else
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            }
                                            break;
                                        }
                                    case '4':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_DELETED) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_DELETED);
                                            if (inst->mapsInst->eventNotificationOldFolderNeeded)
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER;
                                            }
                                            else
                                            {
                                                inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            }
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                CsrPmemFree(tempP);
                                break;
                            }

                        case EVENT_NOTIFICATION_KEY_STATE_SELECT_OLD_FOLDER:
                            {
                                char    *tempP = NULL;

                                tempP = inst->mapsInst->eventNotificationBody;

                                switch(key->key)
                                {
                                    case '1':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_INBOX) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_INBOX);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            break;
                                        }
                                    case '2':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_OUTBOX) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_OUTBOX);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            break;
                                        }
                                    case '3':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_SENT) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_SENT);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            break;
                                        }
                                    case '4':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_OLD_FOLDER_DELETED) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_OLD_FOLDER_DELETED);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE;
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                CsrPmemFree(tempP);
                                break;
                            }

                        case EVENT_NOTIFICATION_KEY_STATE_SELECT_MSG_TYPE:
                            {
                                char    *tempP = NULL;

                                tempP = inst->mapsInst->eventNotificationBody;

                                switch(key->key)
                                {
                                    case '1':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_TYPE_EMAIL) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_TYPE_EMAIL);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    case '2':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_TYPE_SMS_GSM) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_TYPE_SMS_GSM);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    case '3':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_TYPE_SMS_CDMA) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_TYPE_SMS_CDMA);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    case '4':
                                        {
                                            inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(EVENT_NOTIFICATION_TYPE_MMS) + 1);
                                            inst->mapsInst->eventNotificationBody[0] = '\0';
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                                            CsrStrCat(inst->mapsInst->eventNotificationBody, EVENT_NOTIFICATION_TYPE_MMS);
                                            inst->mapsInst->eventNotifiactionKeyState = EVENT_NOTIFICATION_KEY_STATE_DONE;
                                            break;
                                        }
                                    default:
                                        {
                                            printf("\nInvalid key press. Please try again\n");
                                            break;
                                        }
                                }
                                CsrPmemFree(tempP);
                                break;
                            }
                    }
                    printEventNotificationSubMenu(inst->mapsInst);
                    if (inst->mapsInst->eventNotifiactionKeyState == EVENT_NOTIFICATION_KEY_STATE_DONE)
                    {
                        char    *tempP = NULL;

                        tempP = inst->mapsInst->eventNotificationBody;
                        inst->mapsInst->eventNotificationBody = (char *)CsrPmemAlloc(CsrStrLen(tempP) + CsrStrLen(XML_EVENT_NOTIFICATION_FOOTER) + 1);
                        inst->mapsInst->eventNotificationBody[0] = '\0';
                        CsrStrCat(inst->mapsInst->eventNotificationBody, tempP);
                        CsrStrCat(inst->mapsInst->eventNotificationBody, XML_EVENT_NOTIFICATION_FOOTER);
                        CsrPmemFree(tempP);
                        sendRemainingEventNotificationBytes(inst->mapsInst);
                        playMenu(inst);
                    }
                    break;
                }

            case STATE_SELECT_MAPS_INSTANCE:
                {
                    if((key->key - '0') < inst->numberOfInstances)
                    {
                        printf("%c\n", key->key);
                        inst->activeInst = key->key - '0';
                        stateChange(&CURRENT_INST, CURRENT_INST.state_old);
                        playMenu(inst);
                    }
                    else
                    {
                        printf("Bad index selection!\nSelect index from the list: ");
                    }
                    break;
                }
            case STATE_SELECT_MAS_INSTANCE_ID:
                {
                    if((key->key - '0') < CURRENT_INST.masInstanceListSize)
                    {
                        printf("%c\n", key->key);
                        CsrBtMapsSelectMasInstanceResSend(CURRENT_INST.mapsInstanceId,TRUE,CURRENT_INST.masInstanceList[key->key - '0'].masInstanceId);
                        CsrPmemFree(CURRENT_INST.masInstanceList);
                        CURRENT_INST.masInstanceList = NULL;
                        CURRENT_INST.masInstanceListSize = 0;
                        playMenu(inst);
                    }
                    else if (key->key == 'C' || key->key == 'c')
                    {
                        CsrBtMapsSelectMasInstanceResSend(CURRENT_INST.mapsInstanceId,FALSE,0);
                        CsrPmemFree(CURRENT_INST.masInstanceList);
                        CURRENT_INST.masInstanceList = NULL;
                        CURRENT_INST.masInstanceListSize = 0;
                        playMenu(inst);
                    }
                    else
                    {
                        printf("Bad index selection!\nSelect index from the list: ");
                    }
                    break;
                }
            default:
                {
                    break;
                }
        } /* End of switch(CURRENT_INST.state) */
    } /* End of else-part of "if (key->key == 'z')" */
    /* Catch space */
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
