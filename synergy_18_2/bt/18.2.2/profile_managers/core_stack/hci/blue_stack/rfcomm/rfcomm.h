/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm.h
\brief  Public header for modules which wish to use RFCOMM.
*/

#ifndef _RFCOMM_H_
#define _RFCOMM_H_

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_mblk.h"

#ifdef __cplusplus
extern "C" {
#endif

void rfc_init(void **control_data);
#ifdef ENABLE_SHUTDOWN
void rfc_deinit(void **control_data);
#endif
void rfc_iface_handler(void **control_data);

/* Stream interface.
*/
CsrBool rfc_stream_get_bd_addr_by_conn_id(void *control_data,
                                         CsrUint16 conn_id,
                                         TYPED_BD_ADDR_T *addrt);
void rfc_stream_credits_or_data_available(void *control_data,
                                          CsrUint16 conn_id,
                                          CsrMblk *payload,
                                          CsrUint16 rx_credits);

#ifdef __cplusplus
}
#endif

#endif /* _RFCOMM_H_ */
