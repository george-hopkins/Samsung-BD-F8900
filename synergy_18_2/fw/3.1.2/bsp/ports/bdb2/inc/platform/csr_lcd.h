#ifndef CSR_LCD_H__
#define CSR_LCD_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_sdram.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_WIDTH       480
#define LCD_HEIGHT      640
#define LCD_PIXELSIZE   4       /* Number of bytes per pixel in frame buffer */

/* Pointers to the two default frame buffers (the two default framebuffers are at top of SDRAM - 3MiB) */
#define LCD_FB0         ((CsrUint32 *) (SDRAM_TOP - 1024 * 1024 * 3))
#define LCD_FB1         ((CsrUint32 *) (SDRAM_TOP - 1024 * 1024 * 3 + (LCD_WIDTH * LCD_HEIGHT * LCD_PIXELSIZE)))

/* Generate a colour value for a pixel (range of each components colour is 0-255) */
#define LCD_RGB(red, green, blue)           ((CsrUint32) ((blue) << 16 | (green) << 8 | (red)))

/* Set a pixel to a specified colour (use LCD_RGB for 3rd argument) in default frame buffer 0 */
#define LCD_SET_PIXEL(x, y, colour)         {*((CsrUint32 *) ((LCD_FB0) + (y) * LCD_WIDTH + (x))) = (colour);}

/* Set a pixel to a specified colour (use LCD_RGB for 4th argument) in a specified frame buffer */
#define LCD_FB_SET_PIXEL(fb, x, y, colour)  {*((CsrUint32 *) ((fb) + (y) * LCD_WIDTH + (x))) = (colour);}

/* Instant on/off control of driving signals */
#define LCD_OFF()           BIT_CLEAR(rLCDCON1, 1)
#define LCD_ON()            BIT_SET(rLCDCON1, 1)
#define LCD_TOGGLE()        BIT_TOGGLE(rLCDCON1, 1)

/* Frame synchronised on/off control of driving signals */
#define LCD_ON_FE()         REG_SET(rLCDCON1, 1, 0, 3)
#define LCD_OFF_FE()        REG_SET(rLCDCON1, 1, 0, 0)
#define LCD_IS_ACTIVE()     BIT_CHECK(rLCDCON1, 0)

/* Set/toggle the active frame buffer */
#define LCD_FB_SET(number)  {if (number) {BIT_SET(rLCDCON1, 21)} else {BIT_CLEAR(rLCDCON1, 21)}}
#define LCD_FB_TOGGLE()     BIT_TOGGLE(rLCDCON1, 21)

/* Get active/inactive frame buffer number */
#define LCD_FB_ACTIVE_GET()             (BIT_CHECK(rLCDCON1, 21) ? 1 : 0)
#define LCD_FB_INACTIVE_GET()           (BIT_CHECK(rLCDCON1, 21) ? 0 : 1)

/* Initialise and setup local registers and LCD module. Provide pointers to the framebuffers. */
void LCDInitialise(CsrUint32 *fb0, CsrUint32 *fb1);

/* Clear the specified frame buffer */
void LCDClear(CsrUint32 *fb);

/* Draw a horizontal/vertical line or rectangle, optionally filled */
void LCDDrawBox(CsrUint32 *fb, CsrUint16 left, CsrUint16 top, CsrUint16 right, CsrUint16 bottom, CsrUint32 outlineColour, CsrUint32 fillColour, CsrBool fill);

#ifdef __cplusplus
}
#endif

#endif
