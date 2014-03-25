/****************************************************************************

                 (c) Cambridge Silicon Radio Ltd, 2006

                 All rights reserved

FILE:            client.c

DESCRIPTION:     Linux user<->user socket (TCP/IP) peer communication

REVISION:        $Revision: #1 $

****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include "csr_types.h"
#include "csr_pmem.h"

//#include "csr_sched_init.h"

//#include "sched/sched.h"
//#include "sched/CsrPmemAlloc.h"
#include "peer_com_linux_u2u.h"
//#include "bchs_msg_converter.h"
//#include "sched_private.h"

#include "usr2usr_socket.h"
#include "csr_bt_ss_prim.h"
#include "csr_samsung_log.h"
#include "AppHandler.h"

//#include "sco_audio.h"

#ifndef max
#define max(a,b) ((a>b)?a:b)
#endif

#define MAX_BUFFER              1000
#define MAX_TASKS               100
#define PPRE                    "[CSR_DBG] peer_com: "

static CsrBool 				     isAppCSRErrorState=FALSE;

typedef struct sched_tx_fifo_elm_t
{
    CsrUint16  					id;
    CsrUint16                     cmd;
	CsrUint16                     data_len;
    void                         *p_data;
    struct sched_tx_fifo_elm_t   *next;
} sched_tx_fifo_elm_t;

typedef struct
{
    struct sched_tx_fifo_elm_t *first;
    struct sched_tx_fifo_elm_t *last;
    pthread_mutex_t	access;
    pthread_cond_t      cond;
} sched_tx_fifo_t;

typedef struct conn_info_t
{
    int                         fd;
    CsrUint16                    qid;
    struct conn_info_t          *next;
} conn_info_t;

typedef struct int_ext_map_t
{
    CsrUint16                    int_qid;
    CsrUint16                    ext_qid;
    struct int_ext_map_t        *next;
} int_ext_map_t;

static pthread_t                tid_rx_thread = 0;
static pthread_t                tid_tx_thread = 0;
//static pthread_mutex_t          sco_map_access_mutex;
static sem_t                    get_ext_queue_sem[MAX_TASKS];

static sched_tx_fifo_t          *sched_tx_fifo = NULL;
static int                      cli_fd = -1;
//static bchs_msg_converter_t     *converter = NULL;
//static ScoHandleStructure       sco_map[MAX_NUM_OF_SCO];
static int_ext_map_t            *int_ext_map = NULL;
static CsrBool                   peer_com_run = FALSE;
//#define EXTERNAL_DEBUG
#ifdef EXTERNAL_DEBUG_PCM
static FILE            			*localMyFile;
#endif
#if 0
static CsrUint16 map_ext2int_queue(CsrUint16 ext_qid)
{
    CsrUint16 int_qid;
    int_ext_map_t *ptr;

    int_qid = 0xffff;

    ptr = int_ext_map;

    while(ptr)
    {
        if (ptr->ext_qid == ext_qid)
        {
            int_qid = ptr->int_qid;
            break;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    return(int_qid);
}
#endif
void set_external_tid(CsrUint16 tid, CsrUint16 ext_tid)
{
    int_ext_map_t *ptr;

    ptr = int_ext_map;

    while(ptr)
    {
        if (ptr->int_qid == tid)
        {
            ptr->ext_qid = ext_tid;
            break;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    if (ptr == NULL)
    {
        ptr = CsrPmemAlloc(sizeof(int_ext_map_t));
        ptr->int_qid = tid;
        ptr->ext_qid = ext_tid;

        ptr->next = int_ext_map;
        int_ext_map = ptr;
    }
    else
    {
        /* Do nothing */
    }
}
#if 0
static CsrUint16 get_external_tid(qid q)
{
    CsrUint16 ext_qid;
    int_ext_map_t *ptr;

    ext_qid = 0xffff;

    ptr = int_ext_map;

    while(ptr)
    {
        if (ptr->int_qid == q)
        {
            ext_qid = ptr->ext_qid;
            break;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    return(ext_qid);
}
#endif

static int send_CONNECT_req(CsrUint16 id)
{
    CsrUint16 version;
    CsrUint16 type;
    struct iovec out[EXT_CONNECT_req_NO_IOV];
    int n;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "running send_CONNECT_req: id=%d\n", id);
#endif

    version = EXT_VERSION;
    type    = EXT_CONNECT_req;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(CsrUint16);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(CsrUint16);
    out[2].iov_base = &id;
    out[2].iov_len  = sizeof(CsrUint16);

    n = writev(cli_fd, out,EXT_CONNECT_req_NO_IOV);

#ifdef EXTERNAL_DEBUG
    usr2usr_socket_dump_iov(out,EXT_CONNECT_req_NO_IOV);
#endif

    return(n);
}
#if 0
static int send_REGISTER_SCO_HANDLE_req(CsrUint16 sco_handle)
{
    CsrUint16 version;
    CsrUint16 type;
    struct iovec out[EXT_REGISTER_SCO_HANDLE_req_NO_IOV];
    int n;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "running send_REGISTER_SCO_HANDLE_req, sco_handle=0x%0x\n", sco_handle);
#endif

    version    = EXT_VERSION;
    type       = EXT_REGISTER_SCO_HANDLE_req;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(CsrUint16);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(CsrUint16);
    out[2].iov_base = &sco_handle;
    out[2].iov_len  = sizeof(CsrUint16);

    n = writev(cli_fd, out,EXT_REGISTER_SCO_HANDLE_req_NO_IOV);

#ifdef EXTERNAL_DEBUG
    usr2usr_socket_dump_iov(out,EXT_REGISTER_SCO_HANDLE_req_NO_IOV);
#endif

    return(n);
}

