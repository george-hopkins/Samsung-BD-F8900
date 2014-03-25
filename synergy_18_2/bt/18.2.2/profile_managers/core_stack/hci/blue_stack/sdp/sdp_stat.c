/****************************************************************************

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sds.c

DESCRIPTION:       SDP database.



****************************************************************************/

#include "csr_synergy.h"

/*============================================================================*
ANSI C & System-wide Header Files
*============================================================================*/
#include "csr_util.h"

#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT

#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "error.h" /* path removed */
/*============================================================================*
Interface Header Files
*============================================================================*/
#include "sdp_control.h" /* path removed */
#include "sdp_dbase.h" /* path removed */
#include "sdp_stat_private.h" /* path removed */
/*============================================================================*
Public Data
*============================================================================*/
/* None */
/*============================================================================*
Private Defines
*============================================================================*/
/* Size in bytes for the service record size parameter */
#define SR_SIZE    0x02
/*============================================================================*
Private Data Types
*============================================================================*/
/* None */
/*============================================================================*
Private Function Prototypes
*============================================================================*/
/* None */
/*============================================================================*
Private Data
*============================================================================*/
/* None */
/*============================================================================*
Public Function Implementations
*============================================================================*/

static const CsrUint8 SERVICE_RECORDS[] = {
    0x00
};


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
CsrUint8 SDS_DBASE_number_of_records(void)
{
    CsrUint8  number_of_sr = 0; /* number of static service records */
    CsrUint16 sr_size = 0;      /* current size of static service records */
    CsrUint16 total_size;       /* total size of static service records */
    /*
        Check that there are static service records.
        If there are, step though the static service records, counting up the
        number of service records. Until the accumulated size is equal to
        or greater than the total size.
        Then just check to make sure the size is correct, if not issue an
        error.
    */
    /* test to see if there are any static service record */
    if (SR_SIZE <= (total_size = sizeof(SERVICE_RECORDS)))
    {
        /* the static service record contains service records, so we need to */
        /* step though to find out how many by adding up the individual */
        /* sizes until the total size is reached*/
        do
        {
            /* increment the number of static service records */
            number_of_sr++;
            /* total up the size of the static service records */
            sr_size += SDP_UINT16_R(SERVICE_RECORDS, sr_size) + SR_SIZE;
        }
        while(sr_size < total_size);
        /* test the accumulated size is equal to the total size */
        if (sr_size != total_size)
        {
            /* error in accumulated size */
         BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
        }
    }

    /* return the number of static service records */
    return number_of_sr;
}

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
CsrUint16 SDS_DBASE_size_of_record(CsrUint8 sr_number_count)
{
    CsrUint16 rnt_sr_size = 0;  /* size of current service record */
    CsrUint8  number_of_sr = 0; /* current number of static service records */
    CsrUint16 sr_size = 0;      /* current size of static service records */
    CsrUint16 total_size;       /* total size of static service records */
    /*
        Check that there are static service records.
        If there are, step though the static service records. Until the
        required service record has been reached, then stop the search
        and send back the size for the current service record.
        If no service record match is found stop an the end of the service
        records and send zero as the size.
    */

    /* test to see if there are any static service record */
    if (0 != (total_size = sizeof(SERVICE_RECORDS)))
    {
        /* the static service record contains service records, so we need to */
        /* step though to find out how many */
        do
        {
            /* increment the number of static service records */
            number_of_sr++;
            /* store the size of the current static service records */
            rnt_sr_size = SDP_UINT16_R(SERVICE_RECORDS, sr_size);
            /* total up the size of the static service records */
            sr_size += rnt_sr_size + SR_SIZE;
            /* test for the requested static service record */
            if (number_of_sr == sr_number_count)
            {
                /* we have found the request static service record */
                /* stop the any further search */
                sr_size = total_size;
            }
            else
            {
                /* we have not found the requested static service record */
                /* yet so clear the return size to make sure we don't give */
                /* the wrong result */
                rnt_sr_size = 0;
            }
        }
        while(sr_size < total_size);
    }

    /* return the size of static service record */
    return rnt_sr_size;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool SDS_DBASE_get_sr(CsrUint8 sr_number_count, CsrUint16 sr_size,
                            CsrUint8 *sr_data)
 *
 *  DESCRIPTION
 *  Store the static service record in pointer supplied.
 *
 *  RETURNS/MODIFIES
 *  status of static service record data
 *
 *---------------------------------------------------------------------------*/
CsrBool SDS_DBASE_get_sr(CsrUint8 sr_number_count, CsrUint16 sr_size,
                        CsrUint8 *sr_data)
{
    CsrBool   rnt_flag = FALSE;   /* status of static service record data */
    CsrUint8  number_of_sr = 0; /* current number of static service records */
    CsrUint16 st_size = 0;      /* current size of static service records */
    CsrUint16 total_size;       /* total size of static service records */
    /*
        Check that there are static service records.
        If there are, step though the static service records. Until the
        required service record has been reached, then stop the search and fill
        the pointer to the service record with the static service record
        data. set the return flag to true indicating we have found the static
        service record.
        If no service record match is found stop an the end of the service
        records and send the false return flag.
    */

    /* test to see if the static service record contains any service records */
    if (0 != (total_size = sizeof(SERVICE_RECORDS)))
    {
        /* the static service record contains service records, so we need to */
        /* step though to find out how many */
        do
        {
            /* increment the number of static service records */
            number_of_sr++;
            /* test for the requested static service record */
            if (number_of_sr == sr_number_count)
            {
                /* we have found the request static service record */
                /* copy static service record data into memory */
                CsrMemCpy(sr_data,
                       &SERVICE_RECORDS[st_size+SR_SIZE], sr_size);
                /* stop the any further search */
                st_size = total_size;
                /* set the return status to indicate there is a service */
                /* record in the memory */
                rnt_flag = TRUE;
            }
            else
            {
                /* total up the size of the static service records */
                st_size += SDP_UINT16_R(SERVICE_RECORDS, st_size) + SR_SIZE;
            }
        }
        while(st_size < total_size);
    }
    /* return the status of static service record data */
    return rnt_flag;
}

#endif /* INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT */

/*============================================================================*
End Of File
*============================================================================*/
