/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>

#include "nucleus.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_unicode.h"

#include "csr_ui_lower.h"
#include "csr_ui_keycode.h"

#include "platform/csr_lcd.h"
#include "platform/csr_touch.h"
#include "platform/csr_confdata.h"

#include "csr_phone_skin.h"

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

#define TOUCH_HISR_STACK_SIZE 4096
static NU_HISR touchHISR;
static CsrUint32 *fb;
static CsrUint8 touchHISRStack[TOUCH_HISR_STACK_SIZE];

static CsrBool localGuiEnable = FALSE;
static CsrBool localTuiEnable = FALSE;

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

/* High Level Interrupt Service Routine (HISR) to dispatch event to CSR_UI */
static void touchHISRFunction(void)
{
    TouchPoint point;

    TouchPointGet(&point);

    /* Call CSR_UI to deliver the touch event */
    CsrUiTouchEvent(point.x, point.y);
}

/* Called by touch screen driver (registered as callback function in main.c) */
static void touchEvent(CsrInt32 event, TouchPoint touch)
{
    if (event == TOUCH_EVENT_PENDOWN)
    {
        NU_Activate_HISR(&touchHISR);
    }
}

void CsrUiLowerInitialise(CsrBool guiEnable, CsrBool tuiEnable, CsrBool tuiNoCls)
{
    CsrUint32 xc, yc, index;
    TouchCalibrationData touchCalibrationData;

    localGuiEnable = guiEnable;
    localTuiEnable = tuiEnable;
    CSR_UNUSED(tuiNoCls);

    if (guiEnable)
    {
        /* Create Touch Event HISR */
        NU_Create_HISR(&touchHISR, "TOUCH", touchHISRFunction, 1, touchHISRStack, TOUCH_HISR_STACK_SIZE);

        /* Initialise Touch Screen */
        ConfDataLightweightGetValue(CONFDATA_TAG_TOUCHCALIB, sizeof(TouchCalibrationData), (CsrUint8 *) &touchCalibrationData);
        TouchInitialise(touchEvent);
        TouchCalibrationDataSet(&touchCalibrationData);

        /* Initialise LCD */
        LCDInitialise(LCD_FB0, LCD_FB1);

        /* Draw the picture */
        index = 0;
        for (yc = PHONE_SKIN_HEIGHT; yc > 0; yc--)
        {
            for (xc = 0; xc < PHONE_X_OFFSET; xc++)
            {
                LCD_FB_SET_PIXEL(LCD_FB0, xc, yc - 1, LCD_RGB(255, 255, 255));
                LCD_FB_SET_PIXEL(LCD_FB1, xc, yc - 1, LCD_RGB(255, 255, 255));
            }
            for (xc = 0; xc < PHONE_SKIN_WIDTH; xc++)
            {
                LCD_FB_SET_PIXEL(LCD_FB0, PHONE_X_OFFSET + xc, (PHONE_Y_OFFSET + yc - 1), LCD_RGB(phoneSkinImageData[index], phoneSkinImageData[index + 1], phoneSkinImageData[index + 2]));
                LCD_FB_SET_PIXEL(LCD_FB1, PHONE_X_OFFSET + xc, (PHONE_Y_OFFSET + yc - 1), LCD_RGB(phoneSkinImageData[index], phoneSkinImageData[index + 1], phoneSkinImageData[index + 2]));
                index += 3;
            }
            for (xc = PHONE_SKIN_WIDTH + PHONE_X_OFFSET; xc < DISPLAY_WIDTH; xc++)
            {
                LCD_FB_SET_PIXEL(LCD_FB0, xc, yc - 1, LCD_RGB(255, 255, 255));
                LCD_FB_SET_PIXEL(LCD_FB1, xc, yc - 1, LCD_RGB(255, 255, 255));
            }
        }

        /* Register the keys on the phone */
        registerKeys();

        /* Turn on the LCD */
        LCD_ON_FE();
    }
}

/* Called by CSR_UI to paint a particular pixel with a specified colour */
static void setPixel(CsrInt16 x, CsrInt16 y, CsrUint32 colour)
{
    LCD_FB_SET_PIXEL(fb, x, y, colour);
}

static void drawBox(CsrInt16 left, CsrInt16 top, CsrInt16 right, CsrInt16 bottom, CsrUint32 outlineColour, CsrUint32 fillColour, CsrBool fill)
{
    LCDDrawBox(fb, left, top, right, bottom, outlineColour, fillColour, fill);
}

/* Called by CSR_UI before a redraw of the display is started */
static void lock(void)
{
    if (LCD_FB_ACTIVE_GET())
    {
        fb = LCD_FB0;
    }
    else
    {
        fb = LCD_FB1;
    }
}

/* Called by CSR_UI after a redraw of the display completes */
static void unlock(void)
{
    LCD_FB_TOGGLE();
}

void CsrUiGraphicsRendererConfigurationGet(CsrUiGraphicsRenderer *renderer)
{
    renderer->active = localGuiEnable;
    renderer->top = PHONE_SCREEN_Y_OFFSET;
    renderer->left = PHONE_SCREEN_X_OFFSET;
    renderer->setPixel = &setPixel;
    renderer->drawBox = &drawBox;
    renderer->lock = &lock;
    renderer->unlock = &unlock;
}

static void setText(CsrUint16 *status, CsrUint16 *canvas, CsrUint16 *softkey1, CsrUint16 *softkey2)
{
    CsrUtf8String *asciiStatus = CsrUtf16String2Utf8(status);
    CsrUtf8String *asciiCanvas = CsrUtf16String2Utf8(canvas);
    CsrUtf8String *asciiSoftkey1 = CsrUtf16String2Utf8(softkey1);
    CsrUtf8String *asciiSoftkey2 = CsrUtf16String2Utf8(softkey2);

    printf("\n\n********************************************************************************\n\n");
    if (status != NULL)
    {
        printf("%s\n\n", (CsrCharString *) asciiStatus);
        CsrPmemFree(asciiStatus);
    }
    if (canvas != NULL)
    {
        printf("%s\n\n", (CsrCharString *) asciiCanvas);
        CsrPmemFree(asciiCanvas);
    }

    if (softkey1 != NULL)
    {
        printf("%-38s || ", (CsrCharString *) asciiSoftkey1);
        CsrPmemFree(asciiSoftkey1);
    }
    if (softkey2 != NULL)
    {
        printf("%38s", (CsrCharString *) asciiSoftkey2);
        CsrPmemFree(asciiSoftkey2);
    }
    printf("\n\n********************************************************************************");
}

void CsrUiTextRendererConfigurationGet(CsrUiTextRenderer *renderer)
{
    renderer->active = localTuiEnable;
    renderer->setText = setText;
}
