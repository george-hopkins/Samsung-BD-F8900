/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_events_handler.h"

static void csrBtCmBccmdOnchipHciClient(cmInstanceData_t *cmData)
{
    CsrBccmdCfm * prim = (CsrBccmdCfm *) cmData->recvMsgP;

    CsrBtCmAssertRet(prim->status == CSR_RESULT_SUCCESS);
    CsrBtCmAssertRet(cmData->globalState == notReady_s);

    /* Extract PSKEY value */
    cmData->rfcBuild = (prim->payload[6] == 1);

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    dm_sync_register_req(CSR_BT_CM_IFACEQUEUE, 0);
#else
    CsrBtCmDmSyncRegisterCfmHandler(cmData);
#endif

    CsrPmemFree(prim->payload);
}

void CsrBtCmBccmdArrivalHandler(cmInstanceData_t *cmData)
{
    CsrBccmdPrim *primType = (CsrBccmdPrim *) cmData->recvMsgP;

    if (*primType == CSR_BCCMD_CFM)
    {
        CsrBccmdCfm * prim =  (CsrBccmdCfm *) cmData->recvMsgP;

        switch (prim->varId)
        {
            case BCCMD_VARID_PSVALUE:
                {
                    CsrUint16 key;
                    CsrUint8  *temp = prim->payload;

                    CSR_CONVERT_16_FROM_XAP(key, temp);

                    switch (key)
                    {
                        case PSKEY_LM_MAX_EVENT_FILTERS:
                            {
                                CsrBtCmBccmdMaxEventFilterHandler(cmData);
                                break;
                            }
                        case PSKEY_ONCHIP_HCI_CLIENT:
                            {
                                csrBtCmBccmdOnchipHciClient(cmData);
                                break;
                            }
                        default:
                            {
                                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, cmData->recvMsgP);
                                CsrBtCmGeneralException(CSR_BT_CM_PRIM, *primType, "Unknown key");
                                break;
                            }
                    }
                    break;
                }
            case BCCMD_VARID_INQUIRY_PRIORITY:
            {
                CsrBtCmBccmdInquiryPriorityCfmHandler(cmData);
                break;
            }
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            case BCCMD_VARID_MAP_SCO_PCM:
                {
                    CsrBtCmBccmdMapScoPcmCfmHandler(cmData);
                    break;
                }
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            case BCCMD_VARID_GET_SCO_PARAMETERS:
                {
                    CsrUint8  *temp = prim->payload;
                    CsrUint16 tEsco = 0xFFFF;
                    CsrUint16 wEsco = 0xFFFF;
                    CsrUint16 reservedSlots = 0xFFFF;
                    CsrUint16 escoHandle = 0xFFFF;
                    CsrBool   found = FALSE;

                    if (prim->status == CSR_RESULT_SUCCESS)
                    {
                        CSR_CONVERT_16_FROM_XAP(escoHandle, temp);
                        CSR_CONVERT_16_FROM_XAP(tEsco, temp);
                        CSR_CONVERT_16_FROM_XAP(wEsco, temp);
                        CSR_CONVERT_16_FROM_XAP(reservedSlots, temp);
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                        {/* RFC */
                            cmRfcConnElement *currentElem;

                            for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); (currentElem && !found); currentElem = currentElem->next)
                            {
                                if (currentElem->cmRfcConnInst)
                                {
                                    if((hci_connection_handle_t)escoHandle == currentElem->cmRfcConnInst->eScoParms.handle)
                                    {
                                        currentElem->cmRfcConnInst->eScoParms.reservedSlots     = (CsrUint8)reservedSlots;
                                        currentElem->cmRfcConnInst->eScoParms.txInterval        = (CsrUint8)tEsco;
                                        currentElem->cmRfcConnInst->eScoParms.weSco             = (CsrUint8)wEsco;
                                        found = TRUE;

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION
                                        /* Now send event indication if subscribed */
                                        CsrBtCmPropgateEvent(cmData,
                                            CsrBtCmPropgateRfcExtSyncConnectEvents,
                                            CSR_BT_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION,
                                            HCI_SUCCESS,
                                            (void *)currentElem,
                                            NULL);
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION */
                                    }
                                }
                            }
                        }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
                    }
                    /* else send indication event?? */
                    CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, cmData->recvMsgP);
                    break;
                }
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */
            default:
                {
                    CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, cmData->recvMsgP);
                    CsrBtCmGeneralException(CSR_BCCMD_PRIM, *primType, "Unknown varid");
                    break;
                }
        }
    }
    else
    {
        CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, cmData->recvMsgP);
        CsrBtCmGeneralException(CSR_BCCMD_PRIM, *primType, "Unknown CSR_BCCMD message");
    }
}

