#ifndef _SDP_DBASE_H_
#define _SDP_DBASE_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_dbase.h

DESCRIPTION:       This file contains SDP SDS dbase bc01 declarations.



****************************************************************************/

#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/
/* None */

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
*============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 SDS_DBASE_number_of_records(void)
 *
 *  DESCRIPTION
 *  Calculate the number of static service records.
 *
 *  RETURNS/MODIFIES
 *  total number of static service records
 *
 *---------------------------------------------------------------------------*/
CsrUint8 SDS_DBASE_number_of_records(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint16 SDS_DBASE_size_of_record(CsrUint8 sr_number_count)
 *
 *  DESCRIPTION
 *  Calculate the size of the static service record.
 *
 *  RETURNS/MODIFIES
 *  Size of the static service record
 *
 *---------------------------------------------------------------------------*/
CsrUint16 SDS_DBASE_size_of_record(CsrUint8 sr_number_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool SDS_DBASE_get_sr(CsrUint8 sr_number_count, CsrUint8 *sr_data)
 *
 *  DESCRIPTION
 *  Store the static service record in pointer supplied.
 *
 *  RETURNS/MODIFIES
 *  status of static service record data
 *
 *---------------------------------------------------------------------------*/
CsrBool SDS_DBASE_get_sr(CsrUint8 sr_number_count, CsrUint16 sr_size,
                        CsrUint8 *sr_data);

#ifdef __cplusplus
}
#endif

#endif /* INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT */


#endif

