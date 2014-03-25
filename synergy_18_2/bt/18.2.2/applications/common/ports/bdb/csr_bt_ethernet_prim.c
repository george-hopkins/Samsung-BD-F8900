/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_bt_tasks.h"
#include "csr_bt_ethernet_prim.h"
#include "csr_bt_internet_primitives.h"
#include "platform/csr_ethernet.h"
#include "platform/csr_interrupt.h"

typedef struct
{
    CsrSchedBgint bgint;
    EthernetMacAddress macAddress;
    void (*wakeonLanFunc) (void);
} EthernetInstanceData;

static EthernetInstanceData ethernetInstanceData;

void CsrBtTesterEtherSetWakeOnLanCbf(void (*fn) (void))
{
    printf("CsrBtTesterEtherSetWakeOnLanCbf()\n");
    ethernetInstanceData.wakeonLanFunc = fn;
}

#define WAKEONLAN_LENGTH (16*3)  /* length of wake-on lan patter in short */

static void csrBtTesterEthernetWakeOnLan(PACKET_BUFFER *packet)
{
    static unsigned short mac[3] = {0,0,0};
    CsrUint16 *wordPtr;
    CsrUint16 i;
    CsrUint16 temp;

    /* check for ether type - IP */
    if (packet->type != 0x0800)
    {
        return;
    }

    wordPtr = (CsrUint16 *) &packet->data[0]; /* point at IP header */
    temp = CSR_BT_LAN_WORD(*wordPtr) >> 8;
    if ((temp & 0xf0) != 0x40)
    {
        return; /* not IPv4 */
    }
    wordPtr += (temp & 0x0f) * 2; /* header len in 32bit - adjust to 16bit ptr */
    wordPtr += 4; /* discard UDP header */

    /* check for wakeon lan pattern */
    for (i=0; i<3; i++)
    {
        if (*wordPtr++ != 0xffff)
        {
            return;
        }
    }
    for (i=0; i<WAKEONLAN_LENGTH; i++)
    {
        if (*wordPtr++ != mac[i%3])
        {
            return;
        }
    }
    ethernetInstanceData.wakeonLanFunc();
    return;
}

static void csrBtTesterEthernetRxBgIntHandler(void *unused)
{
    PACKET_BUFFER *packet;
    ETHERNET_PACKET *packetInd;

    /* Retrieve a single packet from the ethernet interface */
    packet = (PACKET_BUFFER *) EthernetPacketReceive();

    /* If a packet was received, put it on the IP queue */
    if (packet)
    {
        if (NULL != ethernetInstanceData.wakeonLanFunc)
        {
            csrBtTesterEthernetWakeOnLan(packet);
        }
        packetInd = (ETHERNET_PACKET *) CsrPmemAlloc(sizeof(ETHERNET_PACKET));
        packetInd->type = ETHERNET_PACKET_IND;
        packetInd->packet = packet;
        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, ETHERNET_PRIM, packetInd);
    }

    if (EthernetPacketAvailable())
    {
        /* The CsrSchedBgintSet() call must be protected because it is also called
           in interrupts and no mutex protection is active in the scheduler */
        INT_GLOBAL_DISABLE();
        CsrSchedBgintSet(ethernetInstanceData.bgint);
        INT_GLOBAL_ENABLE();
    }
}

static void csrBtTesterEthernetRxBgIntHandlerWrapper(void)
{
    if(ethernetInstanceData.bgint != CSR_SCHED_BGINT_INVALID)
    {
        CsrSchedBgintSet(ethernetInstanceData.bgint);
    }
    else
    {
        printf("Ethernet Rx BgInt occured during deinit.\n");
    }
}

void CsrBtTesterEthernetInitialise(EthernetMacAddress *macAddress)
{
    ethernetInstanceData.macAddress = *macAddress;
    ethernetInstanceData.bgint = CsrSchedBgintReg(csrBtTesterEthernetRxBgIntHandler, NULL, "csrBtTesterEthernetRxBgIntHandler");
    EthernetInitialise(csrBtTesterEthernetRxBgIntHandlerWrapper, &ethernetInstanceData.macAddress, FALSE);
}

void CsrBtTesterEthernetDeinitialise(void)
{
    EthernetDeinitialise();

    CsrSchedBgintUnreg(ethernetInstanceData.bgint);
    ethernetInstanceData.bgint = CSR_SCHED_BGINT_INVALID;
}

