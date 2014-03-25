/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_private.h

\brief  Internal header for Attribute Protocol
*/
#ifndef    __ATT_PRIVATE_H__
#define    __ATT_PRIVATE_H__

#include "csr_util.h"

#ifndef BUILD_FOR_ROBINSON

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "att.h"
#include "attlib.h"
#include "att_prim.h"
#include "l2cap_prim.h"
#include "l2caplib.h"
#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "error.h"
#include "csr_bt_panic.h"
#include "bkeyval.h"
#include "csr_bt_tasks.h"
#include "tbdaddr.h"
#include "dm_security_manager.h"

#else /* !BUILD_FOR_ROBINSON */

#include "bluetooth.h"
#include "att.h"
#include "att_prim.h"

#include "bt_types.h"
#include "att_prim.h"
#include "l2cap.h"
#include "att_l2cap_if.h"
#include "macros.h"
#include "buf_utils.h"
#include "timer.h"

#endif /* BUILD_FOR_ROBINSON */

#ifdef __cplusplus
extern "C" {
#endif

#define ATT_TIMEOUT             30 /* ATT timeout in seconds, 0 to disable */

/* Define to allow reliable (and long) writes if any write access is
 * allowed. See B-92058 for more information. */
#define DO_NOT_HONOR_RELIABLE_PERMISSIONS

#ifndef BUILD_FOR_ROBINSON
#define ATT_QUEUE_SIZE          64 /* size of prepare write queue */
#define ATT_INVALID_CID         0
#define WQ_SIZE(conn)           (psizeof((conn)->wq))
#else /* !BUILD_FOR_ROBINSON */
#define ATT_QUEUE_SIZE          35 /* size of prepare write queue */
#define WQ_SIZE(conn)           ATT_QUEUE_SIZE
#define ATT_MAX_BLE_U_CONNS     2 /* Maximum number of BLE-U connections */
#define ATT_INVALID_CID         0xffff
#define BITFIELD(type, name, size)  unsigned int name:size
#define    CSR_SCHED_MILLISECOND    ((CsrUint32)(1000))
#define    CSR_SCHED_SECOND        ((CsrUint32)(1000 * CSR_SCHED_MILLISECOND))
#define    CSR_SCHED_MINUTE        ((CsrUint32)(60 * CSR_SCHED_SECOND))
typedef timer_id CsrSchedTid ;
typedef CsrUint16    l2ca_cid_t;
#endif /* BUILD_FOR_ROBINSON */

/* enable to store handle in attribute database */
/*#define ATT_STORE_HANDLE*/

/*! \brief Attribute protocol opcodes */
typedef enum {
    ATT_OP_ERROR_RSP            = 0x01,
    ATT_OP_EXCHANGE_MTU_REQ,    /* 02 */
    ATT_OP_EXCHANGE_MTU_RSP,    /* 03 */
    ATT_OP_FIND_INFO_REQ,       /* 04 */
    ATT_OP_FIND_INFO_RSP,       /* 05 */
    ATT_OP_FIND_BY_TYPE_VALUE_REQ, /* 06 */
    ATT_OP_FIND_BY_TYPE_VALUE_RSP, /* 07 */
    ATT_OP_READ_BY_TYPE_REQ,    /* 08 */
    ATT_OP_READ_BY_TYPE_RSP,    /* 09 */
    ATT_OP_READ_REQ,            /* 0a */
    ATT_OP_READ_RSP,            /* 0b */
    ATT_OP_READ_BLOB_REQ,       /* 0c */
    ATT_OP_READ_BLOB_RSP,       /* 0d */
    ATT_OP_READ_MULTI_REQ,      /* 0e */
    ATT_OP_READ_MULTI_RSP,      /* 0f */
    ATT_OP_READ_BY_GROUP_TYPE_REQ, /* 10 */
    ATT_OP_READ_BY_GROUP_TYPE_RSP, /* 11 */
    ATT_OP_WRITE_REQ,           /* 12 */
    ATT_OP_WRITE_RSP,           /* 13 */
    ATT_OP_PREPARE_WRITE_REQ    = 0x16,
    ATT_OP_PREPARE_WRITE_RSP,   /* 17 */
    ATT_OP_EXECUTE_WRITE_REQ,   /* 18 */
    ATT_OP_EXECUTE_WRITE_RSP,   /* 19 */
    ATT_OP_HANDLE_VALUE_NOT     = 0x1b,
    ATT_OP_HANDLE_VALUE_IND     = 0x1d,
    ATT_OP_HANDLE_VALUE_CFM,    /* 1e */

    ATT_OP_LAST
} att_opcode_t;

#define ATT_OP_RSP_MASK         0x01 /* bit 0 is set in responses */
#define ATT_OP_AUTH_FLAG        0x80 /* bit 7 means there is a signature */
#define ATT_OP_CMD_FLAG         0x40 /* bit 6 means this is a command */
#define ATT_OP_FLAGS            (ATT_OP_AUTH_FLAG | ATT_OP_CMD_FLAG)
#define ATT_OP_MASK             (~ATT_OP_FLAGS)
#define ATT_OP_AUTH_LEN         12 /* Authentication signature length */

/*! \brief Attribute Protocol configuration */
#ifndef BUILD_FOR_HOST
#define ATT_MTU_BREDR           48
#else /* !BUILD_FOR_HOST */
#define ATT_MTU_BREDR           L2CA_MTU_DEFAULT /* 672 */
#endif /* BUILD_FOR_HOST */
#define ATT_MTU_BLE             23
#define ATT_MTU_MIN             ATT_MTU_BLE
#define ATT_MTU_MAX             527 /* see B-90345 / dg 739220 */

#define ATT_PSM                 0x001f

#define ATT_SIZE_DECLARATION    0x0004  /* Size of a declaration record */
#define ATT_SIZE_DECLARATION128 0x000b  /* Size of a declaration record */
#define ATT_PERM_EXT(x) ((x) << 8) /* put ext perms after ATT_PERM_EXTENDED */

#ifndef BUILD_FOR_HOST
typedef struct
{
    BITFIELD(CsrUint8,           type, 4);       /* type */
    BITFIELD(CsrUint8,           flags, 4);      /* flags, see ATT_ATTR_* */
    BITFIELD(CsrUint8,           size_value, 8); /* data length in octets */

    union
    {
        CsrUint16                data[1]; /* placeholder for data only */
        att_attr_full_t         full;
        att_attr_full128_t      full128;
    } v; /* value */
} att_attr_t;
#endif /* !BUILD_FOR_HOST */


/*! Attribute value maximum length in bytes */
#define ATT_LEN_MAX             0x0200

/* Do not use global variables on Host */
#ifndef BUILD_FOR_HOST
#define ATT_GLOBAL_STATE
#endif

#ifdef ATT_GLOBAL_STATE
#define ATT_FUNC_STATE
#define ATT_ARG
#define ATT_STATE(statevar) (att.statevar)
#define ATT_HANDLER_PARAMS (       \
        struct att_conn_tag *conn, \
        ATT_ACCESS_RSP_T *irq)
