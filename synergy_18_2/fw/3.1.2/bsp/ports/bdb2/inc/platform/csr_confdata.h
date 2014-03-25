#ifndef CSR_CONFDATA_H__
#define CSR_CONFDATA_H__
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

/* Reserved tags */
#define CONFDATA_TAG_MACADDRESS             0
#define CONFDATA_TAG_BTADDRESS              1
#define CONFDATA_TAG_TOUCHCALIB             2
#define CONFDATA_TAG_DEVMODE                3
#define CONFDATA_TAG_TOOLBOXSIZE            4
#define CONFDATA_TAG_APPLICATIONSIZE        5
#define CONFDATA_TAG_APPLICATIONARGUMENT    6
#define CONFDATA_TAG_MACADDRESS_WIRELESS    7
#define CONFDATA_TAG_SCDB_ELEMENT_FORMAT    8
#define CONFDATA_TAG_SCDB_ELEMENT_ARRAY     9
#define CONFDATA_TAG_STORESIZE             10

#define CONFDATA_SIZE_MACADDRESS            6

/* Error Codes */
#define CONFDATA_OK                  0
#define CONFDATA_ERROR_READ         -1
#define CONFDATA_ERROR_WRITE        -2
#define CONFDATA_ERROR_OVERSIZE     -3
#define CONFDATA_ERROR_NOTFOUND     -4

/* Allocate a buffer and read the entire Configuration Data Area into the buffer */
CsrInt32 ConfDataRead(CsrUint8 **data);

/* Discard the buffer allocated by ConfDataRead */
CsrInt32 ConfDataDiscard(CsrUint8 *data);

/* Write the data in the buffer to the Configuration Data Area */
CsrInt32 ConfDataWrite(CsrUint8 *data);

/* Retrieve/assign a value from/to the buffer read by ConfDataRead */
CsrInt32 ConfDataSetValue(CsrUint8 **data, CsrUint16 tag, CsrUint16 length, CsrUint8 *value);
CsrInt32 ConfDataGetValue(CsrUint8 *data, CsrUint16 tag, CsrUint16 *length, CsrUint8 *value);

/* Lightweight configuration data read access */
CsrInt32 ConfDataLightweightGetValue(CsrUint16 tag, CsrUint16 length, CsrUint8 *value);

#ifdef __cplusplus
}
#endif

#endif
