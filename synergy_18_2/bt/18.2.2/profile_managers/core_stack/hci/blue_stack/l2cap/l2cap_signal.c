/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_signal.c

\brief  This file implements functions to create L2CAP signals.
*/

#include "csr_synergy.h"

#include "csr_util.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_control.h"
#include "l2cap_cid.h"

/*! \brief Create signal

    This function creates a new signal block including the data block.
    A signal comprises of two memory block, the first is a control block used
    for management of the signal in the L2CAP layer, the other block is the
    data block, this contain the actual payload that is sent over the ACL.

    \param local_cid Local CID that this signal is associated with.
    \param signal_size Size of signal data.
    \param signal_type Signal type.
    \param signal_id ID of this signal.
    \param count Number of subsequent CsrUint16 arguments to be added to signal.
    \return Pointer to signal.
*/
SIG_SIGNAL_T *SIG_SignalCreate(l2ca_cid_t local_cid,
                               CsrUint16 signal_size,
                               CsrUint8 signal_type,
                               l2ca_identifier_t signal_id,
                               unsigned int count,
                               ...)
{
    SIG_SIGNAL_T *sig = zpnew(SIG_SIGNAL_T);
    CsrUint8 *data;
    va_list ap;

    /* We might consider allowing this to fail softly. But if we don't have
       enough memory to create a signal then the game is probably up. */
    if ((sig->signal = CsrMblkMallocCreate((void**)&data,
            (CsrMblkSize)(L2CAP_SIZEOF_SIGNAL_HEADER + signal_size))) == NULL)
        BLUESTACK_PANIC(CSR_BT_PANIC_HEAP_EXHAUSTION);

    CsrMemSet(data, 0, L2CAP_SIZEOF_SIGNAL_HEADER + signal_size);

    sig->signal_type = signal_type;
    sig->signal_id = signal_id;

    /* SIGNAL_COMMAND_REJECT has zeroed timer counts.
       All other signals take the defaults. */
    if (signal_type != SIGNAL_COMMAND_REJECT)
    {
        sig->rtx_timer_count = L2CAP_RTX_RETRIES;
        sig->ertx_timer_count = L2CAP_ERTX_RETRIES;
    }
    sig->local_cid = local_cid;
    sig->p_handle = CSR_SCHED_QID_INVALID;

    write_uints(&data,
                URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
                signal_type,
                signal_id,
                signal_size);

    va_start(ap, count);

    while (count-- != 0)
        write_uint16(&data, (CsrUint16)va_arg(ap, unsigned int));

    va_end(ap);

    return sig;
}

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
/*! \brief Create connect request signal

    This function creates a new connect request signal.
    \param local_cid Local CID.
    \param psm Remote PSM.
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateConnectReq(l2ca_cid_t local_cid, psm_t psm)
{
    return SIG_SignalCreate(local_cid,
                            SIGNAL_CONNECT_REQ_MIN_LENGTH,
                            SIGNAL_CONNECT_REQ,
                            0,
                            2,
                            psm,
                            local_cid);
}

/*! \brief Create connect response signal

    This function creates a new connect response signal.
    \param dest_cid Destination CID (Local).
    \param src_cid Source CID (Remote).
    \param result Connection result.
    \param status Current connection status.
    \param identifier Signal ID from connect request.
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateConnectRsp(l2ca_cid_t dest_cid,
                                         l2ca_cid_t src_cid,
                                         CsrUint16 result,
                                         CsrUint16 status,
                                         l2ca_identifier_t identifier)
{
    return SIG_SignalCreate(dest_cid,
                            SIGNAL_CONNECT_RES_MIN_LENGTH,
                            SIGNAL_CONNECT_RES,
                            identifier,
                            4,
                            dest_cid,
                            src_cid,
                            result,
                            status);
}

SIG_SIGNAL_T *SIG_SignalCreateConfigSignal(struct cidtag *cidcb,
                                           CsrUint16 length,
                                           const CsrUint8 *config,
                                           CsrBool more,
                                           l2ca_identifier_t identifier,
                                           l2ca_conf_result_t result)
{
    SIG_SIGNAL_T *sig;
    CsrUint8 *data;
    /* Set parameters for response signal */
    CsrUint16 signal_min_length = SIGNAL_CONFIG_RES_MIN_LENGTH;
    CsrUint8 signal_type = SIGNAL_CONFIG_RES;

    if (identifier == 0)
    {
        /* Set parameters for request signal */
        signal_min_length = SIGNAL_CONFIG_REQ_MIN_LENGTH;
        signal_type = SIGNAL_CONFIG_REQ;
    }

    sig = SIG_SignalCreate(cidcb->local_cid,
                           (CsrUint16)(signal_min_length + length),
                           signal_type,
                           identifier,
                           2,
                           cidcb->remote_cid,
                           (CsrUint16)(more ? 0x0001 : 0x0000));

    /* Point to just beyond what has been written so far. It doesn't
       matter that 'length' might be 2 less than it should be (because
       of the 'result' field that is written just below this). The
       length field is just used to do range-checking, which we'll have
       to make do without. */
    data = CsrMblkMap(sig->signal,
                    ((CsrMblkSize)L2CAP_SIZEOF_SIGNAL_HEADER + 4),
                    (CsrMblkSize)length);

    /* Extra field in response primitive */
    if (identifier != 0)
        write_uint16(&data, (CsrUint16)result);

    /* Write the options to the signal */
    if (length > 0)
        CsrMemCpy(data, config, length);

    CsrMblkUnmap(sig->signal, data);
    return sig;
}

