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

#include "csr_tftp_prim.h"
#include "csr_tftp_lib.h"
#include "csr_tftp_sef.h"
#include "csr_tftp_handler.h"
#include "csr_tftp_task.h"

#include "csr_ip_socket_lib.h"
#include "csr_sched.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrTftpLto);

typedef void (*CsrTftpStateHandlerType)(CsrTftpInstanceData *instanceData);
static const CsrTftpStateHandlerType appStateHandlers[CSR_TFTP_PRIM_DOWNSTREAM_COUNT] =
{
    CsrTftpActivateHandler,      /* CSR_TFTP_ACTIVATE_REQ   */
    CsrTftpDeactivateHandler,    /* CSR_TFTP_DEACTIVATE_REQ */
    CsrTftpConnectHandler,       /* CSR_TFTP_CONNECT_RES    */
    CsrTftpReadHandler,          /* CSR_TFTP_READ_RES       */
    CsrTftpWriteHandler,         /* CSR_TFTP_WRITE_RES      */
};

void CsrTftpInit(void **gash)
{
    CsrTftpInstanceData *instanceData;

    instanceData = (CsrTftpInstanceData *) CsrPmemAlloc(sizeof(CsrTftpInstanceData));
    CsrMemSet(instanceData, 0, sizeof(CsrTftpInstanceData));

    instanceData->state = CSR_TFTP_STATE_IDLE;
    instanceData->qid = CSR_SCHED_QID_INVALID;
    instanceData->client.timer = CSR_SCHED_TID_INVALID;

    CsrTftpConnectionInit(&instanceData->conn, CSR_TFTP_PORT);
    CsrTftpConnectionClientInit(&instanceData->client);

    *gash = (void *) instanceData;

    CSR_LOG_TEXT_REGISTER(&CsrTftpLto, "TFTP", 0, NULL);
}

void CsrTftpHandler(void **gash)
{
    CsrTftpInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrTftpInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
            case CSR_TFTP_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type < CSR_TFTP_PRIM_DOWNSTREAM_COUNT) && ((appStateHandlers[*type]) != NULL))
                {
                    appStateHandlers[*type](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpLto, 0, event, *type);
                }

                break;
            }
            case CSR_IP_SOCKET_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if (instanceData->state == CSR_TFTP_STATE_DEACTIVATING)
                {
                    switch (*type)
                    {
                        case CSR_IP_SOCKET_UDP_NEW_CFM:
                        {
                            CsrTftpIpSocketUdpNewHandlerDeactivating(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_BIND_CFM:
                        {
                            CsrTftpIpSocketUdpBindHandlerDeactivating(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_DATA_IND:
                        {
                            CsrTftpIpSocketUdpDataIndHandlerDeactivating(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_DATA_CFM:
                        {
                            CsrTftpIpSocketUdpDataCfmHandlerDeactivating(instanceData);
                            break;
                        }
                        default:
                        {
                            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpLto, 0, event, *type);
                            break;
                        }
                    }
                }
                else
                {
                    switch (*type)
                    {
                        case CSR_IP_SOCKET_UDP_NEW_CFM:
                        {
                            CsrTftpIpSocketUdpNewHandler(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_BIND_CFM:
                        {
                            CsrTftpIpSocketUdpBindHandler(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_DATA_IND:
                        {
                            CsrTftpIpSocketUdpDataIndHandler(instanceData);
                            break;
                        }
                        case CSR_IP_SOCKET_UDP_DATA_CFM:
                        {
                            CsrTftpIpSocketUdpDataCfmHandler(instanceData);
                            break;
                        }
                        default:
                        {
                            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpLto, 0, event, *type);
                            break;
                        }
                    }
                }

                break;
            }
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTftpLto, 0, event, 0);
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
void CsrTftpDeinit(void **gash)
{
    CsrUint16 event;
    void *message;

    CsrTftpInstanceData *instanceData;
    instanceData = (CsrTftpInstanceData *) *gash;

    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_TFTP_PRIM:
            {
                CsrTftpFreeDownstreamMessageContents(event, message);
                break;
            }
            case CSR_IP_SOCKET_PRIM:
            {
                CsrIpSocketFreeUpstreamMessageContents(event, message);
                break;
            }
            default:
            {
                break;
            }
        }

        CsrPmemFree(message);
    }

    CsrTftpConnectionInit(&instanceData->conn, CSR_TFTP_PORT);
    CsrTftpConnectionClientInit(&instanceData->client);

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif /* ENABLE_SHUTDOWN */
