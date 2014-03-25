#ifndef CSR_BT_MOD_MATH_CPP_H__
#define CSR_BT_MOD_MATH_CPP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INC_9 10
#define INC_10 11
#define INC_11 12
#define INC_12 13
#define INC_13 14
#define INC_14 15
#define INC_15 16
#define INC_16 17
#define INC_17 18
#define INC_18 19
#define INC_19 20
#define INC_20 21
#define INC_21 22
#define INC_22 23
#define INC_23 24
#define INC_24 25
#define INC_25 26
#define INC_26 27
#define INC_27 28
#define INC_28 29
#define INC_29 30
#define INC_30 31
#define INC_31 0

#define INC_(x) INC_ ## x
#define INC(x) INC_(x)

#define PLUS_0(x)  x
#define PLUS_1(x)  INC(x)
#define PLUS_2(x)  PLUS_1(INC(x))
#define PLUS_3(x)  PLUS_2(INC(x))
#define PLUS_4(x)  PLUS_3(INC(x))
#define PLUS_5(x)  PLUS_4(INC(x))
#define PLUS_6(x)  PLUS_5(INC(x))
#define PLUS_7(x)  PLUS_6(INC(x))
#define PLUS_8(x)  PLUS_7(INC(x))
#define PLUS_9(x)  PLUS_8(INC(x))
#define PLUS_10(x) PLUS_9(INC(x))
#define PLUS_11(x) PLUS_10(INC(x))
#define PLUS_12(x) PLUS_11(INC(x))
#define PLUS_13(x) PLUS_12(INC(x))
#define PLUS_14(x) PLUS_13(INC(x))
#define PLUS_15(x) PLUS_14(INC(x))
#define PLUS_16(x) PLUS_15(INC(x))
#define PLUS_17(x) PLUS_16(INC(x))
#define PLUS_18(x) PLUS_17(INC(x))
#define PLUS_19(x) PLUS_18(INC(x))
#define PLUS_20(x) PLUS_19(INC(x))
#define PLUS_21(x) PLUS_20(INC(x))
#define PLUS_22(x) PLUS_21(INC(x))
#define PLUS_23(x) PLUS_22(INC(x))
#define PLUS_24(x) PLUS_23(INC(x))
#define PLUS_25(x) PLUS_24(INC(x))
#define PLUS_26(x) PLUS_25(INC(x))
#define PLUS_27(x) PLUS_26(INC(x))
#define PLUS_28(x) PLUS_27(INC(x))
#define PLUS_29(x) PLUS_28(INC(x))
#define PLUS_30(x) PLUS_29(INC(x))
#define PLUS_31(x) PLUS_30(INC(x))

#define PLUS_(x,y) PLUS_ ## y(x)
#define PLUS(x,y) PLUS_(x,y)

#define NEG_0 0
#define NEG_1 31
#define NEG_2 30
#define NEG_3 29
#define NEG_4 28
#define NEG_5 27
#define NEG_6 26
#define NEG_7 25
#define NEG_8 24
#define NEG_9 23
#define NEG_10 22
#define NEG_11 21
#define NEG_12 20
#define NEG_13 19
#define NEG_14 18
#define NEG_15 17
#define NEG_16 16
#define NEG_17 15
#define NEG_18 14
#define NEG_19 13
#define NEG_20 12
#define NEG_21 11
#define NEG_22 10
#define NEG_23 9
#define NEG_24 8
#define NEG_25 7
#define NEG_26 6
#define NEG_27 5
#define NEG_28 4
#define NEG_29 3
#define NEG_30 2
#define NEG_31 1

#define NEG_(x) NEG_ ## x
#define NEG(x) NEG_(x)

#define SUB(x,y) PLUS(x,NEG(y))

#define MUL_0(x)  0
#define MUL_1(x)  x
#define MUL_2(x)  PLUS(MUL_1(x),x)
#define MUL_3(x)  PLUS(MUL_2(x),x)
#define MUL_4(x)  PLUS(MUL_3(x),x)
#define MUL_5(x)  PLUS(MUL_4(x),x)
#define MUL_6(x)  PLUS(MUL_5(x),x)
#define MUL_7(x)  PLUS(MUL_6(x),x)
#define MUL_8(x)  PLUS(MUL_7(x),x)
#define MUL_9(x)  PLUS(MUL_8(x),x)
#define MUL_10(x) PLUS(MUL_9(x),x)
#define MUL_11(x) PLUS(MUL_10(x),x)
#define MUL_12(x) PLUS(MUL_11(x),x)
#define MUL_13(x) PLUS(MUL_12(x),x)
#define MUL_14(x) PLUS(MUL_13(x),x)
#define MUL_15(x) PLUS(MUL_14(x),x)
#define MUL_16(x) PLUS(MUL_15(x),x)
#define MUL_17(x) PLUS(MUL_16(x),x)
#define MUL_18(x) PLUS(MUL_17(x),x)
#define MUL_19(x) PLUS(MUL_18(x),x)
#define MUL_20(x) PLUS(MUL_19(x),x)
#define MUL_21(x) PLUS(MUL_20(x),x)
#define MUL_22(x) PLUS(MUL_21(x),x)
#define MUL_23(x) PLUS(MUL_22(x),x)
#define MUL_24(x) PLUS(MUL_23(x),x)
#define MUL_25(x) PLUS(MUL_24(x),x)
#define MUL_26(x) PLUS(MUL_25(x),x)
#define MUL_27(x) PLUS(MUL_26(x),x)
#define MUL_28(x) PLUS(MUL_27(x),x)
#define MUL_29(x) PLUS(MUL_28(x),x)
#define MUL_30(x) PLUS(MUL_29(x),x)
#define MUL_31(x) PLUS(MUL_30(x),x)

#define MUL_(x,y) MUL_ ## y(x)
#define MUL(x,y) MUL_(x,y)

#ifdef __cplusplus
}
#endif

#endif /* MOD_MATH_CPP_H */
