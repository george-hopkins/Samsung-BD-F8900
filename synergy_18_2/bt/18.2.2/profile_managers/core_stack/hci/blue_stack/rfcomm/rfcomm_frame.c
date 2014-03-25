/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_frame.c
\brief  Functions for creating and parsing RFCOMM frames.
*/

#include "csr_synergy.h"

#ifdef INSTALL_RFCOMM_MODULE

#include "rfcomm_private.h"

static CsrBool rfc_is_valid_frame_structure(CsrUint16 rx_frame_len,
                                           CsrUint16 mblk_len,
                                           CsrUint16 exp_frame_len,
                                           CsrUint8  crbit,
                                           CsrUint8  exp_crbit,
                                           CsrUint8  fcs,
                                           CsrUint8  exp_fcs);

static void rfc_extract_pn_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                 RFC_PN_VALUES_T  *pn_values);

static void rfc_extract_rpn_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                  RFC_RPN_VALUES_T  *rpn_values);

static CsrUint16 rfc_get_cmd_length(RFC_CTRL_PARAMS_T *rfc_params);

static void rfc_parse_mux_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params);

static void rfc_process_rx_data(RFC_CTRL_PARAMS_T *rfc_params,
                                CsrUint16 data_len);

/* Lookup table for calculating CRC */
static const CsrUint8 crctable[16] = {
    0x00, 0x1C, 0x38, 0x24, 0x70, 0x6C, 0x48, 0x54,
    0xE0, 0xFC, 0xD8, 0xC4, 0x90, 0x8C, 0xA8, 0xB4
};



/*! \brief Calculate the CRC for a frame.

    \param frame - Pointer to the start of the data for which the checksum will
                   be calculated.
    \param length - length of the data for which the checksum is to be
                    calculated.
*/
CsrUint8 rfc_frame_crc(const CsrUint8 *frame,
                      CsrUint16 length)
{
    CsrUint8 crc = 0xFF;
    CsrUint16 index;

    for (index = 0; index != length; ++index)
    {
        CsrUint8 n = frame[index];
        crc = (crc >> 4) ^ crctable[(crc ^ n) & 0x0f];
        crc = (crc >> 4) ^ crctable[(crc ^ (n >> 4)) & 0x0f];
    }

    return (0xFF - crc);
}

