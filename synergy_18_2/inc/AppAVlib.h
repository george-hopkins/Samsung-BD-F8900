#ifndef APP_AV_LIB_H__
#define APP_AV_LIB_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_types.h"


#ifdef __cplusplus
extern "C" {
#endif



void CsrAppServerAvConnetInd(BD_ADDR_T * pBdaddr);

void CsrAppServerAvConfigInd(CsrUint16 samplerate, CsrUint8 pcm_channels);

void CsrAppServerAvConfigCfm(CsrUint16 samplerate, CsrUint8 pcm_channels);
	
void CsrAppServerAvActivateCfm(void);

void CsrAppServerAvDeactivateCfm(void);

void CsrAppServerAvStreamDataInd(void *data,CsrUint16 length );

void CsrAppServerCloseInd(void);

void CsrAppServerCloseCfm(CsrBool result);

void CsrAppServerOpenInd( CsrBool ss_filter_support);

void CsrAppServerOpenCfm( CsrBool ss_filter_support);

void CsrAppServerAvConnetCfm( BD_ADDR_T * pBdaddr,    CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier);

void CsrAppServerAvDisconnectInd(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier);

void CsrAppServerAvDisconnectCfm(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier);

void CsrAppServerAvStartInd(void);

void CsrAppServerAvSuspendInd(void);


#ifdef __cplusplus
}
#endif

#endif

