#ifndef CSR_BT_GenericSrv_APP_UI_SEF_H__
#define CSR_BT_GenericSrv_APP_UI_SEF_H__
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #1 $
 ****************************************************************************/
#include "csr_synergy.h"
#include "csr_bt_gatt_demo_generic_server_app.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtGattAppHandleGenericSrvCsrUiRegisterUnregister(CsrBtGenericSrvAppInstData *inst,
                                                         CsrBool registered);
void CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(CsrBtGenericSrvAppInstData *inst,
                                                         CsrBool connected,
                                                         CsrBool success);
void CsrBtGattAppHandleGenericSrvSetMainMenuHeader(CsrBtGenericSrvAppInstData *inst);
    
void CsrBtGattAppHandleGenericSrvCsrUiValueUpdate(CsrBtGenericSrvAppInstData *inst, 
                                                  CsrUint16 handle, 
                                                  CsrUint16 value);
void CsrBtGattAppHandleGenericSrvCsrUiPrim(CsrBtGenericSrvAppInstData *inst);
void CsrBtGattAppAcceptPopup(CsrBtGenericSrvAppInstData *inst, char *dialogHeading, char *dialogText);
void CsrBtGattAppHandleGenericSrvCsrUiMenuUpdate(CsrBtGenericSrvAppInstData *inst, CsrBool indication, CsrBool notify);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_GenericSrv_APP_UI_SEF_H__ */