/*! \brief Determine the CR bit value to be used in a frame.

    CR bits are used in two different points within rfcomm frames. The first is
    at the frame level (Control frames) , the second is at the message level
    (for command frames embedded in UIH control frames). The value of the CR bit
    depends on the following rules:-

    FRAME LEVEL:

    If the frame is a UIH then the initiator uses a value of 1 and the responder
    uses 0.
    For non UIH frames, commands from the initiator and responses from the
    responder use a value of 1. Commands from the responder and responses from
    the initiator use 0.

    MESSAGE LEVEL:

    Commands have a value of 1, reponses a value of 0.

    \param from - Initiator or Responder
    \param type - Command, Response or Data
    \param level - Frame or Message
    \param crbit - Returned CR bit value
*/
CsrUint8 rfc_calc_crbit(RFC_LINK_DESIGNATOR from,
                       RFC_CRTYPE type,
                       RFC_CRLEVEL level)
{
    CsrUint8 crbit = 0xFF; /* Default illegal value */

    switch (level)
    {
        case RFC_FRAMELEVEL:

            switch (type)
            {
                case RFC_DATA:
                case RFC_CMD:

                    crbit = (from == RFC_INITIATOR) ? 1 : 0;
                    break;

                case RFC_RSP:

                    crbit = (from == RFC_INITIATOR) ? 0 : 1;
                    break;

                default:
                    break;
            }
            break;

        case RFC_MSGLEVEL:

            switch (type)
            {
                case RFC_CMD:

                    crbit = 1;
                    break;

                case RFC_RSP:

                    crbit = 0;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    return crbit;
}

/*! \brief Create and format the address field for a frame

    \param dlci - DLC for which the frame is required
    \param from - Initiator or Responder
    \param cr_type - Command or Response
    \param level - Frame or Message
    \param addr - Returned formatted address field
*/

CsrUint8 rfc_create_address_field(CsrUint8             dlci,
                                 RFC_LINK_DESIGNATOR from,
                                 RFC_CRTYPE          cr_type,
                                 RFC_CRLEVEL         level)
{
    CsrUint8  addr;
    CsrUint8  cr_bit;

    /* Construct and write the address field.
    */
    cr_bit = rfc_calc_crbit( from, cr_type, level );

    addr = cr_bit << 1 | dlci << 2 ;
    RFC_SET_EA_BIT(addr);

    return addr;

}

/*! \brief Create a control frame

    Creates a control frame (SABM, UA, DISC or DM) for the channel specified.
    The frame is created within a MBLK which is then returned to the caller.

    \param frame - Type of frame to be created.
    \param flags - flags containing the initiator bit
    \param dlci  - DLC for which the frame is required
    \param mblk_frame - Returned MBLK containing the control frame
*/
CsrMblk  *rfc_create_ctrl_frame(RFC_FRAME_TYPE_T frame,
                               CsrUint16 flags,
                               CsrUint8 dlci)
{
    CsrUint8 *ctrl_frame = NULL;
    CsrUint8 *frame_copy = NULL;
    CsrMblk  *mblk_frame = NULL;
    CsrUint8 address = 0;
    CsrUint8 fcs = 0;
    RFC_LINK_DESIGNATOR from = RFC_IS_INITIATOR(flags) ? RFC_INITIATOR
                                                       : RFC_RESPONDER;

    switch (frame)
    {
        case RFC_SABM:
        case RFC_DISC:
            /* Command.
            */
            address = rfc_create_address_field(dlci,
                                               from,
                                               RFC_CMD,
                                               RFC_FRAMELEVEL);
            break;

        case RFC_UA:
        case RFC_DM:
            /* Response.
            */
            address = rfc_create_address_field(dlci,
                                               from,
                                               RFC_RSP,
                                               RFC_FRAMELEVEL);
            break;

        default:
            /* Invalid frame which should never occur as this is an internal
               interface!
            */
            break;
    }

    mblk_frame = CsrMblkMallocCreate((void **)&ctrl_frame, RFC_CTRLFRAME_LEN);
    if (mblk_frame != NULL)
    {
        CsrUint8 length = 0;

        /* Preserve the pointer to the start of the frame as this is needed
           for the frame check sequence however is incremented by the
           write_uints function.
        */
        frame_copy = ctrl_frame;
        write_uint8(&ctrl_frame, address);
        write_uint8(&ctrl_frame, frame);
        RFC_SET_EA_BIT(length);
        write_uint8(&ctrl_frame, length);

        fcs = rfc_frame_crc(frame_copy,RFC_CTRLFRAME_CRC_LEN);

        write_uint8(&ctrl_frame, fcs);
    }
    else
    {
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_frame;
}


/*! \brief Set the EA bit in a length field if required

    Determines whether the EA bit needs to be set in the provided 16 bit length
    field.

    \param len - Pointer to length value
    \param Returns 0 if the EA bit was set, 1 if it wasnt.
*/
CsrUint8 rfc_set_len_EA_bit(CsrUint16 *len)
{
    *len = *len << 1;
    if (*len <= (RFC_CMD_MAX_LEN_OCTET_VAL << 1))
    {
        /* 7 bit length therefore extended addressing is unecessary and the EA
           bit must be set.
        */
        RFC_SET_EA_BIT(*len);
        return 0;
    }

    return 1;
}

/*! \brief Creates an UIH header which precedes a command/response or data
           frame.

           Note: This function assumes that the block of memory created to hold
           the frame header is of the correct size whether or not 1 or 2 bytes
           are required to store the length field, or if a credit field is
           required. It is the caller's responsibility to ensure this.

    \param dlci - Destination channel
    \param frame - Pointer to the address of the data area in which the frame is
                   to be created.
    \param len - length of the data that will follow
    \param from - Initiator or Responder
*/
void rfc_create_uih_header(CsrUint8 dlci,
                           CsrUint8 **frame,
                           CsrUint16 len,
                           RFC_LINK_DESIGNATOR from,
                           CsrUint8 rx_credits)
{
    CsrUint8  addr = 0;
    CsrUint8  crbit;
    CsrUint8  uih_type;
    unsigned int format[] =
        {URW_FORMAT(CsrUint8, 3, UNDEFINED, 0, UNDEFINED, 0),
         URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0)
        };
    CsrUint8  format_index;

    /* Form the address field
    */
    crbit = rfc_calc_crbit(from,
                           RFC_DATA,
                           RFC_FRAMELEVEL);

    RFC_SET_EA_BIT(addr);
    addr |= (crbit << 1);
    addr |= (dlci << 2);

    format_index = rfc_set_len_EA_bit(&len);

    if (rx_credits > 0)
    {
        uih_type = RFC_UIH_PF;
    }
    else
    {
        uih_type = RFC_UIH;
    }

    /* As commands and responses are sent in data (UIH) frames on the mux
       there are no credits sent.
    */
    write_uints(
        frame,
        format[format_index],
        addr,
        uih_type,
        len);

    if (rx_credits > 0)
    {
        write_uint8(frame, rx_credits);
    }
}

/*! \brief Check the validity of a received frame

    Expected values for frame length, crbit and checksum are checked against
    those received. All of them must match for the frame to be declared valid.

    \param rx_frame_len - frame length value in the frame
    \param mblk_len - actual frame length received
    \param exp_frame_len - the expected frame length
    \param crbit - crbit in the frame
    \param exp_crbit - the expected crbit
    \param fcs - the checksum in the frame
    \param exp_fcs - the expected checksum
    \param valid - returned validity. Valid = TRUE
*/
static CsrBool rfc_is_valid_frame_structure(CsrUint16 rx_frame_len,
                                           CsrUint16 mblk_len,
                                           CsrUint16 exp_frame_len,
                                           CsrUint8  crbit,
                                           CsrUint8  exp_crbit,
                                           CsrUint8  fcs,
                                           CsrUint8  exp_fcs)
{
    if (crbit == exp_crbit &&
        rx_frame_len == mblk_len &&
        rx_frame_len == exp_frame_len &&
        fcs == exp_fcs)
    {
        return TRUE;
    }

    return FALSE;

}
/*! \brief Check the validity of a received data frame

    This is very similar to rfc_is_valid_frame_structure() except
    that we are less strict when checking the CR bit, and other
    largely irrelevant parts of the header. If the strict checks fail
    then we recalculate the FCS and declare the frame valid if it
    matches the FCS in the frame, irrespective of what the CR or D
    bits happen to be set to.

    \param rx_frame_len - frame length value in the frame
    \param mblk_len - actual frame length received
    \param exp_frame_len - the expected frame length
    \param crbit - crbit in the frame
    \param exp_crbit - the expected crbit
    \param fcs - the checksum in the frame
    \param exp_fcs - the expected checksum
    \param frame_hdr - pointer to 2 octets of frame header
    \returns TRUE for a valid, or near-valid frame, otherwise FALSE.
*/
static CsrBool rfc_is_valid_data_frame(CsrUint16 rx_frame_len,
                                      CsrUint16 mblk_len,
                                      CsrUint16 exp_frame_len,
                                      CsrUint8  crbit,
                                      CsrUint8  exp_crbit,
                                      CsrUint8  fcs,
                                      CsrUint8  exp_fcs,
                                      CsrUint8 *frame_hdr)
{
    if (rx_frame_len == mblk_len && rx_frame_len == exp_frame_len)
    {
        /* If the crbit or fcs isn't what we were expecting then
           the frame is technically invalid. But, for the sake of
           IOP, we pass it if the FCS is correct for whatever it
           is that they've given us. This is only done for UIH data
           frames where the CR bit doesn't really matter that much. */
        if ((crbit == exp_crbit && fcs == exp_fcs)
                || fcs == rfc_frame_crc(frame_hdr, RFC_UIH_FCS_CALC_SIZE))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*! \brief Extract and decode a parneg frame

    The 'mblk' field in the rfc_params is a scratch area pointer to the data
    within a MBLK containing the data to be processed.

    \param rfc_params - Pointer to rfcomm instance data.
    \param pn_values - Pointer to the area to store the decoded parameters
*/
static void rfc_extract_pn_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                 RFC_PN_VALUES_T  *pn_values)
{
    CsrUint8 frame_size_low, frame_size_high;

    (void)CsrBtMblkReadHead8s(&(rfc_params->mblk), 8,
                           &(pn_values->dlci),
                           &(pn_values->cfc_supported),
                           &(pn_values->priority),
                           &(pn_values->ack_timer),
                           &frame_size_low,
                           &frame_size_high,
                           &(pn_values->max_rtx),
                           &(pn_values->init_creds));

    pn_values->max_frame_size = (frame_size_high << 8) | frame_size_low;

    /* Now destroy the mblk as it is no longer needed.
    */
    CsrMblkDestroy(rfc_params->mblk);

}

/*! \brief Extract and decode a portneg frame

    The 'mblk' field in the rfc_params is a scratch area pointer to the data
    within a MBLK containing the data to be processed.

    \param rfc_params - Pointer to rfcomm instance data.
    \param rpn_values - Pointer to the area to store the decoded parameters
*/
static void rfc_extract_rpn_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                  RFC_RPN_VALUES_T  *rpn_values)
{
    CsrUint8 temp, parameter_mask_low, parameter_mask_high;

    (void)CsrBtMblkReadHead8s(&(rfc_params->mblk), 7,
                           &(rpn_values->params.baud_rate),
                           &temp,
                           &(rpn_values->params.flow_ctrl_mask),
                           &(rpn_values->params.xon),
                           &(rpn_values->params.xoff),
                           &parameter_mask_low,
                           &parameter_mask_high);

    /* The field read into temp has the following format:

       |  7  6  |   5   4   |   3  |    2    |  1  0   |
       |reserved|parity type|parity|stop bits|data bits|
    */
    rpn_values->params.data_bits = temp & 0x3;
    temp = temp >> 2;
    rpn_values->params.stop_bits = temp & 0x1;
    temp = temp >> 1;
    rpn_values->params.parity = temp & 0x1;
    temp = temp >> 1;
    rpn_values->params.parity_type = temp & 0x3;

    /* Top 2 bits of the flow control field are reserved so mask out.
    */
    rpn_values->params.flow_ctrl_mask = rpn_values->params.flow_ctrl_mask & 0x3F;

    rpn_values->params.parameter_mask =
        (parameter_mask_high << 8) | parameter_mask_low;

    /* Now destroy the mblk as it is no longer needed.
    */
    CsrMblkDestroy(rfc_params->mblk);
}

/*! \brief Determine the actual length of a message (command/response)

    The length field for commands and responses can have multiple length octets
    using 7 bits of data , with an EA bit only set on the last octet. This
    function builds the actual length value for a set of length octets.

    \param rfc_params - Pointer to rfcomm instance data.
    \param len - Returned length value
*/
static CsrUint16 rfc_get_cmd_length(RFC_CTRL_PARAMS_T *rfc_params)
{
    CsrUint16  len;
    CsrUint8   byte;
    CsrUint8   shift = RFC_CMD_LEN_SIZE; /* bits */

    (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &byte);

    /* Initialise the return value with the first length octet stripped of its
       EA bit.
    */
    len = byte >> 1;
    while (RFC_IS_EA_BIT_RESET(byte))
    {
        (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &byte);
        len |= (byte >> 1) << shift;
        shift += RFC_CMD_LEN_SIZE;
    }

    return len;
}

/*! \brief Send a disconnect mode (DM) frame to the peer

    \param flags - for the channel being responded on.
    \param dlci  - channel being responded on.
    \param cid   - l2cap channel used for this channel.
*/
void rfc_send_dm_frame(CsrUint16 flags,
                       CsrUint8 dlci,
                       l2ca_cid_t cid)
{
    CsrMblk  *mblk_frame;

    /* Create and send DM frame to the peer to reject the connection.
    */
    mblk_frame = rfc_create_ctrl_frame(RFC_DM,
                                       flags,
                                       dlci);

    L2CA_DataWriteReq(cid,
                      0,/* Indicates MBLK interface*/
                      mblk_frame);

}

static void rfc_new_pn_initiated_server_chan(RFC_CTRL_PARAMS_T *rfc_params,
                                             RFC_PN_VALUES_T  *pn_values)
{
    /* Check that the PN req has arrived for a valid server
       channel. If it is a valid server chan then the DLC
       structure will be created and tagged onto the
       rfc_params.
    */
    if (!rfc_validate_server_connect_req(rfc_params, pn_values->dlci))
    {
        /* PN received for an unknown channel. Create and
           send DM frame to the peer to reject the
           connection.
        */
        rfc_send_dm_frame(rfc_params->p_mux->flags,
                          pn_values->dlci,
                          rfc_params->p_mux->info.mux.cid);
        CsrPmemFree(pn_values);

        return;
    }
    else
    {
        /* Cancel mux shutdown timer if it is running.
        */
        rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_DISCONNECT_CONTEXT]));

        rfc_params->p_dlc->info.dlc.p_temp_pn = pn_values;
        rfc_params->p_dlc->info.dlc.start_type = RFC_START_PARNEG;
        crank_channel_fsm(rfc_params, NULL, RFC_EV_SERVER_IND);
    }
}


