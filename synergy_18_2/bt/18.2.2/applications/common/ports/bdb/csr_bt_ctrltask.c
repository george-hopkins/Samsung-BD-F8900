/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "csr_panic.h"
#include "csr_pmem.h"
#include "csr_log.h"
#include "csr_bt_util.h"
#include "csr_bt_internet_primitives.h"
#include "csr_bt_ethernet_prim.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_icmp_prim.h"
#include "csr_bt_dhcp_prim.h"
#include "csr_bt_udp_prim.h"

#include "platform/csr_confdata.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#define CSR_BT_CTRL_STRINGIFY_REAL(a) #a
#define CSR_BT_CTRL_STRINGIFY(a) CSR_BT_CTRL_STRINGIFY_REAL(a)

#define CSR_BT_CTRL_ASSERT(cond)                        \
do {                                                \
    if (!(cond))                                    \
    {                                               \
        char *panic_arg = "[" __FILE__ ":" CSR_BT_CTRL_STRINGIFY(__LINE__) "] - " CSR_BT_CTRL_STRINGIFY(cond); \
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic_arg); \
    }                                               \
} while(0)

typedef struct
{
    void    *msg;
    CsrUint16 ethernetAddress[3];
    CsrUint16 ipAddress[2]; /* my address */
    CsrSchedQid queue;
} CtrlTaskInstData_t;

static void DhcpActivateClientReqSend(CsrSchedQid queue, CsrUint16 ethernetAddress[])
{
    CsrBtDhcpActivateClientReq * dhcpActivateReq;

    dhcpActivateReq = (CsrBtDhcpActivateClientReq *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientReq));
    dhcpActivateReq->type = CSR_BT_DHCP_ACTIVATE_CLIENT_REQ;
    dhcpActivateReq->applicationQueue = queue;
    dhcpActivateReq->ethernetAddress[0] = ethernetAddress[0];
    dhcpActivateReq->ethernetAddress[1] = ethernetAddress[1];
    dhcpActivateReq->ethernetAddress[2] = ethernetAddress[2];

    CsrSchedMessagePut(CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_PRIM, dhcpActivateReq);
}

void CsrBtCtrlInit(void **gash)
{
    CtrlTaskInstData_t *instanceData;
    CsrUint16 length;
    EthernetMacAddress macAddress;
    CsrInt32 res;
    CsrUint8 *confData;

    *gash = instanceData = (CtrlTaskInstData_t*) CsrPmemZalloc(sizeof(CtrlTaskInstData_t));

    instanceData->queue = CsrSchedTaskQueueGet();

    ConfDataRead(&confData);

    /* Get Mac Address and initialise ethernet card */
    res = ConfDataGetValue(confData, CONFDATA_TAG_MACADDRESS, &length, (CsrUint8 *) instanceData->ethernetAddress);
    CSR_BT_CTRL_ASSERT(res == CONFDATA_OK && length == CONFDATA_SIZE_MACADDRESS);
    CsrMemCpy(macAddress.a, instanceData->ethernetAddress, sizeof(macAddress.a));
    CsrBtTesterEthernetInitialise(&macAddress);

    printf("\nDHCP: Requesting IP address for %02X:%02X:%02X:%02X:%02X:%02X\n", instanceData->ethernetAddress[0] & 0xFF,
                                                                                (instanceData->ethernetAddress[0] >> 8) & 0xFF,
                                                                                instanceData->ethernetAddress[1] & 0xFF,
                                                                                (instanceData->ethernetAddress[1] >> 8) & 0xFF,
                                                                                instanceData->ethernetAddress[2] & 0xFF,
                                                                                (instanceData->ethernetAddress[2] >> 8) & 0xFF);

    DhcpActivateClientReqSend(instanceData->queue, instanceData->ethernetAddress);
}

void CsrBtCtrlHandler(void **gash)
{
    CtrlTaskInstData_t *instanceData;
    CsrUint16 primId=0;
    CsrUint16 event;

    instanceData = (CtrlTaskInstData_t *) *gash;

    if (CsrSchedMessageGet(&primId, &instanceData->msg))
    {
        event = *((CsrUint16 *) instanceData->msg);
        switch (primId)
        {
            case CSR_BT_DHCP_PRIM:
                {
                    switch (event)
                    {
                        case CSR_BT_DHCP_ACTIVATE_CLIENT_CFM:
                            {
                                CsrBtDhcpActivateClientCfm *confirm;

                                confirm = (CsrBtDhcpActivateClientCfm *) instanceData->msg;
                                if ((confirm->result == CSR_BT_DHCP_RESULT_CONFIGURED) || (confirm->result == CSR_BT_DHCP_RESULT_ALREADY_ACTIVE))
                                {
                                    instanceData->ipAddress[0] = confirm->ipAddress[0];
                                    instanceData->ipAddress[1] = confirm->ipAddress[1];
                                    printf("DHCP: Assigned IP address %d.%d.%d.%d\n", (confirm->ipAddress[0]) & 0xFF, (confirm->ipAddress[0] >> 8) & 0xFF, (confirm->ipAddress[1]) & 0xFF, (confirm->ipAddress[1] >> 8) & 0xFF);
                                }
                                else
                                {
                                    printf("DHCP: No response from DHCP server - check cable (Error code: 0x%x) \n", confirm->result);
                                    DhcpActivateClientReqSend(instanceData->queue, instanceData->ethernetAddress);
                                }
                                break;
                            }
                        case CSR_BT_DHCP_DEACTIVATE_CLIENT_IND:
                            {
                                printf("DHCP: Failed to renew IP address - check cable\n");
                                DhcpActivateClientReqSend(instanceData->queue, instanceData->ethernetAddress);
                                break;
                            }
                        default:
                            printf("Unexpected DHCP event 0x%04x\n", event);
                    }
                    break;
                }

            default:
                printf("Unexpected prim 0x%04x\n", primId);
                break;
        }

        CsrPmemFree(instanceData->msg);
        instanceData->msg = NULL;
    }
}

void CsrBtCtrlDeinit(void ** gash)
{
    CtrlTaskInstData_t *instanceData;

    instanceData = (CtrlTaskInstData_t *) *gash;

    CsrBtTesterEthernetDeinitialise();
    CsrPmemFree(instanceData->msg);
    CsrPmemFree(instanceData);
    *gash = NULL;
}

