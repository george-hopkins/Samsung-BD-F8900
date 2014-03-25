#ifndef CSR_GPIO_H__
#define CSR_GPIO_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c24a0.h"
#include "platform/csr_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Valid values for pin parameter of GPIOPinConfigure */
#define GPIO_PIN_0              0
#define GPIO_PIN_1              1
#define GPIO_PIN_2              2
#define GPIO_PIN_3              3
#define GPIO_PIN_4              4
#define GPIO_PIN_5              5
#define GPIO_PIN_6              6
#define GPIO_PIN_7              7
#define GPIO_PIN_8              8
#define GPIO_PIN_9              9
#define GPIO_PIN_10             10
#define GPIO_PIN_11             11
#define GPIO_PIN_12             12
#define GPIO_PIN_13             13
#define GPIO_PIN_14             14
#define GPIO_PIN_15             15
#define GPIO_PIN_16             16
#define GPIO_PIN_17             17
#define GPIO_PIN_18             18
#define GPIO_PIN_19             19
#define GPIO_PIN_20             20
#define GPIO_PIN_21             21
#define GPIO_PIN_22             22
#define GPIO_PIN_23             23
#define GPIO_PIN_24             24
#define GPIO_PIN_25             25
#define GPIO_PIN_26             26
#define GPIO_PIN_27             27
#define GPIO_PIN_28             28
#define GPIO_PIN_29             29
#define GPIO_PIN_30             30
#define GPIO_PIN_31             31

/* Valid values for function parameter if GPIOPinConfigure. All pins can be
   input or output, in addition, the following special functions are available
   for each pin.

    PIN      SPECIAL_1      SPECIAL_2
    -----------------------------------
    GPIO31:  XuRXD1         IrDA_RXD
    GPIO30:  XuTXD1         IrDA_TXD
    GPIO29:  XuRTSn1        IrDA_SDBW
    GPIO28:  XuCTSn1        RTC_ALMINT
    GPIO27:  EXTDMA_ACK1    XkpCOL4
    GPIO26:  EXTDMA_ACK0    XkpCOL3
    GPIO25:  EXTDMA_REQ1    XkpCOL2
    GPIO24:  EXTDMA_REQ0    XkpCOL1
    GPIO23:  PWM_TOUT3      XkpCOL0
    GPIO22:  PWM_TOUT2      XkpROW4
    GPIO21:  PWM_TOUT1      XkpROW3
    GPIO20:  PWM_TOUT0      XkpROW2
    GPIO19:  PWM_ECLK       XkpROW1
    GPIO18:  EINT18         XkpROW0
    GPIO17:  EINT17         XspiCLK
    GPIO16:  EINT16         XspiMISO
    GPIO15:  EINT15         XspiMOSI
    GPIO14:  EINT14         RTC_ALMINT
    GPIO13:  EINT13         Reserved
    GPIO12:  EINT12         Reserved
    GPIO11:  EINT11         Reserved
    GPIO10:  Reserved       Reserved
    GPIO9:   EINT9          Reserved
    GPIO8:   EINT8          Reserved
    GPIO7:   EINT7          Reserved
    GPIO6:   EINT6          Reserved
    GPIO5:   EINT5          PWM_TOUT3
    GPIO4:   EINT4          PWM_TOUT2
    GPIO3:   EINT3          PWM_TOUT1
    GPIO2:   EINT2          PWM_TOUT0
    GPIO1:   EINT1          PWM_ECLK
    GPIO0:   EINT0          Reserved */
#define GPIO_FUNCTION_INPUT         0
#define GPIO_FUNCTION_OUTPUT        1
#define GPIO_FUNCTION_SPECIAL_1     2
#define GPIO_FUNCTION_SPECIAL_2     3

/* Valid values for pull-up parameter of GPIOPinConfigure */
#define GPIO_PULLUP_ON              0
#define GPIO_PULLUP_OFF             1

/* Valid values for trigger parameter of GPIOPinConfigure */
#define GPIO_TRIGGER_LOWLEVEL       0
#define GPIO_TRIGGER_HIGHLEVEL      1
#define GPIO_TRIGGER_FALLINGEDGE    2
#define GPIO_TRIGGER_RISINGEDGE     4
#define GPIO_TRIGGER_BOTHEDGE       6

/* Configure a single GPIO Pin */
void GPIOPinConfigure(CsrUint8 pin, CsrUint8 function, CsrUint8 pullup, CsrUint8 trigger);

/* Set/Clear/Toggle/Read GPIO Pin */
#define GPIO_SET(pin)           BIT_SET(rGPDAT, pin)
#define GPIO_SET_ATOMIC(pin)    INT_ATOMIC_EXECUTE(BIT_SET(rGPDAT, pin))
#define GPIO_CLEAR(pin)         BIT_CLEAR(rGPDAT, pin)
#define GPIO_CLEAR_ATOMIC(pin)  INT_ATOMIC_EXECUTE(BIT_CLEAR(rGPDAT, pin))
#define GPIO_TOGGLE(pin)        BIT_TOGGLE(rGPDAT, pin)
#define GPIO_TOGGLE_ATOMIC(pin) INT_ATOMIC_EXECUTE(BIT_TOGGLE(rGPDAT, pin))
#define GPIO_IN(pin)            BIT_CHECK(rGPDAT, pin)

#ifdef __cplusplus
}
#endif

#endif
