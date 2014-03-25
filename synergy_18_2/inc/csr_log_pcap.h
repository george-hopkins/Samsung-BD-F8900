#ifndef CSR_LOG_PCAP_H__
#define CSR_LOG_PCAP_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_log_formats.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HCI packet types */
#define HCI_H4_TYPE_SYNERGY_V1      0x20
#define HCI_H4_TYPE_SYNERGY_V2      0x21
#define HCI_H4_TYPE_SYNERGY_V3      0x22

CsrLog *CsrLogPcapCreate(void *logHandle);
void CsrLogPcapDestroy(CsrLog *l);

#ifdef __cplusplus
}
#endif

#endif
