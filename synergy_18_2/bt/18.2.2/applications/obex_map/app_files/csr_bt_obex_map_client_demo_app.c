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

#include "csr_sched.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_mapc_prim.h"
#include "csr_bt_mapc_lib.h"
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
#define MAX_DISCOVERY_IN_LIST           10
#define MAX_PACKET_SIZE                 0x4000
#define MAX_DIRLEN                      40
#define MAX_FOLDER_BUFFER               4096
#define MAX_MESSAGE_BUFFER              4096

#define MAX_USERID_PASSWORD_LENGTH  30

#define MAP_UINT16_STRING_LENGTH        5

#define XML_VALUE_DELIMITER '"' /* This is correct XML delimiter */
#define MAP_MESSAGE_HANDLE_ATTR         "<msg "
#define MAP_MESSAGE_LIST_END_ATTR       "/>"


/* Global app states */
typedef enum
{
    STATE_IDLE,
    STATE_SEARCH,
    STATE_CLOSE_SEARCH,
    STATE_PAIRING,
    STATE_SELECT_ADDR,
    STATE_SELECT_MAPC_INSTANCE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_SELECT_LOCAL,
    STATE_SELECT_REMOTE,
    STATE_TRANSFER,
    STATE_DISCONNECTING,
    STATE_NOTIFICATION_REGISTRATION,
    STATE_SERVICE_SEARCH,
    STATE_PAIR_WHILE_CONNECT_ATTEMPT,
    STATE_GET_FOLDER_LISTING_PARAMETERS,
    STATE_GET_MESSAGE_LISTING_PARAMETERS,
    STATE_GET_MESSAGE_PARAMETERS,
    STATE_SET_MESSAGE_STATUS_PARAMETERS,
    STATE_PUSH_MESSAGE_PARAMETERS,
    STATE_SELECT_MAS_INSTANCE_ID,
    STATE_EOL
} MapcAppState_t;

typedef CsrUint8 mapKbmState;
#define MAP_GET_FOLDER_LISTING_MAX_LIST_COUNT          (mapKbmState) 0x00
#define MAP_GET_FOLDER_LISTING_LIST_START_OFFSET       (mapKbmState) 0x01

#define MAP_GET_MESSAGE_LISTING_FOLDER_NAME            (mapKbmState) 0x00
#define MAP_GET_MESSAGE_LISTING_MAX_LIST_COUNT         (mapKbmState) 0x01
#define MAP_GET_MESSAGE_LISTING_LIST_START_OFFSET      (mapKbmState) 0x02
#define MAP_GET_MESSAGE_LISTING_MAX_SUBJECT_LENGTH     (mapKbmState) 0x03
#define MAP_GET_MESSAGE_LISTING_PARAMETER_MASK         (mapKbmState) 0x04
#define MAP_GET_MESSAGE_LISTING_FILTER_MESSAGE_TYPE    (mapKbmState) 0x05
#define MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_BEGIN    (mapKbmState) 0x06
#define MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_END      (mapKbmState) 0x07
#define MAP_GET_MESSAGE_LISTING_FILTER_READ_STATUS     (mapKbmState) 0x08
#define MAP_GET_MESSAGE_LISTING_FILTER_RECIPIENT       (mapKbmState) 0x09
#define MAP_GET_MESSAGE_LISTING_FILTER_ORIGINATOR      (mapKbmState) 0x0A
#define MAP_GET_MESSAGE_LISTING_FILTER_PRIORITY        (mapKbmState) 0x0B

#define MAP_GET_MESSAGE_MESSAGE_HANDLE                 (mapKbmState) 0x00
#define MAP_GET_MESSAGE_ATTACHMENT                     (mapKbmState) 0x01
#define MAP_GET_MESSAGE_CHARSET                        (mapKbmState) 0x02
#define MAP_GET_MESSAGE_FRACTION_REQUEST               (mapKbmState) 0x03

#define MAP_SET_MESSAGE_STATUS_MESSAGE_HANDLE          (mapKbmState) 0x00
#define MAP_SET_MESSAGE_STATUS_STATUS_INDICATOR        (mapKbmState) 0x01
#define MAP_SET_MESSAGE_STATUS_STATUS_VALUE            (mapKbmState) 0x02

#define MAP_PUSH_MESSAGE_FOLDER_NAME                   (mapKbmState) 0x00
#define MAP_PUSH_MESSAGE_TRANSPARENT                   (mapKbmState) 0x01
#define MAP_PUSH_MESSAGE_RETRY                         (mapKbmState) 0x02
#define MAP_PUSH_MESSAGE_CHARSET                       (mapKbmState) 0x03


/* Textual representation of states */
static char *MapcAppStateString[] =
{
    "Idle",
    "Searching",
    "Closing search",
    "Pairing",
    "Select address",
    "Connecting",
    "Connected",
    "Select local file/dir",
    "Select remote file/dir",
    "Transferring",
    "Disconnecting",
    "Create directory",
    "Authenticate",
    "Searching for services"
};

static char *MapcAppParameterBitmaskString[] =
{
    " 0) CSR_BT_MAP_PARAMETERS_ALL                                   select all",
    " 1) CSR_BT_MAP_PARAMETERS_SUBJECT                               bit number 0",
    " 2) CSR_BT_MAP_PARAMETERS_DATETIME                              bit number 1",
    " 3) CSR_BT_MAP_PARAMETERS_SENDER_NAME                           bit number 2",
    " 4) CSR_BT_MAP_PARAMETERS_SENDER_ADDRESSING                     bit number 3",
    " 5) CSR_BT_MAP_PARAMETERS_RECIPIENT_NAME                        bit number 4",
    " 6) CSR_BT_MAP_PARAMETERS_RECIPIENT_ADRESSING                   bit number 5",
    " 7) CSR_BT_MAP_PARAMETERS_TYPE                                  bit number 6",
    " 8) CSR_BT_MAP_PARAMETERS_SIZE                                  bit number 7",
    " 9) CSR_BT_MAP_PARAMETERS_RECEPTION_STATUS                      bit number 8",
    " a) CSR_BT_MAP_PARAMETERS_TEXT                                  bit number 9",
    " b) CSR_BT_MAP_PARAMETERS_ATTACHMENT_SIZE                       bit number 10",
    " c) CSR_BT_MAP_PARAMETERS_PRIORITY                              bit number 11",
    " d) CSR_BT_MAP_PARAMETERS_READ                                  bit number 12",
    " e) CSR_BT_MAP_PARAMETERS_SENT                                  bit number 13",
    " f) CSR_BT_MAP_PARAMETERS_PROTECTED                             bit number 14",
    " g) CSR_BT_MAP_PARAMETERS_REPLYTO_ADDRESSING                    bit number 15",
};

static char *MapcAppFilterMessageTypeString[] =
{
    " 0) CSR_BT_MAP_TYPE_ALL                                         get all types",
    " 1) CSR_BT_MAP_TYPE_NO_SMS_GSM                                  bit number 0",
    " 2) CSR_BT_MAP_TYPE_NO_SMS_CDMA                                 bit number 1",
    " 3) CSR_BT_MAP_TYPE_NO_EMAIL                                    bit number 2",
    " 4) CSR_BT_MAP_TYPE_NO_MSS                                      bit number 3",
};

typedef struct MapcMsgInfo_t
{
    char              *subject;
    char              *datetime;
    char              *type;
    char              *read;
    char              *sent;
} MessageListElement;

/* File info container */
struct MapcInfo_t
{
    char               *name; /* Use to store msg_handle in case of message listing */
    CsrUint32          size;
    CsrBool            is_dir;
    MessageListElement  msgListElement;
    struct MapcInfo_t  *next;
};

typedef struct
{
    CsrSchedQid               mapcInstanceId;                               /* The MAPC instance Id which this instance data belongs to */
    MapcAppState_t    state;                                       /* Global app state */
    MapcAppState_t    state_old;                                   /* Old app state */
    CsrBtDeviceAddr      search_list[MAX_DISCOVERY_IN_LIST];          /* Search list */
    CsrUint8           search_idx;                                  /* Search list index */
    CsrBtDeviceAddr      address;                                     /* Remote active address */
    char              *targ_name;                                  /* Target name */
    CsrBool            targ_remote;                                 /* Is target remote? */
    CsrBool            targ_dir;                                    /* Is target a directory? */
    struct MapcInfo_t *dir_head;                                   /* Browser head */
    struct MapcInfo_t *dir_tail;                                   /* Browser tail */
    CsrUint16          dir_count;                                   /* Number of entries */
    CsrUint16          dir_index;                                   /* Current index */
    char              *dir_local;                                  /* Local directory */
    char              *dir_remote;                                 /* Remote directory */
    char              *dir_newrem;                                 /* New remote directory */
    char              *folder;                                     /* Remote folder listing data */
    CsrUint32          folder_idx;                                  /* Remote folder listing index */
    CsrUint32          folder_size;                                 /* Remote folder listing size */
    CsrFileHandle              *file;                                       /* File transfer */
    CsrUint32          file_idx;                                    /* Transfer index */
    CsrUint32          file_size;                                   /* Transfer total size */
    char              *message;                                     /* Remote message listing data */
    CsrUint32          message_idx;                                  /* Remote message listing index */
    CsrUint32          message_size;                                 /* Remote message listing size */

    CsrTime          file_time;                                   /* Transfer start time */

    CsrBool            registerNotificationStatus;                  /* Used for keeping track of the current MNS status */
    CsrBool            isMnsMaster;                                 /* Used to hold track of the MNS master to a given MSE device */
    mapKbmState       keyboardParameterState;                      /* Used for getting each parameter in a request */

    CsrBtMapcMasInstance *masInstanceList;
    CsrUint16           masInstanceListSize;

    CsrUint8           tempValue[6];
    CsrUint8           tempValueLen;
    /* For CsrBtMapcGetMessageListingReq and CsrBtMapcGetFolderListingReq */
    CsrUint16          maxListCount;                   /* maximum number of folders in the listing */
    CsrUint16          listStartOffset;                /* offset of where to start the listing */
    /* For CsrBtMapcGetMessageListingReq and CsrBtMapcPushMessageReq */
    CsrCharString          *folderName;                    /* null terminated name string of the folder from where the current message listing is to be retreived: NB: if NULL this means current folder */
    /* For CsrBtMapcGetMessageListingReq */
    CsrUint8           maxSubjectLength;               /* maximum string length allowed of the subject field */
    CsrBtMapMesParms     parameterMask;                  /* bitmask of relevant parameters for the message listing. NB: a bit value of 1 means that the parameter should be present and a value of 0 means it should be filtered out */
    CsrBtMapMesTypes     filterMessageType;              /* bitmask specifying which message types to include/exclude in the listing. NB: a bit value of 1 means that the message type should be filtered and a value of 0 means that it should be present */
    CsrCharString          *filterPeriodBegin;             /* null terminated time string */
    CsrCharString          *filterPeriodEnd;               /* null terminated time string */
    CsrBtMapReadStatus   filterReadStatus;               /* bitmask specifying if filtering should be done on behalf of the read status */
    CsrCharString          *filterRecipient;               /* null terminated recipient string */
    CsrCharString          *filterOriginator;              /* null terminated originator string */
    CsrBtMapPriority     filterPriority;                 /* bitmask specifying which priority type to include in the listing */
    /* For CsrBtMapcGetMessageReq and CsrBtMapcPushMessageReq */
    CsrBtMapCharset      charset;                        /* bitmask used to specifying trans-coding of the message */
    /* For CsrBtMapcGetMessageReq */
    CsrUtf8String          *messageHandle;                 /* null terminated message handle string */
    CsrBtMapAttachment   attachment;                     /* bitmask specifying whether to include attachment or not */
    CsrBtMapFracReq      fractionRequest;                /* bitmask which fragment of the message to get _if any_ */
    /* For CsrBtMapcSetMessageStatusReq */
    CsrBtMapStatusInd    statusIndicator;                /* specifies which status information to modify */
    CsrBtMapStatusVal    statusValue;                    /* specifies the new value of the status indication in question */
    /* For CsrBtMapcPushMessageReq */
    CsrUint32          lengthOfObject;                 /* total length of the message to send, NB: if set to zero this informative OBEX header field will not be included in the packet*/
    CsrBtMapTrans        transparent;                    /* specifies if the MSE should keep a copu of the message in the sent folder */
    CsrBtMapRetry        retry;                          /* specifies if the MSE should try to resent if first delivery to the network fails */
} MapcInstance;