/*! \brief Create disconnect request signal

    This function creates a new disconnect request signal.
    \param local_cid Local CID.
    \param remote_cid Remote CID.
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateDisconnectReq(l2ca_cid_t remote_cid, l2ca_cid_t local_cid)
{
    return SIG_SignalCreate(local_cid,
                            SIGNAL_DISCONNECT_REQ_MIN_LENGTH,
                            SIGNAL_DISCONNECT_REQ,
                            0,
                            2,
                            remote_cid,
                            local_cid);
}

/*! \brief Create disconnect response signal

    This function creates a new disconnect response signal.
    \param dest_cid Destination CID (Local).
    \param src_cid Source CID (Remote).
    \param identifier Signal ID from disconnect request.
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateDisconnectRsp(l2ca_cid_t dest_cid,
                                            l2ca_cid_t src_cid,
                                            l2ca_identifier_t identifier)
{
    return SIG_SignalCreate(dest_cid,
                            SIGNAL_DISCONNECT_RES_MIN_LENGTH,
                            SIGNAL_DISCONNECT_RES,
                            identifier,
                            2,
                            dest_cid,
                            src_cid);
}

/*! \brief Copy from CsrUint8 array into signal.

    This function is a CsrMemCpy() for signals. If there is data to copy
    then we copy it from the supplied data pointer at the given offset
    in the signal MBLK.

    \param signal Pointer to signal MBLK to recieve data.
    \param offset Offset, from start of signal, to which we start copying.
    \param length Length of data to copy.
    \param data Pointer to data to be copied.
*/
static void SIG_CopyDataToSignal(CsrMblk *signal,
                          CsrUint16 offset,
                          CsrUint16 length,
                          const CsrUint8 *data)
{
    if (data != NULL && length != 0)
    {
        CsrUint8 *map = CsrMblkMap(signal, offset, length);

        if (map != NULL)
            CsrMemCpy(map, data, length);

        CsrMblkUnmap(signal, map);
    }
}

/*! \brief Create echo request signal

    This function creates a new echo request signal.

    \param p_handle Handle to requester, used to send response back to.
    \param data Pointer to payload.
    \param length Size of payload.
    \param req_ctx Context number to be returned in CFM
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateEchoReq(CsrSchedQid p_handle,
                                      const CsrUint8 *data,
                                      CsrUint16 length,
                                      CsrUint16 req_ctx)
{
    SIG_SIGNAL_T *sig = SIG_SignalCreate(
            req_ctx,
            (CsrUint16)(SIGNAL_ECHO_REQ_MIN_LENGTH + length),
            SIGNAL_ECHO_REQ,
            0,
            0);

    /* Store phandle */
    sig->p_handle = p_handle;

    /* Copy data into signal */
    SIG_CopyDataToSignal(sig->signal, L2CAP_SIZEOF_SIGNAL_HEADER, length, data);

    return sig;
}

/*! \brief Create get info request signal

    This function creates a new get info request signal.
    \param p_handle Handle to requester, used to send response back to.
    \param info_type Information type requested.
    \param req_ctx Context number to be returned in CFM
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateGetInfoReq(CsrSchedQid p_handle,
                                         CsrUint16 info_type,
                                         CsrUint16 req_ctx)
{
    SIG_SIGNAL_T *sig =  SIG_SignalCreate(req_ctx,
                                          SIGNAL_INFO_REQ_MIN_LENGTH,
                                          SIGNAL_INFO_REQ,
                                          0,
                                          1,
                                          info_type);
    /* Store phandle */
    sig->p_handle = p_handle;

    return sig;
}