static int send_DEREGISTER_SCO_HANDLE_req(CsrUint16 sco_handle)
{
    CsrUint16 version;
    CsrUint16 type;
    struct iovec out[EXT_DEREGISTER_SCO_HANDLE_req_NO_IOV];
    int n;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "running send_DEREGISTER_SCO_HANDLE_req, sco_handle=0x%0x\n", sco_handle);
#endif

    version    = EXT_VERSION;
    type       = EXT_DEREGISTER_SCO_HANDLE_req;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(CsrUint16);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(CsrUint16);
    out[2].iov_base = &sco_handle;
    out[2].iov_len  = sizeof(CsrUint16);

    n = writev(cli_fd, out,EXT_DEREGISTER_SCO_HANDLE_req_NO_IOV);

#ifdef EXTERNAL_DEBUG
    usr2usr_socket_dump_iov(out,EXT_DEREGISTER_SCO_HANDLE_req_NO_IOV);
#endif

    return(n);
}
#endif


static csrBtClientCB  Error_BT_CB;

static int handle_external(int fd)
{
    unsigned char header[EXT_HEADER_SIZE];
    CsrUint16 version;
    CsrUint16 payload_length;
    CsrUint16 type;
    int offset;
    int n;
#ifdef EXTERNAL_DEBUG	
    int i;
#endif
//    int missing;
//    void *p;
    CsrUint16 id;
    CsrUint16 cmd;
    unsigned char *data;
    int retval;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "running handle_external, fd=%d\n", fd);