/* Instance data for application */
typedef struct
{
    CsrSchedQid               qid;                                         /* This demo applications queue id (not to be confused with the mapcInstanceIds*/
    CsrUint16          event;                                       /* Message id */
    void              *msg;                                        /* Message body */

    CsrBool            mapcInstanceListReceived;
    CsrUint8           numberOfInstances;
    CsrUint8           activeInst;

    char              *cls;                                        /* Path to CLS */

    MapcInstance      *mapcInst;
    ScAppInstData_t   scInstData;
} MapcAppInst;

/* Macro to get the mapc instance data based on the activeInst parameter which is specified by the user */
#define CURRENT_INST (inst->mapcInst[inst->activeInst])

/* Macro to get the mapc instance data based on the instanceId parameter received in an event from the profile */
#define INST_DATA (&inst->mapcInst[getMapcInstanceNumber(inst, prim->instanceId)])

/* Macro to get the mapc instance Id */
#define INST_ID inst->mapcInstanceId

extern BD_ADDR_T defGlobalBdAddr;

/* Prototypes */
void handleKeyboardEvents(MapcAppInst *inst);

static CsrUint8 getMapcInstanceNumber(MapcAppInst * inst, CsrSchedQid instanceId)
{
    CsrUint8 i;
    for(i=0;i<inst->numberOfInstances;i++)
    {
        if(inst->mapcInst[i].mapcInstanceId == instanceId)
        {
            return i;
        }
    }
    return 0xFF; /* We should never get here */
}

/* Change state */
void stateChange(MapcInstance *inst, MapcAppState_t s)
{
    inst->state_old = inst->state;
    inst->state = s;
}

/* Clean instance */
void cleanInstance(MapcInstance *inst)
{
    CsrBtDeviceAddr zeroAddr;

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

    /* Setup special instance variables */
    inst->state_old = STATE_IDLE;
    stateChange(inst, STATE_IDLE);

    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");

    CsrPmemFree(inst->dir_remote);
    inst->dir_remote = CsrStrDup("/");

    CsrPmemFree(inst->dir_local);
    inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    getcwd(inst->dir_local, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(inst->dir_local, PATH_SEP);

    if(inst->file)
    {
        CsrBtFclose(inst->file);
    }
    inst->file = NULL;
    inst->file_idx = 0;
    inst->file_size = 0;
    inst->file_time = CsrTimeGet(NULL);
}

/* Show menu options */
void playMenu(MapcAppInst *inst)
{
    CsrUint8 i;
    /*system(inst->cls);*/
    printf("\nCSR plc -- Synergy BT Message Access Client demo application\n");
    printf("Hit <ESC> to quit. Hit <SPACE> to reprint menu.\n\n");

    if(inst->mapcInstanceListReceived)
    {
        printf("List of MAPC Instances:\n");
        for(i=0;i < inst->numberOfInstances; i++)
        {
            printf("\t MapcInstanceId[%02d] = 0x%04X, MNS registration = %s, %s\n",i,inst->mapcInst[i].mapcInstanceId,(inst->mapcInst[i].registerNotificationStatus ? "ON" : "OFF"), (i == inst->activeInst ? " *Selected*" : "Not selected"));
        }

        printf("\nState...............%s\n", MapcAppStateString[inst->mapcInst[inst->activeInst].state]);
        printf("Active address......%04X:%02X:%06lX\n",
               inst->mapcInst[inst->activeInst].address.nap,
               inst->mapcInst[inst->activeInst].address.uap,
               inst->mapcInst[inst->activeInst].address.lap);
        printf("Target..............'%s' (%s) %s\n",
               inst->mapcInst[inst->activeInst].targ_name,
               inst->mapcInst[inst->activeInst].targ_remote ? "remote" : "local",
                  inst->mapcInst[inst->activeInst].targ_dir ? "directory" : "file");
        printf("Local dir...........'%s'\n", inst->mapcInst[inst->activeInst].dir_local);
        printf("Remote dir..........'%s'\n", inst->mapcInst[inst->activeInst].dir_remote);
        printf("\nMenu selection:\n");

        /* Options for (almost) all states */
        switch(inst->mapcInst[inst->activeInst].state)
        {
            case STATE_IDLE:
                printf("  s) Start searching for MAP servers\n");
                printf("  p) Security Controller Configuration\n");
                printf("  c) Connect to selected address\n");
                printf("  z) Change MAPC instance\n");
                break;

            case STATE_SEARCH:
                printf("  s) Stop searching\n");
                break;

            case STATE_PAIR_WHILE_CONNECT_ATTEMPT:
                printf("  c) Cancel connect attempt\n");
                break;

            case STATE_CONNECTING:
                printf("  c) Cancel connect attempt\n");
                break;

            case STATE_CONNECTED:
                printf("  d) Disconnect\n");
                printf("  m) Change MNS registration status to %s\n",(inst->mapcInst[inst->activeInst].registerNotificationStatus ? "OFF" : "ON"));
                printf("  l) Select local file / directory\n");
                printf("  r) Select remote directory \n");
                printf("  t) Go to remote root (top) folder\n");
                printf("  u) Go up one remote folder level\n");
                printf("  j) Update Inbox \n");
                printf("  g) Get Message Listing \n");
                printf("  p) PTS: Change to non-existent folder 'sub1'\n");

                if(CsrStrLen(inst->mapcInst[inst->activeInst].targ_name) > 0)
                {
                    if(inst->mapcInst[inst->activeInst].targ_dir)
                    {
                        printf("  c) Change to target directory\n");
                    }
                    else
                    {
                        printf("  x) %s target\n",
                           inst->mapcInst[inst->activeInst].targ_remote ? "Get" : "Push");

                        if(inst->mapcInst[inst->activeInst].targ_remote)
                        {
                            printf("  e) Set Message status\n");
                        }
                    }
                }
                else
                {
                    printf("  e) Set Message status\n");
                    printf("  v) Get Message (manually enter messageHandle) \n");
                }
                printf("  z) Change MAPC instance\n");
                break;

            case STATE_SELECT_LOCAL:
            case STATE_SELECT_REMOTE:
                printf("  c) Cancel selection\n");
                if(inst->mapcInst[inst->activeInst].dir_index >= MAX_DISCOVERY_IN_LIST)
                {
                    printf("  p) Browse previous page\n");
                }
                if(inst->mapcInst[inst->activeInst].dir_index + MAX_DISCOVERY_IN_LIST < inst->mapcInst[inst->activeInst].dir_count)
                {
                    printf("  n) Browse next page\n");
                }
                break;

            case STATE_TRANSFER:
                printf("  a) Abort\n");
                break;

            case STATE_SERVICE_SEARCH:
                printf("  s) Stop service search\n");
                break;

            default:
                break;
        }
    }
    else
    {
        printf("Waiting for MAPC registration to complete \n");
    }
    printf("\n");
}

/* Clean browser info */
void browserEmpty(MapcInstance *inst)
{
    struct MapcInfo_t *next;

    next = inst->dir_head;
    while(next)
    {
        next = inst->dir_head->next;

        CsrPmemFree(inst->dir_head->name);
        CsrPmemFree(inst->dir_head);
        inst->dir_count--;

        inst->dir_head = next;
    }
    inst->dir_head = NULL;
    inst->dir_tail = NULL;
    inst->dir_count = 0;
    inst->dir_index = 0;
}

/* Insert browser info */
void browserAdd(MapcInstance *inst,
                    char *name,
                    CsrUint32 size,
                    CsrBool is_dir,
                    MessageListElement *msgListElement)
{
    struct MapcInfo_t *cur;

    /* Check that entry does not already exist */
    cur = inst->dir_head;
    while(cur != NULL)
    {
        if(CsrStrCmp(name, cur->name) == 0)
        {
            return;
        }
        cur = cur->next;
    }

    cur = CsrPmemAlloc(sizeof(struct MapcInfo_t));
    cur->size = size;
    cur->is_dir = is_dir;
    if(msgListElement != NULL)
    {
        cur->msgListElement.subject = CsrStrDup(msgListElement->subject);
        cur->msgListElement.datetime = CsrStrDup(msgListElement->datetime);
        cur->msgListElement.type = CsrStrDup(msgListElement->type);
        cur->msgListElement.read = CsrStrDup(msgListElement->read);
        cur->msgListElement.sent = CsrStrDup(msgListElement->sent);
    }

    cur->name= CsrStrDup(name);

    cur->next = NULL;

    if(inst->dir_head == NULL)
    {
        inst->dir_head = cur;
        inst->dir_tail = cur;
    }
    else
    {
        inst->dir_tail->next = cur;
        inst->dir_tail = cur;
    }
    inst->dir_count++;
}

/* Pop first browser element off */
struct MapcInfo_t *browserPeek(MapcInstance *inst, CsrUint16 i)
{
    struct MapcInfo_t *cur;
    CsrUint16 this;

    if((inst->dir_count == 0) || (i > inst->dir_count))
    {
        return NULL;
    }

    cur = inst->dir_head;
    this = 0;
    while((cur != NULL) && (this < i))
    {
        cur = cur->next;
        this++;
    }

    return cur;
}

/* Display current browser info entries */
void playBrowser(MapcAppInst *appInst, MapcInstance *inst, CsrBool isFolder)
{
    CsrUint16 i;
    CsrUint16 c;
    struct MapcInfo_t *cur;

    if((inst->dir_head == NULL) || (inst->dir_index >= inst->dir_count))
    {
        printf("\nNo more objects to browse!\n");
        return;
    }

    playMenu(appInst);
    i = inst->dir_index;
    c = 0;
    while((c < MAX_DISCOVERY_IN_LIST) && ((cur = browserPeek(inst, i)) != NULL))
    {
        if(isFolder)
        {
            printf("Entry %i, size %10d, %4s name '%s'\n",
                   c,
                   cur->size,
                   cur->is_dir ? "dir" : "file",
                   cur->name);
        }
        else
        {
            printf("Entry %i, size %10d, %4s handle '%s', subject '%s', datetime '%s' type '%s', read '%s', sent '%s'\n",
                   c,
                   cur->size,
                   cur->is_dir ? "dir" : "file",
                   cur->name,
                   cur->msgListElement.subject,
                   cur->msgListElement.datetime,
                   cur->msgListElement.type,
                   cur->msgListElement.read,
                   cur->msgListElement.sent);
        }
        i++;
        c++;
    }

    printf("\nPlease select entry: ");
}

/* Fetch an XML attribute value */
void getAttributeValue(char *element, CsrUint16 length, char **value)
{
    CsrUint16 i;

    i = 0;
    while((i < length) && (element[i] != XML_VALUE_DELIMITER ))
    {
        i++;
    }
    if(i < length)
    {
        i++;
        *value = element + i;
        while((i < length) && (element[i] != XML_VALUE_DELIMITER ))
        {
            i++;
        }
        element[i] = '\0';
    }
    else
    {
        *value = NULL;
    }
}

/* Get transfer speed */
double transferGetSpeed(MapcInstance *inst)
{
    double speed;
    double time;

    time = ((double)CsrTimeSub(CsrTimeGet(NULL), inst->file_time));
    if(time > 1)
    {
        speed = (((double)inst->file_idx) / (time / 1000000.0)) / 1024;
    }
    else
    {
        speed = 0.0;
    }

    return speed;
}

/* Show transfer statistics */
void transferStats(MapcInstance *inst)
{
    printf("Transferred %8d of %8d bytes, speed %4.3f kB/s\n",
           inst->file_idx,
           inst->file_size,
           transferGetSpeed(inst));
}

/* End transfer (cleanup, etc.) */
void transferEnd(MapcAppInst *appInst, MapcInstance *inst, CsrBool stats, CsrBool removeFile)
{
    stateChange(inst, STATE_CONNECTED);
    playMenu(appInst);

    if(stats)
    {
        printf("%s operation of object: '%s' completed.\n\n",
               (inst->targ_remote ? "GET" : "PUT"),
               inst->targ_name);
        printf("Total transfer size : %d bytes\n", inst->file_size);
        printf("Transfer time       : %f s\n", CsrTimeSub(CsrTimeGet(NULL), inst->file_time) / 1000000.0);
        printf("Transfer speed      : %f kB/s\n", transferGetSpeed(inst));
    }

    /* Cleanup */
    if(inst->file != NULL)
    {
        CsrBtFclose(inst->file);
        if(removeFile)
        {
            int    result;
            result  =  unlink((char*)inst->targ_name);
            if(result == 0)
            {
                /* Success */
            }
            else
            {
                printf("Failed to delete file (%s)!\n", inst->targ_name);
            }
        }
    }
    inst->file = NULL;
    inst->file_idx = 0;
    inst->file_size = 0;
    inst->file_time = CsrTimeGet(NULL);
}

/* Start file transfer */
void transferStart(MapcAppInst *appInst, MapcInstance *inst)
{
    char *mode;
    CsrBtStat_t info;
    CsrCharString *target_name;

    if(CsrStrLen(inst->targ_name) == 0)
    {
        stateChange(inst, STATE_CONNECTED);
        return;
    }

    /* Clean stat info */
    CsrMemSet(&info, 0, sizeof(CsrBtStat_t));

    /* Check get */
    if(inst->targ_remote)
    {
        /* Open for writing (get), file must not exist */
        mode = "wb";
        inst->file_size = 0;
    }
    else
    {
        /* Open for reading (put), file must exist */
        mode = "rb";
        if(CsrBtStat(inst->targ_name, &info) != 0)
        {
            stateChange(inst, STATE_CONNECTED);
            playMenu(appInst);
            printf("File does not exist!\n");
            CsrPmemFree(inst->folderName);
            inst->folderName = NULL;
            return;
        }
        inst->file_size = info.st_size;
    }

    /* Open */
    inst->file = CsrBtFopen(inst->targ_name, mode);
    inst->file_idx = 0;
    if(inst->file == NULL)
    {
        printf("Could not open file!\n");
        return;
    }

    /* Ok, start transfer */
    target_name = (CsrCharString*) CsrStrDup((const char*)inst->targ_name);
    stateChange(inst, STATE_TRANSFER);
    inst->file_time = CsrTimeGet(NULL);
    printf("Started transfer of '%s'...\n",
           inst->targ_name);

    /* Remote file, get it */
    if(inst->targ_remote)
    {
        CsrBtMapcGetMessageReqSend(INST_ID, inst->messageHandle, inst->attachment, inst->charset, inst->fractionRequest, FALSE);
        inst->messageHandle = NULL;
    }
    else
    {
        CsrBtMapcPushMessageReqSend(INST_ID, inst->folderName, info.st_size, inst->transparent, inst->retry, inst->charset);
        inst->folderName = NULL;
    }

    playMenu(appInst);
}

/* Change directory. Does not consume the parameters */
void selectDirectory(MapcAppInst *appInst, MapcInstance *inst, char *dir, CsrBool remote)
{

    /* Local */
    if(!remote)
    {
        chdir(dir);
        CsrPmemFree(inst->dir_local);
        inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
        getcwd(inst->dir_local, CSR_BT_MAX_PATH_LENGTH);
        CsrStrCat(inst->dir_local, PATH_SEP);
        stateChange(inst, STATE_CONNECTED);
    /* Set new target */
    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");
    playMenu(appInst);
    return;
    }

    /* We are certain to enter transfer state now */
    stateChange(inst, STATE_TRANSFER);

    /* Ok, it's remote. Check if's one of the two specialities */
    if(CsrStrLen(dir) == 1)
    {
        if(!CsrStrNCmp(dir, "/", 2))
        {
            /* Track remote directory */
            inst->dir_newrem = CsrStrDup("/");

            printf("Changing remote directory to 'root'...\n");
            CsrBtMapcSetRootFolderReqSend(INST_ID);
        /* Set new target */
        CsrPmemFree(inst->targ_name);
        inst->targ_name = CsrStrDup("");

            return;
        }
    }
    else if(CsrStrLen(dir) == 2)
    {
        if(!CsrStrNCmp(dir, "..", 2))
        {
            int i;
            /* Backtrack remote directory */
            i = CsrStrLen(inst->dir_remote);

            if((i > 0) && (inst->dir_remote[i-1] == '/'))
            {
                i--;
            }

            while((i > 0) &&
                  (inst->dir_remote[i-1] != '\0') &&
                  (inst->dir_remote[i-1] != '/'))
            {
                i--;
            }

            if((i > 0) && (inst->dir_remote[i-1] == '/'))
            {
                i--;
            }

            inst->dir_newrem = CsrPmemAlloc(i + 1);
            strncpy(inst->dir_newrem, inst->dir_remote, i);
            inst->dir_newrem[i] = '\0';

            printf("Changing remote directory to 'parent'...\n");
            CsrBtMapcSetBackFolderReqSend(INST_ID);
        /* Set new target */
        CsrPmemFree(inst->targ_name);
        inst->targ_name = CsrStrDup("");
            return;
        }
    }

    /* Track remote directory */
    inst->dir_newrem = CsrPmemAlloc(CsrStrLen(inst->dir_remote) + 1 +
                               CsrStrLen(dir) + 1);
    CsrStrCpy(inst->dir_newrem, inst->dir_remote);
    if(inst->dir_newrem[CsrStrLen(inst->dir_newrem)-1] != '/')
    {
        CsrStrCat(inst->dir_newrem, "/");
    }
    CsrStrCat(inst->dir_newrem, dir);

    printf("Changing remote directory...\n");
    CsrBtMapcSetFolderReqSend(INST_ID, CsrStrDup((const char*)dir));
    /* Set new target */
    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");
}


/* Remote directory entry found, parse entries into browser-info */
void folderParseEntry(MapcInstance *inst,
                      char *entry,
                      CsrUint16 length,
                      CsrBool is_dir)
{
    CsrUint32 i;
    CsrUint32 isize;
    char *name;
    char *size;

    i = 0;
    name = NULL;
    size = NULL;
    while((i < length) && CsrBtIsSpace(entry[i]))
    {
        i++;
    }
    entry[i] = '\0';
    while(i < length)
    {
        /* Find name entry */
        if((i + 4) < length && !CsrStrNCmp(entry + i, "name", 4))
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &name);
        }
        /* Find size entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "size", 4))
        {
            /* found "size" attribute */
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &size);
        }

        /* Continue scanning */
        i++;
    }

    /* Make sure size is present */
    if(size == NULL)
    {
        size = "0";
    }

    /* Found both, add to browser table */
    if(name != NULL)
    {
        isize = atoi(size);
        browserAdd(inst, name, isize, is_dir, NULL);
    }
}

