/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "csr_types.h"
#include "csr_time.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_memstat.h"
#include "csr_macro.h"

#include "csr_transport.h"
#include "csr_msgconv.h"
#include "csr_converters.h"
#include "csr_ui_keycode.h"
#include "csr_ui_lower.h"
#include "csr_main.h"
#include "csr_app_lower.h"
#include "csr_app_main.h"
#include "csr_arg_search.h"
#include "csr_framework_ext.h"

#include "platform/csr_pmem_init.h"
#include "platform/csr_serial_init.h"
#include "platform/csr_panic_init.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_pcap.h"
#include "csr_log_cleartext.h"
#include "csr_log_fts.h"
#include "csr_log_btsnoop.h"
#include "csr_log_register.h"
#include "platform/csr_logtransport_init.h"
#endif

#include "csr_usr_config.h"
#include "csr_app_main_transport.h"
#include "peer_com_linux_u2u.h"

#include "csr_sched_init.h"
#include "csr_samsung_log.h"

#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)
 


CsrUint32 SSLogLevelText;
CsrUint32 SSIsPaic=FALSE;

extern void CsrAppBtGapError(void);

static void panic(CsrUint8 tech, CsrUint16 reason, const char *logtext)
{
    printf("[CSR_DBG] PANIC: %s (tech %u, reason %u)\n", logtext, tech, reason);
	SSIsPaic=TRUE;
	CsrAppBtGapError();
}

/*
static void appExit(void)
{
#ifdef ENABLE_SHUTDOWN
    CsrSchedStop();
#else
    exit(0);
#endif
}
*/
#ifdef CSR_MEMALLOC_PROFILING
static void leakDump(void *ptr, CsrSize sz,
    const CsrCharString *file, CsrUint32 line)
{
    printf("Leak: %p: %u bytes, '%s:%u'\n", ptr, sz, file, line);
}

#endif

#ifdef CSR_LOG_ENABLE
static CsrCharString *logLevelTextString = NULL;
static CsrCharString *seekDelimiter(CsrCharString *str, CsrCharString delimiter)
{
    while ((*str != delimiter) && (*str != '\0'))
    {
        str += 1;
    }
    if (*str == '\0')
    {
        str = NULL;
    }
    else
    {
        *str = '\0';
        str += 1;
    }
    return str;
}

static CsrLogLevelText parseLogLevel(CsrCharString *str)
{
    CsrLogLevelText logLevel = CSR_LOG_LEVEL_TEXT_OFF;
    while (*str != '\0')
    {
        switch (*str)
        {
            case 'C':
                logLevel |= CSR_LOG_LEVEL_TEXT_CRITICAL;
                break;
            case 'E':
                logLevel |= CSR_LOG_LEVEL_TEXT_ERROR;
                break;
            case 'W':
                logLevel |= CSR_LOG_LEVEL_TEXT_WARNING;
                break;
            case 'I':
                logLevel |= CSR_LOG_LEVEL_TEXT_INFO;
                break;
            case 'D':
                logLevel |= CSR_LOG_LEVEL_TEXT_DEBUG;
                break;
            case 'A':
                logLevel |= CSR_LOG_LEVEL_TEXT_ALL;
                break;
            default:
                break;
        }
        str += 1;
    }
    return logLevel;
}

#endif

