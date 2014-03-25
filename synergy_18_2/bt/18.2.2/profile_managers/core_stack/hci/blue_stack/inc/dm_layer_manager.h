#ifndef _DM_LAYER_MANAGER_H_
#define _DM_LAYER_MANAGER_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_layer_manager.c

\brief  Device Manager functions.
*/

#include "dm_amp_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /* Bitfield -
       BITS 0-4 current version.
       BITS 5-9 requested version.
       BITS 10-14 version used.
    */
    CsrUint16 BT_Version;

    /* Bitfield of supported versions */
    CsrUint16 BT_Versions_Supported; /* = SUPPORT_BT_VERSION_2p0 */

    /* BR/EDR ACL flow control. */
    DM_HCI_FLOW_CONTROL_T fc;

#ifdef SUPPORT_SEPARATE_LE_BUFFERS
    /* LE data flow control. */
    DM_HCI_FLOW_CONTROL_T le_fc;
#endif

    /* Timer ID for the init call to LYM_Chip_Active */
    CsrSchedTid CsrSchedTid_Chip_Active;

    /* HCI Supported Packet Types. */
    hci_pkt_type_t Supported_Packet_Types;

    /* Bitfield - see below */
    CsrUint16 flags;
} LYMCB_T;

/*
  * LYMCB_T BT_Version reading/writing definitions
  * Current - indicates stack version currently in use.
  *
  * Request - indicates stack version requested from upper layers
  *
  * Used - is a temporary mask which indicates the possible version which the code will process for, after
  * receiving a request. It may or may not be equal to requested stack version. Finally after successful
  * processing, current version would be set equal to used version.
  */
#define LYM_READ_CURRENT_BT_VERSION_MASK    0x001F /* Bits numbered 0-4 */
#define LYM_READ_REQ_BT_VERSION_MASK        0x03E0 /* Bits numbered 5-9 */
#define LYM_READ_USED_BT_VERSION_MASK       0xFC00 /* Bits numbered 10 -15 */

#define CURRENT_VERSION_MASK_LSB 0
#define REQ_VERSION_MASK_LSB 5
#define USED_VERSION_MASK_LSB 10


#define LYM_WRITE_CURRENT_BT_VERSION_MASK   (LYM_READ_REQ_BT_VERSION_MASK |LYM_READ_USED_BT_VERSION_MASK)
#define LYM_WRITE_REQ_BT_VERSION_MASK       (LYM_READ_CURRENT_BT_VERSION_MASK |LYM_READ_USED_BT_VERSION_MASK)
#define LYM_WRITE_USED_BT_VERSION_MASK      (LYM_READ_REQ_BT_VERSION_MASK |LYM_READ_CURRENT_BT_VERSION_MASK)

#define LYM_WRITE_CURRENT_BT_VERSION(V, x) \
   ((V) = ((V) & LYM_WRITE_CURRENT_BT_VERSION_MASK) | (x))

#define LYM_WRITE_REQ_BT_VERSION(V, x) \
   ((V) = ((V) & LYM_WRITE_REQ_BT_VERSION_MASK) | ((x) << REQ_VERSION_MASK_LSB))

#define LYM_WRITE_USED_BT_VERSION(V, x) \
   ((V) = ((V) & LYM_WRITE_USED_BT_VERSION_MASK) | ((x) << USED_VERSION_MASK_LSB))

#define LYM_READ_CURRENT_BT_VERSION(V)  \
   ((V) & LYM_READ_CURRENT_BT_VERSION_MASK)

#define LYM_READ_REQ_BT_VERSION(V)  \
   (((V) & LYM_READ_REQ_BT_VERSION_MASK) >> REQ_VERSION_MASK_LSB)

#define LYM_READ_USED_BT_VERSION(V)  \
   (((V) & LYM_READ_USED_BT_VERSION_MASK) >> USED_VERSION_MASK_LSB)

/* LYMCB_T flags bitfield definitions. */
#define LYM_ControllerReady                 0x0001
#define LYM_ApplicationReady                0x0002
#define LYM_ControllerInterrogated          0x0004
#define LYM_ControllerResponded             0x0008
#define LYM_Confirmations_Sent              0x0010
#define LYM_Role_Switch_Supported           0x0020
#define LYM_Encryption_Supported            0x0040
#define LYM_Packet_Boundary_Flag_Supported  0x0080
#define LYM_ULP_ControllerReady             0x0100
#define LYM_ULP_Encryption_Supported        0x0200

#define LYM_Ready (LYM_ControllerReady | LYM_ApplicationReady | \
        LYM_ControllerInterrogated | LYM_ControllerResponded)

#ifdef BUILD_FOR_HOST
#define LYMCB_flags_default \
  (LYM_Role_Switch_Supported | LYM_Encryption_Supported)
#else
    /* When we're on chip, we don't wait for DM_AM_REGISTER_REQ before
       interrogating the controller. So set LYM_ApplicationReady. */
#define LYMCB_flags_default \
  (LYM_ApplicationReady | LYM_Role_Switch_Supported | LYM_Encryption_Supported)
#endif

#define LYMCB_SET(mask) (LYMCB.flags |= (mask))
#define LYMCB_CLEAR(mask) \
    (LYMCB.flags &= ~(mask))
#define LYMCB_TEST(mask) ((LYMCB.flags & (mask)) == (mask))
#define LYMCB_IS_FLAG_SET(flag) ((LYMCB.flags & (flag)) != 0)

#define LYM_Supported_packet_types() (LYMCB.Supported_Packet_Types)

extern LYMCB_T          LYMCB;

void LYM_Initialise(void);
void LYM_Buffer_Size_Ind(HCI_READ_BUFFER_SIZE_RET_T *prim);
#ifdef INSTALL_ULP
void LYM_ULP_Buffer_Size_Ind(CsrUint16 data_packet_length, CsrUint8 num_data_packets);
void LYM_ULP_Interrogate_Controller(void);
void LYM_ULP_Store_Local_Supported_Features(
                                HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_RET_T * features);
#endif /* INSTALL_ULP */
void LYM_InterrogateController(void);
void LYM_Store_Local_Supported_Features(HCI_READ_LOCAL_SUPP_FEATURES_RET_T * features);
void LYM_Set_BT_Version(CsrUint8 version);
void LYM_Set_BT_Version_CFM(CsrUint8 status);

#ifdef __cplusplus
}
#endif


#endif

