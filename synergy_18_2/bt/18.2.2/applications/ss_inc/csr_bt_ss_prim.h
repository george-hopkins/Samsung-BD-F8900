#ifndef CSR_BT_SS_PRIM_H__
#define CSR_BT_SS_PRIM_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



/*id prim*/
#define CSR_CM_SS_ID  		0x0001 /*GAP*/
#define CSR_SD_SS_ID  		0x0002 /*GAP*/
#define CSR_SC_SS_ID  		0x0003 /*GAP*/
#define CSR_AV_SS_ID   		0x0004
#define CSR_AVRCP_SS_ID   	0x0005
#define CSR_HIDH_SS_ID   	0x0006


/*GAP cmd prim*/
#define GAP_DOWNSTREAM_BASE              0x0000
#define GAP_UPSTREAM_BASE                0x0100


#define BT_UI_MESSAGE     0xABBA
typedef struct
{
    CsrUint16 type;
	CsrUint16 id;
    CsrUint16 cmd;
	CsrUint8 * data;
} CsrBtUiMessage_t;



typedef enum
{
   GAP_SET_SCAN_ENABLE_REQ=GAP_DOWNSTREAM_BASE,
   GAP_CONNECTABLE_REQ,
   GAP_LOCAL_BDADDR_REQ,
   GAP_LOCAL_NAME_REQ,
   GAP_SD_REGISTER_SCDB_REQ,
   GAP_SC_PASSKEY_RES,
   GAP_SET_LOCAL_NAME_REQ,
   GAP_SC_DEREGISGER_SCDB_REQ,
   GAP_SD_SEARCH_REQ,
   GAP_SD_SEARCH_CANCEL_REQ,
   GAP_SET_DBG_LEVEL_REQ,
   GAP_SS_INIT_REQ,
   GAP_SS_RESET_REQ,
   GAP_SC_AUTHRIZE_RSP,
   GAP_CM_WRITE_PAGE_TO_REQ,
   GAP_SC_ENCRYPTION_REQ,
   GAP_SC_SSP_JUSTWORKS_RES,
   GAP_DOWNSTREM_MAX,
   
   GAP_SC_PASSKEY_IND=GAP_UPSTREAM_BASE,
   GAP_SC_SSP_JUSTWORS_IND,
   GAP_SC_BOND_IND,
   GAP_SC_ATHORIZE_IND,
   GAP_LOCAL_BDADDR_CFM,
   GAP_LOCAL_NAME_CFM,
   GAP_SD_SEARCH_IND,
   GAP_SD_SEARCH_CLOSE,
   GAP_CHIP_WD_COUNT,
   GAP_GET_DBG_LEVEL_IND,
   GAP_ERROR_IND,
   GAP_BT_INIT_IND,
   GAP_SC_DEREGISGER_SCDB_CFM,
   GAP_SC_ENCRYPTION_CFM,
   GAP_UPSTREM_MAX
}csr_api_gap_prim_type;


#define GAP_DOWNSTREAM_CNT           (GAP_DOWNSTREM_MAX-GAP_DOWNSTREAM_BASE)

#define GAP_UPSTREAM_CNT              (GAP_UPSTREM_MAX-GAP_UPSTREAM_BASE)





/*AV cmd prim*/
#define AV_DOWNSTREAM_BASE              0x0000
#define AV_UPSTREAM_BASE                0x0100


typedef enum
{
   AV_ACTIVATATE_REQ=AV_DOWNSTREAM_BASE,
   AV_DEACTIVATATE_REQ,
   AV_CONNECT_REQ,
   AV_DISCONNECT_REQ,
   AV_CONNECTING_CANCEL_REQ,
   AV_DOWNSTREM_MAX,
   
   AV_ACTIVATATE_CFM=AV_UPSTREAM_BASE,
   AV_DEACTIVATATE_CFM,
   AV_CONNECT_IND,
   AV_CONFIG_IND,
   AV_PCM_STREAM_DATA_IND,
   AV_CLOSE_IND,
   AV_CLOSE_CFM,
   AV_CONNECT_CFM,
   AV_DISCONNECT_IND,
   AV_OPEN_IND,
   AV_CONFIG_CFM,
   AV_OPEN_CFM,
   AV_DISCONNECT_CFM,
   AV_START_IND,
   AV_SUSPEND_IND,
   AV_UPSTREM_MAX
}csr_api_av_prim_type;


#define AV_DOWNSTREAM_CNT           (AV_DOWNSTREM_MAX-AV_DOWNSTREAM_BASE)

#define AV_UPSTREAM_CNT              (AV_UPSTREM_MAX-AV_UPSTREAM_BASE)

/* AV connection states */
typedef CsrUint8 SsAvStatesAvType;

