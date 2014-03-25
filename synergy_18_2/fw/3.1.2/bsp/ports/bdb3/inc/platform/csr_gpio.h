#ifndef CSR_GPIO_H__
#define CSR_GPIO_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_s3c6410.h"
#include "platform/csr_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Valid values for port parameter */
#define GPIO_PORT_A 0x0000
#define GPIO_PORT_B 0x0020
#define GPIO_PORT_C 0x0040
#define GPIO_PORT_D 0x0060
#define GPIO_PORT_E 0x0080
#define GPIO_PORT_F 0x00A0
#define GPIO_PORT_G 0x00C0
#define GPIO_PORT_H 0x00E0
#define GPIO_PORT_I 0x0100
#define GPIO_PORT_J 0x0120
#define GPIO_PORT_K 0x0800
#define GPIO_PORT_L 0x0810
#define GPIO_PORT_M 0x0820
#define GPIO_PORT_N 0x0830
#define GPIO_PORT_O 0x0140
#define GPIO_PORT_P 0x0160
#define GPIO_PORT_Q 0x0180

/* Valid values for function parameter of GPIO_*_FUNCTION_SET. Note that most
   GPIO's have additional functions for values greater than 1. Refer to S3C6410
   user manual. */
#define GPIO_FUNCTION_INPUT         0
#define GPIO_FUNCTION_OUTPUT        1

/* Standard values for pull parameter of GPIO_PULL_SET macro */
#define GPIO_PULL_OFF   0
#define GPIO_PULL_DOWN  1
#define GPIO_PULL_UP    2

/* Valid values for GPIO_EINT_X_X_TRIGGER_SET macros */
#define GPIO_TRIGGER_LOWLEVEL       0
#define GPIO_TRIGGER_HIGHLEVEL      1
#define GPIO_TRIGGER_FALLINGEDGE    2
#define GPIO_TRIGGER_RISINGEDGE     4
#define GPIO_TRIGGER_BOTHEDGE       6

/* Configure GPIO function */
#define GPIO_A_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_A), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_B_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_B), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_C_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_C), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_D_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_D), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_E_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_E), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_F_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_F), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_G_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_G), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_H_FUNCTION_SET(pin, function)  {if ((pin) > 7) {REG_SET(rGPCON0(GPIO_PORT_H), 4 * (pin) + 3, 4 * (pin), function)} \
                                             else {REG_SET(rGPCON1(GPIO_PORT_H), 4 * ((pin) - 8) + 3, 4 * ((pin) - 8), function)}}
#define GPIO_I_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_I), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_J_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_J), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_K_FUNCTION_SET(pin, function)  {if ((pin) > 7) {REG_SET(rGPCON0(GPIO_PORT_K), 4 * (pin) + 3, 4 * (pin), function)} \
                                             else {REG_SET(rGPCON1(GPIO_PORT_K), 4 * ((pin) - 8) + 3, 4 * ((pin) - 8), function)}}
#define GPIO_L_FUNCTION_SET(pin, function)  {if ((pin) > 7) {REG_SET(rGPCON0(GPIO_PORT_L), 4 * (pin) + 3, 4 * (pin), function)} \
                                             else {REG_SET(rGPCON1(GPIO_PORT_L), 4 * ((pin) - 8) + 3, 4 * ((pin) - 8), function)}}
#define GPIO_M_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_M), 4 * (pin) + 3, 4 * (pin), function)
#define GPIO_N_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_N), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_O_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_O), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_P_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_P), 2 * (pin) + 1, 2 * (pin), function)
#define GPIO_Q_FUNCTION_SET(pin, function)  REG_SET(rGPCON(GPIO_PORT_Q), 2 * (pin) + 1, 2 * (pin), function)

/* Configure GPIO pull up/down */
#define GPIO_PULL_SET(port, pin, pull)  REG_SET(rGPPUD(port), 2 * (pin) + 1, 2 * (pin), pull)

/* Configure external interrupt (EINT) trigger condition */
#define GPIO_EINT_0_1_TRIGGER_SET(trigger)      REG_SET(rEINT0CON0, 2, 0, trigger)
#define GPIO_EINT_2_3_TRIGGER_SET(trigger)      REG_SET(rEINT0CON0, 6, 4, trigger)
#define GPIO_EINT_4_5_TRIGGER_SET(trigger)      REG_SET(rEINT0CON0, 10, 8, trigger)
#define GPIO_EINT_6_7_TRIGGER_SET(trigger)      REG_SET(rEINT0CON0, 14, 12, trigger)
#define GPIO_EINT_8_9_TRIGGER_SET(trigger)      REG_SET(rEINT0CON0, 18, 16, trigger)
#define GPIO_EINT_10_11_TRIGGER_SET(trigger)    REG_SET(rEINT0CON0, 22, 20, trigger)
#define GPIO_EINT_12_13_TRIGGER_SET(trigger)    REG_SET(rEINT0CON0, 26, 24, trigger)
#define GPIO_EINT_14_15_TRIGGER_SET(trigger)    REG_SET(rEINT0CON0, 30, 28, trigger)
#define GPIO_EINT_16_17_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 2, 0, trigger)
#define GPIO_EINT_18_19_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 6, 4, trigger)
#define GPIO_EINT_20_21_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 10, 8, trigger)
#define GPIO_EINT_22_23_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 14, 12, trigger)
#define GPIO_EINT_24_25_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 18, 16, trigger)
#define GPIO_EINT_26_27_TRIGGER_SET(trigger)    REG_SET(rEINT0CON1, 22, 20, trigger)

/* Set/Clear/Toggle/Read GPIO Pin */
#define GPIO_SET(port, pin)             BIT_SET(rGPDAT(port), pin)
#define GPIO_SET_ATOMIC(port, pin)      INT_ATOMIC_EXECUTE(GPIO_SET(port, pin))
#define GPIO_CLEAR(port, pin)           BIT_CLEAR(rGPDAT(port), pin)
#define GPIO_CLEAR_ATOMIC(port, pin)    INT_ATOMIC_EXECUTE(GPIO_CLEAR(port, pin))
#define GPIO_TOGGLE(port, pin)          BIT_TOGGLE(rGPDAT(port), pin)
#define GPIO_TOGGLE_ATOMIC(port, pin)   INT_ATOMIC_EXECUTE(GPIO_TOGGLE(port, pin))
#define GPIO_IN(port, pin)              BIT_CHECK(rGPDAT(port), pin)

#ifdef __cplusplus
}
#endif

#endif
