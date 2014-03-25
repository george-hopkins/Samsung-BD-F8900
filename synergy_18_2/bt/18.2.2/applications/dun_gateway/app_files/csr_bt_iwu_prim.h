#ifndef CSR_BT_IWU_PRIM_H__
#define CSR_BT_IWU_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


/*============================================================================*
  Header files on which this one depends
  *============================================================================*/
#include "csr_types.h"
#include "csr_bt_profiles.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CSR_BT_IWU_DG_UART1_SUCCESS       ((CsrUint8) (0x00))

/*============================================================================*
  Interface #defined parameters
 *===========================================================================*/

/* typedefs */
typedef CsrPrim              iwuPrim_t;
#define IWU_RECV_PRIM_BASE        0x0000
#define IWU_SEND_PRIM_BASE        CSR_PRIM_UPSTREAM
/*-------------------------------------------------------------*/
/* Primitive definitions */
/*-------------------------------------------------------------*/

/* Define the primitive numbers */

typedef enum iwu_prim_tag
{
    ENUM_IWU_MODEM_STATUS_REQ = IWU_RECV_PRIM_BASE,
    ENUM_IWU_MODEM_DATA_REQ,
    ENUM_IWU_ACTIVATE_REQ,
    ENUM_IWU_DEACTIVATE_REQ,
    ENUM_IWU_RELEASE_REQ,
    ENUM_IWU_MODEM_RESET_REQ,
    ENUM_IWU_MODEM_READY_REQ,
    ENUM_IWU_DATA_PATH_STATUS_IND,
    ENUM_IWU_UPDATE_DUNGW_STATE_REQ,
    ENUM_HIGHEST_IWU_RECV_PRIM_NUM,

    ENUM_HIGHEST_IWU_SEND_PRIM_NUM
}
IWU_PRIM_T;

#define NUM_OF_IWU_RECV_PRIMS        (ENUM_HIGHEST_IWU_RECV_PRIM_NUM - IWU_RECV_PRIM_BASE)
#define NUM_OF_IWU_SEND_PRIMS        (ENUM_HIGHEST_IWU_SEND_PRIM_NUM - IWU_SEND_PRIM_BASE)

#define IWU_MODEM_STATUS_REQ         ((iwuPrim_t)ENUM_IWU_MODEM_STATUS_REQ)
#define IWU_MODEM_DATA_REQ           ((iwuPrim_t)ENUM_IWU_MODEM_DATA_REQ)
#define IWU_ACTIVATE_REQ             ((iwuPrim_t)ENUM_IWU_ACTIVATE_REQ)
#define IWU_DEACTIVATE_REQ           ((iwuPrim_t)ENUM_IWU_DEACTIVATE_REQ)
#define IWU_RELEASE_REQ              ((iwuPrim_t)ENUM_IWU_RELEASE_REQ)
#define IWU_MODEM_RESET_REQ          ((iwuPrim_t)ENUM_IWU_MODEM_RESET_REQ)
#define IWU_MODEM_READY_REQ          ((iwuPrim_t)ENUM_IWU_MODEM_READY_REQ)
#define IWU_DATA_PATH_STATUS_IND     ((iwuPrim_t)ENUM_IWU_DATA_PATH_STATUS_IND)
#define IWU_UPDATE_DUNGW_STATE_REQ   ((iwuPrim_t)ENUM_IWU_UPDATE_DUNGW_STATE_REQ)

typedef struct
{
    iwuPrim_t      type;                   /* primitive/message identity */
    CsrUint8        modemStatusFlag;        /* Holds the Modem status flag(DCD DSR CTS RI)*/
} IWU_MODEM_STATUS_REQ_T;

typedef struct
{
    iwuPrim_t     type;                    /* primitive/message identity */
    CsrUint16      payloadLength;           /* number of data bytes in data area */
    CsrUint8       *modemPayload;           /* pointer to allocated data */
    CsrUint8        uart1ErrCodes;          /* Holds error codes som UART 1 */
} IWU_MODEM_DATA_REQ_T;

typedef struct
{
    iwuPrim_t     type;
} IWU_ACTIVATE_REQ_T;

typedef struct
{
    iwuPrim_t     type;
} IWU_DEACTIVATE_REQ_T;

typedef struct
{
    iwuPrim_t     type;
} IWU_RELEASE_REQ_T;

typedef struct
{
    iwuPrim_t     type;
} IWU_MODEM_RESET_REQ_T;

typedef struct
{
    iwuPrim_t     type;
    CsrUint8       result;
} IWU_MODEM_READY_REQ_T;

typedef struct
{
    iwuPrim_t     type;                 /* primitive/message identity */
    CsrSchedQid     dgInstanceQueue;
    CsrUint8       status;               /* new status (open/close/lost) */
} IWU_DATA_PATH_STATUS_IND_T;

typedef struct
{
    iwuPrim_t     type;
    CsrUint8       newState;
}IWU_UPDATE_DUNGW_STATE_REQ_T;

#ifdef __cplusplus
}
#endif

#endif  /* _IWU_PRIM_H*/

