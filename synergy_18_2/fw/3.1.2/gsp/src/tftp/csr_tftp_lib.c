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

CsrTftpActivateReq *CsrTftpActivateReq_struct(CsrSchedQid qid, CsrTftpFamily family, CsrUint8 ipAddress[16])
{
    CsrTftpActivateReq *prim;

    prim = (CsrTftpActivateReq *) CsrPmemAlloc(sizeof(CsrTftpActivateReq));
    prim->type = CSR_TFTP_ACTIVATE_REQ;
    prim->qid = qid;
    prim->family = family;
    CsrMemCpy(prim->ipAddress, ipAddress, 16);

    return prim;
}

CsrTftpDeactivateReq *CsrTftpDeactivateReq_struct(CsrSchedQid qid)
{
    CsrTftpDeactivateReq *prim;

    prim = (CsrTftpDeactivateReq *) CsrPmemAlloc(sizeof(CsrTftpDeactivateReq));
    prim->type = CSR_TFTP_DEACTIVATE_REQ;
    prim->qid = qid;

    return prim;
}

CsrTftpConnectRes *CsrTftpConnectRes_struct(CsrResult result)
{
    CsrTftpConnectRes *prim;

    prim = (CsrTftpConnectRes *) CsrPmemAlloc(sizeof(CsrTftpConnectRes));
    prim->type = CSR_TFTP_CONNECT_RES;
    prim->result = result;

    return prim;
}

CsrTftpReadRes *CsrTftpReadRes_struct(CsrUint16 dataLength, CsrUint8 *data, CsrResult result)
{
    CsrTftpReadRes *prim;

    prim = (CsrTftpReadRes *) CsrPmemAlloc(sizeof(CsrTftpReadRes));
    prim->type = CSR_TFTP_READ_RES;
    prim->dataLength = dataLength;
    prim->data = data;
    prim->result = result;

    return prim;
}

CsrTftpWriteRes *CsrTftpWriteRes_struct(CsrResult result)
{
    CsrTftpWriteRes *prim;

    prim = (CsrTftpWriteRes *) CsrPmemAlloc(sizeof(CsrTftpWriteRes));
    prim->type = CSR_TFTP_WRITE_RES;
    prim->result = result;

    return prim;
}
