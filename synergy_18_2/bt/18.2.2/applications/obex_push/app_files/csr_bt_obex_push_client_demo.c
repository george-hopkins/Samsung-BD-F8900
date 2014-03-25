/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "csr_bt_tasks.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_obex.h"
#include "csr_bt_opc_prim.h"
#include "csr_bt_opc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_app_main.h"

#ifdef CSR_AMP_ENABLE
#define MOVE_TO_BREDR                   0
#define MOVE_TO_AMP                     1
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_ampm_prim.h"
#include "csr_bt_ampm_lib.h"
#ifdef CSR_BT_APP_AMP_WIFI
#include "csr_wifi_sme_prim.h"
#include "csr_wifi_sme_lib.h"
#if ((WIFI_MAJOR_VERSION == 3) || (WIFI_MAJOR_VERSION == 4))
#include "csr_wifi_mib_repository.h"
#else
#include "csr_wifi_file_buffer_get.h"
#endif
#endif
#endif

#define PUSH_DIR                       "pushfiles"PATH_SEP""
#define INBOX_DIR                      "inbox"PATH_SEP""
#define VCARD                          "own_card.vcf"
#define VCAL                           "own_cal.vcs"
#define VMSG                           "own_msg.vmg"
#define VNOTE                          "own_note.vnt"
#define VCARD1                         "own_card1.vcf"
#define VCAL1                          "own_cal1.vcs"
#define VMSG1                          "own_msg1.vmg"
#define VNOTE1                         "own_note1.vnt"
#define VCARD2                         "own_card2.vcf"
#define VOBJECTS                       "two_vobjects.vcf"
#define TEXT_VCARD                     ((CsrUint8*)"text/x-vcard")
#define TEXT_VMESSAGE                  ((CsrUint8*)"text/x-vmessage")
#define TEXT_VNOTE                     ((CsrUint8*)"text/x-vnote")
#define TEXT_VCALENDAR                 ((CsrUint8*)"text/x-vcalendar")

#define KEYB_CHECK_TIMEOUT             (100000)
#define OPC_MAX_PACKET_SIZE            (8000)

#define IDLE_KEYBOARD_STATE            0
#define PASSKEY_KEYBOARD_STATE         1
#define MODE_SELECT_KEYBOARD_STATE     2
#define DEVICE_SELECT_KEYBOARD_STATE   3
#define OBJECT_SELECT_KEYBOARD_STATE   4
#define PUT_KEYBOARD_STATE             5
#define ENTER_KEYBOARD_STATE           6

#define SUB_STATE_IDLE                 0
#define SUB_STATE_CONNECT              1
#define SUB_STATE_CONNECTED            2

#define PUT_MODE                       1
#define GET_MODE                       2
#define EXCHANGE_MODE                  3

#define DEFAULT_PP_INQ_TX_POWER        -70  /* Inquiry Transmit Power Level to use for Proximity Pairing */
#define DEFAULT_PP_RSSI_LEVEL          -40  /* RSSI threshold level to use for Proximity Pairing */

typedef struct
{
    CsrBtDeviceAddr    bdAddr;
} ProximityEntry;

typedef struct
{
    CsrSchedQid      CsrSchedQid;
    CsrUint16        event;
    void             *message;
    CsrUint8         keyboardState;
    CsrUint8         prevKeyboardState;
    CsrUint8         exchangeMode;
    CsrUint8         currentNumberInProximity;
    CsrUint8         nextProximityEntryIndex;
    CsrBtDeviceAddr  proximityBdAddress[10];
    CsrBtDeviceName  proximityNames[10];
    CsrUint32        proximityCod[10];
    CsrUint8         pushObjectType;
    CsrUint16        selectedDevice;
    CsrBool          putTwoObj;
    CsrBool          putTwoCardSinglePut;
    CsrBool          putTwoObjSinglePut;
    CsrFileHandle    *currentFileHandle;
    CsrUint32        currentFileSize;
    CsrUint32        currentFilePosition;
    CsrUint16        obexPeerMaxPacketSize;
    CsrBool          disconnectWithOutSave;
    char             szFilename[CSR_BT_MAX_PATH_LENGTH];
    CsrUint16        nFilenameLen;
    CsrUint8         subState;
    ScAppInstData_t  scInstData;
    CsrBool          proximityPairing;
    CsrBool          searchCancelled;
    CsrBtConnId      btConnId;
    CsrBool          srmSupported;
    CsrUint8         srmpLeftToSend;
    CsrBool          abortSend;
} PushClientInstanceDataType;


#if (WIFI_MAJOR_VERSION >= 5)
static CsrCharString mibFileName[1000];
#endif


typedef void (*OpcDemoStateHandlerType)(PushClientInstanceDataType * pInst);
static void objectSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter);
static void modeSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter);
static void deviceSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter);
static void enterFilenameKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter);
static void escapeKeyHandler(PushClientInstanceDataType * pInst);

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

static void showMenu(PushClientInstanceDataType *pInst)
{
    char objName[50];

    system(CLS);
    printf("\nOBEX PUSH Client:\n\n");
    printf("Default VCard: %s\n\n", VCARD);
    printf("Hit <ESC> to quit program!\n\n");

    if (pInst->pushObjectType == 1) CsrStrCpy(objName, "vCard");
    else if (pInst->pushObjectType == 2) CsrStrCpy(objName, "vCal");
    else if (pInst->pushObjectType == 3) CsrStrCpy(objName, "vMsg");
    else CsrStrCpy(objName, "vNote");

    printf("\nProgram options:\n\n");

    printf("    0) Put default %s using Proximity Pairing\n", objName);
#ifdef CSR_BT_INSTALL_OPC_GET
    printf("    1) Get default %s using Proximity Pairing\n", objName);
#endif
    printf("    2) Put default %s\n", objName);
#ifdef CSR_BT_INSTALL_OPC_GET
    printf("    3) Get default vCard\n");
    printf("    4) Exchange vCards\n");
#endif
    printf("    5) Put two vCard items using a single PUT operation\n");
    printf("    t) Put two vObjects items using a single PUT operation\n");
    printf("    6) Select object type\n");
    printf("    7) Select 'put two objects'\n");
    printf("    8) Security Controller Configuration\n");
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (pInst->subState == SUB_STATE_IDLE)
    {
        if (pInst->srmSupported)
        {
            printf("    9) Disable Single Response Mode\n");
        }
        else
        {
            printf("    9) Enable Single Response Mode\n");
        }
        if (pInst->srmpLeftToSend == 2)
        {
            printf("    a) Disable single Response Mode Parameter\n");
        }
        else
        {
            printf("    a) Set Single Response Mode Parameter to %d\n       (hit twice for PTS test SRMP-BV05-C)\n", pInst->srmpLeftToSend + 1);
        }
    }
#endif
    printf("\n");
    printf("    x) Force disconnect\n");

    pInst->keyboardState = MODE_SELECT_KEYBOARD_STATE;
}

