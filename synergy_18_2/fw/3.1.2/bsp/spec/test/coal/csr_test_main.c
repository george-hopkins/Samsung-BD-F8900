/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_usr_config.h"

#include "csr_types.h"
#include "csr_arg_search.h"
#include "csr_result.h"
#include "csr_sched.h"

#include "csr_sched_init.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_pcap.h"
#include "csr_log_fts.h"
#include "csr_log_register.h"
#include "platform/csr_logtransport_init.h"

#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)
#endif

#include "platform/csr_pmem_init.h"
#include "csr_log.h"

#include "csr_sched_test.h"

CsrResult CsrMain(void)
{
    void *schedInstance;
    CsrUint16 i;
#ifdef CSR_LOG_ENABLE
    CsrCharString *parameter;
    CsrCharString *value;
#endif

    /* Init Pmem */
    CsrPmemInit();

    /* Initialise Log */
#ifdef CSR_LOG_ENABLE
    CsrLogInit(8);
#endif

    /* Wireshark File Logging */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_FILE)
    if (CsrArgSearch(NULL, "--log-pcap-file", &parameter, &value))
    {
        void *ltPcapFile;
        CsrCharString *filename = NULL;

        if (value != NULL)
        {
            filename = value;
        }
        else
        {
            filename = "pcap.cap";
        }

        ltPcapFile = CsrLogTransportFileOpen(filename);

        if (ltPcapFile)
        {
            CsrLog *logPcapFile = CsrLogPcapCreate(ltPcapFile);
            CsrLogFormatInstRegister(logPcapFile);
        }
        else
        {
            printf("Unable to open Wireshark log transport on file %s\n", filename);
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    /* Wireshark Live Logging */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_WSPIPE)
    if (CsrArgSearch(NULL, "--log-pcap-live", &parameter, &value))
    {
        void *ltPcapLive = CsrLogTransportWSPipeOpen(NULL, NULL);
        if (ltPcapLive)
        {
            CsrLog *logPcapPipe = CsrLogPcapCreate(ltPcapLive);
            CsrLogFormatInstRegister(logPcapPipe);
        }
        else
        {
            printf("Unable to open Wireshark Pipe log transport\n");
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    /* FTS File Logging */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_FILE)
    if (CsrArgSearch(NULL, "--log-fts-file", &parameter, &value))
    {
        void *ltFtsFile;
        CsrCharString *filename = NULL;

        if (value != NULL)
        {
            filename = value;
        }
        else
        {
            filename = "fts.log";
        }

        ltFtsFile = CsrLogTransportFileOpen(filename);

        if (ltFtsFile)
        {
            CsrLog *logFtsFile = CsrLogFtsCreate(ltFtsFile);
            CsrLogFormatInstRegister(logFtsFile);
        }
        else
        {
            printf("Unable to open FTS log transport on file %s\n", filename);
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    /* FTS Live Logging */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_FTSPIPE)
    if (CsrArgSearch(NULL, "--log-fts-live", &parameter, &value))
    {
        void *ltFtsLive;
        CsrCharString *filename = NULL;

        if (value != NULL)
        {
            filename = value;
        }
        else
        {
            filename = "c:\\Program Files\\Frontline Test System II\\Frontline FTS4BT " STRINGIFY(FTS_VER);
        }

        ltFtsLive = CsrLogTransportFtsPipeOpen(filename);

        if (ltFtsLive)
        {
            CsrLog *logFtsLive = CsrLogFtsCreate(ltFtsLive);
            CsrLogFormatInstRegister(logFtsLive);
        }
        else
        {
            printf("Unable to open FTS Pipe log transport on %s\n", filename);
            return CSR_RESULT_FAILURE;
        }
    }
#endif

#ifdef CSR_LOG_ENABLE
    {
        CsrLogTechInfoRegister();
    }

    if (CsrArgSearch(NULL, "--log-level", &parameter, &value))
    {
        CsrLogLevelEnvironment logLevelEnv;
        CsrLogLevelTask logLevelTask;
        CsrLogLevelText logLevelText;

        if (value == NULL)
        {
            printf("No value specified for parameter --log-level\n");
            return CSR_RESULT_FAILURE;
        }

        if (sscanf(value, "0x%8lx:0x%8lx:0x%8lx", &logLevelEnv, &logLevelTask, &logLevelText) == 3)
        {
            CsrLogLevelEnvironmentSet(logLevelEnv);
            CsrLogLevelTaskSetAll(logLevelTask);
            CsrLogLevelTextSetAll(logLevelText);
        }
        else
        {
            printf("Invalid value '%s' for parameter %s (format: '0xXXXXXXXX:0xXXXXXXXX:0xXXXXXXXX')\n", value, parameter);
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    init_ts_verdict();

    for (i = 0; i < CSR_SCHEDULER_INSTANCES; i++)
    {
        schedInstance = CsrSchedInit(i, 0, 0);
    }

    CsrSched(schedInstance);

    deinit_ts_verdict();

    return 0;
}
