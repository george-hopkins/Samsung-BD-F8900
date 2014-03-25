#ifndef CSR_CONSOLE_H__
#define CSR_CONSOLE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONSOLE_ROUTE_NONE          0
#define CONSOLE_ROUTE_DISPLAY       1
#define CONSOLE_ROUTE_UART          2
#define CONSOLE_ROUTE_DISPLAY_UART  (CONSOLE_ROUTE_DISPLAY | CONSOLE_ROUTE_UART)

void ConsoleRouteSet(CsrUint8 route);
void ConsoleClear(void);

/* Returns TRUE if getchar() will return a character */
CsrBool ConsoleKbhit(void);

/* Data detector. Register a ConsoleNotifyFunction that will be called whenever
   received data are available. The supplied function will be called once when
   data become available, and the callback will then be inhibited until at least
   a single byte is retrieved by a call to a C library functions that retrieve
   characters from standard input (scanf, fgetc, getchar, ...) . */
typedef void (*ConsoleNotifyFunction)(void);
void ConsoleDataDetectorRegister(ConsoleNotifyFunction detect);
void ConsoleDataDetectorUnregister(void);

#ifdef __cplusplus
}
#endif

#endif
