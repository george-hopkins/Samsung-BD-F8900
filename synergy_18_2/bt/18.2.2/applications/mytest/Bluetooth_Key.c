/**********************************************************************
* File Name   : Bluetooth_CM.c
* Description : This file contains functions and variables for
*                Connection Manager.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "csr_bt_app.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_declare_tasks.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_db.h" 
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_mobis_app.h"	
#include "Bluetooth_Key.h"


#define KEYB_CHECK_TIMEOUT                (250000)

#define WRONG_UID_CNT                   100
#define WRONG_UID                       {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x00,0x00}

static void playIntro(btAppInstance_t *instData);

#ifdef UNDER_CE
void KeyMessageService(char m)
{

    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t *)CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
    key->type = KEY_MESSAGE;
    key->key =m;
    CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);

}
#else
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
#endif 




#ifdef __CSR_COUNT_WD__
void CsrAppBtGapChipWdCount(CsrUint16 varId)
{

}
#endif


static void playGap(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion [GAP]\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("1 ) CsrBtSdReadAvailableServicesReqSend\n");
    CL_API_LOGPrintf("2 ) CsrBtScDebugModeReqSend FALSE\n");
    CL_API_LOGPrintf("3 ) CsrBtScDebugModeReqSend TRUE\n");

    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeyGapPrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{
  


    switch(key->key)
    {
        case '1':
        {
            CsrBtDeviceAddr deviceAddr;

            deviceAddr.nap=0xD017;
            deviceAddr.uap=0x6A;
            deviceAddr.lap=0x7DDB3B;
            CL_API_LOGPrintf("CsrBtSdReadAvailableServicesReqSend\n");
            CsrBtSdReadAvailableServicesReqSend(inst->sdinstData->qid, deviceAddr, CSR_BT_SD_SERVICE_USE_STANDARD, 0, NULL);

        break;
        }
        case '2':
        {

            CsrBtScDebugModeReqSend(inst->scinstData->qid, FALSE);
            break;
        }
        case '3':
        {

            CsrBtScDebugModeReqSend(inst->scinstData->qid, TRUE);
            break;
        }

        default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,\n", key->key );
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }
    }
}	

static void playAv(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion [AV]\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("1 ) CsrBtAvrcpCtPassThroughReqSend VOLUME_UP PRESS_RELEASE\n");
    CL_API_LOGPrintf("2 ) CsrBtAvrcpCtPassThroughReqSend VOLUME_DOWN PRESS_RELEASE\n");
    CL_API_LOGPrintf("3 ) CsrBtAvrcpCtPassThroughReqSend FFORWARD PRESS\n");
    CL_API_LOGPrintf("4 ) CsrBtAvrcpCtPassThroughReqSend FFORWARD RELEASE\n");
    CL_API_LOGPrintf("5 ) CsrBtAvrcpCtPassThroughReqSend REWIND PRESS\n");
    CL_API_LOGPrintf("6 ) CsrBtAvrcpCtPassThroughReqSend REWIND RELEASE\n");
    CL_API_LOGPrintf("7 ) CsrBtAvrcpCtGetAttributesReqSend \n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeyAvPrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{



    switch(key->key)
    {
        case '1':
        {
            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_VOLUME_UP, CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);

        break;
        }

        case '2':
        {

            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_VOLUME_DOWN, CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
        break;

        }

        case '3':
        {
            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_FFORWARD, CSR_BT_AVRCP_PT_STATE_PRESS);
            break;

        }
        case '4':
        {
            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_FFORWARD, CSR_BT_AVRCP_PT_STATE_RELEASE);

            break;

        }

        case '5':
        {
            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_REWIND, CSR_BT_AVRCP_PT_STATE_PRESS);
            break;

        }
        case '6':
        {
            CsrBtAvrcpCtPassThroughReqSend(inst->avrcinstData->qid, inst->avrcinstData->connectionId, CSR_BT_AVRCP_PT_OP_ID_REWIND, CSR_BT_AVRCP_PT_STATE_RELEASE);
            break;

        }
        case '7':
        {
            CsrBtAvrcpUid uid = WRONG_UID;

            CsrBtAvrcpCtGetAttributesReqSend(inst->avrcinstData->qid,inst->avrcinstData->connectionId,
                             CSR_BT_AVRCP_SCOPE_MP_FS,
                             uid,
                             WRONG_UID_CNT,
                             CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);
            break;

        }


        default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,\n", key->key );
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }
    }
}	


static void playHf(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion [HF]\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("1 ) CsrBtHfAudioConnectReqSend\n");
    CL_API_LOGPrintf("2 ) CsrBtHfAudioDisconnectReqSend\n");

    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeyHfPrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{
  


    switch(key->key)
    {
        case '1':
        {

            CsrBtHfAudioLinkParameterListConfig  *audSetting;

            audSetting = (CsrBtHfAudioLinkParameterListConfig  *)CsrPmemAlloc(sizeof(CsrBtHfAudioLinkParameterListConfig));
            audSetting->packetType = SCO_PACKET_TYPE;
            audSetting->txBandwidth = SCO_TX_BANDWIDTH;
            audSetting->rxBandwidth = SCO_RX_BANDWIDTH;
            audSetting->maxLatency = SCO_MAX_LATENCY;
            audSetting->voiceSettings = SCO_VOICE_SETTINGS;
            audSetting->reTxEffort = SCO_RETRANSMISSION_EFFORT;
            audSetting->txBandwidth = 12000;
            audSetting->rxBandwidth = 12000;

            CsrBtHfAudioConfigReqSend(inst->hfinstData->connectionId,
                            CSR_BT_HF_AUDIO_OUTGOING_PARAMETER_LIST_CONFIG,
                            (CsrUint8 *)audSetting,
                            sizeof(CsrBtHfAudioLinkParameterListConfig));
            CL_API_LOGPrintf("CsrBtHfAudioConnectReqSend %x\n",inst->hfinstData->connectionId);

            CsrBtHfAudioConnectReqSend(inst->hfinstData->connectionId, 0,NULL,
                       PCM_SLOT,
                       PCM_SLOT_REALLOCATE);
            break;
        }
        case '2':
        {

            CL_API_LOGPrintf("CsrBtHfAudioDisconnectReqSend, %x\n",inst->hfinstData->connectionId);

            CsrBtHfAudioDisconnectReqSend(inst->hfinstData->connectionId, 0xFFFF);


            break;
        }

        default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,\n", key->key );
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }
    }
}	





static void playSpp(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion [SPP]\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("1 ) \n");

    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeySppPrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{
 


    switch(key->key)
    {
        case '1':
        {
         
        break;
        }

        default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,\n", key->key );
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }
    }
}	


static void playPbap(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion [PBAP]\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("1 ) \n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeyPbapPrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{



    switch(key->key)
    {
        case '1':
        {
         
        break;
        }

        default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,\n", key->key );
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }
    }
}	




static void playIdle(btAppInstance_t *instData)
{
    CL_API_LOGPrintf("------------------------------------------------------------\n");
    CL_API_LOGPrintf("\n         mobis Demo Aplicataion\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
    CL_API_LOGPrintf("Hit <ESC> to quit program!\n");
    CL_API_LOGPrintf("Hit 'M' or 'm' to menu list!\n");
    CL_API_LOGPrintf("\n");
    CL_API_LOGPrintf("0 ) GAP\n");
    CL_API_LOGPrintf("1 ) AV\n");
    CL_API_LOGPrintf("2 ) HF\n");
    CL_API_LOGPrintf("3 ) SPP\n");
    CL_API_LOGPrintf("4 ) PBAP\n");
    CL_API_LOGPrintf("\n------------------------------------------------------------\n");
}

/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
static void handlekeyIdlePrim(btAppInstance_t  *inst ,CsrBtKeyPress_t *key)
{



    switch(key->key)
    {

        case '0':
        {
            inst->keyinstData->Keystate=GAP_STATUS;
            break;
        }
        case '1':
        {
            inst->keyinstData->Keystate=AV_SNK_STATUS;
            break;
        }
        case '2':
        {
            inst->keyinstData->Keystate=HF_STATUS;
            break;
        }
        case '3':
        {
            inst->keyinstData->Keystate=SPP_STATUS;
            break;
        }

        case '4':
        {
            inst->keyinstData->Keystate=PBAP_STATUS;
            break;
        }



        default:
        {
            CL_API_LOGPrintf("####### default in handlekeyIdlePrim ctrl handler %x,\n",inst->keyinstData->Keystate );
            inst->keyinstData->Keystate=IDLE_STATUS;
            break;
        /*	  unexpected primitive received    */
        } /*    end switch	 */
    }
    
    playIntro(inst);
    
}	