/*! \brief Create echo response signal

    This function creates a new echo response signal.
    \param identifier Signal ID from request signal.
    \param data Payload from echo request.
    \param length Size of payload.
    \return Pointer to signal control block or NULL if creation failed.
*/
SIG_SIGNAL_T *SIG_SignalCreateEchoRsp(l2ca_identifier_t identifier, const CsrUint8 *data, CsrUint16 length)
{
    SIG_SIGNAL_T *sig =  SIG_SignalCreate(
            L2CA_CID_INVALID,
            (CsrUint16)(SIGNAL_ECHO_RES_MIN_LENGTH + length),
            SIGNAL_ECHO_RES,
            identifier,
            0);

    /* Copy data into signal */
    SIG_CopyDataToSignal(sig->signal, L2CAP_SIZEOF_SIGNAL_HEADER, length, data);

    return sig;
}

/*! \brief Create get info response signal

    This function creates a new get info response signal.
    \param identifier Signal ID from request signal.
    \param info_type Information type requested.
    \return Pointer to signal control block or NULL if creation failed.
*/
#define CSR_BT 0x4CBA
#define CSR_BT_LOW 0xACB
#define CSR_BT_HIGH 0xA
SIG_SIGNAL_T *SIG_SignalCreateGetInfoRsp(l2ca_identifier_t identifier,
                                         CsrUint16 info_type)
{
    SIG_SIGNAL_T *sig;
    CsrUint8 *sig_data_ptr;
    CsrUint8 **sig_data_ptr_ptr = &sig_data_ptr;
    CsrUint16 length;
    CsrUint16 result = 0;
    l2ca_ext_feats_t feat = 0;
#if defined(INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT) \
    || defined(INSTALL_L2CAP_ENHANCED_SUPPORT)
    CsrUint16 mode_mask = 0;
#endif

    switch (info_type)
    {

        case L2CA_GETINFO_TYPE_MTU:
            /* Connectionless MTU */
            length = 2;
            break;
#ifdef BUILD_FOR_HOST
        case CSR_BT:
            /* CSR BCHS extended features */
            /* fallthrough */
#endif
        case L2CA_GETINFO_TYPE_EXT_FEAT:
            /* L2CAP extended feature mask (1.2) */
            length = 4;
            break;
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
        case L2CA_GETINFO_TYPE_FIX_CH:
            /* Fixed channel support */
            length = 8;
            break;
#endif
        default:
            /* Not supported */
            length = 0;
            result = 0x0001;
    }

    sig = SIG_SignalCreate(L2CA_CID_INVALID,
                           (CsrUint16)(SIGNAL_INFO_RES_MIN_LENGTH + length),
                           SIGNAL_INFO_RES,
                           identifier,
                           2,
                           info_type,
                           result);

    sig_data_ptr = CsrMblkMap(sig->signal,
                            (CsrMblkSize)(L2CAP_SIZEOF_SIGNAL_HEADER
                                    + SIGNAL_INFO_RES_MIN_LENGTH),
                            (CsrMblkSize)length);

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
    {
        FIXCID_T *fch;

        /* Walk list of fixed channels */
        for(fch = mcb.fixcid_list; fch != NULL; fch = fch->next_ptr)
        {
            /* Fill in flow mode in mode_mask. */
            if(fch->config.flow != NULL
                    && (fch->config.options & L2CA_SPECIFY_FLOW) != 0)
            {
                mode_mask |= (1 << fch->config.flow->mode);
            }

            /* Fill in fixed channel support if requested. */
            if (info_type == L2CA_GETINFO_TYPE_FIX_CH)
                sig_data_ptr[(fch->cid >> 3)] |= 1 << (fch->cid & 7);
        }
    }
#endif

    switch (info_type)
    {
        case L2CA_GETINFO_TYPE_MTU:
            /* Connectionless MTU size */
            write_uint16(sig_data_ptr_ptr, L2CAP_CL_MTU);
            break;

        case L2CA_GETINFO_TYPE_EXT_FEAT:
        {
            CsrUint32 feat32;

            /* Macros conditionally defined for what is actually supported. */
            feat |= (L2CA_EXT_FEAT_SUPPORTED_BIDIR_QOS
                    | L2CA_EXT_FEAT_SUPPORTED_EXT_FLOW_SPEC
                    | L2CA_EXT_FEAT_SUPPORTED_EXT_WIN_SIZE
                    | L2CA_EXT_FEAT_SUPPORTED_FIXED_CH);

            /* Check per-registration features. */
#if defined(INSTALL_L2CAP_ENHANCED_SUPPORT) \
            || defined(INSTALL_L2CAP_UCD_SUPPORT)
            {
                PSM_T *psm_ptr;
                for (psm_ptr = mcb.psm_list;
                     psm_ptr != NULL;
                     psm_ptr = psm_ptr->next_ptr)
                {
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                    /* Enhanced L2CAP modes */
                    mode_mask |= psm_ptr->mode_mask;
#endif
#ifdef INSTALL_L2CAP_UCD_SUPPORT
                    /* UCD reception */
                    if ((psm_ptr->regflags & L2CA_REGFLAG_ALLOW_RX_UCD) != 0)
                        feat |= L2CA_EXT_FEAT_UCD_RECEPTION;
#endif
                }
            }

            /* Calculate the features mask. Make sure that legacy modes
             * are prohibited. */
            mode_mask &= ~(L2CA_MODE_MASK_RTM_OBSOLETE |
                           L2CA_MODE_MASK_FC_OBSOLETE);
            if (mode_mask & L2CA_MODE_MASK_ENHANCED_RETRANS)
                feat |= (L2CA_EXT_FEAT_ENH_RETRANS | L2CA_EXT_FEAT_OPT_FCS);
            if (mode_mask & L2CA_MODE_MASK_STREAMING)
                feat |= (L2CA_EXT_FEAT_STREAMING | L2CA_EXT_FEAT_OPT_FCS);
#endif

            /* Fill out the response octets */
            feat32 = feat;
            write_uint32(sig_data_ptr_ptr, &feat32);
            break;
        }

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
        case L2CA_GETINFO_TYPE_FIX_CH:
            sig_data_ptr[0] |= (1 << L2CA_CID_SIGNAL)   /* BR/EDR Signalling */
#if defined(INSTALL_L2CAP_CONNLESS_SUPPORT) \
            || defined(INSTALL_L2CAP_UCD_SUPPORT)
                | (1 << L2CA_CID_CONNECTIONLESS)        /* Connectionless/UCD */
#endif
#ifdef INSTALL_ULP
                | (1 << L2CA_CID_LE_SIGNAL)             /* BLE Signalling */
#endif
                ;
            break;
#endif
#ifdef BUILD_FOR_HOST
        case CSR_BT:
            /* CSR BCHS extended features */
        {
            CsrUint32 value = CSR_BT_HIGH;
            value = (value << 28) | (CSR_BT<<12) | CSR_BT_LOW;
            write_uint32(sig_data_ptr_ptr, &value);
            break;
        }
#endif
    }

    CsrMblkUnmap(sig->signal, sig_data_ptr);
    return sig;
}

