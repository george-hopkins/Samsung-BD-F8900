#ifndef CSR_HCI_SCO_H__
#define CSR_HCI_SCO_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_usr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NO_SCO_HANDLE   (255)

typedef void (*CsrHciScoHandlerFuncType)(CsrUint8 *);

typedef struct CsrHciScoHandleStructure
{
    CsrUint16                        scoHandle;
    CsrHciScoHandlerFuncType         scoHandlerFunc;
    struct CsrHciScoHandleStructure *next;
} CsrHciScoHandleStructure;

CsrBool CsrHciRegisterScoHandle(CsrUint16 theScoHandle,
    CsrHciScoHandlerFuncType theFunctionPtr);
void CsrHciSendScoData(CsrUint8 *theData);
void CsrHciDeRegisterScoHandle(CsrUint16 theScoHandle);
void CsrHciInitScoHandle(void);
CsrBool CsrHciLookForScoHandle(const CsrUint8 *theBuf);

#ifdef __cplusplus
}
#endif

#endif
