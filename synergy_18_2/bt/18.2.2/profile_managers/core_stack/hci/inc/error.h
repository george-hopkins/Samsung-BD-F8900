#ifndef _ERROR_H_
#define _ERROR_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              error.h

DESCRIPTION:       This file contains the definitions of Bluetooth stack errors.



****************************************************************************/

#ifdef BUILD_FOR_HOST
#include "csr_exceptionhandler.h"
#include "csr_bt_panic.h"
#else
#include "csr_bt_panic.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef CsrUint16 error_code_t;

/* Error masks */
#define ERROR_MASK      ((error_code_t)0x00ff)

#define GENERAL_ERROR   ((error_code_t)0x0000)
#define RFCOMM_ERROR    ((error_code_t)0x0100)
#define L2CAP_ERROR     ((error_code_t)0x0200)
#define DM_ERROR        ((error_code_t)0x0300)
#define HCI_ERROR       ((error_code_t)0x0400)
#define LM_ERROR        ((error_code_t)0x0500)
#define SDP_ERROR       ((error_code_t)0x0600)
#define CS_ERROR        ((error_code_t)0x0700)

/*****************************************
**      GENERAL_ERRORS                   *
******************************************/

#define NO_ERR                ((error_code_t)GENERAL_ERROR | 0x0000)
#define MEMORY_ERR              ((error_code_t)GENERAL_ERROR | 0x0001)
#define BLUESTACK_ERROR_UNKNOWN ((error_code_t)GENERAL_ERROR | 0x0002)


/*****************************************
**      L2CAP_ERRORS                     *
******************************************/

#define CONTROL_ERR     ((error_code_t)L2CAP_ERROR | 0x0001)
#define CON_HANDLE_ERR  ((error_code_t)L2CAP_ERROR | 0x0002)
#define CID_ERR         ((error_code_t)L2CAP_ERROR | 0x0003)
#define SM_ERR          ((error_code_t)L2CAP_ERROR | 0x0004)
#define SIGNAL_ERR      ((error_code_t)L2CAP_ERROR | 0x0005)
#define PHANDLE_ERR     ((error_code_t)L2CAP_ERROR | 0x0006)    /* No phandle for reply */

/*****************************************
**      DM_ERRORS                        *
******************************************/
#define COMMAND_STATUS_ERR      ((error_code_t)DM_ERROR | 0x0001)
#define COMMAND_COMPLETE_ERR    ((error_code_t)DM_ERROR | 0x0002)
#define RM_ERR                  ((error_code_t)DM_ERROR | 0x0003)
#define CMCB_ERR                ((error_code_t)DM_ERROR | 0x0004)
#define DM_HANDLE_INVALID       ((error_code_t)DM_ERROR | 0x0005)
#define NUM_COMPLETED_PACKETS   ((error_code_t)DM_ERROR | 0x0006)
#define DM_PHANDLE_ERR          ((error_code_t)DM_ERROR | 0x0007)       /* No phandle for reply */
#define DM_HCI_COMMAND_FAILURE  ((error_code_t)DM_ERROR | 0x0008)
#define DM_INVALID_PRIMITIVE    ((error_code_t)DM_ERROR | 0x0009)

/*****************************************
**      HCI_ERRORS                       *
******************************************/
#define FILTER_CONDITION_TYPE    ((error_code_t)HCI_ERROR | 0x0001)
#define FILTER_TYPE        ((error_code_t)HCI_ERROR | 0x0002)
#define EVENT_TRANSLATION    ((error_code_t)HCI_ERROR | 0x0003)
#define COMMAND_TRANSLATION    ((error_code_t)HCI_ERROR | 0x0004)
#define COMMAND_COMPLETE_GROUP    ((error_code_t)HCI_ERROR | 0x0005)
#define COMMAND_COMPLETE    ((error_code_t)HCI_ERROR | 0x0006)
#define HCI_SEND_ACL_DATA       ((error_code_t)HCI_ERROR | 0x0007)


