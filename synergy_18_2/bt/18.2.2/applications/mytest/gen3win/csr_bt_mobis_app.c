#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include "csr_synergy.h"
#include "csr_bt_app.h"
#include "csr_bt_declare_tasks.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_pac_lib.h"
#include "csr_bt_hf_lib.h"
#include "csr_bccmd_lib.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_spp_lib.h"

//#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_mobis_app.h"




               					
//#define NUCLEUS_MSG_SIZE 3
//ScAppInstData_t *inst;
btAppInstance_t *btAppInstance = NULL;


#if defined( _WIN32 ) || defined(__linux__)
void CL_API_LOGPrintf(const char *fm,...)
{

  va_list ap;
  va_start(ap,fm);
  vprintf(fm,ap);
  va_end(ap);

}
#endif



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
void CsrBtAppInit(void ** gash)
{
//	 CsrBtScActivateReqSend(TESTQUEUE);
//	 CsrBtCmWriteScanEnableReqSend(TESTQUEUE ,FALSE,FALSE);
//	 CsrBtCmSetLocalNameReqSend(TESTQUEUE , (CsrUtf8String *) CsrStrDup("MYDEVCE"));
	btAppInstance_t * instData;

//	CL_API_LOGPrintf("\r\n enter CsrBtAppInit");


	*gash = (void*)CsrPmemAlloc(sizeof(btAppInstance_t));
	instData= (btAppInstance_t * )*gash;
	btAppInstance = instData;

	init_sd((void**)&(instData->sdinstData));
	init_sc((void**)&(instData->scinstData));
	init_cm((void**)&(instData->cminstData));
	init_hf((void**)&(instData->hfinstData));
	init_pbac((void**)&(instData->pbacinstData));
	init_av2((void**)&(instData->av2instData));
	init_avrc((void**)&(instData->avrcinstData));
	init_spp((void**)&(instData->sppinstData));
#if defined( _WIN32 ) || defined(__linux__)
	init_key((void**)&(instData));
#endif
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
void CsrBtAppHandler(void ** gash)
{
	CsrUint16                eventType;
 	void                    *prim;
	btAppInstance_t * instData = (btAppInstance_t *) (*gash);

    CsrSchedMessageGet(&eventType, &prim);
//    instData->msgP = prim;

	/* A message has been retrived from the Scheduler's Nucleus queue  */

	switch (eventType)
	{

		case CSR_BT_SC_PRIM:
		{
			instData->scinstData->recvMsgP=prim;
			handleScPrim( instData->scinstData);
			break;
		}
		case CSR_BT_SD_PRIM:     
		{
			instData->sdinstData->recvMsgP=prim;
			handleSdPrim(instData->sdinstData);	
			break;
		}
		case CSR_BT_CM_PRIM:
		{
			instData->cminstData->recvMsgP=prim;
			handleCmPrim(instData->cminstData);					
			break;
		}
        case CSR_BT_HF_PRIM:
        {
			instData->hfinstData->recvMsgP=prim;
            handleHfPrim(instData->hfinstData);
            break;
        }
		case CSR_BT_PAC_PRIM:
		{
			instData->pbacinstData->recvMsgP=prim;
			handlePbacPrim(instData->pbacinstData);
			break;
		}

		case  CSR_BT_AV_PRIM:
		{
			instData->av2instData->recvMsgP=prim;
			handleAv2Prim(instData->av2instData);
			break;
		}
		
		case  CSR_BT_AVRCP_PRIM:
		{
			instData->avrcinstData->recvMsgP=prim;
			handleAvrcPrim(instData->avrcinstData);
			break;
		}
		
		case  CSR_BT_SPP_PRIM:
		{
			instData->sppinstData->recvMsgP=prim;
			handleSppPrim(instData->sppinstData);
			break;
		}

		

#if defined( _WIN32 ) || defined(__linux__)
		case KEY_MESSAGE:
        {
            handlekeyPrim(instData,(CsrBtKeyPress_t *)prim);
            break;
        }		
#endif
		default:
		{
			/* Unexpected primitive received */
			CL_API_LOGPrintf("CsrBtAppHandler Unexpected primitive received  %x\n",eventType);
			break;
		}

	}
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

	deinit_sc(&(btInstData->scinstData));

	deinit_cm(&(btInstData->scinstData));
	
	deinit_sd(&(btInstData->sdinstData));	

	deinit_pbac(&(btInstData->pbacinstData));
	
	deinit_hf(&(btInstData->hfinstData));

	deinit_av2(&(btInstData->av2instData));

	deinit_avrc(&(btInstData->avrcinstData));
	
	deinit_spp(&(btInstData->sppinstData));
#if defined( _WIN32 ) || defined(__linux__)
	deinit_key(&(btInstData));
#endif

	    /*  Get a message from the demoapplication message queue. The
     *  message is returned in prim and the event type in eventType */
    while(CsrSchedMessageGet(&eventType, &prim) )
    {
        switch(eventType)
        {
            case CSR_BT_PAC_PRIM:
            {
                CsrBtPacFreeUpstreamMessageContents(eventType, prim);
                break;
            }
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
			case CSR_BT_HF_PRIM:
            {
                CsrBtHfFreeUpstreamMessageContents(eventType, prim);
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
			case CSR_BT_SPP_PRIM:
			{
                CsrBtSppFreeUpstreamMessageContents(eventType, prim);
                break;
            }
            default:
            { /* Just ignore    */
                break;
            }
        }
        CsrPmemFree(prim);
    }
	

	CsrPmemFree(btInstData);
	*gash = NULL;
	btAppInstance = NULL;
}
#endif