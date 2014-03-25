#ifndef CSR_TIMER_H__
#define CSR_TIMER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c6410.h"
#include "platform/csr_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set the prescalers */
#define TIMER_PRESCALE_0_SET(value)     REG_SET(rTCFG0, 7, 0, value)
#define TIMER_PRESCALE_1_SET(value)     REG_SET(rTCFG0, 15, 8, value)
#define TIMER_PRESCALE_SET(p0, p1)      {rTCFG0 = ((p1) << 8) | (p0);}

/* Set deadtime for timer0 and timer1 */
#define TIMER_DEADZONE_SET(value)       REG_SET(rTCFG0, 23, 16, value)

/* Set MUX for individual timers
    0 = 1/1
    1 = 1/2
    2 = 1/4
    3 = 1/8
    4 = 1/16 */
#define TIMER0_MUX_SET(value)               REG_SET(rTCFG1, 3, 0, value)
#define TIMER1_MUX_SET(value)               REG_SET(rTCFG1, 7, 4, value)
#define TIMER2_MUX_SET(value)               REG_SET(rTCFG1, 11, 8, value)
#define TIMER3_MUX_SET(value)               REG_SET(rTCFG1, 15, 12, value)
#define TIMER4_MUX_SET(value)               REG_SET(rTCFG1, 19, 16, value)
#define TIMER_MUX_SET(t0, t1, t2, t3, t4)   {rTCFG1 = ((t4) << 16) | ((t3) << 12) | ((t2) << 8) | ((t1) << 4) | (t0);}

/* Set DMA mode
    0 = None
    1 = timer0
    2 = timer1
    3 = timer2
    4 = timer3
    5 = timer4 */
#define TIMER_DMAMODE_SET(value)        REG_SET(rTCFG1, 23, 20, value)

/* Common timer control */
#define TIMER_DEADZONE_ON()             BIT_SET(rTCON, 4)
#define TIMER_DEADZONE_OFF()            BIT_CLEAR(rTCON, 4)

/* Timer interrupt control */
#define TIMER_INT_ENABLE(timer)             {rTINT_CSTAT = (rTINT_CSTAT | (1 << (timer))) & 0x1F;}
#define TIMER_INT_ENABLE_ATOMIC(timer)      INT_ATOMIC_EXECUTE(TIMER_INT_ENABLE(timer))
#define TIMER_INT_DISABLE(timer)            {rTINT_CSTAT = (rTINT_CSTAT & ~(1 << (timer))) & 0x1F;}
#define TIMER_INT_DISABLE_ATOMIC(timer)     INT_ATOMIC_EXECUTE(TIMER_INT_DISABLE(timer))
#define TIMER_INT_CLEAR(timer)              {rTINT_CSTAT = (rTINT_CSTAT & 0x1F) | (1 << (5 + (timer)));}
#define TIMER_INT_CLEAR_ATOMIC(timer)       INT_ATOMIC_EXECUTE(TIMER_INT_CLEAR(timer))

/* Timer0 controls */
#define TIMER0_START()                  BIT_SET(rTCON, 0)
#define TIMER0_STOP()                   BIT_CLEAR(rTCON, 0)
#define TIMER0_LOAD(value)              {rTCNTB0 = value;}
#define TIMER0_CMP_LOAD(value)          {rTCMPB0 = value;}
#define TIMER0_READ()                   (rTCNTO0)
#define TIMER0_MANUAL_UPDATE()          BIT_SET(rTCON, 1);BIT_CLEAR(rTCON, 1)
#define TIMER0_INVERT_ON()              BIT_SET(rTCON, 2)
#define TIMER0_INVERT_OFF()             BIT_CLEAR(rTCON, 2)
#define TIMER0_AUTORELOAD_ON()          BIT_SET(rTCON, 3)
#define TIMER0_AUTORELOAD_OFF()         BIT_CLEAR(rTCON, 3)

