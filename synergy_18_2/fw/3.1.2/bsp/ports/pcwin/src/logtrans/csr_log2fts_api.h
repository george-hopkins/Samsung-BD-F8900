#ifndef CSR_LOG2FTS_API_H__
#define CSR_LOG2FTS_API_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    eOpenSetIoDlg,
    eOpenHwDlg,
    eLiveModeStarted,
    eLiveModeStopped,
    eAppClosed,
    eStartCaptureToBuf,
    eStartCaptureToFile,
    ePauseResumeCapture,
    eClearBuffer,
    eCloseFile,
    eResync,
    eReset,
    eCaptureStarted,
    eCaptureStopped,
    eCapturePaused,
    eCaptureResumed,
    eCaptureSuspended,
    eCaptureUnsuspended,
    eCanClose,
    eOkToClose,
    eNotOkToClose,
    eOpenSetupDlg,
    eCloseSetIoDlg,
    eCloseHwDlg,
    eCancelHwDlg,
    eCloseSetupDlg,
    eCancelSetupDlg,
    eOpenXmtDlg,
    eCloseXmtDlg,
    eCancelXmtDlg,
    eXmitOkToClose,
    eXmitNotOkToClose,
    eCloseXmitDlg,
    eOpenXmitDlg,
    eCanTransmit,
    eCannotTransmit
} eNotificationTypes;

typedef void (*NotificationType)(void *pThis);

/* API function pointer definitions */
typedef HRESULT (GetDllVersion)(TCHAR **pszDllVersion, int *piSize);
typedef HRESULT (InitializeLiveImport)(const TCHAR *szMemoryName, const TCHAR *szConfiguration, CsrBool *pboolSuccess);
typedef HRESULT (ReleaseLiveImport)(void);
typedef HRESULT (StillAlive)(CsrBool *pboolIsAppAlive);
typedef HRESULT (IsAppReady)(CsrBool *pboolIsAppReady);

typedef HRESULT (SendFrame)(
    int iOriginalLength,                    /* The "real" length of a frame. Some frames may be truncated, so this may not be the same as included length */
    int iIncludedLength,                    /* The size of the data passed in this call */
    const BYTE *pbytFrame,                  /* The actual bytes of the frame */
    int iDrf,                               /* any errors or other data related flag */
    int iStream,                            /* Which side this data comes from */
    __int64 i64Timestamp);

typedef HRESULT (SendEvent)(BYTE bytData, int iDrf, int iStream, __int64 i64Timestamp);
typedef HRESULT (SendControlSignalChange)(int iNdrf, __int64 i64Timestamp);
typedef HRESULT (SendNondataFlagChange)(int iNdrf, __int64 i64Timestamp);
typedef HRESULT (SendBreak)(int iStream, __int64 i64Timestamp);
typedef HRESULT (SendFlowControl)(CsrBool boolFlowControlIsOn, __int64 i64Timestamp);
typedef HRESULT (SendPower)(int iLevel, __int64 i64Timestamp);
typedef HRESULT (SendConnectionStatus)(CsrBool boolIsConnected, __int64 i64Timestamp);
typedef HRESULT (SendConfigurationString)(const TCHAR *szConfiguration);
typedef HRESULT (SendSetIODialogHwnd)(const HWND hwnd);
typedef HRESULT (SendXmitDialogHwnd)(const HWND hwnd);
typedef HRESULT (SendSpecialEvent)(int iStream, int iEventNumber, __int64 i64Timestamp);
typedef HRESULT (SendStartOfFrame)(int iStream, __int64 i64Timestamp);
typedef HRESULT (SendEndOfFrame)(int iStream, __int64 i64Timestamp);
typedef HRESULT (SendAbortedFrame)(int iStream, __int64 i64Timestamp);
typedef HRESULT (SendByteSimple)(BYTE byData, __int64 i64Timestamp);
typedef HRESULT (CheckForMessages)();
typedef HRESULT (GetAppVersionNumber)(TCHAR **pszAppVersionNumber, int *piSize);
typedef HRESULT (GetAppSerialNumber)(TCHAR **pszAppSerialNumber, int *piSize);
typedef HRESULT (GetAppDisplayedConfigurationName)(TCHAR **pszAppConfigName, int *piSize);
typedef HRESULT (GetSerialNumberSectionKeyValuePairs)(TCHAR **pszKeyValuePairs, int *piSize);
typedef HRESULT (RegisterNotification)(eNotificationTypes eType, NotificationType pNotification, void *pThis);
typedef HRESULT (SendNotification)(eNotificationTypes eType);
typedef HRESULT (SendArraySimple)(BYTE *pbytData, int iLength, __int64 i64Timestamp);
typedef HRESULT (SendStringSimple)(TCHAR *szData, __int64 i64Timestamp);
typedef HRESULT (SendNumberOfLostMessages)(const int iNumberOfLostMessages);
typedef HRESULT (UpdateStat)(int iStream, int iStatNumber, __int64 i64IncrementAmount);
typedef HRESULT (FramesLost)(int iFramesLost);
typedef HRESULT (SetExePath)(TCHAR *szServerPath);
typedef HRESULT (SendPostNotify)(UINT uiId, WPARAM wParam, LPARAM lParam);

typedef struct
{
    HMODULE                              hdl;
    GetDllVersion                       *GetDllVersion;
    InitializeLiveImport                *InitializeLiveImport;
    ReleaseLiveImport                   *ReleaseLiveImport;
    StillAlive                          *StillAlive;
    IsAppReady                          *IsAppReady;
    SendFrame                           *SendFrame;
    SendEvent                           *SendEvent;
    SendControlSignalChange             *SendControlSignalChange;
    SendNondataFlagChange               *SendNondataFlagChange;
    SendBreak                           *SendBreak;
    SendFlowControl                     *SendFlowControl;
    SendConnectionStatus                *SendConnectionStatus;
    SendConfigurationString             *SendConfigurationString;
    SendSetIODialogHwnd                 *SendSetIODialogHwnd;
    SendXmitDialogHwnd                  *SendXmitDialogHwnd;
    SendSpecialEvent                    *SendSpecialEvent;
    SendStartOfFrame                    *SendStartOfFrame;
    SendEndOfFrame                      *SendEndOfFrame;
    SendAbortedFrame                    *SendAbortedFrame;
    SendByteSimple                      *SendByteSimple;
    CheckForMessages                    *CheckForMessages;
    GetAppVersionNumber                 *GetAppVersionNumber;
    GetAppSerialNumber                  *GetAppSerialNumber;
    GetAppDisplayedConfigurationName    *GetAppDisplayedConfigurationName;
    GetSerialNumberSectionKeyValuePairs *GetSerialNumberSectionKeyValuePairs;
    RegisterNotification                *RegisterNotification;
    SendNotification                    *SendNotification;
    SendArraySimple                     *SendArraySimple;
    SendStringSimple                    *SendStringSimple;
    SendNumberOfLostMessages            *SendNumberOfLostMessages;
    UpdateStat                          *UpdateStat;
    FramesLost                          *FramesLost;
    SetExePath                          *SetExePath;
    SendPostNotify                      *SendPostNotify;
    CRITICAL_SECTION                     logMutex;
} csrLogTransportFtsPipeTable;

#ifdef __cplusplus
}
#endif

#endif
