/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "nucleus.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched_init.h"
#include "csr_ui_keycode.h"
#include "csr_ui_lower.h"

#include "platform/csr_console.h"

#include "csr_panic.h"

static CsrBool initialised = FALSE;

#define KEY_INPUT_HISR_STACK_SIZE 4096
static NU_HISR keyInputHisr;
static CsrUint8 keyInputHisrStack[KEY_INPUT_HISR_STACK_SIZE];

CsrUint16 handleAsciiKeyCodes(char key)
{
    switch (key)
    {
        case 13:    /* Enter (CR) */
            return CSR_UI_KEY_SK1;
        case 8:     /* Backspace (^H, BS) */
        case 127:   /* Backspace (^?, DEL) */
            return CSR_UI_KEY_BACK;
        default:
        {
            if ((key >= 0x20) && (key <= 0x7E))
            {
                return CSR_UI_KEY_ASCII(key);
            }
            break;
        }
    }
    return 0;
}

static void keyInput(void)
{
    static CsrUint8 parseState = 0;
    static CsrUint16 csrUiKey = 0;
    int c;

    while (ConsoleKbhit())
    {
        c = getchar();

        switch (parseState)
        {
            case 0:
            {
                if (c == 0x1B) /* ESC */
                {
                    parseState = 1;
                }
                else if (c == 0x03) /* CTRL-C */
                {
#ifdef ENABLE_SHUTDOWN
                    CsrSchedStop();
#else
                    exit(0);
#endif
                }
                else
                {
                    csrUiKey = handleAsciiKeyCodes(c);
                }
                break;
            }
            case 1:
            {
                if (c == '[')
                {
                    parseState = 2;
                }
                else if (c == 0x4F)
                {
                    parseState = 4;
                }
                else
                {
                    csrUiKey = 0;
                    parseState = 0;
                }
                break;
            }
            case 2:
            {
                if (c == 0x41)
                {
                    csrUiKey = CSR_UI_KEY_UP;
                    parseState = 0;
                }
                else if (c == 0x42)
                {
                    csrUiKey = CSR_UI_KEY_DOWN;
                    parseState = 0;
                }
                else if (c == 0x43)
                {
                    csrUiKey = CSR_UI_KEY_RIGHT;
                    parseState = 0;
                }
                else if (c == 0x44)
                {
                    csrUiKey = CSR_UI_KEY_LEFT;
                    parseState = 0;
                }
                else if (c == 0x31) /* Home */
                {
                    csrUiKey = CSR_UI_KEY_SK1;
                    parseState = 3;
                }
                else if (c == 0x33) /* Delete */
                {
                    csrUiKey = CSR_UI_KEY_DEL;
                    parseState = 3;
                }
                else if (c == 0x34) /* End */
                {
                    csrUiKey = CSR_UI_KEY_BACK;
                    parseState = 3;
                }
                else if (c == 0x35) /* PgUp */
                {
                    csrUiKey = CSR_UI_KEY_SK2;
                    parseState = 3;
                }
                else if (c == 0x36) /* PgDn */
                {
                    csrUiKey = CSR_UI_KEY_DEL;
                    parseState = 3;
                }
                else
                {
                    csrUiKey = 0;
                    parseState = 0;
                }
                break;
            }
            case 3:
            {
                if (c == '~')
                {
                    parseState = 0;
                }
                else
                {
                    csrUiKey = 0;
                    parseState = 0;
                }
                break;
            }
            case 4:
            {
                if (c == 0x48) /* Home */
                {
                    csrUiKey = CSR_UI_KEY_SK1;
                    parseState = 0;
                }
                else if (c == 0x46) /* End */
                {
                    csrUiKey = CSR_UI_KEY_BACK;
                    parseState = 0;
                }
                else
                {
                    csrUiKey = 0;
                    parseState = 0;
                }
                break;
            }
            default:
                break;
        }

        if ((csrUiKey != 0) && (parseState == 0))
        {
            CsrUiKeyEvent(csrUiKey);
        }
    }
}

static void dataDetectCallback(void)
{
    NU_Activate_HISR(&keyInputHisr);
}

void CsrKeyInputActivate(void)
{
    if (!initialised)
    {
        NU_Create_HISR(&keyInputHisr, "KEY_INPUT", keyInput, 1, keyInputHisrStack, KEY_INPUT_HISR_STACK_SIZE);
        ConsoleDataDetectorRegister(dataDetectCallback);
        initialised = TRUE;
    }
}

void CsrKeyInputDeactivate(void)
{
    if (initialised)
    {
        ConsoleDataDetectorUnregister();
        NU_Delete_HISR(&keyInputHisr);
        initialised = FALSE;
    }
}