#endif
/*! \brief AMP create channel request signal

    This function creates a new AMP create channel request
    \param local_cid Local CID.
    \param psm Remote PSM.
    \param control AMP controller ID
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalCreateChannelReq(l2ca_cid_t local_cid,
                                         psm_t psm,
                                         l2ca_controller_t control)
{
    CsrUint8 *data;

    SIG_SIGNAL_T *sig = SIG_SignalCreate(local_cid,
                                         SIGNAL_CREATE_CHANNEL_REQ_MIN_LENGTH,
                                         SIGNAL_CREATE_CHANNEL_REQ,
                                         0, /* identifier */
                                         2,
                                         psm,
                                         local_cid);

    data = CsrMblkMap(sig->signal,
                    (CsrMblkSize)(L2CAP_SIZEOF_SIGNAL_HEADER + 4),
                    1);

    *data = (CsrUint8)(control & 0xff);

    CsrMblkUnmap(sig->signal, data);
    return sig;
}
#endif

/*! \brief AMP create channel response signal

    This function creates a new connect response signal.
    \param dest_cid Destination CID (Local).
    \param src_cid Source CID (Remote).
    \param result Connection result.
    \param status Current connection status.
    \param identifier Signal ID from connect request.
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalCreateChannelRsp(l2ca_cid_t dest_cid,
                                         l2ca_cid_t src_cid,
                                         CsrUint16 result,
                                         CsrUint16 status,
                                         l2ca_identifier_t identifier)
{
    return SIG_SignalCreate(dest_cid,
                            SIGNAL_CREATE_CHANNEL_RES_MIN_LENGTH,
                            SIGNAL_CREATE_CHANNEL_RES,
                            identifier,
                            4,
                            dest_cid,
                            src_cid,
                            result,
                            status);
}
#endif

/*! \brief AMP move channel request signal

    This function creates a new AMP create channel request
    \param local_cid Local CID.
    \param control AMP controller ID
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalMoveChannelReq(l2ca_cid_t local_cid,
                                       l2ca_controller_t control)
{
    CsrUint8 *data;

    SIG_SIGNAL_T *sig = SIG_SignalCreate(local_cid,
                                         SIGNAL_MOVE_CHANNEL_REQ_MIN_LENGTH,
                                         SIGNAL_MOVE_CHANNEL_REQ,
                                         0, /* identifier */
                                         1,
                                         local_cid);

    data = CsrMblkMap(sig->signal,
                    (CsrMblkSize)(L2CAP_SIZEOF_SIGNAL_HEADER + 2),
                    1);

    *data = (CsrUint8)(control & 0xff);

    CsrMblkUnmap(sig->signal, data);
    return sig;
}
#endif