#else /* !BUILD_FOR_HOST */
struct att_state_tag;
#define ATT_FUNC_STATE struct att_state_tag *att,
#define ATT_ARG att,
#define ATT_STATE(statevar) (att->statevar)
#define ATT_HANDLER_PARAMS (                    \
        struct att_state_tag *att,              \
        struct att_conn_tag *conn,              \
        ATT_ACCESS_RSP_T *irq)
#endif /* BUILD_FOR_HOST */

/* Handler function for ATT over the air opcode */
#define ATT_HANDLER(func)                       \
    CsrBool att_handle_##func ATT_HANDLER_PARAMS

typedef struct att_writeq_tag
{
    CsrUint16                    handle; /* Attribute handle to write */
    CsrUint16                    offs; /* data offset */
    CsrUint16                    len; /* length of data in bytes */
#ifndef BUILD_FOR_HOST
    CsrUint16                    data[1]; /* packed data */
#else /* !BUILD_FOR_HOST */
    struct att_writeq_tag       *next; /* pointer to next value */
    CsrUint8                     value[1]; /* raw data */
#endif /* BUILD_FOR_HOST */
} att_writeq_t;

typedef struct att_in_tag
{
        CsrUint8 *buf;           /* data buffer */
#ifndef BUILD_FOR_ROBINSON
        const CsrUint8 *data;    /* buffer data position */
#else /* !BUILD_FOR_ROBINSON */
        CsrUint8 *data;          /* buffer data position */
#endif /* BUILD_FOR_ROBINSON */
        CsrUint16 len;           /* buffer size */
#ifdef BUILD_FOR_HOST
        struct att_in_tag *next;/* pointer to next node */
#endif
} att_in_t;

