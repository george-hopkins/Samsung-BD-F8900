#ifndef _DM_MGT_INTERFACE_H_
#define _DM_MGT_INTERFACE_H_

#include "csr_synergy.h"
/*!

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

\file              dm_mgt_interface.h

\brief             This file contains DM ACL Interface declarations.



*/

#include "dm_acl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_sched.h"

/*! Public Data */
extern CsrSchedQid am_phandle;

/*! Public Functions */
void CsrBtDmInit(void **gash);
void dm_am_handler(void **gash);
void dm_unhandled_command(DM_UPRIM_T *p_prim, hci_error_t status);

void DM_SendMessage(void *message);
void DM_SendMessageAMphandle(void *message);
void DM_SendStandardCommandComplete(dm_prim_t type, hci_return_t status);
void DM_SendBDADDRCommandComplete(dm_prim_t type, hci_return_t status, const BD_ADDR_T *bd_addr);
void DM_SendTBDADDRCommandComplete(dm_prim_t type, hci_return_t status, const TYPED_BD_ADDR_T *addrt);
void DM_SendBadMessageInd(CsrUint16 reason, CsrUint8 protocol, CsrUint16 length, void *message);

void send_dm_am_register_cfm(void);
void send_dm_acl_opened_ind(DM_ACL_T *p_acl, CsrUint8 status);
void Send_DM_Inquiry_Result(HCI_UPRIM_T *hci_prim);
void Send_DM_Read_Local_Supp_Features_Complete(CsrUint8 status, HCI_READ_LOCAL_SUPP_FEATURES_RET_T *in_prim);
void Send_Change_Conn_Link_Key_Complete(CsrUint8 status, BD_ADDR_T *p_bd_addr);
void Send_DM_Switch_Role_Complete(const CsrUint8 status, const BD_ADDR_T *const bd_addr, const CsrUint8 role);
void Send_DM_HC_To_Host_FC_Complete(CsrUint8 status);
void Send_DM_Inquiry_Result_With_RSSI(HCI_UPRIM_T *hci_prim);
void Send_DM_Extended_Inquiry_Result(HCI_UPRIM_T *hci_prim);
void send_dm_ble_update_connection_parameters_cfm(const CsrUint8 status, const TYPED_BD_ADDR_T *const addrt);

#ifndef DISABLE_DM_BREDR
void Send_Return_Link_Keys_Complete(HCI_UPRIM_T *hci_prim);
void Send_Master_Link_Key_Complete(HCI_UPRIM_T *hci_prim);
void Send_DM_Mode_Change_Event(const CsrUint8 status, const BD_ADDR_T *const bd_addr, const CsrUint8 mode, const CsrUint16 length);
void Send_DM_HCI_Remote_Name_Cfm(const CsrUint8 status, const BD_ADDR_T *const p_bd_addr, CsrUint8 *pp_name[HCI_LOCAL_NAME_BYTE_PACKET_PTRS]);
#else
#define Send_Return_Link_Keys_Complete    dm_ev_not_used
#define Send_Master_Link_Key_Complete     dm_ev_not_used
#define Send_DM_Mode_Change_Event         dm_ev_not_used
#define Send_DM_HCI_Remote_Name_Cfm       dm_ev_not_used
#endif


#ifdef __cplusplus
}
#endif


#endif