#endif

    retval = 1;

    offset = 0;

    n = read(fd, &header[offset], EXT_HEADER_SIZE);
    if (n>0)
    {
        offset = n;

        while(offset < EXT_HEADER_SIZE)
        {
            //missing = EXT_HEADER_SIZE - offset;

            n = read(fd, &header[offset], EXT_HEADER_SIZE);

            if (n > 0)
            {
                offset += n;
            }
            else
            {
                retval = 0;
                break;
            }
        }

#ifdef EXTERNAL_DEBUG		
	printf("handle_external : n<%d>\n", n);
	printf("EXT_HEADER : ");
	for(i = 0; i < n; i++)
	{
		printf("[0x%02x]", header[i]);
	}
	printf("\n");
#endif	
    }
    else
    {
        retval = 0;
    }

    if(retval > 0)
    {
        version = *(CsrUint16*)&header[EXT_VERSION_IDX];

        if (version != EXT_VERSION)
        {
            printf(PPRE "external protocol version wrong: exp=%d, got=%d\n",
                   EXT_VERSION,version);

            retval = 1;
        }
        else
        {
            type = *(CsrUint16*)&header[EXT_TYPE_IDX];

            switch(type)
            {
                case EXT_DATA_ind:
                    {
                        unsigned char rest[6];
#ifdef EXTERNAL_DEBUG
                       printf(PPRE "rx: DATA_ind\n");
#endif

                        n = read_external_data(fd,6,rest);

                        payload_length = *(CsrUint16*)&rest[0];
                        id = *(CsrUint16*)&rest[2];
                        cmd = *(CsrUint16*)&rest[4];
#ifdef EXTERNAL_DEBUG							
				printf("handle_external : id<%d>, cmd<%d>\n", id, cmd);
#endif

                        if (payload_length>0)
                    	{
                        	data = (unsigned char*)CsrPmemAlloc(payload_length);
							n = read_external_data(fd,payload_length,data);
#ifdef EXTERNAL_DEBUG							
							printf("handle_external : payload_length<%d>, n<%d>\n", payload_length, n);
#endif
                    	}
						else
							data=NULL;

						CsrBtAppEventHandler(id,cmd,payload_length,data);
              
                         if(data)
                         {
							 usr2usr_socket_dump_data(payload_length,data);
                        	 CsrPmemFree(data);
                         }
                    }
                    break;

                case EXT_CONNECT_cfm:
                    {
                        unsigned char rest[4];
                        CsrUint16 tid;
                        CsrUint16 ext_tid;

#ifdef EXTERNAL_DEBUG
                        printf(PPRE "rx: CONNECT_cfm\n");
#endif

                        n = read_external_data(fd,4,rest);

                        tid = *(CsrUint16*)rest;
                        ext_tid = *(CsrUint16*)&rest[2];

                        set_external_tid(tid,ext_tid);

                        sem_post(&get_ext_queue_sem[tid]);
                    }
                    break;
#if 0
                case EXT_SCO_DATA_ind:
                    {
                        unsigned char rest[4];
                        CsrUint16 sco_hdl;
                        ScoHandlerFuncType func;

#ifdef EXTERNAL_DEBUG
                        printf(PPRE "rx: SCO_DATA_ind\n");
#endif

                        n = read_external_data(fd,4,rest);

                        sco_hdl = *(CsrUint16*)rest;
                        payload_length = *(CsrUint16*)&rest[2];

                        data = (unsigned char*)CsrPmemAlloc(payload_length);

                        n = read_external_data(fd,payload_length,data);

                        if (n>0)
                        {
#ifdef EXTERNAL_DEBUG
                            usr2usr_socket_dump_data(payload_length,data);
#endif
                            func = NULL;

                            pthread_mutex_lock(&sco_map_access_mutex);
                            for(i = 0; i < MAX_NUM_OF_SCO; i++)
                            {
                                if(sco_map[i].scoHandle == sco_hdl)
                                {
                                    func = sco_map[i].scoHandlerFunc;
                                    break;
                                }
                            }

                            pthread_mutex_unlock(&sco_map_access_mutex);

                            if(func)
                            {
                                func((char*)data);
                            }
                            else
                            {
                                CsrPmemFree(data);
                            }
                        }
                        else
                        {
                            retval = 0;
                        }
                    }
                    break;

                case EXT_DEREGISTER_SCO_HANDLE_cfm:
                    {
                        unsigned char rest[2];
                        CsrUint16 sco_hdl;

#ifdef EXTERNAL_DEBUG
                        printf(PPRE "rx: DEREGISTER_SCO_HANDLE_cfm\n");
#endif

                        n = read_external_data(fd,2,rest);

                        sco_hdl = *(CsrUint16*)rest;

                        pthread_mutex_lock(&sco_map_access_mutex);
                        for(i = 0; i < MAX_NUM_OF_SCO; i++)
                        {
                            if(sco_map[i].scoHandle == sco_hdl)
                            {
                                sco_map[i].scoHandlerFunc = NULL;
                                sco_map[i].scoHandle = NO_SCO_HANDLE;
                                break;
                            }
                        }

                        pthread_mutex_unlock(&sco_map_access_mutex);
                    }
                    break;
#endif

                default:
                    printf(PPRE "unknown message type 0x%0x\n",type);
                    retval = 0;
                    break;
            }
        }
    }
    else
    {
        retval = 0;
    }

    return(retval);
}

