#ifndef CSR_INTERRUPT_H__
#define CSR_INTERRUPT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c24a0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Usage:

    Registering an interrupts service routine: INT_SR_SET(interrupt, function)
        where:
            interrupt: The top level interrupt identifier
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
        INT_SUB_MASK() and INT_SUB_UNMASK() for sub level interrupts
        INT_EXT_MASK() and INT_EXT_UNMASK() for external interrupts

    Clear pending:
        INT_CLEAR() for top level interrupts
        INT_SUB_CLEAR() for sub level interrupts
        INT_EXT_CLEAR() for external interrupts

    NOTE: INT_CLEAR() will only clear the specified interrupt if all the
    corresponding sub level interrupts or external interrupts are cleared first,
    or if the corresponding top level interrupts are masked first.

    NOTE: It is not neccessary to use INT_MAIN_SR_SET() for the INT_MAIN_IRQ
    main vector table entry in order to register ordinary interrupt service
    routines. A standard interrupt arbiter is already registered by the
    bootstrap (replaced when using Nucleus), which will direct execution to the
    handlers registered using INT_SR_SET, NU_Setup_Vector or NU_Register_LISR.
    It is only neccessary to use the INT_MAIN_SR_SET() macro if an alternative
    handler for one or more of the hardware exception vectors are needed. It is
    not recommended to short circuit the interrupt handling of Nucleus by
    registering another INT_MAIN_IRQ handler.

    NOTE: The interrupt framework contains a set of centralised handlers for
    shared interrupts. See helper functionality for shared interrupts below. */

/************************************************************/
/********* Low Level Interrupt Controller Interface *********/
/************************************************************/

/* Top level interrupt identifiers
    Applicable for:
    SRCPND - Source Pending
    INTPND - Interrupt Pending
    INTMOD - Interrupt Mode
    INTMSK - Interrupt Mask */
#define INT_EINT0_2             0
#define INT_EINT3_6             1
#define INT_EINT7_10            2
#define INT_EINT11_14           3
#define INT_EINT15_18           4
#define INT_TIC                 5
#define INT_DCTQ                6
#define INT_MC                  7
#define INT_ME                  8
#define INT_KEYPAD              9
#define INT_TIMER0             10
#define INT_TIMER1             11
#define INT_TIMER2             12
#define INT_TIMER3_4           13
#define INT_LCD_POST           14
#define INT_CAMIF_CODEC        15
#define INT_WDT_BATFLT         16
#define INT_UART0              17
#define INT_CAMIF_PREVIEW      18
#define INT_MODEM              19
#define INT_DMA                20
#define INT_SDI                21
#define INT_SPI0               22
#define INT_UART1              23
#define INT_AC97_NFLASH        24
#define INT_USBD               25
#define INT_USBH               26
#define INT_IIC                27
#define INT_IrDA_MSTICK        28
#define INT_VLX_SPI1           29
#define INT_RTC                30
#define INT_ADC_PENUP_DOWN     31

/* Sub interrupt identifiers
    Applicable for:
    SUBSRCPND - Sub Source Pending
    INTSUBMSK - Interrupt Sub Mask */
#define INT_SUB_RXD0            0
#define INT_SUB_TXD0            1
#define INT_SUB_ERR0            2
#define INT_SUB_RXD1            3
#define INT_SUB_TXD1            4
#define INT_SUB_ERR1            5
#define INT_SUB_IrDA            6
#define INT_SUB_MSTICK          7
#define INT_SUB_TIMER3         11
#define INT_SUB_TIMER4         12
#define INT_SUB_WDT            13
#define INT_SUB_BATFLT         14
#define INT_SUB_POST           15
#define INT_SUB_DISP_FIFO      16
#define INT_SUB_PENUP          17
#define INT_SUB_PENDN          18
#define INT_SUB_ADC            19
#define INT_SUB_DISP_FRAME     20
#define INT_SUB_NFLASH         21
#define INT_SUB_AC97           22
#define INT_SUB_SPI1           23
#define INT_SUB_VLX            24
#define INT_SUB_DMA0           25
#define INT_SUB_DMA1           26
#define INT_SUB_DMA2           27
#define INT_SUB_DMA3           28

/* External interrupt identifiers
    Applicable for:
    EINTMASK - External Interrupt Mask
    EINTPEND - External Interrupt Pending */
#define INT_EXT_EINT0               0
#define INT_EXT_EINT1               1
#define INT_EXT_EINT2               2
#define INT_EXT_EINT3               3
#define INT_EXT_EINT4               4
#define INT_EXT_EINT5               5
#define INT_EXT_EINT6               6
#define INT_EXT_EINT7               7
#define INT_EXT_EINT8               8
#define INT_EXT_EINT9               9
#define INT_EXT_EINT11             11
#define INT_EXT_EINT12             12
#define INT_EXT_EINT13             13
#define INT_EXT_EINT14             14
#define INT_EXT_EINT15             15
#define INT_EXT_EINT16             16
#define INT_EXT_EINT17             17
#define INT_EXT_EINT18             18

