/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Sends the supplied SDU to the AMPM-SAP  */
static void csrBtAmpmL2capCopySduToAmpmSap(
    CsrBtAmpmInstance * inst,
    const L2CA_DATAREAD_IND_T * pL2caDataInd)
{
    CsrBtAmpmBtdev *pBtDevice;

    CSR_BT_AMPM_ASSERT(pL2caDataInd != NULL);

    pBtDevice = CsrBtAmpmBtdevGetByCid(inst, pL2caDataInd->cid);
    if((NULL != pBtDevice) &&
       (inst->ampmRxSduQid != CSR_SCHED_QID_INVALID))
    {
        CsrBtAmpmUpstreamTestReceivedSduInd(
            inst->ampmRxSduQid,
            pL2caDataInd->cid,
            &(pBtDevice->bdAddr),
            (CsrMblk *) pL2caDataInd->data);
    }
}

/* Handles the received L2CA-REGISTER-FIXED-CID-CFM event. Without
 * Fixed channel support in L2CAP AMP Manager is unable to
 * commmunicate with any peer BT devices.  So we have to wait for the
 * registration with L2CAP to complete before AMP Manager attempts any
 * operations. */
static void csrBtAmpmL2capRegisterFixedCidCfm(
    const L2CA_REGISTER_FIXED_CID_CFM_T * prim)
{
    CSR_BT_AMPM_ASSERT(prim != NULL);
}

/* Handles the received L2CA-DATAREAD-IND event. Which should only
 * contain AMP Manager SDUs, so this functions checks that we've
 * received an AMP Manager SDU decodes it and sends it to the
 * appropriate entity for handling. Currently if a corrupted SDU is
 * received on the L2CAP channel then that channel is closed but what
 * would happen for a fixed channel? */
