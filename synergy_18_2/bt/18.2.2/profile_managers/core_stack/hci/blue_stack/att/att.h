/*!
        (c) CSR plc 2010
        All rights reserved

\file   att.h

\brief  Attribute Protocol/Profile
*/
#ifdef INSTALL_ATT_MODULE
#ifndef    __ATT_H__
#define    __ATT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_FOR_ROBINSON
#include "att_prim.h"

/* att_l2cap.c */
extern void att_connect_req(ATT_CONNECT_REQ_T *req);
extern void att_disconnect_req(ATT_DISCONNECT_REQ_T *req);

/* att_client.c */
extern void att_exchange_mtu_req(ATT_EXCHANGE_MTU_REQ_T *req);
extern void att_find_info_req(ATT_FIND_INFO_REQ_T *req);
extern void att_find_by_type_value_req(ATT_FIND_BY_TYPE_VALUE_REQ_T *req);
extern void att_read_by_type_req(ATT_READ_BY_TYPE_REQ_T *req);
extern void att_read_req(ATT_READ_REQ_T *req);
extern void att_read_blob_req(ATT_READ_BLOB_REQ_T *req);
extern void att_read_multi_req(ATT_READ_MULTI_REQ_T *req);
extern void att_read_by_group_type_req(ATT_READ_BY_GROUP_TYPE_REQ_T *req);
extern void att_write_req(ATT_WRITE_REQ_T *req);
extern void att_prepare_write_req(ATT_PREPARE_WRITE_REQ_T *req);
extern void att_execute_write_req(ATT_EXECUTE_WRITE_REQ_T *req);
extern void att_handle_value_rsp(ATT_HANDLE_VALUE_RSP_T *req);

/* att_server.c */
extern void att_exchange_mtu_rsp(ATT_EXCHANGE_MTU_RSP_T *req);
extern void att_add_db_req(ATT_ADD_DB_REQ_T *req);
extern void att_handle_value_req(ATT_HANDLE_VALUE_REQ_T *req);
extern void att_access_rsp(ATT_ACCESS_RSP_T *req);

extern void att_init(void **gash);

#else /* BUILD_FOR_ROBINSON */
/* att.c */
extern void att_handler(void **gash);
#endif /* !BUILD_FOR_ROBINSON */


#ifdef __cplusplus
}
#endif

#endif    /* __ATT_H__ */
#endif  /* INSTALL_ATT_MODULE */
