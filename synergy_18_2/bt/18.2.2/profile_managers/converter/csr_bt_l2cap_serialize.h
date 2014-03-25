#ifndef CSR_BT_L2CAP_SERIALIZE_H__
#define CSR_BT_L2CAP_SERIALIZE_H__
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

void CsrBtL2capPfree(void *ptr);

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP
CsrSize L2CA_CONNECTION_PAR_UPDATE_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECTION_PAR_UPDATE_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECTION_PAR_UPDATE_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_IND
CsrSize L2CA_AUTO_CONNECT_IND_TSizeof(void *msg);
CsrUint8 *L2CA_AUTO_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_AUTO_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_AUTO_CONNECT_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_IND */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_REQ
CsrSize L2CA_ROUTE_DATA_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_ROUTE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_ROUTE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_ROUTE_DATA_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_ROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_CFM
CsrSize L2CA_DATAWRITE_ABORT_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_DATAWRITE_ABORT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAWRITE_ABORT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DATAWRITE_ABORT_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_CFM */

#ifndef EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND
CsrSize L2CA_UNKNOWN_SIGNAL_IND_TSizeof(void *msg);
CsrUint8 *L2CA_UNKNOWN_SIGNAL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNKNOWN_SIGNAL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNKNOWN_SIGNAL_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_REQ
CsrSize L2CA_DATAWRITE_ABORT_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_DATAWRITE_ABORT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAWRITE_ABORT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DATAWRITE_ABORT_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_REQ
CsrSize L2CA_DATAWRITE_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_DATAWRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAWRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_DATAWRITE_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_DATAWRITE_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ
CsrSize L2CA_REGISTER_FIXED_CID_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_REGISTER_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_REGISTER_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_REGISTER_FIXED_CID_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ
CsrSize L2CA_GET_CHANNEL_INFO_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_GET_CHANNEL_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_GET_CHANNEL_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_GET_CHANNEL_INFO_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_CFM
CsrSize L2CA_AUTO_CONNECT_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_AUTO_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_AUTO_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_AUTO_CONNECT_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_REQ
CsrSize L2CA_MAP_FIXED_CID_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_MAP_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MAP_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MAP_FIXED_CID_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_IND
CsrSize L2CA_UNROUTE_DATA_IND_TSizeof(void *msg);
CsrUint8 *L2CA_UNROUTE_DATA_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNROUTE_DATA_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNROUTE_DATA_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_IND */

#ifndef EXCLUDE_L2CA_AMP_LINK_LOSS_IND
CsrSize L2CA_AMP_LINK_LOSS_IND_TSizeof(void *msg);
CsrUint8 *L2CA_AMP_LINK_LOSS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_AMP_LINK_LOSS_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_AMP_LINK_LOSS_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_REQ
CsrSize L2CA_AUTO_CONNECT_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_AUTO_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_AUTO_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_AUTO_CONNECT_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_DEBUG_DROP_REQ
CsrSize L2CA_DEBUG_DROP_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_DEBUG_DROP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DEBUG_DROP_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DEBUG_DROP_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DEBUG_DROP_REQ */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND
CsrSize L2CA_CONNECTION_PAR_UPDATE_IND_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECTION_PAR_UPDATE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECTION_PAR_UPDATE_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_RSP
CsrSize L2CA_CREATE_CHANNEL_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_CREATE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CREATE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CREATE_CHANNEL_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_DATAREAD_IND
CsrSize L2CA_DATAREAD_IND_TSizeof(void *msg);
CsrUint8 *L2CA_DATAREAD_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAREAD_IND_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_DATAREAD_IND_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_DATAREAD_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CFM
CsrSize L2CA_MOVE_CHANNEL_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_MOVE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MOVE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MOVE_CHANNEL_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_RSP
CsrSize L2CA_CONFIG_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_CONFIG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONFIG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_CONFIG_RSP_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_CONFIG_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_IND
CsrSize L2CA_MOVE_CHANNEL_IND_TSizeof(void *msg);
CsrUint8 *L2CA_MOVE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MOVE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MOVE_CHANNEL_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_REQ
CsrSize L2CA_MOVE_CHANNEL_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_MOVE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MOVE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MOVE_CHANNEL_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_BUSY_REQ
CsrSize L2CA_BUSY_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_BUSY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_BUSY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_BUSY_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_BUSY_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_CFM
CsrSize L2CA_DATAWRITE_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_DATAWRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAWRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DATAWRITE_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DATAWRITE_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_IND
CsrSize L2CA_MAP_FIXED_CID_IND_TSizeof(void *msg);
CsrUint8 *L2CA_MAP_FIXED_CID_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MAP_FIXED_CID_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MAP_FIXED_CID_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_CONFIG_CFM
CsrSize L2CA_CONFIG_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_CONFIG_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_CONFIG_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_RSP
CsrSize L2CA_DISCONNECT_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_DISCONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DISCONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DISCONNECT_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DISCONNECT_RSP */

