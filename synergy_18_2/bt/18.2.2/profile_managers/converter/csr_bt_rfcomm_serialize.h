#ifndef CSR_BT_RFCOMM_SERIALIZE_H__
#define CSR_BT_RFCOMM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtRfcommPfree(void *ptr);

#ifndef EXCLUDE_RFC_REGISTER_REQ
CsrSize RFC_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *RFC_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_REGISTER_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_REGISTER_REQ */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP
CsrSize RFC_L2CA_MOVE_CHANNEL_RSP_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_MOVE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_MOVE_CHANNEL_RSP_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_RFC_DATAREAD_RSP
CsrSize RFC_DATAREAD_RSP_TSizeof(void *msg);
CsrUint8 *RFC_DATAREAD_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DATAREAD_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DATAREAD_RSP_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DATAREAD_RSP */

#ifndef EXCLUDE_RFC_PORTNEG_REQ
CsrSize RFC_PORTNEG_REQ_TSizeof(void *msg);
CsrUint8 *RFC_PORTNEG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_PORTNEG_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_PORTNEG_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_PORTNEG_REQ */

#ifndef EXCLUDE_RFC_REGISTER_CFM
CsrSize RFC_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *RFC_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_REGISTER_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_REGISTER_CFM */

#ifndef EXCLUDE_RFC_TEST_CFM
CsrSize RFC_TEST_CFM_TSizeof(void *msg);
CsrUint8 *RFC_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void RFC_TEST_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_RFC_TEST_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM
CsrSize RFC_L2CA_MOVE_CHANNEL_CFM_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_MOVE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_MOVE_CHANNEL_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_RFC_ERROR_IND
CsrSize RFC_ERROR_IND_TSizeof(void *msg);
CsrUint8 *RFC_ERROR_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_ERROR_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_ERROR_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_ERROR_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_CFM
CsrSize RFC_SERVER_CONNECT_CFM_TSizeof(void *msg);
CsrUint8 *RFC_SERVER_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_SERVER_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_SERVER_CONNECT_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_SERVER_CONNECT_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND
CsrSize RFC_L2CA_MOVE_CHANNEL_IND_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_MOVE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_MOVE_CHANNEL_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_IND
CsrSize RFC_SERVER_CONNECT_IND_TSizeof(void *msg);
CsrUint8 *RFC_SERVER_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_SERVER_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_SERVER_CONNECT_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_SERVER_CONNECT_IND */

#ifndef EXCLUDE_RFC_DATAWRITE_CFM
CsrSize RFC_DATAWRITE_CFM_TSizeof(void *msg);
CsrUint8 *RFC_DATAWRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DATAWRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DATAWRITE_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DATAWRITE_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_IND
CsrSize RFC_PORTNEG_IND_TSizeof(void *msg);
CsrUint8 *RFC_PORTNEG_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_PORTNEG_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_PORTNEG_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_PORTNEG_IND */

#ifndef EXCLUDE_RFC_FC_CFM
CsrSize RFC_FC_CFM_TSizeof(void *msg);
CsrUint8 *RFC_FC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_FC_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_FC_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_FC_CFM */

#ifndef EXCLUDE_RFC_FC_IND
CsrSize RFC_FC_IND_TSizeof(void *msg);
CsrUint8 *RFC_FC_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_FC_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_FC_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_FC_IND */

#ifndef EXCLUDE_RFC_INIT_REQ
CsrSize RFC_INIT_REQ_TSizeof(void *msg);
CsrUint8 *RFC_INIT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_INIT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_INIT_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_INIT_REQ */

#ifndef EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND
CsrSize RFC_L2CA_AMP_LINK_LOSS_IND_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_AMP_LINK_LOSS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_AMP_LINK_LOSS_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_AMP_LINK_LOSS_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_RFC_INIT_CFM
CsrSize RFC_INIT_CFM_TSizeof(void *msg);
CsrUint8 *RFC_INIT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_INIT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_INIT_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_INIT_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ
CsrSize RFC_L2CA_MOVE_CHANNEL_REQ_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_MOVE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_MOVE_CHANNEL_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_RFC_UNREGISTER_CFM
CsrSize RFC_UNREGISTER_CFM_TSizeof(void *msg);
CsrUint8 *RFC_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_UNREGISTER_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_UNREGISTER_CFM */

#ifndef EXCLUDE_RFC_LINESTATUS_REQ
CsrSize RFC_LINESTATUS_REQ_TSizeof(void *msg);
CsrUint8 *RFC_LINESTATUS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_LINESTATUS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_LINESTATUS_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_LINESTATUS_REQ */

