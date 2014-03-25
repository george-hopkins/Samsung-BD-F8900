#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_avrcp_prim.h"

/***** Data related *****/
/* AVCTP structure */
#define AVRCP_DATA_AVCTP_IPID_INDEX             (0)
#define AVRCP_DATA_AVCTP_IPID_MASK              (0x01)
#define AVRCP_DATA_AVCTP_IPID_GET(rxData)       ((rxData)[AVRCP_DATA_AVCTP_IPID_INDEX] & AVRCP_DATA_AVCTP_IPID_MASK)

#define AVRCP_DATA_AVCTP_CR_INDEX               (0)
#define AVRCP_DATA_AVCTP_CR_SHIFT               (1)
#define AVRCP_DATA_AVCTP_CR_MASK                (0x01)
#define AVRCP_DATA_AVCTP_CR_CMD                 (0x0)
#define AVRCP_DATA_AVCTP_CR_RES                 (0x1)
#define AVRCP_DATA_AVCTP_CR_INVALID             (0x2)
#define AVRCP_DATA_AVCTP_CR_GET(rxData)         (CSR_BIT_IS_SET((rxData)[AVRCP_DATA_AVCTP_CR_INDEX], AVRCP_DATA_AVCTP_CR_SHIFT))

#define AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX      (0)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT      (2)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_MASK       (0x3)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_SINGLE     (0x0)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_START      (0x1)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_CONTINUE   (0x2)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_END        (0x3)
#define AVRCP_DATA_AVCTP_PACKET_TYPE_GET(rxData)(((rxData)[AVRCP_DATA_AVCTP_PACKET_TYPE_INDEX] >> AVRCP_DATA_AVCTP_PACKET_TYPE_SHIFT) & AVRCP_DATA_AVCTP_PACKET_TYPE_MASK)

#define AVRCP_DATA_AVCTP_TLABEL_INDEX           (0)
#define AVRCP_DATA_AVCTP_TLABEL_SHIFT           (4)
#define AVRCP_DATA_AVCTP_TLABEL_MASK            (0x0F)
#define AVRCP_TLABEL_ASSIGN(tLabel)             ((CsrUint8)(tLabel = ((tLabel+1) % 16)))
#define AVRCP_TLABEL_GET(data)                  ((CsrUint8)(((data)[AVRCP_DATA_AVCTP_TLABEL_INDEX] >> AVRCP_DATA_AVCTP_TLABEL_SHIFT) & AVRCP_DATA_AVCTP_TLABEL_MASK))
#define AVRCP_TLABEL_INVALID                    (0xFF)

#define AVRCP_DATA_AVCTP_SINGLE_PID_INDEX       (1)
#define AVRCP_DATA_AVCTP_START_PID_INDEX        (2)

#define AVRCP_DATA_AVCTP_SINGLE_MI_INDEX        (3)
#define AVRCP_DATA_AVCTP_START_MI_INDEX         (4)
#define AVRCP_DATA_AVCTP_CONTINUE_MI_INDEX      (1)
#define AVRCP_DATA_AVCTP_END_MI_INDEX           (1)

#define AVRCP_DATA_AVCTP_NUM_OF_PACKETS_INDEX   (1)     /* Only present if AVCTP packet type is 'start' */
#define AVRCP_DATA_AVCTP_HEADER_SIZE_SINGLE     (3)
#define AVRCP_DATA_AVCTP_HEADER_SIZE_START      (4)
#define AVRCP_DATA_AVCTP_HEADER_SIZE_CONTINUE   (1)
#define AVRCP_DATA_AVCTP_HEADER_SIZE_END        (1)
#define AVRCP_DATA_AVCTP_HEADER_SIZE_MIN        (1)     /* Continue/end */

/* AV/C structure - all indexes/offsets are only valid for single packet AVCTP frames */
#define AVRCP_DATA_AVC_MAX_SIZE                 (512)   /* As specified by the AV/C Command Specification */
#define AVRCP_DATA_AVC_CRTYPE_INDEX             (0 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)     /* Indicates which command or response type */
#define AVRCP_DATA_AVC_CRTYPE_MASK              (0x0F)
#define AVRCP_DATA_AVC_CRTYPE_GET(rxData)       (CsrUint8)((rxData)[AVRCP_DATA_AVC_CRTYPE_INDEX] & AVRCP_DATA_AVC_CRTYPE_MASK)

