/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
                           All rights reserved.


REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_types.h"
#include "csr_formatted_io.h"

#include "csr_bt_avrcp_app_ui_sef.h"
#include "csr_bt_avrcp_app_task.h"
#include "csr_bt_avrcp_app_util.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_ui_lib.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bccmd_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_platform.h"

#define ENABLE_TX_VARID  0x4007
#define DISABLE_TX_VARID 0x4008





/* Static functions used by the CSR_UI upstream handler functions                 */
static void appUiKeyHandlerHideMics(void * instData,
                    CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* An key event function used to hide the showed UI. This function is used
     by many different APP UI's                                              */
    CsrUiUieHideReqSend(displayHandle);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(instData);
    CSR_UNUSED(key);
}

static void appUiKeyHandlerGetInputDialogMisc(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from UI's with input dialogs */
    CSR_UNUSED(instData);
    CSR_UNUSED(eventHandle);
    CSR_UNUSED(key);

    CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), displayHandle);
}

#if 0
static void appUiKeyHandlerOnOkFromEstablishedConDialog(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from UI's with input dialogs */

     CSR_UNUSED(eventHandle);
     CSR_UNUSED(key);


     CsrBtAvHideUi(instData, CSR_BT_AVRCP_CON_SPECIFIC_DIALOG_UI);
     CsrBtAvrcpShowUi(instData, CSR_BT_AV_ESTABLISH_CONN_MEANU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

}
#endif

static void appAvrcpChangeVolumeFromCTInputDialogGetCfmHandler(void* instData, CsrUieHandle displayHandle, CsrUint16 *text)
{
    avrcpinstance_t * inst = instData;
    CsrUint8 *str = CsrUtf16String2Utf8(text);
    CsrUint8 theChar;

    theChar = *str;

    /* TBD */
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
    switch (theChar)
    {

        case RETURN_KEY:
        {
            CsrBtAvrcpCtSetVolumeReqSend(inst->CsrSchedQid,
                            inst->avrcpCon[inst->currentConnection].connectionId,
                            inst->avrcpCon[inst->currentConnection].currentVolume);
            /* no break; intended */
        }
        case ' ':
        {
            if (theChar == ' ')
            {/* Return to main menu without changes: restore the original value */
                inst->avrcpCon[inst->currentConnection].currentVolume = inst->avrcpCon[inst->currentConnection].tmpVolume;
            }
            inst->avrcpCon[inst->currentConnection].menuState = DaAvStateAvrcpMainMenu;
            break;
        }
        case BACKSPACE_KEY:
        {
            if (inst->avrcpCon[inst->currentConnection].currentVolume > 0)
            {
                CsrUint8 val;

                if (inst->avrcpCon[inst->currentConnection].currentVolume > 9)
                {
                    val = inst->avrcpCon[inst->currentConnection].currentVolume % 10;
                    inst->avrcpCon[inst->currentConnection].currentVolume -= val;
                }
            }
            break;
        }
        default:
        {
            if ((theChar >= '0') && (theChar <= '9'))
            {
                inst->avrcpCon[inst->currentConnection].currentVolume *= 10;
                inst->avrcpCon[inst->currentConnection].currentVolume += (theChar - '0');
                CsrBtAvrcpCtSetVolumeReqSend(inst->CsrSchedQid,
                            inst->avrcpCon[inst->currentConnection].connectionId,
                            inst->avrcpCon[inst->currentConnection].currentVolume);
            }
            break;
        }
    }
#else
    /* Return to main menu without changes: restore the original value */
    inst->avrcpCon[inst->currentConnection].currentVolume = inst->avrcpCon[inst->currentConnection].tmpVolume;
    inst->avrcpCon[inst->currentConnection].menuState = DaAvStateAvrcpMainMenu;
#endif

    CsrUiUieHideReqSend(displayHandle);

}

static void appCreateAvrcpChangeVolumeFromCTUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;



        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create CSR_BT_AVRCP_CHANGE_VOLUME_UI    */
            csrUiVar->uiIndex = CSR_BT_AVRCP_CHANGE_VOLUME_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);

        }
    }
}

static void appAvrcpChangeVolumeInputDialogGetCfmHandler(void* instData, CsrUieHandle displayHandle, CsrUint16 *text)
{
    avrcpinstance_t * inst = instData;
    CsrUint8 *str = CsrUtf16String2Utf8(text);
    CsrUint8 theChar;

    theChar = *str;

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    switch (theChar)
    {

        case RETURN_KEY:
            {
                CsrBtAvrcpTgNotiVolumeReq(0,inst->avrcpCon[inst->currentConnection].currentVolume);
                /* no break; intended */
            }
            case ' ':
            {
                if (theChar == ' ')
                {/* Return to main menu without changes: restore the original value */
                    inst->avrcpCon[inst->currentConnection].currentVolume = inst->avrcpCon[inst->currentConnection].tmpVolume;
                }
                inst->avrcpCon[inst->currentConnection].menuState = DaAvStateAvrcpMainMenu;
                break;
            }
            case BACKSPACE_KEY:
            {
                if (inst->avrcpCon[inst->currentConnection].currentVolume > 0)
                {
                    CsrUint8 val;

                    if (inst->avrcpCon[inst->currentConnection].currentVolume > 9)
                    {
                        val = inst->avrcpCon[inst->currentConnection].currentVolume % 10;
                        inst->avrcpCon[inst->currentConnection].currentVolume -= val;
                    }
                }
                break;
            }
            default:
            {
                if ((theChar >= '0') && (theChar <= '9'))
                {
                    inst->avrcpCon[inst->currentConnection].currentVolume *= 10;
                    inst->avrcpCon[inst->currentConnection].currentVolume += (theChar - '0');
                    CsrBtAvrcpTgNotiVolumeReq(0,inst->avrcpCon[inst->currentConnection].currentVolume);
                }
                break;
            }
    }
#else
    /* Return to main menu without changes: restore the original value */
    inst->avrcpCon[inst->currentConnection].currentVolume = inst->avrcpCon[inst->currentConnection].tmpVolume;
    inst->avrcpCon[inst->currentConnection].menuState = DaAvStateAvrcpMainMenu;
#endif

    CsrUiUieHideReqSend(displayHandle);

}


static void appCreateAvrcpChangeVolumeUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_CHANGE_VOLUME_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;



        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI    */
            csrUiVar->uiIndex = CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);

        }
    }
}

static void appAvrcpChooseMediaPlayerInputDialogGetCfmHandler(void* instData, CsrUieHandle displayHandle, CsrUint16 *text)
{
   avrcpinstance_t * inst = instData;
   CsrUint8 *str = CsrUtf16String2Utf8(text);
   CsrUint8 theChar;

   theChar = *str;

   switch(theChar)
   {
      case RETURN_KEY:
      {
          inst->avrcpCon[inst->currentConnection].tmpMpIdEdit = FALSE;
          break;
      }
      case BACKSPACE_KEY:
      {
          if ((inst->avrcpCon[inst->currentConnection].tmpMpId > 0) &&
              (inst->avrcpCon[inst->currentConnection].tmpMpIdEdit))
          {
              CsrUint32 val;

              if (inst->avrcpCon[inst->currentConnection].tmpMpId > 9)
              {
                  val = inst->avrcpCon[inst->currentConnection].tmpMpId % 10;
                  inst->avrcpCon[inst->currentConnection].tmpMpId -= val;
              }
          }
          break;
      }
      case ' ':
      {
          inst->avrcpCon[inst->currentConnection].tmpMpIdEdit = FALSE;
          inst->avrcpCon[inst->currentConnection].tmpMpId = 0xFF;
          inst->avrcpCon[inst->currentConnection].menuState = DaAvStateAvrcpMainMenu;
          break;
      }
      default:
      {
          if (inst->avrcpCon[inst->currentConnection].tmpMpIdEdit)
          {
              if ((theChar >= '0') && (theChar <= '9'))
              {
                  inst->avrcpCon[inst->currentConnection].tmpMpId *= 10;
                  inst->avrcpCon[inst->currentConnection].tmpMpId += (theChar - '0');
              }
              else
              {
                  inst->avrcpCon[inst->currentConnection].tmpMpId = 0xFF;
              }
              inst->avrcpCon[inst->currentConnection].tmpMpIdEdit = FALSE;
          }
          break;
      }
    }


    CsrUiUieHideReqSend(displayHandle);

}


static void appCreateAvrcpChooseMediaPlayerUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerGetInputDialogMisc;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;



        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create CSR_BT_TOGGLE_TEST_STATUS_MENU_UI    */
            csrUiVar->uiIndex = CSR_BT_TOGGLE_TEST_STATUS_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);

        }
    }
}


