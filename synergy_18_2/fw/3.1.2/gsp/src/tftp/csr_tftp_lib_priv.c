/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_tftp_prim.h"
#include "csr_tftp_lib.h"

void CsrTftpActivateCfmSend(CsrSchedQid queue, CsrResult result)
{
    CsrTftpActivateCfm *prim;

    prim = (CsrTftpActivateCfm *) CsrPmemAlloc(sizeof(CsrTftpActivateCfm));
    prim->type = CSR_TFTP_ACTIVATE_CFM;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}

void CsrTftpDeactivateCfmSend(CsrSchedQid queue)
{
    CsrTftpDeactivateCfm *prim;

    prim = (CsrTftpDeactivateCfm *) CsrPmemAlloc(sizeof(CsrTftpDeactivateCfm));
    prim->type = CSR_TFTP_DEACTIVATE_CFM;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}

void CsrTftpConnectIndSend(CsrSchedQid queue, CsrTftpFamily family, CsrUint8 *ipAddress, CsrUint16 port, CsrTftpOpcode opcode, CsrCharString *fileName)
{
    CsrTftpConnectInd *prim;

    prim = (CsrTftpConnectInd *) CsrPmemAlloc(sizeof(CsrTftpConnectInd));
    prim->type = CSR_TFTP_CONNECT_IND;
    prim->family = family;

    if (prim->family == CSR_TFTP_FAMILY_IP4)
    {
        CsrMemSet(prim->ipAddress, 0, 16);
        CsrMemCpy(prim->ipAddress, ipAddress, 4);
    }
    else
    {
        CsrMemCpy(prim->ipAddress, ipAddress, 16);
    }
    prim->port = port;
    prim->opcode = opcode;
    prim->fileName = fileName;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}

void CsrTftpReadIndSend(CsrSchedQid queue)
{
    CsrTftpReadInd *prim;

    prim = (CsrTftpReadInd *) CsrPmemAlloc(sizeof(CsrTftpReadInd));
    prim->type = CSR_TFTP_READ_IND;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}

void CsrTftpWriteIndSend(CsrSchedQid queue, CsrUint16 dataLength, CsrUint8 *data)
{
    CsrTftpWriteInd *prim;

    prim = (CsrTftpWriteInd *) CsrPmemAlloc(sizeof(CsrTftpWriteInd));
    prim->type = CSR_TFTP_WRITE_IND;
    prim->dataLength = dataLength;
    prim->data = data;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}

void CsrTftpDisconnectIndSend(CsrSchedQid queue)
{
    CsrTftpDisconnectInd *prim;

    prim = (CsrTftpDisconnectInd *) CsrPmemAlloc(sizeof(CsrTftpDisconnectInd));
    prim->type = CSR_TFTP_DISCONNECT_IND;

    CsrSchedMessagePut(queue, CSR_TFTP_PRIM, prim);
}
