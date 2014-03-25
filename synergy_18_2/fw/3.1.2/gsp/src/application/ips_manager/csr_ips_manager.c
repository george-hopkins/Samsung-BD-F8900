/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_formatted_io.h"

#include "csr_log_text_2.h"

#include "csr_app_prim.h"
#include "csr_app_lib.h"

#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

#include "csr_ips_manager_task.h"

/* Includes needed for IPS */
#include "csr_ips.h"
#include "csr_sched_ips.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrIpsManagerLto);

#define CSR_IPS_MANAGER_UI_KEY_ENABLE                                (0x00)
#define CSR_IPS_MANAGER_UI_KEY_DISABLE                               (0x01)
#define CSR_IPS_MANAGER_UI_KEY_CURRENT_MEASUREMENT                   (0x02)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SYSTEM               (0x03)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_EVERYTHINGELSE       (0x04)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER1           (0x05)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER2           (0x06)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER3           (0x07)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HCI                  (0x08)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BCCMD                (0x09)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FP                   (0x0A)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_VM                   (0x0B)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HQ                   (0x0C)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TM_BLUECORE          (0x0D)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_APP                  (0x0E)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_UI                   (0x0F)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IP                   (0x10)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_AM                   (0x11)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET_MANAGER     (0x12)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BLUECORE_MANAGER     (0x13)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TCP_TRANSCEIVER      (0x14)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER_MANAGER  (0x15)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER          (0x16)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP                 (0x17)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP_FILE            (0x18)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FSAL                 (0x19)
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IPS_MANAGER          (0x1A)
#ifdef DEMO_USE_SDIO
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SDIO_PROBE           (0x1B)
#endif
#ifdef DEMO_USE_ETHERNET_TASK
#define CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET_TASK        (0x1C)
#endif

#define CSR_IPS_MANAGER_MEASURE_INTERVAL_LENGTH              (2000000) /* 2 seconds (in microseconds) */
#define CSR_IPS_MANAGER_MEASURE_INTERVALS                    (60 * 60) /* 2 hours */

#define CSR_IPS_MANAGER_UPDATE_INTERVAL                      (2 * CSR_SCHED_SECOND)

extern CsrSchedQid CSR_HCI_IFACEQUEUE;
extern CsrSchedQid CSR_BCCMD_IFACEQUEUE;
extern CsrSchedQid CSR_FP_IFACEQUEUE;
extern CsrSchedQid CSR_VM_IFACEQUEUE;
extern CsrSchedQid CSR_HQ_IFACEQUEUE;
extern CsrSchedQid CSR_TM_BLUECORE_IFACEQUEUE;
extern CsrSchedQid CSR_APP_IFACEQUEUE;
extern CsrSchedQid CSR_UI_IFACEQUEUE;
extern CsrSchedQid CSR_IP_IFACEQUEUE;
extern CsrSchedQid CSR_AM_IFACEQUEUE;
extern CsrSchedQid CSR_ETHERNET_MANAGER_IFACEQUEUE;
extern CsrSchedQid CSR_BLUECORE_MANAGER_IFACEQUEUE;
extern CsrSchedQid CSR_TCP_TRANSCEIVER_IFACEQUEUE;
extern CsrSchedQid CSR_DHCP_SERVER_MANAGER_IFACEQUEUE;
extern CsrSchedQid CSR_DHCP_SERVER_IFACEQUEUE;
extern CsrSchedQid CSR_TFTP_IFACEQUEUE;
extern CsrSchedQid CSR_TFTP_FILE_IFACEQUEUE;
extern CsrSchedQid CSR_FSAL_IFACEQUEUE;
extern CsrSchedQid CSR_IPS_MANAGER_IFACEQUEUE;
#ifdef DEMO_USE_SDIO
extern CsrSchedQid CSR_SDIO_PROBE_IFACEQUEUE;
#endif
#ifdef DEMO_USE_ETHERNET_TASK
extern CsrSchedQid CSR_ETHERNET_IFACEQUEUE;
#endif

