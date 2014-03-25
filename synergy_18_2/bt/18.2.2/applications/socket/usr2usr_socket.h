#ifndef _USR2USR_SOCKET_H_
#define _USR2USR_SOCKET_H_
/******************************************************************************

                 (c) Cambridge Silicon Radio Ltd, 2006

                 All rights reserved

FILE:            usr2usr_socket.h

DESCRIPTION:     usr2usr_socket defines

REVISION:        $Revision: #1 $

******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/**
 * External protocol
 *
 *  /-----Header---\   /--- Payload --\
 * / uint16   uint16\ /                \
 * [Version][  type  ][     payload     ]
 *
 *
 * type = { CONNECT_req | CONNECT_cfm | DATA_ind |
 *          REGISTER_SCO_HANDLE_req | REGISTER_SCO_HANDLE_cfm |
 *          SCO_DATA_ind }
 *
 * CONNECT_req: CsrUint16 ID
 *
 * CONNECT_cfm: CsrUint16 ID
 *              CsrUint16 Handle
 *
 * DATA_ind   : CsrUint16 payload_length = (type+cmd + msgSize)
 *              CsrUint16 type
 *              CsrUint16 cmd
 *              CsrUint16 msgSize
 *              CsrUint8  msg<>
 *
 * REGISER_SCO_HANDLE_req : CsrUint16 sco_handle
 */
#define EXT_VERSION 1

#define EXT_HEADER_SIZE (4)

#define EXT_CONNECT_req               (0x01)
#define EXT_CONNECT_cfm               (0x02)
#define EXT_DATA_ind                  (0x03)
#define EXT_PCM_DATA_ind			  (0x04)

#define EXT_CONNECT_req_NO_IOV                (3)
#define EXT_CONNECT_cfm_NO_IOV                (4)
#define EXT_DATA_ind_NO_IOV                   (6)
#define EXT_REGISTER_SCO_HANDLE_req_NO_IOV    (3)
#define EXT_DEREGISTER_SCO_HANDLE_req_NO_IOV  (3)
#define EXT_DEREGISTER_SCO_HANDLE_cfm_NO_IOV  (3)
#define EXT_SCO_DATA_ind_NO_IOV               (5)

#define EXT_VERSION_IDX        (0)
#define EXT_TYPE_IDX           (EXT_VERSION_IDX + sizeof(CsrUint16))
#define EXT_CMD_IDX           (EXT_VERSION_IDX + EXT_TYPE_IDX+sizeof(CsrUint16))


#define EXT_PROFIL_GAP_ID    (1)
#define EXT_PROFIL_A2DP_ID   (2)





void usr2usr_socket_dump_data(CsrUint16 len, unsigned char *data);
void usr2usr_socket_dump_iov(struct iovec *v, int count);

int read_external_data(int fd,
                       CsrUint16 length, unsigned char *buffer);

void send_DATA_ind(int fd,
                    CsrUint16  id, CsrUint16  cmd,
                    unsigned char  *msg, CsrUint16  msgSize);
#if 0

void send_SCO_DATA_ind(int fd,
                       CsrUint16 _sco_hdl,
                       CsrUint8 data_len, char *data);
#endif
#ifdef __cplusplus
}
#endif

#endif
