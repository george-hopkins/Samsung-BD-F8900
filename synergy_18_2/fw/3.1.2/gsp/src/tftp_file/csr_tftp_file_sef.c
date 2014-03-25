/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_tftp_file_handler.h"
#include "csr_tftp_file_sef.h"
#include "csr_tftp_file_task.h"

#include "csr_tftp_lib.h"
#include "csr_fsal_lib.h"

#include "csr_log_text_2.h"

void CsrTftpFileActivateHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrTftpActivateCfm *prim = (CsrTftpActivateCfm *) instanceData->msg;

    if (prim->result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrTftpFileLto, 0, "Not able to activate TFTP"));
        CsrFsalSessionDestroyReqSend(instanceData->sessionId);
    }
}

void CsrTftpFileDeactivateHandler(CsrTftpFileInstanceData *instanceData)
{
    /* Ignore as CSR_TFTP_DEACTIVATE_REQ is never sent */
}

void CsrTftpFileConnectHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileOpenFlags flags;
    CsrFsalFileOpenPerms perms;
    CsrTftpConnectInd *prim = (CsrTftpConnectInd *) instanceData->msg;

    if (instanceData->state == CSR_TFTP_FILE_STATE_IDLE)
    {
        instanceData->state = CSR_TFTP_FILE_STATE_OPEN;
        perms = CSR_FSAL_OPEN_PERMS_READ | CSR_FSAL_OPEN_PERMS_WRITE;

        if (prim->opcode == CSR_TFTP_OPCODE_READ)
        {
            flags = CSR_FSAL_OPEN_FLAGS_READ_ONLY;
        }
        else
        {
            flags = CSR_FSAL_OPEN_FLAGS_CREATE | CSR_FSAL_OPEN_FLAGS_WRITE_ONLY | CSR_FSAL_OPEN_FLAGS_TRUNCATE;
        }

        CsrFsalFileOpenReqSend(instanceData->sessionId, (CsrUtf8String *) prim->fileName, flags, perms);
    }
    else
    {
        CsrTftpConnectResSend(CSR_RESULT_FAILURE);
    }
}

void CsrTftpFileReadHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileReadReqSend(instanceData->handle, CSR_TFTP_FILE_DEFAULT_BLKSIZE);
    instanceData->state = CSR_TFTP_FILE_STATE_FILE_ACCESS;
}

void CsrTftpFileWriteHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrTftpWriteInd *prim = (CsrTftpWriteInd *) instanceData->msg;
    CsrFsalFileWriteReqSend(instanceData->handle, prim->dataLength, prim->data);
    instanceData->state = CSR_TFTP_FILE_STATE_FILE_ACCESS;
}

void CsrTftpFileDisconnectHandler(CsrTftpFileInstanceData *instanceData)
{
    switch (instanceData->state)
    {
        case CSR_TFTP_FILE_STATE_ACTIVE:
        {
            CsrFsalFileCloseReqSend(instanceData->handle);
        }
        /* FALLTHROUGH */
        case CSR_TFTP_FILE_STATE_OPEN:
        case CSR_TFTP_FILE_STATE_FILE_ACCESS:
        { /* Wait for a FSAL signal */
            instanceData->state = CSR_TFTP_FILE_STATE_DISCONNECTED;
            break;
        }
        case CSR_TFTP_FILE_STATE_IDLE:
        default:
        {
            CSR_LOG_TEXT_ERROR((CsrTftpFileLto, 0, "Received TFTP disconnect in unknown state 0x%X", instanceData->state));
            break;
        }
    }
}

void CsrTftpFileFsalSessionCreateHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalSessionCreateCfm *prim = (CsrFsalSessionCreateCfm *) instanceData->msg;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        CsrUint8 ip[16];
        CsrMemSet(ip, 0, 16);
        instanceData->sessionId = prim->sessionId;

        CsrTftpActivateReqSend(CSR_TFTP_FILE_IFACEQUEUE, CSR_TFTP_FAMILY_IP4, ip);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrTftpFileLto, 0, "Not able to create FSAL session"));
    }

    CsrPmemFree(prim->currentWorkingDir);
}

void CsrTftpFileFsalSessionDestroyHandler(CsrTftpFileInstanceData *instanceData)
{
    /* Ignore */
}

void CsrTftpFileFsalOpenHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileOpenCfm *prim = (CsrFsalFileOpenCfm *) instanceData->msg;
    CsrUint8 state = CSR_TFTP_FILE_STATE_ACTIVE;
    CsrResult result = CSR_RESULT_SUCCESS;

    if (prim->result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_DEBUG((CsrTftpFileLto, 0, "Not able to open file"));

        state = CSR_TFTP_FILE_STATE_IDLE;
        result = CSR_RESULT_FAILURE;
    }

    instanceData->handle = prim->handle;
    instanceData->state = state;

    CsrTftpConnectResSend(result);
}

void CsrTftpFileFsalCloseHandler(CsrTftpFileInstanceData *instanceData)
{
    instanceData->state = CSR_TFTP_FILE_STATE_IDLE;
}

void CsrTftpFileFsalReadHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileReadCfm *prim = (CsrFsalFileReadCfm *) instanceData->msg;

    if ((prim->result == CSR_RESULT_SUCCESS) || (prim->result == CSR_FSAL_FILE_OP_EOF))
    {
        CsrTftpReadResSend((CsrUint16) prim->dataLen, prim->data, CSR_RESULT_SUCCESS);

        if (prim->dataLen < CSR_TFTP_FILE_DEFAULT_BLKSIZE)
        {
            CsrFsalFileCloseReqSend(instanceData->handle);
            instanceData->state = CSR_TFTP_FILE_STATE_FILE_ACCESS;
        }
        else
        {
            instanceData->state = CSR_TFTP_FILE_STATE_ACTIVE;
        }
    }
    else
    {
        CsrTftpReadResSend(0, NULL, CSR_RESULT_FAILURE);
        CsrFsalFileCloseReqSend(instanceData->handle);
        CsrPmemFree(prim->data);
        prim->data = NULL;
    }
}

void CsrTftpFileFsalWriteHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileWriteCfm *prim = (CsrFsalFileWriteCfm *) instanceData->msg;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        CsrTftpWriteResSend(CSR_RESULT_SUCCESS);

        if (prim->bytesWritten < CSR_TFTP_FILE_DEFAULT_BLKSIZE)
        {
            CsrFsalFileCloseReqSend(instanceData->handle);
            instanceData->state = CSR_TFTP_FILE_STATE_FILE_ACCESS;
        }
        else
        {
            instanceData->state = CSR_TFTP_FILE_STATE_ACTIVE;
        }
    }
    else
    {
        CsrTftpWriteResSend(CSR_RESULT_FAILURE);
        CsrFsalFileCloseReqSend(instanceData->handle);
    }
}

void CsrTftpFileFsalOpenDisconnectedHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileOpenCfm *prim = (CsrFsalFileOpenCfm *) instanceData->msg;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        CsrFsalFileCloseReqSend(instanceData->handle);
    }
    else
    {
        instanceData->state = CSR_TFTP_FILE_STATE_IDLE;
    }
}

void CsrTftpFileFsalReadDisconnectedHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileReadCfm *prim = (CsrFsalFileReadCfm *) instanceData->msg;
    CsrPmemFree(prim->data);
    prim->data = NULL;

    CsrFsalFileCloseReqSend(instanceData->handle);
}

void CsrTftpFileFsalWriteDisconnectedHandler(CsrTftpFileInstanceData *instanceData)
{
    CsrFsalFileCloseReqSend(instanceData->handle);
}
