/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_db.c

\brief  Attribute Protocol attribute database interface

    ATT attribute DB interface contains the following functions:

    att_attr_add - add attribute(s) to the database
    att_attr_find - find an attribute by handle
    att_attr_isgroup - checks whether the attribute is of grouping type [C]
    att_attr_isuuid16 - checks whether the attribute type is UUID16
    att_attr_len - length of the attribute value
    att_attr_match - checks if the attribute value matches
    att_attr_match_uuid - checks if the attribute type matches UUID16
    att_attr_match_uuid128 - checks if the attribute type matches UUID128
    att_attr_next - get the next attribute from the database
    att_attr_perm - get the attribute properties
    att_attr_perm_ext - get the extended attribute properties
    att_attr_read - read the attribute value
    att_attr_read_uuid - read the attribute type in little endian format
    att_attr_remove - remove attribute(s) from the database
    att_attr_set - set the attribute value
    att_wq_add - add an element into the write queue
    att_wq_free - free the write queue
    att_wq_next - get the next item from the write queue

    This file implements common ATT attribute database shared between onchip
    (BlueCore and Robinson) and Host (Synergy). Common code is marked with [C]
    in the list above.
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

/* known grouping UUIDS */
const CsrUint16 group_uuids[] = {
    ATT_UUID_PRI_SERVICE, /* Primary Service */
    ATT_UUID_SEC_SERVICE, /* Secondary Service */
};

/* known auxiliary UUIDS */
const CsrUint16 aux_uuids[] = {
    ATT_UUID_CH_DESCRIPTION, /* Characteristic User Description */
};

/****************************************************************************
NAME
    att_attr_istype - checks whether the attribute is of specified type
*/
static CsrBool attr_istype(att_attr_t *attr, const CsrUint16 *uuids,
                          CsrUint16 size_uuids)
{
    CsrUint16 i;
    CsrUint32 uuid[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};

    CsrBool match = FALSE;

    /* go through known uuids */
    for (i = 0; i < size_uuids && !match; i++)
    {
        uuid[0] = (CsrUint32)uuids[i];
        match = att_attr_match_uuid(attr, uuid);
    }

    return match;
}

/****************************************************************************
NAME
    att_attr_isaux - checks whether the attribute is of auxiliary type
*/
CsrBool att_attr_isaux(att_attr_t *attr)
{
    return attr_istype(attr, aux_uuids,
                       sizeof(aux_uuids) / sizeof(CsrUint16));
}

/****************************************************************************
NAME
    att_attr_isgroup - checks whether the attribute is of grouping type
*/
CsrBool att_attr_isgroup(att_attr_t *attr)
{
    return attr_istype(attr, group_uuids,
                       sizeof(group_uuids) / sizeof(CsrUint16));
}

#endif /* INSTALL_ATT_MODULE */
