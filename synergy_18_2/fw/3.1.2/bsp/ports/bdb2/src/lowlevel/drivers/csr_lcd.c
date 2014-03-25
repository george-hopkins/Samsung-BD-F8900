/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_lcd.h"
#include "platform/csr_gpio.h"
#include "platform/csr_delay.h"
#include "platform/csr_timer.h"

/* Configuration */
#define BURSTLEN    0x0         /* 16-word burst read */
#define DIVEN       0x1         /* Enable clock */
#define CLKVAL      0x1         /* 25MHz LCD clock */
#define CLKDIR      0x1         /* Use CLKVAL */
#define PNRMODE     0x1         /* BGR Display Mode */
#define BPPMODEF    0x7         /* 18bpp */
#define BPPMODEB    0x7         /* 18bpp */
#define IVCLK       0x1         /* Latch pixel data at rising edge of clock */
#define IHSYNC      0x1         /* Invert HSYNC */
#define IVSYNC      0x1         /* Invert VSYNC */
#define IVDEN       0x0         /* Don't invert VDEN (valid data indicator) */
#define BITSWP      0x0         /* No bit swapping */
#define BYTSWP      0x0         /* No byte swapping */
#define HAWSWP      0x0         /* No half-word swapping */
#define VBPD        (2 - 1)     /* Vertical Back Porch */
#define VFPD        (16 - 1)    /* Vertical Front Porch */
#define VSPW        (2 - 1)     /* Vertical Sync Period */
#define VS          (VBPD + 1 + VFPD + 1 + VSPW + 1 + LCD_HEIGHT)   /* Vertical Cycle Length */
#define HBPD        (8 - 1)     /* Horizontal Back Porch */
#define HFPD        (104 - 1)   /* Horizontal Front Porch */
#define HSPW        (8 - 1)     /* Horisontal Sync Period */
#define HS          (HBPD + 1 + HFPD + 1 + HSPW + 1 + LCD_WIDTH)    /* Horizontal Cycle Length */
#define LINEVAL     (LCD_HEIGHT - 1)
#define HOZVAL      (LCD_WIDTH - 1)

/* Chip Select Output @ GPIO24 */
#define XCS_SET()     GPIO_SET(24)
#define XCS_CLEAR()   GPIO_CLEAR(24)

/* Serial Clock Output @ GPIO25 */
#define SCL_SET()     GPIO_SET(25)
#define SCL_CLEAR()   GPIO_CLEAR(25)
#define SCL_CLOCK()   {SCL_CLEAR(); \
                       DelayUs(100); \
                       SCL_SET(); \
                       DelayUs(100);}

/* Serial Data Input to LCD module @ GPIO26*/
#define DIN_SET()     GPIO_SET(26)
#define DIN_CLEAR()   GPIO_CLEAR(26)
#define DIN_IN()      GPIOPinConfigure(GPIO_PIN_26, GPIO_FUNCTION_INPUT, GPIO_PULLUP_OFF, GPIO_TRIGGER_LOWLEVEL)
#define DIN_OUT()     GPIOPinConfigure(GPIO_PIN_26, GPIO_FUNCTION_OUTPUT, GPIO_PULLUP_OFF, GPIO_TRIGGER_LOWLEVEL)

/* Serial Data Output from LCD module @ GPIO27 */
#define DOUT          GPIO_IN(27)

/* Clock a single byte out on serial interface */
static void clockByte(CsrUint8 byte)
{
    CsrUint8 i;

    for (i = 0x80; i != 0; i >>= 1)
    {
        if (i & byte)
        {
            DIN_SET();
        }
        else
        {
            DIN_CLEAR();
        }

        SCL_CLOCK();
    }
}

/* Send a command and 0 to 4 parameters to the serial interface */
static void serialCommandDataSend(CsrUint8 command, CsrUint8 bytes, CsrUint32 data)
{
    CsrInt32 i;

    XCS_CLEAR(); /* Enable target */
    DIN_OUT();   /* Set DIN as output */

    DIN_CLEAR(); /* D/C bit = Command */
    SCL_CLOCK(); /* Clock out */

    /* Clock out 8 command bits */
    clockByte(command);

    for (i = 0; i < bytes; i++)
    {
        DIN_SET(); /* D/C bit = Data */
        SCL_CLOCK(); /* Clock out */

        /* Clock out 8 data bits */
        clockByte((CsrUint8) (0xFF & (data >> (8 * (bytes - i - 1)))));
    }

    DIN_CLEAR(); /* Reset to default state */
    XCS_SET(); /* Disable target */
    DelayUs(1000);
}

/*
Perform initialisation of LCD module
    1) Setup XCS, SCL and DIN as outputs
    2) Perform initialisation sequence of commands to LCD module
    3) Set up frame buffers and timing in processor */
