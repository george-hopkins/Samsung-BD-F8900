/****************************************************************************

               (c) Cambridge Silicon Radio Ltd, 2006

               All rights reserved

FILE:          common.c

DESCRIPTION:   linux user <-> user socket (TCP/IP) peer communication,
               common part between server and client side

REVISION:      $Revision: #1 $

****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
extern int errno;
#include "csr_types.h"

//#include "sched/sched.h"
//#include "sched/pmalloc.h"
#include "peer_com_linux_u2u.h"
//#include "bchs_msg_converter.h"
//#include "sched_private.h"

#include "usr2usr_socket.h"
//#define EXTERNAL_DEBUG

/***
 * The SCO header is 3 bytes
 */
//#define SCO_HEADER_SIZE 3

void usr2usr_socket_dump_data(CsrUint16 len, unsigned char *data)
{
#ifdef EXTERNAL_DEBUG
    int i;
    int r;
    int no_rows;
    int rest;

    no_rows = len / 8;
    rest    = len - (no_rows *8);

    for(r=0;r<no_rows;r++)
    {
        for(i=0;i<8;i++)
        {
            printf("0x%02x ",data[r*8+i]);
        }

        printf("  ");

        for(i=0;i<8;i++)
        {
            if (isprint(data[r*8+i]))
            {
                printf("%c",data[r*8+i]);
            }
            else
            {
                printf(".");
            }
        }

        printf("\n");
    }

    if (rest>0)
    {
        for(i=0;i<rest;i++)
        {
            printf("0x%02x ",data[r*8+i]);
        }

        for(i=0;i<8-rest;i++)
        {
            printf("     ");
        }

        printf("  ");

        for(i=0;i<rest;i++)
        {
            if (isascii(data[r*8+i]))
            {
                printf("%c",data[r*8+i]);
            }
            else
            {
                printf(".");
            }
        }

        printf("\n");
    }

    printf("\n");
#endif
}
#ifdef EXTERNAL_DEBUG

void usr2usr_socket_dump_iov(struct iovec *v, int count)
{
    int i;
    unsigned int j;
    unsigned char *ptr;

    for(i=0;i<count;i++)
    {
        ptr = (unsigned char*)v[i].iov_base;

        printf("iov %i - length %i: ", i, v[i].iov_len);
        for(j=0;j<v[i].iov_len;j++)
        {
            printf("0x%02x ",ptr[j]);
        }
        printf("\n");
    }
}
#endif


