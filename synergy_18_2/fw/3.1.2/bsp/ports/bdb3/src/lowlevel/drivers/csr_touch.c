/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"

#include "platform/csr_s3c6410.h"
#include "platform/csr_touch.h"
#include "platform/csr_interrupt.h"

#define NUMBER_OF_MEASUREMENTS      8       /* Use a power of 2 for more efficient division */

#define ADC_PRESCALER_ENABLE()      BIT_SET(rADCCON, 14)
#define ADC_PRESCALER_DISABLE()     BIT_CLEAR(rADCCON, 14)
#define ADC_PRESCALER_SET(value)    REG_SET(rADCCON, 13, 6, value)
#define ADC_STANDBY_ENABLE()        BIT_SET(rADCCON, 2)
#define ADC_STANDBY_DISABLE()       BIT_CLEAR(rADCCON, 2)
#define ADC_START()                 BIT_SET(rADCCON, 0)
#define ADC_WAIT()                  {while (BIT_CHECK(rADCCON, 0)) {} \
                                     while (!BIT_CHECK(rADCCON, 15)) {}}


#define TOUCH_MODE_WFI_SET()        {REG_SET(rADCTSC, 7, 0, 0xD1); \
                                     BIT_CLEAR(rADCTSC, 3); \
                                     REG_SET(rADCTSC, 7, 0, 0xD3);}
#define TOUCH_MODE_XY_SET()         REG_SET(rADCTSC, 7, 0, 0x0C)
#define TOUCH_MODE_X_SET()          REG_SET(rADCTSC, 7, 0, 0x0A)
#define TOUCH_MODE_Y_SET()          REG_SET(rADCTSC, 7, 0, 0x09)

#define TOUCH_DETECT_UP()           BIT_SET(rADCTSC, 8)
#define TOUCH_DETECT_DOWN()         BIT_CLEAR(rADCTSC, 8)
#define TOUCH_DETECT_TOGGLE()       BIT_TOGGLE(rADCTSC, 8)

#define TOUCH_X_GET()               (rADCDAT0 & 0x3FF)
#define TOUCH_Y_GET()               (rADCDAT1 & 0x3FF)

#define STYLUS_IS_DOWN()            (!BIT_CHECK(rADCDAT0, 15))

#define STATE_UP                0
#define STATE_DOWN_FIRST        2
#define STATE_DOWN              3
#define STATE_MEASURE           4

static CsrUint8 state;

static CsrInt32 measurementCount;
static TouchPoint measurement;
static TouchPoint lastMeasurement;

/* Callback function for notifying of touch events */
static TouchCallbackFunction pCallback;

/* Calibration Data */
static TouchCalibrationData transformationMatrix;

void TouchCalibrationDataGet(TouchCalibrationData *data)
{
    *data = transformationMatrix;
}

void TouchCalibrationDataSet(TouchCalibrationData *data)
{
    transformationMatrix = *data;
}

/* Calibrate using reference measurements
    Return Values:
    0 = Calibration Successful
    1 = Calibration Failed
*/
CsrInt32 TouchCalibrate(TouchPoint displayPoints[3], TouchPoint samplePoints[3])
{
    transformationMatrix.divider = ((samplePoints[0].x - samplePoints[2].x) * (samplePoints[1].y - samplePoints[2].y)) - ((samplePoints[1].x - samplePoints[2].x) * (samplePoints[0].y - samplePoints[2].y));
    if (!transformationMatrix.divider)
    {
        return 1;
    }
    transformationMatrix.aNominator = ((displayPoints[0].x - displayPoints[2].x) * (samplePoints[1].y - samplePoints[2].y)) - ((displayPoints[1].x - displayPoints[2].x) * (samplePoints[0].y - samplePoints[2].y));
    transformationMatrix.bNominator = ((samplePoints[0].x - samplePoints[2].x) * (displayPoints[1].x - displayPoints[2].x)) - ((displayPoints[0].x - displayPoints[2].x) * (samplePoints[1].x - samplePoints[2].x));
    transformationMatrix.cNominator = (samplePoints[2].x * displayPoints[1].x - samplePoints[1].x * displayPoints[2].x) * samplePoints[0].y + (samplePoints[0].x * displayPoints[2].x - samplePoints[2].x * displayPoints[0].x) * samplePoints[1].y + (samplePoints[1].x * displayPoints[0].x - samplePoints[0].x * displayPoints[1].x) * samplePoints[2].y;
    transformationMatrix.dNominator = ((displayPoints[0].y - displayPoints[2].y) * (samplePoints[1].y - samplePoints[2].y)) - ((displayPoints[1].y - displayPoints[2].y) * (samplePoints[0].y - samplePoints[2].y));
    transformationMatrix.eNominator = ((samplePoints[0].x - samplePoints[2].x) * (displayPoints[1].y - displayPoints[2].y)) - ((displayPoints[0].y - displayPoints[2].y) * (samplePoints[1].x - samplePoints[2].x));
    transformationMatrix.fNominator = (samplePoints[2].x * displayPoints[1].y - samplePoints[1].x * displayPoints[2].y) * samplePoints[0].y + (samplePoints[0].x * displayPoints[2].y - samplePoints[2].x * displayPoints[0].y) * samplePoints[1].y + (samplePoints[1].x * displayPoints[0].y - samplePoints[0].x * displayPoints[1].y) * samplePoints[2].y;
    return 0;
}

