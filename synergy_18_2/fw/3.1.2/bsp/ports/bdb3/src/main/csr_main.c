/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "nucleus.h"
#include "csr_types.h"
#include "csr_macro.h"
#include "csr_main.h"
#include "csr_arg_search.h"
#include "csr_util.h"
#include "platform/csr_sdram.h"
#include "platform/csr_mmu.h"
#include "platform/csr_led.h"
#include "platform/csr_confdata.h"
#include "platform/csr_console.h"

/* Disable semihosting. This enables the binary to run from NAND Flash without
   a debugger attached, and ensures that no code uses C library features that
   requires (additional) retargeting of the C library. */
#pragma import(__use_no_semihosting_swi)

/* System Memory definitions */
#define SYSTEM_MEMORY_HEAP_MINIMUM_SIZE         (4 * 1024 * 1024)
#define SYSTEM_MEMORY_HEAP_MINIMUM_ALLOCATION   32
NU_MEMORY_POOL System_Memory;

/* Main thread definitions */
#define MAIN_THREAD_STACK_SIZE                  (4 * 1024)
#define MAIN_THREAD_PRIORITY                    127
#define MAIN_THREAD_TIMESLICES                  0
static NU_TASK mainThread;

/* Base of C library stack/heap. Pass this address to NU_Initialize to reclaim
   the stack/heap during Nucleus initialisation. */
extern unsigned int Image$$ARM_LIB_STACKHEAP$$Base;

/* Limit of C library stack/heap. Pass this address to NU_Initialize to preserve
   the stack/heap after Nucleus starts. */
extern unsigned int Image$$ARM_LIB_STACKHEAP$$ZI$$Limit;

/* System main, called by the C Runtime 0 (CRT0). */
int main(int argc, char *argv[])
{
    /* Clear interrupt mask */
    INT_GLOBAL_MASK_CLEAR();

    /* Enable MMU */
    MMUShutdown();
    MMUSetMTT(MMU_DEFAULT_MTT_BASE, 0x00000000, 0xFFFFFFFF, 0x00000000, MMU_RW_NCNB);
    MMUSetMTT(MMU_DEFAULT_MTT_BASE, SDRAM_BASE, SDRAM_TOP - 1024 * 1024 * 3 - 1, SDRAM_BASE, MMU_RW_CNB);
    MMUInitialise(MMU_DEFAULT_MTT_BASE);

    /* Turn off both LED's */
    LED1_OFF();
    LED2_OFF();

    /* Pass arguments to CsrArgSearch */
    CsrArgSearchInit((CsrUint32) argc, (CsrCharString **) argv);

    /* Start Nucleus Initialisation (passing limit makes Nucleus preserve the stack/heap of the C-library) */
    NU_Initialize(&Image$$ARM_LIB_STACKHEAP$$ZI$$Limit);

    return 0;
}

static void dumpRegisters(NU_ABORT_DUMP *dump)
{
    CsrUint32 i;

    printf("\n\n    %s\n\n\n", dump->type);
    for (i = 0; i <= 9; i++)
    {
        printf("    R%u:   0x%08X\n", i, (CsrUint32) dump->regs[i]);
    }
    for (i = 10; i <= 12; i++)
    {
        printf("    R%u:  0x%08X\n", i, (CsrUint32) dump->regs[i]);
    }
    printf("    SP:   0x%08X\n", (CsrUint32) dump->regs[13]);
    printf("    LR:   0x%08X\n", (CsrUint32) dump->regs[14]);
    printf("    PC:   0x%08X\n\n", (CsrUint32) dump->regs[15]);
    printf("    CPSR: 0x%08X\n\n", (CsrUint32) dump->cpsr);
    printf("    DFSR: 0x%08X\n", MMUReadDFSR());
    printf("    IFSR: 0x%08X\n", MMUReadIFSR());
    printf("    FAR:  0x%08X\n", MMUReadFAR());
    printf("    IFAR: 0x%08X\n", MMUReadIFAR());
}

/* Abort handler called by Nucleus if a fatal abort is encountered */
static void mainAbortHandler(NU_ABORT_DUMP *dump)
{
    /* Reinitialise the console for display only */
    ConsoleRouteSet(CONSOLE_ROUTE_NONE);
    ConsoleRouteSet(CONSOLE_ROUTE_DISPLAY);

    /* Dump registers on display */
    dumpRegisters(dump);

    /* Mask and clear all interrupts, then enable them */
    INT_GLOBAL_MASK_CLEAR_ENABLE();

    /* Reinitialise the console for UART only */
    ConsoleRouteSet(CONSOLE_ROUTE_UART);

    /* Dump the registers on UART */
    dumpRegisters(dump);

    /* Send End-of-Transmission (EOT, ASCII: 4) */
    putchar(4);
}