/* Static functions to create the CSR_BT_TOGGLE_TEST_STATUS_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpToggleTestMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_TOGGLE_TEST_STATUS_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_AVRCP_DISCONNECT_KEY:
        {

             if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
             {
               startAvrcpDisconnect(inst);
             }
             else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

             {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
               CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
             }

             break;
        }
        case CSR_BT_AVRCP_SET_FRAGMENTATION_ABORT_FLAG_KEY:
        {
           if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
           {
               inst->proceedFragmentation = FALSE;

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     TEXT_AVRCP_SETTING_FRAGMENTATION_ABORT_UCS2, TEXT_OK_UCS2, NULL);

           }
           else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

           {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                   CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
           }
           /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
           CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
           break;
        }
        case CSR_BT_AVRCP_GET_ITEM_ATTR_TEST_MENU_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
           if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
           {
               CsrBtAvrcpUid uidZero = {0,0,0,0,0,0,0,0};
               CsrBtAvrcpUid uidTest = {1,2,3,4,5,6,7,8};

               CsrBtAvrcpCtGetAttributesReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_SCOPE_MP_FS,
                                           (CsrMemCmp((CsrUint8*)&inst->lastUidReceived,(CsrUint8*)(&uidZero),CSR_BT_AVRCP_UID_SIZE)
                                                       ? inst->lastUidReceived : uidTest),
                                           inst->lastUidCounter,
                                           CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Getting media attributes (FS scope)...\n") , TEXT_OK_UCS2, NULL);

           }
           else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

           {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                   CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
           }
#else
           CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
               CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_ITEM_ATTR") , TEXT_OK_UCS2, NULL);
#endif
           /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
           CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
           break;
        }
        default:
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Activate Menu Key \n");
            break;
        }

    }



    /* Unlock the CSR_BT_TOGGLE_TEST_STATUS_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpToggleTestMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_TOGGLE_TEST_STATUS_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpToggleTestMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_TOGGLE_TEST_STATUS_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("TOGGLE TG Test Status"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_SET_FRAGMENTATION_ABORT_FLAG_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Set fragmentation abort"), CONVERT_TEXT_STRING_2_UCS2("flag (current: CONTINUE)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_GET_ITEM_ATTR_TEST_MENU_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get item attributes"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            /* Create  CSR_BT_FILE_SYSTEM_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_FILE_SYSTEM_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Static functions to create the CSR_BT_FILE_SYSTEM_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpEnterFileSystemMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_FILE_SYSTEM_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);



    if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
    {
        switch (key)
        {

            case CSR_BT_AVRCP_DISCONNECT_KEY:
            {

                 if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
                 {
                   startAvrcpDisconnect(inst);
                 }
                 else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

                 {
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                   CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 }

                 break;
            }
            case CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_CMD_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                CsrBtAvrcpCtGetFolderItemsReqSend(inst->CsrSchedQid,
                                             inst->avrcpCon[inst->currentConnection].connectionId,
                                             CSR_BT_AVRCP_SCOPE_MP_FS,
                                             0,
                                             3,
                                             CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("GetFolderItems (file system)...\n") , TEXT_OK_UCS2, NULL);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_FOLER_ITEMS_CMD") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            case CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_START_BIGGER_THAN_END_ITEM_CMD_KEY:
            {

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                CsrBtAvrcpCtGetFolderItemsReqSend(inst->CsrSchedQid,
                                             inst->avrcpCon[inst->currentConnection].connectionId,
                                             CSR_BT_AVRCP_SCOPE_MP_FS,
                                             3,
                                             0,
                                             CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("GetFolderItems (file system; start item bigger than end item)...\n") , TEXT_OK_UCS2, NULL);
#else
                                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                         CONVERT_TEXT_STRING_2_UCS2("Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_FOLER_ITEMS_CMD") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            case CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_START_BIGGER_THAN_TOTAL_ITEM_CMD_KEY:
            {

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                CsrBtAvrcpCtGetFolderItemsReqSend(inst->CsrSchedQid,
                                             inst->avrcpCon[inst->currentConnection].connectionId,
                                             CSR_BT_AVRCP_SCOPE_MP_FS,
                                             15,
                                             16,
                                             CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);


                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("GetFolderItems (file system; start item bigger than total number of items)...\n") , TEXT_OK_UCS2, NULL);
#else
                                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                          CONVERT_TEXT_STRING_2_UCS2("Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_FOLER_ITEMS_CMD") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            default : /*                                         */
            {
                CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                            "AVRCP Activate Menu Key \n");
                break;
            }
        }
    }
    else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
    {
      CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
            CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

       /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
      CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
    }


    /* Unlock the CSR_BT_FILE_SYSTEM_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpEnterFileSystemMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_FILE_SYSTEM_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpEnterFileSystemMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_FILE_SYSTEM_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Submenu:  File System menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());


            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_CMD_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Issue GetFolderItems command"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_START_BIGGER_THAN_END_ITEM_CMD_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Issue GetFolderItems command with start item bigger than end item"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_ISSUE_GET_FOLDER_ITEMS_START_BIGGER_THAN_TOTAL_ITEM_CMD_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Issue GetFolderItems command with start item bigger than total number of items"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            /* Create  CSR_BT_NOW_PLAYING_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_NOW_PLAYING_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}



/* Static functions to create the CSR_BT_NOW_PLAYING_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpEnterNowPlayingMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_NOW_PLAYING_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);



    if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
    {
        switch (key)
        {

            case CSR_BT_AVRCP_DISCONNECT_KEY:
            {

                 if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
                 {
                   startAvrcpDisconnect(inst);
                 }
                 else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

                 {
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                   CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 }

                 break;
            }
            case CSR_BT_AVRCP_PLAY_INVALID_UID_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpUid uid = WRONG_UID;

                 CsrBtAvrcpCtPlayReqSend(inst->CsrSchedQid,
                                    inst->avrcpCon[inst->currentConnection].connectionId,
                                    CSR_BT_AVRCP_SCOPE_NPL,
                                    inst->lastUidCounter,
                                    uid);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Sending Play (wrong UID)...\n") , TEXT_OK_UCS2, NULL);
#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable the PLAY-command - For AVRCP 1.0 use pass-through to trigger a play.") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            case CSR_BT_AVRCP_PLAY_ITEM_SCOPE_NOW_PLAYING_LIST_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpCtPlayReqSend(inst->CsrSchedQid,
                                   inst->avrcpCon[inst->currentConnection].connectionId,
                                   CSR_BT_AVRCP_SCOPE_NPL,
                                   inst->lastUidCounter,
                                   inst->lastUidReceived);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Sending Play item (scope = now playing list)...\n") , TEXT_OK_UCS2, NULL);

#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable the PLAY-command - For AVRCP 1.0 use pass-through to trigger a play.") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            case CSR_BT_AVRCP_ADD_TO_NPL_INVALID_UID:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpUid uid = WRONG_UID;

                 CsrBtAvrcpCtAddToNowPlayingReqSend(inst->CsrSchedQid,
                                               inst->avrcpCon[inst->currentConnection].connectionId,
                                               CSR_BT_AVRCP_SCOPE_NPL,
                                               inst->lastUidCounter,
                                               uid);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Adding to NPL (wrong UID)...\n") , TEXT_OK_UCS2, NULL);
#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable ADD_TO_NOW_PLAYING") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            case CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_NOW_PLAYING_LIST_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpCtAddToNowPlayingReqSend(inst->CsrSchedQid,
                                               inst->avrcpCon[inst->currentConnection].connectionId,
                                               CSR_BT_AVRCP_SCOPE_NPL,
                                               inst->lastUidCounter,
                                               inst->lastUidReceived);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Adding to NPL (scope = now playing list)...\n") , TEXT_OK_UCS2, NULL);

#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable ADD_TO_NOW_PLAYING") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            case CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_VIRTUAL_FILE_SYSTEM_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpCtAddToNowPlayingReqSend(inst->CsrSchedQid,
                                               inst->avrcpCon[inst->currentConnection].connectionId,
                                               CSR_BT_AVRCP_SCOPE_MP_FS,
                                               inst->lastUidCounter,
                                               inst->lastUidReceived);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Adding to NPL from search list (scope = Virtual File System...\n") , TEXT_OK_UCS2, NULL);

#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable ADD_TO_NOW_PLAYING") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            case CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_SEARCH_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                 CsrBtAvrcpCtAddToNowPlayingReqSend(inst->CsrSchedQid,
                                               inst->avrcpCon[inst->currentConnection].connectionId,
                                               CSR_BT_AVRCP_SCOPE_SEARCH,
                                               inst->lastUidCounter,
                                               inst->lastUidReceived);

                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Adding to NPL from search list (scope = Search...\n") , TEXT_OK_UCS2, NULL);

#else
                 CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable ADD_TO_NOW_PLAYING") , TEXT_OK_UCS2, NULL);
#endif
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                 CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 break;
            }
            default:
            {
                 CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                            "AVRCP Now Playing Menu Key \n");
                 break;
            }

        }
    }

    else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
    {
      CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
            CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

       /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
      CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
    }

    /* Unlock the CSR_BT_NOW_PLAYING_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpEnterNowPlayingMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_NOW_PLAYING_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpEnterNowPlayingMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_NOW_PLAYING_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Now Playing menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());


            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_INVALID_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play item (scope = now playing list)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_ITEM_SCOPE_NOW_PLAYING_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play (invalid UID)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_ADD_TO_NPL_INVALID_UID,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Add To NPL (invalid UID)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_NOW_PLAYING_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Add To NPL (scope = now playing list)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_VIRTUAL_FILE_SYSTEM_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Add To NPL from search result (scope = Virtual File System)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_ADD_TO_NPL_SCOPE_SEARCH_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Add To NPL from search result (scope = Search)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            /* Create  CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}




/* Static functions to create the CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpEnterUnitInfoSubUnitInfoMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);



    if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
    {
        switch (key)
        {

            case CSR_BT_AVRCP_DISCONNECT_KEY:
            {

                 if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
                 {
                   startAvrcpDisconnect(inst);
                 }
                 else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

                 {
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                   CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 }

                 break;
            }
            case CSR_BT_AVRCP_SEND_UNIT_INFO_CMD_FOR_QUALIFICATION_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                CsrUint16 len = 10;
                CsrUint8 *pData = CsrPmemAlloc(len);

                CsrMemSet(pData,0xFF,len);

                CsrBtAvrcpCtUnitInfoCmdReqSend(inst->CsrSchedQid,
                                    inst->avrcpCon[inst->currentConnection].connectionId,
                                    len, pData);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("CT: Send Unit Info Cmd - needed for qualification...\n") , TEXT_OK_UCS2, NULL);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_CT_MODULE to enable UNIT_INFO") , TEXT_OK_UCS2, NULL);
#endif
                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            case CSR_BT_AVRCP_SEND_SUB_UNIT_INFO_CMD_FOR_QUALIFICATION_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                CsrUint16 len = 10;
                CsrUint8 *pData = CsrPmemAlloc(len);

                CsrMemSet(pData,0xFF,len);

                CsrBtAvrcpCtSubUnitInfoCmdReqSend(inst->CsrSchedQid,
                                    inst->avrcpCon[inst->currentConnection].connectionId,
                                    len, pData);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("CT: Send Sub-Unit Info Cmd - needed for qualification...\n") , TEXT_OK_UCS2, NULL);

#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_CT_MODULE to enable SUB_UNIT_INFO") , TEXT_OK_UCS2, NULL);
#endif
                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            default : /*                                         */
            {
                CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                            "AVRCP Activate Menu Key \n");
                break;
            }
        }
    }
    else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
    {
        CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
            CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
        CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
    }


    /* Unlock the CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpEnterUnitInfoSubunitInfoMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpEnterUnitInfoSubUnitInfoMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter unit-info subunit info menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;


            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());


            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_SEND_UNIT_INFO_CMD_FOR_QUALIFICATION_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("CT: Send Unit Info Cmd - needed for qualification"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_SEND_SUB_UNIT_INFO_CMD_FOR_QUALIFICATION_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("CT: Send Sub-Unit Info Cmd - needed for qualification"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            /* Create  CSR_BT_ENTER_PASS_THROUGH_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_ENTER_PASS_THROUGH_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Static functions to create the CSR_BT_ENTER_PASS_THROUGH_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpEnterPassThroughMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_ENTER_PASS_THROUGH_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);



    if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
    {
        switch (key)
        {

            case CSR_BT_AVRCP_DISCONNECT_KEY:
            {

                 if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
                 {
                   startAvrcpDisconnect(inst);
                 }
                 else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
                 {
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                   CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                 }

                 break;
            }
            case CSR_BT_AVRCP_PASS_THR_NEXT_GROUP_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Next Group */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_NEXT,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            case CSR_BT_AVRCP_PASS_THR_PREVIOUS_GROUP_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Previous Group */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_GROUP_NAV_PREV,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                                
                                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            case CSR_BT_AVRCP_PASS_THR_PLAY_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Play */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_PLAY,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                                
                                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            case CSR_BT_AVRCP_PASS_THR_STOP_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Stop */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_STOP,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                                
                                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            case CSR_BT_AVRCP_PASS_THR_FORWARD_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Previous Group */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_FORWARD,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }

            case CSR_BT_AVRCP_PASS_THR_BACK_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Previous Group */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_BACKWARD,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            case CSR_BT_AVRCP_PASS_THR_PAUSE_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                /* Previous Group */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                          inst->avrcpCon[inst->currentConnection].connectionId,
                                          CSR_BT_AVRCP_PT_OP_ID_PAUSE,
                                          CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            
            case CSR_BT_CHNGE_CATEGORY_AND_RESET_OPN_ID_KEY:
            {
                CsrCharString displayString[100];
                CsrBtAVRCPCsrUiType *csrUiVar = &(inst->csrUiVar);
                CsrUieHandle displayHandle    = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle;
                CsrUieHandle sk1Handle        = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].sk1EventHandle;
                CsrUieHandle backHandle       = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].backEventHandle;

                /* Change category and reset opId */
                inst->currentCategory = ++inst->currentCategory % 4;
                inst->currentOpId = 0;

                CsrMemSet((char*)displayString, 0, 100);
                //CsrBtSprintf((char*)displayString, "Change category (current: %d) and reset operation ID",inst->currentCategory + 1);
                CsrUiMenuSetitemReqSend(displayHandle, CSR_BT_CHNGE_CATEGORY_AND_RESET_OPN_ID_KEY,
                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                             CSR_UI_DEFAULTACTION, backHandle, CSR_UI_DEFAULTACTION);

                CsrMemSet((char*)displayString, 0, 100);
                //CsrBtSprintf((char*)displayString, "Send pass-through command and increase operation ID (current: %d/0x%02X)",inst->currentOpId, getAvrcpOpId(inst));
                CsrUiMenuSetitemReqSend(displayHandle, CSR_BT_AVRCP_CHG_PASS_THR_CMD_AND_INCREASE_OPN_ID_KEY,
                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                             CSR_UI_DEFAULTACTION, backHandle, CSR_UI_DEFAULTACTION);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                   CONVERT_TEXT_STRING_2_UCS2("Category-Changed") , TEXT_OK_UCS2, NULL);
                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            case CSR_BT_AVRCP_CHG_PASS_THR_CMD_AND_INCREASE_OPN_ID_KEY:
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                CsrCharString displayString[100];
                CsrBtAVRCPCsrUiType *csrUiVar = &(inst->csrUiVar);
                CsrUieHandle displayHandle    = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle;
                CsrUieHandle sk1Handle        = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].sk1EventHandle;
                CsrUieHandle backHandle       = csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].backEventHandle;

                                /* Increase opId and send */
                CsrBtAvrcpCtPassThroughReqSend(inst->CsrSchedQid,
                                    inst->avrcpCon[inst->currentConnection].connectionId,
                                    getAvrcpOpId(inst),
                                    CSR_BT_AVRCP_PT_STATE_PRESS_RELEASE);

                                inst->currentOpId = ++inst->currentOpId % 40;

                    CsrMemSet((char*)displayString, 0, 100);
                //CsrBtSprintf((char*)displayString, "Send pass-through command and increase operation ID (current: %d/0x%02X)",inst->currentOpId, getAvrcpOpId(inst));
                CsrUiMenuSetitemReqSend(displayHandle, CSR_BT_AVRCP_CHG_PASS_THR_CMD_AND_INCREASE_OPN_ID_KEY,
                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                             CSR_UI_DEFAULTACTION, backHandle, CSR_UI_DEFAULTACTION);