#ifndef EXCLUDE_RFC_TEST_REQ
CsrSize RFC_TEST_REQ_TSizeof(void *msg);
CsrUint8 *RFC_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void RFC_TEST_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_RFC_TEST_REQ */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_REQ
CsrSize RFC_CLIENT_CONNECT_REQ_TSizeof(void *msg);
CsrUint8 *RFC_CLIENT_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_CLIENT_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void RFC_CLIENT_CONNECT_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_CFM
CsrSize RFC_DISCONNECT_CFM_TSizeof(void *msg);
CsrUint8 *RFC_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DISCONNECT_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_RSP
CsrSize RFC_PORTNEG_RSP_TSizeof(void *msg);
CsrUint8 *RFC_PORTNEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_PORTNEG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_PORTNEG_RSP_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_PORTNEG_RSP */

#ifndef EXCLUDE_RFC_UNREGISTER_REQ
CsrSize RFC_UNREGISTER_REQ_TSizeof(void *msg);
CsrUint8 *RFC_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_UNREGISTER_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_UNREGISTER_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_REQ
CsrSize RFC_DISCONNECT_REQ_TSizeof(void *msg);
CsrUint8 *RFC_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DISCONNECT_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_IND
CsrSize RFC_DISCONNECT_IND_TSizeof(void *msg);
CsrUint8 *RFC_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DISCONNECT_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DISCONNECT_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_CFM
CsrSize RFC_MODEM_STATUS_CFM_TSizeof(void *msg);
CsrUint8 *RFC_MODEM_STATUS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_MODEM_STATUS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_MODEM_STATUS_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_MODEM_STATUS_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_CFM
CsrSize RFC_PORTNEG_CFM_TSizeof(void *msg);
CsrUint8 *RFC_PORTNEG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_PORTNEG_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_PORTNEG_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_PORTNEG_CFM */

#ifndef EXCLUDE_RFC_DATAWRITE_REQ
CsrSize RFC_DATAWRITE_REQ_TSizeof(void *msg);
CsrUint8 *RFC_DATAWRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DATAWRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void RFC_DATAWRITE_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_RFC_DATAWRITE_REQ */

#ifndef EXCLUDE_RFC_LINESTATUS_CFM
CsrSize RFC_LINESTATUS_CFM_TSizeof(void *msg);
CsrUint8 *RFC_LINESTATUS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_LINESTATUS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_LINESTATUS_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_LINESTATUS_CFM */

#ifndef EXCLUDE_RFC_FC_REQ
CsrSize RFC_FC_REQ_TSizeof(void *msg);
CsrUint8 *RFC_FC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_FC_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_FC_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_FC_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_RSP
CsrSize RFC_DISCONNECT_RSP_TSizeof(void *msg);
CsrUint8 *RFC_DISCONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DISCONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_DISCONNECT_RSP_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_DISCONNECT_RSP */

#ifndef EXCLUDE_RFC_LINESTATUS_IND
CsrSize RFC_LINESTATUS_IND_TSizeof(void *msg);
CsrUint8 *RFC_LINESTATUS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_LINESTATUS_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_LINESTATUS_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_LINESTATUS_IND */

#ifndef EXCLUDE_RFC_DATAREAD_IND
CsrSize RFC_DATAREAD_IND_TSizeof(void *msg);
CsrUint8 *RFC_DATAREAD_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_DATAREAD_IND_TDes(CsrUint8 *buffer, CsrSize length);
void RFC_DATAREAD_IND_TSerFree(void *msg);
#endif /* EXCLUDE_RFC_DATAREAD_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_IND
CsrSize RFC_MODEM_STATUS_IND_TSizeof(void *msg);
CsrUint8 *RFC_MODEM_STATUS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_MODEM_STATUS_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_MODEM_STATUS_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_MODEM_STATUS_IND */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_CFM
CsrSize RFC_CLIENT_CONNECT_CFM_TSizeof(void *msg);
CsrUint8 *RFC_CLIENT_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_CLIENT_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_CLIENT_CONNECT_CFM_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_CFM */

#ifndef EXCLUDE_RFC_NSC_IND
CsrSize RFC_NSC_IND_TSizeof(void *msg);
CsrUint8 *RFC_NSC_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_NSC_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_NSC_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_NSC_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_REQ
CsrSize RFC_MODEM_STATUS_REQ_TSizeof(void *msg);
CsrUint8 *RFC_MODEM_STATUS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_MODEM_STATUS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_MODEM_STATUS_REQ_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_MODEM_STATUS_REQ */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_RSP
CsrSize RFC_SERVER_CONNECT_RSP_TSizeof(void *msg);
CsrUint8 *RFC_SERVER_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_SERVER_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_SERVER_CONNECT_RSP_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_SERVER_CONNECT_RSP */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND
CsrSize RFC_L2CA_MOVE_CHANNEL_CMP_IND_TSizeof(void *msg);
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_CMP_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *RFC_L2CA_MOVE_CHANNEL_CMP_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define RFC_L2CA_MOVE_CHANNEL_CMP_IND_TSerFree CsrBtRfcommPfree
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND */

#ifdef __cplusplus
}
#endif

#endif
