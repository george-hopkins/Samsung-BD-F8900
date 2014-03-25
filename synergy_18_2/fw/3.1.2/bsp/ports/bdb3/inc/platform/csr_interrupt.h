#ifndef CSR_INTERRUPT_H__
#define CSR_INTERRUPT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c6410.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Usage:

    Registering an interrupts service routine: INT_SR_SET(vector, function)
        where:
            vector: The top level interrupt identifier (INT_SDI, INT_DMA, ...)
            function: The function to call. Must have the __irq keyword in
                      definition.

    This will save the function pointer directly in the interrupt vector table.
    If the corresponding interrupt is triggered the registered service routine
    will be called transparently. On Nucleus, this macro is equivalent to
    calling NU_Setup_Vector. Example:

        INT_SR_SET(INT_UART0, interruptRoutine);

    The __irq keyword forces the compiler to generate ARM code (as opposed to
    THUMB code). It also forces the compiler to generate code for saving the
    context, pushing the registers that will be used onto the interrupt mode
    stack.

    On Nucleus it is recommended to use the NU_Register_LISR to register any
    function without the __irq keyword to an interrupt vector. Example:

        NU_Register_LISR(INT_UART0, interruptRoutine, ...);

    The ISR registered with NU_Register_LISR can be ARM or THUMB code as needed.
    It is possible to activate a HISR from an ISR by calling the
    NU_Activate_HISR() service, regardles of whether the ISR was installed with
    the INT_SR_SET macro, the NU_Setup_Vector service or the NU_Register_LISR
    service. The main difference between these methods is that Nucleus will
    perform the context save and restore, when using NU_Register_LISR, while
    this is the responsiblity of the ISR when using the INT_SR_SET macro or the
    NU_Setup_Vector service (although the __irq keyword, will force the compiler
    to automatically insert instructions for saving the context). Note that when
    using the NU_Register_LIST method, Nucleus will perform the context save on
    the interrupted tasks stack, and switch to the system stack before calling
    the registered ISR. This means that it will only use a few words on the
    interrupt mode stack for the initial context save, wheras the compiler
    generated code when using the __irq keyword in conjunction with INT_SR_SET
    or NU_Setup_Vector will push all saved registers onto the interrupt mode
    stack, and all code executed/called in the ISR will use the interrupt mode
    stack. An important consideration is that on the return of an ISR registered
    using NU_Register_LISR, Nucleus will be able to perform a rescheduling. This
    means that if the ISR activated a HISR, the execution will continue in the
    HISR, instead of returning to the thread that was interrupted by the
    interrupt. If returning to the interrupted thread, the execution will
    continue in that thread until the next regular rescheduling takes place,
    which means that the HISR can be delayed by a period less than or equal to
    the Nucleus Timer Tick Interval, which in many applications is unacceptable.

    Unmasking/masking:
        INT_MASK() and INT_UNMASK() for top level interrupts
        INT_EXT_MASK() and INT_EXT_UNMASK() for external interrupts

    Clear pending:
        INT_CLEAR() for top level interrupts
        INT_EXT_CLEAR() for external interrupts

    NOTE: INT_CLEAR() will only clear the specified interrupt if all the
    corresponding external interrupts are cleared first, or if the corresponding
    top level interrupts are masked first.

    NOTE: It is not possible to use INT_MAIN_SR_SET() for the INT_MAIN_IRQ
    main vector table entry. The S3C6410 utilises the VIC port of the ARM1176,
    to direct execution directly to the handler registered with INT_SR_SET or
    NU_Setup_Vector. Handlers registered with NU_Register_LISR will be handled
    automatically by Nucleus. */

/************************************************************/
/********* Low Level Interrupt Controller Interface *********/
/************************************************************/

/* Top level interrupt identifiers */
#define INT_ADC         63
#define INT_PENDNUP     62
#define INT_SEC         61
#define INT_RTC_ALARM   60
#define INT_IRDA        59
#define INT_OTG         58
#define INT_HSMMC1      57
#define INT_HSMMC0      56
#define INT_HOSTIF      55
#define INT_MSM         54
#define INT_EINT4       53
#define INT_HSIRX       52
#define INT_HSITX       51
#define INT_I2C0        50
#define INT_HSMMC2      49
#define INT_SPI0        48
#define INT_UHOST       47
#define INT_CFC         46
#define INT_NFC         45
#define INT_ONENAND1    44
#define INT_ONENAND0    43
#define INT_DMA1        42
#define INT_DMA0        41
#define INT_UART3       40
#define INT_UART2       39
#define INT_UART1       38
#define INT_UART0       37
#define INT_AC97        36
#define INT_PCM1        35
#define INT_PCM0        34
#define INT_EINT3       33
#define INT_EINT2       32
#define INT_LCD2        31
#define INT_LCD1        30
#define INT_LCD0        29
#define INT_TIMER4      28
#define INT_TIMER3      27
#define INT_WDT         26
#define INT_TIMER2      25
#define INT_TIMER1      24
#define INT_TIMER0      23
#define INT_KEYPAD      22
#define INT_ARM_DMAS    21
#define INT_ARM_DMA     20
#define INT_ARM_DMAERR  19
#define INT_SDMA1       18
#define INT_SDMA0       17
#define INT_MFC         16
#define INT_JPEG        15
#define INT_BATF        14
#define INT_SCALER      13
#define INT_TVENC       12
#define INT_2D          11
#define INT_ROTATOR     10
#define INT_POST0       9
#define INT_3D          8
#define INT_RESERVED    7
#define INT_I2S         6
#define INT_I2C1        5
#define INT_CAMIF_P     4
#define INT_CAMIF_C     3
#define INT_RTC_TIC     2
#define INT_EINT1       1
#define INT_EINT0       0

