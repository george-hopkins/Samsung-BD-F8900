#ifndef CSR_BT_IWU_HANDLER_H__
#define CSR_BT_IWU_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


/*************************************************************************************
                    Header files on which this one depends
************************************************************************************/
#include "csr_types.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_message_queue.h"
#include "csr_bt_at_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define D                         (0)
#define A                         (1)
#define DATA                      (2)
#define AT                        (3)
#define INIT                      (4)
#define NONE                      (5)
#define SEMICOLON                 (1)
#define CONNECT                   (1)
#define NOT_CONNECT               (0)
#define IWU_STATE_BASE            (0)
#define INFINIT_TIME              (0)
#define MAX_QUEUE_SIZE            (50)
#define MODEM_RESET_DELAY         (1000000)
#define MODEM_CHECK_STARTUP_TIME  (3000000)

/*-------------------------------------------------------------*/
/* State definitions */
/*-------------------------------------------------------------*/
typedef enum
{
  modemReady = IWU_STATE_BASE,
  modemNotReady,
  iwuNumOfState
} iwuStates_t;


/*============================================================================*
                       typedefs
 *============================================================================*/

typedef struct
{
    CsrUint8        s0Value;
    CsrUint8        s3Value;
    CsrUint8        s4Value;
    CsrUint8        s5Value;
    CsrUint8        s6Value;
    CsrUint8        s7Value;
    CsrUint8        s8Value;
    CsrUint8        s10Value;
    CsrUint8        xValue;
}settings_t;


typedef struct
{
    CsrUint8                lastSend;
    CsrUint8                bitSetting;
    CsrUint8                connectFlag;
    CsrBtConnId             btConnId;
    CsrUint8                serverChannel;
    settings_t              paramSettings;
    iwuStates_t             state;
    void                    *iwuMsgP;
    CsrUint8                modemBuf[128];
    CsrUint16               bufCount;
    CsrSchedMsgId                timerId;
    CsrBool                 timerFlag;
    CsrMessageQueueType    *iwuDataReqQueue;
    CsrBool                 iwuCfmFlag;
    CsrUint8                queueCounter;
    CsrBool                 waterMarkFlag;
    CsrUint8                count;
    CsrUint8                countN;
    CsrSchedMsgId                startUpTimerId;
    CsrBool                 startUpTimerActive;
    CsrSchedBgint                bgint_rx;
} iwuInstanceData_t;

void iwuSendCommandAtA(iwuInstanceData_t *iwuData);
void iwuSendCommandAtH(iwuInstanceData_t *iwuData);
void iwuSendCommandAthIn(CsrUint16 dummy, void *iwuData);
void iwuSendCommandAtD(iwuInstanceData_t *iwuData);
void iwuSendModemResetReq(iwuInstanceData_t *iwuData);
void iwuSendModemResetReqIn(CsrUint16 dummy, void *iwuData);
void iwuSendAbortReq(iwuInstanceData_t *iwuData);
void iwuSendAtDgDataReq(iwuInstanceData_t *iwuData);
void iwuSendModemDataReq(int theLength, char *theData, CsrUint8 theResult);
void modemDefaultSetupReq(iwuInstanceData_t *iwuData);
void modemReset(iwuInstanceData_t *iwuData);
void modemAnswerOrDialHandler(iwuInstanceData_t *iwuData);
void modemNoSignalHandler(iwuInstanceData_t *iwuData);
void iwuModemStatusReqHandler(iwuInstanceData_t *iwuData);
void deactivateReq(iwuInstanceData_t *iwuData);
void iwuAtDgPortnegIndHandler(iwuInstanceData_t * iwuData);
void releaseReq(iwuInstanceData_t *iwuData);
void iwuAtDgConnectIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgDataCfmHandler(iwuInstanceData_t *iwuData);
void iwuAtDgDataIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgDisconnectIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgAtcXIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgAtcAndFIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgAtcZIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgControlIndHandler(iwuInstanceData_t *iwuData);
void iwuAtDgAtcOIndHandler(void);
void iwuAtDgDeactivateCfmHandler(iwuInstanceData_t *iwuData);
void iwuAtDgDataPathStatusIndHandler(iwuInstanceData_t *iwuData);
void iwuAtResponseCodeHandler(iwuInstanceData_t *iwuData);
void iwuModemDataReq(iwuInstanceData_t *iwuData);
void bufHandler(iwuInstanceData_t *iwuData);
void timeHandler(CsrUint16 m, iwuInstanceData_t *iwuData);
void signalHandler(iwuInstanceData_t *iwuData);
void sendReadySignaltoDemoApp(void);
void checkModemStat(void);
void rxTimerFkt( CsrUint16 unused1, void *unused2);
void rxBgIntFkt(void *arg);
void UartDrvTx(char *dataPtr, CsrUint16 valueLen, CsrUint16 *numSend);
void setUpSerialPort(iwuInstanceData_t *iwuData);
void startUpTimeHandler(CsrUint16 m, iwuInstanceData_t * iwuData);

#ifdef __cplusplus
}
#endif

#endif

