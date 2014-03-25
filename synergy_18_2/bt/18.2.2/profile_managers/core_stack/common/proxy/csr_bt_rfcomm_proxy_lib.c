/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_rfc_proxy_lib.c

DESCRIPTION:    RFCOMM access library for building RFCOMM downstream primitives.

REVISION:       $Revision: #1 $

****************************************************************************/

#include "csr_synergy.h"


/*=============================================================================*
    ANSI C & System-wide Header Files
 *============================================================================*/

#include "csr_types.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "csr_bt_tasks.h"

/*============================================================================*
    Interface Header Files
 *============================================================================*/
#include "rfcomm_prim.h"
#include "csr_mblk.h"
#include "rfcommlib.h"

/*============================================================================*
    Local Header File
 *============================================================================*/
#include "csr_bt_rfc_proxy_lib.h"

/*============================================================================*
    Public Data
 *============================================================================*/
/* None */

/*============================================================================*
    Private Defines
 *============================================================================*/

/*============================================================================*
    Private Data Types
 *============================================================================*/
/* None */

/*============================================================================*
    Private Function Prototypes
 *============================================================================*/
/* None */

/*============================================================================*
    Private Data
 *============================================================================*/
/* None */

/*============================================================================*
    Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcRegisterReqSend
 *
 *  DESCRIPTION
 *      Build and send a RFC_REGISTER_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcRegisterReqSend(
    CsrSchedQid phandle,
    CsrUint8   flags,
    CsrUint16  context,
    CsrUint8   loc_serv_chan_req
    )
{
    rfc_register_req(phandle, flags, context, loc_serv_chan_req);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcUnregisterReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_UNREGISTER_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcUnregisterReqSend(CsrUint8 loc_serv_chan)
{
    rfc_unregister_req(loc_serv_chan);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcClientConnectReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_CLIENT_CONNECT_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcClientConnectReqSend(
    CsrSchedQid   phandle,
    BD_ADDR_T   *p_bd_addr,
    CsrUint8     rem_serv_chan,
    CsrUint8     flags,
    CsrUint16    context,
    CsrUint16    client_security_chan,
    CsrUint16    max_payload_size,
    CsrUint8     priority,
    CsrUint16    total_credits,
    l2ca_controller_t   remote_l2cap_control,
    l2ca_controller_t   local_l2cap_control,
    CsrUint16    p_reserved_length,
    CsrUint16    *p_reserved,
    CsrUint8     modem_signal,
    CsrUint8     break_signal,
    CsrUint16    msc_timeout)
{
    rfc_client_connect_req(phandle,
                           p_bd_addr,
                           rem_serv_chan,
                           flags,
                           context, 
                           client_security_chan, 
                           max_payload_size,
                           priority, 
                           total_credits,
                           remote_l2cap_control, 
                           local_l2cap_control, 
                           p_reserved_length, 
                           p_reserved,
                           modem_signal,
                           break_signal,
                           msc_timeout);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcServerConnectResSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_SERVER_CONNECT_RSP primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcServerConnectResSend(
        CsrUint8     flags,
        CsrUint16    conn_id,
        RFC_RESPONSE_T   response,
        CsrUint16    max_payload_size,
        CsrUint8     priority,
        CsrUint16    total_credits,
        l2ca_controller_t   remote_l2cap_control,
        l2ca_controller_t   local_l2cap_control,
        CsrUint8     modem_signal,
        CsrUint8     break_signal,
        CsrUint16    msc_timeout)
{
    rfc_server_connect_rsp(flags,
                           conn_id,
                           response,
                           max_payload_size,
                           priority, 
                           total_credits, 
                           remote_l2cap_control, 
                           local_l2cap_control,
                           modem_signal,
                           break_signal,
                           msc_timeout);
}
/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcDisconnectReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_RELEASE_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcDisconnectReqSend(
    CsrUint16 connId
    )
{
   rfc_disconnect_req(connId);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcPortnegReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_PORTNEG_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcPortnegReqSend(
    CsrUint16 connId,
    CsrBool request,
    RFC_PORTNEG_VALUES_T *p_port_pars     /* Can be NULL */
    )
{
    rfc_portneg_req(connId, request, p_port_pars);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcPortnegResSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_PORTNEG_RES primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcPortnegResSend(
    CsrUint16 connId,
    RFC_PORTNEG_VALUES_T *p_port_pars     /* Must be supplied */
    )
{
    rfc_portneg_rsp(connId, p_port_pars);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcModemStatusReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_MODEM_STATUS_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcModemStatusReqSend(
    CsrUint16 connId,
    CsrUint8 modem_signal,
    CsrUint8 break_signal
    )
{
    rfc_modem_status_req(connId, modem_signal, break_signal);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcFcReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_FCON_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcFcReqSend(BD_ADDR_T *p_bd_addr, RFC_FC_T  fc)

{
    rfc_fc_req(p_bd_addr,fc);
}



/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcDatawriteReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_DATAWRITE_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcDatawriteReqSend(
    CsrUint16 connId,
    CsrUint16 rx_credits,
    CsrUint16 payload_length,
    CsrUint8 *payload
    )
{
    CsrMblk *mblk;

    if (payload_length == 0)
    {
        /* Credit update - no need to malloc mblk */
        mblk = NULL;
    }
    else
    {
        /* Allocate new mblk. No need to pmalloc space for data - we pass on the
         * already allocated data from the higher layer. */
        mblk = CsrMblkDataCreate(payload, payload_length, TRUE);
    }

   rfc_datawrite_req(connId,payload_length, mblk, rx_credits);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcDatareadRspSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_DATAREAD_RSP primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcDatareadRspSend(CsrUint16 conn_id)
{
    rfc_dataread_rsp(conn_id);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcLinestatusReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_LINE_STATUS_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcLinestatusReqSend(CsrUint16 conn_id,
                        CsrUint8 error_flag,
                        CsrUint8 line_status)
{
    rfc_linestatus_req(conn_id, error_flag, line_status);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcTestReqSend
 *
 *  DESCRIPTION
 *      Build and send an RFC_TEST_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void RfcTestReqSend(BD_ADDR_T  *p_bd_addr,
                  CsrUint16 test_data_length,
                  CsrUint8 *test_data)
{
    CsrMblk *mblk;

    if (test_data_length == 0)
    {
        /* Credit update - no need to malloc mblk */
        mblk = NULL;
    }
    else
    {
        /* Allocate new mblk. No need to pmalloc space for data - we pass on the
         * already allocated data from the higher layer. */
        mblk = CsrMblkDataCreate(test_data, test_data_length, TRUE);
    }
    rfc_test_req(p_bd_addr,test_data_length,mblk);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      RfcInitReqSend
 *
 *  DESCRIPTION
 *      Build and send a RFC_INIT_REQ primitive to RFCOMM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void RfcInitReqSend( CsrSchedQid phandle )
{
    rfc_init_req(phandle);
}

/*============================================================================*
    Private Function Implementations
 *============================================================================*/
/* None */

/*============================================================================*
    End Of File
 *============================================================================*/
