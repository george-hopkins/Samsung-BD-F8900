#ifndef CSR_TM_BLUECORE_TRANSPORT_DESCRIPTOR_H__
#define CSR_TM_BLUECORE_TRANSPORT_DESCRIPTOR_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Defines the blueCoreTransportDescriptor for different transport */
extern void *CsrBluecoreTransportBcsp;
extern void *CsrBluecoreTransportH4DS;
extern void *CsrBluecoreTransportTypeA;
extern void *CsrBluecoreTransportUsb;

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_TRANSPORT_DESCRIPTOR_H__ */
