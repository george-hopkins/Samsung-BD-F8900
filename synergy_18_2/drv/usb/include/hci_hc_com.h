#ifndef HCI_HC_COM_H__
#define HCI_HC_COM_H__
/*******************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           hci_hc_com.h $

DESCRIPTION:    This file contains interface definition common for HCI top and HC-COM.

REVISION:       $Revision: #3 $

*******************************************************************************/
#include "sched/csr_types.h"
#include "transport_abstraction.h"
#include "sco_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/
/* Start of backwards compatibility section */

/* Send primitive on a unreliable channel */
#define BCSP_UNRELIABLE_CHANNEL        TRANSPORT_UNRELIABLE_CHANNEL

/* Send primitive on a reliable channel */
#define BCSP_RELIABLE_CHANNEL          TRANSPORT_RELIABLE_CHANNEL

#define BCSP_CHANNEL_BCCMD             TRANSPORT_CHANNEL_BCCMD
#define BCSP_CHANNEL_HQ                TRANSPORT_CHANNEL_HQ
#define BCSP_CHANNEL_DM                TRANSPORT_CHANNEL_DM
#define BCSP_CHANNEL_HCI               TRANSPORT_CHANNEL_HCI
#define BCSP_CHANNEL_ACL               TRANSPORT_CHANNEL_ACL
#define BCSP_CHANNEL_SCO               TRANSPORT_CHANNEL_SCO
#define BCSP_CHANNEL_L2CAP             TRANSPORT_CHANNEL_L2CAP
#define BCSP_CHANNEL_RFCOMM            TRANSPORT_CHANNEL_RFCOMM
#define BCSP_CHANNEL_SDP               TRANSPORT_CHANNEL_SDP
#define BCSP_CHANNEL_DFU               TRANSPORT_CHANNEL_DFU
#define BCSP_CHANNEL_VM                TRANSPORT_CHANNEL_VM

#if MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS > 0
bool_t lookForScoHandle(char *theBuf);
void initScoHandle(void);
#else
#define lookForScoHandle(theBuf) FALSE
#define initScoHandle()
#endif

#ifdef __cplusplus
}
#endif

#endif /* ndef _HCI_HC_COM_H */