typedef struct att_conn_tag
{
#ifndef BUILD_FOR_ROBINSON
    struct att_conn_tag *next;
#endif /* !BUILD_FOR_ROBINSON */
    l2ca_cid_t cid;             /* L2CAP channel identifier */
    CsrUint16 flags;             /* connection flags */
    CsrUint16 mtu;               /* maximum ATT mtu */
#ifndef BUILD_FOR_HOST
    CsrUint16 size_wq;           /* prepare queue size for remote */
#endif /* BUILD_FOR_HOST */
#ifndef BUILD_FOR_ROBINSON
    att_writeq_t *wq;           /* prepare write queue */
#else /* !BUILD_FOR_ROBINSON */
    att_writeq_t wq[ATT_QUEUE_SIZE]; /* prepare write queue */
#endif /* BUILD_FOR_ROBINSON */
    CsrUint16 pend;              /* pending operation */
    CsrBool (*handler) ATT_HANDLER_PARAMS; /* pending packet handler */
#if ATT_TIMEOUT
    CsrSchedTid timer;                  /* Timer */
#endif

    att_in_t in;                /* Incoming data buffer */

    struct                      /* Outgoing data buffer */
    {
        CsrUint8 *buf;           /* data buffer */
        CsrUint8 *data;          /* buffer data position */
    } out;

#ifdef BUILD_FOR_HOST /* Extra stuff for mainframes with unlimited memory */
    TYPED_BD_ADDR_T addrt;              /* Address of the remote device */
#endif

#ifdef INSTALL_ATT_BREDR
    l2ca_identifier_t   identifier;       /*!< Used to identify the connect signal */
    CsrUint16            con_ctx;          /*!< Opaque context number returned in other signals */
#endif
} att_conn_t;

typedef struct att_state_tag
{
    CsrSchedQid phandle;          /* application phandle */
    att_attr_t *attr;           /* attribute database */
    CsrUint16 size_attr;         /* size of the attribute database */
#ifndef BUILD_FOR_ROBINSON
    att_conn_t *conn;           /* list of current connections */
#else /* !BUILD_FOR_ROBINSON */
    att_conn_t conn[ATT_MAX_BLE_U_CONNS];           /* list of current connections */
#endif /* BUILD_FOR_ROBINSON */
} ATT_T;

typedef struct att_handler_tag
{
    BITFIELD(CsrUint8, min, 7); /* minimum PDU size */
    BITFIELD(CsrUint8, max, 8); /* maximum PDU size */
    BITFIELD(CsrUint8, cmd, 1); /* Can this be a command? */
    CsrBool (*handler) ATT_HANDLER_PARAMS;
} att_handler_t;

extern const att_handler_t att_handlers[];

extern ATT_T att;
extern const CsrUint32 att_base_uuid[];

/* att.c */
#ifndef BUILD_FOR_ROBINSON
extern void att_send_message(CsrSchedQid queue_id, CsrUint16 msg_id, void *prim);
#endif /* !BUILD_FOR_ROBINSON */
#if ATT_TIMEOUT
#ifndef BUILD_FOR_ROBINSON
extern void att_timer_start(att_conn_t *conn);
#define att_timer_cancel() timer_cancel(&conn->timer)
extern void att_timer_expired(CsrUint16 mi, void *mv);
#else /* !BUILD_FOR_ROBINSON */
#define att_timer_start(c) (conn->timer == 0 ? \
                            conn->timer = timer_create(ATT_TIMEOUT * CSR_SCHED_SECOND, \
                                        TRUE, att_timer_expired) : 0)
#define att_timer_cancel() do { \
            timer_delete(conn->timer); \
            conn->timer = 0; \
            } while(0)
