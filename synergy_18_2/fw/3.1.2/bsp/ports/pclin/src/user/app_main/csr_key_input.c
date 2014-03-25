/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <poll.h>
#include <unistd.h>

#include "csr_types.h"
#include "csr_macro.h"
#include "csr_pmem.h"
#include "csr_ui_keycode.h"
#include "csr_ui_lower.h"

#include "csr_sched_init.h"

static CsrBool initialised = FALSE;

static int threadTerminatePipe[2];
static pthread_t thread;

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

static void *threadFunction(void *nothing)
{
    static struct pollfd handles[2];
    CsrUint8 parseState = 0;
    CsrUint16 csrUiKey = 0;

    CSR_UNUSED(nothing);

    handles[0].fd = fileno(stdin);
    handles[0].events = POLLIN;
    handles[1].fd = threadTerminatePipe[0];
    handles[1].events = POLLIN;

    while (TRUE)
    {
        if ((poll(handles, 2, -1) < 0) || (handles[1].revents & POLLIN))
        {
            return NULL;
        }

        if (handles[0].revents & POLLIN)
        {
            char c = 0;

            if (read(fileno(stdin), &c, 1) < 0)
            {
                return NULL;
            }

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
                        return NULL;
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
}

static struct termios orig_term;

void CsrKeyInputActivate(void)
{
    if (!initialised)
    {
        struct termios raw_term;

        /* save terminal settings */
        tcgetattr(fileno(stdin), &orig_term);
        raw_term = orig_term;
        raw_term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        raw_term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        raw_term.c_cflag &= ~(CSIZE | PARENB);
        raw_term.c_cflag |= CS8;
        raw_term.c_cc[VMIN] = 0;
        raw_term.c_cc[VTIME] = 0;
        tcsetattr(fileno(stdin), TCSANOW, &raw_term);

        if (pipe(threadTerminatePipe) == 0)
        {
            pthread_create(&thread, NULL, threadFunction, NULL);
            initialised = TRUE;
        }
        else
        {
            tcsetattr(fileno(stdin), TCSANOW, &orig_term);
        }
    }
}

void CsrKeyInputDeactivate(void)
{
    if (initialised)
    {
        tcsetattr(fileno(stdin), TCSANOW, &orig_term);
        if (write(threadTerminatePipe[1], "1", 1) > 0)
        {
            pthread_join(thread, NULL);
            close(threadTerminatePipe[0]);
            close(threadTerminatePipe[1]);
            initialised = FALSE;
        }
    }
}
