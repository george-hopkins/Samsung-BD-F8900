/**********************************************************************
* File Name   : Bluetooth_CM.h
* Description : This file define functions and variables for Connection Manager.
* Updates			:
***********************************************************************/

#ifndef _BLUETOOTH_IP_H_
#define _BLUETOOTH_IP_H_

#include "csr_types.h"


extern CsrBool csr_bt_server_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 cmd, void *data, CsrUint16 data_len );

extern void handleServerSocket(CsrPrim    *thePrim);


#endif /*_BLUETOOTH_IP_H_*/
