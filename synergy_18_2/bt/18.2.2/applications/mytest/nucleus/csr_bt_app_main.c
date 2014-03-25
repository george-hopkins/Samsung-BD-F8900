#include <stdlib.h>
#include <stdio.h>
#include "csr_synergy.h"
#include <nucleus.h>
#if 1 /* JCKim111110  */
#include "csr_bt_app.h"
#include "csr_bt_declare_tasks.h"

#endif	/* JCKim111110//  */
               					
/* Common demo setup vars */

char baudRate[10] = DEFAULT_BAUDRATE;
char casira_ComPort[128] = "uart1";
CsrUint16 transportType = TRANSPORT_TYPE_BCSP;
//CsrBool bdAddrSet = FALSE;
//BD_ADDR_T defGlobalBdAddr = {0,0,0};
//CsrUint8  defGlobalPinLen = 0;
//CsrUint8  defGlobalPin[CSR_BT_PASSKEY_MAX_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BD_ADDR_T romBdAddress = {0x123456,0xd2,0x12};
CsrUint16 romCrystalFreq = 0;
CsrUint32 romBaud = DEFAULT_RESET_BAUDRATE;

void CsrPaniccb(CsrUint8 tech, CsrUint16 reason, const char *p)
{
	CL_API_LOGPrintf("\r\n PANIC:%s (tech %hhu, reson %hu)", p, tech, reason);
}


void BtMainTask(UNSIGNED argc, VOID *argv)
{
	void *schedInstance;
	//int Status;
	void * uartHdl = NULL;
	char *extraOptions;
	   
    CsrPanicInit(CsrPaniccb);

    /* Init COAL memory systems */
    CsrPmemInit();

#ifdef CSR_BT_APP_AMP_WIFI
		CsrThreadInit(1);
#endif
	
#if 0
		/* Initialise Nucleus FILE Message (NUFM) and pass pointer to ramdisk */
		NUFM_Initialise(20, 1, RAMDISK_SIZE & ~0x3, (VOID *) RAMDISK_START_ADDRESS);
#endif
	
#ifdef CSR_LOG_ENABLE
		CsrBtAppLogInit();
#endif
	
#ifdef CSR_LOG_ENABLE
		CsrBtAppLogApply();
		CsrBtAppConverterInit();
#endif
    /* Parse commandline and setup operating environment */
    extraOptions = applicationExtraOptions();
    CsrBtAppCmdlineParse(0,NULL, extraOptions);
	
	CsrBtAppTransportSetDefault(transportType);
	uartHdl=CsrBtAppTransportInit();
	CsrBtAppTransportSetTask(transportType);
	CsrBtBootstrapSetInterface(NULL, transportType);
	
    /* Initialise CSR Scheduler */
    CL_API_LOGPrintf("\r\n  Initialise CSR Scheduler ");
	/* Initialise CSR Scheduler */
	schedInstance = CsrSchedInit(ID_STACK,0,0);
	schedInstance = CsrSchedInit(ID_APP,0,0);
	
	/* Start the scheduler threads */
	CsrSched(schedInstance);
	
	CsrBtAppTransportDeinit(uartHdl);

}

//#endif

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
