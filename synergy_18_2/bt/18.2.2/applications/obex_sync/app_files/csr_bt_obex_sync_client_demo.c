/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_syncc_prim.h"
#include "csr_bt_syncc_lib.h"
#include "csr_bt_xml_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_obex_sync_demo_common.h"
#include "csr_sched_init.h"

#include "csr_unicode.h"

#define VERBOSE_BODY_OUTPUT

#define KEY_CHECK_TIMEOUT               100000
#define MAX_DISCOVERY_IN_LIST           10
#define MAX_PACKET_SIZE                 0x4000
#define MAX_USERID_PASSWORD_LENGTH  30

#define SYNC_MAX_OBJECT_NAME_LENGTH          230

typedef CsrUint8 syncKbmState;
#define SYNC_KBM_STATE_NORMAL               (syncKbmState) 0x00
#define SYNC_KBM_STATE_GET_REPOSITORY       (syncKbmState) 0x01
#define SYNC_KBM_STATE_GET_STATIC_VS_LUID   (syncKbmState) 0x02
#define SYNC_KBM_STATE_GET_OBJECT_TYPE      (syncKbmState) 0x03
#define SYNC_KBM_STATE_GET_OBJECT_NAME      (syncKbmState) 0x04
#define SYNC_KBM_STATE_PUT_OPERATION        (syncKbmState) 0x05
#define SYNC_KBM_STATE_PUT_CHANGE_COUNTER   (syncKbmState) 0x06
#define SYNC_KBM_STATE_PUT_TIMESTAMP        (syncKbmState) 0x07
#define SYNC_KBM_STATE_PUT_HARD_SOFT        (syncKbmState) 0x08
#define SYNC_KBM_STATE_LAST                 (syncKbmState) 0x09

#define SYNC_ADD    0x00
#define SYNC_MODIFY 0x01
#define SYNC_DELETE 0x02

/* Global app states */
typedef enum
{
    STATE_IDLE,
    STATE_SEARCH,
    STATE_CLOSE_SEARCH,
    STATE_PAIRING,
    STATE_SELECT_ADDR,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_SELECT_LOCAL,
    STATE_TRANSFER,
    STATE_DISCONNECTING,
    STATE_AUTHENTICATE,
    STATE_SERVICE_SEARCH,
    STATE_PAIR_WHILE_CONNECT_ATTEMPT,
    STATE_ACTIVATING,
    STATE_EOL
} SynccAppState_t;

/* Textual representation of states */
static char *SynccAppStateString[] =
{
    "Idle",
    "Searching",
    "Closing search",
    "Pairing",
    "Select address",
    "Connecting",
    "Connected",
    "Select local object to ADD/MODIFY/DELETE",
    "Transferring",
    "Disconnecting",
    "Authenticate",
    "Searching for services"
    "Pairing caused by connection attempt"
};

/* File info container */
struct SynccInfo_t
{
    char              *name;
    CsrUint32          size;
    CsrBool            is_dir;
    struct SynccInfo_t  *next;
};

/* Authentication states */
#define AUTH_USERID   0
#define AUTH_PASS     1

/* Instance data for application */
typedef struct
{
    CsrSchedQid           CsrSchedQid;                                         /* My application/queue handle id */
    CsrBool              syncCommandServiceActivated;                 /* Sync Command activated */
    SynccAppState_t     state;                                       /* Global app state */
    SynccAppState_t     state_old;                                   /* Old app state */
    CsrBtDeviceAddr        search_list[MAX_DISCOVERY_IN_LIST];          /* Search list */
    CsrUint8             search_idx;                                  /* Search list index */
    CsrBtDeviceAddr        address;                                     /* Remote active address */
    char                *cls;                                        /* Path to CLS */
    CsrUint16            event;                                       /* Message id */
    void                *msg;                                        /* Message body */
    char                *targ_name;                                  /* Target name */
    CsrBool              targ_remote;                                 /* Is target remote? */
    CsrBool              targ_dir;                                    /* Is target a directory? */
    struct SynccInfo_t  *dir_head;                                   /* Browser head */
    struct SynccInfo_t  *dir_tail;                                   /* Browser tail */
    CsrUint16            dir_count;                                   /* Number of entries */
    CsrUint16            dir_index;                                   /* Current index */
    char                *dir_local;                                  /* Local directory */
    char                *dir_root;                                   /* Demo app root directory */
    CsrFileHandle                *file;                                       /* File transfer */
    CsrUint32            file_idx;                                    /* Transfer index */
    CsrUint32            file_size;                                   /* Transfer total size */
    CsrTime              file_time;                                   /* Transfer start time */
    CsrBool              user_id_required;                            /* include user_id in the response */
    char                auth_user[MAX_USERID_PASSWORD_LENGTH+1]; /* Authentication userid */
    CsrUint16            auth_state;                                  /* Authentication state */
    char                auth_pass[MAX_USERID_PASSWORD_LENGTH+1];      /* Authentication password */
    CsrUint16            auth_idx;                                    /* Authentication index */
    CsrUint8             passkey_length;                              /* Passkey length */
    CsrUint8             passkey[CSR_BT_PASSKEY_MAX_LEN];                    /* Passkey */
    syncKbmState        keyboardMode;                                /* Keyboard state when connected */
    CsrCharString            *object_name;                                /* name of the object to get */
    CsrUint32            object_name_index;                           /* index counter for the object_name */
    CsrBool              download_full_store;                         /* Specifies if the entire object store should be downloaded */
    CsrUint8             put_operation;                               /* Specifies ADD/MOD/DEL */
    CsrCharString            *max_expected_change_counter;                /* Max expected change counter used for put operation */
    CsrUint32            max_expected_change_counter_index;           /* index counter for the max_expected_change_counter */
    CsrBool              hard_delete;                                 /* Specified hard or soft delete */
    ScAppInstData_t     scInstData;
} SynccAppInst;

/* Prototypes */
void handleKeyboardEvents(SynccAppInst *inst);
void cleanInstance(SynccAppInst *inst);

/* Change state */
void stateChange(SynccAppInst *inst, SynccAppState_t s)
{
    inst->state_old = inst->state;
    inst->state = s;
}

