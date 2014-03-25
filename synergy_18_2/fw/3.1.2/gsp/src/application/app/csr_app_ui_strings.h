#ifndef CSR_APP_UI_STRINGS_H__
#define CSR_APP_UI_STRINGS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_CSR_DEMO_APPLICATION_STRING        "CSR DEMO APPLICATION"
#define TEXT_OK_STRING                          "OK"
#define TEXT_SELECT_STRING                      "Select"
#define TEXT_MAIN_MENU_STRING                   "Menu"
#define TEXT_BACKLOG_STRING                     "Backlog"
#define TEXT_EXIT_STRING                        "Exit"

#define CONVERT_TEXT_STRING_2_UTF16(_x) CsrUtf82Utf16String((CsrUtf8String *) _x)

/* UI text defines                          */
#define TEXT_CSR_DEMO_APPLICATION_UTF16          CONVERT_TEXT_STRING_2_UTF16(TEXT_CSR_DEMO_APPLICATION_STRING)
#define TEXT_OK_UTF16                            CONVERT_TEXT_STRING_2_UTF16(TEXT_OK_STRING)
#define TEXT_SELECT_UTF16                        CONVERT_TEXT_STRING_2_UTF16(TEXT_SELECT_STRING)
#define TEXT_MAIN_MENU_UTF16                     CONVERT_TEXT_STRING_2_UTF16(TEXT_MAIN_MENU_STRING)
#define TEXT_BACKLOG_UTF16                       CONVERT_TEXT_STRING_2_UTF16(TEXT_BACKLOG_STRING)
#define TEXT_EXIT_UTF16                          CONVERT_TEXT_STRING_2_UTF16(TEXT_EXIT_STRING)

#ifdef __cplusplus
}
#endif

#endif