#define SsAvStateAvDisconnected         (SsAvStatesAvType) 0x00
#define SsAvStateAvConnecting           (SsAvStatesAvType) 0x01
#define SsAvStateAvDisconnectiong       (SsAvStatesAvType) 0x02
#define SsAvStateAvConnected            (SsAvStatesAvType) 0x03
#define SsAvStateAvConnectedOpen        (SsAvStatesAvType) 0x04
#define SsAvStateAvConnectedStreaming   (SsAvStatesAvType) 0x05





/*AVRC cmd prim*/
#define AVRC_DOWNSTREAM_BASE              0x0000
#define AVRC_UPSTREAM_BASE                0x0100


typedef enum
{
   AVRC_ACTIVATATE_REQ=AVRC_DOWNSTREAM_BASE,
   AVRC_DEACTIVATATE_REQ,
   AVRC_CT_PASS_THROUGH_REQ,
   AVRC_CONNECT_REQ,
   AVRC_DISCONNECT_REQ,
   AVRC_CT_SET_VOLUME_REQ,
   AVRC_CONNECTING_CANCEL_REQ,
   AVRC_DOWNSTREM_MAX,
   
   AVRC_ACTIVATATE_CFM=AVRC_UPSTREAM_BASE,
   AVRC_DEACTIVATATE_CFM,
   AVRC_CONNECT_IND,
   AVRC_REMOTE_FEATURES_IND,
   AVRC_CT_PASS_THROUGH_CFM,
   AVRC_CLOSE_IND,
   AVRC_CONNECT_CFM,
   AVRC_DISCONNECT_IND,
   AVRC_TG_PASS_THROUGH_IND,
   AVRC_DISCONNECT_CFM,
   AVRC_UPSTREM_MAX
}csr_api_avrc_prim_type;


#define AVRC_DOWNSTREAM_CNT           (AVRC_DOWNSTREM_MAX-AVRC_DOWNSTREAM_BASE)

#define AVRC_UPSTREAM_CNT              (AVRC_UPSTREM_MAX-AVRC_UPSTREAM_BASE)


/* AVRCP states */
typedef CsrUint8 SSAvStatesAvrcpType;

#define SSAvStateAvrcpIdle  (SSAvStatesAvrcpType) 0x00
#define SSAvStateAvrcpConnecting     (SSAvStatesAvrcpType) 0x02
#define SSAvStateAvrcpDsiconnecting     (SSAvStatesAvrcpType) 0x03
#define SSAvStateAvrcpConnected     (SSAvStatesAvrcpType) 0x04



/*HID cmd prim*/
#define HIDH_DOWNSTREAM_BASE              0x0000
#define HIDH_UPSTREAM_BASE                0x0100





typedef enum
{
   HIDH_ACTIVATATE_REQ=HIDH_DOWNSTREAM_BASE,
   HIDH_DEACTIVATATE_REQ,
   HIDH_CONNECT_REQ,
   HIDH_CONNECT_ACCEPCT_REQ,
   HIDH_DISCONNECT_REQ,
   HIDH_CONTROL_REQ,
   HIDH_SEND_DATA_REQ,
   HIDH_DOWNSTREM_MAX,
   
   HIDH_ACTIVATATE_CFM=HIDH_UPSTREAM_BASE,
   HIDH_DEACTIVATATE_CFM,
   HIDH_CONNECT_IND,
   HIDH_CONNECT_CFM,
   HIDH_DISCONNECT_IND,
   HIDH_DATA_IND,
   HIDH_STATUS_IND,
   HIDH_CONNECT_ACCEPT_CFM,
   HIDH_DISCONNECT_CFM,
   HIDH_UPSTREM_MAX
}csr_api_hidh_prim_type;


#define HIDH_DOWNSTREAM_CNT           (HIDH_DOWNSTREM_MAX-HIDH_DOWNSTREAM_BASE)

#define HIDH_UPSTREAM_CNT              (HIDH_UPSTREM_MAX-HIDH_UPSTREAM_BASE)


/* Demo app states */
typedef CsrUint8 hidhStates_t;
#define HIDH_STATE_IDLE (hidhStates_t) 0x00
#define HIDH_STATE_CONNECTED (hidhStates_t) 0x02
#define HIDH_STATE_AWAIT_CONNECT (hidhStates_t) 0x03
#define HIDH_STATE_UNPLUG (hidhStates_t) 0x04
#define HIDH_STATE_RECONNECTING (hidhStates_t) 0x05
#define HIDH_STATE_AWAIT_USER_REG (hidhStates_t) 0x06
#define HIDH_NOF_STATES (hidhStates_t) 0x07



#define CSR_ERR_CLI_SUCESS               (0)
#define CSR_ERR_CLI_NOT_CONNECT          (-11)
#define CSR_ERR_CLI_INTERNAL             (-12)
#define CSR_ERR_CLI_BAD_PARAM            (-13)
#define CSR_ERR_CLI_REGISTER_ERR          (-14)


#define MAX_APP_REGISTER_DEV 				10


#define SYNERGY_SS_VER                     "2.14"

#ifdef __cplusplus
}
#endif

#endif//CSR_BT_SS_PRIM_H__
