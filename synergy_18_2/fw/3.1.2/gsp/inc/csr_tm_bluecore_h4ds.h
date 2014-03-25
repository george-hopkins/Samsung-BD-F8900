#ifndef CSR_TM_BLUECORE_H4DS_H__
#define CSR_TM_BLUECORE_H4DS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CsrTmBlueCoreH4dsInit(void **gash);
void CsrTmBlueCoreRegisterUartHandleH4ds(void *handle);

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_H4DS_H__ */