/* Parse current collection buffer */
void folderParseBuffer(MapcInstance *inst)
{
    CsrUint32 index;
    CsrUint32 start;
    CsrBool found;
    CsrBool is_dir;

    /* Dummy checking */
    if((inst->folder == NULL) ||
       (inst->folder_size == 0))
    {
        return;
    }

    index = 0;
    start = 0;
    found = FALSE;
    is_dir = FALSE;
    while(index < inst->folder_size)
    {
        if(!found)
        {
            start = index;

            /* Scan for file element */
            if(((index + 5) < inst->folder_size) &&
               (CsrMemCmp(inst->folder + index, "<file", 5) == 0))
            {
                found = TRUE;
                is_dir = FALSE;
                index += 5;
            }
            /* Scan for folder element */
            else if(((index + 8) < inst->folder_size) &&
                    (CsrMemCmp(inst->folder + index, "<folder ", 8) == 0))
            {
                found = TRUE;
                is_dir = TRUE;
                index += 8;
            }
            /* Scan for special parent-folder element */
            else if(((index + 14) < inst->folder_size) &&
                    (CsrMemCmp(inst->folder + index, "<parent-folder", 14) == 0))
            {
                /* Simply add the entry right away and continue scan */
                browserAdd(inst, "..", 0, TRUE, NULL);
                index += 15;
            }
        }
        /* Search for end of element */
        else
        {
            if(((index + 2) < inst->folder_size) &&
               (CsrMemCmp(inst->folder + index, "/>", 2) == 0))
            {
                char *raw;
                char *ascii;

                /* Calculate end */
                raw = inst->folder + start;
                index += 2;

                /* Parse entry */
                ascii = CsrPmemAlloc(index - start + 1);
                CsrStrNCpy(ascii, raw, index - start);
                ascii[index - start] = '\0';
                folderParseEntry(inst,
                                 ascii,
                                 (CsrUint16)(index - start),
                                 is_dir);
                CsrPmemFree(ascii);

                /* Cull buffer */
                raw = CsrPmemAlloc(inst->folder_size - index);
                CsrMemCpy((char*)raw,
                       (char*)((inst->folder) + index),
                       inst->folder_size - index);
                CsrPmemFree(inst->folder);
                inst->folder = raw;
                inst->folder_size -= index;
                inst->folder_idx -= index;

                /* Since buffer was culled, start from beginning */
                found = FALSE;
                index = 0;
                continue;
            }
        }

        /* Continue scanning buffer */
        index++;
    }
}

/* Receive remote folder listing */
void folderReceive(MapcInstance *inst,
                   void *body,
                   CsrUint16 length,
                   CsrUint32 total)
{
    /* Allocate new folder chunk */
    if(total != 0)
    {
        if(inst->folder != NULL)
        {
            printf("Tried to alloc new folder-list, but old one existed!\n");
            CsrBtMapcDisconnectReqSend(INST_ID, TRUE);
#ifdef ENABLE_SHUTDOWN
            CsrSchedStop();
#else
            exit(1);
#endif
            return;
        }

        if(total > MAX_FOLDER_BUFFER)
        {
            inst->folder_size = MAX_FOLDER_BUFFER;
        }
        else
        {
            inst->folder_size = total;
        }

        inst->folder = CsrPmemAlloc(inst->folder_size);
        inst->folder_idx = 0;
        CsrMemSet(inst->folder, 0, inst->folder_size);
    }

    /* Copy received data into buffer */
    if((length > 0) && (body != NULL))
    {
        if((inst->folder_idx + length) > inst->folder_size)
        {
            void *tmp;

            /* Allocate more space for the folder data */
            inst->folder_size += length;
            tmp = CsrPmemAlloc(inst->folder_size);

            /* Copy already received data into buffer */
            CsrMemCpy(tmp, inst->folder, inst->folder_idx);
            CsrPmemFree(inst->folder);
            inst->folder = tmp;
        }

        /* Copy received data into buffer */
        CsrMemCpy((inst->folder + inst->folder_idx),
               body,
               length);
        inst->folder_idx += length;
    }

    /* Parse entries to save memory in the long run */
    folderParseBuffer(inst);
}

/* Last part of remote folder listing received */
void folderFinalize(MapcAppInst *appInst, MapcInstance *inst)
{
    /* Make sure collection buffer is empty */
    folderParseBuffer(inst);

    /* Add special root folder */
    browserAdd(inst, "/", 0, TRUE, NULL);

    /* Display what we've just parsed */
    stateChange(inst, STATE_SELECT_REMOTE);
    playBrowser(appInst, inst, TRUE);

    /* Clear the collection buffer */
    CsrPmemFree(inst->folder);
    inst->folder = NULL;
    inst->folder_idx = 0;
    inst->folder_size = 0;

}

/* Message listing entry found, parse entries into browser-info */
void messageParseEntry(MapcInstance *inst,
                              char *entry,
                              CsrUint16 length)
{
    CsrUint32 i;
    CsrUint32 isize;
    MessageListElement msgListEntry;
    char *size;
    char *name;

    i = 0;
    size = NULL;
    name = NULL;

    msgListEntry.subject    = NULL;
    msgListEntry.datetime   = NULL;
    msgListEntry.type       = NULL;
    msgListEntry.read       = NULL;
    msgListEntry.sent       = NULL;

    while((i < length) && CsrBtIsSpace(entry[i]))
    {
        i++;
    }
    entry[i] = '\0';
    while(i < length)
    {
        /* Find Message Handle entry */
        if((i + 6) < length && !CsrStrNCmp(entry + i, "handle", 6))
        {
            i+=6;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &name);
        }
        /* Find subject entry */
        else if((i + 7) < length && !CsrStrNCmp(entry + i, "subject", 7))
        {
            i+=7;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &(msgListEntry.subject));
        }
        /* Find datetime entry */
        else if((i + 8) < length && !CsrStrNCmp(entry + i, "datetime", 8))
        {
            i+=8;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &(msgListEntry.datetime));
        }
        /* Find type entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "type", 4))
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &(msgListEntry.type));
        }
        /* Find size entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "size", 4) &&
                (entry[i- 1] != '_')) /* To make sure not to confuse with attachment_size attribute */
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &size);
        }
        /* Find read entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "read", 4))
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &(msgListEntry.read));
        }
        /* Find sent entry */
        else if((i + 4) < length && !CsrStrNCmp(entry + i, "sent", 4))
        {
            i+=4;
            getAttributeValue(entry + i, (CsrUint16)(length - i), &(msgListEntry.sent));
        }

        /* Continue scanning */
        i++;
    }

    /* Make sure size is present */
    if(size == NULL)
    {
        size = "0";
    }

    /* Found both, add to browser table */
    if(name != NULL)
    {
        isize = atoi(size);
        browserAdd(inst, name, isize, FALSE, &msgListEntry);
    }
}

