/**********************************************************************
* File Name   : Bluetooth_CM.c
* Description : This file contains functions and variables for
*                Connection Manager.
* Author 	  :	Kim Jin Cheol	(Samsung)
* Updates			:
***********************************************************************/
#include "csr_bt_app.h"
#include "csr_ip_socket_lib.h"
#include "csr_bt_samsung_app.h"	
#include "csr_bt_ss_prim.h"	


void handleSocket(CsrPrim    *thePrim);


static CsrUint16       socketHandle=0;

#define LOCAL_ADDR       {127,0,0,1}
#define SERVER_PORT       34000


CsrBool peer_get_message(CsrUint8       *data,CsrUint16       dataLength )
{
	CsrUint16 id;
	CsrUint16 cmd; 
	void *buf;
	CsrUint16 payload_length;
	

    payload_length=*(CsrUint16 *)&data[0];
    id = *(CsrUint16 *)&data[2];
    cmd = *(CsrUint16 *)&data[4];
	buf =&data[6];


////////////////////////////////////////handle message from client process ///////////
	switch(id)
	{
	    case CSR_GAP_SS_ID:
		{
			handle_gap_server_cmd_handler(cmd, buf, payload_length);
			
		}
		break;
		case CSR_AV_SS_ID:
		{
			handle_av_server_cmd_handler(cmd, buf, payload_length);
			
		}
		break;
		case CSR_AVRCP_SS_ID:
		{
			handle_avrc_server_cmd_handler(cmd, buf, payload_length);
			
		}
		break;
		
	}

	return TRUE;
}

CsrBool csr_bt_server_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 cmd, void *data, CsrUint16 data_len )//csr_bt_server_app_put_message_to_tx_thread
{
	CsrSize datalen=0;
    CsrUint8 *send_data=NULL; 

	send_data=CsrPmemAlloc(data_len+6);
	
    data_len=*(CsrUint16 *)&send_data[0];
    id = *(CsrUint16 *)&send_data[2];
    cmd = *(CsrUint16 *)&send_data[4];
	data = *(CsrUint16 *)&send_data[6];

	*(CsrUint16 *)&send_data[0]=data_len;
	*(CsrUint16 *)&send_data[2]=id;
	*(CsrUint16 *)&send_data[4]=cmd;
	CsrMemCpy(&send_data[6],data,data_len);
	
						

	if(datalen)
	{
		CsrMemCpy((send_data+6),data,data_len);

	   CsrPmemFree(data);
	    
	   data=NULL;
	}
	
    CsrIpSocketTcpDataReqSend(socketHandle,(data_len+6),send_data);
    return TRUE;
}	




void handleServerSocket(CsrPrim    *thePrim)
{
  switch (*thePrim)
  {

    case CSR_IP_SOCKET_UDP_NEW_CFM:
    {
        CsrIpSocketUdpNewCfm *prim= (CsrIpSocketUdpNewCfm *)thePrim;
		CsrUint8 address[4]={127,0,0,1};

        printf("CSR_IP_SOCKET_UDP_NEW_CFM [%d]\n", prim->result);
        CsrIpSocketUdpBindReqSend(prim->socketHandle,address,SERVER_PORT);
        break;
    }
    case CSR_IP_SOCKET_UDP_BIND_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_UDP_DATA_IND:
    {

        break;
    }
    case CSR_IP_SOCKET_UDP_DATA_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM:
    {

        break;
    }


    case CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_TCP_NEW_CFM:
    {
        CsrIpSocketTcpNewCfm *prim= (CsrIpSocketTcpNewCfm *)thePrim;
        CsrUint8 address[4]={127,0,0,1};
        printf("CSR_IP_SOCKET_TCP_NEW_CFM [%d] socket handle[%d]\n", prim->result,prim->socketHandle);
	
        CsrIpSocketTcpBindReqSend(prim->socketHandle,address,SERVER_PORT);
        break;
    }
    case CSR_IP_SOCKET_TCP_BIND_CFM:
    {

        CsrIpSocketTcpBindCfm *prim= (CsrIpSocketTcpBindCfm *)thePrim;
        
        printf("CSR_IP_SOCKET_TCP_BIND_CFM [%d]socket handle[%d]\n", prim->result,prim->socketHandle);
        CsrIpSocketTcpListenReqSend(prim->socketHandle);

        break;
    }
    case CSR_IP_SOCKET_TCP_LISTEN_CFM:
    {
		CsrIpSocketTcpListenCfm *prim= (CsrIpSocketTcpListenCfm *)thePrim;
        printf("CSR_IP_SOCKET_TCP_LISTEN_CFM [%d]socket handle[%d]\n", prim->result,prim->socketHandle);
        break;
    }
    case CSR_IP_SOCKET_TCP_CONNECT_CFM:
    {
		CsrIpSocketTcpConnectCfm *prim= (CsrIpSocketTcpConnectCfm *)thePrim;
        printf("CSR_IP_SOCKET_TCP_CONNECT_CFM [%d]socket handle[%d]\n", prim->result,prim->socketHandle);
        break;
    }
    case CSR_IP_SOCKET_TCP_ACCEPT_IND:
    {
		CsrIpSocketTcpAcceptInd *prim= (CsrIpSocketTcpAcceptInd *)thePrim;
        printf("CSR_IP_SOCKET_TCP_ACCEPT_IND socket handle[%d][%d]\n", prim->socketHandle, prim->listenSocketHandle);
	    socketHandle=prim->socketHandle;
        break;
    }
    case CSR_IP_SOCKET_TCP_DATA_CFM:
    {
        CsrIpSocketTcpDataCfm *prim= (CsrIpSocketTcpDataCfm *)thePrim;
        break;
    }
    case CSR_IP_SOCKET_TCP_DATA_IND:
    {

        CsrIpSocketTcpDataInd *prim= (CsrIpSocketTcpDataInd *)thePrim;
		printf("CSR_IP_SOCKET_TCP_DATA_IND socket handle[%d]\n", prim->socketHandle);

		peer_get_message(prim->data,prim->dataLength);

        CsrIpSocketTcpDataResSend(prim->socketHandle);

        break;
    }
    case CSR_IP_SOCKET_TCP_CLOSE_IND:
    {
        printf("CSR_IP_SOCKET_TCP_CLOSE_IND \n");
        break;
    }
    case CSR_IP_SOCKET_RAW_NEW_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_RAW_BIND_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_RAW_DATA_IND:
    {

        break;
    }
    case CSR_IP_SOCKET_RAW_DATA_CFM:
    {

        break;
    }
    case CSR_IP_SOCKET_OPTIONS_CFM:
    {

        break;
    }


  }
}