static void putObject(const char *objectName,
                      CsrUint8 *objectType,
                      CsrUint32 objectTypeLength,
                      PushClientInstanceDataType *pInst)
{
    char       filename[128];
    int        result;
    CsrBtStat_t statBuffer;
    CsrUint8    *unicodeName;
    CsrUint8    *objPtr = NULL;

    CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
    CsrMemSet(filename, 0, sizeof(filename));

    sprintf(filename, "%s%s", PUSH_DIR, objectName);

    result = CsrBtStat(filename, &statBuffer);
    if (result)
    {
        printf("Failed to get information about the file (%s)!\n", filename);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    pInst->currentFileHandle = CsrBtFopen(filename, "rb");
    if (pInst->currentFileHandle == NULL)
    {
        printf("Failed to open file (%s)!\n", filename);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    pInst->currentFilePosition = 0;
    pInst->currentFileSize = statBuffer.st_size;

    /* Create the unicode name, which we get in a new buffer */
    unicodeName = CsrUtf82Ucs2ByteString((CsrUint8*)objectName);
    if (objectType)
    {
        objPtr = CsrPmemAlloc(objectTypeLength);
        CsrMemCpy(objPtr,objectType,objectTypeLength);
    }
    /* We transfer ownership of both objectType and unicodeName */
    CsrBtOpcPutReqSend(objPtr,
                  (CsrUint16)objectTypeLength,
                  pInst->currentFileSize,
                  unicodeName);

    printf("Putting %s...\n", filename);
    printf("Press A to abort transfer\n\n");

}

static void putVcard(PushClientInstanceDataType *pInst)
{
    if (pInst->putTwoObj == TRUE)
    {
        putObject(VCARD, TEXT_VCARD, CsrStrLen((char*)TEXT_VCARD) + 1, pInst);
    }
    else if (pInst->putTwoCardSinglePut)
    {
        putObject(VCARD2, TEXT_VCARD, CsrStrLen((char*)TEXT_VCARD) + 1, pInst);
    }
    else if (pInst->putTwoObjSinglePut)
    {
        putObject(VOBJECTS, TEXT_VCARD, CsrStrLen((char*)TEXT_VCARD) + 1, pInst);
    }
    else 
    {
        putObject(VCARD1, TEXT_VCARD, CsrStrLen((char*)TEXT_VCARD) + 1, pInst);
    }
}

static void putVcal(PushClientInstanceDataType *pInst)
{
    if (pInst->putTwoObj == TRUE)
    {
        putObject(VCAL, TEXT_VCALENDAR, CsrStrLen((char*)TEXT_VCALENDAR) + 1, pInst);
    }
    else
    {
        putObject(VCAL1, TEXT_VCALENDAR, CsrStrLen((char*)TEXT_VCALENDAR) + 1, pInst);
    }
}

static void putVmsg(PushClientInstanceDataType *pInst)
{
    if (pInst->putTwoObj == TRUE)
    {
        putObject(VMSG, TEXT_VMESSAGE, CsrStrLen((char*)TEXT_VMESSAGE) + 1, pInst);
    }
    else
    {
        putObject(VMSG1, TEXT_VMESSAGE, CsrStrLen((char*)TEXT_VMESSAGE) + 1, pInst);
    }
}

static void putVnote(PushClientInstanceDataType *pInst)
{
    if (pInst->putTwoObj == TRUE)
    {
        putObject(VNOTE, TEXT_VNOTE, CsrStrLen((char*)TEXT_VNOTE) + 1, pInst);
    }
    else
    {
        putObject(VNOTE1, TEXT_VNOTE, CsrStrLen((char*)TEXT_VNOTE) + 1, pInst);
    }
}

static void putAnyType(PushClientInstanceDataType *pInst)
{
    putObject(pInst->szFilename,
              NULL,
              0,
              pInst);
}

static void putMsg(PushClientInstanceDataType *pInst)
{
    if (pInst->pushObjectType == 1)
    {
        putVcard(pInst);
    }
    else if (pInst->pushObjectType == 2)
    {
        putVcal(pInst);
    }
    else if (pInst->pushObjectType == 3)
    {
        putVmsg(pInst);
    }
    else if (pInst->pushObjectType == 4)
    {
        putVnote(pInst);
    }
    else
    {
        putAnyType(pInst);
    }
}

static void SdCloseSearchIndHandler(PushClientInstanceDataType * pInst)
{
    if (!pInst->proximityPairing)
    {
        system(CLS);
        printf("Press 9 to Cancel Connect !!! \n");
    }

    if (pInst->searchCancelled)
    {
        system(CLS);
        showMenu(pInst);
        pInst->searchCancelled = FALSE;
    }
    else
    {
        pInst->subState = SUB_STATE_CONNECT;

        CsrBtOpcConnectReqSend(pInst->CsrSchedQid,
                               OPC_MAX_PACKET_SIZE,
                               pInst->proximityBdAddress[pInst->selectedDevice],
                               0, /* length */
                               0, /* count */
                               0, /* windowSize */
                               pInst->srmSupported);
    }
}

static void SdSearchResultIndHandler(PushClientInstanceDataType * pInst)
{
    int index;
    CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *) pInst->message;

    if (pInst->proximityPairing)
    {
        CsrUint16 nameLen;
        CsrUint8 *name;
        pInst->proximityBdAddress[0] = prim->deviceAddr;
        nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
        if (nameLen > 0)
        {
            CsrUtf8StrNCpy(pInst->proximityNames[0], (CsrUtf8String *) name, CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
            CsrUtf8StrTruncate(pInst->proximityNames[0], CSR_BT_MAX_FRIENDLY_NAME_LEN);
        }
        else
        {
            pInst->proximityNames[0][0] = '\0';
        }

        printf("\nPushing object to %s   %04X:%02X:%06X...\n", (char *) name,
                                                            prim->deviceAddr.nap,
                                                            prim->deviceAddr.uap,
                                                            prim->deviceAddr.lap);
        pInst->selectedDevice = 0;
        CsrBtSdCancelSearchReqSend(pInst->CsrSchedQid);
    }
    else if (pInst->keyboardState == DEVICE_SELECT_KEYBOARD_STATE)
    {
        for (index = 0; index < pInst->currentNumberInProximity; index++)
        {
            if ((pInst->proximityBdAddress[index].nap == prim->deviceAddr.nap)
                && (pInst->proximityBdAddress[index].uap == prim->deviceAddr.uap)
                && (pInst->proximityBdAddress[index].lap == prim->deviceAddr.lap))
            {
                break;
            }
        }
        if (index >= pInst->currentNumberInProximity)
        {
            CsrUint16 nameLen;
            CsrUint8 *name;
            pInst->proximityBdAddress[pInst->nextProximityEntryIndex] = prim->deviceAddr;
            nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
            if (nameLen > 0)
            {
                CsrUtf8StrNCpy(pInst->proximityNames[pInst->nextProximityEntryIndex], (CsrUtf8String *) name, CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
                CsrUtf8StrTruncate(pInst->proximityNames[pInst->nextProximityEntryIndex], CSR_BT_MAX_FRIENDLY_NAME_LEN);
            }
            else
            {
                pInst->proximityNames[pInst->nextProximityEntryIndex][0] = '\0';
            }
            pInst->proximityCod[pInst->nextProximityEntryIndex] = prim->deviceClass;
            if (pInst->currentNumberInProximity < 10)
            {
                pInst->currentNumberInProximity++;
            }

            printf("\n%u: ", pInst->nextProximityEntryIndex);
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

            pInst->nextProximityEntryIndex++;
            if (pInst->nextProximityEntryIndex >= 10)
            {
                pInst->nextProximityEntryIndex = 0;
            }
        }
    }
    CsrPmemFree(prim->info);
}

static void OpcConnectCfmHandler(PushClientInstanceDataType * pInst)
{
    CsrBtOpcConnectCfm * prim;

    prim = (CsrBtOpcConnectCfm *) pInst->message;
    system(CLS);

    pInst->proximityPairing = FALSE;

    if (prim->resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
        prim->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        pInst->obexPeerMaxPacketSize = prim->maxPeerPacketSize;
        pInst->btConnId = prim->btConnId;

#ifdef CSR_AMP_ENABLE
        printf("Registering with AMP manager...\n");
        CsrBtAmpmRegisterReqSend(TESTQUEUE,
                                 0, /* flags */
                                 CSR_BT_AMPM_EVENTS_ENABLE_ALL, /* eventMask */
                                 pInst->btConnId, /* btConnId */
                                 &pInst->proximityBdAddress[pInst->selectedDevice]); /* addr */
#endif

#ifdef CSR_AMP_ENABLE
        printf(" Connected on btConnId 0x%08x\n", prim->btConnId);
        printf("  M) Move channel to AMP\n");
        printf("  B) Move channel to BR/EDR\n");
#endif

        printf("Supported formats in the server:\n");
        if(prim->supportedFormats & CSR_BT_VCARD_2_1_SUPPORT)
        {
            printf(" -Vcard 2.1\n");
        }
        if(prim->supportedFormats & CSR_BT_VCARD_3_0_SUPPORT)
        {
            printf(" -Vcard 3.0\n");
        }
        if(prim->supportedFormats & CSR_BT_VCAL_1_0_SUPPORT)
        {
            printf(" -Vcal 1.0\n");
        }
        if(prim->supportedFormats & CSR_BT_ICAL_2_0_SUPPORT)
        {
            printf(" -Ical 2.0\n");
        }
        if(prim->supportedFormats & CSR_BT_VNOTE_SUPPORT)
        {
            printf(" -Vnote\n");
        }
        if(prim->supportedFormats & CSR_BT_VMESSAGE_SUPPORT)
        {
            printf(" -Vmessage\n");
        }
        if(prim->supportedFormats & CSR_BT_OTHER_TYPE_SUPPORT)
        {
            printf(" -Any other type of object\n");
        }

        if ((pInst->exchangeMode == PUT_MODE) || (pInst->exchangeMode == EXCHANGE_MODE))
        {
            putMsg(pInst);
            pInst->keyboardState = PUT_KEYBOARD_STATE;
        }
        else
        {
#ifdef CSR_BT_INSTALL_OPC_GET
            printf("Getting default VCard... srmp = %d \n", pInst->srmpLeftToSend);
            if (pInst->srmpLeftToSend)
            {
                pInst->srmpLeftToSend--;
                CsrBtOpcGetHeaderReqSend(CSR_BT_VCARD_TYPE, TRUE);
            }
            else
            {
                CsrBtOpcGetHeaderReqSend(CSR_BT_VCARD_TYPE, FALSE);
            }
#else
            printf("Get VCard unsupported without the CSR_BT_INSTALL_OPC_GET flag.\n");
#endif          
            pInst->keyboardState = IDLE_KEYBOARD_STATE;
        }
        pInst->subState = SUB_STATE_CONNECTED;
    }
    else
    {
        pInst->subState = SUB_STATE_IDLE;
        printf("\nConnect failed - resultCode: 0x%04X, resultSupplier: 0x%04X\n", prim->resultCode, prim->resultSupplier);
        showMenu(pInst);
    }
}

static void OpcAbortCfmHandler(PushClientInstanceDataType * pInst)
{
    if (pInst->currentFileHandle)
    {
        CsrBtFclose(pInst->currentFileHandle);
        pInst->currentFileHandle = NULL;
    }
    printf("The current operation is aborted\n");
    showMenu(pInst);
    pInst->abortSend = FALSE;    
}

static void OpcDisconnectIndHandler(PushClientInstanceDataType * pInst)
{
    CsrBtOpcDisconnectInd * prim = (CsrBtOpcDisconnectInd *) pInst->message;
    if (pInst->currentFileHandle)
    {
        CsrBtFclose(pInst->currentFileHandle);
        pInst->currentFileHandle = NULL;
    }
    pInst->subState  = SUB_STATE_IDLE;
    printf("The Obex connection is disconnected with reasonCode: 0x%04X, reasonSupplier: 0x%04X\n\n", prim->reasonCode, prim->reasonSupplier);
    showMenu(pInst);
}

static void OpcPutCfmHandler(PushClientInstanceDataType * pInst)
{
    CsrBtOpcPutCfm *prim    = pInst->message;

    if (pInst->currentFileHandle)
    {
        CsrBtFclose(pInst->currentFileHandle);
        pInst->currentFileHandle = NULL;
    }
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("Put succeeded!\n");
    }
    else
    {
        printf("Put failed - error code: %02X!\n", prim->responseCode);
    }
    if (pInst->exchangeMode == EXCHANGE_MODE)
    {
#ifdef CSR_BT_INSTALL_OPC_GET
        printf("Getting default VCard... srmp = %d \n", pInst->srmpLeftToSend);
        if (pInst->srmpLeftToSend)
        {
            pInst->srmpLeftToSend--;
            CsrBtOpcGetHeaderReqSend(CSR_BT_VCARD_TYPE, TRUE);
        }
        else
        {
            CsrBtOpcGetHeaderReqSend(CSR_BT_VCARD_TYPE, FALSE);
        }
#else
        printf("Get VCard unsupported without the CSR_BT_INSTALL_OPC_GET flag.\n");
#endif          
    }
    else if (pInst->putTwoObj == TRUE)
    {
        pInst->putTwoObj = FALSE;
        putMsg(pInst);
    }
    else
    {   
        if (pInst->putTwoCardSinglePut == TRUE)
        {
            pInst->putTwoCardSinglePut = FALSE;
        }
        if (pInst->putTwoObjSinglePut == TRUE)
        {
            pInst->putTwoObjSinglePut = FALSE;
        }

        printf("Disconnecting, please wait...\n\n\n");
        pInst->keyboardState = IDLE_KEYBOARD_STATE;
        CsrBtOpcDisconnectReqSend(TRUE);
    }
}

static void OpcPutObjectIndHandler(PushClientInstanceDataType * pInst)
{
    CsrBtOpcPutObjectInd *prim = pInst->message;
    CsrUint32 bytesRead = 0;
    CsrUint32 bytesToRead;
    CsrBool finalFlag = FALSE;
    CsrUint8  *buffer;

    if( pInst->abortSend != TRUE)   
    {
        if (pInst->currentFileSize - pInst->currentFilePosition <= prim->objectLength)
        {
            bytesToRead = pInst->currentFileSize - pInst->currentFilePosition;
        }
        else
        {
            bytesToRead = prim->objectLength;
        }

        buffer = CsrPmemAlloc(bytesToRead);
        if (bytesToRead > 0)
        {
            bytesRead = CsrBtFread(buffer, 1, bytesToRead, pInst->currentFileHandle);
            pInst->currentFilePosition += bytesRead;
        }
        if (pInst->currentFilePosition >= pInst->currentFileSize)
        {
            if (pInst->currentFileHandle)
            {
                CsrBtFclose(pInst->currentFileHandle);
                pInst->currentFileHandle = NULL;
            }
            finalFlag = TRUE;
        }

        printf("\rSending %d bytes... (%d of %d now sent)", bytesToRead, pInst->currentFilePosition, pInst->currentFileSize);
        /* We transfer ownership of the buffer! */
        CsrBtOpcPutObjectResSend(finalFlag, (CsrUint16)bytesRead, buffer);
    }
}

#ifdef CSR_BT_INSTALL_OPC_GET
static void OpcGetHeaderCfmHandler(PushClientInstanceDataType * pInst)
{
    CsrUint8 *utf8Name;
    char *fileName;
    CsrBtOpcGetHeaderCfm *prim = pInst->message;

    if (prim->ucs2nameOffset)
    {
        utf8Name = CsrUcs2ByteString2Utf8(prim->payload + prim->ucs2nameOffset);
        fileName = CsrPmemAlloc(((CsrUint16) CsrStrLen((char*)utf8Name) + 8));
        sprintf(fileName, "%s%s", INBOX_DIR, utf8Name);
    }
    else
    {
        if ( (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE) || (prim->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE) )
        {
            printf("No file name included. Using default name: default_received\n");
            utf8Name = CsrStrDup("default_received");
            
            fileName = CsrPmemAlloc(CsrStrLen((char*)utf8Name) + 8);
            sprintf(fileName, "%s%s", INBOX_DIR, utf8Name);
        }
        else
        {
            printf("Get failed - error code: 0x%02X!\n", prim->responseCode);
            CsrBtOpcDisconnectReqSend(TRUE);
            CsrPmemFree(prim->payload);
            return;
        }
    }
    switch (prim->responseCode)
    {
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
            {
                pInst->currentFileHandle = CsrBtFopen(fileName, "wb");
                if (pInst->currentFileHandle)
                {
                    CsrBtFwrite((prim->payload + prim->objectBodyOffset),
                           1, prim->objectBodyLength, pInst->currentFileHandle);
                    CsrBtFclose(pInst->currentFileHandle);
                    pInst->currentFileHandle = NULL;
                    printf("Saved \"%s\" in Inbox. filesize %d\n", utf8Name, prim->totalObjectSize);
                }
                else
                {
                    printf("Failed to save object in inbox (%s)!\n", utf8Name);
                }
                CsrBtOpcDisconnectReqSend(TRUE);
                break;
            }
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
            {
                pInst->currentFileHandle = CsrBtFopen(fileName, "wb");

                if (pInst->currentFileHandle)
                {
                    CsrBtFwrite((prim->payload + prim->objectBodyOffset),
                                1, prim->objectBodyLength,
                                pInst->currentFileHandle);
                    printf("Saving \"%s\" in Inbox. filesize %d, srmp = %d\n", utf8Name, prim->totalObjectSize, pInst->srmpLeftToSend);
                    if (pInst->srmpLeftToSend)
                    {
                        pInst->srmpLeftToSend--;
                        CsrBtOpcGetObjectReqSend(TRUE);
                    }
                    else
                    {
                        CsrBtOpcGetObjectReqSend(FALSE);
                    }

                }
                else
                {
                    printf("Failed to save object in inbox (%s)!\n", utf8Name);
                }
                break;
            }
        default:
            {
                printf("Get failed - error code: 0x%02X!\n", prim->responseCode);
                CsrBtOpcDisconnectReqSend(TRUE);
                break;
            }
    }
    CsrPmemFree(fileName);
    CsrPmemFree(utf8Name);
    CsrPmemFree(prim->payload);
}
static void OpcGetObjectCfmHandler(PushClientInstanceDataType * pInst)
{
    CsrBtOpcGetObjectCfm    * prim = pInst->message;

    switch (prim->responseCode)
    {
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
            {
                CsrBtFwrite((prim->payload + prim->objectBodyOffset),
                       1, prim->objectBodyLength, pInst->currentFileHandle);
                CsrBtFclose(pInst->currentFileHandle);
                pInst->currentFileHandle = NULL;
                printf("Saving done.\n");
                CsrBtOpcDisconnectReqSend(TRUE);
                break;
            }
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
            {
                if( pInst->abortSend != TRUE)   
                {
                    CsrBtFwrite((prim->payload + prim->objectBodyOffset),
                           1, prim->objectBodyLength, pInst->currentFileHandle);
                    if (pInst->srmpLeftToSend)
                    {
                        pInst->srmpLeftToSend--;
                        CsrBtOpcGetObjectReqSend(TRUE);
                    }
                    else
                    {
                        CsrBtOpcGetObjectReqSend(FALSE);
                    }
                }
                else
                {
                    CsrBtOpcDisconnectReqSend(TRUE);
                }
                break;
            }
        default:
            {
                printf("Get failed - error code: 0x%02X!\n", prim->responseCode);
                CsrBtOpcDisconnectReqSend(TRUE);
                break;
            }
    }
    CsrPmemFree(prim->payload);
}
#endif

static const OpcDemoStateHandlerType opcDemoStateHandlers[1][CSR_BT_OPC_PRIM_UPSTREAM_COUNT] =
{
    {
        OpcConnectCfmHandler,          /* CSR_BT_OPC_CONNECT_CFM */
        OpcAbortCfmHandler,            /* CSR_BT_OPC_ABORT_CFM */
        OpcDisconnectIndHandler,       /* CSR_BT_OPC_DISCONNECT_IND */
        OpcPutCfmHandler,              /* CSR_BT_OPC_PUT_CFM */
        OpcPutObjectIndHandler,        /* CSR_BT_OPC_PUT_OBJECT_IND */
#ifdef CSR_BT_INSTALL_OPC_GET
        OpcGetHeaderCfmHandler,        /* CSR_BT_OPC_GET_HEADER_CFM */
        OpcGetObjectCfmHandler,        /* CSR_BT_OPC_GET_OBJECT_CFM */
#else
        NULL,
        NULL,
#endif
    }
};

static void oppScAppSetFnFini(void *pFiniData)
{
    PushClientInstanceDataType *instData = (PushClientInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
    system(CLS);
    showMenu(instData);
}

static void oppScAppSetFnInit(void *pInitData)
{
    PushClientInstanceDataType *instData = (PushClientInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, PASSKEY_KEYBOARD_STATE);
}

static void* oppScAppSetFnGetMessage(void *pRecvMsgPData)
{
    PushClientInstanceDataType *instData = (PushClientInstanceDataType *) pRecvMsgPData;
    void *result                         = instData->message;
    instData->message                    = NULL;
    return result;
}

void CsrBtAppInit(void ** gash)
{
    PushClientInstanceDataType *pInst;
    CsrFileHandle * ifp;
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;
    *gash   = CsrPmemAlloc(sizeof(PushClientInstanceDataType));
    pInst   = *gash;
    pInst->currentNumberInProximity    = 0;
    pInst->nextProximityEntryIndex = 0;
    pInst->pushObjectType = 1;
    pInst->putTwoObj = FALSE;
    pInst->putTwoCardSinglePut = FALSE;
    pInst->putTwoObjSinglePut = FALSE;
    pInst->selectedDevice = 0;
    pInst->currentFileHandle = NULL;
    pInst->szFilename[0] = 0;
    pInst->nFilenameLen = 0;
    pInst->subState     = SUB_STATE_IDLE;
    pInst->proximityPairing = FALSE;
    pInst->searchCancelled = FALSE;
    pInst->srmSupported = TRUE;
    pInst->srmpLeftToSend = 0;
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    pInst->abortSend = FALSE;    

    ifp = CsrBtFopen("ObexPushClientDemo.cfg", "rb");
    CsrBtBdAddrZero(&zeroAddr);
    if (ifp != NULL)
    {
        int index;

        if (CsrBtFread(&pInst->currentNumberInProximity, sizeof(CsrUint8), 1, ifp) == 1)
        {
            for (index = 0; index < pInst->currentNumberInProximity; index++)
            {
                CsrBtFread(&pInst->proximityBdAddress[index], sizeof(CsrBtDeviceAddr), 1, ifp);
                CsrBtFread(pInst->proximityNames[index], sizeof(CsrBtDeviceName), 1, ifp);
                CsrBtFread(&pInst->proximityCod[index], sizeof(CsrUint32), 1, ifp);
            }

            pInst->nextProximityEntryIndex = pInst->currentNumberInProximity;
            if (pInst->nextProximityEntryIndex == 10)
            {
                pInst->nextProximityEntryIndex = 0;
            }

            if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
            {
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].lap = defGlobalBdAddr.lap;
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].nap = defGlobalBdAddr.nap;
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].uap = defGlobalBdAddr.uap;
                CsrStrCpy(pInst->proximityNames[pInst->nextProximityEntryIndex], "Address from command line");
                pInst->proximityCod[pInst->nextProximityEntryIndex] = 0;
                pInst->nextProximityEntryIndex = pInst->nextProximityEntryIndex + 1;
                pInst->currentNumberInProximity = pInst->currentNumberInProximity + 1;
                if (pInst->nextProximityEntryIndex == 10)
                {
                    pInst->nextProximityEntryIndex = 0;
                }
            }
        }
        else
        {
            pInst->currentNumberInProximity = 0;
            if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
            {
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].lap = defGlobalBdAddr.lap;
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].nap = defGlobalBdAddr.nap;
                pInst->proximityBdAddress[pInst->nextProximityEntryIndex].uap = defGlobalBdAddr.uap;
                CsrStrCpy(pInst->proximityNames[pInst->nextProximityEntryIndex], "Address from command line");
                pInst->proximityCod[pInst->nextProximityEntryIndex] = 0;
                pInst->nextProximityEntryIndex = pInst->nextProximityEntryIndex + 1;
                pInst->currentNumberInProximity = pInst->currentNumberInProximity + 1;
                if (pInst->nextProximityEntryIndex == 10)
                {
                    pInst->nextProximityEntryIndex = 0;
                }
            }
        }
        CsrBtFclose(ifp);
    }
    else
    {
        if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
        {
            pInst->proximityBdAddress[pInst->nextProximityEntryIndex].lap = defGlobalBdAddr.lap;
            pInst->proximityBdAddress[pInst->nextProximityEntryIndex].nap = defGlobalBdAddr.nap;
            pInst->proximityBdAddress[pInst->nextProximityEntryIndex].uap = defGlobalBdAddr.uap;
            CsrStrCpy(pInst->proximityNames[pInst->nextProximityEntryIndex], "Address from command line");
            pInst->proximityCod[pInst->nextProximityEntryIndex] = 0;
            pInst->nextProximityEntryIndex = pInst->nextProximityEntryIndex + 1;
            pInst->currentNumberInProximity = pInst->currentNumberInProximity + 1;
            if (pInst->nextProximityEntryIndex == 10)
            {
                pInst->nextProximityEntryIndex = 0;
            }
        }
    }

    /* Application handle */
    pInst->CsrSchedQid = TESTQUEUE;

    showMenu(pInst);

    /* Security App Configuration */
    CsrBtScActivateReqSend(pInst->CsrSchedQid);
    scAppInitInstanceData(&pInst->scInstData);

    scAppSetFnFini(&pInst->scInstData, oppScAppSetFnFini, pInst);
    scAppSetFnInit(&pInst->scInstData, oppScAppSetFnInit, pInst);
    scAppSetSecLevelSend(&pInst->scInstData, scAppSendProfileSecurityLevelReq, &pInst->scInstData);

    scAppSetFnGetMessage(&pInst->scInstData, oppScAppSetFnGetMessage, pInst);

    scAppSetSecLevelPrim(&pInst->scInstData,
                         0,     /* Index 0. Single profile */
                         "OPC", CSR_BT_OPC_PRIM,
                         CSR_BT_OPC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_OPC_SECURITY_OUT_REQ);

