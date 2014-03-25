#ifndef _PEER_COM_LINUX_U2U_H
#define _PEER_COM_LINUX_U2U_H
/****************************************************************************

                         © Cambridge Silicon Radio Ltd, 2004
 	 			
                                   All rights reserved

FILE: peer_com_linux_u2u.h

DESCRIPTION:   Definitions used by the user <> user peer communication

REVISION:  $Revision: #1 $ by $Author: jk05 $

****************************************************************************/



#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_EXTERNAL_SRV_PORT 34000

/**
 * Initializes peer communication
 *
 */
extern void peer_server_com_init(int port);

/**
 * Deinitializes peer communication
 *
 */
extern void peer_server_com_deinit(void);
extern CsrBool csr_bt_server_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 evt, void *data, CsrUint16 data_len );



extern void peer_get_external_conect(void);
/**
 * Initializes peer communication
 *
 */

typedef  void (*csrBtClientCB)(void);
extern CsrBool peer_client_com_init(int port,csrBtClientCB errCb);
/**
 * Deinitializes peer communication
 *
 */
extern CsrBool peer_client_com_deinit(void);
extern CsrInt16 csr_bt_client_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 evt, void *data, CsrUint16 data_len );

extern  void csr_bt_client_app_set_error_state(CsrBool isError);

#ifdef __cplusplus
}
#endif 

#endif//_PEER_COM_LINUX_U2U_H