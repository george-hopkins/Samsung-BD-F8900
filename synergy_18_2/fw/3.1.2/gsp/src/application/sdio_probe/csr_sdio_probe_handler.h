#ifndef CSR_SDIO_PROBE_HANDLER_H__
#define CSR_SDIO_PROBE_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_sdio.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrSdioProbeLto);

#define CSR_SDIO_PROBE_MENU_CATEGORY "SDIO"
#define CSR_SDIO_PROBE_MENU_ITEM_NAME "Probe"

#define CSR_SDIO_PROBE_MAX_FUNCTIONS 8

typedef CsrUint8 CsrSdioProbeState;
#define CSR_SDIO_PROBE_STATE_OFF        ((CsrSdioProbeState) 0)
#define CSR_SDIO_PROBE_STATE_ON         ((CsrSdioProbeState) 1)

typedef CsrUint8 CsrSdioProbeTestType;
#define CSR_SDIO_PROBE_TEST_TYPE_NONE   ((CsrSdioProbeTestType) 0)
#define CSR_SDIO_PROBE_TEST_TYPE_ASYNC  ((CsrSdioProbeTestType) 1)
#define CSR_SDIO_PROBE_TEST_TYPE_SYNC   ((CsrSdioProbeTestType) 2)
#define CSR_SDIO_PROBE_TEST_TYPE_INT    ((CsrSdioProbeTestType) 3)

typedef CsrUint8 CsrSdioProbeChipType;
#define CSR_SDIO_PROBE_CHIP_TYPE_UNKNOWN    ((CsrSdioProbeChipType) 0)
#define CSR_SDIO_PROBE_CHIP_TYPE_WIFI       ((CsrSdioProbeChipType) 1)
#define CSR_SDIO_PROBE_CHIP_TYPE_BC6        ((CsrSdioProbeChipType) 2)
#define CSR_SDIO_PROBE_CHIP_TYPE_BC7        ((CsrSdioProbeChipType) 3)

typedef struct
{
    void                *instanceData;
    CsrUint16            key;
    CsrSdioProbeTestType testType;
    CsrSdioProbeChipType chipType;
    CsrUint32            wakeupInterruptClearAddress;
    CsrUint32            scratchRegister;
    CsrUint8             testState;
    CsrSchedTid          timeout;
    CsrSchedBgint        bgint;
    CsrUint32            dataLength;
    void                *data;
    CsrBool              dataVerify;
    CsrUint8             data8;
    CsrResult            result;
    CsrBool              spurious;
    CsrBool              stop;

    /* Statistics */
    CsrTime   startTime;
    CsrUint32 byteOperations;
    CsrTime   byteOperationsTime;
    CsrUint32 dataOperations;
    CsrTime   dataOperationsTime;
    CsrUint32 interrupts;
    CsrTime   interruptsTime;
} CsrSdioProbeTestData;

typedef struct
{
    CsrSdioProbeState state;        /* Task state */
    void             *message;      /* Last message received */

    /* Various information */
    CsrUint16 controlId;            /* The controlId associated with CSR_APP */
    CsrUint16 testMenuKey;          /* Holds the device key associated with the test menu */

    /* List of functions and associated test data */
    CsrUint32            attachedCount;
    CsrSdioFunction     *functions[CSR_SDIO_PROBE_MAX_FUNCTIONS];
    CsrSdioProbeTestData testData[CSR_SDIO_PROBE_MAX_FUNCTIONS];

    CsrUint16 testsRunning;

    /* UI */
    CsrUieHandle hSelectEvent;      /* Event triggered when selecting a menu item */
    CsrUieHandle hBackEvent;        /* Event that will hide the currently shown UIE */
    CsrUieHandle hMenu;             /* The SDIO Probe main menu */
    CsrUieHandle hTestMenu;         /* The test menu */
    CsrUieHandle hPopup;            /* Popup dialog for notifications */
} CsrSdioProbeInstanceData;

#ifdef __cplusplus
}
#endif

#endif