#ifdef CSR_AMP_ENABLE
#ifdef CSR_BT_APP_AMP_WIFI
    CsrWifiSmeActivateReqSend(TESTQUEUE);
#endif /* amp-wifi */
#endif /* amp-enable */
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    eventType=0;
    void        *prim=NULL;
    PushClientInstanceDataType * pInst = (PushClientInstanceDataType *) *gash;

    scAppDeinitInstanceData(&pInst->scInstData);

    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType                                                        */
    while(CsrSchedMessageGet(&eventType, &prim))
    {
        switch(eventType)
        {
            case CSR_BT_OPC_PRIM:
                CsrBtOpcFreeUpstreamMessageContents(eventType, prim);
                break;
            case CSR_BT_SD_PRIM:
                CsrBtSdFreeUpstreamMessageContents(eventType, prim);
                break;
            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(eventType, prim);
                break;
        }
        CsrPmemFree(prim);
    }
    if ( pInst->currentFileHandle)
    {
        CsrBtFclose(pInst->currentFileHandle);
    }

    CsrPmemFree(pInst);
}
#endif

void checkSpecialKeys(CsrUint8 key)
{
}

#if defined(CSR_AMP_ENABLE) && defined(CSR_BT_APP_AMP_WIFI)
static void handleWifiSme(PushClientInstanceDataType *inst)
{
  switch(*(CsrUint16*)inst->message)
    {
    case CSR_WIFI_SME_ACTIVATE_CFM:
      {
          CsrWifiSmeActivateCfm *prim = (CsrWifiSmeActivateCfm*)inst->message;
          CsrWifiSmeDataBlock *mibFiles = NULL;
          CsrUint16 mibFilesCount = 0;
          CsrWifiMacAddress macAddress;
    
          printf("Received CSR_WIFI_SME_ACTIVATE_CFM, code 0x%04x\n",
           prim->status);
#undef CSR_WIFI_USE_EEPROM_MIB
#define CSR_WIFI_MAC_ADDRESS 0x00025B0ABCDE
#define CSR_WIFI_DESIGN_M2107_V3
    /* MAC */

/* MAC */
#ifdef CSR_WIFI_MAC_ADDRESS
    macAddress.a[0] = (CSR_WIFI_MAC_ADDRESS >> 40) & 0xFF;
    macAddress.a[1] = (CSR_WIFI_MAC_ADDRESS >> 32) & 0xFF;
    macAddress.a[2] = (CSR_WIFI_MAC_ADDRESS >> 24) & 0xFF;
    macAddress.a[3] = (CSR_WIFI_MAC_ADDRESS >> 16) & 0xFF;
    macAddress.a[4] = (CSR_WIFI_MAC_ADDRESS >> 8) & 0xFF;
    macAddress.a[5] = (CSR_WIFI_MAC_ADDRESS >> 0) & 0xFF;
#else
    CsrMemSet(macAddress.a, 0xFF, 6);
#endif
    
    /*Handle the MIB File */
#if !defined(CSR_WIFI_USE_EEPROM_MIB)
    mibFilesCount = 1;
    mibFiles = CsrPmemAlloc(sizeof(CsrWifiSmeDataBlock));
    
#if (WIFI_MAJOR_VERSION >= 5)
    if (strlen(mibFileName) == 0)
      {
        printf("A Mib File is needed. Please specify one\n");
        
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif /* ENABLE_SHUTDOWN */
        
      }
    else
      {
        CsrSize length = 0;
        CsrUint8 *buffer = NULL;
        CsrResult result = CsrWifiFileBufferGet(mibFileName,&length,&buffer);
        
        /* Make sure the length will fit in 16 bits */
        if (result == CSR_RESULT_SUCCESS && length>0 && length<0x10000)
          {
                  mibFiles->length=(CsrUint16)length;
                  mibFiles->data=buffer;
          }
        else
          {
        printf("MIB file load failed\n");

        
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif /* ENABLE_SHUTDOWN */
          }
      }
#else 
#if defined(CSR_WIFI_DESIGN_M2150_V4)
    mibFiles->length = CsrWifiMibRepositoryDesignM2150V4Length;
    mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2150V4, mibFiles->length);
#elif defined(CSR_WIFI_DESIGN_M2107_V3)
    mibFiles->length = CsrWifiMibRepositoryDesignM2107R02Length;
    mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2107R02, mibFiles->length);
#elif defined(CSR_WIFI_DESIGN_M2256_V1)
    mibFiles->length = CsrWifiMibRepositoryDesignM2256V1Length;
    mibFiles->data = CsrMemCpy(CsrPmemAlloc(mibFiles->length), CsrWifiMibRepositoryDesignM2256V1, mibFiles->length);
#else
#error Unknown Chip. Need to add code to choose correct MIB file, or enable CSR_WIFI_USE_EEPROM_MIB.
#endif /* #if defined(CSR_WIFI_DESIGN_M2150_V4) */
#endif /* WIFI_MAJOR_VERSION >= 5 */
#endif /* #if !defined(CSR_WIFI_USE_EEPROM_MIB) */
    
    printf("Sending WIFI ON request (macAddr is %02X:%02X:%02X:%02X:%02X:%02X)\n",
           macAddress.a[0],
           macAddress.a[1],
           macAddress.a[2],
           macAddress.a[3],
           macAddress.a[4],
           macAddress.a[5]);
    printf("                        (mibFilesCount is %d)\n", mibFilesCount);
    CsrWifiSmeWifiOnReqSend(TESTQUEUE, macAddress, mibFilesCount, mibFiles);
    break;
      }
    case CSR_WIFI_SME_WIFI_ON_CFM:
      {
    
    CsrWifiSmeWifiOnCfm *prim = (CsrWifiSmeWifiOnCfm*)inst->message;
    printf("Received CSR_WIFI_SME_WIFI_ON_CFM, code 0x%04x\n",prim->status);
    break;
      }
      
    default:
      printf("Unknown AMP primitive, 0x%04x\n", *(CsrUint16*)inst->message);
      break;
    }
}
#endif