/* Parse current message collection buffer */
void messageParseBuffer(MapcInstance *inst)
{
    CsrUint32 index;
    CsrUint32 start;
    CsrBool found;

    /* Dummy checking */
    if((inst->message == NULL) ||
       (inst->message_size == 0))
    {
        return;
    }

    index = 0;
    start = 0;
    found = FALSE;
    while(index < inst->message_size)
    {
        if(!found)
        {
            start = index;

            /* Scan for message element */
            if(((index + (sizeof(MAP_MESSAGE_HANDLE_ATTR)-1)) < inst->message_size) &&
               (CsrMemCmp(inst->message + index, MAP_MESSAGE_HANDLE_ATTR,
                        (sizeof(MAP_MESSAGE_HANDLE_ATTR)-1)) == 0))
            {
                found = TRUE;
                index += (sizeof(MAP_MESSAGE_HANDLE_ATTR)-1);
            }
        }
        /* Search for end of element */
        else
        {
            if(((index + (sizeof(MAP_MESSAGE_LIST_END_ATTR)-1)) < inst->message_size) &&
               (CsrMemCmp(inst->message + index, MAP_MESSAGE_LIST_END_ATTR,
                        (sizeof(MAP_MESSAGE_LIST_END_ATTR)-1)) == 0))
            {
                char *raw;
                char *ascii;

                /* Calculate end */
                raw = inst->message + start;
                index += (sizeof(MAP_MESSAGE_LIST_END_ATTR)-1);

                /* Parse entry */
                ascii = CsrPmemAlloc(index - start + 1);
                CsrStrNCpy(ascii, raw, index - start);
                ascii[index - start] = '\0';
                messageParseEntry(inst,
                                  ascii,
                                  (CsrUint16)(index - start));
                CsrPmemFree(ascii);

                /* Cull buffer */
                raw = CsrPmemAlloc(inst->message_size - index);
                CsrMemCpy(raw,
                       (inst->message + index),
                       inst->message_size - index);
                CsrPmemFree(inst->message);
                inst->message= raw;
                inst->message_size -= index;
                inst->message_idx -= index;

                /* Since buffer was culled, start from beginning */
                found = FALSE;
                index = 0;
                continue;
            }
        }

        /* Continue scanning buffer */
        index++;
    }
}


/* Receive remote message listing */
void messageReceive(MapcInstance *inst,
                   void *body,
                   CsrUint16 length,
                   CsrUint32 total)
{
    /* Allocate new message chunk */
    if(total != 0)
    {
        if(inst->message!= NULL)
        {
            printf("Tried to alloc new message-list, but old one existed!\n");
            CsrBtMapcDisconnectReqSend(INST_ID, TRUE);
#ifdef ENABLE_SHUTDOWN
            CsrSchedStop();
#else
            exit(1);
#endif
            return;
        }

        if(total > MAX_MESSAGE_BUFFER)
        {
            inst->message_size= MAX_MESSAGE_BUFFER;
        }
        else
        {
            inst->message_size= total;
        }

        inst->message= CsrPmemAlloc(inst->message_size);
        inst->message_idx= 0;
        CsrMemSet(inst->message, 0, inst->message_size);
    }

    /* Copy received data into buffer */
    if((length > 0) && (body != NULL))
    {
        if((inst->message_idx+ length) > inst->message_size)
        {
            void *tmp;

            /* Allocate more space for the folder data */
            inst->message_size+= length;
            tmp = CsrPmemAlloc(inst->message_size);

            /* Copy already received data into buffer */
            CsrMemCpy(tmp, inst->message, inst->message_idx);
            CsrPmemFree(inst->message);
            inst->message= tmp;
        }

        /* Copy received data into buffer */
        CsrMemCpy((inst->message+ inst->message_idx),
               body,
               length);
        inst->message_idx+= length;
    }

    /* Parse entries to save memory in the long run */
    messageParseBuffer(inst);
}

/* Last part of remote message listing received */
void messageFinalize(MapcAppInst *appInst, MapcInstance *inst)
{
    /* Make sure collection buffer is empty */
    messageParseBuffer(inst);

    /* Display what we've just parsed */
    stateChange(inst, STATE_SELECT_REMOTE);
    playMenu(appInst);

    playBrowser(appInst, inst, FALSE);

    /* Clear the collection buffer */
    CsrPmemFree(inst->message);
    inst->message = NULL;
    inst->message_idx = 0;
    inst->message_size = 0;
}

/* Start local file selection */
void selectLocal(MapcAppInst *appInst, MapcInstance *inst)
{
    DIR           *dir;
    struct dirent *entry;
    CsrBtStat_t    info;
    CsrBool        is_dir;

    stateChange(inst, STATE_SELECT_LOCAL);
    browserEmpty(inst);

    /* Read directory */
    dir = CsrBtOpenDir(inst->dir_local);
    if(dir)
    {
        while((entry = CsrBtReadDir(dir)) != NULL)
        {
            CsrMemSet(&info, 0, sizeof(CsrBtStat_t));
            if(CsrBtStat(entry->d_name, &info) == 0)
            {
                if((info.st_mode & S_IFDIR) == S_IFDIR)
                {
                    is_dir = TRUE;
                }
                else
                {
                    is_dir = FALSE;
                }
                browserAdd(inst, entry->d_name, info.st_size, is_dir, NULL);
            }
        }
        CsrBtCloseDir(dir);

        /* Display what we've just read */
        playBrowser(appInst, inst, TRUE);
    }
    else
    {
        printf("Current local directory (%s) is empty\n",
               inst->dir_local);
        stateChange(inst, STATE_CONNECTED);
    }
}

/* Start remote folder selection */
void selectRemote(MapcInstance *inst)
{
    stateChange(inst, STATE_TRANSFER);
    browserEmpty(inst);

    /* Request folder information */
    CsrBtMapcGetFolderListingReqSend(INST_ID, inst->maxListCount, inst->listStartOffset, FALSE);
    printf("Transferring folder information...\n");
}

/* Start remote message selection */
void selectRemoteMessage(MapcInstance *inst)
{
    stateChange(inst, STATE_TRANSFER);
    browserEmpty(inst);

    /* Request folder information */
    CsrBtMapcGetMessageListingReqSend(INST_ID,
                                        inst->folderName,
                                        inst->maxListCount,
                                        inst->listStartOffset,
                                        inst->maxSubjectLength,
                                        inst->parameterMask,
                                        inst->filterMessageType,
                                        inst->filterPeriodBegin,
                                        inst->filterPeriodEnd,
                                        inst->filterReadStatus,
                                        inst->filterRecipient,
                                        inst->filterOriginator,
                                        inst->filterPriority,
                                        FALSE);
    printf("Transferring message information...\n");
    inst->folderName = NULL;
}


/* Start search */
void startSearch(MapcAppInst *appInst, MapcInstance *inst)
{
    inst->search_idx = 0;
    stateChange(inst, STATE_SEARCH);

    CsrBtSdSearchReqSend(appInst->qid,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0, /* Filter length */
                  NULL); /* Filter */

    playMenu(appInst);
}

/* Start connecting */
void startConnecting(MapcAppInst *appInst, MapcInstance *inst)
{
    if((inst->address.lap == 0) &&
       (inst->address.nap == 0) &&
       (inst->address.uap == 0))
    {
        printf("No device selected for connection\n");
        return;
    }
    else
    {
        CsrBtDeviceAddr zeroAddr;
        CsrBtBdAddrZero(&zeroAddr);
        
        if (CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
        {/* Store the address as the global address */
            defGlobalBdAddr.lap = inst->address.lap;
            defGlobalBdAddr.nap = inst->address.nap;
            defGlobalBdAddr.uap = inst->address.uap;
        }
    }

    stateChange(inst, STATE_CONNECTING);
    CsrBtMapcConnectReqSend(INST_ID,
                            appInst->qid,
                            MAX_PACKET_SIZE,
                            inst->address,
                            0, /*length*/
                            0, /*count*/
                            0, /*windowSize*/
                            TRUE); /*enableSrm*/
    playMenu(appInst);
}

/* List MAPC instances */
void listMapcInstances(MapcAppInst *inst)
{
    if(inst->mapcInstanceListReceived)
    {
        CsrUint8 i;
        stateChange(&CURRENT_INST, STATE_SELECT_MAPC_INSTANCE);

        printf("\nList of MAPC instances:\n\n");
        for(i=0;i < inst->numberOfInstances; i++)
        {
            printf("\t %d) MapcInstanceId[%02d] = 0x%04X, current status = %s\n",i,i,inst->mapcInst[i].mapcInstanceId, (i == inst->activeInst ? "Selected" : "Not selected"));
        }
    }
    else
    {
        printf("No MAPC instances available!\n");
    }
}

/* Check for keypresses (timed event) */
void keyboardTimer(CsrUint16 m, void *data)
{
    MapcAppInst *inst;
    inst = (MapcAppInst*)data;

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

/* Handle SD (service discovery) messages */
void handleSdEvents(MapcAppInst *inst)
{
    CsrBtSdPrim type;

    type = *((CsrBtSdPrim*)inst->msg);
    switch(type)
    {
        case CSR_BT_SD_SEARCH_RESULT_IND:
        {
            CsrBtSdSearchResultInd *prim;
            prim = (CsrBtSdSearchResultInd*)inst->msg;
            /* Read device services */
            if(CURRENT_INST.state == STATE_SEARCH)
            {
                CsrUint16 nameLen;
                CsrUint8 *name;
                printf("\n%u: ", CURRENT_INST.search_idx);
                nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                if (nameLen > 0)
                {
                    printf("%s\n   ", name);
                    CsrPmemFree(name);
                }
                printf("%04X:%02X:%06X\n   ",
                    prim->deviceAddr.nap,
                    prim->deviceAddr.uap,
                    prim->deviceAddr.lap);
                printf("%s\n",CodDecodeDeviceClass(prim->deviceClass));

                CURRENT_INST.search_list[CURRENT_INST.search_idx] = prim->deviceAddr;
                CURRENT_INST.search_idx++;
            }

            if(CURRENT_INST.search_idx > MAX_DISCOVERY_IN_LIST-1)
            {
                CsrBtSdCancelSearchReqSend(inst->qid);
                stateChange(&CURRENT_INST, STATE_CLOSE_SEARCH);
            }
            break;
        }

        case CSR_BT_SD_CLOSE_SEARCH_IND:
            if(CURRENT_INST.state == STATE_CLOSE_SEARCH)
            {
                if(CURRENT_INST.search_idx > 0)
                {
                    stateChange(&CURRENT_INST, STATE_SELECT_ADDR);
                    printf("\nSelect a device from the list: ");
                }
                else
                {
                    stateChange(&CURRENT_INST, STATE_IDLE);
                    playMenu(inst);
                }
            }
            break;

        case CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM:
        {
            CsrBtSdReadAvailableServicesCfm *prim;
            prim = (CsrBtSdReadAvailableServicesCfm*)inst->msg;
            if(CURRENT_INST.state == STATE_SERVICE_SEARCH)
            {

                if(CURRENT_INST.state == STATE_SERVICE_SEARCH)
                {
                    CsrUint16 nameLen;
                    CsrUint8 *name;
                    stateChange(&CURRENT_INST, STATE_IDLE);
                    playMenu(inst);

                    nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                    printf("Service search for address %04X:%02X:%06X, name %s completed:\n",
                           prim->deviceAddr.nap,
                           prim->deviceAddr.uap,
                           prim->deviceAddr.lap,
                           name);
                    CsrPmemFree(name);

                    if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID))
                    {
                        printf("MAP service available\n");
                    }
                    else if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, CSR_BT_OBEX_MESSAGE_ACCESS_PROFILE_UUID))
                    {
                        printf("MAS service available\n");
                    }
                    else if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, 0xBEEF))
                    {
                        printf("MAS service in PTS available NB: They use wrong 0xBEEF UUID!!!!\n");
                    }
                    else
                    {
                        printf("MAP not available\n");
                    }
                }
            }
            break;
        }

        case CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM:
            {
                stateChange(&CURRENT_INST, STATE_IDLE);
                playMenu(inst);
                printf("Read available services cancelled\n");
            }
            break;

        default:
            {
                printf("Unhandled SD primitive 0x%04X\n", type);
                break;
            }
    }

    CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg);
}