#define AVRCP_DATA_AVC_CTYPE_CONTROL            (0x0)
#define AVRCP_DATA_AVC_CTYPE_STATUS             (0x1)
#define AVRCP_DATA_AVC_CTYPE_NOTIFY             (0x3)
#define AVRCP_DATA_AVC_CTYPE_LIMIT              (0x7)
/*
 * RTYPES moved to csr_bt_avrcp_prim.h
 */

#define AVRCP_DATA_AVC_OPCODE_INDEX             (2 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_OPCODE_GET(rxData)       ((rxData)[AVRCP_DATA_AVC_OPCODE_INDEX])
#define AVRCP_DATA_AVC_OPCODE_VENDOR_DEPENDENT  (0x00)
#define AVRCP_DATA_AVC_OPCODE_UNIT_INFO         (0x30)
#define AVRCP_DATA_AVC_OPCODE_SUBUNIT_INFO      (0x31)
#define AVRCP_DATA_AVC_OPCODE_PASS_THROUGH      (0x7C)
#define AVRCP_DATA_AVC_OPCODE_INVALID           (0xFF)
#define AVRCP_DATA_AVC_OPERAND_INDEX            (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_MIN_SIZE                 (1 + AVRCP_DATA_AVC_OPERAND_INDEX)

/* Unit/sub unit info - shared */
#define AVRCP_DATA_AVC_SUBUNIT_TYPE_INDEX       (1 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_TYPE_MASK        (0x1F)
#define AVRCP_DATA_AVC_SUBUNIT_TYPE_SHIFT       (3)
#define AVRCP_DATA_AVC_SUBUNIT_TYPE_PANEL       (0x09)
#define AVRCP_DATA_AVC_SUBUNIT_ID_INDEX         (1 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_ID_MASK          (0x7)

/* UNIT INFO specific */
#define AVRCP_DATA_AVC_UNIT_RES_07_INDEX        (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_UNIT_RES_UNIT_TYPE_MASK  (0x1F)
#define AVRCP_DATA_AVC_UNIT_RES_UNIT_TYPE_SHIFT (3)
#define AVRCP_DATA_AVC_UNIT_RES_UNIT_TYPE_INDEX (4)
#define AVRCP_DATA_AVC_UNIT_RES_UNIT_MASK       (0x07)
#define AVRCP_DATA_AVC_UNIT_RES_UNIT_INDEX      (4 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_UNIT_RES_COMP_ID_INDEX   (5 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_UNIT_RES_COMP_ID_SIZE    (3)
#define AVRCP_DATA_AVC_UNIT_RES_SIZE            (11)

/* SUBUNIT INFO specific */
#define AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_INDEX   (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_MASK    (0x7)
#define AVRCP_DATA_AVC_SUBUNIT_RES_PAGE_SHIFT   (4)
#define AVRCP_DATA_AVC_SUBUNIT_RES_EXT_INDEX    (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_RES_EXT_MASK     (0x7)
#define AVRCP_DATA_AVC_SUBUNIT_RES_SU_TYPE_INDEX (4 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_RES_SU_TYPE_MASK (0x1F)
#define AVRCP_DATA_AVC_SUBUNIT_RES_SU_TYPE_SHIFT (3)
#define AVRCP_DATA_AVC_SUBUNIT_RES_MAX_SU_INDEX (4 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_AVC_SUBUNIT_RES_MAX_SU_MASK  (0x7)
#define AVRCP_DATA_AVC_SUBUNIT_RES_PD_INDEX     (5 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX) /* Page data */

#define AVRCP_DATA_AVC_SUBUNIT_RES_SIZE         (11)

/* Pass through specific */
#define AVRCP_DATA_PT_STATE_INDEX               (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_PT_STATE_MASK                (0x80)
#define AVRCP_DATA_PT_STATE_SHIFT               (7)
#define AVRCP_DATA_PT_STATE_PRESSED             (0x00)
#define AVRCP_DATA_PT_STATE_RELEASED            (0x80)