CsrResult CsrMain(void)
{
    CsrCharString *parameter;
    CsrCharString *value;
    void *schedInstance;
    CsrUint16 i;
    CsrResult result;
    //CsrBool guiEnable = TRUE;
    //CsrBool tuiEnable = FALSE;
    //CsrBool tuiNoCls = FALSE;

    if (!CsrArgSearchValidate())
    {
        printf(CSR_SS_AP_LOG_STR "Invalid command line, use --help for usage.\n");
        return CSR_RESULT_FAILURE;
    }

    if (CsrArgSearch(NULL, "--help", NULL, NULL))
    {
        printf("Command line parameters:\n");
        printf("--log-cleartext-stdout [format] - Enable clear text logging to stdout\n");
        printf("--log-cleartext-stdout-schedevents - Enable sched events on the clear text logging\n");
        printf("--log-pcap-file [filename] - Enable Wireshark PCAP logging to file\n");
        printf("--log-pcap-live - Enable Wireshark PCAP live logging\n");
        printf("--log-btsnoop-file [filename] - Enable Btsnoop logging to file\n");
        printf("--log-fts-live [ftspath] - Enable FTS live logging\n");
        printf("--log-level <0xXXXXXXXX:0xXXXXXXXX:0xXXXXXXXX> - Configure log level for environment:task:logtext\n");
		printf("--ss-log <0xX> samsung log\n");
        printf("--log-level-text [origin].[suborigin].[<CEWIDA>] - Configure log level text. Use : to separate multiple arguments\n");
        printf("--tui - Enable Text User Interface (TUI)\n");
        printf("--tui-nocls - Disable clear screen between each redraw in TUI\n");
        CsrAppMainTransportUsage();
        CsrAppMainUsage();
        return CSR_RESULT_SUCCESS;
    }

    /* Disable output buffering */
    setbuf(stdout, NULL);

    /* Init Pmem */
    CsrPmemInit();
#ifdef CSR_MEMALLOC_PROFILING
    CsrMemStatInit();
#endif

    /* Install panic handler */
    CsrPanicInit(panic);

    /* Initialise Transport */
    result = CsrAppMainTransportInitialise();
    if (result != CSR_RESULT_SUCCESS)
    {
        return result;
    }

    /* Initialise Log */
#ifdef CSR_LOG_ENABLE
    CsrLogInit(8);
#endif

    /* Cleartext logging to stdout */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_STDOUT)
    if (CsrArgSearch(NULL, "--log-cleartext-stdout", &parameter, &value))
    {
        void *ltClearTextStdout;
        CsrLog *logClearTextStdOut;
        CsrBool schedEvents = CsrArgSearch(NULL, "--log-cleartext-stdout-schedevents", NULL, NULL) != NULL;

        ltClearTextStdout = CsrLogTransportStdoutOpen();
        logClearTextStdOut = CsrLogCleartextCreate(ltClearTextStdout, value, schedEvents);

        if (ltClearTextStdout && logClearTextStdOut)
        {
            CsrLogFormatInstRegister(logClearTextStdOut);
        }
        else
        {
            printf(CSR_SS_AP_LOG_STR "Unable to open stdout log transport using format: %s\n", value ? value : CSR_LOG_CLEARTEXT_FORMAT);
            return CSR_RESULT_FAILURE;
        }
    }
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
            printf(CSR_SS_AP_LOG_STR "Unable to open Wireshark log transport on file %s\n", filename);
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
            printf(CSR_SS_AP_LOG_STR "Unable to open Wireshark Pipe log transport\n");
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    /* Btsnoop File Logging */
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOGTRANSPORT_HAVE_FILE)
    if (CsrArgSearch(NULL, "--log-btsnoop-file", &parameter, &value))
    {
        void *ltBtsnoopFile;
        CsrCharString *filename = NULL;

        if (value != NULL)
        {
            filename = value;
        }
        else
        {
            filename = "btsnoop.log";
        }

        ltBtsnoopFile = CsrLogTransportFileOpen(filename);

        if (ltBtsnoopFile)
        {
            CsrLog *logBtsnoopFile = CsrLogBtsnoopCreate(ltBtsnoopFile);
            CsrLogFormatInstRegister(logBtsnoopFile);
        }
        else
        {
            printf(CSR_SS_AP_LOG_STR"Unable to open Btsnoop log transport on file %s\n", filename);
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
            printf(CSR_SS_AP_LOG_STR"Unable to open FTS Pipe log transport on %s\n", filename);
            return CSR_RESULT_FAILURE;
        }
    }
#endif

    /* Application Specific Initialisation */
    result = CsrAppMain();
    if (result != CSR_RESULT_SUCCESS)
    {
        return result;
    }

#ifdef CSR_LOG_ENABLE
    {
        CsrLogTechInfoRegister();
    }
#endif

