#ifndef CSR_APP_MAIN_TRANSPORT_H__
#define CSR_APP_MAIN_TRANSPORT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrResult CsrAppMainTransportInitialise(void);
void CsrAppMainTransportDeinitialise(void);
void CsrAppMainTransportUsage(void);

#ifdef __cplusplus
}
#endif

#endif