/*! \brief AMP move channel response signal

    This function creates a new connect response signal.
    \param local_cid Local CID sending the signal
    \param src_cid Source CID (Remote).
    \param result Connection result.
    \param identifier Signal ID from connect request.
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalMoveChannelRsp(l2ca_cid_t local_cid,
                                       l2ca_cid_t src_cid,
                                       CsrUint16 result,
                                       l2ca_identifier_t identifier)
{
    return SIG_SignalCreate(local_cid,
                            SIGNAL_MOVE_CHANNEL_RES_MIN_LENGTH,
                            SIGNAL_MOVE_CHANNEL_RES,
                            identifier,
                            2,
                            src_cid,
                            result);
}
#endif

/*! \brief AMP move channel confirm request signal

    This function creates a new connect response signal.
    \param src_cid Source CID (Remote).
    \param result Connection result.
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalMoveChannelConfirmReq(l2ca_cid_t local_cid,
                                              CsrUint16 result)
{
    return SIG_SignalCreate(local_cid,
                            SIGNAL_MOVE_CONFIRM_REQ_MIN_LENGTH,
                            SIGNAL_MOVE_CONFIRM_REQ,
                            0, /* identifier */
                            2,
                            local_cid,
                            result);
}
#endif

/*! \brief AMP move channel confirm response signal

    This function creates a new connect response signal.
    \param local_cid Local CID sending the signal
    \param src_cid Source CID (Remote).
    \param identifier Signal ID from connect request.
    \return Pointer to signal control block or NULL if creation failed.
*/
#ifdef INSTALL_AMP_SUPPORT
SIG_SIGNAL_T *SIG_SignalMoveChannelConfirmRsp(l2ca_cid_t local_cid,
                                              l2ca_cid_t src_cid,
                                              l2ca_identifier_t identifier)
{
    return SIG_SignalCreate(local_cid,
                            SIGNAL_MOVE_CONFIRM_RES_MIN_LENGTH,
                            SIGNAL_MOVE_CONFIRM_RES,
                            identifier,
                            1,
                            src_cid);
}
#endif

/*! \brief Low energy connection paramter update request signal

    This function creates the connection parameter update request signal.
    \param local_cid Local CID sending the signal
    \param min_interval Interval minimum in slots (1.25ms)
    \param max_interval Interval maximum in slots (1.25ms)
    \param latency Latency in LL events
    \param timeout Timeout in units of 10ms
*/
#ifdef INSTALL_ULP
SIG_SIGNAL_T *SIG_SignalConnParamUpdateReq(CsrUint16 min_interval, CsrUint16 max_interval,
                                           CsrUint16 latency, CsrUint16 timeout)
{
    return SIG_SignalCreate(L2CA_CID_INVALID,
                            SIGNAL_CONN_PARAM_UPDATE_REQ_MIN_LENGTH,
                            SIGNAL_CONN_PARAM_UPDATE_REQ,
                            0,
                            4,
                            min_interval,
                            max_interval,
                            latency,
                            timeout);
}
#endif

/*! \brief Low energy connection parameter update response signal

    This function creates the connection parameter update response signal.
    \param identifier L2CAP signal identifier
    \param status Signal status code
*/
#ifdef INSTALL_ULP
SIG_SIGNAL_T *SIG_SignalConnParamUpdateRsp(l2ca_identifier_t identifier, CsrUint16 status)
{
    return SIG_SignalCreate(L2CA_CID_INVALID,
                            SIGNAL_CONN_PARAM_UPDATE_RES_MIN_LENGTH,
                            SIGNAL_CONN_PARAM_UPDATE_RES,
                            identifier,
                            1,
                            status);
}
#endif

