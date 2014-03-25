/****************************************************************************

                         (c) Cambridge Silicon Radio Ltd, 2006

                         All rights reserved

FILE:                    server.c

DESCRIPTION:             Linux user<->user socket (TCP/IP) peer communication

REVISION:                $Revision: #1 $

****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "csr_pmem.h"
#include "csr_log_text_2.h"


//#include "csr_sched_init.h"
#include "csr_types.h"

//#include "sched/CsrPmemAlloc.h"
//#include "sched/env_prim.h"

#include "peer_com_linux_u2u.h"
//#include "bchs_msg_converter.h"
//#include "sched_private.h"

#include "usr2usr_socket.h"
//#include "sco_audio.h"
#include "csr_bt_ss_prim.h"

#include "csr_samsung_log.h"
//#include "csr_bt_declare_tasks.h"

//#include "csr_bt_av_lib.h"
//#include "csr_bt_av_prim.h"


/* Enable this to be very very verbose */
//#define PEER_DEBUG 
//#define EXTERNAL_DEBUG

#ifndef max
#define max(a,b) ((a>b)?a:b)
#endif

#define MAX_BUFFER               1000
#define MAX_TASKS                100
#define PPRE                     "[CSR_DBG] peer_com: "
#define SELECT_DELAY             1
#define SELECT_UDELAY            0



/* Data buffer element */
typedef struct sched_tx_fifo_elm_t
{
    CsrUint16  					id;
    CsrUint16                     cmd;
	CsrUint16                     data_len;
    void                         *p_data;
    struct sched_tx_fifo_elm_t   *next;
} sched_tx_fifo_elm_t;

/* Data queue container */
typedef struct
{
    struct sched_tx_fifo_elm_t   *first;
    struct sched_tx_fifo_elm_t   *last;
    pthread_mutex_t              access;
    pthread_cond_t               cond;
} sched_tx_fifo_t;

/* Connection info */
typedef struct conn_info_t
{
    int                          fd;
    //CsrUint16                     qid;
    struct conn_info_t           *next;
} conn_info_t;

#if 0
/* SCO handle-fd map */
typedef struct sco_map_t
{
    int                          fd;
    CsrUint16                     sco_handle;
    uint8_t                      sco_state;
    struct sco_map_t             *next;
} sco_map_t;

/* SCO data buffer element */
typedef struct sco_data_t
{
    char                         *data;
    struct sco_data_t            *next;
} sco_data_t;

/* SCO data queue container */
typedef struct
{
    sco_data_t                   *first;
    sco_data_t                   *last;
    int                          count;
    pthread_mutex_t              sco_data_access_mutex;
} sco_data_list_t;
#endif
#if 0
/* Container for used external queue numbers */
typedef struct used_queues_t
{
    CsrUint16                     qid;   
    struct used_queues_t         *next;
} used_queues_t;
#endif

static pthread_mutex_t           conn_access_mutex;
//atic pthread_mutex_t           sco_map_access_mutex;
static pthread_t                 tid_rx_thread = 0;
static pthread_t                 tid_tx_thread = 0;

//static used_queues_t             *used_queues = NULL;
 
static sched_tx_fifo_t           *sched_tx_fifo = NULL;
static conn_info_t               *conn = NULL;

//atic bchs_msg_converter_t      *converter = NULL;
static int                       srv_listen_fd = -1;
static int                       no_connections = 0;
static CsrBool                    peer_com_run = FALSE;


static int connfd=-1;


#if 0
/* Add new external queue handle (non re-entrant!) */
static CsrUint16 get_queue_handle(void)
{
    CsrUint16 no;
    used_queues_t *pre;
    used_queues_t *cur;
    used_queues_t *new;

    /* Scan for next free queue number */
    no = EXTERNAL_QUEUE_START;
    pre = NULL;
    cur = used_queues;
    while(cur && (no <= EXTERNAL_QUEUE_END))
    {
        if(cur->qid > no)
        {
            break;
        }
        no = cur->qid + 1;
        pre = cur;
        cur = cur->next;
    }

    /* Check for overflow */
    if(no > EXTERNAL_QUEUE_END)
    {
        printf(CSR_SS_AP_LOG_STR "no more available external queues"));
        return INVALID_PHANDLE;
    }

    /* Insert element */
    new = CsrPmemAlloc(sizeof(used_queues_t));
    new->qid = no;
    new->next = cur;    

    /* Previous element exists */
    if(pre)
    {
        pre->next = new;
    }
    else
    {
        /* No previous, so set new head */
        used_queues = new;
    }

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "new queue handle found, handle 0x%04x\n", no);
#endif

    return no;
}
#endif

