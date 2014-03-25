/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

#include "csr_bt_tasks.h"
#include "csr_pmem.h"

#include "csr_bt_phdc_mgr_main.h"
#include "csr_bt_phdc_mgr_fsm.h"
#include "csr_bt_phdc_mgr_util.h"
#include "csr_bt_phdc_common.h"

extern void CsrBtPhdcMgrFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

fsm_event_t CsrBtPhdcMgrIfHandler(CsrBtPhdcMgrInstance *pInstance)
{
    fsm_event_t event =  PHDC_INVALID_EVENT;

    void *prim = pInstance->pRecvMsg;

    switch (*(CsrUint16 *) (prim))
    {
        case CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ:
        {
            event = PHDC_MGR_EV_PHDC_CONFIGURE_ENDPOINT_REQ; 
            break;
        }
        case CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES:
        {
            event = PHDC_MGR_EV_PHDC_CONFIGURE_ENDPOINT_RES; 
            break;
        }
        case CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES:
        {
            event = PHDC_MGR_DEVICE_EV_PHDC_ACCEPT_ASSOCIATE_RES;
            break;
        }
        case CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES:
        {
            event = PHDC_MGR_DEVICE_EV_PHDC_ACCEPT_BT_LINK_RES;
            break;
        }
        case CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ:
        {
            event = PHDC_MGR_EV_PHDC_GET_DEVICE_CAPAB_REQ;
            break;
        }
        case CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ:
        {
            event = PHDC_MGR_DEVICE_EV_PHDC_SETUP_BT_LINK_REQ;
            break;
        }
        case CSR_BT_PHDC_MGR_DISASSOCIATE_REQ:
        {
            event = PHDC_MGR_DEVICE_EV_PHDC_DISASSOCIATE_REQ;
            break;
        }
        case CSR_BT_PHDC_MGR_UNREGISTER_REQ:
        {
            event = PHDC_MGR_EV_PHDC_UNREGISTER_REQ;
            break;
        }
        default:
            CSR_BT_PHDC_M_ASSERT(FALSE);
            break;
    }

    return event;
}


fsm_event_t CsrBtPhdcMgrHdpIfHandler(CsrBtPhdcMgrInstance *pInstance)
{
    fsm_event_t event =  PHDC_INVALID_EVENT;

    void *prim = pInstance->pRecvMsg;

    switch (*(CsrUint16 *) (prim))
    {
        case CSR_BT_HDP_REGISTER_IND:
        {
            event = PHDC_MGR_EV_HDP_REGISTER_IND;
            break;
        }
        case CSR_BT_HDP_REGISTER_CFM :
        {
            event = PHDC_MGR_EV_HDP_REGISTER_CFM;
            break;
        }
        case CSR_BT_HDP_ACTIVATE_CFM:
        {
            event = PHDC_MGR_EV_HDP_ACTIVATE_CFM;
            break;
        }
        case CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_DATA_IND:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_DATA_IND;
            break;
        }
        case CSR_BT_HDP_DATA_CFM:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_DATA_CFM;
            break;
        }
        case CSR_BT_HDP_RESUME_IND:
        {
            event = PHDC_MGR_EV_HDP_RESUME_IND;
            break;
        }
        case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM:
        {
            if(pInstance->state == CSR_BT_PHDC_MGR_ST_ACTIVATED)
            {
                event = PHDC_MGR_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM;
            }
            else
            {
                event = PHDC_MGR_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM;
            }
            break;
        }
        case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
        {
            event = PHDC_MGR_EV_HDP_CTRL_GET_CAPAB_IND;
            break;
        }
        case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
        {
            event = PHDC_MGR_EV_HDP_CTRL_GET_CAPAB_CFM;
            break;
        }
        case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM:
        {
            event = PHDC_MGR_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM;
            break;
        }
        case CSR_BT_HDP_DEACTIVATE_CFM:
        {
            event = PHDC_MGR_EV_HDP_DEACTIVATE_CFM;
            break;
        }
        case CSR_BT_HDP_UNREGISTER_CFM:
        {
            event = PHDC_MGR_EV_HDP_UNREGISTER_CFM;
            break;
        }
        default:
            CSR_BT_PHDC_M_ASSERT(FALSE);
            break;
    }
    return event;
}
    


void CsrBtPhdcMgrInstanceInit(CsrBtPhdcMgrInstance *pInst)
{
    pInst->qId = CSR_BT_PHDC_MGR_INVALID_VAL;
    pInst->state = CSR_BT_PHDC_MGR_ST_IDLE;

    CsrCmnListInit(&pInst->deviceList, 
                    0, 
                    CsrBtPhdcMgrCmnListDeviceAdd, 
                    CsrBtPhdcMgrCmnListDeviceRemove);
    CsrCmnListInit(&pInst->typeSpec, 
                    0, 
                    CsrBtPhdcMgrCmnListTypeSpecAdd, 
                    CsrBtPhdcMgrCmnListTypeSpecRemove);    
}

void CsrBtPhdcMgrInit(void **gash)
{
    CsrBtPhdcMgrInstance *pMgrInstance;

    *gash = CsrPmemZalloc(sizeof(CsrBtPhdcMgrInstance));
    pMgrInstance = *gash;

    CsrBtPhdcMgrInstanceInit(pMgrInstance);   
}