#define AVRCP_DATA_PT_OPID_INDEX                (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_PT_OPID_MASK                 (0x7F)
#define AVRCP_DATA_PT_STATE_GET(rxData)        ((CsrUint8)(((rxData)[AVRCP_DATA_PT_STATE_INDEX] & AVRCP_DATA_PT_STATE_MASK) == AVRCP_DATA_PT_STATE_PRESSED ? CSR_BT_AVRCP_PT_STATE_PRESS : CSR_BT_AVRCP_PT_STATE_RELEASE))

#define AVRCP_DATA_PT_OPID_LEN_INDEX            (4 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_PT_OPID_GET(rxData)         ((CsrUint8)((rxData)[AVRCP_DATA_PT_OPID_INDEX] & AVRCP_DATA_PT_OPID_MASK))

#define AVRCP_DATA_PT_SIZE                      (1 + AVRCP_DATA_PT_OPID_LEN_INDEX)

/* Group Navigation specific */
#define AVRCP_DATA_PT_GN_DATA_FIELD_LENGTH      (5)
#define AVRCP_DATA_PT_GN_SIZE                   (AVRCP_DATA_PT_SIZE + AVRCP_DATA_PT_GN_DATA_FIELD_LENGTH)
#define AVRCP_DATA_PT_GN_COMPANY_ID_INDEX       (1 + AVRCP_DATA_PT_OPID_LEN_INDEX)
#define AVRCP_DATA_PT_GN_OPERATION_INDEX        (3 + AVRCP_DATA_PT_GN_COMPANY_ID_INDEX)
#define AVRCP_DATA_PT_GN_OPERATION_NEXT         (0x0000)
#define AVRCP_DATA_PT_GN_OPERATION_PREV         (0x0001)
#define AVRCP_DATA_PT_GN_OPERATION_GET(rxData)  (CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_PT_GN_OPERATION_INDEX]))

/* Metadata structure */
#define AVRCP_DATA_MD_BT_COMPANY_ID_INDEX       (0 + AVRCP_DATA_AVC_OPERAND_INDEX)
#define AVRCP_DATA_MD_BT_COMPANY_ID             (0x001958)
#define AVRCP_DATA_MD_BT_COMPANY_ID_0           (0x00) /* MS */
#define AVRCP_DATA_MD_BT_COMPANY_ID_1           (0x19)
#define AVRCP_DATA_MD_BT_COMPANY_ID_2           (0x58) /* LS */
#define AVRCP_DATA_MD_BT_COMPANY_ID_SIZE        (3)

#define AVRCP_DATA_MD_PDU_ID_INDEX              (3 + AVRCP_DATA_AVC_OPERAND_INDEX)

#define AVRCP_DATA_MD_PACKET_TYPE_INDEX         (4 + AVRCP_DATA_AVC_OPERAND_INDEX)
#define AVRCP_DATA_MD_PACKET_TYPE_MASK          (0x03)
#define AVRCP_DATA_MD_PACKET_TYPE_SINGLE        (0x00)
#define AVRCP_DATA_MD_PACKET_TYPE_START         (0x01)
#define AVRCP_DATA_MD_PACKET_TYPE_CONTINUE      (0x02)
#define AVRCP_DATA_MD_PACKET_TYPE_END           (0x03)
#define AVRCP_DATA_MD_PACKET_TYPE_GET(rxData)   ((CsrUint8)((rxData)[AVRCP_DATA_MD_PACKET_TYPE_INDEX] & AVRCP_DATA_MD_PACKET_TYPE_MASK))

#define AVRCP_DATA_MD_PARAM_LEN_INDEX           (5 + AVRCP_DATA_AVC_OPERAND_INDEX)
#define AVRCP_DATA_MD_PARAM_INDEX               (7 + AVRCP_DATA_AVC_OPERAND_INDEX)

#define AVRCP_DATA_MD_HEADER_SIZE               (AVRCP_DATA_MD_PARAM_INDEX) /* Everything including MD parameter length field */

