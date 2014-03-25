#ifndef CSR_WATCHDOG_H__
#define CSR_WATCHDOG_H__
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

/* Set prescaler */
#define WATCHDOG_PRESCALE_SET(value)    REG_SET(rWTCON, 15, 8, value)

/* Set MUX
    0 = 1/16
    1 = 1/32
    2 = 1/64
    3 = 1/128 */
#define WATCHDOG_MUX_SET(value)         REG_SET(rWTCON, 4, 3, value)

/* Watchdog timer enable/disable */
#define WATCHDOG_TIMER_ENABLE()         BIT_SET(rWTCON, 5);
#define WATCHDOG_TIMER_DISABLE()        BIT_CLEAR(rWTCON, 5);

/* Watchdog timer interrupt enable/disable */
#define WATCHDOG_INTERRUPT_ENABLE()     BIT_SET(rWTCON, 2);
#define WATCHDOG_INTERRUPT_DISABLE()    BIT_CLEAR(rWTCON, 2);

/* Watchdog reset signal generation enable/disable */
#define WATCHDOG_RESET_ENABLE()         BIT_SET(rWTCON, 0);
#define WATCHDOG_RESET_DISABLE()        BIT_CLEAR(rWTCON, 0);

/* Load Watchdog Timer */
#define WATCHDOG_DATA_LOAD(value)       {rWTDAT = value;}
#define WATCHDOG_COUNT_LOAD(value)      {rWTCNT = value;}

/* Macro for resetting the processor after one second has passed */
#define WATCHDOG_PROCESSOR_RESET()      {WATCHDOG_MUX_SET(3); \
                                         WATCHDOG_PRESCALE_SET(124); \
                                         WATCHDOG_COUNT_LOAD(6250); \
                                         WATCHDOG_RESET_ENABLE(); \
                                         WATCHDOG_TIMER_ENABLE();}


#ifdef __cplusplus
}
#endif

#endif