#else
                                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                      CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                                
                                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif                
                break;
            }
            default : /*                                         */
            {
                CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                            "AVRCP Activate Menu Key \n");
                break;
            }
        }
    }
    else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
    {
        CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
            CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
        CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
    }


    /* Unlock the CSR_BT_ENTER_PASS_THROUGH_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpEnterPassThroughMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_ENTER_PASS_THROUGH_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpEnterPassThroughMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_ENTER_PASS_THROUGH_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter pass through menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;
            CsrCharString displayString[100];

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());


            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_NEXT_GROUP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Next Group' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_PREVIOUS_GROUP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Previous Group' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_BACK_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Back' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_FORWARD_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Forward' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_STOP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Stop' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_PAUSE_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Pause' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_PASS_THR_PLAY_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send 'Play' PASS_THROUGH  key"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            //CsrBtSprintf((char*)displayString, "Change category (current: %d) and reset operation ID",inst->currentCategory + 1);
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_CHNGE_CATEGORY_AND_RESET_OPN_ID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrMemSet((char*)displayString, 0, 100);
            //CsrBtSprintf((char*)displayString, "Send pass-through command and increase operation ID (current: %d/0x%02X)",inst->currentOpId, getAvrcpOpId(inst));
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_CHG_PASS_THR_CMD_AND_INCREASE_OPN_ID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);



            /* Create  CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Static functions to create the CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpEnterMediaPlayerMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);



    if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
    {
        switch (key)
        {

            case CSR_BT_AVRCP_GET_LIST_OF_MEDIA_PLAYERS_KEY:
            {
                if (!inst->avrcpCon[inst->currentConnection].tmpMpIdEdit)
                {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                    CsrBtAvrcpCtGetFolderItemsReqSend(inst->CsrSchedQid,
                                                 inst->avrcpCon[inst->currentConnection].connectionId,
                                                 CSR_BT_AVRCP_SCOPE_MP_LIST,
                                                 1,
                                                 4,
                                                 0);
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Getting list of media players...\n") , TEXT_OK_UCS2, NULL);

#else
                   CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_LIST_OF_MEDIA_PLAYERS") , TEXT_OK_UCS2, NULL);
#endif
                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                   CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                }
                break;
            }
            case CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_ID_KEY:
            {
                CsrCharString displayString[200];

                if (!inst->avrcpCon[inst->currentConnection].tmpMpIdEdit)
                {
                   inst->avrcpCon[inst->currentConnection].tmpMpIdEdit = TRUE;
                   inst->avrcpCon[inst->currentConnection].tmpMpId = 0;
                }
                CsrSnprintf(displayString, 200, " Temp media player entered id: %u\n\n",inst->avrcpCon[inst->currentConnection].tmpMpId);
                CsrStrCat(displayString, "Enter addressed mp Id:");
                /* Set the menu on the CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI                               */
                CsrBtAvrcpSetInputDialog(inst, CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI,
                  CONVERT_TEXT_STRING_2_UCS2(displayString), CONVERT_TEXT_STRING_2_UCS2(displayString),
                  CSR_UI_ICON_KEY,  NULL, MAX_EDIT_LENGTH,
                  CSR_UI_KEYMAP_ALPHANUMERIC,TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI on the display                                  */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                break;
            }
            case CSR_BT_AVRCP_SET_ADDRESSED_PLAYER_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                if (!inst->avrcpCon[inst->currentConnection].tmpMpIdEdit)
                {
                    if (inst->avrcpCon[inst->currentConnection].tmpMpId != 0xFF)
                    {
                        inst->avrcpCon[inst->currentConnection].mpId = inst->avrcpCon[inst->currentConnection].tmpMpId;
                        inst->avrcpCon[inst->currentConnection].tmpMpId = 0xFF;
                        CsrBtAvrcpCtSetAddressedPlayerReqSend(inst->CsrSchedQid,
                                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                                         inst->avrcpCon[inst->currentConnection].mpId);

                        CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                            CONVERT_TEXT_STRING_2_UCS2("Set addressed player...\n") , TEXT_OK_UCS2, NULL);

                        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                        CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                    }
                }
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_CURRENT_PAS") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif

                break;
            }
            case CSR_BT_AVRCP_SET_BROWSED_PLAYER_KEY:
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                if (!inst->avrcpCon[inst->currentConnection].tmpMpIdEdit)
                {
                    if (inst->avrcpCon[inst->currentConnection].tmpMpId != 0xFF)
                    {
                        inst->avrcpCon[inst->currentConnection].browsedMpId = inst->avrcpCon[inst->currentConnection].tmpMpId;
                        inst->avrcpCon[inst->currentConnection].tmpMpId = 0xFF;
                        CsrBtAvrcpCtSetBrowsedPlayerReqSend(inst->CsrSchedQid,
                                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                                         inst->avrcpCon[inst->currentConnection].browsedMpId );

                        CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                            CONVERT_TEXT_STRING_2_UCS2("Set browsed player...\n") , TEXT_OK_UCS2, NULL);

                        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                        CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                    }
                }
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable SET_BROWSED_PLAYER") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
                break;
            }
            default : /*                                         */
            {
                CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                            "AVRCP Activate Menu Key \n");
                break;
            }
        }
    }
    else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
    {
        CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
            CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
        CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
    }

    /* Unlock the CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpEnterMediaPlayerMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpEnterMediaPlayerMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter media players menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());




            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_GET_LIST_OF_MEDIA_PLAYERS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get list of media players"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_ID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Choose media player Id"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_SET_ADDRESSED_PLAYER_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Set addressed player"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_SET_BROWSED_PLAYER_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Set browsed player"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);



            /* Create  CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}


/* Static functions to create the CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpRegisterForNotificationsMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);




    switch (key)
    {

        case CSR_BT_AVRCP_DISCONNECT_KEY:
        {

             if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
             {
               startAvrcpDisconnect(inst);
             }
             else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)

             {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
               CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
             }

             break;
        }
        case CSR_BT_AVRCP_REGISTER_FOR_ALL_NOTIFICATIONS_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtNotiRegisterReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_NOTI_FLAG_ALL,
                                           0,
                                           CSR_BT_AVRCP_NOTI_REG_STANDARD);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Registering for all possible notifications...\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable NOTIFICATIONS") , TEXT_OK_UCS2, NULL);

#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_ADDRESSED_MEDIA_PLAYER_CHANGE_NOTIFICATIONS_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtNotiRegisterReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER,
                                           0,
                                           CSR_BT_AVRCP_NOTI_REG_STANDARD);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Registering for addressed media player change notifications....\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable NOTIFICATIONS") , TEXT_OK_UCS2, NULL);
            
#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_UID_COUNT_CHANGE_NOTIFICATIONS_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtNotiRegisterReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_NOTI_FLAG_UIDS,
                                           0,
                                           CSR_BT_AVRCP_NOTI_REG_STANDARD);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Registering for UID count changed notification...\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable NOTIFICATIONS") , TEXT_OK_UCS2, NULL);
            
#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_REGISTER_FOR_PAS_CHANGE_NOTIFICATION_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtNotiRegisterReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_NOTI_FLAG_PAS,
                                           0,
                                           CSR_BT_AVRCP_NOTI_REG_STANDARD);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Registering for EVENT_PLAYER_APPLICATION_SETTING_CHANGED...\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable NOTIFICATIONS") , TEXT_OK_UCS2, NULL);

#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_REGISTER_FOR_VOLUME_CHANGE_NOTIFICATION_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtNotiRegisterReqSend(inst->CsrSchedQid,
                                           inst->avrcpCon[inst->currentConnection].connectionId,
                                           CSR_BT_AVRCP_NOTI_FLAG_VOLUME,
                                           0,
                                           CSR_BT_AVRCP_NOTI_REG_STANDARD);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                         CONVERT_TEXT_STRING_2_UCS2("Registering for volume change notifications....\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable NOTIFICATIONS") , TEXT_OK_UCS2, NULL);

#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Activate Menu Key \n");
            break;
        }
    }



    /* Unlock the CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpRegisterForNotificationsMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpRegisterForNotificationsMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, CONVERT_TEXT_STRING_2_UCS2("Register for notifications menu"), TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());



            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_REGISTER_FOR_ALL_NOTIFICATIONS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for ALL notifications"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_ADDRESSED_MEDIA_PLAYER_CHANGE_NOTIFICATIONS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for addressed media player change notifications"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_UID_COUNT_CHANGE_NOTIFICATIONS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for uid count change notifications"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_REGISTER_FOR_PAS_CHANGE_NOTIFICATION_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Register for PAS (player app. setting) change notifications"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_REGISTER_FOR_VOLUME_CHANGE_NOTIFICATION_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for volume change notifications"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            /* Create  CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI                                        */
            csrUiVar->uiIndex = CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);


        }
    }
}