/* Main vector table identifiers */
#define INT_MAIN_RESET              0xA0
#define INT_MAIN_UNDEF              0xA4
#define INT_MAIN_SWI                0xA8
#define INT_MAIN_PABT               0xAC
#define INT_MAIN_DABT               0xB0
#define INT_MAIN_RESERVED           0xB4
#define INT_MAIN_IRQ                0xB8
#define INT_MAIN_FIQ                0xBC


/*** Assign service routine to a main vector table entry ***/
#define INT_MAIN_SR_SET(interrupt, function)    {(*((CsrUint32 *) (interrupt))) = (CsrUint32) (function);}

/*** Assign service routine to a top level interrupt vector table entry ***/
#define INT_SR_SET(interrupt, function)         {(*((CsrUint32 *) (0x20 + ((interrupt) * 4)))) = (CsrUint32) (function);}


/*** Global ENABLE/DISABLE ***/

/* Temporarily disable IRQ and FIQ globally and execute the statement */
#define INT_ATOMIC_EXECUTE_HEAD()           int ____previous____State____, ____locked____State____; \
    __asm {mrs ____previous____State____, CPSR}; \
    __asm {orr ____locked____State____, ____previous____State____, 0xC0}; \
    __asm {msr CPSR_cxsf, ____locked____State____}
#define INT_ATOMIC_EXECUTE_TAIL()           __asm {msr CPSR_cxsf, ____previous____State____}
#define INT_ATOMIC_EXECUTE(statement)       {INT_ATOMIC_EXECUTE_HEAD(); \
                                             statement; \
                                             INT_ATOMIC_EXECUTE_TAIL();}

/* Enable IRQ and FIQ globally */
#define INT_GLOBAL_ENABLE()                 {int a; \
                                             __asm {mrs a, CPSR}; \
                                             __asm {bic a, a, 0xC0}; \
                                             __asm {msr CPSR_c, a};}

/* Disable IRQ and FIQ globally */
#define INT_GLOBAL_DISABLE()                {int a; \
                                             __asm {mrs a, CPSR}; \
                                             __asm {orr a, a, 0xC0}; \
                                             __asm {msr CPSR_c, a};}

/*** SOURCE PENDING ***/

#define INT_SUB_PENDING_GET()               (rSUBSRCPND)
#define INT_EXT_PENDING_GET()               (rEINTPEND)
#define INT_PENDING_GET()                   (rSRCPND)

#define INT_SUB_PENDING_CHECK(interrupt)    BIT_CHECK(rSUBSRCPND, interrupt)
#define INT_EXT_PENDING_CHECK(interrupt)    BIT_CHECK(rEINTPEND, interrupt)
#define INT_PENDING_CHECK(interrupt)        BIT_CHECK(rSRCPND, interrupt)


/*** CLEAR PENDING ***/

/* Clear pending for SUBSRCPND */
#define INT_SUB_CLEAR(interrupt)            {rSUBSRCPND = ((CsrUint32) 1 << (interrupt));}
#define INT_SUB_CLEAR_ALL()                 {rSUBSRCPND = rSUBSRCPND;}

/* Clear pending for EINTPEND */
#define INT_EXT_CLEAR(interrupt)            {rEINTPEND = ((CsrUint32) 1) << (interrupt); \
                                             DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_CLEAR_ALL()                 {rEINTPEND = rEINTPEND; \
                                             DATA_SYNCHRONIZATION_BARRIER();}

/* Clear pending for SRCPND og INTPND */
#define INT_CLEAR(interrupt)                {rSRCPND = ((CsrUint32) 1 << (interrupt)); \
                                             rINTPND = rINTPND;}
#define INT_CLEAR_ALL()                     {rSRCPND = rSRCPND; \
                                             rINTPND = rINTPND;}

/*** MASK/UNMASK ***/

/* Mask/Unmask for INTSUBMSK */
#define INT_SUB_MASK(interrupt)                         BIT_SET(rINTSUBMSK, (interrupt))
#define INT_SUB_MASK_ATOMIC(interrupt)                  INT_ATOMIC_EXECUTE(INT_SUB_MASK(interrupt))
#define INT_SUB_UNMASK(interrupt)                       BIT_CLEAR(rINTSUBMSK, (interrupt))
#define INT_SUB_UNMASK_ATOMIC(interrupt)                INT_ATOMIC_EXECUTE(INT_SUB_UNMASK(interrupt))
#define INT_SUB_UNMASK_RANGE(interruptH, interruptL)    REG_SET(rINTSUBMSK, (interruptH), (interruptL), 0)
#define INT_SUB_MASK_RANGE(interruptH, interruptL)      REG_SET(rINTSUBMSK, (interruptH), (interruptL), 0xFFFFFFFF)
#define INT_SUB_MASK_ALL()                              {rINTSUBMSK = 0xFFFFFFFF;}
#define INT_SUB_MASK_CHECK(interrupt)                   BIT_CHECK(rINTSUBMSK, (interrupt))