#if 0
/* Remove queue from used queue id list */
static void remove_queue_handle(CsrUint16 qid)
{
    used_queues_t *pre;
    used_queues_t *cur;

    /* Scan for queue */
    pre = NULL;
    cur = used_queues;
    while(cur)
    {
        if(cur->qid == qid)
        {
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    
    /* Element matches, cut it out */
    if(cur && (cur->qid == qid))
    {
#ifdef PEER_DEBUG
        printf(CSR_SS_AP_LOG_STR "remove queue handle 0x%04x\n", qid);
#endif

        if(pre)
        {
            /* Previous element exists, cut us out */
            pre->next = cur->next;
        }
        else
        {
            /* No previous - we are the head - set new */
            used_queues = cur->next;
        }
        CsrPmemFree(cur);
    }
    else
    {
#ifdef PEER_DEBUG
        printf(CSR_SS_AP_LOG_STR "could not find queue handle to remove, handle 0x%04x\n",
               qid);
#endif
    }
}
#endif
  
/* Send connect confirm */
static int send_CONNECT_cfm(int fd, CsrUint16 tid, CsrUint16 hdl)
{
    CsrUint16 version;
    CsrUint16 type;
    struct iovec out[EXT_CONNECT_cfm_NO_IOV];
    int n;

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "running: send_CONNECT_cfm, fd=%d, tid=%d, hdl=0x%02x\n",
           fd, tid, hdl));
#endif

    version = EXT_VERSION;
    type    = EXT_CONNECT_cfm;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(CsrUint16);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(CsrUint16);
    out[2].iov_base = &tid;
    out[2].iov_len  = sizeof(CsrUint16);
    out[3].iov_base = &hdl;
    out[3].iov_len  = sizeof(CsrUint16);

    n = writev(fd, out, EXT_CONNECT_cfm_NO_IOV);

#ifdef PEER_DEBUG
    usr2usr_socket_dump_iov(out, EXT_CONNECT_cfm_NO_IOV);
#endif

    return(n);
}

#if 0
/* Send SCO deregister confirm */
static int send_DEREGISTER_SCO_HANDLE_cfm(int fd, CsrUint16 hdl)
{
    CsrUint16 version;
    CsrUint16 type;
    struct iovec out[EXT_DEREGISTER_SCO_HANDLE_cfm_NO_IOV];
    int n;

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "running: send_DEREGISTER_SCO_HANDLE_cfm, fd=%d, hdl=0x%02x\n",
           fd,hdl);
#endif

    version = EXT_VERSION;
    type    = EXT_DEREGISTER_SCO_HANDLE_cfm;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(CsrUint16);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(CsrUint16);
    out[2].iov_base = &hdl;
    out[2].iov_len  = sizeof(CsrUint16);

    n = writev(fd, out,EXT_DEREGISTER_SCO_HANDLE_cfm_NO_IOV);

#ifdef PEER_DEBUG
    usr2usr_socket_dump_iov(out,EXT_DEREGISTER_SCO_HANDLE_cfm_NO_IOV);
#endif

    return(n);
}
#endif
#if 0
/* Extend connection map to be able to receive another connection */
static void extend_connection(int fd, CsrUint16 q)
{
    conn_info_t *ptr;

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "running: extend_connection, fd=%d, q=%d\n",
           fd, q));
