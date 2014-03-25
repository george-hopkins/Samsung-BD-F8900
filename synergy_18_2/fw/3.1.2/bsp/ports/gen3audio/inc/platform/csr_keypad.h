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

#define JOYPAD_N()            (GPIO_IN(13))      /* North */
#define JOYPAD_S()            (GPIO_IN(14))      /* South */
#define JOYPAD_P()            (GPIO_IN(5))       /* Push */
#define JOYPAD_E()            (GPIO_IN(11))      /* East */
#define JOYPAD_W()            (GPIO_IN(12))      /* West */

#define BUTTON_1()            (GPIO_IN(3))      /* General Button 1 */
#define BUTTON_2()            (GPIO_IN(4))      /* General Button 2 */

#define BUTTON_PW()           (GPIO_IN(6))      /* Power Button */

#ifdef __cplusplus
}
#endif

#endif
