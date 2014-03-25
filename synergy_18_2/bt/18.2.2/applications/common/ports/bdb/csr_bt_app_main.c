/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdlib.h>
#include <stdio.h>

#include <nucleus.h>

#include "platform/csr_i2c.h"
#include "platform/csr_mmu.h"
#include "platform/csr_lcd.h"
#include "platform/csr_led.h"
#include "platform/csr_delay.h"
#include "platform/csr_uart.h"
#include "platform/csr_rtc.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_timer.h"
#include "platform/csr_confdata.h"
#include "platform/csr_pmem_init.h"
#include "platform/csr_panic_init.h"
#include "csr_sched_init.h"
#include "platform/csr_console.h"

#include "csr_transport.h"
#include "csr_hci_lib.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_bt_app.h"
#include "csr_bt_declare_tasks.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_pcap.h"
#include "csr_log_btsnoop.h"
#endif

#include "csr_bt_demoapp.h"

/* #define STRING_LOG_UART__ */

#pragma import(__use_no_semihosting_swi)

static char compileTime[] = __DATE__ " " __TIME__;

/* Size of memory pool (in MiB) available to pmalloc */
#define MEMORY_POOL_SIZE            4
NU_MEMORY_POOL System_Memory;

/* Common demo setup vars */
char baudRate[10] = { 0 };
char casira_ComPort[128] = { 0 };
CsrUint16 transportType = 0;
CsrBool bdAddrSet = FALSE;
BD_ADDR_T defGlobalBdAddr = {0,0,0};
CsrUint8  defGlobalPinLen = 0;
CsrUint8  defGlobalPin[CSR_BT_PASSKEY_MAX_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BD_ADDR_T romBdAddress = {0,0,0};
CsrUint16 romCrystalFreq = 0;
CsrUint32 romBaud = DEFAULT_RESET_BAUDRATE;

#ifdef CSR_LOG_ENABLE
CsrUint32 logTaskLevel             = DEFAULT_TASK_LOG_LEVEL;
CsrUint32 logEnvLevel              = DEFAULT_ENV_LOG_LEVEL;
#endif

/* Size of the ramdisk in NUFM */
#if 0
#define RAMDISK_SIZE                (8 * 1024 * 1024)
#define RAMDISK_START_ADDRESS       ramdisk
static UINT32 ramdisk[RAMDISK_SIZE >> 2];
#endif

/* Function for changing priority on a scheduler thread */
extern CsrBool nucleus_sched_change_priority(CsrUint16 id, OPTION newPriority);

/* Main thread */
static NU_TASK MainThread;

/* Base of C-library stack/heap (pass this address to overwrite) */
extern unsigned int Image$$ARM_LIB_STACKHEAP$$Base;

void CsrSchedStop(void)
{
}

int main(int argc, char *argv[])
{
    CsrUint8 byte;
    TimeStructure now = { 0, 0, 0, 1, 0, 0, 0};
    char *extraOptions;

    /* Clear interrupt mask */
    INT_GLOBAL_MASK_CLEAR();

    /* Enable the MMU (Instruction + Data cache on first 61MiB of SDRAM) */
    MMUShutdown();
    MMUSetMTT(MMU_DEFAULT_MTT_BASE, 0x00000000, 0xFFFFFFFF, 0x00000000, RW_NCNB); /* Initialise Main Translation Table */
    MMUSetMTT(MMU_DEFAULT_MTT_BASE, 0x00000000, 0x00000FFF, 0x00000000, RW_NCNB); /* Steppingstone */
    MMUSetMTT(MMU_DEFAULT_MTT_BASE, 0x10000000, 0x13CFFFFF, 0x10000000, RW_CNB);  /* First 61 MB SDRAM (write-through cache)*/
    MMUInitialise(MMU_DEFAULT_MTT_BASE, 0xFFFFFFFF);

    TimerExtensionActivate();
    RtcTimeSet(&now);

    /* Initialise Display */
    LCDInitialise(LCD_FB0, LCD_FB1);

    /* Setup uart if required */
#ifdef STRING_LOG_UART__
    ConsoleRouteSet(CONSOLE_ROUTE_DISPLAY);
#else
    ConsoleRouteSet(CONSOLE_ROUTE_UART);
#endif

    ConsoleClear();

    /* Connect UART0 to the DEBUG UART (115200, 8N1, Hardware Flow Control) */
    byte = (1 << 7) | (1 << 5);
    I2CWrite(0x22, 1, &byte);

    /* Print CSR header */
    printf("\n\n\nCambridge Silicon Radio\n");
    printf("Novi Science park\n");
    printf("Niels Jernes Vej 10\n");
    printf("DK9220 Aalborg East\n");
    printf("www.csr.com\n\n");

    printf("Compiled: %s\n\n", compileTime);

    /* Start Blinking of Green LED */
    LED1_ON();
    LED2_ON();

#if 0
    /* Reset and power-cycle Radio Module */
    I2CRead(0x10, 1, &byte);
    byte |= 0x18; /* Power on and reset */
    I2CWrite(0x10, 1, &byte);
    DelayUs(50000);
    byte &= ~0x10; /* Negate Reset */
    I2CWrite(0x10, 1, &byte);
    DelayUs(50000);

    /* Reset additional device on Radio Module using LED[0] line (A-connector, pin 35) */
    GPIOPinConfigure(GPIO_PIN_18, GPIO_FUNCTION_OUTPUT, GPIO_PULLUP_OFF, GPIO_TRIGGER_LOWLEVEL);
    GPIO_CLEAR(GPIO_PIN_18); /* Assert Reset */
    byte = 18; /* Map GPIO18 to LED[0] (A-connector, pin 35) */
    I2CWrite(0x2B, 1, &byte);
    DelayUs(50000);
    GPIO_SET(GPIO_PIN_18); /* Negate Reset */
    DelayUs(50000);
#endif

    /* Parse commandline and setup operating environment */
    extraOptions = applicationExtraOptions();
    CsrBtAppCmdlineParse(argc, argv, extraOptions);

    CsrBtAppTransportSetDefault(transportType);
    uartHdl = CsrBtAppTransportInit();
    CsrBtAppTransportSetTask(transportType);
    CsrBtBootstrapSetInterface(NULL, transportType);

    switch(transportType)
    {
        case TRANSPORT_TYPE_H5_THREE_WIRE: /* fall through */
        case TRANSPORT_TYPE_H4_UART:    /* fall through */
        case TRANSPORT_TYPE_H4_DS:      /* fall through */
        case TRANSPORT_TYPE_BCSP:
        {
            /* Route UART1 to BlueCore */
            byte = 0x8;
            I2CWrite(0x21, 1, &byte);
            printf("Routing UART1 to SERIAL1\n");
            break;
        }
        case TRANSPORT_TYPE_TYPE_A:
        {
            /* Route CSPI to BlueCore */
            byte = 0x04;
            I2CWrite(0x25, 1, &byte);
            byte = 16;
            I2CWrite(0x26, 1, &byte);
            byte = 17;
            I2CWrite(0x28, 1, &byte);

            /* CSPI init fails if passing this point too
               fast, exact delay required is unknown, it might
               be better to try to improve the CSPI driver itself,
               by allowing it to retry for longer. */
            DelayUs(1000000);

            printf("Routing CSPI to BlueCore\n");
            break;
        }
        case TRANSPORT_TYPE_USB:
        default:
        {
            break;
        }
    }

    /* Start Nucleus Initialisation (passing base makes Nucleus reclaim all stacks/heaps of the C-library) */
    NU_Initialize(&Image$$ARM_LIB_STACKHEAP$$Base);

    return 0;
}

static void CsrPanicCb(CsrUint8 tech, CsrUint16 reason, const char *logtext)
{
    printf("PANIC: %s (tech %hhu, reason %hu)\n", logtext, tech, reason);

    while(1){};
}

/* from "platform/csr_framework_ext_init.h" */
extern void CsrThreadInit(CsrUint16 size);

static void mainthread(UNSIGNED argc, VOID *argv)
{
    void *schedInstance;
    void *uartHdl = NULL;

    CsrPanicInit(CsrPanicCb);
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


    /* Initialise CSR Scheduler */
    schedInstance = CsrSchedInit(ID_STACK,0,0);
    schedInstance = CsrSchedInit(ID_APP,0,0);

    /* Start the scheduler threads */
    CsrSched(schedInstance);

    CsrBtAppTransportDeinit(uartHdl);
}

VOID Application_Initialize(VOID *first_available_memory)
{
    VOID *pointer;

    /* Initialise memory systems */
    NU_Create_Memory_Pool(&System_Memory, "SYSMEM", first_available_memory, MEMORY_POOL_SIZE * 1024 * 1024, 32, NU_FIFO);

    /* Create main thread */
    NU_Allocate_Memory(&System_Memory, &pointer, 1024, NU_NO_SUSPEND);
    NU_Create_Task(&MainThread, "BT_MAIN", mainthread, 0, NU_NULL, pointer, 1024, 10, 20, NU_PREEMPT, NU_START);
}

/* Retargeting, to allow a no-semihosted environment */

#include <rt_sys.h>

char *_sys_command_string(char *cmd, int len)
{
    /* Look up Application Argument String */
    if (ConfDataLightweightGetValue(CONFDATA_TAG_APPLICATIONARGUMENT, len, (unsigned char *) cmd) != CONFDATA_OK)
    {
        /* Not found. zero-terminate the existing buffer */
        *cmd = 0;
    }

    return cmd;
}

void _ttywrch(int ch)
{
}

void _sys_exit(int return_code)
{
    while (1)
    {
        ;
    }
}

void __rt_raise()
{
    while (1)
    {
        ;
    }
}



