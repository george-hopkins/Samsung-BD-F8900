/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* This is necessary to be able to use DC_PEN and DC_BRUSH */
#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0500)
#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_unicode.h"

#include "csr_ui_lower.h"
#include "csr_ui_keycode.h"

#include "csr_sched_init.h"

#include "csr_phone_skin.h"

#define WINDOW_TITLE "Synergy Application"

#define DISPLAY_HEIGHT                             640
#define DISPLAY_WIDTH                              480

/* The offset of the phone */
#define PHONE_X_OFFSET                             (DISPLAY_WIDTH / 2 - PHONE_SKIN_WIDTH / 2)
#define PHONE_Y_OFFSET                             (0)

/* The size of the screen in the phone */
#define PHONE_SCREEN_WIDTH                         (256)
#define PHONE_SCREEN_HEIGHT                        (320)

/* The offset of the screen */
#define PHONE_SCREEN_X_OFFSET                      (PHONE_X_OFFSET + 32)
#define PHONE_SCREEN_Y_OFFSET                      (PHONE_Y_OFFSET + 57)

/* The SK1, SK2, BACK and DEL keys */
#define PHONE_SK_X_OFFSET                          (PHONE_SCREEN_X_OFFSET + 1)
#define PHONE_SK_Y_OFFSET                          (PHONE_SCREEN_HEIGHT + PHONE_SCREEN_Y_OFFSET + 17)

#define PHONE_SK_HEIGHT                            (26)
#define PHONE_SK_WIDTH                             (82)
#define PHONE_SK_SPACE_BETWEEN_COLS                (94)
#define PHONE_SK_SPACE_BETWEEN_ROWS                (12)

#define PHONE_SK_ROW_ONE_TOP                       (PHONE_SK_Y_OFFSET)
#define PHONE_SK_ROW_ONE_BOTTOM                    (PHONE_SK_Y_OFFSET + PHONE_SK_HEIGHT)
#define PHONE_SK_ROW_TWO_TOP                       (PHONE_SK_Y_OFFSET + PHONE_SK_HEIGHT + PHONE_SK_SPACE_BETWEEN_ROWS)
#define PHONE_SK_ROW_TWO_BOTTOM                    (PHONE_SK_Y_OFFSET + PHONE_SK_HEIGHT * 2 + PHONE_SK_SPACE_BETWEEN_ROWS)

#define PHONE_SK_COL_ONE_LEFT                      (PHONE_SK_X_OFFSET)
#define PHONE_SK_COL_ONE_RIGHT                     (PHONE_SK_X_OFFSET + PHONE_SK_WIDTH)
#define PHONE_SK_COL_TWO_LEFT                      (PHONE_SK_X_OFFSET + PHONE_SK_WIDTH + PHONE_SK_SPACE_BETWEEN_COLS)
#define PHONE_SK_COL_TWO_RIGHT                     (PHONE_SK_X_OFFSET + PHONE_SK_WIDTH * 2 + PHONE_SK_SPACE_BETWEEN_COLS)

/* The navigation keys */
#define PHONE_NAVIGATION_LEFT_KEY_TOP_X_OFFSET     (PHONE_SK_COL_ONE_RIGHT)
#define PHONE_NAVIGATION_LEFT_KEY_TOP_Y_OFFSET     (PHONE_SK_ROW_ONE_TOP - 3)
#define PHONE_NAVIGATION_LEFT_KEY_BOTTOM_X_OFFSET  (PHONE_NAVIGATION_LEFT_KEY_TOP_X_OFFSET + 28)
#define PHONE_NAVIGATION_LEFT_KEY_BOTTOM_Y_OFFSET  (PHONE_SK_ROW_TWO_BOTTOM + 8)