#ifdef CSR_LOG_ENABLE
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

        if (sscanf(value, "0x%8x:0x%8x:0x%8x", &logLevelEnv, &logLevelTask, &logLevelText) == 3)
        {
            CsrLogLevelEnvironmentSet(logLevelEnv);
            CsrLogLevelTaskSetAll(logLevelTask);
            CsrLogLevelTextSetAll(logLevelText);
        }
        else
        {
            printf(CSR_SS_AP_LOG_STR"Invalid value '%s' for parameter %s (format: '0xXXXXXXXX:0xXXXXXXXX:0xXXXXXXXX')\n", value, parameter);
            return CSR_RESULT_FAILURE;
        }
    }
    if (CsrArgSearch(NULL, "--log-level-text", &parameter, &value))
    {
        CsrCharString *logLevelTextTmp;
        CsrCharString *origin = NULL;
        CsrCharString *subOrigin = NULL;
        CsrCharString *logLevel = NULL;

        if (value == NULL)
        {
            printf(CSR_SS_AP_LOG_STR"No value specified for parameter --log-level-text\n");
            return CSR_RESULT_FAILURE;
        }

        logLevelTextString = logLevelTextTmp = CsrStrDup(value);

        do
        {
            origin = logLevelTextTmp;
            logLevelTextTmp = seekDelimiter(logLevelTextTmp, '.');
            if (logLevelTextTmp != NULL)
            {
                subOrigin = logLevelTextTmp;
                logLevelTextTmp = seekDelimiter(logLevelTextTmp, '.');
                if (logLevelTextTmp != NULL)
                {
                    logLevel = logLevelTextTmp;
                    logLevelTextTmp = seekDelimiter(logLevelTextTmp, ':');
                    CsrLogLevelTextSet(origin, subOrigin, parseLogLevel(logLevel));
                }
                else
                {
                    printf(CSR_SS_AP_LOG_STR"Invalid value '%s' for parameter %s\n", value, parameter);
                    return CSR_RESULT_FAILURE;
                }
            }
            else
            {
                printf(CSR_SS_AP_LOG_STR"Invalid value '%s' for parameter %s\n", value, parameter);
                return CSR_RESULT_FAILURE;
            }
        } while (logLevelTextTmp);
    }

	if (CsrArgSearch(NULL, "--ss-log", &parameter, &value))
    {
     
   //     CsrLogLevelText logLevelText;
		CsrCharString * txtlog=CSR_LOG_CLEARTEXT_TEMPLATE_TASK_NAME " " CSR_LOG_CLEARTEXT_TEMPLATE_SUBORIGIN_NAME " " CSR_LOG_CLEARTEXT_TEMPLATE_LOG_LEVEL_NAME ": " CSR_LOG_CLEARTEXT_TEMPLATE_STRING CSR_LOG_CLEARTEXT_TEMPLATE_BUFFER;
		CsrBool schedEvents=FALSE;
	
		CsrUint32 lCsrLogLevelText;

		void *ltClearTextStdout;
        CsrLog *logClearTextStdOut;
		if(sscanf(value, "0x%8x",&SSLogLevelText) != 1)
		{
		    printf("No value specified for parameter --log-level\n");
            return CSR_RESULT_FAILURE;
        }

		switch (SSLogLevelText)
        {
          case CSR_SS_LOG_LEVEL_TEXT_CRITICAL:
		  	 lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_CRITICAL;
			  break;
		  case CSR_SS_LOG_LEVEL_TEXT_ERROR:
		  	  lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
			  break;
		  case CSR_SS_LOG_LEVEL_TEXT_WARNING:
              lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_WARNING|CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
			  break;
		   case CSR_SS_LOG_LEVEL_TEXT_INFO:
              lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_INFO|CSR_LOG_LEVEL_TEXT_WARNING|CSR_LOG_LEVEL_TEXT_ERROR|CSR_LOG_LEVEL_TEXT_CRITICAL;
			  break;
		   case CSR_SS_LOG_LEVEL_TEXT_DEBUG:
              lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_ALL;
			  break;
		   case CSR_SS_LOG_LEVEL_TEXT_ALL:
		   	  lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_ALL;
			  schedEvents=TRUE;
		   	  break;
		   default:
		   	 lCsrLogLevelText =CSR_LOG_LEVEL_TEXT_OFF;
		   	 break;
        } /* End switch */
		

        ltClearTextStdout = CsrLogTransportStdoutOpen();
        logClearTextStdOut = CsrLogCleartextCreate(ltClearTextStdout, txtlog, schedEvents);
				

        if (ltClearTextStdout && logClearTextStdOut)
        {
            CsrLogFormatInstRegister(logClearTextStdOut);
        }
        else
        {
            printf(CSR_SS_AP_LOG_STR "Unable to open stdout log transport using format: %s\n", value ? value : CSR_LOG_CLEARTEXT_FORMAT);
            return CSR_RESULT_FAILURE;
        }
           
        CsrLogLevelEnvironmentSet(0x00);
        CsrLogLevelTaskSetAll(0x00);
		



		CsrLogLevelTextSetAll(lCsrLogLevelText);
    }
		
#endif
    /* Init Scheduler */
    for (i = 0; i < CSR_SCHEDULER_INSTANCES; i++)
    {
        schedInstance = CsrSchedInit(i, CSR_THREAD_PRIORITY_NORMAL, 0);
    }

	peer_server_com_init(DEFAULT_EXTERNAL_SRV_PORT);
    {
        /* Start scheduler */
        CsrSched(schedInstance);
    }


	peer_server_com_deinit();
    /* Deinit Scheduler */
    CsrSchedDeinit(schedInstance);

    /* Deinitialise Log */
#ifdef CSR_LOG_ENABLE
    CsrLogDeinit();
#ifdef ENABLE_SHUTDOWN
    CsrMsgConvDeinit();
#endif
    CsrPmemFree(logLevelTextString);
#endif

    /* Deinitialise Transport */
    CsrAppMainTransportDeinitialise();

    /* Deinit Pmem */
#ifdef CSR_MEMALLOC_PROFILING
    CsrMemStatDeinit(leakDump);
#endif
    CsrPmemDeinit();

    return CSR_RESULT_SUCCESS;
}
