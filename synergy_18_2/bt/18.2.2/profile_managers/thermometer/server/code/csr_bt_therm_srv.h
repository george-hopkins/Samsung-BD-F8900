#ifndef CSR_BT_THERM_SRV_H_
#define CSR_BT_THERM_SRV_H_

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

   REVISION:      $Revision: #2 $
 ****************************************************************************/
#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_message_queue.h"
#include "csr_list.h"
#include "csr_bt_therm_srv_prim.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct CsrBtThermSrvClientConfigElementTag 
{
    struct CsrBtThermSrvClientConfigElementTag   *next;              /*must be first */
    struct CsrBtThermSrvClientConfigElementTag   *prev;              /*must be second */
    CsrBtDeviceAddr                               deviceAddr;        /* registered device addr */
    CsrUint8                                     *value;             /* client config value*/
    CsrBool                                       connectionFailed;  /* true if connect on update has been tried without success */
    CsrBool                                       paired;            /* device paired or not */

    } CsrBtThermSrvClientConfigElement;


/* CSR_BT_THERMS instance data */
typedef struct
{
    char                           *profileName;

    CsrSchedQid                     appHandle;
    CsrMessageQueueType            *saveQueue;
    CsrBool                         restoreFlag;
    CsrBool                         initialized;

    CsrSchedQid                     phandle;                  /* proximity client demo apps own phandle                   */
    CsrBtGattId                     gattId;                  /* gattId that this demo app identifies with                 */

    void                           *recvMsgP;
    CsrBtTypedAddr                  selectedDeviceAddr;
    CsrBtTypedAddr                  connectedDeviceAddr;
    CsrBtConnId                     btConnId;
    CsrBtConnId                     connectedBtConnId;
    CsrUint16                       dbStartHandle;
    CsrUint16                       dbEndHandle;
    CsrSchedTid                     update_timer;
    CsrCmnList_t                    clientConfigList;
    CsrUint8                        *tempData;
    CsrUint16                       tempDataSize;
    CsrBtThermSrvEventMask          eventMask;
    CsrUint16                       battValue;
    CsrBtThermSrvBatteryMask        battMask;
    CsrUint8                        serviceRequired;
    CsrBool                         deactivating;
} CsrBtThermSrvInstData;

void CsrBtThermSrvStartRegister(CsrBtThermSrvInstData *thermsData);
void CsrBtThermSrvStartUnregister(CsrBtThermSrvInstData *thermsData);
void CsrBtThermSrvStartActivate(CsrBtThermSrvInstData *thermsData);
void CsrBtThermSrvStartDeactivate(CsrBtThermSrvInstData *thermsData);

void CsrBtThermSrvFreeClientConfigList(CsrCmnListElm_t *elem);
CsrBool CsrBtThermSrvFindClientConfigByAddr(CsrCmnListElm_t *elem, void *value);
void CsrBtThermSrvClientConfigInitList(CsrCmnListElm_t *elem);
#define CSR_BT_THERM_SRV_ADD_CLIENT_CONFIG(_List) \
    (CsrBtThermSrvClientConfigElement *)CsrCmnListElementAddLast(&(_List), \
                                                                 sizeof(CsrBtThermSrvClientConfigElement))

#define CSR_BT_THERM_SRV_FIND_CLIENT_CONFIG_BY_ADDR(_List,_addr) \
    ((CsrBtThermSrvClientConfigElement *)CsrCmnListSearch(&(_List), \
                                                        CsrBtThermSrvFindClientConfigByAddr, \
                                                        (void *)(_addr)))

#define PROFILE_NAME(name) CsrUtf8StrDup((CsrUtf8String *) name)

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_THERM_SRV_H_ */