#endif

    pthread_mutex_lock(&conn_access_mutex);
    ptr=conn;

    while(ptr)
    {
        if (ptr->fd == fd)
        {
#if 0
            if (ptr->qid == 0xffff)
            {
                ptr->qid = q;
            }
            else
#endif
            {
                conn_info_t *ptr;

                ptr = (conn_info_t*)CsrPmemAlloc(sizeof(conn_info_t));
                ptr->fd  = fd;
                /* ptr->qid = q; */
                ptr->next = conn;
                conn = ptr;
            }
            break;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    pthread_mutex_unlock(&conn_access_mutex);
}

/* Insert a new connection map into table */
static void add_new_connection(int fd)
{
    conn_info_t *ptr;

    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"new connection added, fd=%d\n", fd));

    ptr = (conn_info_t*)CsrPmemAlloc(sizeof(conn_info_t));
    ptr->fd  = fd;
    /* ptr->qid = 0xffff; */

    pthread_mutex_lock(&conn_access_mutex);
    ptr->next=conn;
    conn = ptr;
    pthread_mutex_unlock(&conn_access_mutex);
}
#endif



/* Remove connection map from table.
 * This function is NOT re-entrant and must be called with locks held */
static void remove_connection(int fd)
{
    conn_info_t *ptr;
    conn_info_t *prev;

    prev = NULL;
    ptr = conn;

    /* Scan connection map for the fd we just lost */
    while(ptr)
    {
        if(ptr->fd == fd)
        {
#if 0
            printf(CSR_SS_AP_LOG_STR "connection removed, qid=%d, fd=%d\n",
                   ptr->qid,
                   fd);
            broadcast_message(ENV_PRIM,
                              build_env_cleanup_ind,
                              &ptr->qid);
#endif
            /* Cut us out of link */
            if (prev == NULL)
            {
                conn = ptr->next;
            }
            else
            {
                prev->next = ptr->next;
            }
#if 0
            /* Free external qid */
            remove_queue_handle(ptr->qid);
#endif
            /* Free map */
            CsrPmemFree(ptr);
            ptr = conn;
            prev = NULL;
        }
        else
        {
            prev = ptr;
            ptr = ptr->next;
        }
    }
}