static void *external_cli_tx_thread(void * thread_param)
{
    sched_tx_fifo_elm_t *ptr;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "external_cli_tx_thread started\n");
#endif

    while(peer_com_run)
    {
        pthread_mutex_lock(&sched_tx_fifo->access);

        while((sched_tx_fifo->first == NULL) && (peer_com_run))
        {
#ifdef EXTERNAL_DEBUG
            printf(PPRE "external_cli_tx_thread: no messages, waiting\n");
#endif
            
            pthread_cond_wait(&sched_tx_fifo->cond,
                              &sched_tx_fifo->access);

#ifdef EXTERNAL_DEBUG
            printf(PPRE "external_cli_tx_thread: messages ready\n");
#endif
        }

        if(!peer_com_run)
        {
            /* Flag to exit was set, jo release lock and quit thread */
            pthread_mutex_unlock(&sched_tx_fifo->access);
            return NULL;
        }

        ptr = sched_tx_fifo->first;
        sched_tx_fifo->first = ptr->next;

        if (sched_tx_fifo->first == NULL)
        {
            sched_tx_fifo->last = NULL;
        }

        pthread_mutex_unlock(&sched_tx_fifo->access);

      //  msg_free(ptr->p_data);

        send_DATA_ind(cli_fd,ptr->id, ptr->cmd,(unsigned char  *)ptr->p_data,ptr->data_len);
	  
        if(ptr->p_data != NULL)
        	CsrPmemFree(ptr->p_data);
		
        CsrPmemFree(ptr);
    }
    
    return NULL;
}


/**
 * Client thread that will receive message from the other side
 *
 * This thread will receive messages from the other side and forward them to the
 * scheduler after the messages has been decoded
 *
 * @param thread_param [in] Thread parameter given to the thread when it starts
 */
static void *external_cli_rx_thread(void * thread_param)
{
    int n;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "external_cli_rx_thread started\n");
#endif
    
    while(peer_com_run)
    {
        n = handle_external(cli_fd);

        if(n <= 0)
        {
            /* Shutdown socket and close desriptor */
            shutdown(cli_fd, SHUT_RDWR);
            close(cli_fd);
            cli_fd = -1;
            peer_com_run = FALSE;
        }
    }

	printf(PPRE "external_cli_rx_thread END\n");
//	CsrBtServerCloseEventHandler();
	if(Error_BT_CB)
		Error_BT_CB();
    return NULL;
}

/*
 *  Transfer a message across the socket
 */
 #if 0
CsrBool peer_put_message(/*qid q,*/CsrUint16 mi,void *mv)
{
    CsrBool retval;
    sched_tx_fifo_elm_t *ptr;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "peer_put_message, q=%d, mi=%d, mv=%p\n",q,mi,mv);
#endif

    ptr = (sched_tx_fifo_elm_t*)CsrPmemAlloc(sizeof(sched_tx_fifo_elm_t));
    retval = TRUE;

    ptr->qid  = q;
    ptr->mi   = mi;
    ptr->mv   = mv;
    ptr->next = NULL;

    pthread_mutex_lock(&sched_tx_fifo->access);

    if (sched_tx_fifo->first == NULL)
    {
        sched_tx_fifo->first = ptr;
    }

    if (sched_tx_fifo->last != NULL)
    {
        sched_tx_fifo->last->next = ptr;
        sched_tx_fifo->last = ptr;
    }
    else
    {
        sched_tx_fifo->last = ptr;
    }

    pthread_mutex_unlock(&sched_tx_fifo->access);
    pthread_cond_signal(&sched_tx_fifo->cond);

#ifdef EXTERNAL_DEBUG
    printf(PPRE "peer_put_message, retval=%d\n",retval);
