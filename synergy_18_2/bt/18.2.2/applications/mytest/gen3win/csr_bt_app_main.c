/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "stdafx.h"
#include "csr_synergy.h"
#include "csr_bt_app.h"
#include "csr_bt_declare_tasks.h"
#include <windows.h>
#include <commctrl.h>
#include "csr_converters.h"
/* Global demo setup */
char      baudRate[10]             = DEFAULT_BAUDRATE;
char      casira_ComPort[128];
BD_ADDR_T defGlobalBdAddr;
CsrUint8  defGlobalPinLen;
CsrUint8  defGlobalPin[CSR_BT_PASSKEY_MAX_LEN];
CsrUint16 transportType            = TRANSPORT_TYPE_BCSP;
BD_ADDR_T romBdAddress;
CsrUint16 romCrystalFreq;
CsrUint32 romBaud                  = DEFAULT_RESET_BAUDRATE;
CsrUint32 logTaskLevel             = DEFAULT_TASK_LOG_LEVEL;
CsrUint32 logEnvLevel              = DEFAULT_ENV_LOG_LEVEL;
extern void CsrBtAppConverterInit(void);
void CsrPanicCb(CsrUint8 tech, CsrUint16 reason, const char *logtext)
{
    printf("PANIC: %s (tech %hhu, reason %hu)\n", logtext,
        tech, reason);
}

extern void CsrBtConverterInit(void);
int  CsrMain(void)                    
{
     void *schedInstance;
    char *extraOptions;
    void *uartHdl = NULL;

#if defined(__linux__)
    /* Ensure that output is not buffered */
    setbuf(stdout, NULL);
#endif
    CsrMemSet(&defGlobalBdAddr, 0, sizeof(BD_ADDR_T));
    CsrPanicInit(CsrPanicCb);
    CsrPmemInit();

#ifdef CSR_LOG_ENABLE
    CsrBtAppLogInit();
#endif

    /* Parse commandline and setup operating environment */
    extraOptions = applicationExtraOptions();
  //  CsrBtAppCmdlineParse(argc, argv, extraOptions);
#ifdef CSR_LOG_ENABLE
    CsrBtAppLogApply();
    CsrBtAppConverterInit();
#endif


     CsrConverterInit();
    CsrBtConverterInit();
    CsrBtAppTransportSetDefault(transportType);

    uartHdl = CsrBtAppTransportInit();

    CsrBtAppTransportSetTask(transportType);
    CsrBtBootstrapSetInterface(NULL, transportType);

    /* Start scheduler */
    schedInstance = CsrSchedInit(ID_STACK,0,0);
    schedInstance = CsrSchedInit(ID_APP,0,0);

    CsrSched(schedInstance);

    /* Graceful shutdown */
#ifdef ENABLE_SHUTDOWN
    CsrSchedDeinit(schedInstance);
#ifdef CSR_LOG_ENABLE
    CsrBtAppLogDeinit();
#endif

    CsrMsgConvDeinit();
    CsrPmemDeinit();
#endif /* ENABLE_SHUTDOWN */

    CsrBtAppTransportDeinit(uartHdl);
    return (int) 0;
}

