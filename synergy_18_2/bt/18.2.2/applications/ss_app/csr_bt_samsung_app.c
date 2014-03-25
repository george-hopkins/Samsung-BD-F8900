#include <stdlib.h>
#include <stdio.h>
#include "csr_synergy.h"
#include "csr_bt_app.h"
#include "csr_bt_declare_tasks.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bccmd_lib.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_hidh_lib.h"
#include "csr_bt_samsung_app.h"
#include "csr_bt_ss_prim.h"
#include "csr_samsung_log.h"
#if 0
#include "csr_ip_socket_lib.h"
#include "csr_msgconv.h"
#include "csr_converters.h"
#endif

//btAppInstance_t *btAppInstance = NULL;
CSR_LOG_TEXT_HANDLE_DEFINE(CSR_SS_AP_LOG_ID);

CsrBool handle_cm_server_cmd_handler( CsrUint16 cmd, void *data, CmAppInst *inst);
CsrBool handle_sc_server_cmd_handler( CsrUint16 cmd, void *data,  ScAppInst *inst);
CsrBool handle_sd_server_cmd_handler( CsrUint16 cmd, void *data, SdAppInst *inst);
CsrBool handle_av_server_cmd_handler( CsrUint16 cmd, void *data, Av2AppInst *inst);
CsrBool handle_avrc_server_cmd_handler( CsrUint16 cmd, void *data, AvrcAppInst *inst);
CsrBool handle_hidh_server_cmd_handler( CsrUint16 cmd, void *data, HidhAppInst *inst);	

void handleUiPrim(btAppInstance_t * instData, CsrBtUiMessage_t *the_prim)
{
	////////////////////////////////////////handle message from client process ///////////
	switch(the_prim->id)
	{
		case CSR_CM_SS_ID:
		{
			handle_cm_server_cmd_handler(the_prim->cmd, the_prim->data, instData->cminstData);
		}
		break;
		case CSR_SC_SS_ID:
		{
			handle_sc_server_cmd_handler(the_prim->cmd, the_prim->data, instData->scinstData);
		}
		break;
		case CSR_SD_SS_ID:
		{
			handle_sd_server_cmd_handler(the_prim->cmd, the_prim->data, instData->sdinstData);
		}
		break;
		case CSR_AV_SS_ID:
		{
			handle_av_server_cmd_handler(the_prim->cmd, the_prim->data, instData->av2instData);
		}
		break;
		case CSR_AVRCP_SS_ID:
		{
			handle_avrc_server_cmd_handler(the_prim->cmd, the_prim->data, instData->avrcinstData);
		}
		break;
		
		case CSR_HIDH_SS_ID:
	    {
	      handle_hidh_server_cmd_handler(the_prim->cmd, the_prim->data, instData->hidhinstData);    
	    }
		break;
		default:
		{
			/* Unexpected primitive received */
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"CsrBtAppHandler Unexpected BT_UI_MESSAGE id received  %x\n",the_prim->id));
		}
		break;

	}

	if(the_prim->data)
	{
		CsrPmemFree(the_prim->data);
		the_prim->data = NULL;	
	}
}



/**************************************************************************
*  FUNCTION NAME :
*      void CsrBtAppInit(void **gash);
*
*  DESCRIPTION :
*  INPUT:
*			gash	=
*
*  OUTPUT:	void - Return Type
*  REMARK  :
**************************************************************************/
void CsrBtAppInit(void **gash)
{
	btAppInstance_t * instData;
	
	CSR_LOG_TEXT_REGISTER(&CSR_SS_AP_LOG_ID, CSR_SS_AP_LOG_STR, 0, NULL);

	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," enter CsrBtAppInit"));

	*gash = (void*)CsrPmemAlloc(sizeof(btAppInstance_t));
	instData= (btAppInstance_t * )*gash;
//	btAppInstance = instData;
	init_sd((void**)&(instData->sdinstData));
	init_sc((void**)&(instData->scinstData));
	init_cm((void**)&(instData->cminstData));
	init_av2((void**)&(instData->av2instData));
	init_avrc((void**)&(instData->avrcinstData));
	init_hidh((void**)&(instData->hidhinstData));

}

