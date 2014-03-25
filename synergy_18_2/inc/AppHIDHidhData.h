#ifndef APP_HIDH_DATA_H__
#define APP_HIDH_DATA_H__
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


typedef struct
{
	
    CsrBtDeviceAddr          deviceAddr;         /* address of device connected to */
    CsrBtHidhSdpInfo        *sdpInfo;           /* reference to device's SDP information */
    CsrCharString              *serviceName;       /* SDP service name */
    CsrUint16              descriptorLength;   /* SDP descriptor table length */
    CsrUint8               *descriptor;        /* SDP descriptor */
} CsrAppBtHidhSdpInfoData;



#ifdef __cplusplus
}
#endif

#endif