/*! \brief Parse the message within a UIH frame (Command or Response)

    This function decodes the message within an UIH frame and determines the
    correct course of action to take accordingly.

    \param rfc_params - Pointer to rfcomm instance data.
*/
static void rfc_parse_mux_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params)
{
    CsrUint8 type;
    CsrUint16 len;

    (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &type);
    len = rfc_get_cmd_length(rfc_params);

    switch (type & RFC_MASKOUT_CRBIT)
    {
        case RFC_PN_TYPE:      /* Parameter Negotiation */

            /* Check if the message length is as expected
            */
            if (len == RFC_PN_DATA_LEN)
            {
                RFC_PN_VALUES_T  *pn_values = pnew(RFC_PN_VALUES_T) ;

                rfc_extract_pn_frame(rfc_params, pn_values);
                rfc_find_chan_by_dlci(rfc_params, pn_values->dlci);

                /* Test if this is a command or a response.
                */
                if( RFC_IS_CMD(type) )
                {
                    /* An incoming PN command received on a DLCI initiates a new
                       connection unless there is already an open channel with
                       that DLCI
                    */
                    if (NULL == rfc_params->p_dlc)
                    {
                        rfc_new_pn_initiated_server_chan(rfc_params,
                                                         pn_values);
                    }
                    else
                    {
                        rfc_params->p_dlc->info.dlc.p_temp_pn = pn_values;

                        /* Only handle the parneg ind at this point if the link
                           is marked 'started' ie whichever command initiated
                           the link has provoked the connect ind/rsp process and
                           then been dealt with itself. Otherwise the parneg
                           just needs to be queued and will be dealt with once
                           the connect rsp is received from the host.
                        */
                        if (rfc_params->p_dlc->info.dlc.start_type == RFC_STARTED &&
                            rfc_params->p_dlc->info.dlc.p_config == NULL)
                        {
                            /* This appears to be a renogiating parneg. If the
                               initial credits is non zero however, it could
                               be that this is a connection initiating parneg
                               and that we are in a funny state due to the
                               peer destroying a channel locally after a
                               previous parneg sequence but before an SABM was
                               sent. In this case we want to terminate the
                               previous connection and reinitiate from
                               scratch.
                            */
                            if (pn_values->init_creds > 0)
                            {
                                RFC_RESPONSE_T status = RFC_ABNORMAL_DISCONNECT;

                                /* As the actions requirted to terminate the
                                   DLC in this case are the same as if the
                                   l2cap channel had been dropped, we can
                                   crank the statemachine with a
                                   PROTOCOL_DISCONNECT event to achieve the
                                   necessary results.
                                */

                                /* Ensure pn_values are not destroyed.
                                */
                                rfc_params->p_dlc->info.dlc.p_temp_pn = NULL;
                                crank_channel_fsm(rfc_params, &status, RFC_EV_PROTOCOL_DISCONNECT);
                                rfc_params->p_dlc = NULL;
                                rfc_new_pn_initiated_server_chan(rfc_params,
                                                                 pn_values);
                                return;
                            }

                            crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_IND);
                        }
                    }

                }
                else
                {
                    /* This is a response.
                    */
                    if (NULL != rfc_params->p_dlc)
                    {
                        /* Cancel any pending timer.
                        */
                        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_CONNECT_CONTEXT]));

                        if(!RFC_IS_DISCONNECTING(rfc_params->p_dlc->flags))
                        {
                            rfc_params->p_dlc->info.dlc.p_temp_pn = pn_values;
                            crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_CFM);
                            return;
                        }
                    }

                    /* PN response received for an unknown or disconnecting
                       channel.
                    */
                    CsrPmemFree(pn_values);
                }

            }
            else
            {
                /* Invalid frame received. We cant do anything except ignore it.
                */
            }
            break;

        case RFC_TEST_TYPE:    /* Test */

            /* Test if this is a command or a response.
            */
            if( RFC_IS_CMD(type) )
            {
                /* Need to send a response to the peer , copying out the
                   received data pattern.
                */
                CsrMblk *mblk_test =
                    rfc_create_test_cmdrsp(rfc_params,
                                           RFC_RSP,
                                           rfc_params->mblk);

                /* Ensure MBLK, copied into the response , doesnt get destroyed
                   at the end of the frame processing.
                */
                rfc_params->mblk = NULL;

                /* Send to L2CAP.
                */
                L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                                  0,/* Indicates MBLK interface*/
                                  mblk_test);

            }
            else
            {
                /* This is a response. Pass up to the host. Received test data
                   will be in the 'mblk' field of rfc_params.
                */
                rfc_send_test_cfm(rfc_params->p_mux->phandle,
                                  &(rfc_params->p_mux->info.mux.bd_addr),
                                  rfc_params->mblk,
                                  RFC_SUCCESS);

                /* Ensure MBLK, copied into the response , doesnt get destroyed
                   at the end of the frame processing.
                */
                rfc_params->mblk = NULL;

                /* Cancel any pending timer.
                */
                rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_TEST_CONTEXT]));

            }
            break;

        case RFC_FCON_TYPE:    /* FCon  */

            /* There is not data associated with a FC cmd so the length field
               should be 0. The command is only valid if we do not support
               credit based flow control. If we have already had a FCON without
               a corresponding FCOFF command then we will just ignore this one.
            */
            if (len == 0 &&
                !(RFC_CREDIT_FC_USED(rfc_params->p_mux->flags)))
            {
                if( RFC_IS_CMD(type) )
                {
                    if (!(RFC_IS_FC_TX_ENABLED(rfc_params->p_mux->flags)))
                    {
                        RFC_TOGGLE_FC_TX_ENABLED(rfc_params->p_mux->flags);

                        /* Notify host.
                        */
                        rfc_send_fc_ind(rfc_params,
                                        RFC_FC_ON);

                        /* See if there is any outstanding data to send.
                        */
                        rfc_data_kick_all(rfc_params);
                    }
                    else
                    {
                        /* This appears to be a duplicate fc_on as we are already
                           in that state, thus ignore it!
                        */
                    }

                    /* Send a response to the peer.
                    */
                    rfc_send_fc_cmdrsp(rfc_params, RFC_RSP, RFC_FC_ON);
                }
                else
                {
                    /* Response has been received from the peer thus cancel any
                       timer that is running for this command type.
                    */
                    rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_FCON_CONTEXT]));
                }
            }
            else
            {
                /* Illegal FC cmd, reject it.
                */
                rfc_send_nsc_rsp(rfc_params,
                                 (CsrUint8)RFC_FCON_TYPE,
                                 (CsrUint8)RFC_GET_CRBIT_FROM_DATA_FIELD(type));

            }

            break;

        case RFC_FCOFF_TYPE:   /* FCoff */

            /* There is no data associated with a FC cmd so the length field
               should be 0. The command is only valid if we do not support
               credit based flow control. If we havent already had a FCON
               command then we just ignore this command.
            */
            if ((len == 0) &&
                !(RFC_CREDIT_FC_USED(rfc_params->p_mux->flags)) )
            {
                if( RFC_IS_CMD(type) )
                {
                    if (RFC_IS_FC_TX_ENABLED(rfc_params->p_mux->flags))
                    {
                        RFC_TOGGLE_FC_TX_ENABLED(rfc_params->p_mux->flags);

                        /* Notify host.
                        */
                        rfc_send_fc_ind(rfc_params,
                                        RFC_FC_OFF);
                    }
                    else
                    {
                        /* This appears to be a duplicate fc_off as we are already
                           in that state, thus ignore it!
                        */
                    }

                    /* Send a response to the peer.
                    */
                    rfc_send_fc_cmdrsp(rfc_params, RFC_RSP, RFC_FC_OFF);
                }
                else
                {
                    /* Response has been received from the peer thus cancel any
                       timer that is running for this command type.
                    */
                    rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_FCOFF_CONTEXT]));
                }
            }
            else
            {
                /* Illegal FC cmd, reject it.
                */
                rfc_send_nsc_rsp(rfc_params,
                                 (CsrUint8)RFC_FCOFF_TYPE,
                                 (CsrUint8)RFC_GET_CRBIT_FROM_DATA_FIELD(type));
            }

            break;

        case RFC_MSC_TYPE:     /* Modem Status Command */

            /* Check if full MSC or MSC without break signal
            */
            if (len == RFC_MSC_DATA_LEN || len == RFC_MSC_DATA_LEN - 1)
            {
                CsrUint8 dlci_field;
                CsrUint8 modem_signal;
                CsrUint8 break_signal = 0;
                CsrUint8 dlci;

                (void)CsrBtMblkReadHead8s(&(rfc_params->mblk), 2,
                                       &dlci_field,
                                       &modem_signal);

                if (len == RFC_MSC_DATA_LEN)
                {
                    (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &break_signal);
                }

                /* Extract the dlci value from the dlci field.
                */
                dlci = RFC_GET_DLCI_FROM_ADDR(dlci_field);

                rfc_find_chan_by_dlci(rfc_params, dlci);

                /* Test if this is a command or a response.
                */
                if( RFC_IS_CMD(type) )
                {
                    if (NULL != rfc_params->p_dlc)
                    {
                        CsrMblk *mblk_msc;

                        /* Check validity of flow control part of command
                           before responding.
                        */
                        if ((modem_signal & RFC_MSC_FC_BIT) &&
                            RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
                        {
                            /* Send NSC.
                            */
                            rfc_send_nsc_rsp(rfc_params,
                                             (CsrUint8)RFC_MSC_TYPE,
                                             (CsrUint8)RFC_GET_CRBIT_FROM_DATA_FIELD(type));
                            return;
                        }

                        /* Need to send a response to the peer , copying out the
                           received data.
                        */
                        mblk_msc = rfc_create_modem_status_cmdrsp(rfc_params,
                                                                  RFC_RSP,
                                                                  dlci_field,
                                                                  modem_signal,
                                                                  break_signal);

                        /* Send to L2CAP
                        */
                        L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                                          0,/* Indicates MBLK interface*/
                                          mblk_msc);

                        /* Send notification to the host.
                        */
                        rfc_send_msc_ind(rfc_params,
                                         (CsrUint8)RFC_RESET_EA_BIT(modem_signal),
                                         (CsrUint8)(break_signal >> 1));

                        crank_channel_fsm(rfc_params, NULL, RFC_EV_MSC_RSP);

                        /* Update flow control status if applicable.
                        */
                        rfc_update_msc_fc_status(&(rfc_params->p_dlc->flags),
                                                 rfc_params->p_mux->flags,
                                                 (CsrUint16)RFC_MSC_FC_TX_ENABLED,
                                                 modem_signal);

                        if (RFC_IS_CHAN_OPEN(rfc_params->p_dlc) &&
                            RFC_IS_MSC_FC_TX_ENABLED(rfc_params->p_dlc->flags))
                        {
                            rfc_data_kick_all(rfc_params);
                        }
                    }
                    else
                    {
                        /* MSC received for an unknown dlci. Thus send a DM
                           frame to indicate this dlci is not connected.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          dlci,
                                          rfc_params->p_mux->info.mux.cid);
                        return;
                    }
                }
                else
                {
                    if (NULL != rfc_params->p_dlc)
                    {
                        /* Are we expecting a MSC cfm from the peer ?
                         */
                        if (rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_MSC_CONTEXT]))
                        {
                            /* Cancel the response timer.
                            */
                            rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_MSC_CONTEXT]));

                            crank_channel_fsm(rfc_params, NULL, RFC_EV_MSC_CFM);
                        }
                    }
                }
            }
            else
            {
                /* Invalid frame received. There isnt much we can do other than
                   ignore it.
                */
            }
            break;

        case RFC_RPN_TYPE:     /* Port Negotiation */

            if (len == RFC_RPN_DATA_LEN || len == RFC_RPN_REQUEST_DATA_LEN)
            {
                RFC_RPN_VALUES_T  *rpn_values = pnew(RFC_RPN_VALUES_T);

                (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &(rpn_values->dlci));
                rpn_values->dlci = RFC_GET_DLCI_FROM_ADDR(rpn_values->dlci);

                if (len == RFC_RPN_REQUEST_DATA_LEN)
                {
                    rpn_values->request = TRUE;
                }
                else
                {
                    rfc_extract_rpn_frame(rfc_params, rpn_values);
                    rpn_values->request = FALSE;
                }

                rfc_find_chan_by_dlci(rfc_params, rpn_values->dlci);

                /* Test if this is a command or a response.
                */
                if( RFC_IS_CMD(type) )
                {
                    /* An incoming RPN command received on a DLCI initiates a
                       new connection unless there is already an open channel
                       with that DLCI
                    */
                    if (NULL == rfc_params->p_dlc)
                    {
                        /* Check that the RPN req has arrived for a valid server
                           channel. If it is a valid server chan then the DLC
                           structure will be created and tagged onto the
                           rfc_params.
                        */
                        if (!rfc_validate_server_connect_req(rfc_params, rpn_values->dlci))
                        {
                            /* RPN received for an unknown channel.
                            */
                            CsrPmemFree(rpn_values);
                            return;
                        }
                        else
                        {
                            /* Cancel mux shutdown timer if it is running.
                            */
                            rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_DISCONNECT_CONTEXT]));

                            rfc_params->p_dlc->info.dlc.p_temp_rpn = rpn_values;
                            rfc_params->p_dlc->info.dlc.start_type = RFC_START_PORTNEG;
                            crank_channel_fsm(rfc_params, NULL, RFC_EV_SERVER_IND);
                        }
                    }
                    else
                    {
                        rfc_params->p_dlc->info.dlc.p_temp_rpn = rpn_values;

                        /* Only send a portneg ind to the host at this point
                           if the link is marked 'started' ie whichever
                           command initiated the link has provoked the connect
                           ind/rsp process and then been dealt with itself.
                           Otherwise the portneg just needs to be queued and
                           will be dealt with once the connect rsp is received
                           from the host.
                        */
                        if (rfc_params->p_dlc->info.dlc.start_type == RFC_STARTED)
                        {
                            rfc_send_portneg_ind(rfc_params->p_dlc->phandle,
                                                 rfc_params->p_dlc->info.dlc.conn_id,
                                                 rfc_params->p_dlc->info.dlc.p_temp_rpn);
                            CsrPmemFree(rpn_values);
                            rfc_params->p_dlc->info.dlc.p_temp_rpn = NULL;
                        }
                    }
                }
                else
                {
                    /* This is a response.
                    */
                    if (NULL != rfc_params->p_dlc)
                    {
                        /* Cancel mux shutdown timer if it is running.
                        */
                        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_RPN_CONTEXT]));

                        /* Forward to host.
                        */
                        rfc_send_portneg_cfm(rfc_params->p_dlc->phandle,
                                             rfc_params->p_dlc->info.dlc.conn_id,
                                             rpn_values,
                                             RFC_SUCCESS);
                    }
                    else
                    {
                        /* RPN received for an unknown channel. Create and send
                           DM frame to the peer to reject the connection.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          rpn_values->dlci,
                                          rfc_params->p_mux->info.mux.cid);
                    }
                    CsrPmemFree(rpn_values);

                }
            }
            else
            {
                /* Invalid frame received. We cant do much except just ignore
                   it.
                */
            }

            break;

        case RFC_RLS_TYPE:     /* Line Status */

            if (len == RFC_RLS_DATA_LEN)
            {
                CsrUint8 line_status_field;
                CsrUint8 dlci_field;
                CsrUint8 dlci;

                /* Get the dlci address field.
                */
                (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &dlci_field);

                /* Extract the dlci value from the dlci field.
                */
                dlci = RFC_GET_DLCI_FROM_ADDR(dlci_field);

                /* Test if this is a command or a response.
                */
                if( RFC_IS_CMD(type) )
                {
                    rfc_find_chan_by_dlci(rfc_params, dlci);
                    if (NULL != rfc_params->p_dlc)
                    {
                        CsrMblk  *mblk_rls;
                        CsrUint8 error_flag;
                        CsrUint8 line_status;

                        /* read and decode the error flag and actual line
                           status.
                        */
                        (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &line_status_field);
                        error_flag = line_status_field & RFC_RLS_ERROR_FLAG;
                        line_status = line_status_field >> 1;

                        if (rfc_validate_line_status(error_flag,
                                                     line_status))
                        {
                            rfc_send_line_status_ind(rfc_params,
                                                     error_flag,
                                                     line_status);

                            /* Now respond to this RLS command
                            */
                            mblk_rls = rfc_create_linestatus_cmdrsp(rfc_params,
                                                                    RFC_RSP,
                                                                    line_status_field,
                                                                    dlci_field);

                            /* Send to L2CAP.
                            */
                            L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                                              0,/* Indicates MBLK interface*/
                                              mblk_rls);

                        }
                        else
                        {
                            /* Frame errors are silently ignored.
                            */
                        }

                    }
                    else
                    {
                        /* RLS cmd received for an unknown channel. Create and
                           send DM frame to the peer to reject the connection.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          dlci,
                                          rfc_params->p_mux->info.mux.cid);
                    }
                }
                else
                {
                    /* This is a RLS response. The only action required is to
                       disable the response timer. The response itself is just
                       ignored.
                    */
                    rfc_find_chan_by_dlci(rfc_params, dlci);
                    if (NULL != rfc_params->p_dlc)
                    {
                        /* Cancel any pending timer.
                        */
                        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_RLS_CONTEXT]));
                    }
                    else
                    {
                        /* RLS rsp received for an unknown channel. Create and
                           send DM frame to the peer to reject the connection.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          dlci,
                                          rfc_params->p_mux->info.mux.cid);
                    }

                }

            }
            else
            {
                /* Invalid frame received. Nothing much we can do so just ignore
                   it!
                */
            }

            break;

        case RFC_NSC_TYPE:

            if (len == RFC_NSC_DATA_LEN)
            {
                CsrUint8 command;
                CsrUint8 cmd_res;

                /* Get the unsupported command details.
                */
                (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &command);

                cmd_res = (command >> 1) & 0x1;
                command = command >> 2;

                rfc_send_nsc_ind(rfc_params->p_mux->phandle,
                                 &(rfc_params->p_mux->info.mux.bd_addr),
                                 command,
                                 cmd_res);
            }
            else
            {
                /* Invalid frame received. Nothing much we can do so just ignore
                   it!
                */
            }

            break;
    }

}