static int handle_external(int fd)
{
    unsigned char header[EXT_HEADER_SIZE];
    CsrUint16 version;
    CsrUint16 payload_length;
    CsrUint16 type;
    int offset;
    int n;
//    int missing;
//    void *p;
    CsrUint16 id;
//    CsrUint16 int_q;
    CsrUint16 cmd;
    unsigned char *data=NULL;
    int retval;

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "running: SERVER____handle_external, fd=%d\n",fd);
#endif

    retval = 1;

    offset = 0;

    n = read(fd,&header[offset],EXT_HEADER_SIZE);
    if (n>0)
    {
        offset = n;

        while(offset < EXT_HEADER_SIZE)
        {
            //missing = EXT_HEADER_SIZE - offset;

            n = read(fd,&header[offset],EXT_HEADER_SIZE);

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
    }
    else
    {
        retval = 0;
    }

    if (retval)
    {
        version = *(CsrUint16*)&header[EXT_VERSION_IDX];

        if (version != EXT_VERSION)
        {
            printf("external protocol version wrong, exp=%d, got=%d\n",
                   EXT_VERSION,version);

            retval = 0;
        }
        else
        {
            type = *(CsrUint16*)&header[EXT_TYPE_IDX];

            switch(type)
            {
                case EXT_DATA_ind:
					{
						unsigned char rest[6];
						CsrBtUiMessage_t *key;

#ifdef PEER_DEBUG
						printf(CSR_SS_AP_LOG_STR "rx: DATA_ind");
#endif

						n = read_external_data(fd,6,rest);


						//   payload_length = *(CsrUint16*)rest;
						payload_length=*(CsrUint16 *)&rest[0];
						id = *(CsrUint16 *)&rest[2];
						cmd = *(CsrUint16 *)&rest[4];

#ifdef PEER_DEBUG						
						printf(CSR_SS_AP_LOG_STR "id:[%d] cmd:[%d] payload:[%d]\n", id, cmd, payload_length);
#endif
						if(payload_length)
						{
							data = (unsigned char*)CsrPmemAlloc(payload_length);
							n = read_external_data(fd,payload_length,data);

						}
						else
						{
							data=NULL;
						}

						
						key = CsrPmemAlloc(sizeof(CsrBtUiMessage_t));
						key->type = BT_UI_MESSAGE;
						key->id =id;
						key->cmd=cmd;
						key->data=data;
						CsrSchedMessagePut(TESTQUEUE, BT_UI_MESSAGE, key);


					}
                    break;

                case EXT_CONNECT_req:
                    {
                        unsigned char rest[2];
                        CsrUint16 tid;
                        //CsrUint16 qid;
#ifdef PEER_DEBUG
                        printf(CSR_SS_AP_LOG_STR "rx: CONNECT_req");
#endif

                        n = read_external_data(fd, 2, rest);
                        tid = *(CsrUint16*)rest;

                        pthread_mutex_lock(&conn_access_mutex);
        //                qid = get_queue_handle();
                        pthread_mutex_unlock(&conn_access_mutex);

                        send_CONNECT_cfm(fd, tid, 0);
                        //extend_connection(fd, qid);
                    }
                    break;
#if 0
                case EXT_REGISTER_SCO_HANDLE_req:
                    {
                        unsigned char rest[2];
                        CsrUint16 sco_handle;
#ifdef PEER_DEBUG
                        printf(CSR_SS_AP_LOG_STR "rx: REGISTER_SCO_HANDLE_req");
#endif

                        n = read_external_data(fd,2,rest);
                        sco_handle = *(CsrUint16*)rest;

                        /* Use background interrupt to defer registering */
                        add_new_sco_handle(sco_handle, fd);
                        bg_int3();
                    }
                    break;

                case EXT_DEREGISTER_SCO_HANDLE_req:
                    {
                        unsigned char rest[2];
                        CsrUint16 sco_handle;
#ifdef PEER_DEBUG
                        printf(CSR_SS_AP_LOG_STR "rx: DEREGISTER_SCO_HANDLE_req");
#endif

                        n = read_external_data(fd,2,rest);

                        sco_handle = *(CsrUint16*)rest;

                        /* Use background interrupt to defer unregister */
                        mark_sco_handle_deletion(sco_handle);
                        bg_int3();
                    }
                    break;

                case EXT_SCO_DATA_ind:
                    {
                        unsigned char rest[4];
                        CsrUint16 sco_handle;
                        sco_data_t *sco_data;

#ifdef PEER_DEBUG
                        printf(CSR_SS_AP_LOG_STR "rx: SCO_DATA_ind");
#endif

                        n = read_external_data(fd,4,rest);

                        sco_handle = *(CsrUint16*)rest;
                        payload_length = *(CsrUint16*)&rest[2];

                        data = (unsigned char*)CsrPmemAlloc(payload_length);

                        n = read_external_data(fd,payload_length,data);

                        if (n>0)
                        {
#ifdef PEER_DEBUG
                            usr2usr_socket_dump_data(payload_length,data);
#endif

 
                        }
                        else
                        {
                            retval = 0;
                        }
                    }
                    break;
#endif
                default:
                    printf(CSR_SS_AP_LOG_STR "unknown type 0x%0x\n",type);
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

static void *external_srv_tx_thread(void * thread_param)
{
    sched_tx_fifo_elm_t *ptr;


#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "external_srv_tx_thread started"));
#endif

    while(peer_com_run)
    {
        pthread_mutex_lock(&sched_tx_fifo->access);

        while(sched_tx_fifo->first == NULL)  
        {
#ifdef PEER_DEBUG
            printf(CSR_SS_AP_LOG_STR "external_srv_tx_thread - waiting for messages");
#endif
            pthread_cond_wait(&sched_tx_fifo->cond,
                              &sched_tx_fifo->access);
#ifdef PEER_DEBUG
            printf(CSR_SS_AP_LOG_STR "external_srv_tx_thread - messages ready");
#endif
        }

        ptr = sched_tx_fifo->first;
        sched_tx_fifo->first = ptr->next;

        if (sched_tx_fifo->first == NULL)
        {
            sched_tx_fifo->last = NULL;
        }

        pthread_mutex_unlock(&sched_tx_fifo->access);


//        data = /*nverter->serialize_message(ptr->mi,ptr->mv,&len,&msg_free)*/ /*/data ?*/;
//////////////////////////////////////////////////////////
//////////////////////////////////////////
///////////////////////////////

      //  msg_free(ptr->mv);


#if 0	   
        pthread_mutex_lock(&conn_access_mutex);
        p=conn;

        while(p)
        {
            if (p->qid == q)
            {
                break;
            }
            else
            {
                p = p->next;
            }
        }

        pthread_mutex_unlock(&conn_access_mutex);

        if (p)
#endif				
        {
                    
            send_DATA_ind(connfd,ptr->id, ptr->cmd,(unsigned char  *)ptr->p_data ,ptr->data_len);
        }

		if (ptr->p_data != NULL)
		{
#ifdef PEER_DEBUG		
		    printf("external_srv_tx_thread CsrPmemFree"));
#endif
        	    CsrPmemFree(ptr->p_data);
		}
               CsrPmemFree(ptr);
    }

    return NULL;
}

/**
 * Server thread that will receive message from the other side
 *
 * This thread will receive messages from the other side and forward them to the
 * schedulre after the messages has been deserialized
 *
 * @param thread_param [in] Thread parameter given to the thread when it starts
 */
static void *external_srv_rx_thread(void * thread_param)
{
    struct sockaddr cliaddr;
    unsigned int clilen;

    fd_set rset;
    fd_set tmp_rset;
    int max_fd;
    int n;
    conn_info_t *ptr;
    int no_select;
    struct timeval tv;

#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "external_srv_rx_thread started");
#endif

    FD_ZERO(&rset);
    FD_SET(srv_listen_fd, &rset);
    max_fd = srv_listen_fd;


    while(peer_com_run)
    {
        tmp_rset = rset;
        tv.tv_sec = SELECT_DELAY;
        tv.tv_usec = SELECT_UDELAY;
        no_select = select(max_fd+1, &tmp_rset, NULL, NULL, &tv);
//		printf(CSR_SS_AP_LOG_STR "waiting for external_srv_rx_thread...%d\n",no_select);

        if(no_select > 0)
        {
            if (FD_ISSET(srv_listen_fd,&tmp_rset))
            {
                if(no_connections)
                {
                   printf(CSR_SS_AP_LOG_STR "connection...err ");
					return NULL;
                }
                clilen = sizeof(cliaddr);
                connfd = accept(srv_listen_fd,
                                (struct sockaddr*)&cliaddr,
                                &clilen);

              //  add_new_connection(connfd);

                FD_SET(connfd,&rset);
                max_fd = max(max_fd,connfd);
                no_connections++;
            }
            else
            {
             //  printf(CSR_SS_AP_LOG_STR "pre handle_external...");
                pthread_mutex_lock(&conn_access_mutex);
                ptr = conn;
                while( no_select>0)
                {
                    if (FD_ISSET(connfd,&tmp_rset))
                    {
                        no_select--;
                        n = handle_external(connfd);

                        if (n>0)
                        {
                            /* Do nothing */
                        }
                        else
                        {
                            FD_CLR(connfd,&rset);
                            shutdown(connfd, SHUT_RDWR);
                            close(connfd);
                            remove_connection(connfd);
                            no_connections--;
                        }
                    }
                    else
                    {
                        ptr = ptr->next;
                    }
                }
                pthread_mutex_unlock(&conn_access_mutex);
            }
        }
        else if(no_select == EINTR)
        {
            /* We received an interrupt signal, break out of loop */
            break;
        }
    }

    return NULL;
}


CsrBool csr_bt_server_app_put_message_to_tx_thread(CsrUint16 id,CsrUint16 evt, void *data, CsrUint16 data_len )
{
 CsrBool retval;
 sched_tx_fifo_elm_t *ptr;

#ifdef EXTERNAL_DEBUG
 printf("csr_bt_server_app_put_message_to_tx_thread: id = 0x%04x, evt=0x%04x, data_len<%d>, data=%p\n",id,evt,data_len, data);
#endif

  if( no_connections==0)
  	return FALSE;
  
 ptr = (sched_tx_fifo_elm_t*)CsrPmemAlloc(sizeof(sched_tx_fifo_elm_t));
 retval = TRUE;

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
 printf("external_send: retval = %d\n",retval);
#endif

 return(retval);
}

 
 extern CsrUint32 SSIsPaic;

/* Initialize the peer_com module */
void peer_server_com_init(int port)
{
    extern int errno;
    struct sockaddr_in serveraddr;
    int reuse_addr;
    pthread_mutexattr_t mutex_attr;
    int res;
  
#ifdef PEER_DEBUG
    printf(CSR_SS_AP_LOG_STR "init_external_communication, port=%d\n", port);
#endif

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&conn_access_mutex, &mutex_attr);
 
 
    SSIsPaic=FALSE;

    /* setup socket */
    srv_listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(srv_listen_fd < 0)
    {
        printf(CSR_SS_AP_LOG_STR "could not connect to socket, code %i - %s\n", errno, strerror(errno));
		return;
     //   exit(1);
    }
    CsrMemSet(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    reuse_addr = 1;
    if(setsockopt(srv_listen_fd,
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  (char*)&reuse_addr,
                  sizeof(reuse_addr)) < 0)
    {
        printf(CSR_SS_AP_LOG_STR "could not set socket options, code %i - %s\n", errno, strerror(errno));
			 return;

    }

    if(bind(srv_listen_fd,(struct sockaddr*)&serveraddr,
             sizeof(serveraddr)) < 0)
    {
        printf(CSR_SS_AP_LOG_STR "could bind to socket, code %i - %s\n", errno, strerror(errno));

			 return;

    }

    if (listen(srv_listen_fd,5) < 0)
    {
        printf(CSR_SS_AP_LOG_STR "could not listen on socket, code %i - %s\n", errno, strerror(errno));
			 return;

    }

    /* Setup fifo between scheduler and writer task */
    sched_tx_fifo = (sched_tx_fifo_t*)CsrPmemAlloc(sizeof(sched_tx_fifo_t));
    sched_tx_fifo->first = NULL;
    sched_tx_fifo->last = NULL;
 
    pthread_mutex_init(&sched_tx_fifo->access, NULL);
    pthread_cond_init(&sched_tx_fifo->cond, NULL);
    
    /* Create reader and writer thread */
    peer_com_run = TRUE;

    res = pthread_create(&tid_rx_thread, NULL, external_srv_rx_thread, NULL);
    if(res != 0)
    {
        printf(CSR_SS_AP_LOG_STR  "pthread_create failed to create rx-thread");

			 return;


    }
    res = pthread_create(&tid_tx_thread, NULL, external_srv_tx_thread, NULL);
    if(res != 0)
    {
        printf(CSR_SS_AP_LOG_STR  "pthread_create failed to create tx-thread");

			 return;


    }

}

/* Shutdown the peer_com module */
void peer_server_com_deinit(void)
{
//    sco_data_t *ptr;
//    sco_data_t *next;
    int ret;
    void *thread_res;

    /* Dummy checking */
    if(peer_com_run == FALSE)
    {
        printf(CSR_SS_AP_LOG_STR  "threads never started");
        return;
    }

    /* Raise stop sign */
//    register_bg_int(5,NULL);
    peer_com_run = FALSE;

    /* Join tx thread */
    if(tid_tx_thread != 0)
    {
        pthread_cond_signal(&sched_tx_fifo->cond);
        ret = pthread_join(tid_tx_thread, &thread_res);
        if(ret != 0)
        {
            printf(CSR_SS_AP_LOG_STR  "tx-thread join error");
        }
        tid_tx_thread = 0;
    }
    else
    {
        printf(CSR_SS_AP_LOG_STR  "tx-thread not running");
    }

    /* Join rx thread */
    if(tid_rx_thread != 0)
    {
        ret = pthread_join(tid_rx_thread, &thread_res);
        if(ret != 0)
        {
            printf(CSR_SS_AP_LOG_STR "rx-thread join error");
        }
        tid_rx_thread = 0;
    }
    else
    {
        printf(CSR_SS_AP_LOG_STR  "rx-thread not running");
    }

    close(srv_listen_fd);

    pthread_cond_destroy(&sched_tx_fifo->cond);
    pthread_mutex_destroy(&sched_tx_fifo->access);
    CsrPmemFree(sched_tx_fifo);

 
  
    pthread_mutex_destroy(&conn_access_mutex);

  //  register_external_send(NULL);
}


/* Register the converter */
