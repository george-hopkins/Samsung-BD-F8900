#ifndef CSR_BT_SC_UI_H__
#define CSR_BT_SC_UI_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScStartUi(ScInstanceData_t * scData);
void CsrBtScSdReadDeviceInfoCfmUiS(ScInstanceData_t * scData);
void CsrBtScCmReadRemoteNameCfmUiS(ScInstanceData_t * scData);
void CsrBtScCancelUiS(ScInstanceData_t * scData);
void CsrBtScForwardPendingInd(ScInstanceData_t * scData, CsrBtDeviceName friendlyName);
#define CsrBtScSetCod(_scData, _cod) \
    {\
        if ((_scData)->cod == 0)\
        {\
            (_scData)->cod = (CsrBtClassOfDevice)(_cod);\
        }\
    }

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_UI_H__ */