extern void att_timer_expired(timer_id const id);
#endif /* BUILD_FOR_ROBINSON */
#else
#define att_timer_start(conn)
#define att_timer_cancel(conn)
#endif
extern CsrBool att_pend_set(att_conn_t *conn, CsrUint8 op);
extern CsrBool att_pend_check(att_conn_t *conn, CsrUint8 op);

/* att_client.c */
#ifndef BUILD_FOR_ROBINSON
extern void att_exchange_mtu_req(ATT_FUNC_STATE
                                 ATT_EXCHANGE_MTU_REQ_T *req);
extern void att_find_info_req(ATT_FUNC_STATE
                              ATT_FIND_INFO_REQ_T *req);
extern void att_find_by_type_value_req(ATT_FUNC_STATE
                                       ATT_FIND_BY_TYPE_VALUE_REQ_T *req);
extern void att_read_by_type_req(ATT_FUNC_STATE
                                 ATT_READ_BY_TYPE_REQ_T *req);
extern void att_read_req(ATT_FUNC_STATE
                         ATT_READ_REQ_T *req);
extern void att_read_blob_req(ATT_FUNC_STATE
                              ATT_READ_BLOB_REQ_T *req);
extern void att_read_multi_req(ATT_FUNC_STATE
                               ATT_READ_MULTI_REQ_T *req);
extern void att_read_by_group_type_req(ATT_FUNC_STATE
                                       ATT_READ_BY_GROUP_TYPE_REQ_T *req);
extern void att_write_req(ATT_FUNC_STATE
                          ATT_WRITE_REQ_T *req);
extern void att_prepare_write_req(ATT_FUNC_STATE
                                  ATT_PREPARE_WRITE_REQ_T *req);
extern void att_execute_write_req(ATT_FUNC_STATE
                                  ATT_EXECUTE_WRITE_REQ_T *req);
extern void att_handle_value_rsp(ATT_FUNC_STATE
                                 ATT_HANDLE_VALUE_RSP_T *req);
#endif /* !BUILD_FOR_ROBINSON */

extern ATT_HANDLER(error_rsp);
extern ATT_HANDLER(exchange_mtu_rsp);
extern ATT_HANDLER(find_info_rsp);
extern ATT_HANDLER(find_by_type_value_rsp);
extern ATT_HANDLER(read_by_type_rsp);
extern ATT_HANDLER(read_common_rsp);
extern ATT_HANDLER(read_by_group_type_rsp);
extern ATT_HANDLER(write_common_rsp);
extern ATT_HANDLER(execute_write_rsp);

/* att_db.c */
extern CsrBool att_attr_isaux(att_attr_t *attr);
extern CsrBool att_attr_isgroup(att_attr_t *attr);

/* att_hostdb.c / att_onchipdb.c */
extern att_result_t att_attr_add(ATT_FUNC_STATE
                                 att_attr_t **attrp, CsrUint16 size_attr);
extern att_attr_t *att_attr_find(ATT_FUNC_STATE
                                 CsrUint16 handle, CsrUint16 *found);
#ifndef BUILD_FOR_HOST
extern CsrUint16 att_attr_flags(att_attr_t *attr);
extern CsrBool att_attr_isuuid16(att_attr_t *attr);
extern void attr_unpack_CsrMemCpy(CsrUint8 *dest, const CsrUint16* src, CsrUint16 len, CsrUint16 offset);
#endif
extern CsrBool att_attr_match(att_attr_t *attr, const CsrUint8 *data);
extern CsrBool att_attr_match_uuid(att_attr_t *attr, CsrUint32 *u);
extern CsrBool att_attr_match_uuid128(att_attr_t *attr, CsrUint32 *uuid);
extern att_attr_t *att_attr_next(ATT_FUNC_STATE
                                 att_attr_t *attr, CsrUint16 *handle);
extern CsrUint16 att_attr_perm(ATT_FUNC_STATE
                              att_attr_t *attr);
extern CsrUint16 att_attr_perm_ext(ATT_FUNC_STATE
                                  att_attr_t *attr);
extern CsrUint16 att_attr_read(CsrUint8 **data, att_attr_t *attr, CsrUint16 offs, CsrUint16 len);
extern void att_attr_read_uuid(CsrUint8 **data, att_attr_t *attr);
CsrUint16 att_attr_remove(ATT_FUNC_STATE
                         CsrUint16 handle, CsrUint16 endh);