/* Start AMP move channel */
#ifdef CSR_AMP_ENABLE
static void startMoveChannel(PushClientInstanceDataType *inst, CsrUint8 target)
{
    if(inst->btConnId == CSR_BT_CONN_ID_INVALID)
    {
        printf("OBEX Push is not using OBEX-over-L2CAP, so move channel not possible\n");
        return;
    }

    if(target == MOVE_TO_BREDR)
    {
        printf("Initiate move to BR/EDR controller...\n");
        CsrBtAmpmMoveReqSend(inst->btConnId,
                             CSR_BT_AMP_CONTROLLER_BREDR,
                             CSR_BT_AMP_CONTROLLER_BREDR);
    }
    else if(target == MOVE_TO_AMP)
    {
        printf("Requesting controllers from AMPM...\n");
        CsrBtAmpmControllerReqSend(TESTQUEUE,
                                   &(inst->proximityBdAddress[inst->selectedDevice]),
                                   inst->btConnId);
    }
}
#endif

/* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
static void handleAmpmPrim(PushClientInstanceDataType *inst)
{
    switch(*(CsrBtAmpmPrim*)inst->message)
    {
        case CSR_BT_AMPM_REGISTER_CFM:
            {
                CsrBtAmpmRegisterCfm *prim;
                prim = (CsrBtAmpmRegisterCfm*)inst->message;
                printf("AMPM register confirm: btConnId 0x%08x, result 0x%04x, supplier 0x%04x\n",
                       prim->btConnId, prim->resultCode, prim->resultSupplier);
            }
            break;

        case CSR_BT_AMPM_DEREGISTER_CFM:
            {
                CsrBtAmpmDeregisterCfm *prim;
                prim = (CsrBtAmpmDeregisterCfm*)inst->message;
                printf("AMPM unregister confirm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_CONTROLLER_IND:
        case CSR_BT_AMPM_CONTROLLER_CFM:
            {
                CsrBtAmpmControllerInd *prim;
                CsrUint8 i;

                prim = (CsrBtAmpmControllerInd*)inst->message;
                printf("AMP controller listing (supplier 0x%04x error code 0x%04x)\n",
                       prim->resultSupplier, prim->resultCode);

                for(i = 0; i < prim->ampsCount; i++)
                {
                    printf("  ampType:0x%02x status:0x%02x caps:0x%08x localId:0x%04x remoteId:0x%04x\n",
                           prim->amps[i].ampType,
                           prim->amps[i].status,
                           prim->amps[i].caps,
                           prim->amps[i].localId,
                           prim->amps[i].remoteId);
                }
                printf("\n");

                if((prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) &&
                   (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS))
                {
                    printf("Initiating move to controller 0x%04x/0x%04x...\n",
                           prim->amps[0].localId, prim->amps[0].remoteId);
                    CsrBtAmpmMoveReqSend(inst->btConnId,
                                         prim->amps[0].localId, prim->amps[0].remoteId);
                }
                else
                {
                    printf("Could not initiate move\n");
                }

                CsrPmemFree(prim->amps);
                prim->amps = NULL;
            }
            break;

        case CSR_BT_AMPM_MOVE_IND:
            {
                CsrBtAmpmMoveInd *prim;
                prim = (CsrBtAmpmMoveInd*)inst->message;
                printf("AMP move channel indication, btConnId 0x%08x, localId 0x%04x - accepting...\n",
                       prim->btConnId, prim->localId);
                CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
            }
            break;


        case CSR_BT_AMPM_MOVE_CMP_IND:
            {
                CsrBtAmpmMoveCmpInd *prim;
                prim = (CsrBtAmpmMoveCmpInd*)inst->message;
                printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_MOVE_CFM:
            {
                CsrBtAmpmMoveCfm *prim;
                prim = (CsrBtAmpmMoveCfm*)inst->message;
                printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        default:
            printf("Unhandled AMPM primitive received: type 0x%04x\n",
                   *(CsrPrim*)inst->message);
            break;
    }
}
#endif

void CsrBtAppHandler(void ** gash)
{
    PushClientInstanceDataType * pInst = *gash;

    CsrSchedMessageGet(&pInst->event, &pInst->message);

    switch (pInst->event)
    {
        case CSR_BT_OPC_PRIM:
            {
                CsrBtOpcPrim    * type;

                type = (CsrBtOpcPrim    *) pInst->message;
                if (*type == CSR_BT_OPC_SECURITY_OUT_CFM)
                {
                    handleScSecLevelPrim(&pInst->scInstData);
                }
                else if (((*type - CSR_BT_OPC_PRIM_UPSTREAM_LOWEST) < CSR_BT_OPC_PRIM_UPSTREAM_COUNT ) &&
                    opcDemoStateHandlers[0][(CsrUint16) (*type - CSR_BT_OPC_PRIM_UPSTREAM_LOWEST)] != NULL)
                {
                    opcDemoStateHandlers[0][(CsrUint16) (*type - CSR_BT_OPC_PRIM_UPSTREAM_LOWEST)](pInst);
                }
                else
                {
                    printf("A state event error occurred in the OPC Application layer,\n  it is a CSR_BT_OPC_PRIM type: %02X\n\n",
                           *type);
                    CsrBtOpcFreeUpstreamMessageContents(pInst->event, pInst->message);
                }
                break;
            }

        case CSR_BT_SD_PRIM:
            {
                CsrPrim * sdPrim = pInst->message;

                switch (*sdPrim)
                {
                    case CSR_BT_SD_SEARCH_RESULT_IND:
                        {
                            SdSearchResultIndHandler(pInst);
                            break;
                        }
                    case CSR_BT_SD_CLOSE_SEARCH_IND:
                        {
                            SdCloseSearchIndHandler(pInst);
                            break;
                        }
                    default:
                        {
                            printf("Unknown sdPrim type in CsrBtAppHandler: %02X\n\n", *sdPrim);
                            CsrBtSdFreeUpstreamMessageContents(pInst->event, pInst->message);
                            break;
                        }
                }
                break;
            }
        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;
                key = (CsrBtKeyPress_t*)pInst->message;

                if (key->key == ESCAPE_KEY)
                {
                    escapeKeyHandler(pInst);
                }
                else if (pInst->subState == SUB_STATE_CONNECT && (key->key == '9') &&
                         pInst->keyboardState  != PASSKEY_KEYBOARD_STATE)
                {
                    CsrBtOpcCancelConnectReqSend();
                }
                else
                {
                    switch (pInst->keyboardState)
                    {
                        case PASSKEY_KEYBOARD_STATE:
                            {
                                handleScKeybPrim(&pInst->scInstData);
                                break;
                            }
                        case OBJECT_SELECT_KEYBOARD_STATE:
                            {
                                objectSelectKeyboardStateHandler(pInst, key->key);
                                break;
                            }
                        case DEVICE_SELECT_KEYBOARD_STATE:
                            {
                                checkSpecialKeys(key->key);
                                deviceSelectKeyboardStateHandler(pInst, key->key);
                                break;
                            }
                        case ENTER_KEYBOARD_STATE:
                            {
                                enterFilenameKeyboardStateHandler(pInst, key->key);;
                                break;
                            }
                        case MODE_SELECT_KEYBOARD_STATE:
                            {
                                if((key->key == 'X') || (key->key == 'x'))
                                {
                                    CsrBtOpcDisconnectReqSend(TRUE);
                                    printf("Disconnecting, please wait...\n\n\n");
                                    pInst->keyboardState = IDLE_KEYBOARD_STATE;
                                }
                                else
                                {
                                    modeSelectKeyboardStateHandler(pInst, key->key);
                                }
                                break;
                            }
                        case PUT_KEYBOARD_STATE:
                            {
                                if((key->key == 'A') || (key->key == 'a'))
                                {
                                    CsrBtOpcAbortReqSend();
                                    printf("Aborting, please wait...\n\n\n");
                                    pInst->keyboardState = IDLE_KEYBOARD_STATE;
                                    pInst->abortSend = TRUE;
                                }
                                break;
                            }
                        case IDLE_KEYBOARD_STATE:
                            {
#ifdef CSR_AMP_ENABLE
                                if(key->key == 'M')
                                {
                                    startMoveChannel(pInst, MOVE_TO_AMP);
                                }
                                else if(key->key == 'B')
                                {
                                    startMoveChannel(pInst, MOVE_TO_BREDR);
                                }
#endif
                                break;
                            }
                    }
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&pInst->scInstData);
                break;
            }

#ifdef CSR_AMP_ENABLE
        case CSR_BT_AMPM_PRIM:
            handleAmpmPrim(pInst);
            break;
#endif

#if defined(CSR_AMP_ENABLE) && defined(CSR_BT_APP_AMP_WIFI)
        case CSR_WIFI_SME_PRIM:
            handleWifiSme(pInst);
            break;
#endif

        default:
            printf("Unhandled event in default handler: 0x%04X\n", pInst->event);
            break;
    }
    CsrPmemFree(pInst->message);
}

static void escapeKeyHandler(PushClientInstanceDataType * pInst)
{
    CsrFileHandle *ofp;
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;

    printf("\nUser exit...\n");
    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        pInst->currentNumberInProximity--;
    }
    if (pInst->currentNumberInProximity)
    {
        ofp = CsrBtFopen("ObexPushClientDemo.cfg", "wb");
        if (ofp != NULL)
        {
            int index;

            CsrBtFwrite(&pInst->currentNumberInProximity, sizeof(CsrUint8), 1, ofp);
            if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
            {
                pInst->currentNumberInProximity++;
            }
            for (index = 0; index < pInst->currentNumberInProximity; index++)
            {
                if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
                {
                    if (CsrBtBdAddrEq(&defGlobalBdAddr,&(pInst->proximityBdAddress[index])) )
                    {
                        /* Do nothing, the commandline param will not be stored */
                    }
                    else
                    {
                        CsrBtFwrite(&pInst->proximityBdAddress[index], sizeof(CsrBtDeviceAddr), 1, ofp);
                        CsrBtFwrite(pInst->proximityNames[index], sizeof(CsrBtDeviceName), 1, ofp);
                        CsrBtFwrite(&pInst->proximityCod[index], sizeof(CsrUint32), 1, ofp);
                    }
                }
                else
                {
                    CsrBtFwrite(&pInst->proximityBdAddress[index], sizeof(CsrBtDeviceAddr), 1, ofp);
                    CsrBtFwrite(pInst->proximityNames[index], sizeof(CsrBtDeviceName), 1, ofp);
                    CsrBtFwrite(&pInst->proximityCod[index], sizeof(CsrUint32), 1, ofp);
                }
            }
            CsrBtFclose(ofp);
        }
    }
    if (pInst->currentFileHandle)
    {
        CsrBtFclose(pInst->currentFileHandle);
    }