/* Static functions to create the CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlereAvrcpEstablishConUi(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI  */
    avrcpinstance_t * inst = instData;
    CsrUint16 inputMode = CSR_UI_INPUTMODE_AUTO;
    CSR_UNUSED(eventHandle);


    switch (key)
    {

        case CSR_BT_AVRCP_DISCONNECT_KEY:
        {

             if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
             {
               startAvrcpDisconnect(inst);
             }
             else if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpDisconnected)
             {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
               CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
             }

             break;
        }
        case CSR_BT_AVRCP_CHANGE_UID_CNT_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Set UID counter to '30' */
                CsrBtAvrcpTgNotiUidsReq(0,30);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Test that notification is sent with UID changed to 30 if notification registered!\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Feature is disabled in build") , TEXT_OK_UCS2, NULL);
#endif
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_SIMULATE_NOW_PLAYING_CHG_TO_CT_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Notify now playing changed: CSR_BT_AVRCP_NOTI_FLAG_NOW_PLAYING_CONTENT */
                CsrBtAvrcpTgNotiNowPlayingReq(1);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Simulate now playing content changed notification to CT\n") , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Feature is disabled in build") , TEXT_OK_UCS2, NULL);
#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_SIMULATE_TRACK_CHG_TO_CT_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Notify now playing changed: CSR_BT_AVRCP_NOTI_FLAG_TRACK */
                CsrBtAvrcpTgNotiTrackReq(inst->playerIdList[0],inst->nowPlayingUid);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Simulate track changed notification to CT\n") , TEXT_OK_UCS2, NULL);

            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Feature is disabled in build") , TEXT_OK_UCS2, NULL);
