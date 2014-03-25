#ifndef CSR_BT_COS_CPP_H__
#define CSR_BT_COS_CPP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ENABLE_32BIT_PRECISION
#define COS_DBL2FIX(a) DBL2FIX(a)
#else
#define COS_DBL2FIX(a) (DBL2FIX(a))
#endif

#define COS_0_PI_BY_16  COS_DBL2FIX( 1.00000000000000)
#define COS_1_PI_BY_16  COS_DBL2FIX( 0.98078528040323)
#define COS_2_PI_BY_16  COS_DBL2FIX( 0.92387953251129)
#define COS_3_PI_BY_16  COS_DBL2FIX( 0.83146961230255)
#define COS_4_PI_BY_16  COS_DBL2FIX( 0.70710678118655)
#define COS_5_PI_BY_16  COS_DBL2FIX( 0.55557023301960)
#define COS_6_PI_BY_16  COS_DBL2FIX( 0.38268343236509)
#define COS_7_PI_BY_16  COS_DBL2FIX( 0.19509032201613)
#define COS_8_PI_BY_16  COS_DBL2FIX( 0.00000000000000)

#define COS16__(s,x) (s COS_ ## x ## _PI_BY_16)
#define COS16_(s,x) COS16__(s,x)
#define COS16(x) COS16_(COSSIGN(x), COSWRAP(x))

#define COSWRAP_0  0
#define COSWRAP_1  1
#define COSWRAP_2  2
#define COSWRAP_3  3
#define COSWRAP_4  4
#define COSWRAP_5  5
#define COSWRAP_6  6
#define COSWRAP_7  7
#define COSWRAP_8  8
#define COSWRAP_9  7
#define COSWRAP_10 6
#define COSWRAP_11 5
#define COSWRAP_12 4
#define COSWRAP_13 3
#define COSWRAP_14 2
#define COSWRAP_15 1
#define COSWRAP_16 0
#define COSWRAP_17 1
#define COSWRAP_18 2
#define COSWRAP_19 3
#define COSWRAP_20 4
#define COSWRAP_21 5
#define COSWRAP_22 6
#define COSWRAP_23 7
#define COSWRAP_24 8
#define COSWRAP_25 7
#define COSWRAP_26 6
#define COSWRAP_27 5
#define COSWRAP_28 4
#define COSWRAP_29 3
#define COSWRAP_30 2
#define COSWRAP_31 1

#define COSWRAP_(x) COSWRAP_ ## x
#define COSWRAP(x) COSWRAP_(x)

#define COSSIGN_0  +
#define COSSIGN_1  +
#define COSSIGN_2  +
#define COSSIGN_3  +
#define COSSIGN_4  +
#define COSSIGN_5  +
#define COSSIGN_6  +
#define COSSIGN_7  +
#define COSSIGN_8  +
#define COSSIGN_9  -
#define COSSIGN_10 -
#define COSSIGN_11 -
#define COSSIGN_12 -
#define COSSIGN_13 -
#define COSSIGN_14 -
#define COSSIGN_15 -
#define COSSIGN_16 -
#define COSSIGN_17 -
#define COSSIGN_18 -
#define COSSIGN_19 -
#define COSSIGN_20 -
#define COSSIGN_21 -
#define COSSIGN_22 -
#define COSSIGN_23 -
#define COSSIGN_24 -
#define COSSIGN_25 +
#define COSSIGN_26 +
#define COSSIGN_27 +
#define COSSIGN_28 +
#define COSSIGN_29 +
#define COSSIGN_30 +
#define COSSIGN_31 +

#define COSSIGN_(x) COSSIGN_ ## x
#define COSSIGN(x) COSSIGN_(x)

#ifdef __cplusplus
}
#endif

#endif /* COS_CPP_H */
