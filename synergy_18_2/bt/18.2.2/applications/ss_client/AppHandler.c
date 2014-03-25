/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "csr_bt_ss_prim.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_samsung_log.h"
#include "AppHandler.h"


extern void CsrBtGapAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length);
extern void CsrBtAvAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length);
extern void CsrBtAVRCAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length);
extern void CsrBtHIDHAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length);


void CsrBtAppEventHandler(CsrUint16 id,  CsrUint16 cmd,CsrUint16 payload_length, void* data)
{
	switch(id)
	 { 
		case CSR_CM_SS_ID:
		case CSR_SD_SS_ID:
		case CSR_SC_SS_ID:	
			CsrBtGapAppEventHandler(cmd,data,payload_length);
			break;
		case CSR_AV_SS_ID:								
			CsrBtAvAppEventHandler(cmd,data,payload_length);
			break;
	  
		case CSR_AVRCP_SS_ID:
			CsrBtAVRCAppEventHandler(cmd,data,payload_length);
			break;
		case CSR_HIDH_SS_ID:
			CsrBtHIDHAppEventHandler(cmd,data,payload_length);
			break;		
	 }

}