/* Metadata PDU IDs */
#define AVRCP_DATA_PDU_ID_GET_CAPABILITIES              (0x10)
#define AVRCP_DATA_PDU_ID_LIST_PAS_ATTRIBUTES           (0x11)
#define AVRCP_DATA_PDU_ID_LIST_PAS_VALUES               (0x12)
#define AVRCP_DATA_PDU_ID_GET_CUR_PAS_VALUES            (0x13)
#define AVRCP_DATA_PDU_ID_SET_PAS_VALUES                (0x14)
#define AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT        (0x15)
#define AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT            (0x16)
#define AVRCP_DATA_PDU_ID_INFO_DISP_CHAR_SET            (0x17)
#define AVRCP_DATA_PDU_ID_INFO_BAT_STAT_OF_CT           (0x18)
#define AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES        (0x20)
#define AVRCP_DATA_PDU_ID_GET_PLAY_STATUS               (0x30)
#define AVRCP_DATA_PDU_ID_REG_NOTI                      (0x31)
#define AVRCP_DATA_PDU_ID_REQ_CONTINUING_RES            (0x40)
#define AVRCP_DATA_PDU_ID_ABORT_CONTINUING_RES          (0x41)
#define AVRCP_DATA_PDU_ID_SET_ABSOLUTE_VOLUME           (0x50)
#define AVRCP_DATA_PDU_ID_SET_ADDRESSED_PLAYER          (0x60)

/* Browsing PDU IDs */
#define AVRCP_DATA_PDU_ID_SET_BROWSED_PLAYER            (0x70)
#define AVRCP_DATA_PDU_ID_GET_FOLDER_ITEMS              (0x71)
#define AVRCP_DATA_PDU_ID_CHANGE_PATH                   (0x72)
#define AVRCP_DATA_PDU_ID_GET_ITEM_ATTRIBUTES           (0x73)
#define AVRCP_DATA_PDU_ID_PLAY_ITEM                     (0x74)
#define AVRCP_DATA_PDU_ID_SEARCH                        (0x80)
#define AVRCP_DATA_PDU_ID_ADD_TO_NOW_PLAYING            (0x90)
#define AVRCP_DATA_PDU_ID_GENERAL_REJECT                (0xA0)

#define AVRCP_DATA_PDU_ID_INVALID                       (0xFF)

/* GetCapabilities */
#define AVRCP_DATA_PDU_GET_CAP_CMN_ID_INDEX             (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_CAP_CMN_COMP_ID              (0x02)
#define AVRCP_DATA_PDU_GET_CAP_CMN_NOTI_SUP             (0x03)
#define AVRCP_DATA_PDU_GET_CAP_RES_CAPA_ID_INDEX        (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_CAP_RES_COUNT_INDEX          (1 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_CAP_RES_CAPA_INDEX           (2 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_CAP_RES_HEADER_SIZE          (2)
#define AVRCP_DATA_PDU_GET_CAP_CMD_HEADER_SIZE          (1)

/* RegisterNotifications */
#define AVRCP_DATA_PDU_REG_NOTI_CMN_NOTI_ID_INDEX       (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_REG_NOTI_CMD_HEADER_SIZE         (5)
#define AVRCP_DATA_PDU_REG_NOTI_CMD_PLAY_INT_INDEX      (1 + AVRCP_DATA_MD_PARAM_INDEX)

#define AVRCP_DATA_PDU_REG_NOTI_RES_VAL_INDEX           (1 + AVRCP_DATA_MD_PARAM_INDEX)

