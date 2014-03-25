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

void CsrDspmFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_DSPM_PRIM)
    {
        CsrPrim *prim = (CsrPrim *) message;
        switch (*prim)
        {
            case CSR_DSPM_OPERATOR_START_CFM:
            {
                CsrDspmOperatorStartCfm *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_STREAM_SYNC_CFM:
            {
                CsrDspmStreamSyncCfm *p = message;
                CsrPmemFree(p->syncInfo);
                p->syncInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_RESET_CFM:
            {
                CsrDspmOperatorResetCfm *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_CONNECTION_CREATE_CFM:
            {
                CsrDspmConnectionCreateCfm *p = message;
                CsrPmemFree(p->connectionInfo);
                p->connectionInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_MESSAGE_IND:
            {
                CsrDspmOperatorMessageInd *p = message;
                CsrPmemFree(p->message);
                p->message = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CP_CLOSE_CFM:
            {
                CsrDspmStreamCpCloseCfm *p = message;
                CsrPmemFree(p->cpid);
                p->cpid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_MESSAGE_CFM:
            {
                CsrDspmOperatorMessageCfm *p = message;
                CsrUint8 i;
                for (i = 0; i < p->operatorMessageInfoCount; ++i)
                {
                    CsrPmemFree(p->operatorMessageInfo[i].message);
                }
                CsrPmemFree(p->operatorMessageInfo);
                p->operatorMessageInfo = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CONFIGURE_CFM:
            {
                CsrDspmStreamConfigureCfm *p = message;
                CsrPmemFree(p->configInfo);
                p->configInfo = NULL;
                break;
            }
            case CSR_DSPM_CONNECTION_DESTROY_CFM:
            {
                CsrDspmConnectionDestroyCfm *p = message;
                CsrPmemFree(p->cid);
                p->cid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_CREATE_CFM:
            {
                CsrDspmOperatorCreateCfm *p = message;
                CsrPmemFree(p->operatorInfo);
                p->operatorInfo = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_STOP_CFM:
            {
                CsrDspmOperatorStopCfm *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_DESTROY_CFM:
            {
                CsrDspmOperatorDestroyCfm *p = message;
                CsrPmemFree(p->opid);
                p->opid = NULL;
                break;
            }
            case CSR_DSPM_STREAM_CP_OPEN_CFM:
            {
                CsrDspmStreamCpOpenCfm *p = message;
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
