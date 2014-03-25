/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_types.h

\brief  This file contains internal structures used throughout L2CAP.
*/

#ifndef _L2CAP_TYPES_H
#define _L2CAP_TYPES_H

#include "csr_synergy.h"

#include "csr_bt_core_stack_fsm.h"
#include "l2cap_common.h"

#ifdef INSTALL_AMP_SUPPORT
#include "dm_prim.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief L2CAP sizes for data to be sent over HCI */
typedef struct
{
    CsrUint16 header;
    CsrUint16 body;
    CsrUint16 trailer;
} L2CAP_CH_DATA_SIZES_T;

/*! \brief Duplicate request signal detection

    This is a placeholder to deal with duplicate request signals. We
    need to store the last seen identifier, our latest response
    identifier and the actual response blob.
*/
typedef struct dupsigtag
{
    BITFIELD(CsrUint8,                   type, 8);       /*!< Signal Type */
    BITFIELD(CsrUint8,                   identifier, 8); /*!< Signal Identifier */
    CsrUint16                            length;         /*!< Signal Length (including header) */
    CsrMblk                              *sig_data;      /*!< Response signal data (if sent) */
} DUPSIG_T;

/*! \brief Transmit queue

    The generalised transmit queue used both by the BR/EDR path
    directly from the CHCB structure, and an AMP transmit queue.
*/
typedef struct champqtag
{
#ifdef INSTALL_AMP_SUPPORT
    struct champqtag                     *next_ptr;             /*!< Next AMP queue element */
    CsrUint8                               physical_handle;      /*!< AMP physical channel ID */
    amp_link_id_t                         logical_handle;       /*!< AMP logical channel ID */
    l2ca_controller_t                     controller_id;        /*!< AMP local controller ID */
    CsrSchedQid                             hci_data_queue;       /*!< PAL scheduler queue */

    /* Number of credits currently consumed by this queue and the total number
       that we are allowed to consume. In the absence of AMP, we assume that
       there will not be more than 127 credits. */
    CsrUint16                              credits_taken;
#else /* INSTALL_AMP_SUPPORT */
    BITFIELD(CsrUint16, credits_taken, 7);
#endif /* INSTALL_AMP_SUPPORT */
    BITFIELD(CsrBool,                      tx_active, 1);        /*!< Transmit loop active, prevent callback deadlock */
    BITFIELD(CsrUint8, reserved_credit, 1);                      /*!< Our own private credit */
    struct txqetag                       *tx_queue[L2CAP_MAX_TX_QUEUES]; /*!< Transmit queue */
    struct txqetag                       *tx_done;              /*!< Completed queue */
    struct txqetag                       *tx_current;           /*!< Current element under transmission */
} TXQUEUE_T;

/*! \brief ACL reassembly buffer

    For host builds, the ACL reassembly function is a part of L2CAP.
    For this, we need a place to store non-complete packets.
*/
#ifdef BUILD_FOR_HOST
typedef struct
{
    CsrMblk                               *mblk;                 /*!< Reassembly buffer MBLK */
    l2ca_fs_flush_t                       length;               /*!< Expected length - total size can be 2^16+4 */
    l2ca_fs_flush_t                       offset;               /*!< How much received so far - total size can be 2^16+4 */
} CHCB_REASSEMBLE_T;
#endif

/*! \brief Connection Handle Control Block.

    The Connection Handle Control Block holds the parameters needs to
    manage a L2CAP connection.  Each L2CAP connection will have an
    instance of this control block.
*/
typedef struct chtag
{
    struct cidtag                        *cidcb_list;           /*!< Pointer to first CIDCB instance for this connection */
#ifdef BUILD_FOR_HOST
    CHCB_REASSEMBLE_T                     reassem;              /*!< On-host reassembly buffer */
#endif
#ifdef INSTALL_AMP_SUPPORT
    TXQUEUE_T                            *amp_queues;           /*!< Linked list of AMP transmit queues */
#endif
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    CsrUint16                              signal_mtu;           /*!< Signalling MTU */
#endif
#ifdef INSTALL_L2CAP_RAW_SUPPORT
    CsrSchedQid                             raw_handle;           /*!< App handle for raw packet reception */
    BITFIELD(CsrBool,                      raw_hci_sniff, 1);    /*!< Copy raw HCI data to application */
    BITFIELD(CsrBool,                      raw_rx, 1);           /*!< Raw packet support status */
    BITFIELD(CsrBool,                      raw_crc_rejects, 1);  /*!< Copy CRC rejects to application */
#endif
    BITFIELD(CsrBool,                      signal_scheduled, 1); /*!< Next available signal ID and duplicate avoidance buffer */
    BITFIELD(l2ca_identifier_t,           signal_id, 8);        /*!< Last outgoing signal ID */
    DUPSIG_T                              signal_dup;           /*!< Duplicate signal detection placeholder */
    struct sig_signal_tag                *signal_queue;         /*!< Queue of signals */
    struct sig_signal_tag                *signal_pending;       /*!< Queue of signals pending response */
    TXQUEUE_T                             queue;                /*!< BR/EDR transmit queue and friends */
} L2CAP_CHCB_T;

#ifdef __cplusplus
}
#endif

#endif