void send_DATA_ind(int fd,
                    CsrUint16  id, CsrUint16  cmd,
                    unsigned char  *msg, CsrUint16  msgSize)
{
    CsrUint16 version;
    CsrUint16 payload_length = 0;
    CsrUint16 type;
    int ret = -1;

    struct iovec out[EXT_DATA_ind_NO_IOV];

#ifdef EXTERNAL_DEBUG
    printf("send_DATA_ind: fd=0x%d,id=0x%x,cmd=0x%x,msgSize=%d,msg=%p\n",
           fd,id,cmd,msgSize,msg);
#endif
#ifdef EXTERNAL_DEBUG_PCM
	if(cmd == 0x0103)  ///AV_PCM_STREAM_DATA_IND
	{
		int i=0;
		printf("send_DATA_ind  : len<%d>\n", payload_length);

		for(i=0; i<10;i++)
		{
		   printf("[0x%2x] ",msg[i]);
		}
		printf("\n");
		printf("[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]\n");

	}
#endif

#if 0
	if(cmd == AV_PCM_STREAM_DATA_IND)
	{
		
		CsrUint8 *p_serialized_data = (CsrUint8 *)CsrPmemAlloc(sizeof() + );
		type = EXT_PCM_DATA_ind;
	    payload_length = msgSize+2*sizeof(CsrUint16);

	    out[0].iov_base = &version;
	    out[0].iov_len  = sizeof(version);
	    out[1].iov_base = &type;
	    out[1].iov_len  = sizeof(type);
		out[2].iov_base = &msgSize;
	    out[2].iov_len  = sizeof(msgSize);
		out[3].iov_base = &id;
	    out[3].iov_len  = sizeof(id);
	    out[4].iov_base = &cmd;
	    out[4].iov_len  = sizeof(cmd);
	    out[5].iov_base = &msg;
	    out[5].iov_len  = msgSize;

	    writev(fd, out,EXT_DATA_ind_NO_IOV);
	}
	else
#endif
	{
		version 	   = EXT_VERSION;
	    type           = EXT_DATA_ind;

	    //payload_length = msgSize + (5 * sizeof(CsrUint16));

	    out[0].iov_base = &version;
	    out[0].iov_len  = sizeof(version);
	    out[1].iov_base = &type;
	    out[1].iov_len  = sizeof(type);
		out[2].iov_base = &msgSize;
	    out[2].iov_len  = sizeof(msgSize);
		out[3].iov_base = &id;
	    out[3].iov_len  = sizeof(id);
	    out[4].iov_base = &cmd;
	    out[4].iov_len  = sizeof(cmd);
	    out[5].iov_base = &msg;
	    out[5].iov_len  = msgSize;

#if 0
	   ret = writev(fd, &out[0], 1);

           payload_length -= ret;
	   printf("send_DATA_ind : ret_0<%d>, payload_length<%d>\n", ret, payload_length);

	   ret = writev(fd, &out[1], 1);

           payload_length -= ret;
	   printf("send_DATA_ind : ret_1<%d>, payload_length<%d>\n", ret, payload_length);

	   ret = writev(fd, &out[2], 1);
           payload_length -= ret;
	   printf("send_DATA_ind : ret_2<%d>, payload_length<%d>\n", ret, payload_length);

	   ret = writev(fd, &out[3], 1);
           payload_length -= ret;
	   printf("send_DATA_ind : ret_3<%d>, payload_length<%d>\n", ret, payload_length);

	   ret = writev(fd, &out[4], 1);
           payload_length -= ret;
	   printf("send_DATA_ind : ret_4<%d>, payload_length<%d>\n", ret, payload_length);

	   ret = writev(fd, &out[5], 1);

	   if(ret > 0)
	   {
		   payload_length -= ret;
		   printf("send_DATA_ind : ret_5<%d>, payload_length<%d>\n", ret, payload_length);
	   }

           ret = 0;

	   while(payload_length > 0 && ret >= 0)
	   {
		 out[5].iov_base = &msg[ret];
		 out[5].iov_len = payload_length;

                 ret = writev(fd, &out[5], 1);
	         payload_length -= ret;
	        printf("send_DATA_ind : loop   ret<%d>, payload_length<%d>\n", ret, payload_length);
	   }
#else
	{
		ret = writev(fd, out, 5);
#ifdef EXTERNAL_DEBUG
		printf("send_DATA_ind : ret<%d>\n", ret);
#endif
		if(msgSize > 0 && msg)
		{
			payload_length = msgSize;
#ifdef EXTERNAL_DEBUG
			printf("send_DATA_ind : ret<%d>, payload_length<%d>\n", ret, payload_length);
#endif		
			do
			{
				ret = write(fd, msg, payload_length);
				payload_length -= ret;
#ifdef EXTERNAL_DEBUG				
				printf("send_DATA_ind : write....ret <%d>, payload_length<%d>\n", ret, payload_length);
#endif
			}while(ret > 0 && payload_length > 0);
		}
	}
#if 0
	else
	{
		ret = writev(fd, out, 6);
		printf("send_DATA_ind : ret<%d>\n", ret);
	}
#endif
#endif
	}

#ifdef EXTERNAL_DEBUG
    usr2usr_socket_dump_iov(out,EXT_DATA_ind_NO_IOV);
#endif
}
#if 0

void send_SCO_DATA_ind(int fd,
                       CsrUint16 sco_hdl,
                       uint8_t data_len, char *data)
{
    CsrUint16 version;
    CsrUint16 type;
    CsrUint16 payload_length;
    struct iovec out[EXT_SCO_DATA_ind_NO_IOV];

#ifdef EXTERNAL_DEBUG
    printf("send_SCO_DATA_ind: fd=%d,sco_hdl=0x%0x, data_len=%d,data=%p\n",
           fd,sco_hdl,data_len,data);
#endif

    version        = EXT_VERSION;
    type           = EXT_SCO_DATA_ind;
    payload_length = SCO_HEADER_SIZE+data_len;

    out[0].iov_base = &version;
    out[0].iov_len  = sizeof(version);
    out[1].iov_base = &type;
    out[1].iov_len  = sizeof(type);
    out[2].iov_base = &sco_hdl;
    out[2].iov_len  = sizeof(sco_hdl);
    out[3].iov_base = &payload_length;
    out[3].iov_len  = sizeof(payload_length);
    out[4].iov_base = data;
    out[4].iov_len  = SCO_HEADER_SIZE+data_len;

    writev(fd, out,EXT_SCO_DATA_ind_NO_IOV);

#ifdef EXTERNAL_DEBUG
    usr2usr_socket_dump_iov(out,EXT_SCO_DATA_ind_NO_IOV);
#endif
}
#endif

int read_external_data(int fd,
                       CsrUint16 length, unsigned char *buffer)
{
    int missing;
    int no_read;
    int n;

    no_read = 0;
    missing = length;
#ifdef EXTERNAL_DEBUG
		printf("read_external_data\n");
#endif

    while(missing)
    {
        n = read(fd,&buffer[no_read],missing);

        if (n>0)
        {
            missing -= n;
            no_read += n;
        }
        else
        {
            no_read = 0;
            break;
        }
    }

    return(no_read);
}