void LCDInitialise(CsrUint32 *fb0, CsrUint32 *fb1)
{
    /* Initialise Serial Connection */
    XCS_SET();
    SCL_SET();
    DIN_CLEAR();
    DelayUs(150000); /* Time from reset release to first command must be at least 150ms */

    /* Initialise LCD module */
    serialCommandDataSend(0, 0, 0);
    DelayUs(1000);
    serialCommandDataSend(0, 0, 0);
    DelayUs(1000);
    serialCommandDataSend(0, 0, 0);
    DelayUs(1000);

    serialCommandDataSend(0xBC, 1, 0x80); /* SIGCON enable */
    serialCommandDataSend(0xBF, 1, 0x10); /* VCOMAC enable */
    serialCommandDataSend(0xB1, 1, 0x56); /* Booster voltage adjustment */
    serialCommandDataSend(0xB5, 1, 0x35); /* VCS voltage adjustment */
    serialCommandDataSend(0xB9, 1, 0x04); /* DCCLK and DCEV timing adjustment */
    serialCommandDataSend(0xBD, 1, 0x00); /* ASW signal slew rate adjustment */
    serialCommandDataSend(0xEC, 2, HS + 16); /* Set Horizontal Cycle */
    serialCommandDataSend(0xD0, 2, (((HBPD + 1) + (HSPW + 1)) << 8) | ((VBPD + 1) + (VSPW + 1))); /* Set Horizontal and Vertical Blanking */

    serialCommandDataSend(0x11, 0, 0); /* Sleep out */

    DelayUs(40000); /* 40 ms delay */

    serialCommandDataSend(0x29, 0, 0); /* Display on */

    /* Setup timing */
    rLCDCON1 = (BURSTLEN << 28) |
               (DIVEN << 19) |
               (CLKVAL << 13) |
               (CLKDIR << 12) |
               (PNRMODE << 9) |
               (BPPMODEF << 6) |
               (BPPMODEB << 2);

    rLCDCON2 = (IVCLK << 7) |
               (IHSYNC << 6) |
               (IVSYNC << 5) |
               (IVDEN << 3) |
               (BITSWP << 2) |
               (BYTSWP << 1) |
               (HAWSWP);

    rLCDTCON1 = (VBPD << 16) | (VFPD << 8) | VSPW;
    rLCDTCON2 = (HBPD << 16) | (HFPD << 8) | HSPW;
    rLCDTCON3 = (LINEVAL << 11) | HOZVAL;

    /* Set up frame buffers */
    if (fb0)
    {
        rLCDSADDRB1 = (CsrUint32) fb0;
        rLCDEADDRB1 = ((CsrUint32) fb0) + LCD_WIDTH * LCD_HEIGHT * LCD_PIXELSIZE - 1;
        LCDClear(fb0);
    }
    if (fb1)
    {
        rLCDSADDRB2 = (CsrUint32) fb1;
        rLCDEADDRB2 = ((CsrUint32) fb1) + LCD_WIDTH * LCD_HEIGHT * LCD_PIXELSIZE - 1;
        LCDClear(fb1);
    }

    /* Time from last command to analog signals must be at least 10 ms */
    DelayUs(10000);

    /* Enable display with frame buffer 0 active */
    if (fb0)
    {
        LCD_FB_SET(0);
        LCD_ON();
    }
    else if (fb1)
    {
        LCD_FB_SET(1);
        LCD_ON();
    }
}

void LCDClear(CsrUint32 *fb)
{
    CsrInt32 i;

    for (i = 0; i < (LCD_WIDTH * LCD_HEIGHT); i++)
    {
        fb[i] = LCD_RGB(0, 0, 0);
    }
}

void LCDDrawBox(CsrUint32 *fb, CsrUint16 left, CsrUint16 top, CsrUint16 right, CsrUint16 bottom, CsrUint32 outlineColour, CsrUint32 fillColour, CsrBool fill)
{
    CsrUint32 *ptr1;
    CsrUint32 *ptr2;
    CsrUint16 x, y, i;

    /* Vertical line sub-case */
    if (left == right)
    {
        ptr1 = fb + top * LCD_WIDTH + left;
        for (i = top; i <= bottom; i++)
        {
            *ptr1 = outlineColour;
            ptr1 += LCD_WIDTH;
        }
        return;
    }

    /* Horizontal line sub-case */
    if (top == bottom)
    {
        ptr1 = fb + top * LCD_WIDTH + left;
        for (i = left; i <= right; i++)
        {
            *ptr1++ = outlineColour;
        }
        return;
    }

    /* Draw outline */
    ptr1 = fb + top * LCD_WIDTH + left;
    ptr2 = fb + bottom * LCD_WIDTH + left;
    for (i = left; i <= right; i++)
    {
        *ptr1++ = *ptr2++ = outlineColour;
    }

    ptr1 = fb + (top + 1) * LCD_WIDTH + left;
    ptr2 = fb + (top + 1) * LCD_WIDTH + right;
    for (i = top + 1; i < bottom; i++)
    {
        *ptr1 = outlineColour;
        *ptr2 = outlineColour;
        ptr1 += LCD_WIDTH;
        ptr2 += LCD_WIDTH;
    }


    /* Fill */
    if (fill)
    {
        for (y = top + 1; y < bottom; y++)
        {
            ptr1 = fb + y * LCD_WIDTH + (left + 1);
            for (x = left + 1; x < right; x++)
            {
                *ptr1++ = fillColour;
            }
        }
    }
}