#ifdef ENABLE_SHUTDOWN
    CsrSchedStop();
#else
    exit(0);
#endif
}

static void objectSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter)
{
    CsrBool isShow = TRUE;

    pInst->pushObjectType = theCharacter - '0';
    switch (pInst->pushObjectType)
    {
        case 1:
            printf("vCard selected\n");
            break;
        case 2:
            printf("vCal selected\n");
            break;
        case 3:
            printf("vMsg selected\n");
            break;
        case 4:
            printf("vNote selected\n");
            break;
        default:
            printf("Any type selected\n\nEnter file name: ");
            pInst->szFilename[0] = 0;
            pInst->nFilenameLen = 0;
            pInst->keyboardState = ENTER_KEYBOARD_STATE;
            isShow = FALSE;
            break;
    }
    if (isShow)
    {
        showMenu(pInst);
    }
}

static void enterFilenameKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        if (0 == pInst->nFilenameLen)
        {
            printf("No file\n");
        }
        pInst->keyboardState = MODE_SELECT_KEYBOARD_STATE;
        pInst->szFilename[pInst->nFilenameLen] = '\0';
        showMenu(pInst);
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (pInst->nFilenameLen > 0)
        {
            pInst->nFilenameLen--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (pInst->nFilenameLen < CSR_BT_MAX_PATH_LENGTH - 1)
        {
            pInst->szFilename[pInst->nFilenameLen++] = theCharacter;
            printf("%c", theCharacter);
        }
    }
}

