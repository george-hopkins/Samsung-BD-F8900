#ifndef CSR_TM_BLUECORE_TASK_H__
#define CSR_TM_BLUECORE_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_sched.h"
#include "csr_tm_bluecore_bcsp.h"
#include "csr_tm_bluecore_h4ds.h"
#include "csr_tm_bluecore_usb.h"
#include "csr_tm_bluecore_type_a.h"
#include "csr_tm_bluecore_hci_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tm queue definitions */
extern CsrSchedQid CSR_TM_BLUECORE_IFACEQUEUE;

/* CSR_TM_BCSP */
void CsrTmBlueCoreHandler(void **gash);
void CsrTmBlueCoreDeinit(void **gash);

#define CSR_TM_BLUECORE_BCSP_INIT   CsrTmBlueCoreBcspInit
#define CSR_TM_BLUECORE_H4DS_INIT   CsrTmBlueCoreH4dsInit
#define CSR_TM_BLUECORE_H4I_INIT    CsrTmBlueCoreH4iInit
#define CSR_TM_BLUECORE_USB_INIT    CsrTmBlueCoreUsbInit
#define CSR_TM_BLUECORE_TYPE_A_INIT CsrTmBlueCoreTypeAInit
#define CSR_TM_BLUECORE_HCI_SOCKET_INIT  CsrTmBlueCoreHciSocketInit

#define CSR_TM_BLUECORE_HANDLER     CsrTmBlueCoreHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_TM_BLUECORE_DEINIT  CsrTmBlueCoreDeinit
#else
#define CSR_TM_BLUECORE_DEINIT  NULL
#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_TASK_H__ */
