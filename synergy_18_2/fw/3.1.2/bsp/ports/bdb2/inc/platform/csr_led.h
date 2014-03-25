#ifndef CSR_LED_H__
#define CSR_LED_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "platform/csr_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LED1_ON()     GPIO_SET(21)
#define LED1_OFF()    GPIO_CLEAR(21)
#define LED1_TOGGLE() GPIO_TOGGLE(21)

#define LED2_ON()     GPIO_SET(22)
#define LED2_OFF()    GPIO_CLEAR(22)
#define LED2_TOGGLE() GPIO_TOGGLE(22)

#ifdef __cplusplus
}
#endif

#endif