/*****************************************
**      LM_ERRORS                        *
******************************************/
#define LM_ERR_NO_ERROR                 ((error_code_t)LM_ERROR | 0x0000)
#define LM_ERR_LC_LMP_FAIL              ((error_code_t)LM_ERROR | 0x0001)
#define LM_ERR_LC_PAGE_SCAN_FAIL        ((error_code_t)LM_ERROR | 0x0002)
#define LM_ERR_LC_END_PAGE_SCAN_FAIL    ((error_code_t)LM_ERROR | 0x0003)
#define LM_ERR_LC_HOLD_FAIL             ((error_code_t)LM_ERROR | 0x0004)
#define LM_ERR_LC_SET_ACL_PACKET_FAIL   ((error_code_t)LM_ERROR | 0x0005)
#define LM_ERR_LC_DESTROY_ACL_FAIL      ((error_code_t)LM_ERROR | 0x0006)
#define LM_ERR_LC_DESTROY_SCO_FAIL      ((error_code_t)LM_ERROR | 0x0007)
#define LM_ERR_NO_INSTANCE_FOUND        ((error_code_t)LM_ERROR | 0x0008)       /* Null pointer for specified instance */
#define LM_ERR_BAD_AM_ADDR              ((error_code_t)LM_ERROR | 0x0009)       /* am_addr exceeds limits */
#define LM_ERR_UNKNOWN_STATE            ((error_code_t)LM_ERROR | 0x000A)       /* State Machine State in error */
#define LM_ERR_PMALLOC_FAILED           ((error_code_t)LM_ERROR | 0x000B)
#define LM_ERR_LC_INQUIRE_SCAN_FAIL     ((error_code_t)LM_ERROR | 0x000C)
#define LM_ERR_LC_INQUIRE_FAIL          ((error_code_t)LM_ERROR | 0x000D)
#define LM_ERR_ACL_LOOKUP_CONFLICT      ((error_code_t)LM_ERROR | 0x000E)       /* ACL Entry we are trying to fill is in use */
#define LM_ERR_LC_ERROR                 ((error_code_t)LM_ERROR | 0x000F)       /* General non-fatal LC error */
#define LM_ERR_LM_FATAL_ERROR           ((error_code_t)LM_ERROR | 0x0010)       /* General fatal LM error */
#define LM_ERR_LC_FATAL_ERROR           ((error_code_t)LM_ERROR | 0x0011)       /* General fatal LC error */
#define LM_ERR_LC_NOT_RECOGNISED_HANDLE ((error_code_t)LM_ERROR | 0x0012)       /* General LC handle error */

/*****************************************
**      SDP_ERRORS                       *
******************************************/

#define SDC_ERR  ((error_code_t)SDP_ERROR | 0x0000)
#define SDS_ERR  ((error_code_t)SDP_ERROR | 0x0001)

/*****************************************
**      CS_ERRORS                        *
******************************************/

#define CS_LINK_FAIL                ((error_code_t)CS_ERROR | 0x0001)
#define DRV_FAIL_CREATE_READ_THD        ((error_code_t)CS_ERROR | 0x0002)
#define DRV_FAIL_CREATE_READ_WRITE_EVENT    ((error_code_t)CS_ERROR | 0x0003)
#define DRV_FAIL_OPEN_DEVICE            ((error_code_t)CS_ERROR | 0x0004)
#define DRV_FAIL_CONFIG_PORT            ((error_code_t)CS_ERROR | 0x0005)
#define DRV_FAIL_MEM                ((error_code_t)CS_ERROR | 0x0006)
#define DRV_FAIL_READ                ((error_code_t)CS_ERROR | 0x0007)
#define DRV_FAIL_WRITE                ((error_code_t)CS_ERROR | 0x0008)
#define DRV_FAIL_SLIP_DECODE            ((error_code_t)CS_ERROR | 0x0009)
#define DRV_SLIP_BUF_OVERFLOW            ((error_code_t)CS_ERROR | 0x000a)

/*****************************************
**      Macro showing source line        *
******************************************/

#ifdef BUILD_FOR_HOST
#define BLUESTACK_WARNING(error_code) \
    CsrGeneralWarning(NULL, (CsrUint16)(error_code), 0, NULL)
#define BLUESTACK_PANIC(panic_code) \
    CsrPanic(CSR_TECH_BT, (CsrUint16)(panic_code), NULL)
#else
#define BLUESTACK_WARNING(error_code)
#define BLUESTACK_PANIC(panic_code) BLUESTACK_PANIC(panic_code)
#endif


/*============================================================================*
Public Data Types
*============================================================================*/

/*============================================================================*
Public Data
*============================================================================*/
/* None */


/*============================================================================*
Public Functions
*============================================================================*/

/*----------------------------------------------------------------------------*
 * NAME
 *     error_handler
 *
 * DESCRIPTION
 *      Handle any unhandled error from anywhere in the stack
 *      If catastrophic is true then stop everything
 *
 * RETURNS/MODIFIES
 *      Depending on catastrophic it may never return
 *
 *----------------------------------------------------------------------------*/
#if defined(ERROR_HANDLER_LINE)
void error_handler_line(const char *file, int line, CsrUint16 error_code,
        CsrBool catastrophic);
#define error_handler(error_code,catastrophic) error_handler_line(__FILE__,__LINE__,error_code,catastrophic)
#else
void error_handler(CsrUint16 error_code, CsrBool catastrophic);
#endif

/*----------------------------------------------------------------------------*
 * NAME
 *     get_last_error
 *
 * DESCRIPTION
 *      Return the last reported error.
 *
 * RETURNS/MODIFIES
 *        file    - Return param - source code file.
 *        line    - Return param - source code line.
 *        error    - Return param - error code.
 *
 *      Returns TRUE if there has been an error!
 *
 *    Note that file/line are only valid if error was reported via error_handler_line.
 *
 *----------------------------------------------------------------------------*/
CsrBool get_last_error(char **file, int *line, CsrUint16 * error);

#ifdef __cplusplus
}
#endif

#endif