#endif
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_REGISTER_ONE_OR_MORE_MEDIA_PLAYER_KEY:
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Register new media player */
                if (inst->registeredMpIdx < MAX_NUMBER_OF_MP)
                {
                    CsrUint16 pasLen = 0;
                    CsrUint8 *pas = NULL;
                    CsrBtAvrcpMpFeatureMask mpFeatures;
                    /* All opcodes allowed */
                    mpFeatures[0] = 0xFFFFFFFF;
                    mpFeatures[1] = 0xFFFFFFFF;
                    mpFeatures[2] = 7;
                    mpFeatures[3] = 0;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, "NewEqualizer1234567890");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON, "On");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, "NewEqualizer123456789098");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, "NewEqualizer123456789098765678NewEqualizer122");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    /* Custom example of equalizer presets */
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, "NewEqualizer presets");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x01, "Funk");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x02, "Pop");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x03, "Rock");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x04, "Classic");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x05, "Jazz");
#endif
                    CsrBtAvrcpTgMpRegisterReqSend(inst->CsrSchedQid,                             /* Player handle */
                                             CSR_BT_AVRCP_NOTI_FLAG_ALL,                 /* Notifications */
                                             CSR_BT_AVRCP_TG_MP_REGISTER_CONFIG_NONE,   /* Other configuration */
                                             pasLen,
                                             pas,
                                             CSR_BT_AVRCP_MP_TYPE_MAJOR_AUDIO,
                                             CSR_BT_AVRCP_MP_TYPE_SUB_NONE,
                                             mpFeatures,
                                             CsrUtf8StrDup("New CSR Player"));
                }
                else
                {
                    CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Not possible: maximum number of media players already registered! \n") , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                }
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_TG_MODULE to enable this feature.") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_UNREGISTER_ONE_MEDIA_PLAYER_KEY:
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* De-register latest registered media player */
                if (inst->playerIdList[inst->registeredMpIdx] != 0xFF)
                {
                    CsrBtAvrcpTgMpUnregisterReqSend(inst->CsrSchedQid,inst->playerIdList[inst->registeredMpIdx]);
                }
                else
                {
                    CsrCharString displayString[100];

                    CsrSnprintf(displayString, 100, "No media players left to unregister! index: %u, media player id: %u\n", inst->registeredMpIdx,
                           inst->playerIdList[inst->registeredMpIdx]);
                    CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
                }
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_TG_MODULE to enable this feature.") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_CHANGE_VOLUME_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                inst->avrcpCon[inst->currentConnection].tmpVolume = inst->avrcpCon[inst->currentConnection].currentVolume;
                inst->avrcpCon[inst->currentConnection].currentVolume = 0;
                /* Set the menu on the CSR_BT_AVRCP_CHANGE_VOLUME_UI                               */
                CsrBtAvrcpSetInputDialog(inst, CSR_BT_AVRCP_CHANGE_VOLUME_UI,
                  NULL, CONVERT_TEXT_STRING_2_UCS2("Enter New Volume:"),
                  CSR_UI_ICON_KEY,  NULL, MAX_EDIT_LENGTH,
                  CSR_UI_KEYMAP_ALPHANUMERIC,TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_CHANGE_VOLUME_UI on the display                                  */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_CHANGE_VOLUME_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }
        case CSR_BT_AVRCP_TOGGLE_EQUALIZER_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrCharString displayString[100];

                CsrBtAvrcpPasAttValPair *changedPas = CsrPmemAlloc(sizeof(CsrBtAvrcpPasAttValPair));

                changedPas->attribId = CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID;
                changedPas->valueId = inst->avrcpCon[inst->currentConnection].equalizerOn ? CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF : CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON;

                inst->avrcpCon[inst->currentConnection].equalizerOn = changedPas->valueId == CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF ? TRUE : FALSE;

                CsrBtAvrcpTgPasSetReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].mpId,
                                         1,
                                         changedPas);

                //CsrBtSprintf((char *)displayString, "Toggling the equalizer %s...\n", inst->avrcpCon[inst->currentConnection].equalizerOn ? "OFF" : "ON");
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_TG_MODULE to enable this feature.") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            
            break;
        }
        case CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrCharString displayString[100];
                inst->tgTest = !inst->tgTest;

                //CsrBtSprintf((char *)displayString, "TG Test status is now %s\n", ((inst->tgTest == TRUE) ? "ON" : "OFF"));
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }
        case CSR_BT_AVRCP_SEND_SET_ADDRESSED_PLAYER_CMD_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrCharString displayString[100];

                /* Changing current addressed player ID*/
                inst->avrcpCon[inst->currentConnection].mpId++;
                //CsrBtSprintf(displayString, "Addressed player changed to 0x%x\n", inst->avrcpCon[inst->currentConnection].mpId);
                CsrBtAvrcpTgSetAddressedPlayerReqSend(inst->CsrSchedQid,inst->avrcpCon[inst->currentConnection].mpId,0);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);

            }
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile without EXCLUDE_CSR_BT_AVRCP_TG_MODULE to enable this feature.") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_INFORM_DISPLAYABLE_CHR_SET_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Inform displayable characterset */
                CsrBtAvrcpCharSet *charSet = CsrPmemAlloc(sizeof(CsrBtAvrcpCharSet));

                *charSet = CSR_BT_AVRCP_CHARACTER_SET_UTF_8;

                CsrBtAvrcpCtInformDispCharSetReqSend(inst->CsrSchedQid,
                                                inst->avrcpCon[inst->currentConnection].connectionId,
                                                1,
                                                charSet);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Informing about displayable character set...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable INFO_DISP_CHR_SET") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_WITH_WRONG_UID_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpUid uid = WRONG_UID;

                CsrBtAvrcpCtGetAttributesReqSend(inst->CsrSchedQid,
                                                 inst->avrcpCon[inst->currentConnection].connectionId,
                                                 CSR_BT_AVRCP_SCOPE_MP_FS,
                                                 uid,
                                                 WRONG_UID_CNT,
                                                 CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Getting media attributes (Wrong UID)...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_ITEM_ATTR") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_GET_PLAY_STATUS_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtGetPlayStatusReqSend(inst->CsrSchedQid,
                                                            inst->avrcpCon[inst->currentConnection].connectionId);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Getting play status...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_PLAYER_STATUS") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_RETRIEVE_ALL_PAS_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                /* Retrieve all PAS (attributes + values + all text) */
                CsrBtAvrcpCtPasAttIdReqSend(inst->CsrSchedQid,
                                                      inst->avrcpCon[inst->currentConnection].connectionId);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Retrieving all available Player Application Settings...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable RETRIEVE_ALL_PAS") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_GET_CURRENT_PAS_KEY:
        {

#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrUint8 *attribs = CsrPmemAlloc(sizeof(CsrBtAvrcpPasAttId) * 2);

                attribs[0] = CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID;
                attribs[1] = CSR_BT_AVRCP_PAS_SCAN_ATT_ID;

                CsrBtAvrcpCtPasCurrentReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                         2,
                                         attribs);
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Retrieving current PAS values for a couple of attributes...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_CURRENT_PAS") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_CT_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpPasAttValPair *valPair = CsrPmemAlloc(2 * sizeof(CsrBtAvrcpPasAttValPair));

                valPair[0].attribId = CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID;
                valPair[0].valueId  = CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON;
                valPair[1].attribId = CSR_BT_AVRCP_PAS_SCAN_ATT_ID;
                valPair[1].valueId  = CSR_BT_AVRCP_PAS_SCAN_VAL_GROUP;

                CsrBtAvrcpCtPasSetReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                         2, valPair);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Changing two pas values: \n\t Equalizer set to ON (2)\n\t Scan Att Id set to 'GROUP' (3)\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable TOGGLE_EQUALIZER_FROM_CT") , TEXT_OK_UCS2, NULL);
             /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_TG_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                CsrCharString displayString[100];

                CsrBtAvrcpPasAttValPair *changedPas = CsrPmemAlloc(sizeof(CsrBtAvrcpPasAttValPair));

                changedPas->attribId = CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID;
                changedPas->valueId = inst->avrcpCon[inst->currentConnection].equalizerOn ? CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF : CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON;


                CsrBtAvrcpTgPasSetReqSend(inst->CsrSchedQid,
                                     0, /* PlayerId */
                                     1,
                                     changedPas);

                //CsrBtSprintf((char *)displayString, "Toggling the equalizer %s from the TG...\n", inst->avrcpCon[inst->currentConnection].equalizerOn ? "OFF" : "ON");
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Not possible when 'EXCLUDE_CSR_BT_AVRCP_TG_MODULE' is defined \n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }
        case CSR_BT_AVRCP_GET_PLAY_LIST_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                CsrBtAvrcpCtGetFolderItemsReqSend(inst->CsrSchedQid,
                                             inst->avrcpCon[inst->currentConnection].connectionId,
                                             CSR_BT_AVRCP_SCOPE_NPL,
                                             0,
                                             3,
                                             0);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Getting playlist (now playing)...\n") , TEXT_OK_UCS2, NULL);
#else
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Error: Compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_PLAY_LIST") , TEXT_OK_UCS2, NULL);
#endif
                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }
        case CSR_BT_GET_MEDIA_ATTRIBUTES_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtGetAttributesReqSend(inst->CsrSchedQid,
                                            inst->avrcpCon[inst->currentConnection].connectionId,
                                            CSR_BT_AVRCP_SCOPE_NPL,
                                            inst->lastUidReceived,
                                            inst->lastUidCounter,
                                            CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);


                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Getting media attributes...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_ITEM_ATTR") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_GET_MEDIA_ATTRIBUTES_UID_LAST_SEARCH_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtGetAttributesReqSend(inst->CsrSchedQid,
                                            inst->avrcpCon[inst->currentConnection].connectionId,
                                            CSR_BT_AVRCP_SCOPE_SEARCH,
                                            inst->lastUidReceived,
                                            inst->lastUidCounter,
                                            CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Getting media attributes (playing)...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_ITEM_ATTR") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_SEND_SEARCH_CIMMAND_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtSearchReqSend(inst->CsrSchedQid, inst->avrcpCon[inst->currentConnection].connectionId, CsrUtf8StrDup("tomorrow"));
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable SEARCH_REQ") , TEXT_OK_UCS2, NULL);
            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_LAST_UID_RECEIVED_IN_SEARCH_GFI_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtGetAttributesReqSend(inst->CsrSchedQid, inst->avrcpCon[inst->currentConnection].connectionId,
                                 CSR_BT_AVRCP_SCOPE_SEARCH, inst->lastUidReceived, inst->lastUidCounter, CSR_BT_AVRCP_ITEM_ATT_MASK_ALL);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("Error: Please compile with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable GET_ITEM_ATTR") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_PLAY_ITEM_FROM_SEARCH_LAST_UID_RECEIVED_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtPlayReqSend(inst->CsrSchedQid,
                       inst->avrcpCon[inst->currentConnection].connectionId,
                       CSR_BT_AVRCP_SCOPE_SEARCH,
                       inst->lastUidCounter,
                       inst->lastUidReceived);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Please compile without the CSR_BT_EXCLUDE_AVRCP_CT_13_14 flag to enable the PLAY-command - For AVRCP 1.0 use pass-through to trigger a play.") , TEXT_OK_UCS2, NULL);