/* Main Thread */
static void mainThreadEntry(UNSIGNED argc, VOID *argv)
{
    int retval;

    CSR_UNUSED(argc);
    CSR_UNUSED(argv);

    retval = CsrMain();

    exit(retval);
}

/* This function is called by Nucleus as part of the initialisation. */
void Application_Initialize(void *first_available_memory)
{
    UNSIGNED heapSize;
    void *pointer;

    NU_Abort_Handler_Register(mainAbortHandler);

    heapSize = (SDRAM_TOP - 1024 * 1024 * 3) - (UNSIGNED) first_available_memory;

    if (((UNSIGNED) first_available_memory > (SDRAM_TOP - 1024 * 1024 * 3)) ||
        (heapSize < SYSTEM_MEMORY_HEAP_MINIMUM_SIZE))
    {
        NU_ABORT_DUMP dump;
        dump.type = "HEAP OVERFLOW";
        dump.cpsr = 0;
        CsrMemSet(dump.regs, 0, sizeof(dump.regs));
        mainAbortHandler(&dump);
        while (1)
        {
            /* There is not enough memory to satisfy SYSTEM_MEMORY_HEAP_MINIMUM_SIZE */
        }
    }

    /* Create the global System_Memory pool that will be used for all heap allocation. */
    NU_Create_Memory_Pool(&System_Memory, "SYSMEM", first_available_memory, heapSize, SYSTEM_MEMORY_HEAP_MINIMUM_ALLOCATION, NU_FIFO);

    /* Create the Main Thread. The thread will start execution on return of Application_Initialize. */
    NU_Allocate_Memory(&System_Memory, (void **) &pointer, MAIN_THREAD_STACK_SIZE, NU_NO_SUSPEND);
    NU_Create_Task(&mainThread, NULL, mainThreadEntry, 0, NULL, pointer, MAIN_THREAD_STACK_SIZE, MAIN_THREAD_PRIORITY, MAIN_THREAD_TIMESLICES, NU_PREEMPT, NU_START);
}

/********************* Retargeting *********************/

#include <rt_sys.h>
#include <rt_misc.h>

/* Last resort character output. Only used by C library when it encounters a
   fatal error that will render normal (stdout/stderr) output facilities
   unusable. */
void _ttywrch(int ch)
{
}

/* Called by the C library when the main function returns or exit() is called. */
void _sys_exit(int returnCode)
{
    while (1)
    {
    }
}

/* This function is called by the C library before main is called to retrieve
   the application argument string (command line), which is stored in the NAND
   Flash. */
#define CMD_STRING_LEN_MAX 4096
#define ARGV_MAX 128
static char cmd[CMD_STRING_LEN_MAX];
static char *argv[ARGV_MAX];
__value_in_regs struct __argc_argv __ARM_get_argv(void *internal_use_only)
{
    struct __argc_argv args = {0};
    int state = 0, argc = 0;
    char *c;

    CSR_UNUSED(internal_use_only);

    /* Look up Application Argument String */
    ConfDataLightweightGetValue(CONFDATA_TAG_APPLICATIONARGUMENT, CMD_STRING_LEN_MAX - 1, (CsrUint8 *) cmd);

    for (c = cmd; *c != '\0'; ++c)
    {
        switch (state)
        {
            case 0: /* BASE */
                if (*c == '"')
                {
                    state = 2;
                }
                else if ((*c != ' ') && (*c != '\n'))
                {
                    argv[argc] = c;
                    argc += 1;
                    state = 1;
                }
                break;
            case 1: /* UNQUOTED ARGUMENT */
                if ((*c == ' ') || (*c == '\n'))
                {
                    *c = '\0';
                    state = 0;
                }
                else if (*c == '"')
                {
                    *c = '\0';
                    state = 2;
                }
                break;
            case 2: /* QUOTE BEGIN */
                argv[argc] = c;
                argc += 1;
                if (*c == '"')
                {
                    *c = '\0';
                    state = 0;
                }
                else
                {
                    state = 3;
                }
                break;
            case 3: /* QUOTED ARGUMENT */
                if (*c == '"')
                {
                    *c = '\0';
                    state = 0;
                }
                break;
            default:
                break;
        }
    }

    args.argc = argc;
    args.argv = argv;
    return args;
}