#define PHONE_NAVIGATION_UP_KEY_TOP_X_OFFSET       (PHONE_NAVIGATION_LEFT_KEY_BOTTOM_X_OFFSET)
#define PHONE_NAVIGATION_UP_KEY_TOP_Y_OFFSET       (PHONE_NAVIGATION_LEFT_KEY_TOP_Y_OFFSET)
#define PHONE_NAVIGATION_UP_KEY_BOTTOM_X_OFFSET    (PHONE_NAVIGATION_UP_KEY_TOP_X_OFFSET + 36)
#define PHONE_NAVIGATION_UP_KEY_BOTTOM_Y_OFFSET    (PHONE_NAVIGATION_UP_KEY_TOP_Y_OFFSET + 37)

#define PHONE_NAVIGATION_RIGHT_KEY_TOP_X_OFFSET    (PHONE_NAVIGATION_UP_KEY_BOTTOM_X_OFFSET)
#define PHONE_NAVIGATION_RIGHT_KEY_TOP_Y_OFFSET    (PHONE_SK_ROW_ONE_TOP - 3)
#define PHONE_NAVIGATION_RIGHT_KEY_BOTTOM_X_OFFSET (PHONE_NAVIGATION_RIGHT_KEY_TOP_X_OFFSET + 28)
#define PHONE_NAVIGATION_RIGHT_KEY_BOTTOM_Y_OFFSET (PHONE_SK_ROW_TWO_BOTTOM + 8)

#define PHONE_NAVIGATION_DOWN_KEY_TOP_X_OFFSET     (PHONE_NAVIGATION_LEFT_KEY_TOP_X_OFFSET + 28)
#define PHONE_NAVIGATION_DOWN_KEY_TOP_Y_OFFSET     (PHONE_NAVIGATION_UP_KEY_BOTTOM_Y_OFFSET)
#define PHONE_NAVIGATION_DOWN_KEY_BOTTOM_X_OFFSET  (PHONE_NAVIGATION_RIGHT_KEY_TOP_X_OFFSET)
#define PHONE_NAVIGATION_DOWN_KEY_BOTTOM_Y_OFFSET  (PHONE_NAVIGATION_RIGHT_KEY_BOTTOM_Y_OFFSET)

/* The rest of the keys */
#define PHONE_KEYS_X_OFFSET                        (PHONE_SK_X_OFFSET - 1)
#define PHONE_KEYS_Y_OFFSET                        (PHONE_SK_ROW_TWO_BOTTOM + 5)

#define PHONE_KEYS_HEIGHT                          (36)
#define PHONE_KEYS_WIDTH                           (86)
#define PHONE_KEYS_SPACE_BETWEEN_ROWS              (1)
#define PHONE_KEYS_SPACE_BETWEEN_COLS              (1)

#define PHONE_KEYS_ROW_ONE_TOP                     (PHONE_KEYS_Y_OFFSET)
#define PHONE_KEYS_ROW_TWO_TOP                     (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT + PHONE_KEYS_SPACE_BETWEEN_ROWS)
#define PHONE_KEYS_ROW_THREE_TOP                   (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT * 2 + PHONE_KEYS_SPACE_BETWEEN_ROWS * 2)
#define PHONE_KEYS_ROW_FOUR_TOP                    (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT * 3 + PHONE_KEYS_SPACE_BETWEEN_ROWS * 3)

#define PHONE_KEYS_ROW_ONE_BOTTOM                  (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT)
#define PHONE_KEYS_ROW_TWO_BOTTOM                  (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT * 2 + PHONE_KEYS_SPACE_BETWEEN_ROWS)
#define PHONE_KEYS_ROW_THREE_BOTTOM                (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT * 3 + PHONE_KEYS_SPACE_BETWEEN_ROWS * 2)
#define PHONE_KEYS_ROW_FOUR_BOTTOM                 (PHONE_KEYS_Y_OFFSET + PHONE_KEYS_HEIGHT * 4 + PHONE_KEYS_SPACE_BETWEEN_ROWS * 3)