static void modeSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter)
{
    int     index;
    char    objName[50];

    if      (pInst->pushObjectType == 1) CsrStrCpy(objName, "vCard");
    else if (pInst->pushObjectType == 2) CsrStrCpy(objName, "vCal");
    else if (pInst->pushObjectType == 3) CsrStrCpy(objName, "vMsg");
    else    CsrStrCpy(objName, "vNote");

    switch (theCharacter)
    {
        case '0':
            {
                pInst->exchangeMode     = PUT_MODE;
                pInst->keyboardState    = DEVICE_SELECT_KEYBOARD_STATE;
                pInst->proximityPairing = TRUE;
                scAppSetProximityPairing(&pInst->scInstData, TRUE);
                printf("\n\nUsing Proximity Pairing to put %s...\n", objName);
                printf("Inquiry TX power level is %d and RSSI threshold level is %d\n", DEFAULT_PP_INQ_TX_POWER, DEFAULT_PP_RSSI_LEVEL);
                CsrBtSdProximitySearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              4000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              DEFAULT_PP_RSSI_LEVEL, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL, /* Filter */
                              DEFAULT_PP_INQ_TX_POWER); /* Inquiry Transmit power level */
                break;
            }
#ifdef CSR_BT_INSTALL_OPC_GET
        case '1':
            {
                pInst->exchangeMode = GET_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                pInst->proximityPairing = TRUE;
                scAppSetProximityPairing(&pInst->scInstData, TRUE);
                printf("\n\nUsing Proximity Pairing to get %s...\n", objName);
                printf("Inquiry TX power level is %d and RSSI threshold level is %d\n", DEFAULT_PP_INQ_TX_POWER, DEFAULT_PP_RSSI_LEVEL);
                CsrBtSdProximitySearchReqSend(pInst->CsrSchedQid,
                                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                                  4000, /* RSSI buffer timeout */
                                  CSR_BT_INFINITE_TIME, /* Search timeout */
                                  DEFAULT_PP_RSSI_LEVEL, /* RSSI threshold */
                                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                                  0, /* Filter length */
                                  NULL, /* Filter */
                                  DEFAULT_PP_INQ_TX_POWER); /* Inquiry Transmit power level */
                break;
            }
#endif      
        case '2':
            {
                pInst->exchangeMode     = PUT_MODE;
                pInst->keyboardState    = DEVICE_SELECT_KEYBOARD_STATE;
                scAppSetProximityPairing(&pInst->scInstData, FALSE);
                printf("Put mode chosen...\n");
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
#ifdef CSR_BT_INSTALL_OPC_GET
        case '3':
            {
                pInst->exchangeMode = GET_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                printf("Get mode chosen...\n");
                scAppSetProximityPairing(&pInst->scInstData, FALSE);
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
        case '4':
            {
                pInst->exchangeMode = EXCHANGE_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                pInst->pushObjectType = 1;
        scAppSetProximityPairing(&pInst->scInstData, FALSE);
                printf("Exchange mode chosen...\n");
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
#endif      
        case '5':
            {
                pInst->putTwoCardSinglePut = TRUE;
                printf("\nPut of two vCards selected for next single put operation\n");
                pInst->exchangeMode = PUT_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                scAppSetProximityPairing(&pInst->scInstData, FALSE);
                printf("Put mode chosen...\n");
                pInst->pushObjectType = 1;
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
        case 't':
            {
                pInst->putTwoObjSinglePut = TRUE;
                printf("\nPut of two objects selected for next single put operation\n");
                pInst->exchangeMode = PUT_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                scAppSetProximityPairing(&pInst->scInstData, FALSE);
                printf("Put mode chosen...\n");
                pInst->pushObjectType = 1;
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
        case '6':
            {
                pInst->keyboardState = OBJECT_SELECT_KEYBOARD_STATE;
                printf("\nSelect the wanted object type to push:\n\n");
                printf("    1) vCard\n    2) vCal\n    3) vMsg\n    4) vNote\n    5) any type\n");
                return;
            }
        case '7':
            {
                pInst->putTwoObj = TRUE;
                printf("\nPut of two objects selected for next put operation\n");
                pInst->exchangeMode = PUT_MODE;
                pInst->keyboardState = DEVICE_SELECT_KEYBOARD_STATE;
                scAppSetProximityPairing(&pInst->scInstData, FALSE);
                printf("Put mode chosen...\n");
                CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                              CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                              6000, /* RSSI buffer timeout */
                              CSR_BT_INFINITE_TIME, /* Search timeout */
                              CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                              CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                              CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                              0, /* Filter length */
                              NULL); /* Filter */
                break;
            }
        case '8':
        {
            handleScKeybPrim(&pInst->scInstData);
            return;
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        case '9':
        {
            if (pInst->subState == SUB_STATE_IDLE)
            {
                if (pInst->srmSupported)
                {
                    pInst->srmSupported = FALSE;
                    printf("Single Response Mode is now disable\n");
                }
                else
                {
                    pInst->srmSupported = TRUE;
                    printf("Single Response Mode is now enable\n");
                }
            }
            return;
        }
        case 'a':
        case 'A':
        {
            if (pInst->subState == SUB_STATE_IDLE)
            {
                pInst->srmpLeftToSend = (pInst->srmpLeftToSend + 1)%3;
                if (pInst->srmpLeftToSend == 0)
                {
                    printf("Single Response Mode Parameter is now disable\n");
                }
                else
                {
                    printf("Single Response Mode Parameter is now set to %d\n", pInst->srmpLeftToSend);
                }
            }
            return;
        }
#endif
    }

    if (pInst->proximityPairing)
    {
        printf("\nPress c to cancel search\n\n");
    }
    else
    {
        printf("\nHit the number corresponding to the device to exchange information with:\n\n");
        for (index = 0; index < pInst->currentNumberInProximity; index++)
        {
            printf("\n%u: ", index);
            if (*(pInst->proximityNames[index]) != '\0')
            {
                printf("%s\n   ", pInst->proximityNames[index]);
            }
            printf("%04X:%02X:%06X\n   ",
                   pInst->proximityBdAddress[index].nap,
                   pInst->proximityBdAddress[index].uap,
                   pInst->proximityBdAddress[index].lap);
            printf("%s\n",CodDecodeDeviceClass(pInst->proximityCod[index]));
        }
    }
}

static void deviceSelectKeyboardStateHandler(PushClientInstanceDataType * pInst, CsrUint8 theCharacter)
{
    if (pInst->proximityPairing &&
       (theCharacter == 'c' || theCharacter == 'C'))
    {
        pInst->searchCancelled = TRUE;
        printf("\nCancelling search...\n");
        CsrBtSdCancelSearchReqSend(pInst->CsrSchedQid);
    }
    else if ((theCharacter >= '0') && (theCharacter <= '9'))
    {
        if (pInst->currentNumberInProximity && (pInst->currentNumberInProximity >= (theCharacter - '0')))
        {
            pInst->selectedDevice = (CsrUint16) (theCharacter - '0');
            CsrBtSdCancelSearchReqSend(pInst->CsrSchedQid);
        }
    }
}

void applicationUsage(char *col, char *des, char *opt)
{
#if (WIFI_MAJOR_VERSION >= 5)
  strcat(col,"[-Z <MIB file>]");
  sprintf(opt,"\t-Z <MIB file>\t\tMIB file to use\n");
  strcat(des,opt);
#else
#endif
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
#if (WIFI_MAJOR_VERSION >= 5)
  if (ch == 'Z')
    {
      sscanf(optionArgument,"%s",mibFileName);
    }
  
  return NULL;
#else
  return NULL;
#endif
}

void applicationSetup(void)
{
#if (WIFI_MAJOR_VERSION >= 5)
  CsrMemSet(mibFileName,0,sizeof(mibFileName));
#else
#endif
}

char* applicationExtraOptions(void)
{
#if (WIFI_MAJOR_VERSION >= 5)
    return "Z:";
#else
    return "";
#endif
}