extern att_result_t att_attr_set(ATT_FUNC_STATE
                                 att_attr_t **attrp, CsrUint16 offs,
                                 CsrUint16 len, const CsrUint8 *v);

#ifndef BUILD_FOR_HOST
extern CsrUint16 att_attr_size(att_attr_t *attr);
#endif
extern att_result_t att_wq_add(att_conn_t *conn, CsrUint16 handle, CsrUint16 offs, CsrUint16 len, const CsrUint8 *d);
#ifndef BUILD_FOR_HOST
extern att_writeq_t *att_wq_next(att_conn_t *conn, att_writeq_t *wq);
#endif
extern void att_wq_free(att_conn_t *conn);

#ifndef BUILD_FOR_HOST
extern CsrUint16 *att_attr_value(att_attr_t *attr);
#endif

#ifdef INSTALL_ATT_BREDR
extern void att_connect_rsp(ATT_FUNC_STATE
                    ATT_CONNECT_RSP_T *rsp);
#endif

#ifdef BUILD_FOR_HOST
#define att_attr_flags(attr) ((attr)->flags)
#define att_attr_isuuid16(attr) ATT_IS_UUID16(attr->uuid)
#define att_attr_size(attr) ((CsrUint16)(attr)->size_value)
#define att_attr_value(attr) (attr->value)
#define att_wq_next(conn, wq) (wq->next)
#endif /* BUILD_FOR_HOST */

/* att_interface.c */
extern void att_dm_sm_data_sign_req(att_conn_t *conn, CsrBool verify,
                                    CsrUint16 size_data, CsrUint8 *data);

/* att_l2cap.c */
extern att_conn_t *att_conn_add(ATT_FUNC_STATE
                                l2ca_cid_t cid);
extern att_conn_t *att_conn_find(ATT_FUNC_STATE
                                 l2ca_cid_t cid);
#ifndef BUILD_FOR_ROBINSON
extern void att_connect_req(ATT_FUNC_STATE
                            ATT_CONNECT_REQ_T *req);
extern void att_disconnect_req(ATT_FUNC_STATE
                               ATT_DISCONNECT_REQ_T *req);
extern void att_l2ca_dataread_ind(ATT_FUNC_STATE
                                  L2CA_DATAREAD_IND_T *ind, att_conn_t *c);
extern void att_l2cap_handler(ATT_FUNC_STATE
                              CsrUint16 type, L2CA_UPRIM_T *prim);
#else /* !BUILD_FOR_ROBINSON */
extern void att_l2ca_dataread_ind(L2CA_DATA_READ_IND_T *ind, att_conn_c *c);
#endif /* BUILD_FOR_ROBINSON */
extern void att_disconnect(att_conn_t *conn);
extern CsrUint16 att_pkt_create(att_conn_t *conn, CsrUint8 op);
extern CsrUint16 att_pkt_write(att_conn_t *conn,
                              const CsrUint8 *value,
                              CsrUint16 size_value);
extern void att_pkt_send(att_conn_t *conn);
extern CsrUint16 att_read_uuid(att_conn_t *conn, CsrUint32 *uuid,
                              CsrUint16 uuid_type);
extern void att_conn_rm(ATT_FUNC_STATE
                        l2ca_cid_t cid);

/* att_server.c */
#ifndef BUILD_FOR_ROBINSON
extern void att_exchange_mtu_rsp(ATT_FUNC_STATE
                                 ATT_EXCHANGE_MTU_RSP_T *req);
extern void att_register_req(ATT_FUNC_STATE
                             ATT_REGISTER_REQ_T *req);
extern void att_unregister_req(ATT_FUNC_STATE
                               ATT_UNREGISTER_REQ_T *req);
#ifndef BUILD_FOR_HOST
extern void att_add_db_req(ATT_FUNC_STATE
                           ATT_ADD_DB_REQ_T *req);
#else /* !BUILD_FOR_HOST */
extern void att_add_req(ATT_FUNC_STATE
                        ATT_ADD_REQ_T *req);
extern void att_remove_req(ATT_FUNC_STATE
                           ATT_REMOVE_REQ_T *req);