/* External interrupt identifiers
    Applicable for:
    EINT0MASK - External Interrupt Mask
    EINT0PEND - External Interrupt Pending */
#define INT_EXT_EINT0   0
#define INT_EXT_EINT1   1
#define INT_EXT_EINT2   2
#define INT_EXT_EINT3   3
#define INT_EXT_EINT4   4
#define INT_EXT_EINT5   5
#define INT_EXT_EINT6   6
#define INT_EXT_EINT7   7
#define INT_EXT_EINT8   8
#define INT_EXT_EINT9   9
#define INT_EXT_EINT11  11
#define INT_EXT_EINT12  12
#define INT_EXT_EINT13  13
#define INT_EXT_EINT14  14
#define INT_EXT_EINT15  15
#define INT_EXT_EINT16  16
#define INT_EXT_EINT17  17
#define INT_EXT_EINT18  18
#define INT_EXT_EINT19  19
#define INT_EXT_EINT20  20
#define INT_EXT_EINT21  21
#define INT_EXT_EINT22  22
#define INT_EXT_EINT23  23
#define INT_EXT_EINT24  24
#define INT_EXT_EINT25  25
#define INT_EXT_EINT26  26
#define INT_EXT_EINT27  27

/* Main vector table identifiers */
#define INT_MAIN_RESET      /* Not supported */
#define INT_MAIN_UNDEF      0x0C001FE4
#define INT_MAIN_SWI        0x0C001FE8
#define INT_MAIN_PABT       0x0C001FEC
#define INT_MAIN_DABT       0x0C001FF0
#define INT_MAIN_RESERVED   /* Not supported */
#define INT_MAIN_IRQ        0x0C001FF8 /* Note: not used, when VIC port is enabled (default) */
#define INT_MAIN_FIQ        0x0C001FFC

/* Macros to locate source in the two daisy chained VIC controllers */
#define INT_TO_VIC(interrupt)   ((interrupt) >> 5)
#define INT_TO_INDEX(interrupt) ((interrupt) & 0x1F)

/*** Assign service routine to a main vector table entry ***/
#define INT_MAIN_SR_SET(vector, function)   {(*((CsrUint32 *) (vector))) = (CsrUint32) (function);}

/*** Assign service routine to a top level interrupt vector table entry ***/
#define INT_SR_SET(interrupt, function)     {rVICVECTADDR(INT_TO_VIC(interrupt), INT_TO_INDEX(interrupt)) = (CsrUint32) (function);}

/*** Global ENABLE/DISABLE ***/

/* Temporarily disable IRQ and FIQ globally and execute the statement */
#define INT_ATOMIC_EXECUTE_HEAD()           CsrUint32 ____previous____State____; \
    __asm {mrs ____previous____State____, CPSR}; \
    __disable_irq();
#define INT_ATOMIC_EXECUTE_TAIL()           __asm {msr CPSR_cxsf, ____previous____State____}
#define INT_ATOMIC_EXECUTE(statement)       {INT_ATOMIC_EXECUTE_HEAD(); \
                                             statement; \
                                             INT_ATOMIC_EXECUTE_TAIL();}

/* Enable IRQ and FIQ globally */
#define INT_GLOBAL_ENABLE()                 {__enable_irq();}

/* Disable IRQ and FIQ globally */
#define INT_GLOBAL_DISABLE()                {__disable_irq();}

/*** SOURCE PENDING ***/

#define INT_EXT_PENDING_GET()               (rEINT0PEND)
#define INT_PENDING_GET(vic)                (rVICRAWINTR(vic))

#define INT_EXT_PENDING_CHECK(interrupt)    BIT_CHECK(INT_EXT_PENDING_GET(), interrupt)
#define INT_PENDING_CHECK(interrupt)        BIT_CHECK(INT_PENDING_GET(INT_TO_VIC(interrupt)), INT_TO_INDEX(interrupt))

