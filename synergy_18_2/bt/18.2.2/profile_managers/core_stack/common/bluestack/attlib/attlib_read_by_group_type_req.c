/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_read_by_group_type_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_read_by_group_type_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_READ_BY_GROUP_TYPE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      Ownership of block pointed to by group is NOT taken over by this
 *      function. Caller must dispose of it as appropriate.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_read_by_group_type_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 start,
    CsrUint16 end,
    att_uuid_type_t group_type,
    CsrUint32 *group,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_READ_BY_GROUP_TYPE_REQ_T *prim = zpnew(ATT_READ_BY_GROUP_TYPE_REQ_T);

    prim->type = ATT_READ_BY_GROUP_TYPE_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->start = start;
    prim->end = end;
    prim->group_type = group_type;

    CsrMemCpy(prim->group, group, 4*sizeof(CsrUint32));

    if (pp_prim)
    {
        *pp_prim = (ATT_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(ATT_IFACEQUEUE, ATT_PRIM, prim);
    }
}

#endif /* INSTALL_ATT_MODULE */