/*
  Arrays of measurements. Each element of a array represents a interval of
  lenght configured in CsrIpsConfigure() which is updated by IPS with the number
  of ticks used. The current interval used by IPS can be found by calling
  CsrIpsCurrentIntervalGet()
*/
static CsrUint32 systemMeasurements[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 everythingelseMeasurements[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 scheduler1[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 scheduler2[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 scheduler3[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskHci[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskBccmd[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskFp[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskVm[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskHq[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskTmBluecore[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskApp[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskUi[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskIp[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskAm[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskEthernetManager[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskBluecoreManager[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskTcpTransceiver[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskDhcpServerManager[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskDhcpServer[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskTftp[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskTftpFile[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskFsal[CSR_IPS_MANAGER_MEASURE_INTERVALS];
static CsrUint32 schedulerTaskIpsManager[CSR_IPS_MANAGER_MEASURE_INTERVALS];
#ifdef DEMO_USE_SDIO
static CsrUint32 schedulerTaskSdioProbe[CSR_IPS_MANAGER_MEASURE_INTERVALS];
#endif
#ifdef DEMO_USE_ETHERNET_TASK
static CsrUint32 schedulerTaskEthernet[CSR_IPS_MANAGER_MEASURE_INTERVALS];
#endif

typedef struct
{
    CsrUieHandle hndlMainMenu;
    CsrUieHandle hndlCurrentMeasurementMenu;
    CsrUieHandle hndlBackEvent;
    CsrUieHandle hndlCurrentMeasurementEvent;
    CsrUieHandle hndlEnableDisableEvent;

    CsrBool uiMenuEnableVisible;
    CsrBool uiMenuDisableVisible;
    CsrBool uiMenuCurrentMeasurementVisible;
    CsrBool uiCurrentMeasurementVisible;

    CsrBool     ipsEnabled;
    CsrSchedTid timer;
} CsrIpsManagerInstanceData;

static void csrIpsManagerResetMeasurements(CsrIpsManagerInstanceData *inst)
{
    CsrMemSet(systemMeasurements, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(everythingelseMeasurements, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(scheduler1, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(scheduler2, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(scheduler3, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskHci, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskBccmd, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskFp, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskVm, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskHq, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskTmBluecore, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskApp, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskUi, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskIp, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskAm, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskEthernetManager, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskBluecoreManager, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskTcpTransceiver, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskDhcpServerManager, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskDhcpServer, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskTftp, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskTftpFile, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskFsal, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
    CsrMemSet(schedulerTaskIpsManager, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
#ifdef DEMO_USE_SDIO
    CsrMemSet(schedulerTaskSdioProbe, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
#endif
#ifdef DEMO_USE_ETHERNET_TASK
    CsrMemSet(schedulerTaskEthernet, 0, CSR_IPS_MANAGER_MEASURE_INTERVALS);
#endif
}

static void csrIpsManagerConfigure(CsrIpsManagerInstanceData *inst)
{
    /*
      IPS needs to be configured as the first step. The parameters are:
      currentInterval: 0, the first interval used for profiling
      maximumInterval: CSR_IPS_MANAGER_MEASURE_INTERVALS, the number of intervals used for profiling
      intervalLength:  CSR_IPS_MANAGER_MEASURE_INTERVAL_LENGTH, the lenght of each interval in microseconds

      In this demo application the lenght of each interval is set to 2 seconds and the number of intervals are 3600
      which gives a profiling time of 2 hours
    */
    CsrIpsConfigure(0, CSR_IPS_MANAGER_MEASURE_INTERVALS, CSR_IPS_MANAGER_MEASURE_INTERVAL_LENGTH);

    /* Measure the system; the time used in the system and by the idle context */
    CsrIpsInsert(CSR_IPS_CONTEXT_SYSTEM, systemMeasurements);
    CsrIpsInsert(CSR_IPS_CONTEXT_EVERYTHINGELSE, everythingelseMeasurements);

    /* Measure scheduler */
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_SYSTEM(0), scheduler1);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_SYSTEM(1), scheduler2);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_SYSTEM(2), scheduler3);

    /* Measure scheduler tasks */
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_HCI_IFACEQUEUE), schedulerTaskHci);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_BCCMD_IFACEQUEUE), schedulerTaskBccmd);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_FP_IFACEQUEUE), schedulerTaskFp);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_VM_IFACEQUEUE), schedulerTaskVm);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_HQ_IFACEQUEUE), schedulerTaskHq);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_TM_BLUECORE_IFACEQUEUE), schedulerTaskTmBluecore);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_APP_IFACEQUEUE), schedulerTaskApp);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_UI_IFACEQUEUE), schedulerTaskUi);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_IP_IFACEQUEUE), schedulerTaskIp);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_AM_IFACEQUEUE), schedulerTaskAm);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_ETHERNET_MANAGER_IFACEQUEUE), schedulerTaskEthernetManager);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_BLUECORE_MANAGER_IFACEQUEUE), schedulerTaskBluecoreManager);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_TCP_TRANSCEIVER_IFACEQUEUE), schedulerTaskTcpTransceiver);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE), schedulerTaskDhcpServerManager);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_DHCP_SERVER_IFACEQUEUE), schedulerTaskDhcpServer);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_TFTP_IFACEQUEUE), schedulerTaskTftp);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_TFTP_FILE_IFACEQUEUE), schedulerTaskTftpFile);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_FSAL_IFACEQUEUE), schedulerTaskFsal);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_IPS_MANAGER_IFACEQUEUE), schedulerTaskIpsManager);
#ifdef DEMO_USE_SDIO
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_SDIO_PROBE_IFACEQUEUE), schedulerTaskSdioProbe);
#endif
#ifdef DEMO_USE_ETHERNET_TASK
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_TASK(CSR_ETHERNET_IFACEQUEUE), schedulerTaskEthernet);
#endif
}