static VOID isrAdc(INT unused)
{
    rADCCLRINT = 1;

    measurement.x += TOUCH_X_GET();
    measurement.y += TOUCH_Y_GET();

    if ((state == STATE_DOWN_FIRST) || (state == STATE_MEASURE))
    {
        if (--measurementCount)
        {
            TOUCH_MODE_XY_SET();
            ADC_START();
        }
        else
        {
            CsrInt32 x, y;

            TOUCH_MODE_WFI_SET();

            x = measurement.x / NUMBER_OF_MEASUREMENTS;
            y = measurement.y / NUMBER_OF_MEASUREMENTS;

            lastMeasurement.x = ((transformationMatrix.aNominator * x) + (transformationMatrix.bNominator * y) + transformationMatrix.cNominator) / transformationMatrix.divider;
            lastMeasurement.y = ((transformationMatrix.dNominator * x) + (transformationMatrix.eNominator * y) + transformationMatrix.fNominator) / transformationMatrix.divider;

            pCallback((state == STATE_DOWN_FIRST) ? TOUCH_EVENT_PENDOWN : TOUCH_EVENT_MEASUREMENT, lastMeasurement);

            state = STATE_DOWN;
        }
    }

    INT_CLEAR();
}

static VOID isrPenDnUp(INT unused)
{
    rADCCLRINTPNDNUP = 1;

    TOUCH_DETECT_TOGGLE();

    if (STYLUS_IS_DOWN())
    {
        /* Stylus Down */
        if (state == STATE_UP)
        {
            state = STATE_DOWN_FIRST;
            TOUCH_MODE_XY_SET();
            ADC_START();
            measurementCount = NUMBER_OF_MEASUREMENTS;
            measurement.x = 0;
            measurement.y = 0;
        }
        else
        {
            TOUCH_DETECT_UP();
        }
    }
    else
    {
        /* Stylus Up */
        if (state != STATE_UP)
        {
            TouchPoint point = {0, 0};
            state = STATE_UP;
            pCallback(TOUCH_EVENT_PENUP, point);
        }
        else
        {
            TOUCH_DETECT_DOWN();
        }
    }

    INT_CLEAR();
}

void TouchInitialise(TouchCallbackFunction callback)
{
    VOID (*previous)(INT);

    TouchPoint dP[3] = {{100, 100}, {300, 300}, {200, 500}};
    TouchPoint sP[3] = {{100, 100}, {300, 300}, {200, 500}};

    /* Mask and clear */
    INT_MASK(INT_ADC);
    INT_MASK(INT_PENDNUP);
    rADCCLRINT = 1;
    rADCCLRINTPNDNUP = 1;

    /* Save callback function */
    pCallback = callback;

    if (callback == NULL)
    {
        return;
    }

    /* Reset state */
    state = STATE_UP;

    /* Setup Interrupt Service Routines */
    NU_Register_LISR(INT_ADC, isrAdc, &previous);
    NU_Register_LISR(INT_PENDNUP, isrPenDnUp, &previous);

    /* Load unity matrix into calibration data */
    TouchCalibrate(dP, sP);

    /* Set Delay */
    rADCDLY = (0xFFFF);

    ADC_PRESCALER_SET(192); /* 343750kHz ADC Clock (14.5us conversion time @ 5 cycles) */
    ADC_PRESCALER_ENABLE();
    ADC_STANDBY_DISABLE();

    TOUCH_DETECT_DOWN(); /* Detect pen down */
    TOUCH_MODE_WFI_SET(); /* Waiting for Interrupt Mode */

    /* Enable interrupt */
    INT_UNMASK(INT_ADC);
    INT_UNMASK(INT_PENDNUP);
}

void TouchMeasurementStart(void)
{
    INT_ATOMIC_EXECUTE_HEAD();

    if (state == STATE_DOWN)
    {
        state = STATE_MEASURE;
        TOUCH_MODE_XY_SET();
        ADC_START();
        measurementCount = NUMBER_OF_MEASUREMENTS;
        measurement.x = 0;
        measurement.y = 0;
    }

    INT_ATOMIC_EXECUTE_TAIL();
}

/* Read out the position of the touch */
void TouchPointGet(TouchPoint *point)
{
    INT_ATOMIC_EXECUTE_HEAD();

    point->x = lastMeasurement.x;
    point->y = lastMeasurement.y;

    INT_ATOMIC_EXECUTE_TAIL();
}
