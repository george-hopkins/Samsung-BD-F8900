/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_debug.c

\brief  Debugging and sanity functions.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
#ifdef SM_DEBUG_PAIRING_SANITY

/*! \brief Check pairing structure for possible memory corruption.
    \param pairing Pointer to pairing structure to check.
*/
void sm_pairing_sanity(SM_PAIRING_T *pairing)
{
    if (pairing == NULL || pairing->sanity != 0xBEEF)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
}

#endif /* SM_DEBUG_PAIRING_SANITY */
#endif /* INSTALL_SM_MODULE */