/* Timer1 controls */
#define TIMER1_START()                  BIT_SET(rTCON, 8)
#define TIMER1_STOP()                   BIT_CLEAR(rTCON, 8)
#define TIMER1_LOAD(value)              {rTCNTB1 = value;}
#define TIMER1_CMP_LOAD(value)          {rTCMPB1 = value;}
#define TIMER1_READ()                   (rTCNTO1)
#define TIMER1_MANUAL_UPDATE()          BIT_SET(rTCON, 9);BIT_CLEAR(rTCON, 9)
#define TIMER1_INVERT_ON()              BIT_SET(rTCON, 10)
#define TIMER1_INVERT_OFF()             BIT_CLEAR(rTCON, 10)
#define TIMER1_AUTORELOAD_ON()          BIT_SET(rTCON, 11)
#define TIMER1_AUTORELOAD_OFF()         BIT_CLEAR(rTCON, 11)

/* Timer2 controls */
#define TIMER2_START()                  BIT_SET(rTCON, 12)
#define TIMER2_STOP()                   BIT_CLEAR(rTCON, 12)
#define TIMER2_LOAD(value)              {rTCNTB2 = value;}
#define TIMER2_CMP_LOAD(value)          {rTCMPB2 = value;}
#define TIMER2_READ()                   (rTCNTO2)
#define TIMER2_MANUAL_UPDATE()          BIT_SET(rTCON, 13);BIT_CLEAR(rTCON, 13)
#define TIMER2_AUTORELOAD_ON()          BIT_SET(rTCON, 15)
#define TIMER2_AUTORELOAD_OFF()         BIT_CLEAR(rTCON, 15)

/* Timer3 controls */
#define TIMER3_START()                  BIT_SET(rTCON, 16)
#define TIMER3_STOP()                   BIT_CLEAR(rTCON, 16)
#define TIMER3_LOAD(value)              {rTCNTB3 = value;}
#define TIMER3_CMP_LOAD(value)          {rTCMPB3 = value;}
#define TIMER3_READ()                   (rTCNTO3)
#define TIMER3_MANUAL_UPDATE()          BIT_SET(rTCON, 17);BIT_CLEAR(rTCON, 17)
#define TIMER3_AUTORELOAD_ON()          BIT_SET(rTCON, 19)
#define TIMER3_AUTORELOAD_OFF()         BIT_CLEAR(rTCON, 19)

/* Timer4 controls */
#define TIMER4_START()                  BIT_SET(rTCON, 20)
#define TIMER4_STOP()                   BIT_CLEAR(rTCON, 20)
#define TIMER4_LOAD(value)              {rTCNTB4 = value;}
#define TIMER4_READ()                   (rTCNTO4)
#define TIMER4_MANUAL_UPDATE()          BIT_SET(rTCON, 21);BIT_CLEAR(rTCON, 21)
#define TIMER4_AUTORELOAD_ON()          BIT_SET(rTCON, 22)
#define TIMER4_AUTORELOAD_OFF()         BIT_CLEAR(rTCON, 22)


/*******************************************************************************

    NAME
        GetTime

    DESCRIPTION
        GetTime returns 32bit high and low part timer values. The value returned
        is monotonically increasing at a rate of 1 per microsecond. The funtion
        returns the low part, and if non-NULL the contents of output parameter
        highPart will be set to the high part.

        It is possible to use GetTime before starting Nucleus, but the range
        will be limited by the current range setting of Timer0 plus one (default
        is TCNTB0=9999us). Furthermore, the value returned may be offset by a
        value equal to TCNTB0 + 1.

        Note: When using Nucleus GetTime will automatically adjust itself to the
        period of Timer0 (determined by the TCNTB0 register), but the counting
        frequency must be 1MHz as set by the Bootloader.

    PARAMETERS
        highPart - if non-NULL the most significant 32bit of the timer value
                   (high part) will be written to the variable pointed at by
                   the argument.

    RETURNS
        The least significant 32bit of the timer value (low part).

*******************************************************************************/
CsrUint32 GetTime(CsrUint32 *highPart);

#ifdef __cplusplus
}
#endif

#endif
