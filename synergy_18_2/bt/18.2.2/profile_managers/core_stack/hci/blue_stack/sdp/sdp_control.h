#ifndef _SDP_CONTROL_H_
#define _SDP_CONTROL_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_control.h

DESCRIPTION:       This file contains SDP control declarations.



****************************************************************************/

#ifndef BUILD_FOR_HOST
#include "vm_const.h"
#endif

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/

/*------------------------------------------------
     SDC_INLUDE is defined to include SDC
--------------------------------------------------*/
#ifndef EXCLUDE_SDC
#define SDC_INCLUDE
#endif

/*------------------------------------------------
     SDS_INLUDE is defined to include SDS
--------------------------------------------------*/
#ifndef EXCLUDE_SDS
#define SDS_INCLUDE
#endif

/*------------------------------------------------
     SDS_DEGUG_LOOK_AT_REC to debug service records
--------------------------------------------------*/
#ifndef SDS_DEGUG_LOOK_AT_REC
/*
#define SDS_DEGUG_LOOK_AT_REC
*/
#endif

/*------------------------------------------------
 Configuration option header size
--------------------------------------------------*/
#define SIZEOF_MTU_HEADER       0x02
#define SIZEOF_FLUSH_HEADER     0x02
#define SIZEOF_QOS_HEADER       0x02

/*------------------------------------------------
     Service Record Handle Start
--------------------------------------------------*/
#define SERVER_RECORD_HANDLE     0x00000000
#define SR_HANDLE_START          0x00010000


/*------------------------------------------------
 Service Discovery functions and macros
 for big enden data types
--------------------------------------------------*/


extern CsrUint16 sdp_uint16_r(const CsrUint8 *ptr, const CsrUint16 offset);
extern CsrUint32 sdp_uint32_r(const CsrUint8 *ptr, const CsrUint16 offset);
extern void sdp_uint16_w(CsrUint8 *ptr, const CsrUint16 offset, const CsrUint16 data);
extern void sdp_uint32_w(CsrUint8 *ptr, const CsrUint16 offset, const CsrUint32 data);

#ifdef BUILD_FOR_HOST
#define SDP_UINT8_R(ptr, offset) ((ptr)[(offset)])
#else
#define SDP_UINT8_R(ptr, offset) \
    (CsrUint8)vm_const_fetch((const CsrUint16*)(ptr) + (offset))
#endif
#define SDP_UINT16_R(ptr, offset) sdp_uint16_r((const CsrUint8*)(ptr), (offset))
#define SDP_UINT32_R(ptr, offset) sdp_uint32_r((const CsrUint8*)(ptr), (offset))
#define SDP_UINT16_W(ptr, offset, data) sdp_uint16_w((ptr), (offset), (data))
#define SDP_UINT32_W(ptr, offset, data) sdp_uint32_w((ptr), (offset), (data))

/* Utility function to build L2CAP configuration table */
#define SDP_L2CAP_CONFTAB_SIZE 5
extern CsrUint16 *sdp_control_conftab(l2ca_mtu_t mtu);

/*============================================================================*
Public Data Types
*============================================================================*/
/* None */

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
*============================================================================*/

#ifdef __cplusplus
}
#endif

#endif

