/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_obex.h"
#include "csr_bt_ops_prim.h"
#include "csr_bt_ops_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_bt_demoapp.h"
#include "csr_app_main.h"

#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_prim.h"
#include "csr_bt_ampm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
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

/* #define DO_NOT_LOAD_DEFAULT_CARD */
#define KEYB_CHECK_TIMEOUT         (100000)

#define INBOX_DIR                  "inbox"PATH_SEP""
#define PUSH_DIR                   "pushfiles"PATH_SEP""
#define VCARD                      "own_card.vcf"
#define VCAL                       "own_cal.vcs"
#define VMSG                       "own_msg.vmg"
#define VNOTE                      "own_note.vnt"

#define IDLE_KEYBOARD_STATE        0
#define PASSKEY_KEYBOARD_STATE     1
#define CSR_BT_OPS_PUT_IND_STATE          2
#define CSR_BT_OPS_GET_IND_STATE          3
#define CONFIGURE_KEYBOARD_STATE   4 /* Config MIME-types at start */

#define MAX_MIME_LENGTH  30

typedef struct
{
    CsrSchedQid                qid;
    CsrUint16             event;
    void                 *message;
    CsrUint8              keyboardState;
    CsrBtDeviceAddr       bondingAddr;
    CsrUint8              passkeyLength;
    CsrUint8              passkey[CSR_BT_PASSKEY_MAX_LEN];
    char                  filename[128];
    CsrBtOpsPutInd       *opsPutIndMsg;
    CsrBtOpsGetInd       *opsGetIndMsg;
    char                 *lastfileSaved;
    CsrUint16             obexPeerMaxPacketSize;
    CsrUint32             currentFilePosition;
    CsrUint32             currentFileSize;
    char                  getObjName[128];
    CsrFileHandle                 *file;
    CsrUint16             mimeType;
    ScAppInstData_t       scInstData;
    CsrBool               srmSupported;
    CsrBtObexResponseCode obexResult;
    CsrBool               transferObject;
    CsrBtConnId           btConnId;
    CsrBool               performingPut;
    CsrUint8              srmpLeftToSend;
} PushServerInstanceDataType;

#if (WIFI_MAJOR_VERSION >= 5)
static CsrCharString mibFileName[1000];
#endif


static void showMenu(PushServerInstanceDataType *pInst)
{
    printf("\nOBEX PUSH SERVER:\n\n");
    printf("Default VCard: %s\n\n", pInst->filename);
    printf("Hit <ESC> to quit program!\n\n");
    printf("Program options:\n\n");
    printf("  1) Security Controller Configuration\n");
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (pInst->keyboardState == CONFIGURE_KEYBOARD_STATE)
    {

        if (pInst->srmSupported)
        {
            printf("  7) Disable Single Response Mode\n");
        }
        else
        {
            printf("  7) Enable Single Response Mode\n");
        }
        if (pInst->srmpLeftToSend == 3)
        {
            printf("  a)  Disable single Response Mode Parameter\n");
        }
        else
        {
            printf("  a) Set Single Response Mode Parameter to %d\n", pInst->srmpLeftToSend + 1);
        }
    }
#endif

    printf("\nSetup MIME-support:\n");
    printf("\tNONE supported, press 'd'\n");
    printf("\tVCARD only,     press 'v'\n");
    printf("\tALL but VCARD,  press 'w'\n");
    printf("\tALL  supported, press 'e' (or the anykey)\n\n");
}

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

static void oppScAppSetFnFini(void *pFiniData)
{
    PushServerInstanceDataType *instData = (PushServerInstanceDataType *) pFiniData;
    instData->keyboardState              = CONFIGURE_KEYBOARD_STATE;
    system(CLS);
    showMenu(instData);
}

static void oppScAppSetFnInit(void *pInitData)
{
    PushServerInstanceDataType *instData = (PushServerInstanceDataType *) pInitData;
    instData->keyboardState              = PASSKEY_KEYBOARD_STATE;
}

static void oppTransferingObject(PushServerInstanceDataType *pInst)
{
    if (pInst->obexResult != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        pInst->transferObject = FALSE;
    }
    else
    {
        pInst->transferObject = TRUE;
    }
}

