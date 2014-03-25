#ifndef HDP_DEMO_UTIL_H__
#define HDP_DEMO_UTIL_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_platform.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_sched_init.h"
#include "csr_bt_sc_demo_app.h"


/* The time to wait for bonding and connection in number of microseconds */
#define MAX_ACTIVATE_TIME                 (0)
#define KEYB_CHECK_TIMEOUT                (200000)
#define DELAY_BEFORE_GETTING_INSTANCES    (1000000)
#define MAX_DISCOVERY_RESULTS_IN_LIST     (10)

#define CLIENT                            (TRUE)
#define SERVER                            (FALSE)
#define MAX_FILE_NAME_LEN                 (20)
#define SPP_INSTANCE_UNDEFINED            (0xFF)
#define TIME_WITH_NO_DATA                 (3000000) /* In micro seconds */
#define MAX_NUMBER_OF_HDP_INSTANCES       (2)
#define WRAPAROUND_TIME_SEC               ((double) 0xFFFFFFFF/1000000)

/* upf-32 */
#define MAX_MDEP                          (10)

#define HDP_DEMO_APP_NO_END_POINTS      2
#define HDP_DEMO_APP_SOURCE             CSR_BT_HDP_MDEP_SOURCE
#define HDP_DEMO_APP_SINK               CSR_BT_HDP_MDEP_SINK
#define HDP_DEMO_APP_BOTH_ROLES         0x02
#define HDP_NUM_CONNECTIONS             1


/* Demoapp states */
typedef enum
{
   init,
   idle,
   pairing,
   inquiry,
   selecting,
   registering,
   registered,
   connectable,
   getcapab,
   connecting,
   connected,
   entryNum,
   suspend,
   resume,
   ieee_agent,
   ieee_manager

} CsrBtHdpDemoStates_t;

/* Ieee sub state  */
typedef enum
{
    associate,
    configuring,
    w4config,
    mds,
    mds_recv,
    operating,
    disassociating,
    disassociate,
    pass
}CsrBtHdpDemoIeeeSubStates_t;

typedef struct
{
    CsrUint8                    numberOfHdpInstances;
    void                        *recvMsgP;
    CsrSchedQid                      phandle;
    CsrSchedQid                      sdcAppHandle;

    CsrBool                     doneInquiry;
    CsrBtHdpDemoStates_t        state;
    CsrBtHdpDemoStates_t        prevState;
    CsrBtHdpDemoIeeeSubStates_t ieeeState;
    CsrUint16                   devConfigId;
    CsrUint8                    prevSelection;

    CsrUint8                    role;

    CsrUint8                    numInList;
    CsrBtDeviceAddr             bdLocal;
    CsrUint8                    selectedRemoteDeviceIdx;
    CsrBtDeviceAddr             bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];

    CsrBtHdpDemoStates_t        keyState;
    CsrUint32                   instanceId;
    CsrUint8                    idxAssocChId;
    CsrUint32                   assocChId[MAX_MDEP];
    CsrUint32                   virtualChId;
    CsrBtMdepId                 mdepId[MAX_MDEP];
    CsrBtMdepDataType           mdepDataType[MAX_MDEP];
    CsrBtMdepRole               mdepRole[MAX_MDEP];
    ScAppInstData_t             scInstData;
} hdpDemoGlobalInstanceData_t;


void  hdpdemoSendAarqApdu(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoSendAareApdu(hdpDemoGlobalInstanceData_t *hdpData,CsrBool result );
void  hdpdemoSendApduConfig(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoSendApduConfigRsp(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoSendDataMDSGet(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoSendDataMDSAttrib(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoDataMeasurement(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoDataMeasurementCfm(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoRlrqApdu(hdpDemoGlobalInstanceData_t *hdpData);
void  hdpdemoRlreApdu(hdpDemoGlobalInstanceData_t *hdpData);



#ifdef __cplusplus
}
#endif

#endif