/* Mask/Unmask for EINTMASK */
#define INT_EXT_MASK(interrupt)                         {BIT_SET(rEINTMASK, (interrupt)); \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_ATOMIC(interrupt)                  INT_ATOMIC_EXECUTE(INT_EXT_MASK(interrupt))
#define INT_EXT_UNMASK(interrupt)                       BIT_CLEAR(rEINTMASK, (interrupt))
#define INT_EXT_UNMASK_ATOMIC(interrupt)                INT_ATOMIC_EXECUTE(INT_EXT_UNMASK(interrupt))
#define INT_EXT_UNMASK_RANGE(interruptH, interruptL)    REG_SET(rEINTMASK, (interruptH), (interruptL), 0)
#define INT_EXT_MASK_RANGE(interruptH, interruptL)      {REG_SET(rEINTMASK, (interruptH), (interruptL), 0x7FFFF); \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_ALL()                              {rEINTMASK = 0x7FFFF; \
                                                         DATA_SYNCHRONIZATION_BARRIER();}
#define INT_EXT_MASK_CHECK(interrupt)                   BIT_CHECK(rEINTMASK, (interrupt))
#define INT_EXT_MASK_GET()                              (rEINTMASK)

/* Mask/Unmask for INTMSK
   IMPORTANT: There is an unknown delay between masking an interrupt, using the
              INT_MASK/INT_MASK_ATOMIC/INT_MASK_ALL macros, and the point at
              which the mask is effective. A Data Synchronization Barrier is not
              effective at working around this. */
#define INT_MASK(interrupt)                             BIT_SET(rINTMSK, (interrupt))
#define INT_MASK_ATOMIC(interrupt)                      INT_ATOMIC_EXECUTE(INT_MASK(interrupt))
#define INT_UNMASK(interrupt)                           BIT_CLEAR(rINTMSK, (interrupt))
#define INT_UNMASK_ATOMIC(interrupt)                    INT_ATOMIC_EXECUTE(INT_UNMASK(interrupt))
#define INT_UNMASK_RANGE(interruptH, interruptL)        REG_SET(rINTMSK, (interruptH), (interruptL), 0)
#define INT_MASK_RANGE(interruptH, interruptL)          REG_SET(rINTMSK, (interruptH), (interruptL), 0xFFFFFFFF)
#define INT_MASK_ALL()                                  {rINTMSK = 0xFFFFFFFF;}
#define INT_MASK_CHECK(interrupt)                       BIT_CHECK(rINTMSK, (interrupt))

/*** Global MASK+CLEAR ***/

#define INT_GLOBAL_MASK_CLEAR()             {INT_SUB_MASK_ALL(); \
                                             INT_EXT_MASK_ALL(); \
                                             INT_MASK_ALL(); \
                                             INT_SUB_CLEAR_ALL(); \
                                             INT_EXT_CLEAR_ALL(); \
                                             INT_CLEAR_ALL();}

#define INT_GLOBAL_MASK_CLEAR_ENABLE()      {INT_GLOBAL_MASK_CLEAR(); \
                                             INT_GLOBAL_ENABLE();}

/*** Wait for Interrupt ***/

#define WAIT_FOR_INTERRUPT()                {int r0 = 0; \
                                             __asm {mcr p15, 0, r0, c7, c0, 4};}


/*************************************/
/********* Shared Interrupts *********/
/*************************************/

/*
    The INT_EINT7_10 interrupt is shared by the SDIO card insertion/removal source
    and the GSM_RING signal source. The Interrupt Framework contains an interrupt
    handler for this interrupt that will automatically determine the external source, and
    call a callback function previously supplied by a call to INTRegisterSDSlotHandler() and/or
    INTRegisterGSMRingHandler(). These function calls will automatically mask/unmask the
    appropriate interrupts, and the shared interrupt handler will perform appropriate
    clearing of pending bits, when an interrupt occurs. */

/* Callback function type */
typedef void (*INTCallbackFunction)(void);

/* Register callbacks for individual sources sharing INT_EINT7_10 */
void INTSharedRegisterHandlerSDSlot(INTCallbackFunction function);    /* Card inserted or removed from SD slot */
void INTSharedRegisterHandlerGSMRing(INTCallbackFunction function);   /* Triggered when the GSM Ring signal is asserted */
void INTSharedUnregisterHandlerSDSlot(void);
void INTSharedUnregisterHandlerGSMRing(void);

/* Direct (fast) masking of individual sources sharing INT_EINT7_10
   Only unmask if a callback function has been registered!
   The register functions will automatically mask/unmask as needed,
   these are only for temporarily masking individual sources fast. */
#define INT_SHARED_UNMASK_SDSLOT()     INT_EXT_UNMASK(INT_EXT_EINT7)
#define INT_SHARED_MASK_SDSLOT()       INT_EXT_MASK(INT_EXT_EINT7)
#define INT_SHARED_UNMASK_GSMRING()    INT_EXT_UNMASK(INT_EXT_EINT8)
#define INT_SHARED_MASK_GSMRING()      INT_EXT_MASK(INT_EXT_EINT8)

#ifdef __cplusplus
}
#endif

#endif