#endif
            break;
        }
        case CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                inst->avrcpCon[inst->currentConnection].tmpVolume = inst->avrcpCon[inst->currentConnection].currentVolume;
                inst->avrcpCon[inst->currentConnection].currentVolume = 0;

                /* Set the menu on the CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI                               */
                CsrBtAvrcpSetInputDialog(inst, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI,
                  NULL, CONVERT_TEXT_STRING_2_UCS2("Enter New Volume:"),
                  CSR_UI_ICON_KEY,  NULL, MAX_EDIT_LENGTH,
                  CSR_UI_KEYMAP_ALPHANUMERIC,TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI on the display                                  */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }
        case CSR_BT_AVRCP_CHANGE_PATH_DOWN_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpCtChangePathReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                         inst->lastUidCounter,
                                         CSR_BT_AVRCP_CHANGE_PATH_DOWN,
                                         inst->lastFolderUidReceived);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Changing path (UID)...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compiler with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable CHANGE_PATH") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        case CSR_BT_AVRCP_CHANGE_PATH_UP_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpUid uidZero = {0,0,0,0,0,0,0,0};
                CsrBtAvrcpCtChangePathReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                         inst->lastUidCounter,
                                         CSR_BT_AVRCP_CHANGE_PATH_UP,
                                         uidZero); /* Ignored */
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Changing path (UP)...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compiler with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable CHANGE_PATH") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;

        }
        case CSR_BT_AVRCP_CHANGE_PATH_WRONG_UID_KEY:
        {
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
            if((inst->avrcpCon[inst->currentConnection].state)== DaAvStateAvrcpConnected)
            {
                CsrBtAvrcpUid uid = WRONG_UID;

                CsrBtAvrcpCtChangePathReqSend(inst->CsrSchedQid,
                                         inst->avrcpCon[inst->currentConnection].connectionId,
                                         WRONG_UID_CNT,
                                         CSR_BT_AVRCP_CHANGE_PATH_UP,
                                         uid);

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("Changing path (UP)...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                     CONVERT_TEXT_STRING_2_UCS2("Error: AVRCP-Disconnected Already - Please try AVRCP-Establish con first") , TEXT_OK_UCS2, NULL);
                 /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
            }
#else
            CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                 CONVERT_TEXT_STRING_2_UCS2("Error: Please compiler with CSR_BT_INSTALL_AVRCP_CT_13_14 to enable CHANGE_PATH") , TEXT_OK_UCS2, NULL);
            CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, inputMode, CSR_BT_AVRCP_STD_PRIO);
#endif
            break;
        }
        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Start or Resume Stream Key \n");
            break;
        }
    }


    /* Unlock the CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);

}


static void appCreateAvrcpEstablishConUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI */
    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlereAvrcpEstablishConUi;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
      if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
      { /* Save the event handle as SK1, and set input mode to blocked        */

         csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
         csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

         CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

         /* Create a BACK event                                              */
         CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
      }
      else
      { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

         CsrCharString displayString[100];

         CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
         CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

         /* Save the event handle as Back, and set input mode to blocked     */
         csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

         CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

         /* Updated the CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI to include all the supported
         client applications                                              */


         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_UID_CNT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change UID counter "), CONVERT_TEXT_STRING_2_UCS2("mpId = 0; UID cnt: 30"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_NOW_PLAYING_CHG_TO_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate now playing "), CONVERT_TEXT_STRING_2_UCS2("change notification to CT (player Id 1)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
         
         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_TRACK_CHG_TO_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate track changed"), CONVERT_TEXT_STRING_2_UCS2("change notification to CT (player Id 1)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_REGISTER_ONE_OR_MORE_MEDIA_PLAYER_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register one more "), CONVERT_TEXT_STRING_2_UCS2("media player"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_UNREGISTER_ONE_MEDIA_PLAYER_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Unregister one "), CONVERT_TEXT_STRING_2_UCS2("media player"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrMemSet((char*)displayString, 0, 100);
         //CsrBtSprintf((char*)displayString, "Current value: %d",inst->avrcpCon[inst->currentConnection].currentVolume);
         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change Volume."), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrMemSet((char*)displayString, 0, 100);
         //CsrBtSprintf((char*)displayString, " (currently: %s)\n",inst->avrcpCon[inst->currentConnection].equalizerOn ? "OFF" : "ON");
         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle the equalizer"), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrMemSet((char*)displayString, 0, 100);
         //CsrBtSprintf((char*)displayString, "(currently: %s)\n",((inst->tgTest == TRUE) ? "ON" : "OFF"));
         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle TG test status "), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

         CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SEND_SET_ADDRESSED_PLAYER_CMD_KEY,
                CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send SetAddrssdPlayer CMD"), CONVERT_TEXT_STRING_2_UCS2("to send RegisterNTFNRject"), sk1Handle,
                CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION)

         /* Create  CSR_BT_AVRCP_ACTIVATE_MENU_UI */
         csrUiVar->uiIndex = CSR_BT_AVRCP_ACTIVATE_MENU_UI;
         CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
      }
    }
}



/* Static functions to create the CSR_BT_AVRCP_ACTIVATE_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlerAvrcpActivateMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_AVRCP_ACTIVATE_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CSR_UNUSED(eventHandle);

    switch (key)
    {
        case CSR_BT_AVRCP_ESTABLISH_CONN_KEY:
        {
            if((inst->avrcpCon[inst->currentConnection].state) == DaAvStateAvrcpDisconnected)
            {

               inst->remoteAddr[inst->currentConnection] = inst->selectedDevice;

               if((inst->selectedDevice.lap != 0) &&
                  (inst->selectedDevice.uap != 0) &&
                  (inst->selectedDevice.nap != 0))
               {
                  startConnectingAvrcp(inst);
               }
               else
               {
               /* Update the CSR_BT_AVRCP_DEFAULT_INFO_UI menu on the display                           */
                  CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                              CONVERT_TEXT_STRING_2_UCS2("Error:Try to establish a connection only after selecting and bonding from the GAP menu") , TEXT_OK_UCS2, NULL);
                  /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                  CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
               }
            }
            else
            {

              CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }

            break;
        }
        case CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY:
        {
            if((inst->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_ONLY) ||
                (inst->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_CT))
            {
                CsrCharString displayString[100];
                inst->tgTest = !inst->tgTest;

                //CsrBtSprintf((char *)displayString, "TG Test status is now %s\n", ((inst->tgTest == TRUE) ? "ON" : "OFF"));
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                    CONVERT_TEXT_STRING_2_UCS2("This option is valid only for the TARGET") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }

            break;
        }
        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Activate Menu Key \n");
            break;
        }
    }



    /* Unlock the CSR_BT_AVRCP_ACTIVATE_MENU_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}

static void appCreateAvrcpActivateMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_ACTIVATE_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerAvrcpActivateMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_AVRCPRCP_ACTIVATE_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, TEXT_AVRCP_ACTIVATE_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            CsrCharString displayString[100];
#endif
            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());



            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_ESTABLISH_CONN_KEY,
                            CSR_UI_ICON_NONE, TEXT_AVRCP_ESTABLISH_CON_UCS2, NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
            CsrMemSet((char*)displayString, 0, 100);
            //CsrBtSprintf((char*)displayString, "(currently: %s)\n",((inst->tgTest == TRUE) ? "ON" : "OFF"));
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY,
                           CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle TG test status "), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                           CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
#endif


            /* Create  CSR_BT_AVRCP_MODE_MENU_UI */
            csrUiVar->uiIndex = CSR_BT_AVRCP_MODE_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
        }
    }
}