static void csrBtAmpmL2capDataReadInd(
    CsrBtAmpmInstance * inst,
    const L2CA_DATAREAD_IND_T * pL2caDataInd)
{
    CSR_BT_AMPM_ASSERT(pL2caDataInd != NULL);

    if (pL2caDataInd->result == L2CA_DATA_SUCCESS)
    {
        CsrMblk *pMsgBlk = (CsrMblk *) pL2caDataInd->data;
        CsrUint16 len = CsrMblkGetLength(pMsgBlk);

        if (len != 0)
        {
            const CsrUint8 *pData = CsrMblkMap(pMsgBlk, 0, len);
            CsrUint8 sdu_id = 0;

            sdu_id = CSR_BT_AMPM_DECODE_U8_IE(pData, CSR_BT_AMPM_SDU_CODE_OFFSET);

            if (CsrBtAmpmDataIgnoreSdu(inst, sdu_id))
            {
                return;
            }
            else if (CsrBtAmpmSendRxSduToAmpmSap(inst))
            {
                csrBtAmpmL2capCopySduToAmpmSap(inst, pL2caDataInd);
                return;
            }
            else if (CsrBtAmpmL2capCopyRxSduToAmpmSap(inst))
            {
                csrBtAmpmL2capCopySduToAmpmSap(inst, pL2caDataInd);
            }

            /* Based on the SDU code we can now process the received SDU */
            switch (sdu_id)
            {
                case CSR_BT_AMPM_COMMAND_REJECT:
                    if (CsrBtAmpmSduValidateAmpCommandReject(len, pData))
                    {
                        /* Either the receiver of a SDU that we sent didn't recognise it
                         * or sending the correct response was not appropriate.
                         */
                        if (!CsrBtAmpmBtdevAmpCommandReject(inst, pData,
                                pL2caDataInd->cid))
                        {
                            if (!CsrBtAmpmPhyAmpCommandReject(inst, pData,
                                                              pL2caDataInd->cid))
                            {
                            }
                        }
                    }
                    break;

                case CSR_BT_AMPM_DISCOVER_REQUEST:
                    if (CsrBtAmpmSduValidateAmpDiscoverRequest(len, pData))
                    {
                        /* A peer AMP Manager is requesting to find
                         * out the local AMPs.  Deal with the
                         * request */
                        CsrBtAmpmBtdevAmpDiscoverRequest(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_DISCOVER_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpDiscoverResponse(len, pData))
                    {
                        /* A peer AMP Manager has responded to request for it's local AMPs
                         * This response needs to be directed to the appropriate BT Device
                         * instance that triggered the request
                         */
                        CsrBtAmpmBtdevAmpDiscoverResponse(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_CHANGE_NOTIFY:
                    if (CsrBtAmpmSduValidateAmpChangeNotify(len, pData))
                    {
                        /* The AMP capability at a peer device has changed.
                         * This indication needs to be directed to the appropriate BT Device
                         * responsible for tracking the AMPs on a particular peer device.
                         *
                         * NOTE: Handling of this indication may affect other existing
                         *       AMP instances associated with the peer device.
                         */
                        CsrBtAmpmBtdevAmpChangeNotify(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_CHANGE_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpChangeResponse(len, pData))
                    {
                        /* A peer AMP Manager has received our indication that the local AMP
                         * capability has changed.
                         */
                        CsrBtAmpmBtdevAmpChangeResponse(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_GET_INFO_REQUEST:
                    if (CsrBtAmpmSduValidateAmpGetInfoRequest(len, pData))
                    {
                        /* A peer AMP Manager has sent a request for status and AMP specific
                         * information for a particular local AMP. Probably in preparation for
                         * attempting to establish a physical link over the specified AMP.
                         */
                        CsrBtAmpmBtdevAmpGetInfoRequest(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_GET_INFO_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpGetInfoResponse(len, pData))
                    {
                        CsrBtAmpmBtdevAmpGetInfoResponse(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_GET_AMP_ASSOC_REQUEST:
                    if (CsrBtAmpmSduValidateAmpGetAmpAssocRequest(len, pData))
                    {
                        /* A peer AMP Manager has sent a request for the Assoc for a particular
                         * local AMP. Probably in preparation for attempting to establish a
                         * physical link over the specified AMP.
                         */
                        CsrBtAmpmBtdevAmpGetAmpAssocRequest(inst, pData,
                            pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_GET_AMP_ASSOC_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpGetAmpAssocResponse(len,
                            pData))
                    {
                        /* A peer AMP Manager has responded to our request for status and AMP
                         * specific information for a particular AMP. We can now proceed with
                         * the physical link establishment (if the AMP is useable).
                         */
                        CsrBtAmpmPhyAmpGetAmpAssocResponse(inst, pData,
                                                           pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_CREATE_PHYSICAL_LINK_REQUEST:
                    if (CsrBtAmpmSduValidateAmpCreatePhysicalLinkRequest(len,
                            pData))
                    {
                        /* A peer AMP Manager is requesting the establishment of a physical
                         * link between the two devices, using a specified AMP.
                         */

                        /* Need to create the physical link instance associated with the
                         * appropriate local and remote AMP instances to handle the request.
                         */
                        CsrBtAmpmPhyAmpCreatePhysicalLinkRequest(inst, pData,
                                                                 pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpCreatePhysicalLinkResponse(len,
                            pData))
                    {
                        /* A peer AMP Manager has responded to out request to establish a
                         * physical link. The response will indicate if the establishment
                         * of the physical link has started or whether it has failed.
                         */

                        /* Need to direct the appropriate physical link instance to handle
                         * the response.
                         */
                        CsrBtAmpmPhyAmpCreatePhysicalLinkResponse(inst, pData,
                                                                  pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_REQUEST:
                    if (CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkRequest
                        (len, pData))
                    {
                        /* A peer AMP Manager wants to either abort the set up of a physical
                         * link that has started to be established or disconnect as existing
                         * physical link.
                         */
                        CsrBtAmpmPhyAmpDisconnectPhysicalLinkRequest(inst, pData,
                                                                     pL2caDataInd->cid);
                    }
                    break;

                case CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE:
                    if (CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkResponse
                        (len, pData))
                    {
                        /* A peer AMP Manager has responded to our
                         * request to cancel or disconnect a physical
                         * link. Not sure that there is actually
                         * anything to do here because if we sent the
                         * request we've already started clearing the
                         * connection.  Also we can not take any
                         * action at the remote end if it fails to
                         * disconnect so we might just want to ignore
                         * this.
                         */
                        CsrBtAmpmPhyAmpDisconnectPhysicalLinkResponse(inst, pData,
                                                                      pL2caDataInd->cid);
                    }
                    break;

                default:
                    {
                        CsrMblk *pAMPCommandReject;

                        /* Assuming that we need to send AMP COMMAND REJECT in response. */
                        pAMPCommandReject = CsrBtAmpmSduCreateAmpCommandReject(
                            CSR_BT_AMPM_DECODE_U8_IE(pData, CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET));

                        L2CA_DataWriteReq(pL2caDataInd->cid, 0,
                            pAMPCommandReject);
                    }
                    break;
            }

            CsrMblkUnmap(pMsgBlk, (void*)pData);
        }
    }
}

void CsrBtAmpmL2capHandler(CsrBtAmpmInstance * inst)
{
    void *prim = (inst->msg);

    switch (*(CsrUint16 *) prim)
    {
        case L2CA_REGISTER_FIXED_CID_CFM:
            csrBtAmpmL2capRegisterFixedCidCfm((const L2CA_REGISTER_FIXED_CID_CFM_T *) prim);
            break;

        case L2CA_MAP_FIXED_CID_CFM:
            CsrBtAmpmL2capMapFixedCidCfm(inst,
                (L2CA_MAP_FIXED_CID_CFM_T *) prim);
            break;

        case L2CA_MAP_FIXED_CID_IND:
            CsrBtAmpmL2capMapFixedCidInd(inst,
                (L2CA_MAP_FIXED_CID_IND_T *) prim);
            break;

        case L2CA_UNMAP_FIXED_CID_IND:
            /* Ignore - we use ACL close indications */
            break;

        case L2CA_GETINFO_CFM:
            CsrBtAmpmL2capGetInfoCfm(inst, (L2CA_GETINFO_CFM_T *) prim);
            break;

        case L2CA_DATAWRITE_CFM:
            /* Ignored. A2MP is a command-response protocol with
             * window size 1, so we don't use buffering - hence ignore
             * this message */
            break;

        case L2CA_DATAREAD_IND:
            /* Acknowledge sent data and process it */
            L2CA_DataReadRsp(((L2CA_DATAREAD_IND_T *) prim)->cid,
                             ((L2CA_DATAREAD_IND_T *) prim)->packets);
            csrBtAmpmL2capDataReadInd(inst, (L2CA_DATAREAD_IND_T *) prim);
            break;

        default:
            /* ignore */
            break;
    }
}