#define PHONE_KEYS_COL_ONE_LEFT                    (PHONE_KEYS_X_OFFSET)
#define PHONE_KEYS_COL_TWO_LEFT                    (PHONE_KEYS_X_OFFSET + PHONE_KEYS_WIDTH + PHONE_KEYS_SPACE_BETWEEN_COLS)
#define PHONE_KEYS_COL_THREE_LEFT                  (PHONE_KEYS_X_OFFSET + PHONE_KEYS_WIDTH * 2 + PHONE_KEYS_SPACE_BETWEEN_COLS * 2)

#define PHONE_KEYS_COL_ONE_RIGHT                   (PHONE_KEYS_X_OFFSET + PHONE_KEYS_WIDTH)
#define PHONE_KEYS_COL_TWO_RIGHT                   (PHONE_KEYS_X_OFFSET + PHONE_KEYS_WIDTH * 2 + PHONE_KEYS_SPACE_BETWEEN_COLS)
#define PHONE_KEYS_COL_THREE_RIGHT                 (PHONE_KEYS_X_OFFSET + PHONE_KEYS_WIDTH * 3 + PHONE_KEYS_SPACE_BETWEEN_COLS * 2)

static CsrBool localGuiEnable = FALSE;
static CsrBool localTuiEnable = FALSE;

static HWND mainWindowHandle;
static CRITICAL_SECTION screenMutex;
static HDC displayHdc;

CsrUint16 processKeyWmChar(WPARAM key)
{
    switch (key)
    {
        case 13:    /* Enter */
            return CSR_UI_KEY_SK1;
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

CsrUint16 processKeyWmKeydown(WPARAM key)
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

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_PAINT:
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            EnterCriticalSection(&screenMutex);

            if (displayHdc != NULL)
            {
                BitBlt(hdc, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, displayHdc, 0, 0, SRCCOPY);
            }
            EndPaint(hWnd, &ps);

            LeaveCriticalSection(&screenMutex);

            break;
        }
        case WM_LBUTTONUP:
            if ((LOWORD(lParam) >= DISPLAY_WIDTH) || (HIWORD(lParam) >= DISPLAY_HEIGHT))
            {
                break;
            }
            CsrUiTouchEvent(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_CHAR:
        {
            CsrUint16 csrUiKey;
            /*printf("WM_CHAR: wParam: %d, lParam: %d\n", wParam, lParam);*/
            csrUiKey = processKeyWmChar(wParam);
            if (csrUiKey)
            {
                CsrUiKeyEvent(csrUiKey);
            }
            break;
        }
        case WM_KEYDOWN:
        {
            CsrUint16 csrUiKey;
            /*printf("WM_KEYDOWN: wParam: %d, lParam: %d\n", wParam, lParam);*/
            csrUiKey = processKeyWmKeydown(wParam);
            if (csrUiKey)
            {
                CsrUiKeyEvent(csrUiKey);
            }
            break;
        }
        case WM_DESTROY:
#ifdef ENABLE_SHUTDOWN
            CsrSchedStop();
#else
            exit(0);
#endif
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static void registerClass(void)
{
    HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(NULL);
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC) WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = WINDOW_TITLE;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);
}

static void createMainWindow(void)
{
    HBITMAP displayBitmap;
    HDC windowHDC;
    RECT displayRect = {0, 0, 480, 640};
    HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(NULL);

    mainWindowHandle = CreateWindow(
        WINDOW_TITLE,
        WINDOW_TITLE,
        WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        0,
        DISPLAY_WIDTH + 2 * GetSystemMetrics(SM_CXFIXEDFRAME),
        DISPLAY_HEIGHT + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYFIXEDFRAME),
        NULL,
        NULL,
        hInstance,
        NULL);

    /* Make a clone of the Main Window HDC and fill it with white */
    windowHDC = GetDC(mainWindowHandle);
    displayHdc = CreateCompatibleDC(windowHDC);
    displayBitmap = CreateCompatibleBitmap(windowHDC, 480, 640);
    SelectObject(displayHdc, displayBitmap);
    FillRect(displayHdc, &displayRect, (HBRUSH) GetStockObject(WHITE_BRUSH));
    ReleaseDC(mainWindowHandle, windowHDC);
}

