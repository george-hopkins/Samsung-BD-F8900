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

#define LED1_ON()     GPIO_SET(GPIO_PORT_F, 15)
#define LED1_OFF()    GPIO_CLEAR(GPIO_PORT_F, 15)
#define LED1_TOGGLE() GPIO_TOGGLE(GPIO_PORT_F, 15)

#define LED2_ON()     GPIO_SET(GPIO_PORT_G, 6)
#define LED2_OFF()    GPIO_CLEAR(GPIO_PORT_G, 6)
#define LED2_TOGGLE() GPIO_TOGGLE(GPIO_PORT_G, 6)

#ifdef __cplusplus
}
#endif

#endif
