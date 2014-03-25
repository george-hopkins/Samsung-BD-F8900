/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

void CsrBtAmpmInit(void **gash)
{
    CsrBtAmpmInstance * inst;
    L2CA_CONFIG_T config;

    inst = CsrPmemZalloc(sizeof(CsrBtAmpmInstance));
    *gash = inst;

    inst->ampmRxSduControl = CSR_BT_AMPM_CONTROL_RX_SDU_DISABLED;
    inst->ampmRxSduQid = CSR_SCHED_QID_INVALID;
    inst->ampPowerOnQid = CSR_SCHED_QID_INVALID; 

    /* Init local subsystems */
    CsrBtAmpmAppInit(inst);
    CsrBtAmpmPrfInit(inst);
    CsrBtAmpmDataInit(inst);
    CsrBtAmpmBtdevInit(inst);
    CsrBtAmpmLocalInit(inst);
    CsrBtAmpmPhyInit(inst);
    CsrBtAmpmLogicalInit(inst);
    CsrBtAmpmL2capInit(inst);

    /* Register our interest in fixed channel 3 */
    (void)CsrMemSet(&config, 0, sizeof(L2CA_CONFIG_T));
    config.options = L2CA_SPECIFY_MTU | L2CA_SPECIFY_FLOW;
    config.mtu = CSR_BT_AMPM_L2CONF_MTU_MPS;
    config.fcs = L2CA_FCS_ON;
    config.flow = (L2CA_FLOW_T*)CsrPmemAlloc(sizeof(L2CA_FLOW_T));
    config.flow->mode = L2CA_FLOW_MODE_ENHANCED_RETRANS;
    config.flow->tx_window = CSR_BT_AMPM_L2CONF_TXWIN;
    config.flow->max_retransmit = CSR_BT_AMPM_L2CONF_MAX_RETX;
    config.flow->retrans_timeout = CSR_BT_AMPM_L2CONF_RETRANS_TO;
    config.flow->monitor_timeout = CSR_BT_AMPM_L2CONF_MONITOR_TO;
    config.flow->maximum_pdu = CSR_BT_AMPM_L2CONF_MTU_MPS;

    /* We don't specify the service type as "best effort" or the flush
     * timeout as these are both defaults. */
    L2CA_RegisterFixedCidReq(CSR_BT_AMPM_IFACEQUEUE,
                             L2CA_CID_AMP_MANAGER, /* cid 3 */
                             &config,
                             0); /* reg_ctx */

    /* Register with DM to receive various DM ACL information */
    dm_ampm_register_req(CSR_BT_AMPM_IFACEQUEUE);

    /* And register ourselves with the CM in order to receive all AMP
     * related L2CAP messages */
    CsrBtCmRegisterHandlerReqSend(CSR_BT_CM_HANDLER_TYPE_AMP,
                                  CSR_BT_AMPM_IFACEQUEUE,
                                  0); /* flags */
}

void CsrBtAmpmDeinit(void **gash)
{
    CsrBtAmpmInstance *inst = (CsrBtAmpmInstance*)*gash;
    void *prim=NULL;
    CsrUint16 class=0;

    /* Empty message queue */
    while(CsrSchedMessageGet(&class, &prim))
    {
        switch(class)
        {
            case CSR_BT_L2CAP_PRIM:
                L2CA_FreePrimitive(prim);
                prim = NULL;
                break;

            case CSR_BT_AMPM_PRIM:
                CsrBtAmpmFreeDownstreamMessageContents(class, prim);
                break;

            case CSR_BT_DM_PRIM:
                dm_free_upstream_primitive(prim);
                prim = NULL;
                break;

            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(class, prim);
                break;

            case CSR_BT_CM_PRIM:
                CsrBtCmFreeUpstreamMessageContents(class, prim);
                break;

            default:
                /* Ignore */
                break;
        }
        /* Free message container */
        CsrPmemFree(prim);
    }

    /* Shutdown local subsystems */
    CsrBtAmpmAppDeinit(inst);
    CsrBtAmpmPrfDeinit(inst);
    CsrBtAmpmLogicalDeinit(inst);
    CsrBtAmpmPhyDeinit(inst);
    CsrBtAmpmL2capDeinit(inst);
    CsrBtAmpmLocalDeinit(inst);
    CsrBtAmpmBtdevDeinit(inst);
    CsrBtAmpmDataDeinit(inst);

    /* Free instance */
    CsrPmemFree(inst);
    *gash = NULL;
}

void CsrBtAmpmInterfaceHandler(void **gash)
{
    CsrBtAmpmInstance * inst;

    inst = (CsrBtAmpmInstance*)*gash;

    if(CsrSchedMessageGet(
                     (CsrUint16*)&(inst->msg_class),
                     (void*)&(inst->msg)))
    {
        switch (inst->msg_class)
        {
            case CSR_BT_L2CAP_PRIM:
                CsrBtAmpmL2capHandler(inst);
                L2CA_FreePrimitive(inst->msg);
                inst->msg = NULL;
                break;

            case CSR_BT_AMPM_PRIM:
                if(*(CsrUint16*)inst->msg == CSR_BT_AMPM_PAL_HCI_EVENT_REQ)
                {
                    CsrBtAmpmHciHandler(inst);
                }
                else
                {
                    CsrBtAmpmHandler(inst);
                }
                CsrBtAmpmFreeDownstreamMessageContents(CSR_BT_AMPM_PRIM, inst->msg);
                break;

            case CSR_BT_DM_PRIM:
                CsrBtAmpmDmHandler(inst);
                dm_free_upstream_primitive(inst->msg);
                inst->msg = NULL;
                break;

            case CSR_BT_SC_PRIM:
                CsrBtAmpmDataScHandler(inst);
                CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, inst->msg);
                break;

            case CSR_BT_CM_PRIM:
                CsrBtAmpmAppCmHandler(inst);
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, inst->msg);
                break;

            case CSR_SCHED_PRIM:
                /* Ignore */
                break;

            default:
                /* Ignore */
                break;
        }

        CsrPmemFree(inst->msg);
        inst->msg = NULL;
        inst->msg_class = 0;
    }
}