static void registerKeys()
{
    /* The SK1, SK2, BACK and DEL keys */
    CsrUiRegisterKey(PHONE_SK_COL_ONE_LEFT, PHONE_SK_ROW_ONE_TOP, PHONE_SK_COL_ONE_RIGHT, PHONE_SK_ROW_ONE_BOTTOM, CSR_UI_KEY_SK1);
    CsrUiRegisterKey(PHONE_SK_COL_TWO_LEFT, PHONE_SK_ROW_ONE_TOP, PHONE_SK_COL_TWO_RIGHT, PHONE_SK_ROW_ONE_BOTTOM, CSR_UI_KEY_SK2);
    CsrUiRegisterKey(PHONE_SK_COL_ONE_LEFT, PHONE_SK_ROW_TWO_TOP, PHONE_SK_COL_ONE_RIGHT, PHONE_SK_ROW_TWO_BOTTOM, CSR_UI_KEY_BACK);
    CsrUiRegisterKey(PHONE_SK_COL_TWO_LEFT, PHONE_SK_ROW_TWO_TOP, PHONE_SK_COL_TWO_RIGHT, PHONE_SK_ROW_TWO_BOTTOM, CSR_UI_KEY_DEL);

    /* The numeric key */
    CsrUiRegisterKey(PHONE_KEYS_COL_ONE_LEFT, PHONE_KEYS_ROW_ONE_TOP, PHONE_KEYS_COL_ONE_RIGHT, PHONE_KEYS_ROW_ONE_BOTTOM, CSR_UI_KEY_N1);
    CsrUiRegisterKey(PHONE_KEYS_COL_TWO_LEFT, PHONE_KEYS_ROW_ONE_TOP, PHONE_KEYS_COL_TWO_RIGHT, PHONE_KEYS_ROW_ONE_BOTTOM, CSR_UI_KEY_N2);
    CsrUiRegisterKey(PHONE_KEYS_COL_THREE_LEFT, PHONE_KEYS_ROW_ONE_TOP, PHONE_KEYS_COL_THREE_RIGHT, PHONE_KEYS_ROW_ONE_BOTTOM, CSR_UI_KEY_N3);
    CsrUiRegisterKey(PHONE_KEYS_COL_ONE_LEFT, PHONE_KEYS_ROW_TWO_TOP, PHONE_KEYS_COL_ONE_RIGHT, PHONE_KEYS_ROW_TWO_BOTTOM, CSR_UI_KEY_N4);
    CsrUiRegisterKey(PHONE_KEYS_COL_TWO_LEFT, PHONE_KEYS_ROW_TWO_TOP, PHONE_KEYS_COL_TWO_RIGHT, PHONE_KEYS_ROW_TWO_BOTTOM, CSR_UI_KEY_N5);
    CsrUiRegisterKey(PHONE_KEYS_COL_THREE_LEFT, PHONE_KEYS_ROW_TWO_TOP, PHONE_KEYS_COL_THREE_RIGHT, PHONE_KEYS_ROW_TWO_BOTTOM, CSR_UI_KEY_N6);
    CsrUiRegisterKey(PHONE_KEYS_COL_ONE_LEFT, PHONE_KEYS_ROW_THREE_TOP, PHONE_KEYS_COL_ONE_RIGHT, PHONE_KEYS_ROW_THREE_BOTTOM, CSR_UI_KEY_N7);
    CsrUiRegisterKey(PHONE_KEYS_COL_TWO_LEFT, PHONE_KEYS_ROW_THREE_TOP, PHONE_KEYS_COL_TWO_RIGHT, PHONE_KEYS_ROW_THREE_BOTTOM, CSR_UI_KEY_N8);
    CsrUiRegisterKey(PHONE_KEYS_COL_THREE_LEFT, PHONE_KEYS_ROW_THREE_TOP, PHONE_KEYS_COL_THREE_RIGHT, PHONE_KEYS_ROW_THREE_BOTTOM, CSR_UI_KEY_N9);
    CsrUiRegisterKey(PHONE_KEYS_COL_ONE_LEFT, PHONE_KEYS_ROW_FOUR_TOP, PHONE_KEYS_COL_ONE_RIGHT, PHONE_KEYS_ROW_FOUR_BOTTOM, CSR_UI_KEY_STAR);
    CsrUiRegisterKey(PHONE_KEYS_COL_TWO_LEFT, PHONE_KEYS_ROW_FOUR_TOP, PHONE_KEYS_COL_TWO_RIGHT, PHONE_KEYS_ROW_FOUR_BOTTOM, CSR_UI_KEY_N0);
    CsrUiRegisterKey(PHONE_KEYS_COL_THREE_LEFT, PHONE_KEYS_ROW_FOUR_TOP, PHONE_KEYS_COL_THREE_RIGHT, PHONE_KEYS_ROW_FOUR_BOTTOM, CSR_UI_KEY_HASH);

    /* The navigation keys */
    CsrUiRegisterKey(PHONE_NAVIGATION_LEFT_KEY_TOP_X_OFFSET, PHONE_NAVIGATION_LEFT_KEY_TOP_Y_OFFSET, PHONE_NAVIGATION_LEFT_KEY_BOTTOM_X_OFFSET,
        PHONE_NAVIGATION_LEFT_KEY_BOTTOM_Y_OFFSET, CSR_UI_KEY_LEFT);

    CsrUiRegisterKey(PHONE_NAVIGATION_RIGHT_KEY_TOP_X_OFFSET, PHONE_NAVIGATION_RIGHT_KEY_TOP_Y_OFFSET, PHONE_NAVIGATION_RIGHT_KEY_BOTTOM_X_OFFSET,
        PHONE_NAVIGATION_RIGHT_KEY_BOTTOM_Y_OFFSET, CSR_UI_KEY_RIGHT);

    CsrUiRegisterKey(PHONE_NAVIGATION_UP_KEY_TOP_X_OFFSET, PHONE_NAVIGATION_UP_KEY_TOP_Y_OFFSET, PHONE_NAVIGATION_UP_KEY_BOTTOM_X_OFFSET,
        PHONE_NAVIGATION_UP_KEY_BOTTOM_Y_OFFSET, CSR_UI_KEY_UP);

    CsrUiRegisterKey(PHONE_NAVIGATION_DOWN_KEY_TOP_X_OFFSET, PHONE_NAVIGATION_DOWN_KEY_TOP_Y_OFFSET, PHONE_NAVIGATION_DOWN_KEY_BOTTOM_X_OFFSET,
        PHONE_NAVIGATION_DOWN_KEY_BOTTOM_Y_OFFSET, CSR_UI_KEY_DOWN);
}

