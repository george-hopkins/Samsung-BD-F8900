#ifndef __TRANSPORT_ABSTRACTION_H__
#define __TRANSPORT_ABSTRACTION_H__
/****************************************************************************

                (c) CSR plc 2008
                  
                All rights reserved

FILE:           transport_abstraction.h

DESCRIPTION:    Transport Abstraction interface

REVISION:       $Revision: #2 $

****************************************************************************/

#include "sched/csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions for transport channels */

/* Send primitive on a unreliable channel */
#define TRANSPORT_UNRELIABLE_CHANNEL         (0)

/* Send primitive on a reliable channel */
#define TRANSPORT_RELIABLE_CHANNEL           (1)

#define TRANSPORT_CHANNEL_BCCMD              (2)
#define TRANSPORT_CHANNEL_HQ                 (3)
#define TRANSPORT_CHANNEL_DM                 (4)
#define TRANSPORT_CHANNEL_HCI                (5)
#define TRANSPORT_CHANNEL_ACL                (6)
#define TRANSPORT_CHANNEL_SCO                (7)
#define TRANSPORT_CHANNEL_L2CAP              (8)
#define TRANSPORT_CHANNEL_RFCOMM             (9)
#define TRANSPORT_CHANNEL_SDP                (10)
#define TRANSPORT_CHANNEL_DFU                (12)
#define TRANSPORT_CHANNEL_VM                 (13)

/* Defines for setting/resetting the msb of the handle for downstream/upstream primitives */
#define TRANSPORT_HANDLE_ON_CHIP_FLAG_SET    (0x8000)
#define TRANSPORT_HANDLE_ON_CHIP_FLAG_RESET  (0x7FFF)


/* Definitions of transport types */
#define TRANSPORT_TYPE_BCSP                  (1)
#define TRANSPORT_TYPE_USB                   (2)
#define TRANSPORT_TYPE_H4_UART               (3) /* Unsupported - Use the H4DS protocol */
#define TRANSPORT_TYPE_H5_THREE_WIRE         (6) /* Unsupported - Use the BCSP protocol */
#define TRANSPORT_TYPE_H4_DS                 (7)
#define TRANSPORT_TYPE_TYPE_A                (8)
#define TRANSPORT_TYPE_PIPE                  (9)

/* Message struct used when data is sent/received to/from the host controller*/
typedef struct 
{
    char         *buf;
    unsigned     buflen;
    unsigned     dex;
} MessageStructure;

typedef struct txmsg {
    struct txmsg        *next;  /* Next in linked list. */
    MessageStructure    *m;     /* The message (reference) itself. */
    unsigned int        chan:4; /* BCSP channel. */
    unsigned int        seq:3;  /* Sequence number. */
} TXMSG;

/* Transport Abstraction functions */
bool_t TransportStart(char *configString);
void TransportStop(void);
void TransportReset(void);
uint16_t TransportTypeQuery(void);
void TransportMsgTx(TXMSG *msg);
void TransportMsgClean(void * msg);
void TransportScoTx(char *scoData);
bool_t TransportScoRx(char *scoData);

/*----------------------------------------------------------------------------*
 *  NAME
 *      TransportMsgRx
 *
 *  DESCRIPTION
 *      This function is called when a message is received from the transport
 *      layer, e.g.UART/BCSP, SDIO etc.
 *
 *  RETURNS
 *
 *----------------------------------------------------------------------------*/
void TransportMsgRx(MessageStructure *msg, uint8_t transportChannel);


/* For backwards compatibility */
/*----------------------------------------------------------------------------*
 *  NAME
 *      hcCom_ReceiveMsg
 *
 *  DESCRIPTION
 *      This function is called when a message is received from abcsp.
 *
 *  RETURNS
 *
 *----------------------------------------------------------------------------*/
void hcCom_ReceiveMsg(void *msg, uint8_t bcspChannel, uint8_t rel);

#ifdef __cplusplus
}
#endif

#endif 