/*** CLEAR PENDING ***/

/* Clear pending for EINT0PEND */
#define INT_EXT_CLEAR(interrupt)            {rEINT0PEND = ((CsrUint32) 1) << (interrupt); \
                                             DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_CLEAR_ALL()                 {rEINT0PEND = rEINT0PEND; \
                                             DATA_SYNCHRONIZATION_BARRIER();}

/* Clear active interrupt (acknowledge) */
#define INT_CLEAR()                         {rVICADDRESS(0) = 0; \
                                             rVICADDRESS(1) = 0;}

/*** MASK/UNMASK ***/

/* Mask/Unmask for EINT0MASK */
#define INT_EXT_MASK(interrupt)                         {BIT_SET(rEINT0MASK, (interrupt)); \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_ATOMIC(interrupt)                  INT_ATOMIC_EXECUTE(INT_EXT_MASK(interrupt))
#define INT_EXT_UNMASK(interrupt)                       BIT_CLEAR(rEINT0MASK, (interrupt))
#define INT_EXT_UNMASK_ATOMIC(interrupt)                INT_ATOMIC_EXECUTE(INT_EXT_UNMASK(interrupt))
#define INT_EXT_UNMASK_RANGE(interruptH, interruptL)    REG_SET(rEINT0MASK, (interruptH), (interruptL), 0)
#define INT_EXT_MASK_RANGE(interruptH, interruptL)      {REG_SET(rEINT0MASK, (interruptH), (interruptL), 0xFFFFFFF); \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_ALL()                              {rEINT0MASK = 0xFFFFFFF; \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_CHECK(interrupt)                   BIT_CHECK(rEINT0MASK, (interrupt))
#define INT_EXT_MASK_GET()                              (rEINT0MASK)

/* Mask/Unmask for top level interrupts
   IMPORTANT: There is an unknown delay between masking an interrupt, using the
              INT_MASK/INT_MASK_ATOMIC/INT_MASK_ALL macros, and the point at
              which the mask is effective. A Data Synchronization Barrier is not
              effective at working around this. */
#define INT_MASK(interrupt)                             {rVICINTENCLEAR(INT_TO_VIC(interrupt)) = ((CsrUint32) 1) << INT_TO_INDEX(interrupt);}
#define INT_MASK_ATOMIC(interrupt)                      INT_MASK(interrupt)
#define INT_UNMASK(interrupt)                           {rVICINTENABLE(INT_TO_VIC(interrupt)) = ((CsrUint32) 1) << INT_TO_INDEX(interrupt);}
#define INT_UNMASK_ATOMIC(interrupt)                    INT_UNMASK(interrupt)
#define INT_MASK_ALL()                                  {rVICINTENCLEAR(0) = 0xFFFFFFFF; \
                                                         rVICINTENCLEAR(1) = 0xFFFFFFFF;}
#define INT_MASK_CHECK(interrupt)                       BIT_CHECK(rVICINTENABLE(INT_TO_VIC(interrupt)), INT_TO_INDEX(interrupt))

/*** SOFTWARE INTERRUPT ***/

#define INT_SOFTWARE_TRIGGER(interrupt)                 {rVICSOFTINT(INT_TO_VIC) = ((CsrUint32) 1) << INT_TO_INDEX(interrupt);}
#define INT_SOFTWARE_CLEAR(interrupt)                   {rVICSOFTINTCLEAR(INT_TO_VIC) = ((CsrUint32) 1) << INT_TO_INDEX(interrupt); \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_SOFTWARE_CLEAR_ALL()                        {rVICSOFTINTCLEAR(0) = 0xFFFFFFFF; \
                                                         rVICSOFTINTCLEAR(1) = 0xFFFFFFFF; \
                                                         DATA_SYNCHRONIZATION_BARRIER();}

/*** Global MASK+CLEAR ***/

#define INT_GLOBAL_MASK_CLEAR()             {CsrInt32 i; \
                                             INT_EXT_MASK_ALL(); \
                                             INT_MASK_ALL(); \
                                             INT_EXT_CLEAR_ALL(); \
                                             INT_SOFTWARE_CLEAR_ALL(); \
                                             for (i = 0; i < 16; i++) {INT_CLEAR();}}

#define INT_GLOBAL_MASK_CLEAR_ENABLE()      {INT_GLOBAL_MASK_CLEAR(); \
                                             INT_GLOBAL_ENABLE();}

/*** Wait for Interrupt ***/

#define WAIT_FOR_INTERRUPT()                {int r0 = 0; \
                                             __asm {mcr p15, 0, r0, c7, c0, 4};}

#ifdef __cplusplus
}
#endif

#endif
