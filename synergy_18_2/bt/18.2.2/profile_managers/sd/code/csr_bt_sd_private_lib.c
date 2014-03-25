/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"
#include "csr_bt_sd_private_lib.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtSdRegisterAvailableServiceReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ message
 *      to the SD message queue.
 *
 *  PARAMETERS
 *      service:  The service a given client wants to search/connect to
 *----------------------------------------------------------------------------*/
void CsrBtSdRegisterAvailableServiceReqSend(CsrBtUuid32 service)
{
    CsrBtSdRegisterAvailableServiceReq *prim = (CsrBtSdRegisterAvailableServiceReq *) CsrPmemAlloc(sizeof(CsrBtSdRegisterAvailableServiceReq));
    prim->type      = CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ;
    prim->service   = service;

    CsrBtSdMsgTransport(prim);
}