#ifndef EXCLUDE_L2CA_REGISTER_CFM
CsrSize L2CA_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_REGISTER_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM
CsrSize L2CA_REGISTER_FIXED_CID_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_REGISTER_FIXED_CID_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_REGISTER_FIXED_CID_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_REGISTER_FIXED_CID_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_MTU_VIOLATION_IND
CsrSize L2CA_MTU_VIOLATION_IND_TSizeof(void *msg);
CsrUint8 *L2CA_MTU_VIOLATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MTU_VIOLATION_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MTU_VIOLATION_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MTU_VIOLATION_IND */

#ifndef EXCLUDE_L2CA_BUSY_IND
CsrSize L2CA_BUSY_IND_TSizeof(void *msg);
CsrUint8 *L2CA_BUSY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_BUSY_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_BUSY_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_BUSY_IND */

#ifndef EXCLUDE_L2CA_RAW_DATA_REQ
CsrSize L2CA_RAW_DATA_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_RAW_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_RAW_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_RAW_DATA_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_RAW_DATA_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM
CsrSize L2CA_GET_CHANNEL_INFO_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_GET_CHANNEL_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_GET_CHANNEL_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_GET_CHANNEL_INFO_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_CFM
CsrSize L2CA_ROUTE_DATA_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_ROUTE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_ROUTE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_ROUTE_DATA_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_ROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_CFM
CsrSize L2CA_CONNECT_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECT_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ
CsrSize L2CA_UNMAP_FIXED_CID_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_UNMAP_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNMAP_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNMAP_FIXED_CID_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_REQ
CsrSize L2CA_CREATE_CHANNEL_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_CREATE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CREATE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CREATE_CHANNEL_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_PING_CFM
CsrSize L2CA_PING_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_PING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_PING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_PING_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_PING_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_REQ
CsrSize L2CA_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_REGISTER_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_L2CA_CONFIG_REQ
CsrSize L2CA_CONFIG_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_CONFIG_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_CONFIG_REQ */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_RSP
CsrSize L2CA_MAP_FIXED_CID_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_MAP_FIXED_CID_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MAP_FIXED_CID_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MAP_FIXED_CID_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND
CsrSize L2CA_MOVE_CHANNEL_CMP_IND_TSizeof(void *msg);
CsrUint8 *L2CA_MOVE_CHANNEL_CMP_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MOVE_CHANNEL_CMP_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MOVE_CHANNEL_CMP_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM
CsrSize L2CA_CONNECTION_PAR_UPDATE_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECTION_PAR_UPDATE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECTION_PAR_UPDATE_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_RSP
CsrSize L2CA_AUTO_CONNECT_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_AUTO_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_AUTO_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_AUTO_CONNECT_RSP_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_CONNECT_REQ
CsrSize L2CA_CONNECT_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECT_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_CFM
CsrSize L2CA_DISCONNECT_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DISCONNECT_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_L2CA_PING_REQ
CsrSize L2CA_PING_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_PING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_PING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_PING_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_PING_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_CFM
CsrSize L2CA_UNROUTE_DATA_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_UNROUTE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNROUTE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNROUTE_DATA_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_IND
CsrSize L2CA_CONNECT_IND_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECT_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECT_IND */

