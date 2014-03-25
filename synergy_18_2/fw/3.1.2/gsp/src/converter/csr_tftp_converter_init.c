/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_tftp_converter_init.h"
#ifndef EXCLUDE_CSR_TFTP_MODULE
#include "csr_tftp_serialize.h"
#include "csr_tftp_prim.h"

static CsrMsgConvMsgEntry csr_tftp_conv_lut[] = {
#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_CFM
    { CSR_TFTP_ACTIVATE_CFM, CsrTftpActivateCfmSizeof, CsrTftpActivateCfmSer, CsrTftpActivateCfmDes, CsrTftpActivateCfmSerFree },
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_REQ
    { CSR_TFTP_ACTIVATE_REQ, CsrTftpActivateReqSizeof, CsrTftpActivateReqSer, CsrTftpActivateReqDes, CsrTftpActivateReqSerFree },
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
    { CSR_TFTP_CONNECT_IND, CsrTftpConnectIndSizeof, CsrTftpConnectIndSer, CsrTftpConnectIndDes, CsrTftpConnectIndSerFree },
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */
#ifndef EXCLUDE_CSR_TFTP_CONNECT_RES
    { CSR_TFTP_CONNECT_RES, CsrTftpConnectResSizeof, CsrTftpConnectResSer, CsrTftpConnectResDes, CsrTftpConnectResSerFree },
#endif /* EXCLUDE_CSR_TFTP_CONNECT_RES */
#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_CFM
    { CSR_TFTP_DEACTIVATE_CFM, CsrTftpDeactivateCfmSizeof, CsrTftpDeactivateCfmSer, CsrTftpDeactivateCfmDes, CsrTftpDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_REQ
    { CSR_TFTP_DEACTIVATE_REQ, CsrTftpDeactivateReqSizeof, CsrTftpDeactivateReqSer, CsrTftpDeactivateReqDes, CsrTftpDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_TFTP_DISCONNECT_IND
    { CSR_TFTP_DISCONNECT_IND, CsrTftpDisconnectIndSizeof, CsrTftpDisconnectIndSer, CsrTftpDisconnectIndDes, CsrTftpDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_TFTP_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_TFTP_READ_IND
    { CSR_TFTP_READ_IND, CsrTftpReadIndSizeof, CsrTftpReadIndSer, CsrTftpReadIndDes, CsrTftpReadIndSerFree },
#endif /* EXCLUDE_CSR_TFTP_READ_IND */
#ifndef EXCLUDE_CSR_TFTP_READ_RES
    { CSR_TFTP_READ_RES, CsrTftpReadResSizeof, CsrTftpReadResSer, CsrTftpReadResDes, CsrTftpReadResSerFree },
#endif /* EXCLUDE_CSR_TFTP_READ_RES */
#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
    { CSR_TFTP_WRITE_IND, CsrTftpWriteIndSizeof, CsrTftpWriteIndSer, CsrTftpWriteIndDes, CsrTftpWriteIndSerFree },
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */
#ifndef EXCLUDE_CSR_TFTP_WRITE_RES
    { CSR_TFTP_WRITE_RES, CsrTftpWriteResSizeof, CsrTftpWriteResSer, CsrTftpWriteResDes, CsrTftpWriteResSerFree },
#endif /* EXCLUDE_CSR_TFTP_WRITE_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrTftpConverterInit(void)
{
    CsrMsgConvInsert(CSR_TFTP_PRIM, csr_tftp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_tftp_conv_info = {
    CSR_TFTP_PRIM,
    "CSR_TFTP_PRIM",
    csr_tftp_conv_lut
};

CsrLogPrimitiveInformation* CsrTftpTechInfoGet(void)
{
    return &csr_tftp_conv_info;
}
#endif
#endif

