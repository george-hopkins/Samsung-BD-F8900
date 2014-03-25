#ifndef CSR_UI_CTRL_H__
#define CSR_UI_CTRL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ui_private_prim.h"
#include "csr_ui_lower.h"
#include "csr_pmem.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrInt16 statusLeft;
    CsrInt16 statusTop;

    CsrInt16 canvasLeft;
    CsrInt16 canvasTop;

    CsrInt16 skLeft;
    CsrInt16 skTop;
    CsrInt16 skRight;
    CsrInt16 skBottom;
    CsrInt16 sk1Right;
    CsrInt16 sk2Left;

    CsrInt16 menuitemStart;

    CsrInt16 dialogBoxTop;
    CsrInt16 dialogBoxLeft;
    CsrInt16 dialogBoxBottom;
    CsrInt16 dialogBoxRight;
    CsrInt16 dialogTextTop;
    CsrInt16 dialogTextLeft;
    CsrInt16 dialogTextBottom;
    CsrInt16 dialogTextRight;

    CsrInt16 inputdialogMessageTop;
    CsrInt16 inputdialogIconLeft;
    CsrInt16 inputdialogIconTop;
    CsrInt16 inputdialogMessageLeft;
    CsrInt16 inputdialogBoxTop;
    CsrInt16 inputdialogBoxLeft;
    CsrInt16 inputdialogBoxBottom;
    CsrInt16 inputdialogBoxRight;
    CsrInt16 inputdialogTextTop;
    CsrInt16 inputdialogTextLeft;
    CsrInt16 inputdialogTextBottom;
    CsrInt16 inputdialogTextRight;
    CsrInt16 inputdialogLines;
    CsrInt16 inputdialogCharacters;

    CsrInt16 idlescreenHeadingTop;
    CsrInt16 idlescreenLogoTop;
    CsrInt16 idlescreenLogoLeft;
    CsrInt16 idlescreenTextTop;
    CsrInt16 idlescreenTextLeft;
} ScreenCoordinates;

/* Instance Data */
typedef struct
{
    void                 *msg;                  /* Received message */
    CsrUint16             HeapSize;              /* Current Size of UIE Heap */
    void                **Heap;                 /* Pointer to first element of UIE Heap array of void pointers */
    void                 *DisplayStack;         /* Pointer to first element of Display Stack linked list */
    CsrUint16             MKkey;                 /* MultiKey key */
    CsrUint8              MKindex;               /* MultiKey sub key */
    CsrSchedTid           MKtid;                        /* MultiKey timer ID */
    CsrBool               MKcaps;                /* MultiKey Caps Lock flag */
    ScreenCoordinates     coor;                 /* Struct with coordinates for the screen on the phone */
    CsrBool               bluetoothActive;       /* Bluetooth Active state */
    CsrBool               wifiActive;            /* Wifi Active state */
    CsrInt8               headset1;              /* State of headset 1 */
    CsrInt8               headset2;              /* State of headset 2 */
    CsrUint8              batteryPercentage;     /* Current status of battery */
    CsrUint8              rssiPercentage;        /* Current rssi status */
    CsrUiTextRenderer     textRenderer;          /* Text Renderer */
    CsrUiGraphicsRenderer graphicsRenderer;      /* Graphics Renderer */
} CsrUiInstanceData;

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_CTRL_H__ */