#define AVRCP_DATA_PDU_REG_NOTI_RES_PS_HEADER_SIZE      (2)                                 /* Playback status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PS_STATUS_INDEX     (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Playback status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_TC_HEADER_SIZE      (9)                                 /* Track changed */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PC_TRACK_INDEX      (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Playback status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_TE_HEADER_SIZE      (1)                                 /* Track start */
#define AVRCP_DATA_PDU_REG_NOTI_RES_TS_HEADER_SIZE      (1)                                 /* Track end */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PBP_HEADER_SIZE     (5)                                 /* Playback position */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PBP_POS_INDEX       (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Playback position */
#define AVRCP_DATA_PDU_REG_NOTI_RES_BATT_HEADER_SIZE    (2)                                 /* Battery status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_BATT_LEVEL_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Battery status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_SYS_HEADER_SIZE     (2)                                 /* System status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_SYS_STATUS_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* System status */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_HEADER_SIZE     (2)                                 /* PAS changed */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_MIN_HEADER_SIZE (4)                                 /* PAS changed */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_NUM_INDEX       (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Number of changed PASs */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_ATT_INDEX       (1 + AVRCP_DATA_PDU_REG_NOTI_RES_PAS_NUM_INDEX)
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_VAL_INDEX       (1 + AVRCP_DATA_PDU_REG_NOTI_RES_PAS_ATT_INDEX)
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_PART_SIZE       (2)                                 /*  */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_ATT_ID_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_REG_NOTI_RES_PAS_VAL_ID_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_REG_NOTI_RES_NPL_HEADER_SIZE     (1)                                 /* NPL */
#define AVRCP_DATA_PDU_REG_NOTI_RES_AP_HEADER_SIZE      (5)                                 /* Available players */
#define AVRCP_DATA_PDU_REG_NOTI_RES_AP_PID_INDEX        (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Available players */
#define AVRCP_DATA_PDU_REG_NOTI_RES_AP_UIDC_INDEX       (3 + AVRCP_DATA_MD_PARAM_INDEX)     /* Available players */
#define AVRCP_DATA_PDU_REG_NOTI_RES_UIDS_HEADER_SIZE    (3)                                 /* Available players */
#define AVRCP_DATA_PDU_REG_NOTI_RES_UIDS_UIDC_INDEX     (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* UIDs */
#define AVRCP_DATA_PDU_REG_NOTI_RES_VOL_HEADER_SIZE     (2)                                 /* Volume */
#define AVRCP_DATA_PDU_REG_NOTI_RES_VOL_INDEX           (1 + AVRCP_DATA_MD_PARAM_INDEX)     /* Volume */




/* PlayerApplicationSettings */
#define AVRCP_DATA_PDU_PAS_ATT_ID_SIZE                  (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_VAL_ID_SIZE                  (1)                                 /*  */

/***** ListPlayerApplicationSettingsAttributes *****/
/* RSP: Size      |<---1---->|<-AttCount*1->|
        Parameter | AttCount |   AttIDs...  |*/
#define AVRCP_DATA_PDU_PAS_LIST_ATT_CMD_SIZE            (0)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_ATT_RES_HEADER_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_ATT_RES_MIN_SIZE        (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_ATT_RES_NUM_INDEX       (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_LIST_ATT_RES_ATT_ID_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */

/***** ListPlayerApplicationSettingsValues *****/
/* CMD: Size      |<--1-->|
        Parameter | AttID |*/
/* RSP: Size      |<---1---->|<-ValCount*1->|
        Parameter | ValCount |   ValIDs...  |*/
#define AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_HEADER_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_VAL_CMD_ATT_ID_INDEX    (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_LIST_VAL_RES_HEADER_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_VAL_RES_MIN_SIZE        (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_LIST_VAL_RES_NUM_INDEX       (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_LIST_VAL_RES_VAL_ID_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */

/***** GetCurrentPlayerApplicationSettingsValue *****/
/* CMD: Size      |<---1---->|<-AttCount*1->|
        Parameter | AttCount |   AttIDs...  |*/