#define PIXEL_RGB(red, green, blue)           ((CsrUint32) ((blue) << 16 | (green) << 8 | (red)))

void CsrUiLowerInitialise(CsrBool guiEnable, CsrBool tuiEnable, CsrBool tuiNoCls)
{
    CsrUint32 xc, yc, index;

    localGuiEnable = guiEnable;
    localTuiEnable = tuiEnable;
    CSR_UNUSED(tuiNoCls);

    InitializeCriticalSection(&screenMutex);
    registerClass();
    createMainWindow();

    if (guiEnable)
    {
        /* Draw the picture */
        index = 0;
        for (yc = PHONE_SKIN_HEIGHT; yc > 0; yc--)
        {
            for (xc = 0; xc < PHONE_X_OFFSET; xc++)
            {
                SetPixel(displayHdc, xc, yc - 1, PIXEL_RGB(255, 255, 255));
            }
            for (xc = 0; xc < PHONE_SKIN_WIDTH; xc++)
            {
                SetPixel(displayHdc, PHONE_X_OFFSET + xc, (PHONE_Y_OFFSET + yc - 1), PIXEL_RGB(phoneSkinImageData[index], phoneSkinImageData[index + 1], phoneSkinImageData[index + 2]));
                index += 3;
            }
            for (xc = PHONE_SKIN_WIDTH + PHONE_X_OFFSET; xc < DISPLAY_WIDTH; xc++)
            {
                SetPixel(displayHdc, xc, yc - 1, PIXEL_RGB(255, 255, 255));
            }
        }

        /* Register the keys on the phone */
        registerKeys();
    }

    ShowWindow(mainWindowHandle, SW_SHOWNORMAL);
    UpdateWindow(mainWindowHandle);
}

