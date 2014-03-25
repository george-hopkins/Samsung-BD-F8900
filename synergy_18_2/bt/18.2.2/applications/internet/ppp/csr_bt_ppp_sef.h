#ifndef CSR_BT_PPP_SEF_H__
#define CSR_BT_PPP_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_ppp_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void HandlePppMessage(PppInstanceData * instanceData);
extern void HandleLslMessage(PppInstanceData * instanceData);

#ifdef __cplusplus
}
#endif

#endif