static void playIntro(btAppInstance_t *instData)
{
    switch(instData->keyinstData->Keystate)
    {
       case IDLE_STATUS:
        playIdle(instData);
        break;
      case GAP_STATUS:
        playGap(instData);
        break;
      case AV_SNK_STATUS:
        playAv(instData);
        break;  
      case HF_STATUS:
        playHf(instData);
        break;
      case SPP_STATUS:
        playSpp(instData);
        break;  
      case PBAP_STATUS:
        playPbap(instData);
        break;   
    }

}



/*****************************************************************************
* Function Name : init_key()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_key(void **gash)
{
    btAppInstance_t * instData=(btAppInstance_t *)*gash;

    instData->keyinstData=(KeyAppInst*) CsrPmemAlloc(sizeof(KeyAppInst));
    CsrMemSet(instData->keyinstData, 0, sizeof(KeyAppInst));


    instData->keyinstData->qid = TESTQUEUE;
    instData->keyinstData->recvMsgP = NULL;
    instData->keyinstData->Keystate=IDLE_STATUS;
    playIntro(instData);
#ifndef UNDER_CE
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
#endif
}


/*****************************************************************************
* Function Name : deinit_key()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void deinit_key(void **gash)
{
    btAppInstance_t * instData=(btAppInstance_t *)*gash;

    CsrPmemFree(instData->keyinstData);

}


/*****************************************************************************
* Function Name : handlekeyPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void handlekeyPrim(void  *gash ,CsrBtKeyPress_t *key)
{
    btAppInstance_t  *inst=(btAppInstance_t *)gash;
    if (key->key == ESCAPE_KEY)
    {
        CL_API_LOGPrintf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
        return;
#else
        exit(0);
        return;
#endif
    }
    if ((key->key == 'M')||(key->key == 'm'))
    {
        inst->keyinstData->Keystate=IDLE_STATUS;
        playIntro(inst);
        return;
    }
    switch(inst->keyinstData->Keystate)
    {
       case IDLE_STATUS:
        handlekeyIdlePrim(inst,key);
        break;
      case GAP_STATUS:
        handlekeyGapPrim(inst,key);
        break;
       case AV_SNK_STATUS:
        handlekeyAvPrim(inst,key);
        break;   
      case HF_STATUS:
        handlekeyHfPrim(inst,key);
        break;
      case SPP_STATUS:
        handlekeySppPrim(inst,key);
        break;  
      case PBAP_STATUS:
        handlekeyPbapPrim(inst,key);
        break; 
      default:
        {
            CL_API_LOGPrintf("####### default in keybPrim ctrl handler %x,stat[%x]\n", key->key, inst->keyinstData->Keystate);
            inst->keyinstData->Keystate=IDLE_STATUS;
            playIntro(inst);
            /*	  unexpected primitive received    */
        }  
    }
}