static void* oppScAppSetFnGetMessage(void *pRecvMsgPData)
{
    PushServerInstanceDataType *instData = (PushServerInstanceDataType *) pRecvMsgPData;
    void *result                         = instData->message;
    instData->message                    = NULL;
    return result;
}

void CsrBtAppInit(void ** gash)
{
    PushServerInstanceDataType *pInst;
    CsrFileHandle * vcfFile;
    CsrBtStat_t statBuffer;
    int result;

    *gash = CsrPmemAlloc(sizeof(PushServerInstanceDataType));
    pInst = *gash;
    pInst->keyboardState = CONFIGURE_KEYBOARD_STATE;
    pInst->opsPutIndMsg = NULL;
    pInst->lastfileSaved = NULL;
    pInst->srmSupported = FALSE;
    pInst->transferObject   = FALSE;
    pInst->srmpLeftToSend = 0;

    CsrMemSet(pInst->filename, 0, sizeof(pInst->filename));
    sprintf(pInst->filename, "%s%s", PUSH_DIR, VCARD);

    showMenu(pInst);

    result = CsrBtStat(pInst->filename, &statBuffer);


    if (result != 0)
    {
        printf("Failed to get information about the default VCard file (%s)!\n",
               pInst->filename);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    /* Application handle */
    pInst->qid = TESTQUEUE;

    vcfFile = CsrBtFopen(pInst->filename, "rb");
    if(vcfFile == NULL)
    {
        printf("Failed to open default VCard file (%s)!\n", pInst->filename);
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
    }
    CsrBtFclose(vcfFile);

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    CsrBtScActivateReqSend(pInst->qid);

    /* Security App Configuration */
    CsrBtScActivateReqSend(pInst->qid);
    scAppInitInstanceData(&pInst->scInstData);

    scAppSetFnFini(&pInst->scInstData, oppScAppSetFnFini, pInst);
    scAppSetFnInit(&pInst->scInstData, oppScAppSetFnInit, pInst);
    scAppSetSecLevelSend(&pInst->scInstData, scAppSendProfileSecurityLevelReq, &pInst->scInstData);

    scAppSetFnGetMessage(&pInst->scInstData, oppScAppSetFnGetMessage, pInst);

    scAppSetSecLevelPrim(&pInst->scInstData,
                         0,     /* Index 0. Single profile */
                         "OPS", CSR_BT_OPS_PRIM,
                         CSR_BT_OPS_IFACEQUEUE,
                         CSR_BT_OPS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);

#ifdef CSR_AMP_ENABLE
#ifdef CSR_BT_APP_AMP_WIFI
    CsrWifiSmeActivateReqSend(TESTQUEUE);
#endif /* amp-wifi */
#endif /* amp-enable */
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    msgType=0;
    void        *msgData=NULL;
    PushServerInstanceDataType *pInst = *gash;

    scAppDeinitInstanceData(&pInst->scInstData);

    while(CsrSchedMessageGet(&msgType, &msgData))
    {
        switch (msgType)
        {
            case CSR_BT_OPS_PRIM:
                CsrBtOpsFreeUpstreamMessageContents(msgType, msgData);
                break;
            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(msgType, msgData);
                break;

        }
        CsrPmemFree(msgData);
    }
    if (pInst->opsPutIndMsg != NULL)
    {
        CsrPmemFree(pInst->opsPutIndMsg->payload);
        CsrPmemFree(pInst->opsPutIndMsg);
        pInst->opsPutIndMsg = NULL;
    }
    CsrPmemFree(pInst->lastfileSaved);
    CsrPmemFree(pInst->opsPutIndMsg);
    CsrPmemFree(pInst);
}
#endif

#if defined(CSR_AMP_ENABLE) && defined(CSR_BT_APP_AMP_WIFI)
static void handleWifiSme(PushServerInstanceDataType *inst)
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
#define CSR_WIFI_MAC_ADDRESS 0x00025B0ABEEF
#define CSR_WIFI_DESIGN_M2107_V3

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

/* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
static void handleAmpmPrim(PushServerInstanceDataType *inst)
{
    switch(*(CsrBtAmpmPrim*)inst->message)
    {
        case CSR_BT_AMPM_REGISTER_CFM:
            {
                CsrBtAmpmRegisterCfm *prim;
                prim = (CsrBtAmpmRegisterCfm*)inst->message;
                printf("AMPM register confirm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_DEREGISTER_CFM:
            {
                CsrBtAmpmDeregisterCfm *prim;
                prim = (CsrBtAmpmDeregisterCfm*)inst->message;
                printf("AMPM deregister confirm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_MOVE_IND:
            {
                CsrBtAmpmMoveInd *prim;
                prim = (CsrBtAmpmMoveInd*)inst->message;
                printf("AMP move indication, btConnId 0x%08x, localId 0x%04x -- accepting\n",
                       prim->btConnId, prim->localId);
                CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
            }
            break;

        case CSR_BT_AMPM_MOVE_CMP_IND:
            {
                CsrBtAmpmMoveCmpInd *prim;
                prim = (CsrBtAmpmMoveCmpInd*)inst->message;
                printf("AMP move complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_MOVE_CFM:
            {
                CsrBtAmpmMoveCfm *prim;
                prim = (CsrBtAmpmMoveCfm*)inst->message;
                printf("AMP move confirm, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
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

/* Handle CM primitives */
void handleCmPrim(PushServerInstanceDataType *inst)
{
    switch(*(CsrPrim*)inst->message)
    {
        default:
            printf("Unhandled CM primitive received: type 0x%04x\n", *(CsrPrim*)inst->message);
            break;
    }
}

void CsrBtAppHandler(void ** gash)
{
    PushServerInstanceDataType *pInst = *gash;

    CsrSchedMessageGet(&pInst->event, &pInst->message);
    switch (pInst->event)
    {
        case CSR_BT_OPS_PRIM:
        {
            CsrBtOpsPrim *opsPrim = pInst->message;

            switch (*opsPrim)
            {
                case CSR_BT_OPS_SECURITY_IN_CFM:
                {
                    handleScSecLevelPrim(&pInst->scInstData);
                    break;
                }
                case CSR_BT_OPS_CONNECT_IND:
                {
                    CsrBtOpsConnectInd *connectInd = pInst->message;

                    pInst->obexPeerMaxPacketSize = connectInd->obexPeerMaxPacketSize;
                    printf("\n\nBluetooth address: %04X %02X %06X is now connected to the server (btConnId 0x%08x)\n",
                           connectInd->deviceAddr.nap,
                           connectInd->deviceAddr.uap,
                           connectInd->deviceAddr.lap,
                           connectInd->btConnId);

                    CsrBtOpsConnectResSend(CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);

                    pInst->btConnId = connectInd->btConnId;
                    pInst->bondingAddr = connectInd->deviceAddr;


#ifdef CSR_AMP_ENABLE
                printf("Registering with AMP manager...\n");
                CsrBtAmpmRegisterReqSend(TESTQUEUE,
                                         0, /* flags */
                                         0, /* eventMask */
                                         pInst->btConnId, /* btConnId */
                                         &connectInd->deviceAddr); /* addr */
#endif
                    break;
                }
                case CSR_BT_OPS_GET_IND:
                {
#ifdef CSR_BT_INSTALL_OPS_GET
                    CsrBtOpsGetInd *getInd = pInst->message;
                    CsrBtStat_t statBuffer;
                    CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
                    switch (getInd->bodyType)
                    {
                        case CSR_BT_VCARD_TYPE:
                            sprintf(pInst->getObjName, "%s", VCARD);
                            break;

                        case CSR_BT_VCAL_TYPE:
                            sprintf(pInst->getObjName, "%s", VCAL);
                            break;

                        case CSR_BT_VNOTE_TYPE:
                            sprintf(pInst->getObjName, "%s", VMSG);
                            break;

                        case CSR_BT_VMESSAGE_TYPE:
                            sprintf(pInst->getObjName, "%s", VNOTE);
                            break;

                        default:
                            printf("No type\n");
                            break;
                    }
                    printf("Allow get object (%s) (y/n)?.\n", pInst->getObjName);
                    pInst->keyboardState = CSR_BT_OPS_GET_IND_STATE;
                    pInst->opsGetIndMsg = getInd;
                    pInst->performingPut = FALSE;
                    pInst->message = NULL;
#else                    
                    printf("ERROR!!!! Please build with CSR_BT_INSTALL_OPS_GET to enable GET-operations...\n");
#endif
                    break;
                }
#ifdef CSR_BT_INSTALL_OPS_GET
                case CSR_BT_OPS_GET_NEXT_IND:
                {

                    CsrUint8 *body = NULL;
                    CsrBtObexResponseCode obexResult;
                    CsrBool finalFlag;
                    CsrUint32 bytesToRead, bytesRead;


                    obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                    if(pInst->file != NULL)
                    {
                        if(pInst->currentFileSize - pInst->currentFilePosition <= pInst->obexPeerMaxPacketSize)
                        {
                            bytesToRead = pInst->currentFileSize - pInst->currentFilePosition;
                        }
                        else
                        {
                            bytesToRead = pInst->obexPeerMaxPacketSize;
                        }
                        body = CsrPmemAlloc(bytesToRead);
                        bytesRead = CsrBtFread(body, 1, bytesToRead, pInst->file);
                        pInst->currentFilePosition += bytesRead;
                        if(pInst->currentFilePosition >= pInst->currentFileSize)
                        {
                            CsrBtFclose(pInst->file);
                            pInst->file = NULL;
                            finalFlag = TRUE;
                        }
                        else
                        {
                            finalFlag = FALSE;
                        }
                    }
                    else
                    {
                        printf("Get next indication received, but file is missing !\n");
                        obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                        pInst->file = NULL;
                        bytesRead=0;
                        finalFlag = TRUE;
                    }
                    if (pInst->srmpLeftToSend)
                    {
                        pInst->srmpLeftToSend--;
                        printf("srmp set in answer to client srmp = %d \n", pInst->srmpLeftToSend);
                        CsrBtOpsGetNextResSend(finalFlag, obexResult, bytesRead, body, TRUE);
                    }
                    else
                    {
                        CsrBtOpsGetNextResSend(finalFlag, obexResult, bytesRead, body, FALSE);
                    }
                    break;
                }
#endif                
                case CSR_BT_OPS_PUT_IND:
                {
                    CsrBtOpsPutInd *putInd = pInst->message;
                    CsrUint8 *name;

                    name = CsrUcs2ByteString2Utf8(putInd->payload + putInd->ucs2nameOffset);
                    printf("Object \"%s\" received. Store object (y/n)?.\n", name);
                    pInst->keyboardState = CSR_BT_OPS_PUT_IND_STATE;
                    pInst->opsPutIndMsg = putInd;
                    pInst->message = NULL;
                    pInst->performingPut = TRUE;
                    CsrPmemFree(name);
                    break;
                }
                case CSR_BT_OPS_PUT_NEXT_IND:
                {
                    CsrBtOpsPutNextInd *putNextInd = pInst->message;

                    if (pInst->obexResult == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
                        pInst->obexResult == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
                    {
                        CsrFileHandle *putFile;
                        char fileName[128];
                        char *name;

                        name = pInst->lastfileSaved;
                        if (putNextInd->bodyLength > 0)
                        {
                            printf("Append to file : %s\n", name);
                            if (pInst->transferObject)
                            {
                                printf("  8) Reject current operation\n");
                            }
                            sprintf(fileName, "%s%s", INBOX_DIR, name);
                            putFile = CsrBtFopen(fileName, "ab");
                            if (putFile != NULL)
                            {
                                CsrBtFwrite((putNextInd->payload + putNextInd->bodyOffset),
                                          1, putNextInd->bodyLength, putFile);
                                CsrBtFclose(putFile);
                                printf("Saved \"%s\" in inbox.\n", name);
                                if (putNextInd->finalFlag)
                                {
                                    pInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                                    if (pInst->lastfileSaved)
                                    {
                                        CsrPmemFree(pInst->lastfileSaved);
                                        pInst->lastfileSaved = NULL;
                                    }
                                }
                                else
                                {
                                    pInst->obexResult = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                                }
                            }
                            else
                            {
                                printf("Failed to save object in inbox (%s)!\n", name);
                                pInst->obexResult = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                            }
                        }
                        else
                        {
                            pInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        }
                    }
                    oppTransferingObject(pInst);
                    if (pInst->srmpLeftToSend)
                    {
                        pInst->srmpLeftToSend--;
                        printf("srmp set in answer to client srmp = %d \n", pInst->srmpLeftToSend);
                        CsrBtOpsPutNextResSend(pInst->obexResult, TRUE);
                    }
                    else
                    {
                        CsrBtOpsPutNextResSend(pInst->obexResult, FALSE);
                    }

                    if (putNextInd->payload)
                    {
                        CsrPmemFree(putNextInd->payload);
                        putNextInd->payloadLength = 0;
                        putNextInd->payload       = NULL;
                    }
                    break;
                }
                case CSR_BT_OPS_ABORT_IND:
                {
                    CsrBtOpsAbortInd *abortInd = pInst->message;
                    CsrUint8        * description;
                    CsrFileHandle *putFile;

                    description = CsrUcs2ByteString2Utf8(abortInd->payload + abortInd->descriptionOffset);
                    printf("CSR_BT_OPS_ABORT_IND received description %s\n", description);
                    CsrPmemFree(description);

                    if (abortInd->payload)
                    {
                        CsrPmemFree(abortInd->payload);
                        abortInd->payloadLength = 0;
                        abortInd->payload       = NULL;
                    }

                    if (pInst->performingPut)
                    {
                        char fileName[128];

                        sprintf(fileName, "%s%s", INBOX_DIR, pInst->lastfileSaved);
                        putFile = CsrBtFopen(fileName, "r");
                        if (putFile != NULL)
                        {/*if file exists -> delete it*/
                            CsrBtFclose(putFile);
                            CsrBtRemove(fileName);
                        }
                    }

                    break;
                }
                case CSR_BT_OPS_DISCONNECT_IND:
                {
                    CsrBtOpsDisconnectInd *disconnectInd = pInst->message;

                    printf("\n\nBluetooth address: %04X %02X %06X is now disconnected from the server\n",
                           disconnectInd->deviceAddr.nap, disconnectInd->deviceAddr.uap, disconnectInd->deviceAddr.lap);
                    pInst->performingPut = FALSE;

#ifdef CSR_AMP_ENABLE
                    printf("Unregistering with AMPM...\n");
                    CsrBtAmpmDeregisterReqSend(TESTQUEUE,
                                               pInst->btConnId,
                                               &disconnectInd->deviceAddr);
#endif
                    break;
                }
                default:
                    CsrBtOpsFreeUpstreamMessageContents(*opsPrim, pInst->message);
                    break;
            }
            break;
        }
        case KEY_MESSAGE:
        {
            CsrBtKeyPress_t *key;
            key = (CsrBtKeyPress_t*)pInst->message;

            if (key->key == ESCAPE_KEY)
            {
                printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
                CsrSchedStop();
#else
                exit(0);
#endif
                return;
            }
            else
            {
                if (key->key == '1')
                {
                    handleScKeybPrim(&pInst->scInstData);
                    CsrPmemFree(pInst->message);
                    return;
                }
                if (pInst->transferObject && key->key == '8')
                {
                    pInst->transferObject = FALSE;
                    pInst->obexResult     = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                    CsrPmemFree(pInst->message);
                    return;

                }

                switch (pInst->keyboardState)
                {
                    case CONFIGURE_KEYBOARD_STATE: {
                        CsrUint8 * str;
                        pInst->mimeType = 0x0000;
                        str = CsrPmemAlloc(sizeof(CsrUint8)*14);

                        switch(key->key){
                            case 'd':{
                                pInst->mimeType = CSR_BT_NO_TYPE_SUPPORT;
                                strcpy((char *) str, "NO type");
                                break;}
                            case 'e':{
                                pInst->mimeType = CSR_BT_ANY_TYPE_SUPPORT;
                                strcpy((char *) str,"ANY type");
                                break;}
                            case 'v':{ /* VCARD only */
                                pInst->mimeType = CSR_BT_NO_TYPE_SUPPORT | CSR_BT_VCARD_2_1_SUPPORT | CSR_BT_VCARD_3_0_SUPPORT;
                                strcpy((char *) str,"Only VCARD");
                                break;}
                            case 'w':{ /* All but VCARD */
                                pInst->mimeType = CSR_BT_VCAL_1_0_SUPPORT | CSR_BT_ICAL_2_0_SUPPORT | CSR_BT_VNOTE_SUPPORT |
                                                  CSR_BT_VMESSAGE_SUPPORT | CSR_BT_OTHER_TYPE_SUPPORT;
                                strcpy((char *) str,"All but VCARD");
                                break;}
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                            case '7':
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
                                CsrPmemFree(str);
                                CsrPmemFree(pInst->message);
                                return;
                            }
                            case 'a':
                            case 'A':
                            {
                                pInst->srmpLeftToSend = (pInst->srmpLeftToSend + 1)%4;
                                if (pInst->srmpLeftToSend == 0)
                                {
                                    printf("Single Response Mode Parameter is now disable\n");
                                }
                                else
                                {
                                    printf("Single Response Mode Parameter is now set to %d\n", pInst->srmpLeftToSend);
                                }
                                CsrPmemFree(str);
                                CsrPmemFree(pInst->message);
                                return;
                            }
#endif
                            default:{
                                printf("Unhandled (any?) key, setting MIME type to all supported\n");
                                strcpy((char *) str,"ANY type");
                                pInst->mimeType= CSR_BT_ANY_TYPE_SUPPORT;
                                break;}
                        }

                        /* Make connectable */
                        CsrBtOpsActivateReqSend(pInst->qid,
                                                pInst->mimeType,
                                                CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                                0, /*windowSize*/
                                                pInst->srmSupported);
                        pInst->keyboardState = IDLE_KEYBOARD_STATE;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                        if (pInst->srmSupported)
                        {
                            printf("\n\nServer active with: %s and Single Response Mode supported\n",str);
                        }
                        else
#endif
                        {
                            printf("\n\nServer active with: %s supported\n",str);
                        }
                        CsrPmemFree(str);
                        break;
                    }

                    case PASSKEY_KEYBOARD_STATE:
                    {
                        handleScKeybPrim(&pInst->scInstData);
                        break;
                    }
                    case CSR_BT_OPS_GET_IND_STATE:
                    {
#ifdef CSR_BT_INSTALL_OPS_GET

                        CsrBtOpsGetInd *getInd = pInst->opsGetIndMsg;
                        CsrBtStat_t statBuffer;
                        CsrUint8 *name;
                        CsrBtObexResponseCode obexResult;
                        long result;
                        CsrUint8 *body;
                        CsrBool finalFlag;
                        CsrUint32 bytesToRead, bytesRead;
                        char path[CSR_BT_MAX_PATH_LENGTH];

                        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));

                        sprintf(path, "%s%s", PUSH_DIR, pInst->getObjName);
                        if( (key->key == 'y') )
                        {

                            name = CsrUtf82Ucs2ByteString((CsrUint8*)pInst->getObjName);
                            obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            result = CsrBtStat((char*)path, &statBuffer);
                            finalFlag = TRUE;

                            if(result == 0)
                            {
                                pInst->file = CsrBtFopen((char*)path, "rb");
                                if(pInst->file != NULL)
                                {
                                    printf("Getting object : (%s) \n", pInst->getObjName);
                                    pInst->currentFilePosition = 0;
                                    pInst->currentFileSize = statBuffer.st_size;
                                    if(pInst->currentFileSize - pInst->currentFilePosition <= (pInst->obexPeerMaxPacketSize - CsrUcs2ByteStrLen(name)))
                                    {
                                        bytesToRead = pInst->currentFileSize - pInst->currentFilePosition;
                                    }
                                    else
                                    {
                                        bytesToRead = pInst->obexPeerMaxPacketSize - CsrUcs2ByteStrLen(name);
                                    }
                                    body = CsrPmemAlloc(bytesToRead);
                                    bytesRead = CsrBtFread(body, 1, bytesToRead, pInst->file);
                                    pInst->currentFilePosition += bytesRead;

                                    if(pInst->currentFilePosition >= pInst->currentFileSize)
                                    {
                                        CsrBtFclose(pInst->file);
                                        pInst->file = NULL;
                                        finalFlag = TRUE;
                                    }
                                    else
                                    {
                                        finalFlag = FALSE;
                                    }
                                }
                                else
                                {
                                    printf("Failed to open object (%s) !\n", path);
                                    finalFlag = TRUE;
                                    obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                    bytesRead = 0;
                                    body = NULL;
                                    name = NULL;
                                    pInst->currentFileSize = 0;
                                }
                            }
                            else
                            {
                                printf("Failed to get information about the object (%s) !\n", path);
                                finalFlag = TRUE;
                                obexResult = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                                bytesRead = 0;
                                body = NULL;
                                name = NULL;
                                pInst->currentFileSize = 0;
                            }
                        }
                        else
                        {
                            printf("Rejected get request (%s)\n", path);
                            finalFlag = TRUE;
                            obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                            bytesRead = 0;
                            body = NULL;
                            name = NULL;
                            pInst->currentFileSize = 0;

                        }
                        if (pInst->srmpLeftToSend)
                        {
                            pInst->srmpLeftToSend--;
                            printf("srmp set in answer to client srmp = %d \n", pInst->srmpLeftToSend);
                            CsrBtOpsGetResSend(finalFlag, obexResult, getInd->bodyType, pInst->currentFileSize,
                                  name, bytesRead, body, TRUE);
                        }
                        else
                        {
                            CsrBtOpsGetResSend(finalFlag, obexResult, getInd->bodyType, pInst->currentFileSize,
                                  name, bytesRead, body, FALSE);
                        }
                        pInst->keyboardState = IDLE_KEYBOARD_STATE;
#else
                        printf("ERROR - Please build with CSR_BT_INSTALL_OPS_GET to enable GET operations. \n");
                        
#endif
                        break;
                    }

                    case CSR_BT_OPS_PUT_IND_STATE:
                    {
                        CsrBtOpsPutInd *putInd = pInst->opsPutIndMsg;

                        CsrFileHandle *putFile;
                        char fileName[128];
                        char *name;
                        CsrUint8 * ptr;
                        CsrUint8 i, ii;
                        char mimeBuffer[MAX_MIME_LENGTH+1];
                        CsrBool acceptedMIME = FALSE;
                        pInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                        name = (char*)CsrUcs2ByteString2Utf8(putInd->payload + putInd->ucs2nameOffset);
                        CsrPmemFree(pInst->lastfileSaved);
                        pInst->lastfileSaved = NULL;
                        pInst->lastfileSaved = name;
                        if ( (key->key == 'y') || (key->key == 'Y') )
                        {

                            /* Extract mime-type */
                            ptr = putInd->payload + putInd->bodyTypeOffset;
                            i = 0;
                            printf("\nMIME: ");
                            while((*(ptr + i) != '\0')
                                  && (i <= MAX_MIME_LENGTH)){
                                printf("%c", *(ptr + i));
                                mimeBuffer[i] = *(ptr + i);
                                i++;
                            }
                            mimeBuffer[i] = '\0';

                            if(i >= MAX_MIME_LENGTH){
                                printf("WARNING:\n");
                                printf("Suspiciously long MIME-type. If application crashes \n");
                                printf("or reports unknown MIME-type the setting of this info h\n");
                                printf("in the received object is probably not correct\n"); }
                            printf("\n");

                            /* Find MIME-type */
                            if( !CsrStrCmp(mimeBuffer, "text/x-vcard")){
                                if((pInst->mimeType & CSR_BT_VCARD_2_1_SUPPORT) || (pInst->mimeType & CSR_BT_VCARD_3_0_SUPPORT)){
                                    acceptedMIME = TRUE;
                                } else {
                                    printf("(text/x-vcard MIME-type not enabled!)\n");}

                            } else if ( !CsrStrCmp(mimeBuffer, "text/x-vcalendar")){
                                if((pInst->mimeType & CSR_BT_VCAL_1_0_SUPPORT) || (pInst->mimeType & CSR_BT_ICAL_2_0_SUPPORT)){
                                    acceptedMIME = TRUE;
                                } else {
                                    printf("(text/x-vcalendar MIME-type not enabled!)\n");
                                }

                            } else if ( !CsrStrCmp(mimeBuffer, "text/x-vnote")) {
                                if(pInst->mimeType & (CSR_BT_VNOTE_SUPPORT)) {
                                    acceptedMIME = TRUE;
                                } else {
                                    printf("(text/x-vnote MIME-type not enabled!)\n");
                                }

                            } else if ( !CsrStrCmp(mimeBuffer, "text/x-vmessage")){
                                if(pInst->mimeType & (CSR_BT_VMESSAGE_SUPPORT)){
                                    acceptedMIME = TRUE;}
                                else {
                                    printf("(text/x-vmessage MIME-type not enabled)\n");}

                            } else {
                                if(pInst->mimeType & (CSR_BT_OTHER_TYPE_SUPPORT)){
                                    acceptedMIME = TRUE;
                                } else {
                                    printf("(text/x-<other> MIME-type not enabled)\n");
                                }
                            }

                            if (acceptedMIME == TRUE){
                            sprintf(fileName, "%s%s", INBOX_DIR, name);
                            putFile = CsrBtFopen(fileName, "wb");
                            if (putFile != NULL)
                            {
                                CsrBtFwrite((putInd->payload + putInd->bodyOffset),
                                          1, putInd->bodyLength, putFile);
                                CsrBtFclose(putFile);
                                printf("Saved \"%s\" in inbox.\n", name);
                                pInst->obexResult = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                            }
                            else
                            {
                                printf("Failed to save object in inbox (%s)!\n", pInst->lastfileSaved);
                                pInst->obexResult = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
                            }
                            } else {
                                printf("Unrecognized/unsupported MIME-type: ");
                                ii = 0;
                                while(ii < i){
                                    printf("%c",mimeBuffer[ii]);
                                    ii++;
                                }
                                printf("\nObject not saved\n");
                                pInst->obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;

                        }
                        }
                        else if ((key->key == 'u') || (key->key == 'U') )
                        {
                            printf("Unsupported. Do not save object (%s)!\n", name);
                            pInst->obexResult = CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE;
                        }
                        else
                        {
                            printf("Rejecting received object (%s)\n", name);
                            pInst->obexResult = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                            CsrPmemFree(pInst->lastfileSaved);
                            pInst->lastfileSaved = NULL;
                        }
                        oppTransferingObject(pInst);
                        if (pInst->srmpLeftToSend)
                        {
                            pInst->srmpLeftToSend--;
                            printf("srmp set in answer to client srmp = %d \n", pInst->srmpLeftToSend);
                            CsrBtOpsPutResSend(pInst->obexResult, TRUE); 
                        }
                        else
                        {
                            CsrBtOpsPutResSend(pInst->obexResult, FALSE);
                        }

                        if (pInst->opsPutIndMsg)
                        {
                            CsrPmemFree(pInst->opsPutIndMsg->payload);
                            CsrPmemFree(pInst->opsPutIndMsg);
                            pInst->opsPutIndMsg = NULL;
                        }
                        pInst->keyboardState = IDLE_KEYBOARD_STATE;
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

        case CSR_BT_CM_PRIM:
            handleCmPrim(pInst);
            break;

#ifdef CSR_BT_APP_AMP_WIFI
        case CSR_WIFI_SME_PRIM:
            handleWifiSme(pInst);
            break;
#endif

        default:
        {
            printf("UNKNOWN prim\n");
            break;
        }
    }
    CsrPmemFree(pInst->message);
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