void CsrIpsManagerInit(void **gash)
{
    CsrIpsManagerInstanceData *inst;

    inst = CsrPmemAlloc(sizeof(*inst));
    *gash = inst;

    inst->hndlMainMenu = 0;
    inst->hndlCurrentMeasurementMenu = 0;

    inst->hndlBackEvent = 0;
    inst->hndlCurrentMeasurementEvent = 0;
    inst->hndlEnableDisableEvent = 0;

    inst->uiMenuEnableVisible = FALSE;
    inst->uiMenuDisableVisible = FALSE;
    inst->uiMenuCurrentMeasurementVisible = FALSE;
    inst->uiCurrentMeasurementVisible = FALSE;

    inst->ipsEnabled = FALSE;
    inst->timer = CSR_SCHED_TID_INVALID;

    csrIpsManagerResetMeasurements(inst);

    CsrUiUieCreateReqSend(CSR_IPS_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);

    CSR_LOG_TEXT_REGISTER(&CsrIpsManagerLto, "IPS_MANAGER", 0, NULL);
}

void CsrIpsManagerDeinit(void **gash)
{
    CsrIpsManagerInstanceData *inst;

    inst = (CsrIpsManagerInstanceData *) *gash;

    if (inst->ipsEnabled)
    {
        CsrIpsDisable();
    }

    if (inst->timer != CSR_SCHED_TID_INVALID)
    {
        CsrSchedTimerCancel(inst->timer, NULL, NULL);
        inst->timer = CSR_SCHED_TID_INVALID;
    }

    CsrPmemFree(inst);
}