#endif /* BUILD_FOR_HOST */
extern void att_handle_value_req(ATT_FUNC_STATE
                                 ATT_HANDLE_VALUE_REQ_T *req);
extern void att_access_rsp(ATT_FUNC_STATE
                           ATT_ACCESS_RSP_T *req);
#endif /* !BUILD_FOR_ROBINSON */

extern ATT_HANDLER(exchange_mtu_req);
extern ATT_HANDLER(find_info_req);
extern ATT_HANDLER(read_by_common_req);
extern ATT_HANDLER(read_common_req);
extern ATT_HANDLER(read_multi_req);
extern ATT_HANDLER(write_common_req);
extern ATT_HANDLER(prepare_write_req);
extern ATT_HANDLER(execute_write_req);
extern ATT_HANDLER(handle_value_cfm);
extern void att_send_error_rsp(att_conn_t *conn, CsrUint8 op, CsrUint16 handle,
                               att_result_t err);
#ifdef BUILD_FOR_HOST
extern void att_check_pending_queue(ATT_FUNC_STATE
                                   att_conn_t *conn);
extern CsrBool att_process_queued_ind(ATT_FUNC_STATE
                                   att_conn_t *conn);
#endif
extern const att_handler_t *get_pkt(CsrUint8 *op, att_conn_t *conn);

/* att_upstream.c */
extern void att_register_cfm(CsrSchedQid phandle, att_result_t rc);
extern void att_unregister_cfm(CsrSchedQid phandle, att_result_t rc);
#ifndef BUILD_FOR_HOST
extern void att_add_db_cfm(CsrSchedQid phandle, att_result_t rc);
#else /* !BUILD_FOR_HOST */
extern void att_add_cfm(CsrSchedQid phandle, att_result_t rc);
extern void att_remove_cfm(CsrSchedQid phandle, CsrUint16 num_attr,
                           att_result_t rc);
#endif /* BUILD_FOR_HOST */
extern void att_connect_cfm(CsrSchedQid phandle, TYPED_BD_ADDR_T *addr,
                            l2ca_conflags_t flags, CsrUint16 mtu,
                            l2ca_cid_t cid, l2ca_conn_result_t result);
extern void att_connect_ind(CsrSchedQid phandle, TYPED_BD_ADDR_T *addr,
                            l2ca_conflags_t flags, CsrUint16 mtu,
                            l2ca_cid_t cid);
extern void att_disconnect_ind(CsrSchedQid phandle, CsrUint16 cid,
                               l2ca_disc_result_t reason);
extern void att_exchange_mtu_ind(CsrSchedQid phandle, l2ca_cid_t cid,
                                 CsrUint16 mtu);
extern void att_exchange_mtu_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                 CsrUint16 mtu, att_result_t rc);
extern void att_find_info_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                              CsrUint16 handle, att_uuid_type_t uuid_type,
                              CsrUint32 *uuid, att_result_t rc);
extern void att_find_by_type_value_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                       CsrUint16 handle, CsrUint16 end,
                                       att_result_t rc);
extern void att_read_by_type_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                 CsrUint16 handle, att_result_t rc,
                                 CsrUint16 size_value, CsrUint8 *value);
extern void att_read_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc,
                         CsrUint16 size_value, CsrUint8 *value);
extern void att_read_blob_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                              att_result_t rc, CsrUint16 size_value,
                              CsrUint8 *value);
extern void att_read_multi_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                               att_result_t rc, CsrUint16 size_value,
                               CsrUint8 *value);
extern void att_read_by_group_type_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                       att_result_t rc, CsrUint16 handle,
                                       CsrUint16 end, CsrUint16 size_value,
                                       CsrUint8 *value);
extern void att_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc);
extern void att_prepare_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                  CsrUint16 handle, CsrUint16 offset,
                                  att_result_t rc, CsrUint16 size_value,
                                  CsrUint8 *value);
extern void att_execute_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                  CsrUint16 handle, att_result_t rc);
extern void att_handle_value_ind(CsrSchedQid phandle, l2ca_cid_t cid,
                                 CsrUint16 handle, CsrUint16 flags,
                                 CsrUint16 size_value, CsrUint8 *value);
