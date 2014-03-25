/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_log_text_2.h"

#include "csr_tftp_file_sef.h"
#include "csr_tftp_file_handler.h"
#include "csr_tftp_file_task.h"

#include "csr_tftp_lib.h"
#include "csr_fsal_lib.h"
#include "csr_sched.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrTftpFileLto);

typedef void (*CsrTftpFileStateHandlerType)(CsrTftpFileInstanceData *instanceData);
static const CsrTftpFileStateHandlerType tftpStateHandlers[CSR_TFTP_PRIM_UPSTREAM_COUNT] =
{
    CsrTftpFileActivateHandler,      /* CSR_TFTP_ACTIVATE_CFM   */
    CsrTftpFileDeactivateHandler,    /* CSR_TFTP_DEACTIVATE_CFM */
    CsrTftpFileConnectHandler,       /* CSR_TFTP_CONNECT_IND    */
    CsrTftpFileReadHandler,          /* CSR_TFTP_READ_IND       */
    CsrTftpFileWriteHandler,         /* CSR_TFTP_WRITE_IND      */
    CsrTftpFileDisconnectHandler,    /* CSR_TFTP_DISCONNECT_IND */
};

void CsrTftpFileInit(void **gash)
{
    CsrTftpFileInstanceData *instanceData;

    instanceData = (CsrTftpFileInstanceData *) CsrPmemAlloc(sizeof(CsrTftpFileInstanceData));
    CsrMemSet(instanceData, 0, sizeof(CsrTftpFileInstanceData));

    *gash = (void *) instanceData;

    CsrFsalSessionCreateReqSend(CSR_TFTP_FILE_IFACEQUEUE);
    CSR_LOG_TEXT_REGISTER(&CsrTftpFileLto, "TFTP_FILE", 0, NULL);
}

void CsrTftpFileHandler(void **gash)
{
    CsrTftpFileInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrTftpFileInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
            case CSR_TFTP_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type - CSR_TFTP_PRIM_UPSTREAM_LOWEST < CSR_TFTP_PRIM_UPSTREAM_COUNT) && ((tftpStateHandlers[*type - CSR_TFTP_PRIM_UPSTREAM_LOWEST]) != NULL))
                {
                    tftpStateHandlers[*type - CSR_TFTP_PRIM_UPSTREAM_LOWEST](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpFileLto, 0, event, *type);
                }

                break;
            }
            case CSR_FSAL_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if (instanceData->state != CSR_TFTP_FILE_STATE_DISCONNECTED)
                {
                    switch (*type)
                    {
                        case CSR_FSAL_SESSION_CREATE_CFM:
                        {
                            CsrTftpFileFsalSessionCreateHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_SESSION_DESTROY_CFM:
                        {
                            CsrTftpFileFsalSessionDestroyHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_OPEN_CFM:
                        {
                            CsrTftpFileFsalOpenHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_CLOSE_CFM:
                        {
                            CsrTftpFileFsalCloseHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_READ_CFM:
                        {
                            CsrTftpFileFsalReadHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_WRITE_CFM:
                        {
                            CsrTftpFileFsalWriteHandler(instanceData);
                            break;
                        }
                        default:
                        {
                            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpFileLto, 0, event, *type);
                            break;
                        }
                    }
                }
                else
                {
                    switch (*type)
                    {
                        case CSR_FSAL_FILE_OPEN_CFM:
                        {
                            CsrTftpFileFsalOpenDisconnectedHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_READ_CFM:
                        {
                            CsrTftpFileFsalReadDisconnectedHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_WRITE_CFM:
                        {
                            CsrTftpFileFsalWriteDisconnectedHandler(instanceData);
                            break;
                        }
                        case CSR_FSAL_FILE_CLOSE_CFM:
                        {
                            CsrTftpFileFsalCloseHandler(instanceData);
                            break;
                        }
                        default:
                        {
                            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpFileLto, 0, event, *type);
                            break;
                        }
                    }
                }

                break;
            }
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpFileLto, 0, event, 0);
                break;
            }
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->msg);
        instanceData->msg = NULL;
    }
    else
    {
        /* Failed to retrieve message from queue */
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrTftpFileDeinit(void **gash)
{
    CsrUint16 event;
    void *message;

    CsrTftpFileInstanceData *instanceData;
    instanceData = (CsrTftpFileInstanceData *) *gash;

    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_TFTP_PRIM:
            {
                CsrTftpFreeUpstreamMessageContents(event, message);
                break;
            }
            case CSR_FSAL_PRIM:
            {
                CsrFsalFreeUpstreamMessageContents(event, message);
                break;
            }
            default:
            {
                break;
            }
        }

        CsrPmemFree(message);
    }

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif /* ENABLE_SHUTDOWN */