/* Static functions to create the CSR_BT_AVRCP_MODE_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlereAvrcpModeMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_AVRCP_MODE_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_AVRCP_CONTROLLER_MODE_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
               #ifdef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_CT_ONLY;
               #endif
               #ifdef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_TG_ONLY;
               #endif
               #if !defined(EXCLUDE_CSR_BT_AVRCP_TG_MODULE) && !defined (EXCLUDE_CSR_BT_AVRCP_CT_MODULE)
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_CT_ONLY;
               #endif

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Controller Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Mode Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        case CSR_BT_AVRCP_TARGET_MODE_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
               #ifdef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_CT_ONLY;
               #endif
               #ifdef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_TG_ONLY;
               #endif
               #if !defined(EXCLUDE_CSR_BT_AVRCP_TG_MODULE) && !defined (EXCLUDE_CSR_BT_AVRCP_CT_MODULE)
                   inst->avrcpConfig = DA_AV_CONFIG_AVRCP_TG_ONLY;
               #endif

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Target Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Mode Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        case CSR_BT_AVRCP_TARGET_CONTROLLER_MODE_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
                #ifdef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                    inst->avrcpConfig = DA_AV_CONFIG_AVRCP_CT_ONLY;
                #endif
                #ifdef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                    inst->avrcpConfig = DA_AV_CONFIG_AVRCP_TG_ONLY;
                #endif
                #if !defined(EXCLUDE_CSR_BT_AVRCP_TG_MODULE) && !defined (EXCLUDE_CSR_BT_AVRCP_CT_MODULE)
                    inst->avrcpConfig = DA_AV_CONFIG_AVRCP_TG_CT;
                #endif

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                            CONVERT_TEXT_STRING_2_UCS2("Target + Controller Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Mode Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Con Mode Menu Key \n");
            break;
        }
    }

    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
    CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

    /* Unlock the CSR_BT_AVRCP_DEFAULT_INFO_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpModeMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_MODE_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlereAvrcpModeMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_AVRCP_MODE_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, TEXT_AVRCP_MODE_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_AVRCP_MODE_MENU_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_CONTROLLER_MODE_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("CONTROLLER"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TARGET_MODE_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("TARGET"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TARGET_CONTROLLER_MODE_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("TARGET + CONTROLLER"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_AVRCP_VERSION_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);


        }
    }
}

/* Static functions to create the CSR_BT_AVRCP_VERSION_MENU_UI and handle key events
   releated to this UI                                                          */
static void appUiKeyHandlereAvrcpVersionMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_AVRCP_VERSION_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_AVRCP_VERSION_1_0_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
               inst->avrcpVersion = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("AVRCP Version 1.0 Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Version Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        case CSR_BT_AVRCP_VERSION_1_3_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
               inst->avrcpVersion = CSR_BT_AVRCP_CONFIG_SR_VERSION_13;

               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("AVRCP Version 1.3 Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Version Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        case CSR_BT_AVRCP_VERSION_1_4_KEY:
        {
            if(inst->state == DaAvStateAppInit)
            {
                inst->avrcpVersion = CSR_BT_AVRCP_CONFIG_SR_VERSION_14;

                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                        CONVERT_TEXT_STRING_2_UCS2("AVRCP Version 1.4 Selected") , TEXT_OK_UCS2, NULL);
            }
            else
            {
               CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                        CONVERT_TEXT_STRING_2_UCS2("Version Cann't be changed once the profile has been activated") , TEXT_OK_UCS2, NULL);
            }
            break;
        }
        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Version Menu Key \n");
            break;
        }
    }

    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
    CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

    /* Unlock the CSR_BT_AVRCP_DEFAULT_INFO_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}



static void appCreateAvrcpVersionMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_VERSION_MENU_UI */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlereAvrcpVersionMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_AVRCP_VERSION_MENU_UI */
        CsrUiMenuSetReqSend(prim->handle, TEXT_AVRCP_MODE_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */

            csrUiVar->eventState = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else
        { /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                           */

            CsrUieHandle displayHandle  = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            /* Save the event handle as Back, and set input mode to blocked     */
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

            /* Updated the CSR_BT_AVRCP_VERSION_MENU_UI to include all the supported
               client applications                                              */
            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_VERSION_1_0_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("AVRCP Version : v1.0"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_VERSION_1_3_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("AVRCP Version : v1.3"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_VERSION_1_4_KEY,
                            CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("AVRCP Version : v1.4"), NULL, sk1Handle,
                            CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            csrUiVar->uiIndex = CSR_BT_AVRCP_MAIN_MENU_UI;
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);


        }
    }
}



static void appUiKeyHandlereAvrcpMainMenu(void * instData,
                   CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key)
{ /* This function handles the SK1 action event from the CSR_BT_AVRCP_MAIN_MENU_UI  */
    avrcpinstance_t * inst = instData;
    CSR_UNUSED(eventHandle);


    switch (key)
    {
        case CSR_BT_AVRCP_ACTIVATE_KEY:
        {
            if((inst->state == DaAvStateAppInit) && (inst->state != DaAvStateAppActive))
            {
                startAVRCPActivate(inst);

                 /* Show the CSR_BT_AVRCP_ACTIVATE_MENU_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_ACTIVATE_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            else
            {
               /* Update the CSR_BT_AVRCP_DEFAULT_INFO_UI menu on the display                           */
                  CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                              CONVERT_TEXT_STRING_2_UCS2("Profile Already activated") , TEXT_OK_UCS2, NULL);
                  /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                  CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, 5);

                   /* Show the CSR_BT_AVRCP_ACTIVATE_MENU_UI on the display                   */
                  CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_ACTIVATE_MENU_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }

        case CSR_BT_AVRCP_DEACTIVATE_KEY:
        {
            if (inst->state == DaAvStateAppActive)
            {
               CsrBtAvrcpDeactivateReqSend();
            }
            else
            {
                CsrBtAvrcpSetDialog(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                                              CONVERT_TEXT_STRING_2_UCS2("Please close connection or active before deactivating") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(inst, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

                /* Unlock the CSR_BT_AVRCP_DEFAULT_INFO_UI                               */
                CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
            }
            break;
        }

        default : /*                                         */
        {
            CsrGeneralException("CSR_BT_AVRCP", 0, ((CsrUiUieCreateCfm *) (inst->recvMsgP))->type,
                        "AVRCP Main Menu Key \n");
            break;
        }
    }



    /* Unlock the CSR_BT_AVRCP_DEFAULT_INFO_UI                               */
    CsrUiDisplaySetinputmodeReqSend(displayHandle, CSR_UI_INPUTMODE_AUTO);
}


static void appCreateAvrcpMainMenuUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_MAIN_MENU_UI                                */
    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_MENU)
    {
        csrUiVar->eventState                                               = CSR_BT_AVRCP_CREATE_SK1_EVENT;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlereAvrcpMainMenu;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Set the menu on the CSR_BT_AVRCP_MAIN_MENU_UI                                */
        CsrUiMenuSetReqSend(prim->handle, TEXT_MAIN_MENU_UCS2, TEXT_SELECT_UCS2, NULL);

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    {
        if (csrUiVar->eventState == CSR_BT_AVRCP_CREATE_SK1_EVENT)
        { /* Save the event handle as SK1, and set input mode to blocked        */
            csrUiVar->eventState                                          = CSR_BT_AVRCP_CREATE_BACK_EVENT;
            csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle  = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_AUTO,  CsrSchedTaskQueueGet());
            /* Create a BACK event                                              */
            CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
        }
        else /* CSR_BT_AVRCP_CREATE_BACK_EVENT                                        */
        { /* Save the event handle as BACK, and set input mode to blocked       */
            CsrUieHandle sk1EventHandle;
            CsrUieHandle sk1Handle      = csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle;

            CsrUieHandle menu = csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle;

            csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle = prim->handle;

            CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK,  CsrSchedTaskQueueGet());

            /* Add all the static menu items to CSR_BT_AVRCP_MAIN_MENU_UI               */


            sk1EventHandle = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ACTIVATE_MENU_UI].displayHandle;

            CsrUiMenuAdditemReqSend(menu, CSR_UI_FIRST, CSR_BT_AVRCP_ACTIVATE_KEY, CSR_UI_ICON_MAGNIFIER,
                                   TEXT_AVRCP_ACTIVATE_MENU_UCS2, NULL, sk1Handle,
                                   CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            CsrUiMenuAdditemReqSend(menu, CSR_UI_LAST, CSR_BT_AVRCP_DEACTIVATE_KEY, CSR_UI_ICON_TOOLS,
                                   TEXT_AVRCP_DEACTIVATE_UCS2, NULL, sk1Handle,
                                   CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);

            sk1EventHandle = csrUiVar->displayesHandlers[CSR_BT_AVRCP_MODE_MENU_UI].displayHandle;

            CsrUiMenuAdditemReqSend(menu, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY, CSR_UI_ICON_TOOLS,
                                   TEXT_AVRCP_MODE_UCS2, NULL, sk1EventHandle,
                                   CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);


            sk1EventHandle = csrUiVar->displayesHandlers[CSR_BT_AVRCP_VERSION_MENU_UI].displayHandle;

            CsrUiMenuAdditemReqSend(menu, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY, CSR_UI_ICON_TOOLS,
                       TEXT_AVRCP_VERSION_UCS2, NULL, sk1EventHandle,
                       CSR_UI_DEFAULTACTION, prim->handle, CSR_UI_DEFAULTACTION);
        }
    }
}


static void appCreateAvrcpDefaultInfoDialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_DEFAULT_INFO_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}


static void appCreateAvrcpEventNotfcn_1_DialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_AVRCP_EVENT_NOTFCN_2_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

static void appCreateAvrcpEventNotfcn_2_DialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_EVENT_NOTFCN_2_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_AVRCP_EVENT_NOTFCN_3_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

static void appCreateAvrcpEventNotfcn_3_DialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_EVENT_NOTFCN_3_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