static void dumpReceivedPayload(CsrUint8 *buf, CsrUint16 len)
{
    CsrUint16 i;
    for(i=0;i<len;i++)
    {
        printf("%c",buf[i]);
    }
}

static void clearMnsRegistrationForOtherMasInstancesToThisDevice(MapcAppInst *inst, MapcInstance *curInst)
{
    CsrUint8 i;
    for (i=0;i<inst->numberOfInstances;i++)
    {
        if(&inst->mapcInst[i] != curInst)
        { /* Don't include own instance */
            if(CsrBtBdAddrEq(&curInst->address, &inst->mapcInst[i].address))
            {
                inst->mapcInst[i].registerNotificationStatus = FALSE;
            }
        }
    }
}


/* Handle the MAPC messages */
void handleMapcEvents(MapcAppInst *inst)
{
    CsrBtMapcPrim type;

    type = *((CsrBtMapcPrim*)inst->msg);
    switch(type)
    {
        case CSR_BT_MAPC_GET_INSTANCE_IDS_CFM:
        {
            CsrBtMapcGetInstanceIdsCfm * prim = inst->msg;
            CsrUint8 i;

            inst->mapcInstanceListReceived = TRUE;
            inst->numberOfInstances = prim->instanceIdsListSize;
            inst->activeInst = 0;

            inst->mapcInst = CsrPmemAlloc(sizeof(MapcInstance) * prim->instanceIdsListSize);

            for( i=0; i< prim->instanceIdsListSize;i++)
            {
                CsrMemSet(&inst->mapcInst[i], 0, sizeof(MapcInstance));
                cleanInstance(&inst->mapcInst[i]);
                inst->mapcInst[i].mapcInstanceId = prim->instanceIdsList[i];
            }
            playMenu(inst);
            break;
        }
        case CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND :
            {
                int i;
                CsrBtMapcSelectMasInstanceInd    *prim;

                prim        = (CsrBtMapcSelectMasInstanceInd *) inst->msg;

                system(inst->cls);

                stateChange(INST_DATA, STATE_SELECT_MAS_INSTANCE_ID);
                for (i = 0; i < prim->masInstanceListSize; i++)
                {
                    printf("Entry: %d, Service name: %s, MASInstanceId: %d, supportedMessages: 0x%02X\n", i, prim->masInstanceList[i].serviceName, prim->masInstanceList[i].masInstanceId, prim->masInstanceList[i].supportedMessages);
                }
                printf("\nPress Entry number to select MAS instance to connect, or C to cancel\n\n");
                INST_DATA->masInstanceList = prim->masInstanceList;
                INST_DATA->masInstanceListSize = prim->masInstanceListSize;
                prim->masInstanceList = NULL; /* Prevent this pointer from being freed it will be done when the user responds */
                prim->masInstanceListSize = 0;
                break;
            }

        case CSR_BT_MAPC_CONNECT_CFM:
        {
            CsrBtMapcConnectCfm* prim;

            prim = (CsrBtMapcConnectCfm*)inst->msg;
            if((prim->resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE) && (prim->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX))
            {
                stateChange(INST_DATA, STATE_CONNECTED);
                playMenu(inst);
                printf("Connection successful!\n");
                printf("To device with, Service name: %s, MASInstanceId: %d, supportedMessages: 0x%02X\n", prim->serviceName, prim->masInstanceId, prim->supportedMessages);
            }
            else
            {
                stateChange(INST_DATA, STATE_IDLE);
                playMenu(inst);
                printf("Could not connect, resultCode = 0x%04x, resultSupplier = 0x%04X\n",
                       prim->resultCode, prim->resultSupplier);
            }
            break;
        }
        case CSR_BT_MAPC_DISCONNECT_IND:
            {
                CsrBtMapcDisconnectInd *prim = inst->msg;
                if(INST_DATA->isMnsMaster)
                {
                    clearMnsRegistrationForOtherMasInstancesToThisDevice(inst,INST_DATA);
                }
                cleanInstance(INST_DATA);
                playMenu(inst);
                printf("Disconnected!\n");
                break;
            }

        case CSR_BT_MAPC_SET_FOLDER_CFM:
            /* Fallthrough */
        case CSR_BT_MAPC_SET_BACK_FOLDER_CFM:
            /* Fallthrough */
        case CSR_BT_MAPC_SET_ROOT_FOLDER_CFM:
            {
                CsrBtMapcSetFolderCfm *prim;
                prim = (CsrBtMapcSetFolderCfm*)inst->msg;

                stateChange(INST_DATA, STATE_CONNECTED);
                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Set new directory which we tracked before sending the req */
                    if(INST_DATA->dir_newrem != NULL)
                    {
                        CsrPmemFree(INST_DATA->dir_remote);
                        INST_DATA->dir_remote = INST_DATA->dir_newrem;
                        INST_DATA->dir_newrem = NULL;
                    }

                    playMenu(inst);
                    printf("Successfully changed directory\n");
                }
                else
                {
                    CsrPmemFree(INST_DATA->dir_newrem);
                    playMenu(inst);
                    printf("Set folder failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_GET_FOLDER_LISTING_IND:
            {
                CsrBtMapcGetFolderListingInd *prim = inst->msg;

                folderReceive(INST_DATA,
                              prim->payload + prim->bodyOffset,
                              prim->bodyLength,
                              0);

                printf("FolderListingInd: fullFolderListingSize=%d, folderListingRetrieved=%d\n",prim->fullFolderListingSize, prim->folderListingRetrieved);
                dumpReceivedPayload(prim->payload + prim->bodyOffset,prim->bodyLength);
                /* Not final, so ask for more */
                CsrBtMapcGetFolderListingResSend(prim->instanceId, FALSE);

                break;
            }

        case CSR_BT_MAPC_GET_FOLDER_LISTING_CFM:
            {
                CsrBtMapcGetFolderListingCfm *prim;

                prim = (CsrBtMapcGetFolderListingCfm*)inst->msg;
                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    folderReceive(INST_DATA,
                                  prim->payload + prim->bodyOffset,
                                  prim->bodyLength,
                                  0);

                    stateChange(INST_DATA, STATE_CONNECTED);
                    folderFinalize(inst, INST_DATA);

                    printf("FolderListingCfm: fullFolderListingSize=%d\n",prim->fullFolderListingSize);
                    dumpReceivedPayload(prim->payload + prim->bodyOffset,prim->bodyLength);
                }
                else
                {
                    stateChange(INST_DATA, STATE_CONNECTED);
                    playMenu(inst);
                    printf("Get folder list failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_GET_MESSAGE_LISTING_IND:
            {
                CsrBtMapcGetMessageListingInd *prim = inst->msg;
                messageReceive(INST_DATA,
                              prim->payload + prim->bodyOffset,
                              prim->bodyLength,
                              prim->fullMessageListingSize);
                printf("MessageListingInd: fullMessageListingSize=%d, messageListingRetrieved=%d, mseTime=%s\n\n",prim->fullMessageListingSize, prim->messageListingRetrieved, prim->mseTime);
                dumpReceivedPayload(prim->payload + prim->bodyOffset,prim->bodyLength);

                /* Not final, so ask for more */
                CsrBtMapcGetMessageListingResSend(prim->instanceId, FALSE);

                break;
            }

        case CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM:
            {
                CsrBtMapcGetMessageListingCfm *prim;

                prim = (CsrBtMapcGetMessageListingCfm*)inst->msg;
                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    messageReceive(INST_DATA,
                                  prim->payload + prim->bodyOffset,
                                  prim->bodyLength,
                                  prim->fullMessageListingSize);
                    printf("MessageListingCfm: fullMessageListingSize=%d, newMessages=0x%02X\n, mseTime=%s\n",prim->fullMessageListingSize, prim->newMessages, prim->mseTime);
                    dumpReceivedPayload(prim->payload + prim->bodyOffset,prim->bodyLength);

                    stateChange(INST_DATA, STATE_CONNECTED);

                    messageFinalize(inst, INST_DATA);

                }
                else
                {
                    stateChange(INST_DATA, STATE_CONNECTED);
                    playMenu(inst);
                    printf("Get message list failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }


        case CSR_BT_MAPC_GET_MESSAGE_IND:
            {
                CsrBtMapcGetMessageInd *prim;

                prim = (CsrBtMapcGetMessageInd*)inst->msg;

                if(INST_DATA->file)
                {
                    /* Data to write */
                    if(prim->bodyOffset > 0)
                    {
                        CsrBtFwrite((prim->payload + prim->bodyOffset),
                               1,
                               prim->bodyLength,
                               INST_DATA->file);
                    }

                    /* Show progress */
                    INST_DATA->file_idx = prim->bodyLength;
/* Removed because we don't know the actual length of the message
                    INST_DATA->file_size = prim->lengthOfObject;
                    if(INST_DATA->file_size != 0)
                    {
                        transferStats(INST_DATA);
                    }
*/
                    /* Send data request */
                    CsrBtMapcGetMessageResSend(prim->instanceId, FALSE);
                }
                break;
            }

        case CSR_BT_MAPC_GET_MESSAGE_CFM:
            {
                CsrBtMapcGetMessageCfm *prim;

                prim = (CsrBtMapcGetMessageCfm*)inst->msg;

                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Data to write */
                    if(prim->bodyOffset > 0)
                    {
                        CsrBtFwrite((prim->payload + prim->bodyOffset),
                               1,
                               prim->bodyLength,
                               INST_DATA->file);
                    }

                    /* Show progress */
                    INST_DATA->file_idx += prim->bodyLength;
                    INST_DATA->file_size = INST_DATA->file_idx;
                    if(INST_DATA->file_size != 0)
                    {
                        transferStats(INST_DATA);
                    }

                    /* Fully end transfer */
                    transferEnd(inst, INST_DATA, TRUE, FALSE);

                    if(INST_DATA->targ_name)
                    {
                        CsrPmemFree(INST_DATA->targ_name);
                        INST_DATA->targ_name = CsrStrDup("");
                    }
                }
                else
                {
                    transferEnd(inst, INST_DATA, FALSE, FALSE);

                    if(INST_DATA->targ_name)
                    {
                        CsrPmemFree(INST_DATA->targ_name);
                        INST_DATA->targ_name = CsrStrDup("");
                    }
                    printf("Get Message failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM:
            {
                CsrBtMapcSetMessageStatusCfm *prim;

                prim = (CsrBtMapcSetMessageStatusCfm*)inst->msg;

                stateChange(INST_DATA, STATE_CONNECTED);

                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    CsrPmemFree(INST_DATA->targ_name);
                    INST_DATA->targ_name = CsrStrDup("");
                    playMenu(inst);
                    printf("Successfully changed remote messagestatus!\n");
                }
                else
                {
                    playMenu(inst);
                    printf("Change remote messagestatus failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_PUSH_MESSAGE_IND:
            {
                CsrBtMapcPushMessageInd *prim;
                CsrBool finalFlag;
                CsrUint16 length;
                CsrUint8 *body;

                prim = (CsrBtMapcPushMessageInd*)inst->msg;

                /* Read in data */
                body = CsrPmemAlloc(prim->maxAllowedPayloadSize);
                length = CsrBtFread(body,
                               1,
                               prim->maxAllowedPayloadSize,
                               INST_DATA->file);

                /* End reached? */
                if(CsrBtIsAtEof(INST_DATA->file, INST_DATA->file_size))
                {
                    /* Close file now, obj-body handler will end transfer */
                    finalFlag = TRUE;
                    CsrBtFclose(INST_DATA->file);
                    INST_DATA->file = NULL;
                }
                else
                {
                    finalFlag = FALSE;
                }

                /* Show progress and send data request */
                INST_DATA->file_idx += length;
                CsrBtMapcPushMessageResSend(prim->instanceId, finalFlag,
                                     length,
                                     body);
                transferStats(INST_DATA);
                break;
            }

        case CSR_BT_MAPC_PUSH_MESSAGE_CFM:
            {
                CsrBtMapcPushMessageCfm *prim;

                prim = (CsrBtMapcPushMessageCfm*)inst->msg;

                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    transferEnd(inst, INST_DATA, TRUE, FALSE);
                }
                else
                {
                    transferEnd(inst, INST_DATA, FALSE, FALSE);
                    printf("Push Message failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_UPDATE_INBOX_CFM:
            {
                CsrBtMapcUpdateInboxCfm *prim;

                prim = (CsrBtMapcUpdateInboxCfm *)inst->msg;

                if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    playMenu(inst);
                    printf("Update Inbox operation succeeded\n");
                }
                else
                {
                    playMenu(inst);
                    printf("Update Inbox operation failed, OBEX error code 0x%04x\n",
                           prim->result);
                }
                break;
            }

        case CSR_BT_MAPC_ABORT_CFM:
            {
                CsrBtMapcAbortCfm *prim;

                prim = (CsrBtMapcAbortCfm*)inst->msg;

                /* Cleanup */
                if(INST_DATA->file != NULL)
                {
                    CsrBtFclose(INST_DATA->file);
                }
                INST_DATA->file = NULL;
                INST_DATA->file_idx = 0;
                INST_DATA->file_size = 0;
                INST_DATA->file_time = CsrTimeGet(NULL);

                stateChange(INST_DATA, STATE_CONNECTED);
                playMenu(inst);
                printf("Abort confirm received!\n");
                break;
            }

        case CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND:
            {
                CsrBtMapcNotificationRegistrationOffInd *prim = inst->msg;

                if(INST_DATA->isMnsMaster)
                {
                    clearMnsRegistrationForOtherMasInstancesToThisDevice(inst,INST_DATA);
                }
                printf("MSE abruptly terminated the MNS Notification Registration! You need to reregister for notifications in order to receive them again\n");
                CURRENT_INST.registerNotificationStatus = FALSE;
                break;
            }

        case CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM:
        {
            CsrBtMapcNotificationRegistrationCfm *prim = inst->msg;

            if(prim->result == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
            {
                stateChange(&CURRENT_INST, STATE_CONNECTED);
                if(CURRENT_INST.registerNotificationStatus)
                {
                   CURRENT_INST.registerNotificationStatus = FALSE;
                }
                else
                {
                   CURRENT_INST.registerNotificationStatus = TRUE;
                }
                playMenu(inst);
                printf("Successfully changed Notification Registration!\n");
            }
            else
            {
                playMenu(inst);
                printf("Change Notification Registration failed, OBEX error code 0x%04x\n",
                       prim->result);
            }
            break;
        }

        case CSR_BT_MAPC_EVENT_NOTIFICATION_IND:
            {
                CsrBtMapcEventNotificationInd *prim = inst->msg;

                printf("EventNotificationInd: finalFlag=%d\n",prim->finalFlag);
                dumpReceivedPayload(prim->payload + prim->bodyOffset,prim->bodyLength);

                /* Not final, so ask for more */
                CsrBtMapcEventNotificationResSend(prim->instanceId,
                                                  (CsrUint8) (prim->finalFlag ? CSR_BT_OBEX_SUCCESS_RESPONSE_CODE : CSR_BT_OBEX_CONTINUE_RESPONSE_CODE),
                                                  FALSE);

                break;
            }

        default:
            {
                printf("Unhandled MAPC primitive 0x%04X\n", type);
                break;
            }
    }

    CsrBtMapcFreeUpstreamMessageContents(CSR_BT_MAPC_PRIM, inst->msg);
}

static void mapScAppSetFnFini(void *pFiniData)
{
    MapcAppInst *inst = (MapcAppInst *) pFiniData;
    stateChange(&CURRENT_INST, CURRENT_INST.state_old);
    system(CLS);
    playMenu(inst);
}

static void mapScAppSetFnInit(void *pInitData)
{
    MapcAppInst *inst = (MapcAppInst *) pInitData;
    if(STATE_CONNECTING == CURRENT_INST.state)
    {
        stateChange(&CURRENT_INST, STATE_PAIR_WHILE_CONNECT_ATTEMPT);
    }
    else
    {
        stateChange(&CURRENT_INST, STATE_PAIRING);
    }
}

static void* mapScAppSetFnGetMessage(void *pRecvMsgPData)
{
    MapcAppInst *inst  = (MapcAppInst *) pRecvMsgPData;
    void *result          = inst->msg;
    inst->msg         = NULL;
    return result;
}

/* Initialize demo app */
void CsrBtAppInit(void **gash)
{
    MapcAppInst *inst;

    inst = CsrPmemAlloc(sizeof(MapcAppInst));
    CsrMemSet(inst, 0, sizeof(MapcAppInst));
    *gash = inst;

    /* Application handle */
    inst->qid = TESTQUEUE;
    inst->mapcInstanceListReceived = FALSE;

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
                         "MAPC", CSR_BT_MAPC_PRIM,
                         CSR_BT_MAPC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_MAPC_SECURITY_OUT_REQ);

    CsrBtMapcGetInstanceIdsReqSend(inst->qid);
    playMenu(inst);
}

/* Demo application handler */
void CsrBtAppHandler(void **gash)
{
    MapcAppInst *inst = *gash;

    if(CsrSchedMessageGet(&(inst->event), &(inst->msg)))
    {
        switch (inst->event)
        {
            case KEY_MESSAGE:
                handleKeyboardEvents(inst);
                break;

            case CSR_BT_MAPC_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) inst->msg;
                    switch(*prim)
                    {
                        case CSR_BT_MAPC_SECURITY_OUT_CFM:
                            handleScSecLevelPrim(&inst->scInstData);
                            break;

                        default:
                            handleMapcEvents(inst);
                            break;
                    }
                    break;
                }
            case CSR_BT_SC_PRIM:
                handleScPrim(&inst->scInstData);
                break;

            case CSR_BT_SD_PRIM:
                handleSdEvents(inst);
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
    MapcAppInst *inst;

    inst = *gash;
    printf("Cleaning up...\n");

    /* Get a message from the demoapplication message queue. The message is returned in prim
     * and the event type in eventType */
    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
            case CSR_BT_MAPC_PRIM:
            {
                CsrBtMapcFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                CsrBtSdFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
        }
        CsrPmemFree(msgData);
    }

    for(i=0; i<inst->numberOfInstances;i++)
    {
        /* Free browser info */
        browserEmpty(&inst->mapcInst[i]);

        /* Free instance */
        if(inst->mapcInst[i].file)
        {
            CsrBtFclose(inst->mapcInst[i].file);
        }
        CsrPmemFree(inst->mapcInst[i].folder);
        CsrPmemFree(inst->mapcInst[i].dir_newrem);
        CsrPmemFree(inst->mapcInst[i].targ_name);
        CsrPmemFree(inst->mapcInst[i].dir_local);
        CsrPmemFree(inst->mapcInst[i].dir_remote);
    }
    CsrPmemFree(inst->cls);
    CsrPmemFree(inst);
}
#endif

static void resetParameters(MapcInstance *inst)
{
    CsrMemSet(inst->tempValue,0,6);
    inst->tempValueLen = 0;
    CsrPmemFree(inst->folderName);
    inst->folderName = NULL;                  /* null terminated name string of the folder from where the current message listing is to be retreived: NB: if NULL this means current folder */
    inst->maxListCount = 0;                   /* maximum number of messages in the listing */
    inst->listStartOffset = 0;                /* offset of where to start the listing */
    inst->maxSubjectLength = 0;               /* maximum string length allowed of the subject field */
    inst->parameterMask = 0;                  /* bitmask of relevant parameters for the message listing. NB: a bit value of 1 means that the parameter should be present and a value of 0 means it should be filtered out */
    inst->filterMessageType = 0;              /* bitmask specifying which message types to include/exclude in the listing. NB: a bit value of 1 means that the message type should be filtered and a value of 0 means that it should be present */
    CsrPmemFree(inst->filterPeriodBegin);
    inst->filterPeriodBegin = NULL;           /* null terminated time string */
    CsrPmemFree(inst->filterPeriodEnd);
    inst->filterPeriodEnd = NULL;             /* null terminated time string */
    inst->filterReadStatus = 0;               /* bitmask specifying if filtering should be done on behalf of the read status */
    CsrPmemFree(inst->filterRecipient);
    inst->filterRecipient = NULL;               /* null terminated recipient string */
    CsrPmemFree(inst->filterOriginator);
    inst->filterOriginator = NULL;              /* null terminated originator string */
    inst->filterPriority = 0;                 /* bitmask specifying which priority type to include in the listing */

    inst->charset = 0;                        /* bitmask used to specifying trans-coding of the message */

    CsrPmemFree(inst->messageHandle);
    inst->messageHandle = NULL;                 /* null terminated message handle string */
    inst->attachment = 0;                     /* bitmask specifying whether to include attachment or not */
    inst->fractionRequest = 0;                /* bitmask which fragment of the message to get _if any_ */
    inst->statusIndicator = 0;                /* specifies which status information to modify */
    inst->statusValue = 0;                    /* specifies the new value of the status indication in question */
    inst->lengthOfObject = 0;                 /* total length of the message to send, NB: if set to zero this informative OBEX header field will not be included in the packet*/
    inst->transparent = 0;                    /* specifies if the MSE should keep a copu of the message in the sent folder */
    inst->retry = 0;                          /* specifies if the MSE should try to resent if first delivery to the network fails */
}

static void startGetFolderListing(MapcInstance *inst)
{
    printf("\nEnter the maximum number of folders in the listing: \n");
    stateChange(inst, STATE_GET_FOLDER_LISTING_PARAMETERS);

    inst->keyboardParameterState = MAP_GET_FOLDER_LISTING_MAX_LIST_COUNT;
    resetParameters(inst);
}

static void startGetMessageListing(MapcInstance *inst)
{
    printf("\nEnter name of the child folder from which the listing should be obtained (NB: leave blank for current folder): \n");
    stateChange(inst, STATE_GET_MESSAGE_LISTING_PARAMETERS);

    inst->keyboardParameterState = MAP_GET_MESSAGE_LISTING_FOLDER_NAME;
    resetParameters(inst);
}

static void startGetMessage(MapcInstance *inst)
{
    printf("\nEnter specific message handle to get (NB: leave blank for current selected message): \n");
    stateChange(inst, STATE_GET_MESSAGE_PARAMETERS);

    inst->keyboardParameterState = MAP_GET_MESSAGE_MESSAGE_HANDLE;
    resetParameters(inst);
}

static void startSetMessageStatus(MapcInstance *inst)
{
    printf("\nEnter specific message handle to get (NB: leave blank for current selected message): \n");
    stateChange(inst, STATE_SET_MESSAGE_STATUS_PARAMETERS);

    inst->keyboardParameterState = MAP_SET_MESSAGE_STATUS_MESSAGE_HANDLE;
    resetParameters(inst);
}

static void startPushMessage(MapcInstance *inst)
{
    printf("\nEnter name of the child folder to which the message should be pushed (NB: leave blank for current folder): \n");
    stateChange(inst, STATE_PUSH_MESSAGE_PARAMETERS);

    inst->keyboardParameterState = MAP_PUSH_MESSAGE_FOLDER_NAME;
    resetParameters(inst);
}

static CsrBool getUint8Value(MapcInstance *inst, CsrUint8 *value, CsrUint8 key)
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

static CsrBool getUint16Value(MapcInstance *inst, CsrUint16 *value, CsrUint8 key)
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

static void playCurrentUint8BitMask(CsrUint8 bitMask, char** array, CsrUint32 sizeOfArray)
{
    CsrUint8 i;

    printf("Current bitMask = 0x%02X\n",bitMask);
    for(i = 0; i < sizeOfArray; i++)
    {
        printf("%s\n",array[i]);
    }
}

static void playCurrentUint32BitMask(CsrUint32 bitMask, char** array, CsrUint32 sizeOfArray)
{
    CsrUint8 i;

    printf("Current bitMask = 0x%08X\n",bitMask);
    for(i = 0; i < sizeOfArray; i++)
    {
        printf("%s\n",array[i]);
    }
}

static CsrBool isMnsNotActiveForOtherMasInstancesToThisDevice(MapcAppInst *inst, MapcInstance *curInst)
{
    CsrUint8 i;
    CsrBool ret = TRUE;

    for (i=0;i<inst->numberOfInstances;i++)
    {
        if(&inst->mapcInst[i] != curInst)
        { /* Don't include own instance */
            if(CsrBtBdAddrEq(&curInst->address, &inst->mapcInst[i].address))
            {
                if(inst->mapcInst[i].registerNotificationStatus)
                {
                    ret = FALSE;
                    break;
                }
            }
        }
    }

    return ret;
}

/* Handle keypresses */
void handleKeyboardEvents(MapcAppInst *inst)
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

    if(!inst->mapcInstanceListReceived)
    {
        return;
    }

    /* State-based catching of keys */
    switch(CURRENT_INST.state)
    {
        case STATE_IDLE:
            switch(key->key)
            {
                case 's':
                    startSearch(inst, &CURRENT_INST);
                    return;

                case 'p':
                    handleScKeybPrim(&inst->scInstData);
                    return;

                case 'c':
                    startConnecting(inst, &CURRENT_INST);
                    return;

                case 'z':
                    listMapcInstances(inst);
                    return;
            }
            break;

        case STATE_SEARCH:
            switch(key->key)
            {
                case 's':
                    /* Stop search */
                    printf("Stopping search...\n");
                    stateChange(&CURRENT_INST, STATE_CLOSE_SEARCH);
                    CsrBtSdCancelSearchReqSend(inst->qid);
                    return;
            }
            break;

        case STATE_SELECT_ADDR:
            {
                if((key->key - '0') < CURRENT_INST.search_idx)
                {
                    printf("%c\n", key->key);
                    CURRENT_INST.address = CURRENT_INST.search_list[(key->key - '0')];
                    stateChange(&CURRENT_INST, STATE_SERVICE_SEARCH);
                    printf("Searching for services on selected device...\n");
                    CsrBtSdReadAvailableServicesReqSend(inst->qid,
                                                   CURRENT_INST.address,
                                                   0,       /* Service search configuration */
                                                   0,       /* Filter length */
                                                   NULL);   /* Filter */
                }
                else
                {
                    printf("Bad index selection!\nSelect device from the list: ");
                }
                return;
            }
            break;

        case STATE_PAIRING:
            {
                handleScKeybPrim(&inst->scInstData);
                return;
            }
            break;

        case STATE_PAIR_WHILE_CONNECT_ATTEMPT:
            {
                switch(key->key)
                {
                    case 'c':
                        CsrBtMapcCancelConnectReqSend(CURRENT_INST.mapcInstanceId);
                        return;
                    default:
                        handleScKeybPrim(&inst->scInstData);
                        return;
                }
            }
            break;

        case STATE_CONNECTING:
            {
                switch(key->key)
                {
                    case 'c':
                        CsrBtMapcCancelConnectReqSend(CURRENT_INST.mapcInstanceId);
                        return;
                }
            }
            break;

        case STATE_CONNECTED:
            switch(key->key)
            {
                case 'd':
                    stateChange(&CURRENT_INST, STATE_DISCONNECTING);
                    CsrBtMapcDisconnectReqSend(CURRENT_INST.mapcInstanceId, TRUE);
                    return;
                case 'm':
                    stateChange(&CURRENT_INST, STATE_NOTIFICATION_REGISTRATION);
                    if(CURRENT_INST.registerNotificationStatus)
                    {
                        CsrBtMapcNotificationRegistrationReqSend(CURRENT_INST.mapcInstanceId, FALSE, FALSE);
                    }
                    else
                    {
                        CURRENT_INST.isMnsMaster = isMnsNotActiveForOtherMasInstancesToThisDevice(inst,&CURRENT_INST);
                        CsrBtMapcNotificationRegistrationReqSend(CURRENT_INST.mapcInstanceId, TRUE, CURRENT_INST.isMnsMaster);
                    }

                case 'l':
                    selectLocal(inst, &CURRENT_INST);
                    return;

                case 'r':
                    startGetFolderListing(&CURRENT_INST);
                    return;

                case 't':
                    selectDirectory(inst, &CURRENT_INST, "/", TRUE);
                    return;

                case 'u':
                    selectDirectory(inst, &CURRENT_INST, "..", TRUE);
                    return;

                case 'j':
                    CsrBtMapcUpdateInboxReqSend(CURRENT_INST.mapcInstanceId);
                    return;

                case 'g':
                    startGetMessageListing(&CURRENT_INST);
                    return;

                case 'p':
                    stateChange(&CURRENT_INST, STATE_TRANSFER);
                    CsrBtMapcSetFolderReqSend(CURRENT_INST.mapcInstanceId, CsrStrDup((const char*)"sub1"));
                    return;

                case 'x':
                    if(CURRENT_INST.targ_remote)
                    {
                        startGetMessage(&CURRENT_INST);
                    }
                    else
                    {
                        startPushMessage(&CURRENT_INST);
                    }
                    return;

                case 'v':
                    startGetMessage(&CURRENT_INST);
                    return;


                case 'e':
                    startSetMessageStatus(&CURRENT_INST);

                    return;

                case 'c':
                    if(CsrStrLen(CURRENT_INST.targ_name) > 0)
                    {
                        if(CURRENT_INST.targ_dir)
                        {
                            selectDirectory(inst, &CURRENT_INST, CURRENT_INST.targ_name, CURRENT_INST.targ_remote);
                        }
                    }
                    return;

                case 'z':
                    listMapcInstances(inst);
                    break;
            }
            break;

        case STATE_SELECT_LOCAL:
        case STATE_SELECT_REMOTE:
            {
                if(key->key == ' ')
                {
                    /* Ignore spaced */
                }
                if(key->key == 'c')
                {
                    stateChange(&CURRENT_INST, STATE_CONNECTED);
                    playMenu(inst);
                }
                if(key->key == 'n')
                {
                    if(CURRENT_INST.dir_count - CURRENT_INST.dir_index + MAX_DISCOVERY_IN_LIST > 0)
                    {
                        CURRENT_INST.dir_index += MAX_DISCOVERY_IN_LIST;
                    }
                    playBrowser(inst, &CURRENT_INST, TRUE);
                    return;
                }
                else if(key->key == 'p')
                {
                    if(CURRENT_INST.dir_index < MAX_DISCOVERY_IN_LIST)
                    {
                        CURRENT_INST.dir_index = 0;
                    }
                    else
                    {
                        CURRENT_INST.dir_index -= MAX_DISCOVERY_IN_LIST;
                    }
                    playBrowser(inst, &CURRENT_INST, TRUE);
                    return;
                }
                else if((key->key >= '0') && (key->key <= '9') && (CURRENT_INST.dir_index + key->key - '0' <= CURRENT_INST.dir_count))
                {
                    char *name;
                    CsrBool dir;

                    CURRENT_INST.dir_index = CURRENT_INST.dir_index + key->key - '0';
                    name = browserPeek(&CURRENT_INST, CURRENT_INST.dir_index)->name;
                    dir = browserPeek(&CURRENT_INST, CURRENT_INST.dir_index)->is_dir;
                    CURRENT_INST.targ_remote = (CURRENT_INST.state == STATE_SELECT_REMOTE) ? TRUE : FALSE;

                    /* Set new target */
                    CsrPmemFree(CURRENT_INST.targ_name);
                    CURRENT_INST.targ_dir = dir;
                    CURRENT_INST.targ_name = CsrStrDup(name);
                    stateChange(&CURRENT_INST, STATE_CONNECTED);
                    playMenu(inst);
                    return;
                }
                /* Unhandled key */
                break;
            }
            break;

        case STATE_TRANSFER:
            switch(key->key)
            {
                case 'a':
                    {
                        CsrBtMapcAbortReqSend(CURRENT_INST.mapcInstanceId);
                        printf("Abort request sent...\n");
                    }
                    break;
            }
            break;

        case STATE_SERVICE_SEARCH:
            switch(key->key)
            {
                case 's':
                    {
                        CsrBtSdCancelReadAvailableServicesReqSend(inst->qid);
                        return;
                    }
            }
            break;

        case STATE_SELECT_MAPC_INSTANCE:
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
                return;
            }
            break;
        case STATE_SELECT_MAS_INSTANCE_ID:
            {
                if((key->key - '0') < CURRENT_INST.masInstanceListSize)
                {
                    printf("%c\n", key->key);
                    CsrBtMapcSelectMasInstanceResSend(CURRENT_INST.mapcInstanceId,TRUE,CURRENT_INST.masInstanceList[key->key - '0'].masInstanceId);
                    CsrPmemFree(CURRENT_INST.masInstanceList);
                    CURRENT_INST.masInstanceList = NULL;
                    CURRENT_INST.masInstanceListSize = 0;
                    stateChange(&CURRENT_INST, STATE_CONNECTING);
                    playMenu(inst);
                }
                else if (key->key == 'C' || key->key == 'c')
                {
                    CsrBtMapcSelectMasInstanceResSend(CURRENT_INST.mapcInstanceId,FALSE,0);
                    CsrPmemFree(CURRENT_INST.masInstanceList);
                    CURRENT_INST.masInstanceList = NULL;
                    CURRENT_INST.masInstanceListSize = 0;
                    playMenu(inst);
                }
                else
                {
                    printf("Bad index selection!\nSelect index from the list: ");
                }
                return;
            }
            break;
        case  STATE_GET_FOLDER_LISTING_PARAMETERS:
        {
            switch(CURRENT_INST.keyboardParameterState)
            {
                case MAP_GET_FOLDER_LISTING_MAX_LIST_COUNT:
                {
                    if(getUint16Value(&CURRENT_INST, &CURRENT_INST.maxListCount, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_FOLDER_LISTING_LIST_START_OFFSET;
                        printf("\nEnter Listing start offset\n");
                    }
                    break;
                }
                case MAP_GET_FOLDER_LISTING_LIST_START_OFFSET:
                {
                    if(getUint16Value(&CURRENT_INST, &CURRENT_INST.listStartOffset, key->key))
                    { /* Final parameter start getting the folder listing */
                        selectRemote(&CURRENT_INST);
                    }
                    break;
                }
            }
            break;
        }

        case  STATE_GET_MESSAGE_LISTING_PARAMETERS:
        {
            switch(CURRENT_INST.keyboardParameterState)
            {
                case MAP_GET_MESSAGE_LISTING_FOLDER_NAME:
                {
                    if(getString(&CURRENT_INST.folderName, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_MAX_LIST_COUNT;
                        printf("\nEnter Max List Count (NB: a value of 0 will just return the actual number of messages in the folder!)");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_MAX_LIST_COUNT:
                {
                    if(getUint16Value(&CURRENT_INST, &CURRENT_INST.maxListCount, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_LIST_START_OFFSET;
                        printf("\nEnter Listing start offset\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_LIST_START_OFFSET:
                {
                    if(getUint16Value(&CURRENT_INST, &CURRENT_INST.listStartOffset, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_MAX_SUBJECT_LENGTH;
                        printf("\nEnter Max Subject Length\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_MAX_SUBJECT_LENGTH:
                {
                    if(getUint8Value(&CURRENT_INST, &CURRENT_INST.maxSubjectLength, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_PARAMETER_MASK;
                        printf("\nToggle Parameter bitmask and press ENTER to continue\n");
                        playCurrentUint32BitMask(CURRENT_INST.parameterMask,MapcAppParameterBitmaskString, 16);
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_PARAMETER_MASK:
                {
                    if(getUint32Bitmask(&CURRENT_INST.parameterMask, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_MESSAGE_TYPE;
                        printf("\nToggle Filter Message Type bitmask and press ENTER to continue\n");
                        playCurrentUint8BitMask(CURRENT_INST.filterMessageType,MapcAppFilterMessageTypeString, 5);
                    }
                    else
                    {
                        playCurrentUint32BitMask(CURRENT_INST.parameterMask,MapcAppParameterBitmaskString, 16);
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_MESSAGE_TYPE:
                {
                    if(getUint8Bitmask(&CURRENT_INST.filterMessageType, key->key, 4))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_BEGIN;
                        printf("\nEnter Filter Period Begin in UTC time format YYYYMMDDTHHMMSSZ: (NB: Leave blank to disable this filter option)\n");
                    }
                    else
                    {
                        playCurrentUint8BitMask(CURRENT_INST.filterMessageType,MapcAppFilterMessageTypeString, 5);
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_BEGIN:
                {
                    if(getString(&CURRENT_INST.filterPeriodBegin, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_END;
                        printf("\nEnter Filter Period End in UTC time format YYYYMMDDTHHMMSSZ: (NB: Leave blank to disable this filter option)\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_PERIOD_END:
                {
                    if(getString(&CURRENT_INST.filterPeriodEnd, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_READ_STATUS;
                        printf("\nSelect Filter Read Status:\n");
                        printf("0) CSR_BT_MAP_STATUS_NO_FILTERING\n");
                        printf("1) CSR_BT_MAP_STATUS_UNREAD\n");
                        printf("2) CSR_BT_MAP_STATUS_READ\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_READ_STATUS:
                {
                    CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_RECIPIENT;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.filterReadStatus = CSR_BT_MAP_STATUS_NO_FILTERING;
                            break;
                        case '1':
                            CURRENT_INST.filterReadStatus = CSR_BT_MAP_STATUS_UNREAD;
                            break;
                        case '2':
                            CURRENT_INST.filterReadStatus = CSR_BT_MAP_STATUS_READ;
                            break;
                        default:
                            CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_READ_STATUS;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(CURRENT_INST.keyboardParameterState == MAP_GET_MESSAGE_LISTING_FILTER_RECIPIENT)
                    {
                        printf("\nEnter Filter Recipient (NB: Leave blank to disable this filter option)\n");
                    }

                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_RECIPIENT:
                {
                    if(getString(&CURRENT_INST.filterRecipient, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_ORIGINATOR;
                        printf("\nEnter Filter Originator (NB: Leave blank to disable this filter option)\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_ORIGINATOR:
                {
                    if(getString(&CURRENT_INST.filterOriginator, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_LISTING_FILTER_PRIORITY;
                        printf("\nSelect Filter Priority:\n");
                        printf("0) CSR_BT_MAP_PRIORITY_NO_FILTERING\n");
                        printf("1) CSR_BT_MAP_PRIORITY_HIGH\n");
                        printf("2) CSR_BT_MAP_PRIORITY_NON_HIGH\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_LISTING_FILTER_PRIORITY:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.filterPriority = CSR_BT_MAP_STATUS_NO_FILTERING;
                            break;
                        case '1':
                            CURRENT_INST.filterPriority = CSR_BT_MAP_PRIORITY_HIGH;
                            break;
                        case '2':
                            CURRENT_INST.filterPriority = CSR_BT_MAP_PRIORITY_NON_HIGH;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        selectRemoteMessage(&CURRENT_INST);
                    }

                    break;
                }
                default:
                {
                    printf("Invalid get message listing state %d: 0x%02X (file: %s, line %d)\n",CURRENT_INST.keyboardParameterState , key,__FILE__,__LINE__);
                }
            }
            break;
        }
        case  STATE_GET_MESSAGE_PARAMETERS:
        {
            switch(CURRENT_INST.keyboardParameterState)
            {
                case MAP_GET_MESSAGE_MESSAGE_HANDLE:
                {
                    if(getString((CsrCharString**)&CURRENT_INST.messageHandle, key->key))
                    {
                        if(CURRENT_INST.targ_name && !CURRENT_INST.messageHandle)
                        {
                            /* TargetName available. Message Handle is the Name in case of Get Message */
                            CURRENT_INST.messageHandle = CsrUtf8StrDup(CURRENT_INST.targ_name);
                        }
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_ATTACHMENT;
                        printf("\nSelect Whether to include attachment if present, or not:\n");
                        printf("0) CSR_BT_MAP_ATTACHMENT_OFF\n");
                        printf("1) CSR_BT_MAP_ATTACHMENT_ON\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_ATTACHMENT:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.attachment = CSR_BT_MAP_ATTACHMENT_OFF;
                            break;
                        case '1':
                            CURRENT_INST.attachment = CSR_BT_MAP_ATTACHMENT_ON;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_CHARSET;
                        printf("\nSelect Message charset:\n");
                        printf("0) CSR_BT_MAP_CHARSET_NATIVE\n");
                        printf("1) CSR_BT_MAP_CHARSET_UTF8\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_CHARSET:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.charset = CSR_BT_MAP_CHARSET_NATIVE;
                            break;
                        case '1':
                            CURRENT_INST.charset = CSR_BT_MAP_CHARSET_UTF8;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.keyboardParameterState = MAP_GET_MESSAGE_FRACTION_REQUEST;
                        printf("\nSelect Fraction Request:\n");
                        printf("0) CSR_BT_MAP_FRACTION_REQ_NOT_USED\n");
                        printf("1) CSR_BT_MAP_FRACTION_REQ_FIRST\n");
                        printf("2) CSR_BT_MAP_FRACTION_REQ_NEXT\n");
                    }
                    break;
                }
                case MAP_GET_MESSAGE_FRACTION_REQUEST:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.fractionRequest = CSR_BT_MAP_FRACTION_REQ_NOT_USED;
                            break;
                        case '1':
                            CURRENT_INST.fractionRequest = CSR_BT_MAP_FRACTION_REQ_FIRST;
                            break;
                        case '2':
                            CURRENT_INST.fractionRequest = CSR_BT_MAP_FRACTION_REQ_NEXT;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.targ_remote = TRUE;
                        if(CURRENT_INST.messageHandle)
                        {
                            CsrPmemFree(CURRENT_INST.targ_name);
                            CURRENT_INST.targ_name = CsrPmemAlloc(CsrUtf8StringLengthInBytes(CURRENT_INST.messageHandle) + 5);
                            CsrStrCpy(CURRENT_INST.targ_name,(char *)CURRENT_INST.messageHandle);
                            CsrStrCat(CURRENT_INST.targ_name,".msg");
                        }
                        else
                        {
                            CsrPmemFree(CURRENT_INST.targ_name);
                            CURRENT_INST.targ_name = CsrStrDup("");
                        }
                        transferStart(inst, &CURRENT_INST);
                    }

                    break;
                }
                default:
                {
                    printf("Invalid get message listing state %d: 0x%02X (file: %s, line %d)\n",CURRENT_INST.keyboardParameterState , key,__FILE__,__LINE__);
                }
            }
            break;
        }

        case  STATE_SET_MESSAGE_STATUS_PARAMETERS:
        {
            switch(CURRENT_INST.keyboardParameterState)
            {
                case MAP_SET_MESSAGE_STATUS_MESSAGE_HANDLE:
                {
                    if(getString((CsrCharString **)&CURRENT_INST.messageHandle, key->key))
                    {
                        if(CURRENT_INST.messageHandle == NULL)
                        {
                            CURRENT_INST.messageHandle = CsrUtf8StrDup(CURRENT_INST.targ_name);
                        }
                        CURRENT_INST.keyboardParameterState = MAP_SET_MESSAGE_STATUS_STATUS_INDICATOR;
                        printf("\nSelect message status indicator to change:\n");
                        printf("0) CSR_BT_MAP_STATUS_IND_READ\n");
                        printf("1) CSR_BT_MAP_STATUS_IND_DELETE\n");
                    }
                    break;
                }
                case MAP_SET_MESSAGE_STATUS_STATUS_INDICATOR:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.statusIndicator = CSR_BT_MAP_STATUS_IND_READ;
                            break;
                        case '1':
                            CURRENT_INST.statusIndicator = CSR_BT_MAP_STATUS_IND_DELETE;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.keyboardParameterState = MAP_SET_MESSAGE_STATUS_STATUS_VALUE;
                        printf("\nSelect status value:\n");
                        printf("0) CSR_BT_MAP_STATUS_VAL_NO\n");
                        printf("1) CSR_BT_MAP_STATUS_VAL_YES\n");
                    }
                    break;
                }
                case MAP_SET_MESSAGE_STATUS_STATUS_VALUE:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.statusValue = CSR_BT_MAP_STATUS_VAL_NO;
                            break;
                        case '1':
                            CURRENT_INST.statusValue = CSR_BT_MAP_STATUS_VAL_YES;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        stateChange(&CURRENT_INST, STATE_CONNECTED);
                        CsrBtMapcSetMessageStatusReqSend(CURRENT_INST.mapcInstanceId, CURRENT_INST.messageHandle, CURRENT_INST.statusIndicator, CURRENT_INST.statusValue);
                        CURRENT_INST.messageHandle = NULL;
                    }
                    break;
                }
                default:
                {
                    printf("Invalid get message listing state %d: 0x%02X (file: %s, line %d)\n",CURRENT_INST.keyboardParameterState , key,__FILE__,__LINE__);
                }
            }
            break;
        }

        case  STATE_PUSH_MESSAGE_PARAMETERS:
        {
            switch(CURRENT_INST.keyboardParameterState)
            {
                case MAP_PUSH_MESSAGE_FOLDER_NAME:
                {
                    if(getString(&CURRENT_INST.folderName, key->key))
                    {
                        CURRENT_INST.keyboardParameterState = MAP_PUSH_MESSAGE_TRANSPARENT;
                        printf("\nSelect Whether the MSE should keep a copy of the message in the sent folder, or not:\n");
                        printf("0) CSR_BT_MAP_TRANSPARENT_NOT_SPECIFIED\n");
                        printf("1) CSR_BT_MAP_TRANSPARENT_OFF\n");
                        printf("2) CSR_BT_MAP_TRANSPARENT_ON\n");
                    }
                    break;
                }
                case MAP_PUSH_MESSAGE_TRANSPARENT:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.transparent = CSR_BT_MAP_TRANSPARENT_NOT_SPECIFIED;
                            break;
                        case '1':
                            CURRENT_INST.transparent = CSR_BT_MAP_TRANSPARENT_OFF;
                            break;
                        case '2':
                            CURRENT_INST.transparent = CSR_BT_MAP_TRANSPARENT_ON;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.keyboardParameterState = MAP_PUSH_MESSAGE_RETRY;
                        printf("\nSelect Whether the MSE should retry sending the message if first delivery to the network fails:\n");
                        printf("0) CSR_BT_MAP_RETRY_NOT_SPECIFIED\n");
                        printf("1) CSR_BT_MAP_RETRY_OFF\n");
                        printf("2) CSR_BT_MAP_RETRY_ON\n");
                    }
                    break;
                }
                case MAP_PUSH_MESSAGE_RETRY:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.retry = CSR_BT_MAP_RETRY_NOT_SPECIFIED;
                            break;
                        case '1':
                            CURRENT_INST.retry = CSR_BT_MAP_RETRY_OFF;
                            break;
                        case '2':
                            CURRENT_INST.retry = CSR_BT_MAP_RETRY_ON;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        CURRENT_INST.keyboardParameterState = MAP_PUSH_MESSAGE_CHARSET;
                        printf("\nSelect Message charset:\n");
                        printf("0) CSR_BT_MAP_CHARSET_NATIVE\n");
                        printf("1) CSR_BT_MAP_CHARSET_UTF8\n");
                    }
                    break;
                }
                case MAP_PUSH_MESSAGE_CHARSET:
                {
                    CsrBool done = TRUE;
                    switch(key->key)
                    {
                        case '0':
                            CURRENT_INST.charset = CSR_BT_MAP_CHARSET_NATIVE;
                            break;
                        case '1':
                            CURRENT_INST.charset = CSR_BT_MAP_CHARSET_UTF8;
                            break;
                        default:
                            done = FALSE;
                            printf("Invalid selection: 0x%02X (file: %s, line %d)\n", key->key,__FILE__,__LINE__);
                            break;
                    }

                    if(done)
                    {
                        transferStart(inst, &CURRENT_INST);
                    }

                    break;
                }
                default:
                {
                    printf("Invalid get message listing state %d: 0x%02X (file: %s, line %d)\n",CURRENT_INST.keyboardParameterState , key,__FILE__,__LINE__);
                }
            }
            break;
        }

        default:
            break;
    }

    /* Catch space */
    if(key->key == ' ')
    {
        playMenu(inst);
    }
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