extern void att_handle_value_cfm(CsrSchedQid phandle, l2ca_cid_t cid,
                                 att_result_t rc);
extern void att_access_ind(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle,
                           CsrUint16 flags, CsrUint16 offs, CsrUint16 size_value,
                           CsrUint8 *value);
extern void att_debug_ind(ATT_FUNC_STATE
                          const char *msg, CsrUint16 len);
extern void att_error_cfm(CsrSchedQid phandle, att_conn_t *conn, CsrUint8 op,
                          CsrUint16 handle, att_result_t rc);

extern att_result_t att_attr_exe_set(ATT_FUNC_STATE
                          att_attr_t **attrp, att_writeq_t *wq);

#ifndef BUILD_FOR_ROBINSON
#define MAKE_PRIM(TYPE) \
    TYPE##_T *prim = zpnew(TYPE##_T); prim->type = TYPE;

#define SEND_PRIM() \
    att_send_message(prim->phandle, ATT_PRIM, prim)
#else /* !BUILD_FOR_ROBINSON */
/* 'attUpPrim' variable is used as a memory block to send ATT primitives to higher layer */
extern ATT_UPRIM_T               att_hl_prim;

/* Defined by application using ATT protocol */
extern void app_att_mssg_handler(ATT_UPRIM_T *prim);

#define MAKE_PRIM(TYPE) \
    TYPE##_T *prim = (TYPE##_T *)&att_hl_prim; prim->type = TYPE;

/* Use application handler for ATT messages */
#define SEND_PRIM() \
    app_att_mssg_handler((ATT_UPRIM_T *)prim)

#define BLUESTACK_PANIC(err_id)

extern void att_disconnect_cfm(CsrSchedQid phandle, CsrUint16 cid, l2ca_disc_result_t reason);
#endif /* BUILD_FOR_ROBINSON */

/* Check if UUID128 can be represented in 16 bits */
#define ATT_IS_UUID16(u)                        \
    (!((u)[0] & att_base_uuid[0]) &&            \
     (u)[1] == att_base_uuid[1] &&              \
     (u)[2] == att_base_uuid[2] &&              \
     (u)[3] == att_base_uuid[3])

/* protocol debug */
#if 1
#define DEBUG_PROT(x) x
#else
#define DEBUG_PROT(x)
#endif

/* type for uuid match function pointer */
typedef CsrBool (*uuid_match_t) (att_attr_t *, CsrUint32 *);

/* ATT protocol flow control
 *
 * conn->pend = xxyy
 * xx = pending request (server)
 * yy = pending response (client)
 */
#define PEND_OP_MASK    0x1f /* Mask for pending opcode */
#define PEND_REQ()      (conn->pend & 0xff00)
#define PEND_GET_REQ()  ((conn->pend >> 8) & PEND_OP_MASK)
#define PEND_SET_REQ(p) (conn->pend = \
                         (conn->pend & ~(PEND_OP_MASK << 8)) | ((p) << 8))
#define PEND_RSP()      (conn->pend & 0x00ff)
#define PEND_GET_RSP()  (conn->pend & PEND_OP_MASK)
#define PEND_SET_RSP(p) (conn->pend = \
                         (conn->pend & ~PEND_OP_MASK) | (p))
#define PEND_NONE       0x0000 /* not waiting for anything */
#define PEND_HANDLE_CFM 0x0080 /* waiting for handle value cfm */
#define PEND_HANDLE_RSP 0x8000 /* waiting for app to ack handle value ind */
#define PEND_AUTH_CHECK 0x0040 /* waiting for signature check */
#define PEND_AUTH_SIGN  0x4000 /* waiting for data sign to finish */

/* ATT buffer handling */
#define IN_BUF_LEN(conn) (CsrUint16)((conn)->in.len - ((conn)->in.data - (conn)->in.buf))

#define GET_IN_PDU(conn) ((conn)->in.buf[0])
#define GET_IN_OP(conn) ((CsrUint8)(GET_IN_PDU(conn) & ATT_OP_MASK))


#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
#define TIME_LAPSE_ENCRYPTION_ATT_CMD 10 /* in milliseconds */
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */

#ifdef __cplusplus
}
#endif

#endif /* __ATT_PRIVATE_H__ */
