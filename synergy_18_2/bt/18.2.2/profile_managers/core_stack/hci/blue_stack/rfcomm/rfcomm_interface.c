/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_interface.c
\brief  Handles the top level interface to RFCOMM.
*/

#include "csr_synergy.h"

#ifdef INSTALL_RFCOMM_MODULE

#include "rfcomm_private.h"


/*! \brief Handler function for RFCOMM primitives.

     \param control_data - Pointer to rfcomm instance data.
*/
void rfc_iface_handler(void **control_data)
{
    CsrUint16 mi;
    void *rx_prim;
    RFC_CTRL_PARAMS_T  rfc_params = {NULL, NULL, NULL, NULL, 0};

    rfc_params.rfc_ctrl = (RFC_CTRL_T *)*(control_data);

    /* Get message from interface queue */
    if (!CsrSchedMessageGet(&mi, &rx_prim))
        return;

    switch (mi)
    {
        case HOSTIO_SOURCED_PRIM:
        case RFCOMM_PRIM:

            /* hostio sets mi to 0, so assume those are also RFCOMM prims
            */
            rfc_host_handler(&rfc_params,
                             (RFCOMM_UPRIM_T *)rx_prim);
            break;

        case L2CAP_PRIM:
            rfc_l2cap_handler(&rfc_params,
                              (L2CA_UPRIM_T *)rx_prim);
            break;

        case DM_PRIM:
        {
            DM_UPRIM_T *dm_prim = (DM_UPRIM_T *)rx_prim;

            /* The only DM primitive that should be received is a
               DM_SM_ACCESS_CFM in response to the associated request when
               opening a data channel.
            */
            if (dm_prim->type == DM_SM_ACCESS_CFM)
            {
                rfc_check_sm_response(&rfc_params,
                                      (DM_SM_ACCESS_CFM_T *)dm_prim);
            }
            else
            {
                /* Unexpected DM primitive.
                */
            }
            CsrPmemFree(dm_prim);

            break;
        }

        default:
            /* Unexpected primitive type.
            */
            break;

    }
}

#endif /* INSTALL_RFCOMM_MODULE */
