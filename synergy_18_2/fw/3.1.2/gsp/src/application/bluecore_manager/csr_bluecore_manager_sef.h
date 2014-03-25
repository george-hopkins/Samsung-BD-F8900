#ifndef CSR_BLUECORE_MANAGER_SEF_H__
#define CSR_BLUECORE_MANAGER_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_bluecore_manager_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBluecoreManagerTmBluecoreActivateTransportCfmHandler(CsrBluecoreManagerInstanceData *instanceData);
void CsrBluecoreManagerTmBluecoreDeactivateTransportCfmHandler(CsrBluecoreManagerInstanceData *instanceData);
void CsrBluecoreManagerTmBluecoreDeactivateTransportIndHandler(CsrBluecoreManagerInstanceData *instanceData);

void CsrBluecoreManagerAppRegisterCfmHandler(CsrBluecoreManagerInstanceData *instanceData);
void CsrBluecoreManagerAppTakeControlIndHandler(CsrBluecoreManagerInstanceData *instanceData);

void CsrBluecoreManagerUiUieCreateCfmHandler(CsrBluecoreManagerInstanceData *instanceData);
void CsrBluecoreManagerUiEventIndHandler(CsrBluecoreManagerInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
