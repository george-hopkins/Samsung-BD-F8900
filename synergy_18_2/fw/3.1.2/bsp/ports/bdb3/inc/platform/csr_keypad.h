#ifndef CSR_KEYPAD_H__
#define CSR_KEYPAD_H__
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

#define JOYPAD_N()            (GPIO_IN(GPIO_PORT_N, 8))      /* North */
#define JOYPAD_S()            (GPIO_IN(GPIO_PORT_N, 5))      /* South */
#define JOYPAD_P()            (GPIO_IN(GPIO_PORT_N, 5))      /* Push */
#define JOYPAD_E()            (GPIO_IN(GPIO_PORT_N, 10))     /* East */
#define JOYPAD_W()            (GPIO_IN(GPIO_PORT_N, 11))     /* West */

#define BUTTON_1()            (GPIO_IN(GPIO_PORT_N, 6))      /* General Button 1 */
#define BUTTON_2()            (GPIO_IN(GPIO_PORT_N, 7))      /* General Button 2 */

#define BUTTON_PW()           (GPIO_IN(GPIO_PORT_N, 4))      /* Power Button */

#ifdef __cplusplus
}
#endif

#endif