/* Called by CSR_UI to paint a particular pixel with a specified colour */
static void setPixel(CsrInt16 x, CsrInt16 y, CsrUint32 colour)
{
    SetPixel(displayHdc, x, y, colour);
}

static void drawBox(CsrInt16 left, CsrInt16 top, CsrInt16 right, CsrInt16 bottom, CsrUint32 outlineColor, CsrUint32 fillColor, CsrBool fill)
{
    SelectObject(displayHdc, GetStockObject(DC_PEN));
    SetDCPenColor(displayHdc, outlineColor);

    /* Vertical line sub-case */
    if (left == right)
    {
        MoveToEx(displayHdc, left, top, NULL);
        LineTo(displayHdc, left, bottom + 1);
        return;
    }

    /* Horizontal line sub-case */
    if (top == bottom)
    {
        MoveToEx(displayHdc, left, top, NULL);
        LineTo(displayHdc, right + 1, top);
        return;
    }

    if (fill)
    {
        SelectObject(displayHdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(displayHdc, fillColor);
    }
    else
    {
        SelectObject(displayHdc, GetStockObject(NULL_BRUSH));
    }

    Rectangle(displayHdc, left, top, right + 1, bottom + 1);
}

/* Called by CSR_UI before a redraw of the display is started */
static void lock(void)
{
    EnterCriticalSection(&screenMutex);
}

/* Called by CSR_UI after a redraw of the display completes */
static void unlock(void)
{
    LeaveCriticalSection(&screenMutex);

    /* Invalidate the entire window, forcing a redraw */
    InvalidateRect(mainWindowHandle, NULL, FALSE);
}

void CsrUiGraphicsRendererConfigurationGet(CsrUiGraphicsRenderer *renderer)
{
    renderer->active = localGuiEnable;
    renderer->top = PHONE_SCREEN_Y_OFFSET;
    renderer->left = PHONE_SCREEN_X_OFFSET;
    renderer->setPixel = setPixel;
    renderer->drawBox = drawBox;
    renderer->lock = lock;
    renderer->unlock = unlock;
}

static void setText(CsrUint16 *status, CsrUint16 *canvas, CsrUint16 *softkey1, CsrUint16 *softkey2)
{
    system("cls");
    wprintf(L"\n\n********************************************************************************\n\n");
    if (status != NULL)
    {
        wprintf(L"%s\n\n", status);
    }
    if (canvas != NULL)
    {
        wprintf(L"%s\n\n", canvas);
    }
    if (softkey1 != NULL)
    {
        wprintf(L"%-38s || ", softkey1);
    }
    if (softkey2 != NULL)
    {
        wprintf(L"%38s", softkey2);
    }
    wprintf(L"\n\n********************************************************************************");
}

void CsrUiTextRendererConfigurationGet(CsrUiTextRenderer *renderer)
{
    renderer->active = localTuiEnable;
    renderer->setText = setText;
}