/* RSP: Size      |<--1-->|<--Count*PartSize-->|
        Parameter | Count |  AttId  |  ValID   |*/
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_HEADER_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_MIN_SIZE        (2)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_NUM_INDEX       (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_CMD_ATT_ID_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_HEADER_SIZE     (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_MIN_SIZE        (3)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_NUM_INDEX       (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_PART_SIZE       (2)                                 /* PartSize */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_ATT_ID_INDEX    (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_CURR_VAL_RES_VAL_ID_INDEX    (2 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */

/***** SetPlayerApplicationSettingsValue *****/
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_HEADER_SIZE          (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_PART_SIZE            (2)                                 /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_MIN_SIZE             (3)                                 /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_NUM_INDEX            (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_ATT_ID_INDEX         (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_CMD_VAL_ID_INDEX         (2 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_SET_VAL_RES_HEADER_SIZE          (0)                                 /*  */

/***** GetPlayerApplicationSettingsAttributeText *****/
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_HEADER_SIZE      (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_MIN_SIZE         (2)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_NUM_INDEX        (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_CMD_ATT_ID_INDEX     (AVRCP_DATA_MD_PARAM_INDEX + 1)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_HEADER_SIZE      (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_MIN_SIZE         (6)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_PART_SIZE        (4)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_NUM_INDEX        (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_ATT_ID_INDEX     (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_CSET_INDEX       (2 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_TXT_LEN_INDEX    (4 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_ATT_TXT_RES_TXT_INDEX        (5 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */

/***** GetPlayerApplicationSettingsValueText *****/
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_HEADER_SIZE      (2)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_MIN_SIZE         (3)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_ATT_ID_INDEX     (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_NUM_INDEX        (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_CMD_VAL_ID_INDEX     (2 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_HEADER_SIZE      (1)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_MIN_SIZE         (6)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_PART_SIZE        (4)                                 /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_NUM_INDEX        (AVRCP_DATA_MD_PARAM_INDEX)         /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_VAL_ID_INDEX     (1 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_CSET_INDEX       (2 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_TXT_LEN_INDEX    (4 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */
#define AVRCP_DATA_PDU_PAS_GET_VAL_TXT_RES_TXT_INDEX        (5 + AVRCP_DATA_MD_PARAM_INDEX)     /*  */

/***** GetElementAttributes *****/
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_HEADER_SIZE         (9)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ID_INDEX            (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ID_SIZE             (8)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_NUM_INDEX           (8 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_INDEX           (9 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_CMD_ATT_SIZE            (4)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_HEADER_SIZE         (1)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_COUNT_INDEX     (0 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_PART_SIZE           (8)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_INDEX            (1 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ID_SIZE             (4)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_CSET_INDEX          (5 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_VAL_LEN_INDEX   (7 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_ELEM_ATT_RES_ATT_VAL_INDEX       (9 + AVRCP_DATA_MD_PARAM_INDEX)

/***** InformDisplayableCharacterSet *****/
#define AVRCP_DATA_PDU_DISP_CSET_CMD_HEADER_SIZE            (1)
#define AVRCP_DATA_PDU_DISP_CSET_CMD_MIN_SIZE               (3)
#define AVRCP_DATA_PDU_DISP_CSET_CMD_NUM_INDEX              (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_DISP_CSET_CMD_CSET_INDEX             (1 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_DISP_CSET_RES_HEADER_SIZE            (0)

/***** GetPlayStatus *****/
#define AVRCP_DATA_PDU_GET_PLAY_STATUS_CMD_HEADER_SIZE      (0)
#define AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_HEADER_SIZE      (9)
#define AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SL_INDEX         (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_SP_INDEX         (4 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_GET_PLAY_STATUS_RES_PS_INDEX         (8 + AVRCP_DATA_MD_PARAM_INDEX)

/***** Request/AbortContinuingReponse *****/
#define AVRCP_DATA_PDU_CONTINUING_CMD_HEADER_SIZE           (1)
#define AVRCP_DATA_PDU_CONTINUING_CMD_PDU_ID_INDEX          (AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_CONTINUING_RES_HEADER_SIZE           (0) /* Only response for abort */

/* AVRCP error/status codes */
/* Metadata error codes - for use in responses */
#define AVRCP_DATA_STATUS_HEADER_SIZE                       (1)
#define AVRCP_DATA_STATUS_AVC_INDEX                         (AVRCP_DATA_MD_PARAM_INDEX)

/* Browsing specific */
#define AVRCP_DATA_BROWSING_PDU_ID_INDEX                    (0 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_BROWSING_PARAM_LEN_INDEX                 (1 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_BROWSING_PARAM_INDEX                     (3 + AVRCP_DATA_AVCTP_SINGLE_MI_INDEX)
#define AVRCP_DATA_BROWSING_HEADER_SIZE                     (6)
#define AVRCP_DATA_BROWSING_PDU_ID_GET(rxData)              ((rxData)[AVRCP_DATA_BROWSING_PDU_ID_INDEX])
#define AVRCP_DATA_BROWSING_PARAM_LEN_GET(rxData)           (CSR_GET_UINT16_FROM_BIG_ENDIAN(&rxData[AVRCP_DATA_BROWSING_PARAM_LEN_INDEX]))

#define AVRCP_DATA_ITEM_MP_PART_SIZE                        (31) /* + MP name */
#define AVRCP_DATA_ITEM_MP_ITEM_TYPE_INDEX                  (0)
#define AVRCP_DATA_ITEM_MP_ITEM_LENGTH_INDEX                (1)
#define AVRCP_DATA_ITEM_MP_PLAYER_ID_INDEX                  (3)
#define AVRCP_DATA_ITEM_MP_MAJOR_TYPE_INDEX                 (5)
#define AVRCP_DATA_ITEM_MP_SUB_TYPE_INDEX                   (6)
#define AVRCP_DATA_ITEM_MP_PLAY_STATUS_INDEX                (7)
#define AVRCP_DATA_ITEM_MP_FEATURE_MASK_INDEX               (8)
#define AVRCP_DATA_ITEM_MP_CHARSET_INDEX                    (24)
#define AVRCP_DATA_ITEM_MP_NAME_LEN_INDEX                   (26)
#define AVRCP_DATA_ITEM_MP_NAME_INDEX                       (28)

/***** SetAbsoluteVolume *****/
#define AVRCP_DATA_PDU_SET_VOLUME_CMD_HEADER_SIZE           (1)
#define AVRCP_DATA_PDU_SET_VOLUME_CMD_INDEX                 (0 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_VOLUME_RES_HEADER_SIZE           (1)
#define AVRCP_DATA_PDU_SET_VOLUME_RES_INDEX                 (0 + AVRCP_DATA_MD_PARAM_INDEX)

/***** SetAddressedPlayer *****/
#define AVRCP_DATA_PDU_SET_AP_CMD_HEADER_SIZE               (2)
#define AVRCP_DATA_PDU_SET_AP_CMD_PID_INDEX                 (0 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_AP_RES_HEADER_SIZE               (1)
#define AVRCP_DATA_PDU_SET_AP_RES_STATUS_INDEX              (0 + AVRCP_DATA_MD_PARAM_INDEX)

/***** SetBrowsedPlayer *****/
#define AVRCP_DATA_PDU_SET_BP_CMD_HEADER_SIZE               (2)
#define AVRCP_DATA_PDU_SET_BP_CMD_PID_INDEX                 (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_MIN_HEADER_SIZE           (10) /* + folderLen/Name pairs */
#define AVRCP_DATA_PDU_SET_BP_RES_STATUS_INDEX              (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_UID_COUNT_INDEX           (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_NUMBER_OF_ITEMS_INDEX     (3 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_CHARSET_INDEX             (7 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_FOLDER_DEPTH_INDEX        (9 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_FOLDER_PAIRS_INDEX        (10 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_RES_FOLDER_PAIRS_NAME_INDEX   (12 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SET_BP_FOLDER_LENGTH_SIZE            (2)

/***** GetFolderItems *****/
#define AVRCP_DATA_PDU_GFI_CMD_MIN_HEADER_SIZE              (11)
#define AVRCP_DATA_PDU_GFI_CMD_SCOPE_INDEX                  (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_CMD_START_INDEX                  (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_CMD_END_INDEX                    (5 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_CMD_ATT_COUNT_INDEX              (9 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_CMD_ATT_LIST_INDEX               (10 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_RES_MIN_HEADER_SIZE              (5)
#define AVRCP_DATA_PDU_GFI_RES_STATUS_INDEX                 (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_RES_UID_COUNT_INDEX              (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_RES_NUM_OF_ITEMS_INDEX           (3 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GFI_RES_ITEM_LIST_INDEX              (5 + AVRCP_DATA_BROWSING_PARAM_INDEX)

/***** ChangePath *****/
#define AVRCP_DATA_PDU_CHANGE_PATH_CMD_HEADER_SIZE          (11)
#define AVRCP_DATA_PDU_CHANGE_PATH_CMD_UID_COUNT_INDEX      (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_CHANGE_PATH_CMD_DIRECTION_INDEX      (2 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_CHANGE_PATH_CMD_FOLDER_UID_INDEX     (3 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_CHANGE_PATH_RES_HEADER_SIZE          (5)
#define AVRCP_DATA_PDU_CHANGE_PATH_RES_STATUS_INDEX         (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_CHANGE_PATH_RES_NUM_OF_ITEMS_INDEX   (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)

/***** GetItemAttributes *****/
#define AVRCP_DATA_PDU_GIA_CMD_MIN_HEADER_SIZE              (12)
#define AVRCP_DATA_PDU_GIA_CMD_SCOPE_INDEX                  (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GIA_CMD_UID_INDEX                    (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GIA_CMD_UID_COUNT_INDEX              (9 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GIA_CMD_ATTRIB_COUNT_INDEX           (11 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_GIA_CMD_ATTRIB_INDEX                 (12 + AVRCP_DATA_BROWSING_PARAM_INDEX)

#define AVRCP_DATA_PDU_GIA_RES_STATUS_INDEX                 (6)
#define AVRCP_DATA_PDU_GIA_RES_NOA_INDEX                    (7)
#define AVRCP_DATA_PDU_GIA_RES_MIN_HEADER_SIZE              (2)

/***** Search *****/
#define AVRCP_DATA_PDU_SEARCH_CMD_MIN_HEADER_SIZE           (4)
#define AVRCP_DATA_PDU_SEARCH_CMD_CHARSET_INDEX             (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SEARCH_CMD_LEN_INDEX                 (2 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SEARCH_CMD_SEARCH_TXT_INDEX          (4 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SEARCH_RES_HEADER_SIZE               (7)
#define AVRCP_DATA_PDU_SEARCH_RES_STATUS_INDEX              (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SEARCH_RES_UID_COUNT_INDEX           (1 + AVRCP_DATA_BROWSING_PARAM_INDEX)
#define AVRCP_DATA_PDU_SEARCH_RES_ITEM_COUNT_INDEX          (3 + AVRCP_DATA_BROWSING_PARAM_INDEX)

/***** PlayItem *****/
#define AVRCP_DATA_PDU_PLAY_ITEM_CMD_HEADER_SIZE            (11)
#define AVRCP_DATA_PDU_PLAY_ITEM_CMD_SCOPE_INDEX            (0 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_PLAY_ITEM_CMD_ITEM_UID_INDEX         (1 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_PLAY_ITEM_CMD_UID_COUNT_INDEX        (9 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_PLAY_ITEM_RES_HEADER_SIZE            (1)
#define AVRCP_DATA_PDU_PLAY_ITEM_RES_STATUS_INDEX           (0 + AVRCP_DATA_MD_PARAM_INDEX)

/***** AddToNowPlaying *****/
#define AVRCP_DATA_PDU_ADD_TO_NPL_CMD_HEADER_SIZE           (11)
#define AVRCP_DATA_PDU_ADD_TO_NPL_CMD_SCOPE_INDEX           (0 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_ADD_TO_NPL_CMD_ITEM_UID_INDEX        (1 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_ADD_TO_NPL_CMD_UID_COUNT_INDEX       (9 + AVRCP_DATA_MD_PARAM_INDEX)
#define AVRCP_DATA_PDU_ADD_TO_NPL_RES_HEADER_SIZE           (1)
#define AVRCP_DATA_PDU_ADD_TO_NPL_RES_STATUS_INDEX          (0 + AVRCP_DATA_MD_PARAM_INDEX)

/***** GeneralReject *****/
#define AVRCP_DATA_PDU_GENERAL_REJECT_RES_HEADER_SIZE       (1)
#define AVRCP_DATA_PDU_GENERAL_REJECT_RES_REASON_INDEX      (0 + AVRCP_DATA_BROWSING_PARAM_INDEX)



/***** Notifications *****/
#define AVRCP_NOTI_FLAG_CHANGE_ADDRESSED                    ((CsrBtAvrcpNotiMask)(CSR_BT_AVRCP_NOTI_FLAG_PLAYBACK_STATUS | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_TRACK | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_TRACK_END | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_TRACK_START | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_PLAYBACK_POS | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_PAS | \
                                                             CSR_BT_AVRCP_NOTI_FLAG_NOW_PLAYING_CONTENT))


#define AVRCP_NOTI_FLAG_CHANGE_BROWSED                      ((CsrBtAvrcpNotiMask)(CSR_BT_AVRCP_NOTI_FLAG_UIDS))


#ifdef __cplusplus
extern "C" }
#endif

#endif