#ifndef EXCLUDE_L2CA_RAW_MODE_REQ
CsrSize L2CA_RAW_MODE_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_RAW_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_RAW_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_RAW_MODE_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_RAW_MODE_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_REQ
CsrSize L2CA_DISCONNECT_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DISCONNECT_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_L2CA_UNREGISTER_CFM
CsrSize L2CA_UNREGISTER_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNREGISTER_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNREGISTER_CFM */

#ifndef EXCLUDE_L2CA_UNREGISTER_REQ
CsrSize L2CA_UNREGISTER_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNREGISTER_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_L2CA_TIMEOUT_IND
CsrSize L2CA_TIMEOUT_IND_TSizeof(void *msg);
CsrUint8 *L2CA_TIMEOUT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_TIMEOUT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_TIMEOUT_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_TIMEOUT_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_RSP
CsrSize L2CA_MOVE_CHANNEL_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_MOVE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MOVE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MOVE_CHANNEL_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_REQ
CsrSize L2CA_UNROUTE_DATA_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_UNROUTE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNROUTE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNROUTE_DATA_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_GETINFO_REQ
CsrSize L2CA_GETINFO_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_GETINFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_GETINFO_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_GETINFO_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_GETINFO_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_CFM
CsrSize L2CA_CREATE_CHANNEL_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_CREATE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CREATE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CREATE_CHANNEL_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_IND
CsrSize L2CA_CONFIG_IND_TSizeof(void *msg);
CsrUint8 *L2CA_CONFIG_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONFIG_IND_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_CONFIG_IND_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_CONFIG_IND */

#ifndef EXCLUDE_L2CA_DATAREAD_RSP
CsrSize L2CA_DATAREAD_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_DATAREAD_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DATAREAD_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DATAREAD_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DATAREAD_RSP */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ
CsrSize L2CA_CONNECTION_PAR_UPDATE_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECTION_PAR_UPDATE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECTION_PAR_UPDATE_REQ_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ */

#ifndef EXCLUDE_L2CA_MULTICAST_REQ
CsrSize L2CA_MULTICAST_REQ_TSizeof(void *msg);
CsrUint8 *L2CA_MULTICAST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MULTICAST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_MULTICAST_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_MULTICAST_REQ */

#ifndef EXCLUDE_L2CA_RAW_DATA_IND
CsrSize L2CA_RAW_DATA_IND_TSizeof(void *msg);
CsrUint8 *L2CA_RAW_DATA_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_RAW_DATA_IND_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_RAW_DATA_IND_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_RAW_DATA_IND */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_CFM
CsrSize L2CA_MAP_FIXED_CID_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_MAP_FIXED_CID_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_MAP_FIXED_CID_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_MAP_FIXED_CID_CFM_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_IND
CsrSize L2CA_UNMAP_FIXED_CID_IND_TSizeof(void *msg);
CsrUint8 *L2CA_UNMAP_FIXED_CID_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_UNMAP_FIXED_CID_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_UNMAP_FIXED_CID_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_IND
CsrSize L2CA_CREATE_CHANNEL_IND_TSizeof(void *msg);
CsrUint8 *L2CA_CREATE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CREATE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CREATE_CHANNEL_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_CONNECT_RSP
CsrSize L2CA_CONNECT_RSP_TSizeof(void *msg);
CsrUint8 *L2CA_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_CONNECT_RSP_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_GETINFO_CFM
CsrSize L2CA_GETINFO_CFM_TSizeof(void *msg);
CsrUint8 *L2CA_GETINFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_GETINFO_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void L2CA_GETINFO_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_L2CA_GETINFO_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_IND
CsrSize L2CA_DISCONNECT_IND_TSizeof(void *msg);
CsrUint8 *L2CA_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *L2CA_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define L2CA_DISCONNECT_IND_TSerFree CsrBtL2capPfree
#endif /* EXCLUDE_L2CA_DISCONNECT_IND */

#ifdef __cplusplus
}
#endif

#endif
