/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_sda_deinit.c

DESCRIPTION:    Shutdown code for SDS.



****************************************************************************/

#include "csr_synergy.h"

#ifdef ENABLE_SHUTDOWN

#include "csr_bt_bluestack_types.h"
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_util.h"
#include "csr_mblk.h"

#include "l2cap_prim.h"
#include "sds_prim.h"
#include "sdp_control.h"
#include "sdp_sds.h"
#include "sdp_sdc.h"
#include "sdc_prim.h"

void sds_deinit(void)
{
    CsrUint8 *sr_ptr;
    CsrUint8 *sr_ptr_last = NULL;
    CsrUint32 srcounter;
    CsrUint32 srhandle;
    CsrUint32 srhandleTemp;
    extern SDSCON_T sdscon; /* defined in sdp_sds.c */
    extern SDSCB_T *sdscb;  /* defined in sdp_sds.c */
    extern SDSLL_T sdsll;   /* defined in sdp_sds.c */

    /* Clear SDSATT_T (Linked list of attributes) */
    if(sdscon.first_connection != NULL)
    {
        /* test if any attributes in the link list */
        sdscb = (SDSCB_T *)sdscon.first_connection;
        while(sdscb != NULL)
        {
            /* frees the attribute list _only_ for the element currently pointed to by sdscb */
            free_attribute_list();
            sdscb = (SDSCB_T *)sdscb->next_connection;
        }
    }

    /* Clear SDSLL_T - Clear service record */

    /* Init to set the lower limit for the handles */
    sr_ptr = sdsll.first_rc;
    (void)CsrMemCpy(&sr_ptr, sr_ptr, sizeof(CsrUint8 *));

    /* If the first record is empty, no other service record exist */
    if (sr_ptr != NULL)
    {
        srhandle = store_service_record_handle(sr_ptr + SERVICE_DISCOVERY_SERVER_INDEX_HANDLE);

        for (srcounter = srhandle; srcounter <= sdscon.SR_Handle; srcounter++)
        {
            sr_ptr = sdsll.first_rc;
            while (sr_ptr != NULL)
            {
                /* The handle for the sr_ptr */
                srhandleTemp = store_service_record_handle(sr_ptr + SERVICE_DISCOVERY_SERVER_INDEX_HANDLE);

                /* Match of handles */
                if(srhandleTemp == srcounter)
                {
                    CsrUint8 *tmp;

                    /* Stores the pointer to the next service record in the last */
                    if(sr_ptr_last)
                    {
                        (void)CsrMemCpy(sr_ptr_last, sr_ptr, sizeof(CsrUint8 *));
                    }

                    /* Free the service record data */
#ifdef BUILD_FOR_HOST
                    writePtrValue((CsrUint8 *) &tmp,
                                  &(sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
#else
                    tmp = (CsrUint8*)sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE];
#endif
                    CsrPmemFree(tmp);

                    /* Free the service record header */
                    CsrPmemFree(sr_ptr);
                    sr_ptr = NULL;
                }
                else /* No handle match */
                {
                    /* Update the last pointer to the current */
                    sr_ptr_last = sr_ptr;

                    /* Update the pointer to the next service record */
                    (void)CsrMemCpy(&sr_ptr, sr_ptr, sizeof(CsrUint8 *));
                }
            }
        }
    }

    /* Frees the last memory allocated at the start of the service record */
    sr_ptr = sdsll.first_rc;

#ifdef BUILD_FOR_HOST
    writePtrValue( (CsrUint8 *) &sr_ptr, &(sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
#else
    sr_ptr = (CsrUint8*)sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE];
#endif
    CsrPmemFree(sr_ptr);
    sr_ptr = sdsll.first_rc;
    CsrPmemFree(sr_ptr);

    /* Clear SDSRCH_T */
    if(sdscon.first_connection != NULL)
    {
        sdscb = (SDSCB_T *)sdscon.first_connection;
        while(sdscb  != NULL)
        {
            /* frees handle list for _only_ for the element currently pointed to by sdscb */
            free_handle_list();
            sdscb = (SDSCB_T *)sdscb->next_connection;
        }
    }

    /* Clear SDSCON_T and SDSCB_T */

    /* Test to see if there is any connection data */
    if(sdscon.first_connection != NULL)
    {
        sdscb = (SDSCB_T *)sdscon.first_connection;
        while(sdscb  != NULL)
        {
            SDSCB_T *sdscb_temp;

            sdscb_temp = (SDSCB_T *)sdscb;
            sdscb = (SDSCB_T *)sdscb->next_connection;
            CsrPmemFree(sdscb_temp);
        }

        sdscon.first_connection = NULL;
        sdscon.current_connection = NULL;
    }
}

#endif
