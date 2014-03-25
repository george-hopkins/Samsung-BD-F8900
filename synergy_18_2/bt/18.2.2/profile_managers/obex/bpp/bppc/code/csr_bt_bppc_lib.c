/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_bppc_lib.h"
#include "csr_msg_transport.h"

void CsrBtBppcMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_BPPC_IFACEQUEUE, CSR_BT_BPPC_PRIM, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtBppcCreateJobReqSend
 *
 *  DESCRIPTION
 *      Used to configure a print job
 *
 *    PARAMETERS
 *        cancelOnLostLink:
 *        copies:
 *        sides:
 *        numberUp:
 *        orientation:
 *        printQuality:
 *        jobName:
 *        jobUserName:
 *        documentFormat:
 *        mediaSize:
 *        mediaType:
 *----------------------------------------------------------------------------*/
CsrBtBppcCreateJobReq *CsrBtBppcCreateJobReqSend_struct(CsrBool          cancelOnLostLink,
                                                        CsrUint8         copies,
                                                        CsrUint8         numberUp,
                                                        const CsrUtf8String  *sides,
                                                        const CsrUtf8String  *orientation,
                                                        const CsrUtf8String  *printQuality,
                                                        const CsrUtf8String  *jobName,
                                                        const CsrUtf8String  *jobUserName,
                                                        const CsrUtf8String  *documentFormat,
                                                        const CsrUtf8String  *mediaSize,
                                                        const CsrUtf8String  *mediaType,
                                                        CsrBool          srmpOn)
{
    CsrUint32              length;
    CsrBtBppcCreateJobReq *prim;

    prim                        = (CsrBtBppcCreateJobReq *) CsrPmemZalloc(sizeof(CsrBtBppcCreateJobReq));
    prim->type                  = CSR_BT_BPPC_CREATE_JOB_REQ;
    prim->cancelOnLostLink      = cancelOnLostLink;
    prim->copies                = copies;
    prim->numberUp              = numberUp;
    prim->srmpOn                = srmpOn;

    if (sides != NULL)
    {
        length = CsrStrLen((char*)sides) + 1;
        if (length > 1)
        {
            prim->sides = CsrMemDup(sides, length);
        }
    }

    if (orientation != NULL)
    {
        length = CsrStrLen((char*)orientation) + 0;
        if (length > 1)
        {
            prim->orientation = CsrMemDup(orientation, length);
        }
    }

    if (printQuality != NULL)
    {
        length = CsrStrLen((char*)printQuality) + 1;
        if (length > 1)
        {
            prim->printQuality = CsrMemDup(printQuality, length);
        }
    }

    if (jobName != NULL)
    {
        length = CsrStrLen((char*)jobName) + 1;
        if (length > 1)
        {
            prim->jobName = CsrMemDup(jobName, length);
        }
    }

    if (jobUserName != NULL)
    {
        length = CsrStrLen((char*)jobUserName) + 1;
        if (length > 1)
        {
            prim->jobUserName = CsrMemDup(jobUserName, length);
        }
    }

    if (documentFormat != NULL)
    {
        length = CsrStrLen((char*)documentFormat) + 1;
        if (length > 1)
        {
            prim->documentFormat = CsrMemDup(documentFormat, length);
        }
    }

    if (mediaSize != NULL)
    {
        length = CsrStrLen((char*)mediaSize) + 1;
        if (length > 1)
        {
            prim->mediaSize = CsrMemDup(mediaSize, length);
        }
    }

    if (mediaType != NULL)
    {
        length = CsrStrLen((char*)mediaType) + 1;
        if (length > 1)
        {
            prim->mediaType = CsrMemDup(mediaType, length);
        }
    }

    return(prim);
}