#endif

    return(retval);
}
#endif



CsrBool peer_client_com_init(int port,csrBtClientCB ClsoeCB)
{
    struct sockaddr_in serveraddr;
    int i;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "init_external_communication, port=%d\n", port);
#endif

    if(ClsoeCB)
		Error_BT_CB=ClsoeCB;

    /* setup socket */
    cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(cli_fd < 0)
    {
        extern int errno;
        printf(PPRE "could not connect to socket, code %i - %s\n", errno, strerror(errno));

     return FALSE;		
    }
    if(peer_com_run == TRUE)  //already start.....
    {
       isAppCSRErrorState=FALSE;
       printf(PPRE "already start...\n");
       return TRUE;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(port);

    if(connect(cli_fd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        extern int errno;
        printf(PPRE "connection error, code %i - %s\n", errno, strerror(errno));
		//SAMSUNG_DTV	 Dont use system	
	    return FALSE;

    }
    else
    {
        printf(PPRE "connection to server established...\n");
    }

    /* Setup fifo between scheduler and writer task */
    sched_tx_fifo = (sched_tx_fifo_t*)CsrPmemAlloc(sizeof(sched_tx_fifo_t));
    sched_tx_fifo->first = NULL;
    sched_tx_fifo->last = NULL;

    pthread_mutex_init(&sched_tx_fifo->access, NULL);
    pthread_cond_init(&sched_tx_fifo->cond, NULL);
#if 0
    pthread_mutex_init(&sco_map_access_mutex,NULL);

    for(i = 0; i < MAX_NUM_OF_SCO; i++)
    {
        sco_map[i].scoHandlerFunc = NULL;
        sco_map[i].scoHandle      = NO_SCO_HANDLE;
    }
#endif
    /* Create reader and writer threads */
    peer_com_run = TRUE;
	isAppCSRErrorState=FALSE;

    i = pthread_create(&tid_rx_thread, NULL, external_cli_rx_thread, NULL);
    if(i != 0)
    {
        printf(PPRE "pthread_create failed to create rx-thread\n");		
		  return FALSE;

    }
    i = pthread_create(&tid_tx_thread, NULL, external_cli_tx_thread, NULL);
    if(i != 0)
    {
        printf(PPRE "pthread_create failed to create tx-thread\n");
		return FALSE;

    }
	return TRUE;
}

CsrBool peer_client_com_deinit(void)
{
    int ret;
    void *thread_res;
	CsrBool retvalue=TRUE;

    /* Dummy checking */
    if(peer_com_run == FALSE)
    {
        printf(PPRE "threads never started\n");
		retvalue=FALSE;
    
    }

    /* Raise stop sign */
    peer_com_run = FALSE;

    /* Join tx thread */
    if(tid_tx_thread != 0)
    {
        pthread_cond_signal(&sched_tx_fifo->cond);
        ret = pthread_join(tid_tx_thread, &thread_res);
        if(ret != 0)
        {
            printf(PPRE "tx-thread join error\n");
			retvalue=FALSE;
        }
        tid_tx_thread = 0;
    }
    else
    {
        printf(PPRE "tx-thread not running\n");
		retvalue=FALSE;
    }

    /* Cancel rx thread */
    if(tid_rx_thread != 0)
    {
        ret = pthread_cancel(tid_rx_thread);
        if(ret != 0)
        {
            printf(PPRE "rx-thread cancel error\n");
			retvalue=FALSE;
        }
        tid_rx_thread = 0;
    }
    else
    {
        printf(PPRE "rx-thread not running\n");
		retvalue=FALSE;
    }

    /* Make sure socket is closed */
    if(cli_fd > -1)
    {
        shutdown(cli_fd, SHUT_RDWR);
        close(cli_fd);
        cli_fd = -1;

    }

    /* Cleanup the rest */
 //   pthread_mutex_destroy(&sco_map_access_mutex);
    pthread_cond_destroy(&sched_tx_fifo->cond);
    pthread_mutex_destroy(&sched_tx_fifo->access);

    CsrPmemFree(sched_tx_fifo);
	return retvalue;
}

void csr_bt_client_app_set_error_state(CsrBool isError)
{
   isAppCSRErrorState=isError;
}

CsrInt16 csr_bt_client_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 evt, void *data, CsrUint16 data_len )
{
 sched_tx_fifo_elm_t *ptr;

#ifdef EXTERNAL_DEBUG
 printf("external_send: id = %d, evt=%d, data=%p\n",id,evt,data);
#endif 

 if(peer_com_run==FALSE)
 	 return CSR_ERR_CLI_NOT_CONNECT;
 if(isAppCSRErrorState)
 	return CSR_ERR_CLI_INTERNAL; 
 

 ptr = (sched_tx_fifo_elm_t*)CsrPmemAlloc(sizeof(sched_tx_fifo_elm_t));

 ptr->id  = id;
 ptr->cmd   = evt;
 ptr->data_len   = data_len;
 ptr->p_data=data;
 ptr->next = NULL;

 pthread_mutex_lock(&sched_tx_fifo->access);
 


 if (sched_tx_fifo->first == NULL)
 { 
	 sched_tx_fifo->first = ptr;	   
 }

 if (sched_tx_fifo->last != NULL)
 {
	 sched_tx_fifo->last->next = ptr;
	 sched_tx_fifo->last = ptr;
 }
 else
 {
	 sched_tx_fifo->last = ptr;
 }

 pthread_mutex_unlock(&sched_tx_fifo->access);
 pthread_cond_signal(&sched_tx_fifo->cond);

#ifdef EXTERNAL_DEBUG
 printf("external_send: sucsses\n");
#endif

 return(CSR_ERR_CLI_SUCESS);
}



