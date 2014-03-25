#ifndef CSR_BT_TPT_CMD_SERIALIZE_H__
#define CSR_BT_TPT_CMD_SERIALIZE_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8         *serializeTptCommand(TptCommand_t *cmd, CsrUint16 *len);
TptCommand_t    *deserializeTptCommand(CsrUint8 *buf);

#ifdef __cplusplus
}
#endif

#endif /* !TPT_CMD_SERIALIZE_H__ */
