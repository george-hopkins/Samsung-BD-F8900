#ifndef CSR_BT_DUNC_MAIN_H__
#define CSR_BT_DUNC_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_cmn_sdc_rfc_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dunc states */

#define DUNC_DISCONNECTING_STATE            0x00
#define DUNC_CONNECTED_STATE                0x01
#define DUNC_CONNECT_STATE                  0x02
#define DUNC_IDLE_STATE                     0x03

#define DUNC_NUMBER_OF_DOWNSTREAM_STATES    0x04
#define DUNC_NUMBER_OF_UPSTREAM_STATES      0x02


typedef CsrUint8                         DuncStatesType;

/* dunc instance data */
typedef struct
{
    CsrSchedQid               ctrlHandle;              /* application protocol handle for the control part */
    CsrSchedQid               dataHandle;              /* application protocol handle for the data part */
    BD_ADDR_T               bdAddr;                  /* the bluetooth address of the device to connect to */
    DuncStatesType          state;                   /* the state of the profile */
    CsrUint8           serverChannel;           /* Local server channel */
    CsrUint16                rfcMaxPacketSize;        /* CSRMAX packet size for Rfcomm connection */
    CsrUint32                btConnId;                   /* the mux id for the established connection */
    void                    *recvMsgP;
    CsrBool                  doingCleanup;            /* a cleanup of a specific apphandle is being performed */
    CsrBool                  lowPower;                /* is the device running as low power device */
    dm_security_level_t     secOutgoing;             /* Outgoing security level */
    void                    *connectInst;
} DuncInstanceDataType;

typedef void (* DuncStateHandlerType)(DuncInstanceDataType * duncInstanceData);

/* Prototypes from dunc_free_down.c */
void CsrBtDuncFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);
#ifdef __cplusplus
}
#endif

#endif /* _DUNC_HANDLER_H_ */
