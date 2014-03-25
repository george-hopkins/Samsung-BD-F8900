/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifndef EXCLUDE_CSR_DSPM_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_prim.h"

void CsrDspmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_DSPM_PRIM)
    {
        CsrPrim *prim = (CsrPrim *) message;
        switch (*prim)
        {
            case CSR_DSPM_OPERATOR_CREATE_REQ:
            {
                CsrDspmOperatorCreateReq *p = message;
                CsrPmemFree(p->operatorInfo);
                p->operatorInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_STOP_REQ:
            {
                CsrDspmOperatorStopReq *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CP_CLOSE_REQ:
            {
                CsrDspmStreamCpCloseReq *p = message;
                CsrPmemFree(p->cpid);
                p->cpid = NULL;
                break;
            }
            case CSR_DSPM_CONNECTION_DESTROY_REQ:
            {
                CsrDspmConnectionDestroyReq *p = message;
                CsrPmemFree(p->cid);
                p->cid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_DESTROY_REQ:
            {
                CsrDspmOperatorDestroyReq *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_CAPABILITY_DOWNLOAD_REQ:
            {
                CsrDspmCapabilityDownloadReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_RESET_REQ:
            {
                CsrDspmOperatorResetReq *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_STREAM_SYNC_REQ:
            {
                CsrDspmStreamSyncReq *p = message;
                CsrPmemFree(p->syncInfo);
                p->syncInfo = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CONFIGURE_REQ:
            {
                CsrDspmStreamConfigureReq *p = message;
                CsrPmemFree(p->configInfo);
                p->configInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_MESSAGE_REQ:
            {
                CsrDspmOperatorMessageReq *p = message;
                CsrUint8 i;
                for (i = 0; i < p->operatorMessageInfoCount; ++i)
                {
                    CsrPmemFree(p->operatorMessageInfo[i].message);
                }
                CsrPmemFree(p->operatorMessageInfo);
                p->operatorMessageInfo = NULL;
                break;
            }
            case CSR_DSPM_CONNECTION_CREATE_REQ:
            {
                CsrDspmConnectionCreateReq *p = message;
                CsrPmemFree(p->connectionInfo);
                p->connectionInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_START_REQ:
            {
                CsrDspmOperatorStartReq *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CP_OPEN_REQ:
            {
                CsrDspmStreamCpOpenReq *p = message;
                CsrPmemFree(p->cpInfo);
                p->cpInfo = NULL;
                break;
            }
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_DSPM_MODULE */