static void csrIpsManagerUpdateMainMenu(CsrIpsManagerInstanceData *inst)
{
    if (inst->uiMenuEnableVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hndlMainMenu, CSR_IPS_MANAGER_UI_KEY_ENABLE);
        inst->uiMenuEnableVisible = FALSE;
    }

    if (inst->uiMenuDisableVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hndlMainMenu, CSR_IPS_MANAGER_UI_KEY_DISABLE);
        inst->uiMenuDisableVisible = FALSE;
    }

    if (inst->uiMenuCurrentMeasurementVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hndlMainMenu, CSR_IPS_MANAGER_UI_KEY_CURRENT_MEASUREMENT);
        inst->uiMenuCurrentMeasurementVisible = FALSE;
    }

    if (inst->ipsEnabled)
    {
        CsrUiMenuAdditemReqSend(inst->hndlMainMenu,
            CSR_UI_LAST,
            CSR_IPS_MANAGER_UI_KEY_DISABLE,
            CSR_UI_ICON_NONE,
            CsrUtf82Utf16String((CsrUtf8String *) "Disable"),
            NULL,
            inst->hndlEnableDisableEvent,
            CSR_UI_DEFAULTACTION,
            inst->hndlBackEvent,
            CSR_UI_DEFAULTACTION);

        inst->uiMenuDisableVisible = TRUE;
    }
    else
    {
        CsrUiMenuAdditemReqSend(inst->hndlMainMenu,
            CSR_UI_LAST,
            CSR_IPS_MANAGER_UI_KEY_ENABLE,
            CSR_UI_ICON_NONE,
            CsrUtf82Utf16String((CsrUtf8String *) "Enable"),
            NULL,
            inst->hndlEnableDisableEvent,
            CSR_UI_DEFAULTACTION,
            inst->hndlBackEvent,
            CSR_UI_DEFAULTACTION);

        inst->uiMenuEnableVisible = TRUE;
    }

    CsrUiMenuAdditemReqSend(inst->hndlMainMenu,
        CSR_UI_LAST,
        CSR_IPS_MANAGER_UI_KEY_CURRENT_MEASUREMENT,
        CSR_UI_ICON_NONE,
        CsrUtf82Utf16String((CsrUtf8String *) "Current measurements"),
        NULL,
        inst->hndlCurrentMeasurementEvent,
        CSR_UI_DEFAULTACTION,
        inst->hndlBackEvent,
        CSR_UI_DEFAULTACTION);

    inst->uiMenuCurrentMeasurementVisible = TRUE;

    CsrUiUieShowReqSend(inst->hndlMainMenu,
        CSR_IPS_MANAGER_IFACEQUEUE,
        CSR_UI_INPUTMODE_AUTO,
        CSR_UI_LOWESTPRIORITY);
}

/* Returns the measurement in ticks of the previos interval. The previous interval is used as IPS is updating the current interval */
static CsrUint32 csrIpsManagerCurrentConsumedTimeGet(CsrUint32 *measurements, CsrUint32 interval)
{
    return measurements[interval > 0 ? interval - 1 : 0];
}

/* Calculates the number of procent the given measurement has used in the previous interval */
static CsrUint32 csrIpsManagerCurrentUtilisationGet(CsrUint32 *measurements, CsrUint32 interval)
{
    CsrUint32 intervalDuration;

    /* CsrIpsResolutionGet() returns how many nanoseconds each tick represents. intervalDuration is the maximun number of ticks in a interval */
    intervalDuration = (1000 * CSR_IPS_MANAGER_MEASURE_INTERVAL_LENGTH) / CsrIpsResolutionGet();

    return (csrIpsManagerCurrentConsumedTimeGet(measurements, interval) / (intervalDuration / 1000) + 5) / 10;
}

static void csrIpsManagerAddMenuItem(CsrIpsManagerInstanceData *inst, CsrUint32 *measurements, CsrUint32 interval, CsrUtf16String *text, CsrUint16 key)
{
    CsrCharString label[50];

    CsrSnprintf(label, 50, "%3d%% (%10dns)", csrIpsManagerCurrentUtilisationGet(measurements, interval),
        csrIpsManagerCurrentConsumedTimeGet(measurements, interval) * CsrIpsResolutionGet());

    CsrUiMenuAdditemReqSend(inst->hndlCurrentMeasurementMenu,
        CSR_UI_LAST,
        key,
        CSR_UI_ICON_NONE,
        text,
        CsrUtf82Utf16String((CsrUtf8String *) label),
        inst->hndlBackEvent,
        CSR_UI_DEFAULTACTION,
        inst->hndlBackEvent,
        CSR_UI_DEFAULTACTION);
}

static void csrIpsManagerUpdateMenuItem(CsrIpsManagerInstanceData *inst, CsrUint32 *measurements, CsrUint32 interval, CsrUtf16String *text, CsrUint16 key)
{
    CsrCharString label[50];

    CsrSnprintf(label, 50, "%3d%% (%10dns)", csrIpsManagerCurrentUtilisationGet(measurements, interval),
        csrIpsManagerCurrentConsumedTimeGet(measurements, interval) * CsrIpsResolutionGet());

    CsrUiMenuSetitemReqSend(inst->hndlCurrentMeasurementMenu,
        key,
        CSR_UI_ICON_NONE,
        text,
        CsrUtf82Utf16String((CsrUtf8String *) label),
        inst->hndlBackEvent,
        CSR_UI_DEFAULTACTION,
        inst->hndlBackEvent,
        CSR_UI_DEFAULTACTION);
}