/**************************************************************************
*  FUNCTION NAME :
*      void CsrBtAppHandler(void **gash);
*
*  DESCRIPTION :
*  INPUT:
*			gash	=
*
*  OUTPUT:	void - Return Type
*  REMARK  :
**************************************************************************/
void CsrBtAppHandler(void **gash)
{
	CsrUint16                eventType;
 	void                    *prim; 
	btAppInstance_t * instData = (btAppInstance_t *) (*gash);

    CsrSchedMessageGet(&eventType, &prim);

	/* A message has been retrived from the Scheduler's Nucleus queue  */

	switch (eventType)
	{
		case CSR_BT_SC_PRIM:
		{
			handleScPrim( instData->scinstData,prim);
			break;
		}
		case CSR_BT_SD_PRIM:     
		{
			handleSdPrim(instData->sdinstData,prim);	
			break;
		}
		case CSR_BT_CM_PRIM:
		{
			handleCmPrim(instData->cminstData,prim);					
			break;
		}
		case  CSR_BT_AV_PRIM:
		{
			handleAv2Prim(instData->av2instData,prim);
			break;
		}
		case CSR_BT_AVRCP_PRIM:
	    {
			handleAvrcPrim(instData->avrcinstData,prim);
			break;
		}
		case CSR_BT_HIDH_PRIM:
		{
			handleHidhPrim(instData->hidhinstData, prim);
			break;
		}
		case BT_UI_MESSAGE:
		{
			handleUiPrim(instData,(CsrBtUiMessage_t *)prim);
			break;
		}			
#if 0		
		case CSR_IP_SOCKET_PRIM:
	    {
			handleServerSocket((CsrPrim * )prim);
             break;
		} 
#endif		
		default:
		{
			/* Unexpected primitive received */
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"CsrBtAppHandler Unexpected primitive received  %x\n",eventType));
			break;
		}

	}
	 /*    free the received signal again. If the signal for some reason must be stored in the application
          the pointer (prim) must be set to NULL in order not to free it here */
	CsrPmemFree(prim);
	prim=NULL;
}



#ifdef ENABLE_SHUTDOWN
/**************************************************************************
*  FUNCTION NAME :
*      void CsrBtAppDeinit(void **gash);
*
*  DESCRIPTION :
*  INPUT:
*			gash	=
*
*  OUTPUT:	void - Return Type
*  REMARK  :
**************************************************************************/
void CsrBtAppDeinit(void **gash)
{
	btAppInstance_t *btInstData = *gash;
	CsrUint16                eventType;
 	void                    *prim;

	deinit_sc((void **)&(btInstData->scinstData));

	deinit_cm((void **)&(btInstData->scinstData));
	
	deinit_sd((void **)&(btInstData->sdinstData));	


	deinit_av2((void **)&(btInstData->av2instData));

	deinit_avrc((void **)&(btInstData->avrcinstData));
	
	deinit_hidh((void **)&(btInstData->hidhinstData));

	    /*  Get a message from the demoapplication message queue. The
     *  message is returned in prim and the event type in eventType */
    while(CsrSchedMessageGet(&eventType, &prim) )
    {
        switch(eventType)
        {

            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(eventType,prim);
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                CsrBtSdFreeUpstreamMessageContents(eventType, prim);
                break;
            }
			case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(eventType, prim);
                break;
            }
            case CSR_BCCMD_PRIM:
            {
                CsrBccmdFreeUpstreamMessageContents(eventType, prim);
                break;
            }
			case CSR_BT_AV_PRIM:
			{
                CsrBtAvFreeUpstreamMessageContents(eventType, prim);
                break;
            }
			case CSR_BT_AVRCP_PRIM:
			{
                CsrBtAvrcpFreeUpstreamMessageContents(eventType, prim);
                break;
            }
			case CSR_BT_HIDH_PRIM:
            {
                CsrBtHidhFreeUpstreamMessageContents(eventType, prim);
                break;
            }
			
#if 0			
			case CSR_IP_SOCKET_PRIM:
			{
				CsrIpSocketFreeUpstreamMessageContents(eventType, prim);
				break;
			}
#endif			
            default:
            { /* Just ignore    */
                break;
            }
        }
        CsrPmemFree(prim);
    }
	

	CsrPmemFree(btInstData);
	*gash = NULL;
//	btAppInstance = NULL;
}
#endif