/*! \brief See whether an option is enabled and/or hinted.

    \param config Pointer to L2CAP configuration.
    \param option L2CAP option specifier.
    \returns 0 if the specified option is not present, option if the
        option is present, or option | L2CA_OPTION_HINT if the option is
        hinted.
 */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static unsigned int SIG_Try_Option(L2CA_CONFIG_T *config, l2ca_options_t option)
{
    l2ca_options_t mask = L2CA_OPTION_MASK(option);

    /* Set the 'hint' option if the option is hinted. */
    if ((config->hints & mask) != 0)
        option |= L2CA_OPTION_HINT;

    /* If the option isn't there at all then we return zero. */
    if ((config->options & mask) == 0)
        option = 0;

    return option;
}
#endif

/*! \brief Serialise a BlueStack configuration parameter

    This function serialises a BlueStack L2CAP configuration option parameter
    into the on-wire/on-air format.
    \param buf pointer to buffer-pointer where data will be written. Auto-incremented.
    \param config Pointer to configuration structure
    \return TRUE if successfull, FALSE otherwise
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
CsrBool SIG_ConfigSerialise(CsrUint8 **buf,
                           L2CA_CONFIG_T *config)
{
    unsigned int option;

    if ((option = SIG_Try_Option(config, L2CA_OPTION_MTU)) != 0)
    {
        write_uints(buf,
                    URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_MTU,
                    config->mtu);
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_FLUSH)) != 0)
    {
        write_uints(buf,
                    URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_FLUSH,
                    SIG_FlushToH2L(config->flush_to));
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_QOS)) != 0)
    {
        L2CA_QOS_T *qos = config->qos;

        write_uints(buf,
                    URW_FORMAT(CsrUint8, 4, CsrUint32, 5, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_QOS,
                    qos->flags,
                    qos->service_type,
                    &qos->token_rate,
                    &qos->token_bucket,
                    &qos->peak_bw,
                    &qos->latency,
                    &qos->delay_var);
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_FLOW)) != 0)
    {
        L2CA_FLOW_T *flow = config->flow;

        write_uints(buf,
                    URW_FORMAT(CsrUint8, 5, CsrUint16, 3, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_FLOW,
                    flow->mode,
                    flow->tx_window,
                    flow->max_retransmit,
                    flow->retrans_timeout,
                    flow->monitor_timeout,
                    flow->maximum_pdu);
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_FCS)) != 0)
    {
        write_uints(buf,
                    URW_FORMAT(CsrUint8, 3, UNDEFINED, 0, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_FCS,
                    config->fcs);
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_FLOWSPEC)) != 0)
    {
        L2CA_FLOWSPEC_T *flowspec = config->flowspec;

        write_uints(buf,
                    URW_FORMAT(CsrUint8, 4, CsrUint16, 1, CsrUint32, 3),
                    option,
                    L2CAP_SIZEOF_OPTION_FLOWSPEC,
                    flowspec->fs_identifier,
                    flowspec->fs_service_type,
                    (CsrUint16)flowspec->fs_max_sdu,
                    &flowspec->fs_interarrival,
                    &flowspec->fs_latency,
                    &flowspec->fs_flush_to);
    }

    if ((option = SIG_Try_Option(config, L2CA_OPTION_EXT_WINDOW)) != 0)
    {
        write_uints(buf,
                    URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
                    option,
                    L2CAP_SIZEOF_OPTION_EXT_WINDOW,
                    config->ext_window);
    }

    if(config->unknown_length > 0)
    {
        CsrMemCpy(*buf, config->unknown, config->unknown_length);
        *buf += config->unknown_length;
    }

    return TRUE;
}


/*! \brief Append an "unknown" L2CAP configuration option to the
           configuration block

    Only the "config" block will be modified.
*/
static void SIG_ConfigAppendUnknown(const CsrUint8 *buf,
                                    CsrUint8 length,
                                    CsrUint8 type,
                                    L2CA_CONFIG_T *config)
{
    CsrUint8 newlen;
    CsrUint8 *new_config;

    /* Re-allocate */
    newlen = config->unknown_length + length + 2;
    new_config = bpxprealloc(config->unknown,
                             config->unknown_length, /* old size */
                             newlen); /* new size */
    if(new_config == NULL)
    {
        /* Reallocation failed, but original block untouched */
        return;
    }

    /* Append new stuff including type and length fields */
    CsrMemCpy(new_config + config->unknown_length,      &type,    1);
    CsrMemCpy(new_config + config->unknown_length + 1,  &length,  1);
    CsrMemCpy(new_config + config->unknown_length + 2,  buf,      length);
    config->unknown        = new_config;
    config->unknown_length = newlen;
}