static void appCreateAvrcpEventNotfcn_CT_DialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        csrUiVar->uiIndex = CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

static void appCreateAvrcpEventNotfcn_TG_DialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        /* Create  CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI                                        */
        csrUiVar->uiIndex = CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
    }
}

static void appCreateAvrcpPASNotfcnDialogUi(avrcpinstance_t * inst)
{ /* This function Creates the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI                               */

    CsrBtAVRCPCsrUiType    * csrUiVar  = &(inst->csrUiVar);
    CsrUiUieCreateCfm * prim     = (CsrUiUieCreateCfm *) inst->recvMsgP;

    if (prim->elementType == CSR_UI_UIETYPE_DIALOG)
    {
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].displayHandle       = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandleFunc  = appUiKeyHandlerHideMics;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandleFunc = appUiKeyHandlerHideMics;

        /* Create a SK1 event                                                   */
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    }
    else
    { /* Save the event handle as SK1/BACK, and set input mode to blocked       */
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].sk1EventHandle      = prim->handle;
        csrUiVar->displayesHandlers[csrUiVar->uiIndex].backEventHandle     = prim->handle;

        CsrUiEventSetReqSend(prim->handle, CSR_UI_INPUTMODE_BLOCK, CsrSchedTaskQueueGet());

        /* Create  CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI                                        */
        csrUiVar->uiIndex = CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI;
        CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
    }
}




/* static Up-stream message                                                     */

static const CsrBtAvrcpEventType csrUiUieCreateCfmHandlers[NUMBER_OF_CSR_BT_AVRCP_APP_UI] =
{
    appCreateAvrcpChangeVolumeFromCTUi,                      /* CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI   */
    appCreateAvrcpChangeVolumeUi,                            /* CSR_BT_AVRCP_CHANGE_VOLUME_UI */
    appCreateAvrcpChooseMediaPlayerUi,                       /* CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI */
    appCreateAvrcpToggleTestMenuUi,                          /* CSR_BT_TOGGLE_TEST_STATUS_MENU_UI */
    appCreateAvrcpEnterFileSystemMenuUi,                     /* CSR_BT_FILE_SYSTEM_MENU_UI */
    appCreateAvrcpEnterNowPlayingMenuUi,                     /* CSR_BT_NOW_PLAYING_MENU_UI */
    appCreateAvrcpEnterUnitInfoSubunitInfoMenuUi,            /* CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI */
    appCreateAvrcpEnterPassThroughMenuUi,                    /* CSR_BT_ENTER_PASS_THROUGH_MENU_UI */
    appCreateAvrcpEnterMediaPlayerMenuUi,                    /* CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI */
    appCreateAvrcpRegisterForNotificationsMenuUi,            /* CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI */
    appCreateAvrcpEstablishConUi,                            /* CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI */
    appCreateAvrcpActivateMenuUi,                            /* CSR_BT_AVRCP_ACTIVATE_MENU_UI */
    appCreateAvrcpModeMenuUi,                                /* CSR_BT_AVRCP_MODE_MENU_UI */
    appCreateAvrcpVersionMenuUi,                             /* CSR_BT_AVRCP_VERSION_MENU_UI */
    appCreateAvrcpMainMenuUi,                                /* CSR_BT_AVRCP_MAIN_MENU_UI */
    appCreateAvrcpDefaultInfoDialogUi,                       /* CSR_BT_AVRCP_DEFAULT_INFO_UI */
    appCreateAvrcpEventNotfcn_1_DialogUi,                    /* CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI */
    appCreateAvrcpEventNotfcn_2_DialogUi,                    /* CSR_BT_AVRCP_EVENT_NOTFCN_2_DIALOG_UI */
    appCreateAvrcpEventNotfcn_3_DialogUi,                    /* CSR_BT_AVRCP_EVENT_NOTFCN_3_DIALOG_UI */
    appCreateAvrcpEventNotfcn_CT_DialogUi,                   /* CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI */
    appCreateAvrcpEventNotfcn_TG_DialogUi,                   /* CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI */
    appCreateAvrcpPASNotfcnDialogUi                          /* CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI */
};

static const CsrBtAvrcpInputDialogGetCfmType csrUiInputDialogGetCfmHandlers[NUMBER_OF_CSR_BT_AVRCP_APP_UI] =
{
    appAvrcpChangeVolumeFromCTInputDialogGetCfmHandler,                   /* CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_UI   */
    appAvrcpChangeVolumeInputDialogGetCfmHandler,                         /* CSR_BT_AVRCP_CHANGE_VOLUME_UI */
    appAvrcpChooseMediaPlayerInputDialogGetCfmHandler,                    /* CSR_BT_AVRCP_CHOOSE_MEDIA_PLAYER_UI */
    NULL,                                                                 /* CSR_BT_TOGGLE_TEST_STATUS_MENU_UI */
    NULL,                                                                 /* CSR_BT_FILE_SYSTEM_MENU_UI */
    NULL,                                                                 /* CSR_BT_NOW_PLAYING_MENU_UI */
    NULL,                                                                 /* CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI */
    NULL,                                                                 /* CSR_BT_ENTER_PASS_THROUGH_MENU_UI */
    NULL,                                                                 /* CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI */
    NULL,                                                                 /* CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_ACTIVATE_MENU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_MODE_MENU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_VERSION_MENU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_MAIN_MENU_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_DEFAULT_INFO_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI */
    NULL,                                                                 /* CSR_BT_AVRCP_EVENT_NOTFCN_2_DIALOG_UI */
    NULL,                                                                 /* CSR_BT_AVRCp_EVENT_NOTFCN_3_DIALOG_UI */
    NULL                                                                  /* CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI */
};

/* CSR_UI upstream handler functions                                              */
void CsrBtAvrcpCsrUiUieCreateCfmHandler(avrcpinstance_t * inst)
{
    if (inst->csrUiVar.uiIndex < NUMBER_OF_CSR_BT_AVRCP_APP_UI)
    {
         csrUiUieCreateCfmHandlers[inst->csrUiVar.uiIndex](inst);
    }
    else
    { /* An unexpected number of CSR_UI CsrUiUieCreateCfm messages
         has been received                                                      */
    }
}

void CsrBtAvrcpCsrUiInputdialogGetCfmHandler(avrcpinstance_t * inst)
{
    CsrUiInputdialogGetCfm * prim = (CsrUiInputdialogGetCfm *) inst->recvMsgP;

    if(csrUiInputDialogGetCfmHandlers[inst->csrUiVar.uiIndex] != NULL)
    {
      csrUiInputDialogGetCfmHandlers[inst->csrUiVar.uiIndex](inst, prim->handle, prim->text);
    }
    else
    {
      /* Input Dialog Get Cfm received for unknown I/P UI */
      /* An unexpected CsrUiInputdialogGetCfm is received                        */
      CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                           "Unexpected CsrUiInputdialogGetCfm is received");
    }

    CsrPmemFree(prim->text);
}

void CsrBtAvrcpCsrUiEventIndHandler(avrcpinstance_t * inst)
{ /* A CsrUiEventInd is received from the CSR_UI layer                             */
    CsrUint8     i;

    CsrBtAVRCPDisplayHandlesType * uiHandlers = inst->csrUiVar.displayesHandlers;
    CsrUiEventInd            * prim       = (CsrUiEventInd *) inst->recvMsgP;

    for ( i = 0; i < NUMBER_OF_CSR_BT_AVRCP_APP_UI; i++)
    { /* Go through the list of known displayes handles to find the one
         that is equal to the received prim->displayElement                     */
        if (prim->displayElement == uiHandlers[i].displayHandle)
        { /* Found a displayed handle that match                                */
            if (prim->event == uiHandlers[i].sk1EventHandle)
            { /* The event is a SK1                                             */
                if (uiHandlers[i].sk1EventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].sk1EventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                        "No function is assign to this SK1 event");
                }
            }
            else if (prim->event == uiHandlers[i].sk2EventHandle)
            { /* The event is a SK2                                             */
                if (uiHandlers[i].sk2EventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].sk2EventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                        "No function is assign to this SK2 event");
                }
            }
            else if (prim->event == uiHandlers[i].backEventHandle)
            { /* The event is a BACK                                            */
                if (uiHandlers[i].backEventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].backEventHandleFunc(inst,
                                prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                        "No function is assign to this BACK event");
                }
            }
            else if (prim->event == uiHandlers[i].deleteEventHandle)
            { /* The event is a DEL                                             */
                if (uiHandlers[i].deleteEventHandleFunc)
                { /* Called the function that is linked to the event            */
                    uiHandlers[i].deleteEventHandleFunc(inst,
                                          prim->displayElement, prim->event, prim->key);
                }
                else
                { /* An exception has occurred. No function is assign to
                     this event                                                 */
                    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                        "No function is assign to this DEL event");
                }
            }
            else
            { /* An exception has occurred. No UIEHandle is assign to
                 this event                                                     */
                    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
                        "No UIEHandle is assign to this event");
            }
            return;
        }
        else
        { /* Continue                                                           */
            ;
        }
    }

    /* An exception has occurred. None of the know displayes handles match
       the received                                                             */
    CsrGeneralException("CSR_BT_AVRCP", 0, prim->type,
        "None of the know displayes handles match the received one");
}