void peer_get_external_conect()
{
//    CsrUint16 ext_tid;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "peer_get_external_conect");
#endif

    sem_init(&get_ext_queue_sem[0],0,0);

    send_CONNECT_req(0);

    sem_wait(&get_ext_queue_sem[0]);

    return;
}


#if 0

qid peer_get_external_qid(qid q)
{
    CsrUint16 ext_tid;

#ifdef EXTERNAL_DEBUG
    printf(PPRE "peer_get_external_qid: qid = %d\n",q);
#endif

    sem_init(&get_ext_queue_sem[q],0,0);

    send_CONNECT_req(q);

    sem_wait(&get_ext_queue_sem[q]);
    ext_tid = get_external_tid(q);

    return(ext_tid);
}


void peer_register_converter(bchs_msg_converter_t *_converter)
{
    converter = _converter;
}

CsrBool RegisterScoHandle(CsrUint16 theScoHandle,
                         ScoHandlerFuncType theFunctionPtr)
{
    int i;
    CsrBool retval;

    retval = FALSE;

    printf(PPRE "running RegisterScoHandle\n");

    pthread_mutex_lock(&sco_map_access_mutex);

    for(i = 0; i < MAX_NUM_OF_SCO; i++)
    {
        if(sco_map[i].scoHandle == NO_SCO_HANDLE)
        {
            sco_map[i].scoHandlerFunc = theFunctionPtr;
            sco_map[i].scoHandle      = theScoHandle;
            retval = TRUE;
            break;
        }
    }

    pthread_mutex_unlock(&sco_map_access_mutex);

    if (retval == TRUE)
    {
        send_REGISTER_SCO_HANDLE_req(theScoHandle);
    }
    else
    {
        /* Do nothing */
    }

    return (retval);
}

void SendScoData(char *data)
{
    CsrUint16 sco_hdl;
    uint8_t data_len;

    sco_hdl = (CsrUint16)(((data[1] & 0xf0) << 8) | (data[0] & 0xff));
    data_len = (uint8_t)data[2];

    send_SCO_DATA_ind(cli_fd,sco_hdl,data_len,data);

    CsrPmemFree(data);
}

void deRegisterScoHandle(CsrUint16 theScoHandle)
{
    printf(PPRE "running DeRegisterScoHandle\n");

    send_DEREGISTER_SCO_HANDLE_req(theScoHandle);
}
#endif
