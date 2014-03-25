/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_touch.h"
#include "platform/csr_interrupt.h"

#define NUMBER_OF_MEASUREMENTS      8       /* Use a power of 2 for more efficient division */

#define ADC_PRESCALER_ENABLE()      BIT_SET(rADCCON, 14)
#define ADC_PRESCALER_DISABLE()     BIT_CLEAR(rADCCON, 14)
#define ADC_PRESCALER_SET(_value)   REG_SET(rADCCON, 13, 6, _value)
#define ADC_STANDBY_ENABLE()        BIT_SET(rADCCON, 2)
#define ADC_STANDBY_DISABLE()       BIT_CLEAR(rADCCON, 2)
#define ADC_START()                 BIT_SET(rADCCON, 0)
#define ADC_WAIT()                  {while (BIT_CHECK(rADCCON, 0)) {} \
                                     while (!BIT_CHECK(rADCCON, 15)) {}}


#define TOUCH_MODE_WFI_SET()        {rADCTSC = 0xD1; \
                                     BIT_CLEAR(rADCTSC, 3); \
                                     rADCTSC = 0xD3;}
#define TOUCH_MODE_XY_SET()         {rADCTSC = 0x0C;}
#define TOUCH_MODE_X_SET()          {rADCTSC = 0x0A;}
#define TOUCH_MODE_Y_SET()          {rADCTSC = 0x09;}

#define TOUCH_X_GET()               (rADCDAY & 0x3FF)   /* Note that X and Y are mixed up in hardware */
#define TOUCH_Y_GET()               (rADCDAX & 0x3FF)

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

static void touchInterruptServiceRoutine(INT unused)
{
    CsrUint32 pending = INT_SUB_PENDING_GET();

    /* Stylus Down */
    if (BIT_CHECK(pending, INT_SUB_PENDN))
    {
        if (state == STATE_UP)
        {
            state = STATE_DOWN_FIRST;
            TOUCH_MODE_XY_SET();
            ADC_START();
            measurementCount = NUMBER_OF_MEASUREMENTS;
            measurement.x = 0;
            measurement.y = 0;
        }
        INT_SUB_CLEAR(INT_SUB_PENDN);
    }

    /* Conversion Complete */
    if (BIT_CHECK(pending, INT_SUB_ADC))
    {
        CsrInt32 x, y, stylusIsDown;

        x = TOUCH_X_GET();
        y = TOUCH_Y_GET();

        /* Check if measurement indicates no stylus down */
        if ((y < 0x50) || (x < 0x50))
        {
            stylusIsDown = FALSE;
        }
        else
        {
            stylusIsDown = TRUE;
        }

        measurement.x += x;
        measurement.y += y;

        TOUCH_MODE_WFI_SET();

        if (--measurementCount && stylusIsDown)
        {
            TOUCH_MODE_XY_SET();
            ADC_START();
        }
        else
        {
            if ((state == STATE_DOWN_FIRST) || (state == STATE_MEASURE))
            {
                if (!stylusIsDown)
                {
                    TouchPoint point = {0, 0};
                    pCallback(TOUCH_EVENT_PENUP, point);
                    state = STATE_UP;
                }
                else
                {
                    CsrInt32 x, y;

                    x = measurement.x / NUMBER_OF_MEASUREMENTS;
                    y = measurement.y / NUMBER_OF_MEASUREMENTS;

                    lastMeasurement.x = ((transformationMatrix.aNominator * x) + (transformationMatrix.bNominator * y) + transformationMatrix.cNominator) / transformationMatrix.divider;
                    lastMeasurement.y = ((transformationMatrix.dNominator * x) + (transformationMatrix.eNominator * y) + transformationMatrix.fNominator) / transformationMatrix.divider;

                    pCallback((state == STATE_DOWN_FIRST) ? TOUCH_EVENT_PENDOWN : TOUCH_EVENT_MEASUREMENT, lastMeasurement);

                    state = STATE_DOWN;
                }
            }
        }

        INT_SUB_CLEAR(INT_SUB_ADC);
    }

    /* Stylus Up */
    if (BIT_CHECK(pending, INT_SUB_PENUP))
    {
        if (state != STATE_UP)
        {
            TouchPoint point = {0, 0};
            pCallback(TOUCH_EVENT_PENUP, point);
        }
        state = STATE_UP;
        INT_SUB_CLEAR(INT_SUB_PENUP);
    }

    INT_CLEAR(INT_ADC_PENUP_DOWN);
}

void TouchInitialise(TouchCallbackFunction callback)
{
    VOID (*previous)(INT);

    TouchPoint dP[3] = {{100, 100}, {300, 300}, {200, 500}};
    TouchPoint sP[3] = {{100, 100}, {300, 300}, {200, 500}};

    /* Mask and clear */
    INT_SUB_MASK(INT_SUB_PENDN);
    INT_SUB_MASK(INT_SUB_PENUP);
    INT_SUB_MASK(INT_SUB_ADC);
    INT_MASK(INT_ADC_PENUP_DOWN);
    INT_SUB_CLEAR(INT_SUB_PENDN);
    INT_SUB_CLEAR(INT_SUB_PENUP);
    INT_SUB_CLEAR(INT_SUB_ADC);
    INT_CLEAR(INT_ADC_PENUP_DOWN);

    /* Save callback function */
    pCallback = callback;

    if (callback == NULL)
    {
        return;
    }

    /* Reset state */
    state = STATE_UP;

    /* Setup Interrupt Service Routine */
    NU_Register_LISR(INT_ADC_PENUP_DOWN, touchInterruptServiceRoutine,
        &previous);

    /* Load unity matrix into calibration data */
    TouchCalibrate(dP, sP);

    /* Set Delay */
    rADCDLY = (0xFFFF);

    ADC_PRESCALER_SET(255); /* 390625kHz ADC Clock (12.8us conversion time @ 5 cycles) */
    ADC_PRESCALER_ENABLE();
    ADC_STANDBY_DISABLE();

    TOUCH_MODE_WFI_SET(); /* Waiting for Interrupt Mode */

    /* Enable interrupt */
    INT_SUB_UNMASK(INT_SUB_PENDN);
    INT_SUB_UNMASK(INT_SUB_PENUP);
    INT_SUB_UNMASK(INT_SUB_ADC);
    INT_UNMASK(INT_ADC_PENUP_DOWN);
}

void TouchMeasurementStart(void)
{
    INT_MASK(INT_ADC_PENUP_DOWN);

    if (state == STATE_DOWN)
    {
        state = STATE_MEASURE;
        TOUCH_MODE_XY_SET();
        ADC_START();
        measurementCount = NUMBER_OF_MEASUREMENTS;
        measurement.x = 0;
        measurement.y = 0;
    }

    INT_UNMASK(INT_ADC_PENUP_DOWN);
}

/* Read out the position of the touch */
void TouchPointGet(TouchPoint *point)
{
    INT_MASK(INT_ADC_PENUP_DOWN);

    point->x = lastMeasurement.x;
    point->y = lastMeasurement.y;

    INT_UNMASK(INT_ADC_PENUP_DOWN);
}