static void csrIpsManagerUpdateCurrentMeasurement(CsrUint16 mi, void *mv)
{
    CsrIpsManagerInstanceData *inst = (CsrIpsManagerInstanceData *) mv;

    if (inst->uiCurrentMeasurementVisible)
    {
        /* CsrIpsCurrentIntervalGet() returns the current interval updated by IPS */
        CsrUint32 interval = CsrIpsCurrentIntervalGet();

        csrIpsManagerUpdateMenuItem(inst, systemMeasurements, interval, CsrUtf82Utf16String((CsrUtf8String *) "System"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SYSTEM);
        csrIpsManagerUpdateMenuItem(inst, everythingelseMeasurements, interval, CsrUtf82Utf16String((CsrUtf8String *) "Everythingelse"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_EVERYTHINGELSE);
        csrIpsManagerUpdateMenuItem(inst, scheduler1, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 1"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER1);
        csrIpsManagerUpdateMenuItem(inst, scheduler2, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 2"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER2);
        csrIpsManagerUpdateMenuItem(inst, scheduler3, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 3"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER3);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskHci, interval, CsrUtf82Utf16String((CsrUtf8String *) "HCI"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HCI);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskBccmd, interval, CsrUtf82Utf16String((CsrUtf8String *) "BCCMD"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BCCMD);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskFp, interval, CsrUtf82Utf16String((CsrUtf8String *) "FP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FP);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskVm, interval, CsrUtf82Utf16String((CsrUtf8String *) "VM"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_VM);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskHq, interval, CsrUtf82Utf16String((CsrUtf8String *) "HQ"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HQ);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskTmBluecore, interval, CsrUtf82Utf16String((CsrUtf8String *) "TM Bluecore"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TM_BLUECORE);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskApp, interval, CsrUtf82Utf16String((CsrUtf8String *) "App"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_APP);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskUi, interval, CsrUtf82Utf16String((CsrUtf8String *) "UI"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_UI);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskIp, interval, CsrUtf82Utf16String((CsrUtf8String *) "IP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IP);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskAm, interval, CsrUtf82Utf16String((CsrUtf8String *) "AM"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_AM);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskEthernetManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "Ethernet Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET_MANAGER);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskBluecoreManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "Bluecore Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BLUECORE_MANAGER);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskTcpTransceiver, interval, CsrUtf82Utf16String((CsrUtf8String *) "TCP Transceiver"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TCP_TRANSCEIVER);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskDhcpServerManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER_MANAGER);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskDhcpServer, interval, CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskTftp, interval, CsrUtf82Utf16String((CsrUtf8String *) "TFTP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskTftpFile, interval, CsrUtf82Utf16String((CsrUtf8String *) "TFTP File"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP_FILE);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskFsal, interval, CsrUtf82Utf16String((CsrUtf8String *) "FSAL"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FSAL);
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskIpsManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "IPS Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IPS_MANAGER);
#ifdef DEMO_USE_SDIO
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskSdioProbe, interval, CsrUtf82Utf16String((CsrUtf8String *) "SDIO Probe"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SDIO_PROBE);
#endif
#ifdef DEMO_USE_ETHERNET_TASK
        csrIpsManagerUpdateMenuItem(inst, schedulerTaskEthernet, interval, CsrUtf82Utf16String((CsrUtf8String *) "Ethernet"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET);
#endif

        if (inst->ipsEnabled)
        {
            inst->timer = CsrSchedTimerSet(CSR_IPS_MANAGER_UPDATE_INTERVAL, csrIpsManagerUpdateCurrentMeasurement, 0, inst);
        }
    }
}

void CsrIpsManagerHandler(void **gash)
{
    CsrIpsManagerInstanceData *inst;
    void *message;
    CsrUint16 event;

    inst = (CsrIpsManagerInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_APP_PRIM:
            {
                CsrAppPrim *type;

                type = (CsrAppPrim *) message;

                if (*type == CSR_APP_REGISTER_CFM)
                {
                    /* Ignore */
                }
                else if (*type == CSR_APP_TAKE_CONTROL_IND)
                {
                    csrIpsManagerUpdateMainMenu(inst);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrIpsManagerLto, 0, event, *type);
                }

                break;
            }
            case CSR_UI_PRIM:
            {
                CsrUint16 *type;

                type = (CsrUint16 *) message;

                if (*type == CSR_UI_EVENT_IND)
                {
                    CsrUiEventInd *indication;

                    indication = (CsrUiEventInd *) message;

                    if (indication->event == inst->hndlBackEvent)
                    {
                        inst->uiCurrentMeasurementVisible = FALSE;

                        if (inst->timer != CSR_SCHED_TID_INVALID)
                        {
                            CsrSchedTimerCancel(inst->timer, NULL, NULL);
                            inst->timer = CSR_SCHED_TID_INVALID;
                        }

                        CsrUiUieHideReqSend(indication->displayElement);
                    }
                    else if (indication->event == inst->hndlEnableDisableEvent)
                    {
                        if (inst->ipsEnabled)
                        {
                            /* In order to disable the IPS CsrIpsDisable() must be called */
                            CsrIpsDisable();
                            if (inst->timer != CSR_SCHED_TID_INVALID)
                            {
                                CsrSchedTimerCancel(inst->timer, NULL, NULL);
                                inst->timer = CSR_SCHED_TID_INVALID;
                            }
                        }
                        else
                        {
                            csrIpsManagerResetMeasurements(inst);
                            csrIpsManagerConfigure(inst);

                            /* In order to enable IPS CsrIpsEnable() must be called */
                            CsrIpsEnable();
                        }

                        inst->ipsEnabled = !inst->ipsEnabled;

                        csrIpsManagerUpdateMainMenu(inst);
                    }
                    else if (indication->event == inst->hndlCurrentMeasurementEvent)
                    {
                        inst->uiCurrentMeasurementVisible = TRUE;

                        csrIpsManagerUpdateCurrentMeasurement(0, inst);

                        CsrUiUieShowReqSend(inst->hndlCurrentMeasurementMenu,
                            CSR_IPS_MANAGER_IFACEQUEUE,
                            CSR_UI_INPUTMODE_AUTO,
                            CSR_UI_LOWESTPRIORITY);
                    }
                }
                else if (*type == CSR_UI_UIE_CREATE_CFM)
                {
                    CsrUiUieCreateCfm *confirm;

                    confirm = (CsrUiUieCreateCfm *) message;

                    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
                    {
                        if (!inst->hndlBackEvent)
                        {
                            inst->hndlBackEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->hndlBackEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_IPS_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_IPS_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
                        }
                        else if (!inst->hndlCurrentMeasurementEvent)
                        {
                            inst->hndlCurrentMeasurementEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->hndlCurrentMeasurementEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_IPS_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_IPS_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
                        }
                        else if (!inst->hndlEnableDisableEvent)
                        {
                            inst->hndlEnableDisableEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->hndlEnableDisableEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_IPS_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_IPS_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_MENU);
                        }
                    }
                    else if (confirm->elementType == CSR_UI_UIETYPE_MENU)
                    {
                        if (!inst->hndlMainMenu)
                        {
                            inst->hndlMainMenu = confirm->handle;

                            CsrUiMenuSetReqSend(inst->hndlMainMenu,
                                CsrUtf82Utf16String((CsrUtf8String *) "IPS Manager"),
                                CsrUtf82Utf16String((CsrUtf8String *) "Select"), NULL);

                            CsrUiUieCreateReqSend(CSR_IPS_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
                        }
                        else if (!inst->hndlCurrentMeasurementMenu)
                        {
                            /* CsrIpsCurrentIntervalGet() returns the current interval updated by IPS */
                            CsrUint32 interval = CsrIpsCurrentIntervalGet();

                            inst->hndlCurrentMeasurementMenu = confirm->handle;

                            CsrUiMenuSetReqSend(inst->hndlCurrentMeasurementMenu,
                                CsrUtf82Utf16String((CsrUtf8String *) "Current measurements"),
                                NULL, NULL);

                            csrIpsManagerAddMenuItem(inst, systemMeasurements, interval, CsrUtf82Utf16String((CsrUtf8String *) "System"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SYSTEM);
                            csrIpsManagerAddMenuItem(inst, everythingelseMeasurements, interval, CsrUtf82Utf16String((CsrUtf8String *) "Everythingelse"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_EVERYTHINGELSE);
                            csrIpsManagerAddMenuItem(inst, scheduler1, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 1"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER1);
                            csrIpsManagerAddMenuItem(inst, scheduler2, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 2"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER2);
                            csrIpsManagerAddMenuItem(inst, scheduler3, interval, CsrUtf82Utf16String((CsrUtf8String *) "Scheduler 3"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SCHEDULER3);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskHci, interval, CsrUtf82Utf16String((CsrUtf8String *) "HCI"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HCI);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskBccmd, interval, CsrUtf82Utf16String((CsrUtf8String *) "BCCMD"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BCCMD);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskFp, interval, CsrUtf82Utf16String((CsrUtf8String *) "FP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FP);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskVm, interval, CsrUtf82Utf16String((CsrUtf8String *) "VM"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_VM);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskHq, interval, CsrUtf82Utf16String((CsrUtf8String *) "HQ"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_HQ);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskTmBluecore, interval, CsrUtf82Utf16String((CsrUtf8String *) "TM Bluecore"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TM_BLUECORE);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskApp, interval, CsrUtf82Utf16String((CsrUtf8String *) "App"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_APP);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskUi, interval, CsrUtf82Utf16String((CsrUtf8String *) "UI"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_UI);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskIp, interval, CsrUtf82Utf16String((CsrUtf8String *) "IP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IP);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskAm, interval, CsrUtf82Utf16String((CsrUtf8String *) "AM"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_AM);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskEthernetManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "Ethernet Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET_MANAGER);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskBluecoreManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "Bluecore Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_BLUECORE_MANAGER);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskTcpTransceiver, interval, CsrUtf82Utf16String((CsrUtf8String *) "TCP Transceiver"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TCP_TRANSCEIVER);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskDhcpServerManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER_MANAGER);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskDhcpServer, interval, CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_DHCP_SERVER);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskTftp, interval, CsrUtf82Utf16String((CsrUtf8String *) "TFTP"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskTftpFile, interval, CsrUtf82Utf16String((CsrUtf8String *) "TFTP File"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_TFTP_FILE);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskFsal, interval, CsrUtf82Utf16String((CsrUtf8String *) "FSAL"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_FSAL);
                            csrIpsManagerAddMenuItem(inst, schedulerTaskIpsManager, interval, CsrUtf82Utf16String((CsrUtf8String *) "IPS Manager"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_IPS_MANAGER);
#ifdef DEMO_USE_SDIO
                            csrIpsManagerAddMenuItem(inst, schedulerTaskSdioProbe, interval, CsrUtf82Utf16String((CsrUtf8String *) "SDIO Probe"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_SDIO_PROBE);
#endif
#ifdef DEMO_USE_ETHERNET_TASK
                            csrIpsManagerAddMenuItem(inst, schedulerTaskEthernet, interval, CsrUtf82Utf16String((CsrUtf8String *) "Ethernet"), CSR_IPS_MANAGER_UI_KEY_MENU_MEASUREMENT_ETHERNET);
#endif

                            CsrAppRegisterReqSend(CSR_IPS_MANAGER_IFACEQUEUE,
                                (CsrUtf8String *) CsrStrDup("IPS"),
                                (CsrUtf8String *) CsrStrDup("IPS Manager"));
                        }
                    }
                    else
                    {
                        CSR_LOG_TEXT_ERROR((CsrIpsManagerLto, 0, "Unknown CSR_UI elementType (0x%X)", confirm->elementType));
                    }
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrIpsManagerLto, 0, event, *type);
                }

                break;
            }
        }

        CsrPmemFree(message);
    }
}