void CsrBtPhdcMgrDeinit(void **gash)
{
    CsrBtPhdcMgrInstance *pMgrInstance = (CsrBtPhdcMgrInstance *)*gash;
    void *prim=NULL;
    CsrUint16 eventClass=0;

    /* Free any message received */
    while(CsrSchedMessageGet(&eventClass, &prim))
    {
        switch(eventClass)
        {
            case CSR_BT_HDP_PRIM:
            {   
                CsrBtHdpFreeUpstreamMessageContents(eventClass, prim);
                break;
            }

            case CSR_BT_PHDC_MGR_PRIM:
            {
                CsrBtPhdcMgrFreeDownstreamMessageContents(eventClass, prim);
                break;
            }

            default:
                break;
        }

        CsrPmemFree(prim);
    }

    CsrPmemFree(pMgrInstance);
    *gash = NULL;
}


void CsrBtPhdcMgrHandler(void **gash)
{
    CsrBtPhdcMgrInstance *pMgrInstance = (CsrBtPhdcMgrInstance *)*gash;
    fsm_event_t event =  PHDC_INVALID_EVENT;
    
    
    if(CsrSchedMessageGet(&pMgrInstance->eventClass,
                          &pMgrInstance->pRecvMsg) )
    {
        PhdcMgrDeviceListEntry *pDevice = NULL;

        switch (pMgrInstance->eventClass)
        {
            case CSR_BT_HDP_PRIM:
            {
                event = CsrBtPhdcMgrHdpIfHandler(pMgrInstance);
                break;
            }
            case CSR_BT_PHDC_MGR_PRIM:
            {
                event = CsrBtPhdcMgrIfHandler(pMgrInstance);
                break;
            }
            default:
                break;
        }

        if( pMgrInstance->state == CSR_BT_PHDC_MGR_ST_ACTIVATED && 
           !( pMgrInstance->eventClass == CSR_BT_HDP_PRIM && 
              ( *(CsrUint16 *)pMgrInstance->pRecvMsg == CSR_BT_HDP_CTRL_GET_CAPAB_IND ||  
                *(CsrUint16 *)pMgrInstance->pRecvMsg == PHDC_MGR_EV_PHDC_GET_DEVICE_CAPAB_REQ || *
                 (CsrUint16 *)pMgrInstance->pRecvMsg == PHDC_MGR_EV_HDP_CTRL_GET_CAPAB_CFM ) ) && 
           (*(CsrUint16 *)pMgrInstance->pRecvMsg != CSR_BT_PHDC_MGR_UNREGISTER_REQ )  &&
           (*(CsrUint16 *)pMgrInstance->pRecvMsg != CSR_BT_HDP_RESUME_IND ) ) 
        {            
            switch (event)
            {
                case PHDC_MGR_DEVICE_EV_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtHdpAcceptConnectAssociateChannelInd*)pMgrInstance->pRecvMsg)->deviceAddr));
                    if (pDevice == NULL )  /* new connection from the device */
                    {
                        pDevice = PHDC_MGR_DEVICE_LIST_ADD_DEVICE(pMgrInstance->deviceList);
                    }
                    break;
                }
                case PHDC_MGR_DEVICE_EV_PHDC_ACCEPT_BT_LINK_RES:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtPhdcMgrAcceptBtLinkRes*)pMgrInstance->pRecvMsg)->deviceAddr));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtHdpConnectAssociateChannelInd*)pMgrInstance->pRecvMsg)->deviceAddr));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_PHDC_SETUP_BT_LINK_REQ: 
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtPhdcMgrSetupBtLinkReq*)pMgrInstance->pRecvMsg)->deviceAddr));
                    if (pDevice == NULL )  /* new connection from the device */
                    {
                        pDevice = PHDC_MGR_DEVICE_LIST_ADD_DEVICE(pMgrInstance->deviceList);
                    }                    
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtHdpConnectAssociateChannelCfm*)pMgrInstance->pRecvMsg)->deviceAddr));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_PHDC_ACCEPT_ASSOCIATE_RES:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pMgrInstance->deviceList, &(((CsrBtPhdcMgrAcceptBtLinkRes*)pMgrInstance->pRecvMsg)->deviceAddr));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(pMgrInstance->deviceList, &(((CsrBtHdpDisconnectAssociateChannelInd*)pMgrInstance->pRecvMsg)->assocChId));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(pMgrInstance->deviceList, &(((CsrBtHdpDisconnectAssociateChannelCfm*)pMgrInstance->pRecvMsg)->assocChId));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_DATA_IND:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(pMgrInstance->deviceList, &(((CsrBtHdpDataInd*)pMgrInstance->pRecvMsg)->assocChId));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_HDP_DATA_CFM:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(pMgrInstance->deviceList, &(((CsrBtHdpDataCfm*)pMgrInstance->pRecvMsg)->assocChId));
                    break;
                }
                case PHDC_MGR_DEVICE_EV_PHDC_DISASSOCIATE_REQ:
                {
                    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(pMgrInstance->deviceList, &(((CsrBtPhdcMgrDisassociateReq*)pMgrInstance->pRecvMsg)->dataChannelId));
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        if(pDevice)
        {
            csrBtPhdcMgrDeviceFsmRun(pMgrInstance, pDevice, event, NULL);
        }
        else
        {
            csrBtPhdcMgrFsmRun(pMgrInstance, event, NULL);
        }

        CsrPmemFree(pMgrInstance->pRecvMsg);
        pMgrInstance->pRecvMsg = NULL;
        pMgrInstance->eventClass = 0;
    }
}

#endif

