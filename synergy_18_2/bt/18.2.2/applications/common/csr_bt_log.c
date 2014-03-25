/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_app.h"
#include "csr_bt_log_tech_info_register.h"
#ifdef CSR_BT_APP_AMP_WIFI
#include "csr_wifi_log_tech_info_register.h"
#endif

#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)

/* We support 4 transports: pcap, BTsnoop, Live-FTS, Live-WS */
#define CSR_BT_APP_MAX_LOG_TRANSPORTS 4

/* Log transport/instance containters for fw2+ */
static void *ltPcap = NULL;
static CsrLog *logPcap = NULL;
static void *ltBtsnoop = NULL;
static CsrLog *logBtsnoop = NULL;
#ifdef CSR_LOGTRANSPORT_HAVE_FTSPIPE
static void *ltFts = NULL;
static CsrLog *logFts = NULL;
#endif
#ifdef CSR_LOGTRANSPORT_HAVE_WSPIPE
static void *ltWs = NULL;
static CsrLog *logWs = NULL;
#endif

void CsrBtAppLogInit()
{
    CsrLogInit(CSR_BT_APP_MAX_LOG_TRANSPORTS);
#ifdef CSR_LOGTRANSPORT_HAVE_FILE
    {
        CsrCharString fileStr[128];
        CsrTime logTimeValLow;
        CsrTime logTimeValHigh;

        logTimeValLow = CsrTimeGet(&logTimeValHigh);

        CsrSnprintf(fileStr, 128, "csr-log-%08lu%08lu-pcap.cap", logTimeValHigh, logTimeValLow);
        ltPcap = CsrLogTransportFileOpen(fileStr);
        logPcap = CsrLogPcapCreate(ltPcap);
        CsrLogFormatInstRegister(logPcap);

        CsrSnprintf(fileStr, 128, "csr-log-%08lu%08lu-btsnoop.log", logTimeValHigh, logTimeValLow);
        ltBtsnoop = CsrLogTransportFileOpen(fileStr);
        logBtsnoop = CsrLogBtsnoopCreate(ltBtsnoop);
        CsrLogFormatInstRegister(logBtsnoop);
    }
#endif /* HAVE_FILE */
}

/* Log deinitialisation */
void CsrBtAppLogDeinit()
{
    /* Log deinitilisation for framework 2.x and up */
    if(ltBtsnoop)
    {
        CsrLogTransportFileClose(ltBtsnoop);
    }
    if(ltPcap)
    {
        CsrLogTransportFileClose(ltPcap);
    }
#ifdef CSR_LOGTRANSPORT_HAVE_WSPIPE
    if(ltWs)
    {
        CsrLogTransportWSPipeClose(ltWs);
    }
#endif
#ifdef CSR_LOGTRANSPORT_HAVE_FTSPIPE
    if(ltFts)
    {
        CsrLogTransportFtsPipeClose(ltFts);
    }
#endif
    CsrLogDeinit();
}

/* Enable Wireshark live logging */
void CsrBtAppLogWireshark()
{
#ifdef CSR_LOGTRANSPORT_HAVE_WSPIPE
    ltWs = CsrLogTransportWSPipeOpen(NULL, NULL);
    if (ltWs)
    {
        logWs = CsrLogPcapCreate(ltWs);
        CsrLogFormatInstRegister(logWs);
    }
    else
#endif
    {
        printf("Error: WSPipe log transport failed\n");
        exit(1);
    }
}

/* Enable FTS live logging */
void CsrBtAppLogFrontline()
{
#ifdef CSR_LOGTRANSPORT_HAVE_FTSPIPE
    CsrCharString *filename = "c:\\Program Files\\Frontline Test System II\\Frontline FTS4BT " STRINGIFY(FTS_VER);
    ltFts = CsrLogTransportFtsPipeOpen(filename);
    if (ltFts)
    {
        logFts = CsrLogFtsCreate(ltFts);
        CsrLogFormatInstRegister(logFts);
    }
    else
    {
        printf("Error: FTSPipe log transport failed on %s\n", filename);
        exit(1);
    }
#else
    {
        printf("Error: FTSPipe log transport failed\n");
        exit(1);
    }
#endif
}

/* Apply log levels */
void CsrBtAppLogApply()
{
    CsrLogLevelEnvironmentSet(logEnvLevel);
    CsrLogLevelTaskSetAll(logTaskLevel);

    CsrLogTechInfoRegister();
    CsrBtLogTechInfoRegister();
#ifdef CSR_BT_APP_AMP_WIFI
    CsrWifiLogTechInfoRegister();
#endif
}
