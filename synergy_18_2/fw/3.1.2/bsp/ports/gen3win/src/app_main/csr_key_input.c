/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_ui_keycode.h"
#include "csr_ui_lower.h"

static CsrBool initialised = FALSE;
static HANDLE thread;

#define HANDLE_THREAD_TERMINATE 0
#define HANDLE_CONSOLE 1
#define HANDLE_COUNT 2
static HANDLE handles[HANDLE_COUNT];

static CsrUint16 processKeyAscii(WPARAM key)
{
    switch (key)
    {
        case 13:    /* Enter */
            return CSR_UI_KEY_SK1;
        case 8:     /* Backspace */
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

static CsrUint16 processKeyVirtualKeyCode(WPARAM key)
{
    switch (key)
    {
        case 37:    /* Left Arrow */
            return CSR_UI_KEY_LEFT;
        case 38:    /* Up Arrow */
            return CSR_UI_KEY_UP;
        case 39:    /* Right Arrow */
            return CSR_UI_KEY_RIGHT;
        case 40:    /* Down Arrow */
            return CSR_UI_KEY_DOWN;
        case 13:    /* Enter */
            return CSR_UI_KEY_SK1;
        case 8:     /* Backspace */
            return CSR_UI_KEY_BACK;
        case 46:    /* Delete */
            return CSR_UI_KEY_DEL;
        case 36:    /* Home */
            return CSR_UI_KEY_SK1;
        case 33:    /* PgUp */
            return CSR_UI_KEY_SK2;
        case 35:    /* End */
            return CSR_UI_KEY_BACK;
        case 34:    /* PgDown */
            return CSR_UI_KEY_DEL;
    }
    return 0;
}

static void handleConsoleKeyDownEvent(KEY_EVENT_RECORD ker)
{
    CsrUint16 csrUiKey = 0;

    if (ker.dwControlKeyState & ENHANCED_KEY)
    {
        csrUiKey = processKeyVirtualKeyCode(ker.wVirtualKeyCode);
    }
    else
    {
        csrUiKey = processKeyAscii(ker.uChar.AsciiChar);
    }
    if (csrUiKey)
    {
        CsrUiKeyEvent(csrUiKey);
    }
}

static DWORD WINAPI threadFunction(LPVOID nothing)
{
    handles[HANDLE_CONSOLE] = GetStdHandle(STD_INPUT_HANDLE);

    while (TRUE)
    {
        INPUT_RECORD ir;
        DWORD cEventsRead;
        DWORD waitResult;

        waitResult = WaitForMultipleObjects(HANDLE_COUNT, handles, FALSE, INFINITE);

        if (waitResult != (WAIT_OBJECT_0 + HANDLE_CONSOLE))
        {
            break;
        }

        if (!ReadConsoleInputA(handles[HANDLE_CONSOLE], &ir, 1, &cEventsRead))
        {
            break;
        }

        if (ir.EventType & KEY_EVENT)
        {
            KEY_EVENT_RECORD ker = ir.Event.KeyEvent;
            if (ker.bKeyDown)
            {
                handleConsoleKeyDownEvent(ker);
            }
        }
    }

    ExitThread(0);
    return 0;
}

void CsrKeyInputActivate(void)
{
    if (!initialised)
    {
        handles[HANDLE_THREAD_TERMINATE] = CreateEvent(NULL, FALSE, FALSE, NULL);
        thread = CreateThread(NULL, 0, threadFunction, NULL, 0, NULL);
        initialised = TRUE;
    }
}

void CsrKeyInputDeactivate(void)
{
    if (initialised)
    {
        SetEvent(handles[HANDLE_THREAD_TERMINATE]);
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
        CloseHandle(handles[HANDLE_THREAD_TERMINATE]);
        initialised = FALSE;
    }
}