/* Show menu options */
void playMenu(SynccAppInst *inst)
{
    system(inst->cls);
    printf("\nCSR plc -- Synergy BT Synchronization Client demo application\n");
    printf("Hit <ESC> to quit. Hit <SPACE> to reprint menu.\n\n");

    printf("Sync Command status:%s\n",(inst->syncCommandServiceActivated ? "Activated" : "Deactivated"));
    printf("State...............%s\n", SynccAppStateString[inst->state]);
    printf("Active address......%04X:%02X:%06X\n",
           inst->address.nap,
           inst->address.uap,
           inst->address.lap);
    printf("Local dir...........'%s'\n", inst->dir_local);
    printf("\nMenu selection:\n");

    /* Options for (almost) all states */
    switch(inst->state)
    {
        case STATE_IDLE:
            if(inst->syncCommandServiceActivated)
            {
                printf("  a) Deactivate Sync Command feature\n\n");
            }
            else
            {
                printf("  a) Activate Sync Command feature\n\n");
            }
            printf("  s) Start searching for SYNC servers\n");
            printf("  p) Security Controller Configuration\n");
            printf("  c) Connect to selected address\n");
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
            if(inst->syncCommandServiceActivated)
            {
                printf("  a) Deactivate Sync Command feature\n\n");
            }
            else
            {
                printf("  a) Activate Sync Command feature\n\n");
            }
            printf("  d) Disconnect\n\n");

            printf("  l) Select local object to ADD/MODIFY/DELETE \n\n");

            printf("  m) Get device information of the server \n");
            printf("  n) Get real time clock of the server \n\n");

            printf("  p) Get full \"phone book\" store \n");
            printf("  q) Get full \"message/xx\" store \n");
            printf("  t) Get full \"calender\" store \n");
            printf("  u) Get full \"notes\" store\n\n");

            printf("  1) Get object from \"phone book\" store \n");
            printf("  2) Get object from \"message\" store \n");
            printf("  3) Get object from \"calender\" store \n");
            printf("  4) Get object from \"notes\" store\n\n");

            printf("  5) Get a manually specified object \n");

            break;

        case STATE_SELECT_LOCAL:
            printf("  c) Cancel selection\n");
            if(inst->dir_index >= MAX_DISCOVERY_IN_LIST)
            {
                printf("  p) Browse previous page\n");
            }
            if(inst->dir_index + MAX_DISCOVERY_IN_LIST < inst->dir_count)
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
    printf("\n");
}

/* Clean browser info */
void browserEmpty(SynccAppInst *inst)
{
    struct SynccInfo_t *next;

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
void browserAdd(SynccAppInst *inst, char *name, CsrUint32 size, CsrBool is_dir)
{
    struct SynccInfo_t *cur;

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

    cur = CsrPmemAlloc(sizeof(struct SynccInfo_t));
    cur->name = CsrStrDup(name);
    cur->size = size;
    cur->is_dir = is_dir;
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
struct SynccInfo_t *browserPeek(SynccAppInst *inst, CsrUint16 i)
{
    struct SynccInfo_t *cur;
    CsrUint16 this;

    if((inst->dir_count == 0) || (i >= inst->dir_count))
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
void playBrowser(SynccAppInst *inst)
{
    CsrUint16 i;
    CsrUint16 c;
    struct SynccInfo_t *cur;

    if((inst->dir_head == NULL) || (inst->dir_index >= inst->dir_count))
    {
        printf("\nNo more files to browse!\n");
        return;
    }

    playMenu(inst);
    i = inst->dir_index;
    c = 0;
    while((c < MAX_DISCOVERY_IN_LIST) && ((cur = browserPeek(inst, i)) != NULL))
    {
        printf("Entry %i, size %10d, %4s name '%s'\n",
               c,
               cur->size,
               cur->is_dir ? "dir" : "file",
               cur->name);
        i++;
        c++;
    }

    printf("\nPlease select entry: ");
}

/* Change directory. Does not consume the parameters */
void selectDirectory(SynccAppInst *inst, char *dir)
{
    /* Local */
    chdir(dir);

    CsrPmemFree(inst->dir_local);
    inst->dir_local = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);

    getcwd(inst->dir_local, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(inst->dir_local, PATH_SEP);

    /* Set new target */
    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");

    playMenu(inst);
}

/* Get transfer speed */
double transferGetSpeed(SynccAppInst *inst)
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
void transferStats(SynccAppInst *inst)
{
    printf("Transferred %8d of %8d bytes, speed %4.3f kB/s\n",
           inst->file_idx,
           inst->file_size,
           transferGetSpeed(inst));
}

/* End transfer (cleanup, etc.) */
void transferEnd(SynccAppInst *inst, CsrBool stats, CsrBool removeFile)
{
    stateChange(inst, STATE_CONNECTED);
    playMenu(inst);

    if(stats)
    {
        printf("%s operation of file '%s' completed.\n\n",
               (inst->targ_remote ? "GET" : "PUT"),
               inst->targ_name);
        printf("Total transfer size : %d bytes\n", inst->file_size);
        printf("Transfer time       : %f s\n", CsrTimeSub(CsrTimeGet(NULL), inst->file_time) / 1000000.0);
        printf("Transfer speed      : %f kB/s\n", transferGetSpeed(inst));
    }

    CsrPmemFree(inst->object_name);
    inst->object_name = NULL;
    inst->download_full_store = FALSE;

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
void transferStart(SynccAppInst *inst)
{
    char *mode;
    CsrBtStat_t info;
    CsrUint8 *unicode;
    CsrUint32 index = 0;

    stateChange(inst, STATE_TRANSFER);
    selectDirectory(inst, inst->dir_root);

    printf("Transferring information... for object: %s\n",inst->object_name);

    inst->targ_name = CsrPmemAlloc(inst->object_name_index + 1);
    CsrStrCpy(inst->targ_name, inst->object_name);

    if(CsrStrLen(inst->targ_name) == 0)
    {
        stateChange(inst, STATE_CONNECTED);
        CsrPmemFree(inst->object_name);
        CsrPmemFree(inst->targ_name);
        return;
    }

    /* Clean stat info */
    CsrMemSet(&info, 0, sizeof(CsrBtStat_t));

    /* Check get */
    if(inst->targ_remote)
    {
        /* Open for writing (get), file must not exist */
        mode = "wb";
        if(CsrBtStat(inst->targ_name, &info) == 0)
        {
            printf("File already exists! we overwrite it\n");
        }
        inst->file_size = 0;
    }
    else
    {
        /* Open for reading (put), file must exist */
        mode = "rb";
        if(CsrBtStat(inst->targ_name, &info) != 0)
        {
            printf("File does not exist!\n");
            stateChange(inst, STATE_CONNECTED);
            CsrPmemFree(inst->object_name);
            CsrPmemFree(inst->targ_name);
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
    unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->object_name);
    stateChange(inst, STATE_TRANSFER);
    inst->file_time = CsrTimeGet(NULL);
    printf("Started transfer of '%s'...\n",
           inst->object_name);

    /* Remote file, get it */
    if(inst->targ_remote)
    {
        CsrBtSynccGetObjectReqSend(unicode, FALSE);
    }
    else
    {
        switch(inst->put_operation)
        {
            case SYNC_ADD:
            { /* Find out if we should strip the object name because of unique indexing (the server assigns the name!) */
                CsrBool unique_indexing = FALSE;
                CsrUint32 extension_start = 0;

                while(--inst->object_name_index >= 5)
                { /* Search backwards through the object string to find the extension and first slash */
                    if(inst->object_name[inst->object_name_index] == '/')
                    {
                        if(!CsrMemCmp(&inst->object_name[inst->object_name_index-5],"/luid",5))
                        {
                            unique_indexing = TRUE;
                            CsrStrCpy(&inst->object_name[++inst->object_name_index],&inst->object_name[extension_start]);
                            break;
                        }
                        else
                        { /* Abandon search luid has to be the first thing prior to the last slash */
                            break;
                        }
                    }
                    else if(inst->object_name[inst->object_name_index] == '.' && extension_start == 0)
                    {
                        extension_start = inst->object_name_index;
                    }
                }

                if(unique_indexing)
                { /* object_name has been updated */
                    CsrPmemFree(unicode);
                    unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->object_name);
                }

                CsrBtSynccAddObjectReqSend(unicode,info.st_size);
                break;
            }
            case SYNC_MODIFY:
            {
                CsrBtSynccModifyObjectReqSend(unicode,inst->max_expected_change_counter,info.st_size);
                break;
            }
            case SYNC_DELETE:
            {
                CsrBtSynccDeleteObjectReqSend(inst->hard_delete, inst->max_expected_change_counter, unicode);
                break;
            }
        }
    }

    playMenu(inst);
}

/* Start local file selection */
void selectLocal(SynccAppInst *inst)
{
    DIR           *dir;
    struct dirent *entry;
    CsrBtStat_t    info;
    CsrBool        is_dir;
    char          *testRoot;
    CsrUint32      dir_root_length;

    stateChange(inst, STATE_SELECT_LOCAL);
    browserEmpty(inst);

    inst->dir_index = 0;

    dir_root_length = CsrStrLen(inst->dir_root);
    testRoot = CsrPmemAlloc(dir_root_length + 2);
    CsrStrNCpyZero(testRoot, inst->dir_root, dir_root_length + 2);
    CsrStrCpy(&testRoot[dir_root_length],PATH_SEP);

    if(!CsrStrCmp(inst->dir_local, testRoot))
    {
        selectDirectory(inst, "telecom");
    }
    CsrPmemFree(testRoot);

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
                browserAdd(inst, entry->d_name, info.st_size, is_dir);
            }
        }
        CsrBtCloseDir(dir);

        /* Display what we've just read */
        playBrowser(inst);
    }
    else
    {
        printf("Current local directory (%s) is empty\n",
               inst->dir_local);
        stateChange(inst, STATE_CONNECTED);
    }
}

static void syncKbmGetRepository(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        inst->keyboardMode = SYNC_KBM_STATE_GET_STATIC_VS_LUID;
        switch(key)
        {
            case '1':
                if(inst->download_full_store)
                {
                    CsrStrCpy(inst->object_name, SYNC_MESSAGING_INCOMING_NAME);
                    inst->object_name_index += CsrStrLen(SYNC_MESSAGING_INCOMING_NAME);
                }
                else
                {
                    CsrStrCpy(&inst->object_name[inst->object_name_index], "in/");
                    inst->object_name_index += CsrStrLen("in/");
                }
                break;
            case '2':
                if(inst->download_full_store)
                {
                    CsrStrCpy(inst->object_name, SYNC_MESSAGING_OUTGOING_NAME);
                    inst->object_name_index += CsrStrLen(SYNC_MESSAGING_OUTGOING_NAME);
                }
                else
                {
                    CsrStrCpy(&inst->object_name[inst->object_name_index], "out/");
                    inst->object_name_index += CsrStrLen("out/");
                }
                break;
            case '3':
                if(inst->download_full_store)
                {
                    CsrStrCpy(inst->object_name, SYNC_MESSAGING_SENT_NAME);
                    inst->object_name_index += CsrStrLen(SYNC_MESSAGING_SENT_NAME);
                }
                else
                {
                    CsrStrCpy(&inst->object_name[inst->object_name_index], "sent/");
                    inst->object_name_index += CsrStrLen("sent/");
                }
                break;
            default:
                inst->keyboardMode = SYNC_KBM_STATE_GET_REPOSITORY; /* Stay in current state */
                printf("Invalid selection try again\n");
                break;
        }

        if(inst->download_full_store)
        {
            inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
            transferStart(inst);
        }

        if(inst->keyboardMode == SYNC_KBM_STATE_GET_STATIC_VS_LUID)
        {
            printf("Select static vs. unique (luid) object indexing:\n");
            printf("\t1) static indexing\n");
            printf("\t2) unique indexing\n");
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmGetStaticVsLuid(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        inst->keyboardMode = SYNC_KBM_STATE_GET_OBJECT_TYPE;
        switch(key)
        {
            case '1':
                /* Do nothing ojectname is already correct */
                break;
            case '2':
                CsrStrNCpyZero(&inst->object_name[inst->object_name_index], "luid/", SYNC_MAX_OBJECT_NAME_LENGTH - inst->object_name_index);
                inst->object_name_index += CsrStrLen("luid/");
                break;
            default:
                inst->keyboardMode = SYNC_KBM_STATE_GET_STATIC_VS_LUID; /* Stay in current state */
                printf("Invalid selection try again\n");
                break;
        }

        if(inst->keyboardMode == SYNC_KBM_STATE_GET_OBJECT_TYPE)
        {
            printf("Select object:\n");
            printf("\t1) Info log (info.log)\n");
            printf("\t2) Current Change log (cc.log)\n");
            printf("\t3) Manually enter object name \n");
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmGetObjectType(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
        switch(key)
        {
            case '1':
                CsrStrNCpyZero(&inst->object_name[inst->object_name_index], "info.log", SYNC_MAX_OBJECT_NAME_LENGTH - inst->object_name_index);
                inst->object_name_index += CsrStrLen("info.log");
                break;
            case '2':
                CsrStrNCpyZero(&inst->object_name[inst->object_name_index], "cc.log", SYNC_MAX_OBJECT_NAME_LENGTH - inst->object_name_index);
                inst->object_name_index += CsrStrLen("cc.log");
                break;
            case '3':
                inst->keyboardMode = SYNC_KBM_STATE_GET_OBJECT_NAME;
                break;
            default:
                inst->keyboardMode = SYNC_KBM_STATE_GET_OBJECT_TYPE; /* Stay in current state */
                printf("Invalid selection try again\n");
                break;
        }

        if(inst->keyboardMode == SYNC_KBM_STATE_GET_OBJECT_NAME)
        {
            printf("Enter name of oject the object to pull from the store: \"%s\" (NB: Remember the object extension):\n",inst->object_name);
        }
        else if(inst->keyboardMode == SYNC_KBM_STATE_NORMAL)
        {
            transferStart(inst);
        }
        else
        {
            ; /* do nothing */
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmGetobject_name(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        if (RETURN_KEY == key)
        {
            inst->object_name[inst->object_name_index] = '\0';

            printf("\n");
            inst->keyboardMode = SYNC_KBM_STATE_NORMAL;

            transferStart(inst);
        }
        else if (BACKSPACE_KEY == key)
        {
            if (inst->object_name_index > 0)
            {
                inst->object_name_index--;
                printf("\b \b");
            }
        }
        else if ((key >= 32) && (key < 127))
        {
            if (inst->object_name_index < SYNC_MAX_OBJECT_NAME_LENGTH)
            {
                inst->object_name[inst->object_name_index++] = key;
                printf("%c", key);
            }
        }
        else
        {
            printf("Invalid char: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmput_operation(SynccAppInst *inst, CsrUint8 key)
{
    inst->targ_remote = FALSE;
    if(inst->object_name)
    {
        inst->keyboardMode = SYNC_KBM_STATE_PUT_CHANGE_COUNTER;
        switch(key)
        {
            case '1': /* Add */
                inst->put_operation = SYNC_ADD;
                inst->keyboardMode = SYNC_KBM_STATE_NORMAL;

                transferStart(inst);
                break;
            case '2': /* Modify */
                inst->put_operation = SYNC_MODIFY;
                break;
            case '3': /* Delete */
                inst->put_operation = SYNC_DELETE;
                break;
            default:
                inst->keyboardMode = SYNC_KBM_STATE_PUT_OPERATION; /* Stay in current state */
                printf("Invalid selection try again\n");
                break;
        }

        if(inst->keyboardMode == SYNC_KBM_STATE_PUT_CHANGE_COUNTER)
        {
            inst->max_expected_change_counter = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
            CsrMemSet(inst->max_expected_change_counter, 0, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->max_expected_change_counter_index = 0;

            printf("Enter CSRMAX expected change counter value:\n");
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmPutHardSoft(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
        switch(key)
        {
            case '1': /* HARD */
                inst->hard_delete = TRUE;
                break;
            case '2': /* SOFT */
                inst->hard_delete = FALSE;
                break;
            default:
                inst->keyboardMode = SYNC_KBM_STATE_PUT_HARD_SOFT; /* Stay in current state */
                printf("Invalid selection try again\n");
                break;
        }

        if(inst->keyboardMode == SYNC_KBM_STATE_NORMAL)
        {
            transferStart(inst);
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

static void syncKbmPutChangeCounter(SynccAppInst *inst, CsrUint8 key)
{
    if(inst->object_name)
    {
        if (RETURN_KEY == key)
        {
            inst->max_expected_change_counter[inst->max_expected_change_counter_index] = '\0';

            printf("\n");

            if(inst->put_operation == SYNC_MODIFY)
            { /* We got all the info we need start transfer */
                inst->keyboardMode = SYNC_KBM_STATE_NORMAL;

                transferStart(inst);
            }
            else
            {
                inst->keyboardMode = SYNC_KBM_STATE_PUT_HARD_SOFT;

                printf("Select hard or soft delete:\n");
                printf("\t1) Hard delete (%s)\n",inst->object_name);
                printf("\t2) Soft delete (%s)\n",inst->object_name);
            }
        }
        else if (BACKSPACE_KEY == key)
        {
            if (inst->max_expected_change_counter_index > 0)
            {
                inst->max_expected_change_counter_index--;
                printf("\b \b");
            }
        }
        else if ((key >= 32) && (key < 127))
        {
            if (inst->max_expected_change_counter_index < SYNC_MAX_OBJECT_NAME_LENGTH)
            {
                inst->max_expected_change_counter[inst->max_expected_change_counter_index++] = key;
                printf("%c", key);
            }
        }
        else
        {
            printf("Invalid char: 0x%02X (file: %s, line %d)\n", key,__FILE__,__LINE__);
        }
    }
    else
    {
        printf("ERROR: object_name not allocated (file: %s, line %d)\n",__FILE__,__LINE__);
    }
}

/* Start transfer of real time clock from server */
static void getRealTimeClock(SynccAppInst *inst)
{
    inst->targ_remote = TRUE;
    CsrPmemFree(inst->object_name);
    inst->object_name = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
    inst->object_name_index = CsrStrLen(SYNC_REAL_TIME_CLOCK_NAME);
    CsrStrCpy((char *)inst->object_name, SYNC_REAL_TIME_CLOCK_NAME);

    transferStart(inst);
}

/* Start transfer of device info from server */
void getDeviceInfo(SynccAppInst *inst)
{
    inst->targ_remote = TRUE;
    CsrPmemFree(inst->object_name);
    inst->object_name = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
    inst->object_name_index = CsrStrLen(SYNC_DEVICE_INFORMATION_NAME);
    CsrStrCpy((char *)inst->object_name, SYNC_DEVICE_INFORMATION_NAME);

    transferStart(inst);
}

#define SYNC_STORE_MANUAL_ENTRY       0
#define SYNC_STORE_PHONE_BOOK_ENTRY   1
#define SYNC_STORE_CALENDAR_ENTRY     2
#define SYNC_STORE_MESSAGES_ENTRY     3
#define SYNC_STORE_NOTES_ENTRY        4
#define SYNC_STORE_PHONE_BOOK_FULL    5
#define SYNC_STORE_CALENDAR_FULL      6
#define SYNC_STORE_MESSAGES_FULL      7
#define SYNC_STORE_NOTES_FULL         8


/* Start remote obejct selection */
void selectRemote(SynccAppInst *inst, CsrUint8 store)
{
    inst->targ_remote = TRUE;
    inst->download_full_store = FALSE;
    inst->keyboardMode = SYNC_KBM_STATE_GET_STATIC_VS_LUID;
    CsrPmemFree(inst->object_name);
    inst->object_name = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
    inst->object_name_index = 0;

    switch(store)
    {
        case SYNC_STORE_MANUAL_ENTRY:
            printf("Enter name of oject the object to pull from server (NB: Remember the object extension):\n");
            inst->keyboardMode = SYNC_KBM_STATE_GET_OBJECT_NAME;
            break;
        case SYNC_STORE_PHONE_BOOK_ENTRY:
            CsrStrNCpyZero(inst->object_name, SYNC_PHONE_BOOK_OBJECT_STORE, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_PHONE_BOOK_OBJECT_STORE);
            break;
        case SYNC_STORE_CALENDAR_ENTRY:
            CsrStrNCpyZero(inst->object_name, SYNC_CALENDAR_OBJECT_STORE, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_CALENDAR_OBJECT_STORE);
            break;
        case SYNC_STORE_MESSAGES_ENTRY:
            CsrStrNCpyZero(inst->object_name, SYNC_MESSAGING_OBJECT_STORE, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_MESSAGING_OBJECT_STORE);
            inst->keyboardMode = SYNC_KBM_STATE_GET_OBJECT_NAME;
            break;
        case SYNC_STORE_NOTES_ENTRY:
            CsrStrNCpyZero(inst->object_name, SYNC_NOTES_OBJECT_STORE, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_NOTES_OBJECT_STORE);
            break;
        case SYNC_STORE_PHONE_BOOK_FULL:
            inst->download_full_store = TRUE;
            inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
            CsrStrNCpyZero(inst->object_name, SYNC_PHONE_BOOK_NAME, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_PHONE_BOOK_NAME);
            break;
        case SYNC_STORE_CALENDAR_FULL:
            inst->download_full_store = TRUE;
            inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
            CsrStrNCpyZero(inst->object_name, SYNC_CALENDAR_NAME, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_CALENDAR_NAME);
            break;
        case SYNC_STORE_MESSAGES_FULL:
            inst->download_full_store = TRUE;
            CsrStrNCpyZero(inst->object_name, SYNC_MESSAGING_OBJECT_STORE, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_MESSAGING_OBJECT_STORE);
            inst->keyboardMode = SYNC_KBM_STATE_GET_REPOSITORY;
            break;
        case SYNC_STORE_NOTES_FULL:
            inst->download_full_store = TRUE;
            inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
            CsrStrNCpyZero(inst->object_name, SYNC_NOTES_NAME, SYNC_MAX_OBJECT_NAME_LENGTH);
            inst->object_name_index = CsrStrLen(SYNC_NOTES_NAME);
            break;

        default:
            printf("ERROR (file %s, line %d\n",__FILE__,__LINE__);
            break;
    }

    if(inst->keyboardMode == SYNC_KBM_STATE_GET_STATIC_VS_LUID)
    {
        printf("Select static vs. unique (luid) object indexing:\n");
        printf("\t1) static indexing\n");
        printf("\t2) unique indexing\n");
    }
    else if(inst->keyboardMode == SYNC_KBM_STATE_GET_REPOSITORY)
    {
        printf("Select messaging sub store to access:\n");
        printf("\t1) Inbox      (in)\n");
        printf("\t2) Outbox     (out)\n");
        printf("\t3) Sent items (sent)\n");
    }
    else if(inst->keyboardMode == SYNC_KBM_STATE_NORMAL)
    {
        transferStart(inst);
    }
}

/* Delete selected target */
void selectDelete(SynccAppInst *inst)
{
    CsrUint8 *unicode;

    /* Catch local file deletion */
    if(!inst->targ_remote)
    {
        int res;
    if(inst->targ_dir)
    {
        res = 0; /* TODO: recursiveRmDir(inst->targ_name); */
    }
    else
    {
        res = remove(inst->targ_name);
        }
    if(res != 0)
        {
            printf("Could not delete target '%s', error code %d\n",
                   inst->targ_name,
                   res);
        }
        else
        {
            playMenu(inst);
            printf("Local target successfully deleted\n");
        }
    /* Set new target */
    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");

        return;
    }

    /* Request delete of remote file */
    stateChange(inst, STATE_TRANSFER);
    unicode = CsrUtf82Ucs2ByteString((CsrUint8*)inst->targ_name);
    CsrBtSynccDeleteObjectReqSend(TRUE, NULL, unicode);
}

/* Start search */
void startSearch(SynccAppInst *inst)
{
    inst->search_idx = 0;
    stateChange(inst, STATE_SEARCH);

    CsrBtSdSearchReqSend(inst->CsrSchedQid,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0, /* Filter length */
                  NULL); /* Filter */

    playMenu(inst);
}

/* Start connecting */
void startConnecting(SynccAppInst *inst)
{
    if((inst->address.lap == 0) &&
       (inst->address.lap == 0) &&
       (inst->address.lap == 0))
    {
        printf("No device selected for connection\n");
        return;
    }

    stateChange(inst, STATE_CONNECTING);
    CsrBtSynccConnectReqSend(inst->CsrSchedQid,
                             MAX_PACKET_SIZE,
                             (inst->address),
                             FALSE, /*authorise*/
                             0, /* realmLength */
                             NULL, /* realm */
                             0, /* passwordLength */
                             NULL, /* password */
                             NULL, /* userId */
                             0, /* length */
                             0, /* count */
                             0, /* windowSize */
                             TRUE); /* enableSrm */
    playMenu(inst);
}

/* Check for keypresses (timed event) */
void keyboardTimer(CsrUint16 m, void *data)
{
    SynccAppInst *inst;
    inst = (SynccAppInst*)data;

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
void handleSdEvents(SynccAppInst *inst)
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
            if(inst->state == STATE_SEARCH)
            {
                CsrUint16 nameLen;
                CsrUint8 *name;
                printf("\n%02u: ", inst->search_idx);
                nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                if (nameLen > 0)
                {
                    printf("%s\n    ", name);
                    CsrPmemFree(name);
                }
                printf("%04X:%02X:%06X\n    ",
                    prim->deviceAddr.nap,
                    prim->deviceAddr.uap,
                    prim->deviceAddr.lap);
                printf("%s\n",CodDecodeDeviceClass(prim->deviceClass));

                inst->search_list[inst->search_idx] = prim->deviceAddr;
                inst->search_idx++;
            }

            if(inst->search_idx > MAX_DISCOVERY_IN_LIST-1)
            {
                CsrBtSdCancelSearchReqSend(inst->CsrSchedQid);
                stateChange(inst, STATE_CLOSE_SEARCH);
            }
            CsrPmemFree(prim->info);
            break;
        }

        case CSR_BT_SD_CLOSE_SEARCH_IND:
            if(inst->state == STATE_CLOSE_SEARCH)
            {
                if(inst->search_idx > 0)
                {
                    stateChange(inst, STATE_SELECT_ADDR);
                    printf("\nSelect a device from the list: ");
                }
                else
                {
                    stateChange(inst, STATE_IDLE);
                    playMenu(inst);
                }
            }
            break;

        case CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM:
        {
            CsrBtSdReadAvailableServicesCfm *prim;
            prim = (CsrBtSdReadAvailableServicesCfm*)inst->msg;
            if(inst->state == STATE_SERVICE_SEARCH)
            {

                if(inst->state == STATE_SERVICE_SEARCH)
                {
                    CsrUint16 nameLen;
                    CsrUint8 *name;
                    stateChange(inst, STATE_IDLE);
                    playMenu(inst);

                    nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                    printf("Service search for address %04X:%02X:%06X, name %s completed:\n",
                           prim->deviceAddr.nap,
                           prim->deviceAddr.uap,
                           prim->deviceAddr.lap,
                           name);
                    CsrPmemFree(name);

                    if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID))
                    {
                        printf("SYNC service available\n");
                    }
                    else
                    {
                        printf("SYNC not available\n");
                    }
                }
            }
            CsrPmemFree(prim->info);
            break;
        }

        case CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM:
            {
                stateChange(inst, STATE_IDLE);
                playMenu(inst);
                printf("Read available services cancelled\n");
            }
            break;

        default:
            {
                printf("Unhandled SD primitive 0x%04X\n", type);
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg);
                break;
            }
    }
}

/* Handle the SYNCC messages */
void handleSynccEvents(SynccAppInst *inst)
{
    CsrBtSynccPrim type;

    type = *((CsrBtSynccPrim*)inst->msg);
    switch(type)
    {
        case CSR_BT_SYNCC_CONNECT_CFM:
            {
                CsrBtSynccConnectCfm* prim;

                prim = (CsrBtSynccConnectCfm*)inst->msg;
                if(prim->resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && prim->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
                {
                    stateChange(inst, STATE_CONNECTED);
                    playMenu(inst);
                    printf("Connection successful!\n");
                }
                else
                {
                    stateChange(inst, STATE_IDLE);
                    playMenu(inst);
                    printf("Could not connect, OBEX resultCode: 0x%04x, resultSupplier: 0x%04x\n", prim->resultCode, prim->resultSupplier);
                }
                break;
            }

        case CSR_BT_SYNCC_DISCONNECT_IND:
            {
                CsrBtSynccDisconnectInd* prim;

                prim = (CsrBtSynccDisconnectInd*)inst->msg;

                inst->state_old = STATE_IDLE;
                stateChange(inst, STATE_IDLE);
                cleanInstance(inst);
                playMenu(inst);
                printf("Disconnected with reasonCode: 0x%04x, reasonSupplier: 0x%04x\n", prim->reasonCode, prim->reasonSupplier);
                break;
            }

        case CSR_BT_SYNCC_AUTHENTICATE_IND:
            {
                CsrUint16            i;
                CsrBtSynccAuthenticateInd  * prim;

                prim = (CsrBtSynccAuthenticateInd *)inst->msg;

                stateChange(inst, STATE_AUTHENTICATE);

                for(i=0; i<MAX_USERID_PASSWORD_LENGTH; i++)
                {
                    inst->auth_pass[i] = 0;
                    inst->auth_user[i] = 0;
                }
                inst->auth_idx = 0;

                printf("OBEX authentication requested by server. \n");
                printf("\tAllowed peer access mode is: %s\n", (prim->options & 0x02) ? "Read Only" : "Full Access");
                printf("\tRequired authentication parameters: %s\n", (prim->options & 0x01) ? "UserId and Password" : "Password only");

                if(prim->realm)
                {
                    if(prim->realm[0] == 0)
                    {
                        printf("\tRealm charset sent by peer is ASCII and string is: %s\n", &prim->realm[1]);
                    }
                    else if(prim->realm[0] == 0xFF)
                    {
                        CsrCharString *string = CsrUcs2ByteString2Utf8(&prim->realm[1]);
                        printf("\tRealm charset sent by peer is UNICODE and string is: %s\n", string);
                        CsrPmemFree(string);
                    }
                    else if(prim->realm[0] >= 1 && prim->realm[0] <= 9)
                    {
                        printf("\tRealm charset sent by peer is ISO-8859-%d\n", prim->realm[0]);
                    }
                    else
                    {
                        printf("\tRealm charset sent by peer is unknown: %d\n", prim->realm[0]);
                    }
                }

                inst->user_id_required = (prim->options & 0x01);
                if ((prim->options & 0x01))
                {
                    inst->auth_state = AUTH_USERID;

                    printf("Userid (CSRMAX %d characters):\n", CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH);
                }
                else
                {
                    inst->auth_state = AUTH_PASS;

                    printf("Password (CSRMAX %d characters):\n", MAX_USERID_PASSWORD_LENGTH);
                }

                break;
            }

        case CSR_BT_SYNCC_GET_OBJECT_IND:
            {
                CsrBtSynccGetObjectInd *prim;

                prim = (CsrBtSynccGetObjectInd*)inst->msg;

                if(inst->file)
                {
                    /* Data to write */
                    if(prim->bodyOffset > 0)
                    {
                        CsrBtFwrite((prim->payload + prim->bodyOffset),
                               1,
                               prim->bodyLength,
                               inst->file);
                    }

                    /* keep count of data received */
                    inst->file_idx += prim->bodyLength;
                }

#ifdef VERBOSE_BODY_OUTPUT
                printf("\n<body>\n");
                while(prim->bodyOffset < prim->payloadLength)
                    putchar(*((CsrUint8 *)(prim->payload + prim->bodyOffset++)));
                printf("</body>\n");
#endif
                /* Send data request */
                CsrBtSynccGetObjectResSend(FALSE);

                break;
            }

        case CSR_BT_SYNCC_GET_OBJECT_CFM:
            {
                CsrBtSynccGetObjectCfm *prim;

                prim = (CsrBtSynccGetObjectCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* Data to write */
                    if(prim->bodyOffset > 0)
                    {
                        CsrBtFwrite((prim->payload + prim->bodyOffset),
                               1,
                               prim->bodyLength,
                               inst->file);
                    }

                    /* Show transfer speed */
                    inst->file_idx += prim->bodyLength;
                    inst->file_size = inst->file_idx;

                    /* Fully end transfer */
                    transferEnd(inst, TRUE, FALSE);

#ifdef VERBOSE_BODY_OUTPUT
                    printf("\n<body>\n");
                    while(prim->bodyOffset < prim->payloadLength)
                        putchar(*((CsrUint8 *)(prim->payload + prim->bodyOffset++)));
                    printf("</body>\n");
#endif
                }
                else
                {
                    /* Transfer failed so we remove the file again */
                    transferEnd(inst, FALSE, TRUE);
                    printf("Get object body failed, OBEX responseCode 0x%04x\n",
                           prim->responseCode);
                }
                break;
            }


        case CSR_BT_SYNCC_ADD_OBJECT_IND:
            {
                CsrBtSynccAddObjectInd *prim;
                CsrBool finalFlag;
                CsrUint16 length;
                CsrUint8 *body;

                prim = (CsrBtSynccAddObjectInd*)inst->msg;

                /* Read in data */
                body = CsrPmemAlloc(prim->maxAllowedPayloadSize);
                length = CsrBtFread(body,
                               1,
                               prim->maxAllowedPayloadSize,
                               inst->file);

                /* End reached? */
                if(CsrBtIsAtEof(inst->file, inst->file_size))
                {
                    /* Close file now, obj-body handler will end transfer */
                    finalFlag = TRUE;
                    CsrBtFclose(inst->file);
                    inst->file = NULL;
                }
                else
                {
                    finalFlag = FALSE;
                }

                /* Show progress and send data request */
                inst->file_idx += length;
                CsrBtSynccAddObjectResSend(finalFlag,
                                     length,
                                     body);
                transferStats(inst);
                break;
            }

        case CSR_BT_SYNCC_ADD_OBJECT_CFM:
            {
                CsrBtSynccAddObjectCfm *prim;

                prim = (CsrBtSynccAddObjectCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    /* If we used unique indexing the server has assigned the luid so we have to update the local file name with the new luid */
                    char *oldFileName = NULL;
                    char *newFileName = NULL;

                    /* We know that inst->object_name_index is pointing directly to the extension part if we're using unique indexing */
                    if(inst->object_name[inst->object_name_index] == '.' && !CsrMemCmp(&inst->object_name[inst->object_name_index-6],"/luid/",6))
                    {
                        char *tempExtension = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
                        CsrStrCpy(tempExtension, &inst->object_name[inst->object_name_index]);
                        CsrStrCpy(&inst->object_name[inst->object_name_index],prim->luid);
                        inst->object_name_index += CsrStrLen(prim->luid);
                        CsrStrCat(inst->object_name,tempExtension);

                        oldFileName = CsrStrDup(inst->targ_name);
                        newFileName = CsrStrDup(inst->object_name);
                        CsrPmemFree(tempExtension);
                    }

                    transferEnd(inst, TRUE, FALSE);

                    printf("Add object operation succeeded\n");
                    printf("Info returned was \n\tluid:%s\n\tchangeCounter:%s\n\ttimeStamp:%s\n",
                        (prim->luid ? (char *) prim->luid : "N/A"),
                        (prim->changeCounter ? (char *) prim->changeCounter : "N/A"),
                        (prim->timeStamp ? (char *) prim->timeStamp : "N/A"));

                    if(oldFileName && newFileName)
                    {
                        /* Now we rename the old file with the new luid name */
                        printf("We rename: %s to %s\n",oldFileName,newFileName);
                        rename(oldFileName,newFileName);
                        CsrPmemFree(oldFileName);
                        CsrPmemFree(newFileName);
                    }

                }
                else
                {
                    transferEnd(inst, FALSE, FALSE);
                    printf("Put object body failed, OBEX responseCode 0x%04x\n", prim->responseCode);
                }
                break;
            }
        case CSR_BT_SYNCC_MODIFY_OBJECT_IND:
            {
                CsrBtSynccModifyObjectInd *prim;
                CsrBool finalFlag;
                CsrUint16 length;
                CsrUint8 *body;

                prim = (CsrBtSynccModifyObjectInd*)inst->msg;

                /* Read in data */
                body = CsrPmemAlloc(prim->maxAllowedPayloadSize);
                length = CsrBtFread(body,
                               1,
                               prim->maxAllowedPayloadSize,
                               inst->file);

                /* End reached? */
                if(CsrBtIsAtEof(inst->file, inst->file_size))
                {
                    /* Close file now, obj-body handler will end transfer */
                    finalFlag = TRUE;
                    CsrBtFclose(inst->file);
                    inst->file = NULL;
                }
                else
                {
                    finalFlag = FALSE;
                }

                /* Show progress and send data request */
                inst->file_idx += length;
                CsrBtSynccModifyObjectResSend(finalFlag,
                                     length,
                                     body);
                transferStats(inst);
                break;
            }

        case CSR_BT_SYNCC_MODIFY_OBJECT_CFM:
            {
                CsrBtSynccModifyObjectCfm *prim;

                prim = (CsrBtSynccModifyObjectCfm*)inst->msg;

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    transferEnd(inst, TRUE, FALSE);

                    printf("Modify object operation succeeded\n");
                    printf("Info returned was \n\tluid:%s\n\tchangeCounter:%s\n\ttimeStamp:%s\n",
                        (prim->luid ? (char *) prim->luid : "N/A"),
                        (prim->changeCounter ? (char *) prim->changeCounter : "N/A"),
                        (prim->timeStamp ? (char *) prim->timeStamp : "N/A"));

                }
                else
                {
                    transferEnd(inst, FALSE, FALSE);
                    printf("Modify object body failed, OBEX responseCode 0x%04x\n", prim->responseCode);
                }
                break;
            }

        case CSR_BT_SYNCC_DELETE_OBJECT_CFM:
            {
                CsrBtSynccDeleteObjectCfm *prim;

                prim = (CsrBtSynccDeleteObjectCfm*) inst->msg;

                stateChange(inst, STATE_CONNECTED);

                if(prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                {
                    transferEnd(inst, TRUE, TRUE);

                    printf("Delete object operation succeeded\n");
                    printf("Info returned was \n\tluid:%s\n\tchangeCounter:%s\n\ttimeStamp:%s\n",
                        (prim->luid ? (char *) prim->luid : "N/A"),
                        (prim->changeCounter ? (char *) prim->changeCounter : "N/A"),
                        (prim->timeStamp ? (char *) prim->timeStamp : "N/A"));
                }
                else
                {
                    playMenu(inst);
                    printf("Delete object failed, OBEX responseCode 0x%04x\n", prim->responseCode);
                }
                break;
            }

        case CSR_BT_SYNCC_ABORT_CFM:
            {
                CsrBtSynccAbortCfm *prim;

                prim = (CsrBtSynccAbortCfm*)inst->msg;

                /* Cleanup */
                if(inst->file != NULL)
                {
                    CsrBtFclose(inst->file);
                }
                inst->file = NULL;
                inst->file_idx = 0;
                inst->file_size = 0;
                inst->file_time = CsrTimeGet(NULL);

                stateChange(inst, STATE_CONNECTED);
                playMenu(inst);
                printf("Abort confirm received!\n");
                break;
            }

        case CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM:
            {
                CsrBtSynccActivateSyncCommandCfm *prim = inst->msg;
                stateChange(inst, inst->state_old);

                if((prim->resultCode == CSR_BT_RESULT_CODE_OBEX_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_OBEX_PROFILES))
                {
                    inst->syncCommandServiceActivated = TRUE;
                    playMenu(inst);
                    printf("Profile Activated successfully!\n");
                }
                else
                {
                    printf("Profile Activation failed (resultCode=0x04%X, resultSupplier=0x04%X)!\n", prim->resultCode, prim->resultSupplier);
                }
                break;
            }

        case CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM:
            {
                CsrBtSynccDeactivateSyncCommandCfm *prim = inst->msg;
                stateChange(inst, inst->state_old);

                if((prim->resultCode == CSR_BT_RESULT_CODE_OBEX_SUCCESS) && (prim->resultSupplier == CSR_BT_SUPPLIER_OBEX_PROFILES))
                {
                    inst->syncCommandServiceActivated = FALSE;
                    playMenu(inst);
                    printf("Profile Deactivated successfully!\n");
                }
                else
                {
                    printf("Profile Deactivation failed (resultCode=0x04%X, resultSupplier=0x04%X)!\n", prim->resultCode, prim->resultSupplier);
                }
                break;
            }
        case CSR_BT_SYNCC_SYNC_COMMAND_IND:
            {
                CsrBtSynccSyncCommandInd *prim;
                char *objName = NULL;

                prim = (CsrBtSynccSyncCommandInd*)inst->msg;
                if(prim->ucs2nameOffset)
                {
                    CsrUint32 objNameLength = CsrUcs2ByteStrLen(prim->payload + prim->ucs2nameOffset)>>1;

                    if(objNameLength)
                    {
                        objName = CsrUcs2ByteString2Utf8(prim->payload + prim->ucs2nameOffset);
                    }
                }
                printf("The OBEX Sync Server has sent a SYNC Command\n");
                printf("\tObject name received: %s",objName ? objName : "N/A");
                printf("\tLength of the received object: %d\n",prim->lengthOfObject);

#ifdef VERBOSE_BODY_OUTPUT
                printf("\n<body>\n");
                while(prim->bodyOffset < prim->payloadLength)
                    putchar(*((CsrUint8 *)(prim->payload + prim->bodyOffset++)));
                printf("</body>\n");
#endif
                CsrPmemFree(objName);
                CsrBtSynccSyncCommandResSend((CsrUint8)(prim->finalFlag ? CSR_BT_OBEX_SUCCESS_RESPONSE_CODE : CSR_BT_OBEX_CONTINUE_RESPONSE_CODE),
                                             FALSE);
                break;
            }

        case CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND:
            {
                CsrBtSynccSyncCommandAbortInd *prim;
                char *description = NULL;

                prim = (CsrBtSynccSyncCommandAbortInd*)inst->msg;
                if(prim->descriptionOffset)
                {
                    CsrUint32 descriptionLength = CsrUcs2ByteStrLen(prim->payload + prim->descriptionOffset)>>1;

                    if(descriptionLength)
                    {
                        description = (char *)CsrUcs2ByteString2Utf8(prim->payload + prim->descriptionOffset);
                    }
                }

                printf("The OBEX Sync Server has aborted the SYNC Command operation \n");

                if(prim->descriptionLength)
                {
                    printf("\tAbort reason: \"%s\"",description ? description : "N/A");
                }
                CsrPmemFree(description);
                break;
            }

        default:
            {
                printf("Unhandled SYNCC primitive 0x%04X\n", type);
                break;
            }
    }
    CsrBtSynccFreeUpstreamMessageContents(CSR_BT_SYNCC_PRIM, inst->msg);
}

/* Clean instance */
void cleanInstance(SynccAppInst *inst)
{
    CsrPmemFree(inst->targ_name);
    inst->targ_name = CsrStrDup("");

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

    inst->keyboardMode = SYNC_KBM_STATE_NORMAL;
}

static void syncScAppSetFnFini(void *pFiniData)
{
    SynccAppInst *instData = (SynccAppInst *) pFiniData;
    stateChange(instData, instData->state_old);
    system(CLS);
    playMenu(instData);
}

static void syncScAppSetFnInit(void *pInitData)
{
    SynccAppInst *instData = (SynccAppInst *) pInitData;
    if(STATE_CONNECTING == instData->state)
    {
        stateChange(instData, STATE_PAIR_WHILE_CONNECT_ATTEMPT);
    }
    else
    {
        stateChange(instData, STATE_PAIRING);
    }
}

static void* syncScAppSetFnGetMessage(void *pRecvMsgPData)
{
    SynccAppInst *instData  = (SynccAppInst *) pRecvMsgPData;
    void *result          = instData->msg;
    instData->msg         = NULL;
    return result;
}

/* Initialize demo app */
void CsrBtAppInit(void **gash)
{
    SynccAppInst *inst;
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;

    inst = CsrPmemAlloc(sizeof(SynccAppInst));
    CsrMemSet(inst, 0, sizeof(SynccAppInst));
    *gash = inst;

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

    /* Application handle */
    inst->CsrSchedQid = TESTQUEUE;

    /* Setup special instance variables */
    stateChange(inst, STATE_IDLE);
/*    stateChange(inst, STATE_CONNECTED);*/ /* TESTING TESTING TESTING */
#ifdef __linux__
    inst->cls = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    getcwd(inst->cls, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(inst->cls, PATH_SEP);
    CsrStrCat(inst->cls, CLS);
#else
    inst->cls = CsrStrDup("cls");
#endif

    inst->dir_root = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);

    getcwd(inst->dir_root, CSR_BT_MAX_PATH_LENGTH);

    /* create folder structure that matches the spec. */
    CsrBtMkDir("telecom");
    CsrBtMkDir("telecom"PATH_SEP"pb");
    CsrBtMkDir("telecom"PATH_SEP"pb"PATH_SEP"luid");
    CsrBtMkDir("telecom"PATH_SEP"cal");
    CsrBtMkDir("telecom"PATH_SEP"cal"PATH_SEP"luid");
    CsrBtMkDir("telecom"PATH_SEP"nt");
    CsrBtMkDir("telecom"PATH_SEP"nt"PATH_SEP"luid");
    CsrBtMkDir("telecom"PATH_SEP"msg");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"in");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"in"PATH_SEP"luid");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"out");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"out"PATH_SEP"luid");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"sent");
    CsrBtMkDir("telecom"PATH_SEP"msg"PATH_SEP"sent"PATH_SEP"luid");

    /* Clean instance takes care of rest */
    cleanInstance(inst);

    CsrSchedTimerSet(KEY_CHECK_TIMEOUT, keyboardTimer, 0, inst);

    /* Security App Configuration */
    CsrBtScActivateReqSend(inst->CsrSchedQid);
    scAppInitInstanceData(&inst->scInstData);

    scAppSetFnFini(&inst->scInstData, syncScAppSetFnFini, inst);
    scAppSetFnInit(&inst->scInstData, syncScAppSetFnInit, inst);
    scAppSetSecLevelSend(&inst->scInstData, scAppSendProfileSecurityLevelReq, &inst->scInstData);

    scAppSetFnGetMessage(&inst->scInstData, syncScAppSetFnGetMessage, inst);

    scAppSetSecLevelPrim(&inst->scInstData,
                         0,     /* Index 0. Single profile */
                         "SYNCC", CSR_BT_SYNCC_PRIM,
                         CSR_BT_SYNCC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_SYNCC_SECURITY_OUT_REQ);

    playMenu(inst);
}

/* Demo application handler */
void CsrBtAppHandler(void **gash)
{
    SynccAppInst *inst = *gash;

    if(CsrSchedMessageGet(&(inst->event), &(inst->msg)))
    {
        switch (inst->event)
        {
            case KEY_MESSAGE:
                handleKeyboardEvents(inst);
                break;

            case CSR_BT_SYNCC_PRIM:
                {
                    CsrBtSynccPrim *prim = (CsrBtSynccPrim *) inst->msg;
                    switch(*prim)
                    {
                        case CSR_BT_SYNCC_SECURITY_OUT_CFM:
                            handleScSecLevelPrim(&inst->scInstData);
                            break;

                        default:
                            handleSynccEvents(inst);
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
    CsrUint16 msgType=0;
    void *msgData=NULL;
    SynccAppInst *inst;

    inst = *gash;
    printf("Cleaning up...\n");

    /* Get a message from the demoapplication message queue. The message is returned in prim
     * and the event type in eventType */
    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
            case CSR_BT_SYNCC_PRIM:
            {
                CsrBtSynccFreeUpstreamMessageContents(msgType, msgData);
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

    /* Free instance */
    if(inst->file)
    {
        CsrBtFclose(inst->file);
    }
    CsrPmemFree(inst->max_expected_change_counter);
    CsrPmemFree(inst->object_name);
    CsrPmemFree(inst->targ_name);
    CsrPmemFree(inst->dir_local);
    CsrPmemFree(inst->dir_root);
    CsrPmemFree(inst->cls);
    CsrPmemFree(inst);
}
#endif

/* Handle OBEX authentication */
void handleAuthentication(SynccAppInst *inst, char key)
{
    if(key == RETURN_KEY)
    {
        printf("\n");
        if(inst->auth_state == AUTH_USERID)
        {
            inst->auth_state = AUTH_PASS;
            inst->auth_idx = 0;
            printf("Password (CSRMAX %d characters):\n",MAX_USERID_PASSWORD_LENGTH);
        }
        else
        {
            CsrCharString *userId = (inst->user_id_required ? CsrPmemAlloc(CsrStrLen(inst->auth_user)+1) : NULL);
            CsrUint16 passLen = 0;
            CsrCharString *password = CsrPmemAlloc(CsrStrLen(inst->auth_pass)+1);

            if (inst->user_id_required)
            {
                CsrStrCpy((char *)userId, inst->auth_user);
            }
            CsrStrCpy((char *)password, inst->auth_pass);
            if(password != NULL)
            {
                passLen = CsrStrLen((char*)password);
            }

            /* Go back to previous state */
            inst->state = inst->state_old;
            printf("Sending authentication reply...\n");

            CsrBtSynccAuthenticateResSend(password, passLen, userId);

            /* Update start time for transfers */
            inst->file_time = CsrTimeGet(NULL);
        }
    }
    else if (key == BACKSPACE_KEY)
    {
        if (inst->auth_idx > 0)
        {
            inst->auth_idx--;
            printf("\b \b");
        }
    }
    else if((inst->auth_state == AUTH_USERID) &&
            (inst->auth_idx < CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH))
    {
        inst->auth_user[inst->auth_idx++] = key;
        printf("%c", key);
    }
    else if((inst->auth_state == AUTH_PASS) &&
            (inst->auth_idx < MAX_USERID_PASSWORD_LENGTH))
    {
        inst->auth_pass[inst->auth_idx++] = key;
        printf("%c", key);
    }
}

/* Handle keypresses */
void handleKeyboardEvents(SynccAppInst *inst)
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

    /* State-based catching of keys */
    switch(inst->state)
    {
        case STATE_IDLE:
            switch(key->key)
            {
                case 'a':
                    stateChange(inst, STATE_ACTIVATING);
                    if(inst->syncCommandServiceActivated)
                    {
                        CsrBtSynccDeactivateSyncCommandReqSend();
                    }
                    else
                    {
                        CsrBtSynccActivateSyncCommandReqSend(inst->CsrSchedQid, MAX_PACKET_SIZE);
                    }
                    return;

                case 's':
                    startSearch(inst);
                    return;

                case 'p':
                    handleScKeybPrim(&inst->scInstData);
                    return;

                case 'c':
                    startConnecting(inst);
                    return;
            }
            break;

        case STATE_SEARCH:
            switch(key->key)
            {
                case 's':
                    /* Stop search */
                    printf("Stopping search...\n");
                    stateChange(inst, STATE_CLOSE_SEARCH);
                    CsrBtSdCancelSearchReqSend(inst->CsrSchedQid);
                    return;
            }
            break;

        case STATE_SELECT_ADDR:
            {
                if((key->key - '0') < inst->search_idx)
                {
                    printf("%c\n", key->key);
                    inst->address = inst->search_list[(key->key - '0')];
                    stateChange(inst, STATE_SERVICE_SEARCH);
                    printf("Searching for services on selected device...\n");
                    CsrBtSdReadAvailableServicesReqSend(inst->CsrSchedQid,
                                                   inst->address,
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
                        CsrBtSynccCancelConnectReqSend();
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
                        CsrBtSynccCancelConnectReqSend();
                        return;
                }
            }
            break;

        case STATE_CONNECTED:

            switch (inst->keyboardMode)
            {
                case SYNC_KBM_STATE_NORMAL:
                {
                    switch(key->key)
                    {
                        case 'a':
                            stateChange(inst, STATE_ACTIVATING);
                            if(inst->syncCommandServiceActivated)
                            {
                                CsrBtSynccDeactivateSyncCommandReqSend();
                            }
                            else
                            {
                                CsrBtSynccActivateSyncCommandReqSend(inst->CsrSchedQid, MAX_PACKET_SIZE);
                            }
                            return;

                        case 'd':
                            stateChange(inst, STATE_DISCONNECTING);
                            CsrBtSynccDisconnectReqSend(TRUE);
                            return;

                        case 'l': /*printf("  l) Select local object to ADD/MODIFY/DELETE \n");*/
                            CsrPmemFree(inst->object_name);
                            inst->object_name = CsrPmemAlloc(SYNC_MAX_OBJECT_NAME_LENGTH);
                            CsrStrNCpyZero(inst->object_name, "telecom/", SYNC_MAX_OBJECT_NAME_LENGTH);
                            inst->object_name_index = CsrStrLen("telecom/");

                            selectLocal(inst);
                            return;

                        case 'm': /*printf("  m) Get device information of the server \n");*/
                            getDeviceInfo(inst);
                            return;

                        case 'n': /*printf("  n) Get real time clock of the server \n");*/
                            getRealTimeClock(inst);
                            return;

                        case 'p': /*printf("  p) Get object from \"phone book\" store \n");*/
                            selectRemote(inst, SYNC_STORE_PHONE_BOOK_FULL);
                            return;

                        case 'q': /*printf("  q) Get object from \"message\" store \n");*/
                            selectRemote(inst, SYNC_STORE_MESSAGES_FULL);
                            return;

                        case 't': /*printf("  t) Get object from \"calender\" store \n");*/
                            selectRemote(inst, SYNC_STORE_CALENDAR_FULL);
                            return;

                        case 'u': /*printf("  u) Get object from \"notes\" store\n");*/
                            selectRemote(inst, SYNC_STORE_NOTES_FULL);
                            return;


                        case '1': /*printf("  p) Get object from \"phone book\" store \n");*/
                            selectRemote(inst, SYNC_STORE_PHONE_BOOK_ENTRY);
                            return;

                        case '2': /*printf("  q) Get object from \"message\" store \n");*/
                            selectRemote(inst, SYNC_STORE_MESSAGES_ENTRY);
                            return;

                        case '3': /*printf("  t) Get object from \"calender\" store \n");*/
                            selectRemote(inst, SYNC_STORE_CALENDAR_ENTRY);
                            return;

                        case '4': /*printf("  u) Get object from \"notes\" store\n");*/
                            selectRemote(inst, SYNC_STORE_NOTES_ENTRY);
                            return;

                        case '5': /*printf("  o) Get a manually specified object from the server \n");*/
                            selectRemote(inst, SYNC_STORE_MANUAL_ENTRY);
                            return;
                    }

                    break;
                }
                case SYNC_KBM_STATE_GET_REPOSITORY:
                {
                    syncKbmGetRepository(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_GET_STATIC_VS_LUID:
                {
                    syncKbmGetStaticVsLuid(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_GET_OBJECT_TYPE:
                {
                    syncKbmGetObjectType(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_GET_OBJECT_NAME:
                {
                    syncKbmGetobject_name(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_PUT_OPERATION:
                {
                    syncKbmput_operation(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_PUT_CHANGE_COUNTER:
                {
                    syncKbmPutChangeCounter(inst, key->key);
                    break;
                }
                case SYNC_KBM_STATE_PUT_HARD_SOFT:
                {
                    syncKbmPutHardSoft(inst, key->key);
                    break;
                }
                default:
                {
                    printf("Invalid Keyboard Mode %d (file: %s, line %d)\n",inst->keyboardMode, __FILE__,__LINE__);
                    break;
                }
            }
            break;

        case STATE_SELECT_LOCAL:
            {
                if(key->key == ' ')
                {
                    /* Ignore spaced */
                }
                if(key->key == 'c')
                {
                    stateChange(inst, STATE_CONNECTED);
                    selectDirectory(inst, inst->dir_root);
                    playMenu(inst);
                }
                if(key->key == 'n')
                {
                    if(inst->dir_count - inst->dir_index + MAX_DISCOVERY_IN_LIST > 0)
                    {
                        inst->dir_index += MAX_DISCOVERY_IN_LIST;
                    }
                    playBrowser(inst);
                    return;
                }
                else if(key->key == 'p')
                {
                    if(inst->dir_index < MAX_DISCOVERY_IN_LIST)
                    {
                        inst->dir_index = 0;
                    }
                    else
                    {
                        inst->dir_index -= MAX_DISCOVERY_IN_LIST;
                    }
                    playBrowser(inst);
                    return;
                }
                else if((key->key >= '0') && (key->key <= '9') && (inst->dir_index + key->key - '0' < inst->dir_count))
                {
                    char *name;
                    CsrBool dir;

                    inst->dir_index = inst->dir_index + key->key - '0';
                    name = browserPeek(inst, inst->dir_index)->name;
                    dir = browserPeek(inst, inst->dir_index)->is_dir;

                    CsrStrCpy(&inst->object_name[inst->object_name_index],name);
                    inst->object_name_index += CsrStrLen(name);

                    if(dir)
                    {
                        if(!CsrStrCmp(name,".") || !CsrStrCmp(name,".."))
                        {
                            if(inst->object_name_index >= CsrStrLen(name))
                            {
                                inst->object_name_index -= CsrStrLen(name) + 1; /* +1 to skip the '/' */
                            }
                            while(inst->object_name_index--)
                            {
                                if(inst->object_name[inst->object_name_index] == '/')
                                {
                                    inst->object_name[++inst->object_name_index] = 0;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            inst->object_name[inst->object_name_index++] = '/';
                        }
                        selectDirectory(inst, name);
                        selectLocal(inst);
                    }
                    else
                    {
                        inst->keyboardMode = SYNC_KBM_STATE_PUT_OPERATION;
                        stateChange(inst, STATE_CONNECTED);

                        printf("Select operation to perform with the selected object:%s\n",inst->object_name);
                        printf("\t1) Add\n");
                        printf("\t2) Modify\n");
                        printf("\t3) Delete\n");

                    }
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
                        CsrBtSynccAbortReqSend();
                        printf("Abort request sent...\n");
                    }
                    break;
            }
            break;


        case STATE_AUTHENTICATE:
            {
                handleAuthentication(inst, key->key);
                return;
            }
            break;

        case STATE_SERVICE_SEARCH:
            switch(key->key)
            {
                case 's':
                {
                    CsrBtSdCancelReadAvailableServicesReqSend(inst->CsrSchedQid);
                    return;
                }
            }
            break;

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