/*! \brief Process data received from the peer

    \param rfc_params - Pointer to rfcomm instance data.
    \param data_len - length of the received data
*/
static void rfc_process_rx_data(RFC_CTRL_PARAMS_T *rfc_params,
                                CsrUint16 data_len)
{
    if (data_len > 0)
    {
        /* Determine where to send the data.
        */
        if (rfc_params->p_dlc->info.dlc.vtable->notify_fn(rfc_params))
        {
            /* Ensure MBLK, doesnt get destroyed below.
            */
            rfc_params->mblk = NULL;
        }
    }

    /* We may have been granted credits in the received data packet thus see if
       we can send any more data.
    */
    rfc_params->p_dlc->info.dlc.vtable->kick_fn(rfc_params, FALSE);
}

/*! \brief Parse a received frame

    This function decodes the rfcomm frame received within a l2cap dataread
    prim. It will only be invoked if the l2cap prim was received for a valid Mux
    channel (which will be set within the rfc_params strucuture).
    The appropriate action will be taken depending on the frame type. The
    received frame will be pointed to by the mblk field in the rfc params.

    \param rfc_params - Pointer to rfcomm instance data.
    \param mblk_len - length of the mblk containing the frame
*/
void rfc_parse_frame(RFC_CTRL_PARAMS_T *rfc_params,
                     CsrUint16        mblk_len)
{
    CsrUint8 fcs ;
    CsrUint16 data_len;
    CsrUint8  data_len_hi;
    CsrUint16 frame_len = RFC_MIN_FRAME_LEN;
    CsrUint8 dlci;
    CsrUint8 crbit;
    CsrUint8 credits;
    CsrUint8 frame_hdr[RFC_MIN_FRAME_HDR_LEN];
    RFC_LINK_DESIGNATOR from;

    /* Get the frame check sequence. This is the last byte in the data block.
    */
    CsrMblkReadTail(&(rfc_params->mblk), &fcs, 1);

    /* Get the minimum sized frame header.
    */
    (void)CsrMblkReadHead(&(rfc_params->mblk), frame_hdr, RFC_MIN_FRAME_HDR_LEN);
    data_len = (CsrUint16)frame_hdr[RFC_FRAME_LEN_OFFSET];

    if (!RFC_IS_EA_BIT_SET(data_len))
    {
        /* 16 bit length field thus read the next 8 bits and
           assemble the 16 bit value.
        */
        CsrBtMblkReadHead8(&(rfc_params->mblk), &data_len_hi);
        data_len |= ((CsrUint16)data_len_hi) << 8;
        frame_len ++;
    }

    /* Now remove the EA bit.
    */
    data_len = data_len >> 1;

    frame_len += data_len;

    /* Check that the EA bit is set in the address field.
    */
    if(RFC_IS_EA_BIT_SET(frame_hdr[RFC_FRAME_ADDR_OFFSET]))
    {
        crbit = RFC_GET_CRBIT_FROM_DATA_FIELD(frame_hdr[RFC_FRAME_ADDR_OFFSET]);
        dlci = RFC_GET_DLCI_FROM_ADDR(frame_hdr[RFC_FRAME_ADDR_OFFSET]);

        /* Calculate what the local dirbit is, the direction designator will
           be the inverse of that.
        */
        from = RFC_IS_INITIATOR(rfc_params->p_mux->flags) ? RFC_RESPONDER : RFC_INITIATOR;

        switch (frame_hdr[RFC_FRAME_CTRL_OFFSET])
        {
            case RFC_SABM:
                /* Validate the frame structure.
                */
                if (rfc_is_valid_frame_structure(
                       frame_len,
                       mblk_len,
                       RFC_CTRLFRAME_LEN,
                       crbit,
                       rfc_calc_crbit(from, RFC_CMD, RFC_FRAMELEVEL),
                       fcs,
                       rfc_frame_crc(frame_hdr ,RFC_CTRLFRAME_CRC_LEN)))
                {
                    if (dlci == 0)
                    {
                        CsrBool  incoming = TRUE;

                        rfc_params->p_dlc = rfc_params->p_mux;

                        /* This is an incoming connection so the 2nd parameter is
                           set to TRUE.
                        */
                        crank_channel_fsm(rfc_params, &incoming, RFC_EV_OPEN_IND_MUX);
                    }
                    else
                    {
                        rfc_find_chan_by_dlci(rfc_params, dlci);

                        if (NULL == rfc_params->p_dlc)
                        {
                            /* The remote side has sent a SABM to open a DLC
                               connection without having done any parameter
                               negotiation on this channel thus there wont be a
                               dlc set up yet. First check that the request is
                               for a valid server channel.
                            */
                            if (!rfc_validate_server_connect_req(rfc_params, dlci))
                            {
                                /* SABM received for an unknown channel. Send
                                   DM. Create and send DM frame to the peer to
                                   reject the connection.
                                */
                                rfc_send_dm_frame(rfc_params->p_mux->flags,
                                                  dlci,
                                                  rfc_params->p_mux->info.mux.cid);

                                return;
                            }
                            else
                            {
                                /* Cancel mux shutdown timer if it is running.
                                */
                                rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_DISCONNECT_CONTEXT]));

                                /* This SABM has initiated the channel thus
                                   reflect this in the start type.
                                */
                                rfc_params->p_dlc->info.dlc.start_type = RFC_START_SABM;
                                crank_channel_fsm(rfc_params, NULL, RFC_EV_SERVER_IND);
                            }
                        }
                        else
                        {
                            /* dlc structure already exists so channel may
                               have been initiated by a Parneg or Portneg
                               command.
                            */
                            crank_channel_fsm(rfc_params, NULL, RFC_EV_OPEN_IND_DLC);
                        }

                    }
                }
                break;

            case RFC_DISC:
                /* Validate the frame structure.
                */
                if (rfc_is_valid_frame_structure(
                       frame_len,
                       mblk_len,
                       RFC_CTRLFRAME_LEN,
                       crbit,
                       rfc_calc_crbit(from, RFC_CMD, RFC_FRAMELEVEL),
                       fcs,
                       rfc_frame_crc(frame_hdr ,RFC_CTRLFRAME_CRC_LEN)))
                {
                    rfc_find_chan_by_dlci(rfc_params, dlci);

                    if (rfc_params->p_dlc != NULL)
                    {
                        crank_channel_fsm(rfc_params, NULL, RFC_EV_DISCONNECT_IND);
                    }
                    else
                    {
                        /* No matching channel found for the specified dlci, so
                           respond with a DM frame on that dlci.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          dlci,
                                          rfc_params->p_mux->info.mux.cid);
                    }
                }
                else
                {
                    /* Invalid frame received, there's nothing much we can do
                       except just ignore it.
                    */
                }

                break;

            case RFC_UA:
                /* Validate the frame structure.
                */
                if (rfc_is_valid_frame_structure(
                       frame_len,
                       mblk_len,
                       RFC_CTRLFRAME_LEN,
                       crbit,
                       rfc_calc_crbit(from, RFC_RSP, RFC_FRAMELEVEL),
                       fcs,
                       rfc_frame_crc(frame_hdr ,RFC_CTRLFRAME_CRC_LEN)))
                {
                    /* find which channel the UA was received for...
                    */
                    rfc_find_chan_by_dlci(rfc_params, dlci);

                    if (rfc_params->p_dlc != NULL)
                    {
                        fsm_event_t event = RFC_EV_ACK_CFM;

                        /* Cancel any pending timer. Could be for a connect or
                           disconnect.
                        */
                        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_CONNECT_CONTEXT]));
                        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_DISCONNECT_CONTEXT]));

                        if (rfc_params->p_dlc->dlci == 0)
                        {
                            event = RFC_EV_ACK_CFM_MUX;
                        }
                        else
                        {
                            if (RFC_IS_DISCONNECTING(rfc_params->p_dlc->flags))
                            {
                                event = RFC_EV_ACK_CFM_DISC;
                            }
                        }

                        crank_channel_fsm(rfc_params, NULL, event);
                    }
                    else
                    {
                        /* No matching channel found for the specified dlci thus
                           just ignore this response.
                        */
                    }
                }
                else
                {
                    /* Invalid format frame, not much we can do except ignore
                       it.
                    */
                }

                break;

            case RFC_DM:
            case RFC_DM_PF:
                /* Validate the frame structure.
                */
                if (rfc_is_valid_frame_structure(
                       frame_len,
                       mblk_len,
                       RFC_CTRLFRAME_LEN,
                       crbit,
                       rfc_calc_crbit(from, RFC_RSP, RFC_FRAMELEVEL),
                       fcs,
                       rfc_frame_crc(frame_hdr ,RFC_CTRLFRAME_CRC_LEN)))
                {
                    /* find which channel the DM was received for...
                    */
                    rfc_find_chan_by_dlci(rfc_params, dlci);

                    if (rfc_params->p_dlc != NULL)
                    {
                        crank_channel_fsm(rfc_params, NULL, RFC_EV_DISC_MODE_CFM);
                    }
                    else
                    {
                        /* The peer device has sent a DM frame for an unknown
                           dlci. This implies they dont know about that dlci and
                           neither do we so just ignore it.
                        */

                    }
                }
                break;

            case RFC_UIH:

                /* If an invalid data frame is received then it is just
                   ignored.
                */
                if (dlci == 0)
                {
                    /* Validate the frame structure.
                    */
                    if (rfc_is_valid_data_frame(
                           frame_len,
                           mblk_len,
                           frame_len,
                           crbit,
                           rfc_calc_crbit(from, RFC_DATA, RFC_FRAMELEVEL),
                           fcs,
                           (CsrUint8)RFC_FCS_UIH(rfc_params->p_mux->fcs_in),
                           frame_hdr))
                    {
                        /* This is a Mux command/response frame.
                        */
                        rfc_parse_mux_cmdrsp(rfc_params);
                    }
                }
                else
                {
                    /* find which channel the data frame was received on...
                    */
                    rfc_find_chan_by_dlci(rfc_params, dlci);
                    if (rfc_params->p_dlc != NULL)
                    {
                        /* If a data frame is received before the MSC
                           initialisation is complete we assume a badly
                           behaving peer and for IOP sake early terminate the
                           MSC synchronisation.
                        */
                        if (!RFC_IS_MSC_COMPLETE(rfc_params->p_dlc->flags))
                        {
                            crank_channel_fsm(rfc_params, NULL, RFC_EV_MSC_COMPLETE);
                        }

                        /* This is a pure data frame.
                        */
                        if (rfc_is_valid_data_frame(
                                frame_len,
                                mblk_len,
                                frame_len,
                                crbit,
                                rfc_calc_crbit(from, RFC_DATA, RFC_FRAMELEVEL),
                                fcs,
                                (CsrUint8)RFC_FCS_UIH(rfc_params->p_dlc->fcs_in),
                                frame_hdr))
                        {
                            rfc_process_rx_data(rfc_params, data_len);
                        }
                        else
                        {
                            /* This is an invalid data frame, there is not
                               much we can do except ignore it!
                            */
                        }
                    }
                    else
                    {
                        /* No matching channel found for the specified dlci, so
                           respond with a DM frame on that dlci.
                        */
                        rfc_send_dm_frame(rfc_params->p_mux->flags,
                                          dlci,
                                          rfc_params->p_mux->info.mux.cid);
                    }
                }
                break;

            case RFC_UIH_PF:

                /* If an invalid data frame is received then it is just
                   ignored.
                */

                /* Credit based flow control is being used. Read the credit
                   field.
                */
                (void)CsrBtMblkReadHead8(&(rfc_params->mblk), &credits);
                frame_len ++;

                /* find which channel the data frame was received on...
                */
                rfc_find_chan_by_dlci(rfc_params, dlci);
                if (rfc_params->p_dlc != NULL)
                {
                    /* If a data frame is received before the MSC
                       initialisation is complete we assume a badly
                       behaving peer and for IOP sake early terminate the
                       MSC synchronisation.
                    */
                    if (!RFC_IS_MSC_COMPLETE(rfc_params->p_dlc->flags))
                    {
                        crank_channel_fsm(rfc_params, NULL, RFC_EV_MSC_COMPLETE);
                    }

                    /* Validate the frame structure.
                    */
                    if (rfc_is_valid_data_frame(
                           frame_len,
                           mblk_len,
                           frame_len,
                           crbit,
                           rfc_calc_crbit(from, RFC_DATA, RFC_FRAMELEVEL),
                           fcs,
                           (CsrUint8)RFC_FCS_UIH_PF(rfc_params->p_dlc->fcs_in),
                           frame_hdr))
                    {
                        /* Check for valid credit data frame type.
                        */
                        if (dlci != 0 && RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
                        {
                            /* This could purely be a credit frame or it could
                               also contain real data.
                            */
                            rfc_params->p_dlc->info.dlc.tx_credits += credits;

                            rfc_process_rx_data(rfc_params, data_len);
                        }
                    }
                }
                else
                {
                    /* No matching channel found for the specified dlci, so
                       respond with a DM frame on that dlci.
                    */
                    rfc_send_dm_frame(rfc_params->p_mux->flags,
                                      dlci,
                                      rfc_params->p_mux->info.mux.cid);
                }

                break;

            default:
                /* Invalid RFCOMM frame type received, just ignore it.
                */
               break;
        }
    }

    CsrMblkDestroy(rfc_params->mblk);
}


#endif /* INSTALL_RFCOMM_MODULE */