#endif
/*! \brief Update options/hint mask
 */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static CsrBool SIG_PutOption(const CsrUint8 **p_buf,
                             L2CA_CONFIG_T *config,
                             CsrUint8 length_expected)
{
    const CsrUint8 *buf = *p_buf;
    CsrUint8 type = *(buf - 2);
    CsrUint8 length = *(buf - 1);
    l2ca_options_t mask;

    /* Check that length matches */
    if(length != length_expected)
        return FALSE;

    /* Write to options and hints mask. */
    mask = L2CA_OPTION_MASK((type & ~L2CA_OPTION_HINT));
    config->options |= mask;
    if (type & L2CA_OPTION_HINT)
        config->hints |= mask;

    return TRUE;
}
#endif

/*! \brief Parallelise a BlueStack configuration parameter

    This function fills out a BlueStack L2CAP configuration option parameter
    from the on-wire byte stream format.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
CsrBool SIG_ConfigParallelise(const CsrUint8 **buf,
                             CsrUint16 *size,
                             L2CA_CONFIG_T *config)
{
    L2CA_QOS_T *qos;
    L2CA_FLOW_T *flow;
    L2CA_FLOWSPEC_T *flowspec;
    CsrUint8 length;
    CsrUint8 type;

    /* Sanity check */
    if (*size < 2 || (length = (*buf)[1]) > (*size -= 2))
        return FALSE;

    *size -= length;

    /* Ok, decode configuration */
    type = (*buf)[0];

    *buf += 2;
    switch(type & ~L2CA_OPTION_HINT)
    {
        case L2CA_OPTION_MTU:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_MTU))
                return FALSE;

            config->mtu = read_uint16(buf);
            break;

        case L2CA_OPTION_FLUSH:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_FLUSH))
                return FALSE;

            config->flush_to = SIG_FlushToL2H(read_uint16(buf));
            break;

        case L2CA_OPTION_QOS:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_QOS))
                return FALSE;

            if(config->qos == NULL)
                config->qos = pnew(L2CA_QOS_T);

            qos = config->qos;
            read_uints(buf,
                       URW_FORMAT(CsrUint8, 2, CsrUint32, 5, UNDEFINED, 0),
                       &qos->flags,
                       &qos->service_type,
                       &qos->token_rate,
                       &qos->token_bucket,
                       &qos->peak_bw,
                       &qos->latency,
                       &qos->delay_var);
            break;

        case L2CA_OPTION_FLOW:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_FLOW))
                return FALSE;

            if(config->flow == NULL)
                config->flow = pnew(L2CA_FLOW_T);

            flow = config->flow;
            read_uints(buf,
                       URW_FORMAT(CsrUint8, 3, CsrUint16, 3, UNDEFINED, 0),
                       &flow->mode,
                       &flow->tx_window,
                       &flow->max_retransmit,
                       &flow->retrans_timeout,
                       &flow->monitor_timeout,
                       &flow->maximum_pdu);
            break;

        case L2CA_OPTION_FCS:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_FCS))
                return FALSE;

            config->fcs = read_uint8(buf);
            break;

        case L2CA_OPTION_FLOWSPEC:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_FLOWSPEC))
                return FALSE;

            if(config->flowspec == NULL)
                config->flowspec = pnew(L2CA_FLOWSPEC_T);

            flowspec = config->flowspec;
            read_uints(buf,
                       URW_FORMAT(CsrUint8, 2, CsrUint16, 1, CsrUint32, 3),
                       &flowspec->fs_identifier,
                       &flowspec->fs_service_type,
                       &flowspec->fs_max_sdu,
                       &flowspec->fs_interarrival,
                       &flowspec->fs_latency,
                       &flowspec->fs_flush_to);
            break;

        case L2CA_OPTION_EXT_WINDOW:
            if (!SIG_PutOption(buf, config, L2CAP_SIZEOF_OPTION_EXT_WINDOW))
                return FALSE;

            config->ext_window = read_uint16(buf);
            break;

        default:
            /* Append the "raw" configuration */
            SIG_ConfigAppendUnknown(*buf, length, type, config);
            *buf += length;
            break;
    }

    /* Done */
    return TRUE;
}

