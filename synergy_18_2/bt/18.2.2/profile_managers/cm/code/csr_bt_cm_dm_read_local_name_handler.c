/****************************************************************************

        Copyright Cambridge Silicon Radio Limited and its licensors 2009-2011. 
 
        All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"
#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_util.h"

/* Extracts the name from the DM primitives and */
static void csrBtCmExtractLocalName(DM_HCI_READ_LOCAL_NAME_CFM_T *dmPrim, CsrUtf8String **name)
{
    CsrUintFast16 i, s, nameLength = 0;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    if (*name == NULL)
    {
        /* Determine length of name in order to allocate correct amount of memory */
        for (i = 0; i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS && dmPrim->name_part[i] != NULL; i++)
        {
            for (s = 0;
                 ((s < HCI_LOCAL_NAME_BYTES_PER_PTR)
                  && (dmPrim->name_part[i][s] != '\0')
                  && (nameLength < CSR_BT_MAX_FRIENDLY_NAME_LEN));
                 s++)
            {
                nameLength++;
            }
        }

        *name = (CsrUtf8String*)CsrPmemAlloc(nameLength + 1);
    }
#endif
    /* Copy name from arrays of pointers to *name */
    for (nameLength = 0, i = 0; i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS && dmPrim->name_part[i] != NULL; i++)
    {
        for (s = 0;
             ((s < HCI_LOCAL_NAME_BYTES_PER_PTR)
              && (dmPrim->name_part[i][s] != '\0')
              && (nameLength < CSR_BT_MAX_FRIENDLY_NAME_LEN));
             s++)
        {
            *(*name + nameLength) = (CsrUtf8String)dmPrim->name_part[i][s];
            nameLength++;
        }
    }

    CsrUtf8StrTruncate(*name, nameLength);
}

void CsrBtCmDmHciReadLocalNameCompleteHandler(cmInstanceData_t *cmData)
{
    /* This event is the confirmation that the host controller has
     * read (or attempted to read) the local name. Build and send
     * CSR_BT_CM_READ_LOCAL_NAME_CFM to the application */
    DM_HCI_READ_LOCAL_NAME_CFM_T  *dmPrim = (DM_HCI_READ_LOCAL_NAME_CFM_T*)cmData->recvMsgP;
    CsrUintFast8 i;
    CsrBool eventNotSent = TRUE;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    localEirData_t *localEirData = cmData->dmVar.localEirData;
    if(cmData->globalState == notReady_s)
    {
        /* A name is present in the chip - insert into EIR */
        if (localEirData && !localEirData->requestedName && dmPrim->status == HCI_SUCCESS)
        {
            csrBtCmExtractLocalName(dmPrim, &localEirData->requestedName);
            cmData->dmVar.appHandle = CSR_BT_CM_IFACEQUEUE;
            CsrBtCmEirUpdateName(cmData);
            eventNotSent = FALSE;
        }
    }
    else
#endif
    {
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_NAME
        CsrBtCmReadLocalNameCfm *prim;
        prim = (CsrBtCmReadLocalNameCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadLocalNameCfm));
        prim->type = CSR_BT_CM_READ_LOCAL_NAME_CFM;

        if(dmPrim->status == HCI_SUCCESS)
        {
            prim->localName = NULL;
            csrBtCmExtractLocalName(dmPrim, &prim->localName);
        }
        else
        {
            /*  Read Local Name request failed */
            prim->localName = NULL;
        }

        CsrBtCmPutMessage(cmData->dmVar.appHandle, prim);
#endif
        CsrBtCmDmLocalQueueHandler();
    }


    /* Set CM instance data cache of name */
    if (dmPrim->status == HCI_SUCCESS)
    {
        CsrBool first;
        /* If this is the first time we read the name, we should
         * propagate it to subscribers */
        first = (CsrBool)(cmData->dmVar.localName == NULL ? TRUE : FALSE);

        CsrPmemFree(cmData->dmVar.localName);
        cmData->dmVar.localName = NULL;
        csrBtCmExtractLocalName(dmPrim, &(cmData->dmVar.localName));

        if (first && eventNotSent)
        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE
            /* Propagate name change to subscribers */
            CsrBtCmPropgateEvent(cmData,
                                 CsrBtCmPropagateLocalNameChangeEvent,
                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE,
                                 HCI_SUCCESS,
                                 cmData->dmVar.localName, /* not modified */
                                 NULL);
#endif
        }
    }

    /* Free bits */
    for (i = 0; i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS; i++)
    {
        CsrPmemFree(dmPrim->name_part[i]);
    }
}

#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_NAME
void CsrBtCmReadLocalNameReqHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application desired to know the local
     device address. */
    CsrBtCmReadLocalNameReq *cmPrim;

    cmPrim    = (CsrBtCmReadLocalNameReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle        = cmPrim->phandle;
    dm_hci_read_local_name(NULL);
}
#endif
#endif  /* CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME */
