#ifndef CSR_TOUCH_H__
#define CSR_TOUCH_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Event types signaled by callback function */
#define TOUCH_EVENT_PENDOWN         0       /* Stylus has made contact with the surface, a valid TouchPoint is contained in callback */
#define TOUCH_EVENT_MEASUREMENT     1       /* Generated in response to a TouchPointGet() function call, a valid TouchPoint is contained in callback */
#define TOUCH_EVENT_PENUP           2       /* Stylus has left the surface, the TouchPoint contained in callback is invalid */

/* Error codes */
#define TOUCH_RESULT_SUCCESS        0
#define TOUCH_RESULT_FAILURE        1

/* Represents a touch point on the surface of the touch screen */
typedef struct
{
    CsrInt32 x;
    CsrInt32 y;
} TouchPoint;

typedef struct
{
    CsrInt32 aNominator;
    CsrInt32 bNominator;
    CsrInt32 cNominator;
    CsrInt32 dNominator;
    CsrInt32 eNominator;
    CsrInt32 fNominator;
    CsrInt32 divider;
} TouchCalibrationData;

/* Callback function definition */
typedef void (*TouchCallbackFunction)(CsrInt32 event, TouchPoint point);

/* Initialise the Touch Screen and set the callback function - will reset the transformation to the unity matrix */
void TouchInitialise(TouchCallbackFunction callback);

/* Calibration */
CsrInt32 TouchCalibrate(TouchPoint displayPoints[3], TouchPoint samplePoints[3]);

/* Retrieve or set the transformation matrix (note that TouchInitialise resets the transformation matrix to the unity matrix) */
void TouchCalibrationDataGet(TouchCalibrationData *data);
void TouchCalibrationDataSet(TouchCalibrationData *data);

/* Call to trigger a TOUCH_EVENT_MEASUREMENT event on a callback.
   This call will be ignored if the stylus is not down. Also note,
   that if the measurement indicates that the stylus is not on the
   surface, the TOUCH_EVENT_MEASUREMENT event will be ommited and
   replaced by a TOUCH_EVENT_PENUP event. Call this function
   periodically to track the movement of the stylus between a
   TOUCH_EVENT_PENDOWN and TOUCH_EVENT_PENUP event. */
void TouchMeasurementStart(void);

/* Retrieve the TouchPoint most recently passed in a TOUCH_EVENT_PENDOWN or TOUCH_EVENT_MEASUREMENT event */
void TouchPointGet(TouchPoint *point);

#ifdef __cplusplus
}
#endif

#endif