/*! \brief Calculate length of on-air configuration options size

*/
CsrUint16 SIG_ConfigOptionsLength(L2CA_CONFIG_T *config)
{
    CsrUint16 size = config->unknown_length;
    l2ca_options_t options = config->options;

    if (options & L2CA_SPECIFY_MTU)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_MTU;
    }
    if (options & L2CA_SPECIFY_FLUSH)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_FLUSH;
    }
    if (options & L2CA_SPECIFY_QOS)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_QOS;
    }
    if (options & L2CA_SPECIFY_FLOW)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_FLOW;
    }
    if (options & L2CA_SPECIFY_FCS)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_FCS;
    }
    if (options & L2CA_SPECIFY_FLOWSPEC)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_FLOWSPEC;
    }
    if (options & L2CA_SPECIFY_EXT_WINDOW)
    {
        size += L2CAP_SIZEOF_OPTION_HEADER + L2CAP_SIZEOF_OPTION_EXT_WINDOW;
    }

    return size;
}

#endif

/*! \brief Convert HCI flush timeout units to L2CAP units (slots->ms)
 */
CsrUint16 SIG_FlushToH2L(CsrUint16 fto)
{
    CsrUint16 round;

    if (fto == FLUSH_INFINITE_TIMEOUT)
        return L2CA_FLUSH_TO_INFINITE;

    /* L2CAP timeout is in ms whereas HCI is in slots ie 0.625ms.
     * As we want to store the local flush timeout internally in
     * the HCI format the value needs to be converted.
     * Conversion is L2CAP_To/0.625 which is equivalent to
     * (L2CAP_To * 8)/5
     */
    round = (CsrUint16)((fto * 5 + 4) >> 3); /* The "+4" fixes rounding */
    return (round > 0 ? round : (CsrUint16)1);
}

/*! \brief Convert L2CAP flush timeout units to HCI units (ms->slots)
 */
CsrUint16 SIG_FlushToL2H(CsrUint16 fto)
{
    CsrUint16 round;

    if (fto == L2CA_FLUSH_TO_INFINITE)
        return FLUSH_INFINITE_TIMEOUT;

    /* With AMP automatic flush-timeout this clipping may not be valid
     * as we limit ourselves BD/EDRs maximum of 1279.375ms.  However,
     * we never pass up the BD/EDR flush-to for AMP enabled configs as
     * CONFIG_CFM/IND will pass through CID_FixupConfiguration first,
     * so this shouldn't be a problem. See the CID function for
     * further information. */
    if (fto >= L2CA_FLUSH_TO_MAX_HCI_LIMIT_MS)
        return HCI_MAX_FLUSH_TIMEOUT;

    /* L2CAP timeout is in ms whereas HCI is in slots ie 0.625ms.
     * As we want to store the local flush timeout internally in
     * the HCI format the value needs to be converted.
     * Conversion is L2CAP_To/0.625 which is equivalent to
     * (L2CAP_To * 8)/5. Make sure we round the correct way.
     */
    round = (CsrUint16)(((fto << 3) + 2) / 5); /* The "+2" fixes rounding */
    return (round > 0 ? round : (CsrUint16)1);
}


#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
/*! \brief Read Info Type field from GetInfo signal */
CsrUint16 SIG_ReadGetInfoType(const SIG_SIGNAL_T *sig_ptr)
{
    CsrUint8 *data;
    CsrUint16 info_type;

    /* Make sure that there is a signal and it's the right length. */
    if (sig_ptr->signal == NULL || CsrMblkGetLength(sig_ptr->signal) !=
            L2CAP_SIZEOF_SIGNAL_HEADER + SIGNAL_INFO_REQ_MIN_LENGTH)
        return L2CA_GETINFO_TYPE_UNKNOWN;

    data = CsrMblkMap(sig_ptr->signal,
                    (CsrMblkSize)L2CAP_SIZEOF_SIGNAL_HEADER,
                    2);
    info_type = read_uint16((const CsrUint8 **)&data);

    CsrMblkUnmap(sig_ptr->signal, data);
    return info_type;
}
#endif

/*! \brief Convert an extended flowspec flushtimeout into a HCI unit
 *         (us->slots)
 */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
CsrUint16 SIG_FlushToFS2H(CsrUint32 fto)
{
    /* Infinite */
    if(fto == L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
    {
        return FLUSH_INFINITE_TIMEOUT;
    }

    /* Immediate flush - never retry */
    if(fto == L2CA_FLOWSPEC_FLUSH_TO_IMMEDIATE)
    {
        return 1;
    }

    /* Use normal conversion */
    return SIG_FlushToL2H((CsrUint16)(fto / 1000));
}
#endif
