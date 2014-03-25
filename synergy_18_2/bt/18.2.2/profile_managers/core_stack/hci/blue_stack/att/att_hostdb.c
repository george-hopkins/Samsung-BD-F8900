/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_hostdb.c

\brief  Attribute Protocol attribute database for host

    See common database code (att_db.c) for information about ATT
    database interface.
*/

#include "csr_synergy.h"
#include "att_private.h"

#if defined(INSTALL_ATT_MODULE) && defined(BUILD_FOR_HOST)

/****************************************************************************
NAME
    att_attr_add - add attribute(s) to the database
*/
att_result_t att_attr_add(ATT_FUNC_STATE
                          att_attr_t **attrp, CsrUint16 size_attr)
{
    att_attr_t *last = NULL;
    att_attr_t *attr;
    att_attr_t **db;
    CsrUint16 endh = 0;

    PARAM_UNUSED(size_attr);

    /* check that attributes are in ascending order, and find the end */
    for (attr = *attrp; attr; attr = attr->next)
    {
        if (attr->handle > endh)
        {
            endh = attr->handle;
            last = attr;
        }
        else
        {
            /* invalid handle */
            return ATT_RESULT_INVALID_HANDLE;
        }
    }

    /* find correct place for attributes */
    for (db = &ATT_STATE(attr); (attr = *db) != NULL; db = &attr->next)
    {
        /* next handle in db is larger than our starting handle, stop here */
        if (attr->handle > (*attrp)->handle)
        {
            break;
        }

        /* handle already registered */
        else if (attr->handle == (*attrp)->handle)
        {
            return ATT_RESULT_INVALID_HANDLE;
        }
    }

    /* add new attributes if they fit between existing handles */
    if (!attr || attr->handle > last->handle)
    {
        last->next = *db;
        *db = *attrp;

        /* clear original pointer, ATT owns the pointer now */
        *attrp = NULL;

        return ATT_RESULT_SUCCESS;
    }

    /* supplied handle range conflicts with the existing db */
    return ATT_RESULT_INVALID_HANDLE;
}

/****************************************************************************
NAME
    att_attr_find - find an attribute by handle
*/
att_attr_t *att_attr_find(ATT_FUNC_STATE
                          CsrUint16 handle, CsrUint16 *found)
{
    att_attr_t *attr;

    for (attr = ATT_STATE(attr);
         attr && attr->handle < handle;
         attr = att_attr_next(ATT_ARG attr, NULL)) /* nop */;

    if (attr)
    {
        if (found)
        {
            *found = attr->handle;
        }

        else if (attr->handle != handle)
        {
            return NULL;
        }

        return attr;
    }

    return NULL;
}

/****************************************************************************
NAME
    att_attr_isgroup - checks whether the attribute is of grouping type

NOTES
    Common between onchip and host, implemented in att_db.c
*/

/****************************************************************************
NAME
    att_attr_isuuid16 - checks whether the attribute type is UUID16

NOTES
    #defined in att_private.h
*/

/****************************************************************************
NAME
    att_attr_match - checks if the attribute value matches
*/
CsrBool att_attr_match(att_attr_t *attr, const CsrUint8 *data)
{
    CsrUint16 len = attr->size_value;
    CsrUint8 *p = att_attr_value(attr);
    CsrUint16 i;

    for (i = 0; i < len; i++)
    {
        if (p[i] != data[i]) break;
    }

    return i == len;
}

/****************************************************************************
NAME
    att_attr_match_uuid - checks if the attribute type matches UUID16
*/
CsrBool att_attr_match_uuid(att_attr_t *attr, CsrUint32 *u)
{
    return
        /* UUID16 */
        (att_attr_isuuid16(attr)?attr->uuid[0] == u[0]:
        /* UUID128 */
        (attr->uuid[0] == u[0] && attr->uuid[1] == u[1] &&
         attr->uuid[2] == u[2] && attr->uuid[3] == u[3]));
}

/****************************************************************************
NAME
    att_attr_match_uuid128 - checks if the attribute type matches UUID128
*/
CsrBool att_attr_match_uuid128(att_attr_t *attr, CsrUint32 *uuid)
{
    return att_attr_match_uuid(attr, uuid);
}

/****************************************************************************
NAME
    att_attr_next - get the next attribute from the database
*/
att_attr_t *att_attr_next(ATT_FUNC_STATE
                          att_attr_t *attr, CsrUint16 *handle)
{
    att_attr_t *p = attr->next;

    if (handle)
    {
        if (p)
        {
            *handle = p->handle;
        }
        else
        {
            /* not found, but increase the handle to make sure upper
             * layer wont stay in a loop forever */
            (*handle) += 1;
        }
    }

    return p;
}

/****************************************************************************
NAME
    att_attr_perm - get the attribute properties

*/
CsrUint16 att_attr_perm(ATT_FUNC_STATE
                       att_attr_t *attr)
{
    CsrUint16 perm = attr->perm & 0xff;

    /* Auxiliary types use extended permissions */
    if (att_attr_isaux(attr) &&
        att_attr_perm_ext(ATT_ARG attr) & ATT_PERM_WRITE_AUX)
    {
        /* add write bits */
        perm |= ATT_PERM_WRITE_CMD | ATT_PERM_WRITE_REQ;
    }

    return perm;
}

/****************************************************************************
NAME
    att_attr_perm_ext - get the extended attribute properties

DESCRIPTION
    Find characteristic extended properties from the database, and return
    the value.
*/
CsrUint16 att_attr_perm_ext(ATT_FUNC_STATE
                           att_attr_t *attr)
{
    CsrUint16 aux = att_attr_isaux(attr);
    CsrUint16 target = attr->handle;
    att_attr_t *ext = NULL;

    /* go through database to find the attribute */
    for (attr = ATT_STATE(attr);
         attr;
         attr = att_attr_next(ATT_ARG attr, NULL))
    {
        if (!att_attr_isuuid16(attr))
        {
            continue;
        }

        switch ((CsrUint16)attr->uuid[0] & 0xffff)
        {
            case ATT_UUID_CHARACTERISTIC:
                if (attr->handle > target)
                    return 0;

                ext = NULL;
                break;

            case ATT_UUID_CH_EXTENDED:
                ext = attr;
                break;

            default:
                /* ignore */
                break;
        }

        /* found extended permissions */
        if (attr->handle >= target && ext)
        {
            /* type is ch_extended so just return the value */
            CsrUint16 perm = ext->value[0] | ext->value[1] << 8;

            /* Auxiliary types use extended permissions */
            if (aux)
            {
                if (!(perm & ATT_PERM_WRITE_AUX))
                {
                    /* do not allow reliable writes */
                    perm &= ~ATT_PERM_RELIABLE_WRITE;
                }
#ifdef DO_NOT_HONOR_RELIABLE_PERMISSIONS
                else
                {
                    /* allow reliable writes as well */
                    perm |= ATT_PERM_RELIABLE_WRITE;
                }
#endif
            }

            return perm;
        }
    }

    return 0;
}

/****************************************************************************
NAME
    att_attr_read - read the attribute value
*/
CsrUint16 att_attr_read(CsrUint8 **data, att_attr_t *attr, CsrUint16 offs, CsrUint16 len)
{
    CsrUint8 *p = attr->value;
    CsrUint16 i;

    /* find last byte to be read */
    len = CSRMIN(attr->size_value, len + offs);

    /* read value byte by byte in little endian byte order */
    for (i = offs; i < len; i++)
    {
        write_uint8(data, p[i]);
    }

    /* return number of bytes read */
    return offs > len ? 0 : len - offs;
}

/****************************************************************************
NAME
    att_attr_read_uuid - read the attribute type in little endian format
*/
void att_attr_read_uuid(CsrUint8 **data, att_attr_t *attr)
{
    if (att_attr_isuuid16(attr))
    {
        write_uint16(data, (CsrUint16)(attr->uuid[0] & 0xffff));
    }
    else
    {
        CsrUint16 i;
        for (i = 0; i < 4; i++)
        {
            write_uint16(data, (CsrUint16)(attr->uuid[3 - i] & 0xffff));
            write_uint16(data, (CsrUint16)(attr->uuid[3 - i] >> 16));
        }
    }
}

/****************************************************************************
NAME
    att_attr_remove - remove attribute(s) from the database
*/
CsrUint16 att_attr_remove(ATT_FUNC_STATE
                         CsrUint16 handle, CsrUint16 endh)
{
    CsrUint16 handles = 0;
    att_attr_t **db;
    att_attr_t *attr;

    for (db = &ATT_STATE(attr);
         (attr = *db) != NULL && attr->handle <= endh;
         /* nop */)
    {
        if (attr->handle >= handle)
        {
            *db = attr->next;
            CsrPmemFree(attr->value);
            CsrPmemFree(attr);
            handles++;
        }
        else
        {
            db = &attr->next;
        }
    }

    return handles;
}

/****************************************************************************
NAME
    att_attr_set - set the attribute value
*/
att_result_t att_attr_set(ATT_FUNC_STATE
                          att_attr_t **attrp, CsrUint16 offs, CsrUint16 len,
                          const CsrUint8 *v)
{
    att_attr_t *attr = *attrp;
    CsrUint16 totlen = offs + len;
    CsrUint16 i;

    /* change attribute size */
    if (totlen != attr->size_value)
    {
        if ( offs > attr->size_value )
            return ATT_RESULT_INVALID_OFFSET;

        if (!(attr->flags & ATT_ATTR_DYNLEN) && totlen > attr->size_value)
            return ATT_RESULT_INVALID_LENGTH;

        /* allocate larger memory block if necessary */
        if (totlen > attr->size_value)
        {
            CsrUint8 *p = CsrPmemAlloc(totlen * sizeof(CsrUint8));

            /* copy data to the new pool */
            CsrMemCpy(p, attr->value, attr->size_value);
            CsrPmemFree(attr->value);
            attr->value = p;
        }

        /* set the new length */
        if (attr->flags & ATT_ATTR_DYNLEN)
            attr->size_value = totlen;
    }

    /* set the new value */
    for (i = offs; i < totlen; i++)
    {
        attr->value[i] = *v++;
    }

    return ATT_RESULT_SUCCESS;
}

/****************************************************************************
NAME
    att_wq_add - add an element into the write queue
*/
att_result_t att_wq_add(att_conn_t *conn, CsrUint16 handle, CsrUint16 offs, CsrUint16 len, const CsrUint8 *d)
{
    att_writeq_t *wq, **db;
    CsrUint16 i = 0;

    for (db = &conn->wq; (wq = *db) != NULL; db = &wq->next, i++) /* nop */;

    if (i >= ATT_QUEUE_SIZE)
    {
        return ATT_RESULT_PREPARE_QUEUE_FULL;
    }

    /* add new entry */
    wq = CsrPmemAlloc(sizeof(att_writeq_t) + len * sizeof(CsrUint8));
    wq->next = *db;
    *db = wq;

    /* fill in values */
    wq->handle = handle;
    wq->offs = offs;
    wq->len = len;
    CsrMemCpy(&wq->value, d, len * sizeof(CsrUint8));

    return ATT_RESULT_SUCCESS;
}

/****************************************************************************
NAME
    att_wq_free - free the write queue
*/
void att_wq_free(att_conn_t *conn)
{
    att_writeq_t *wq, **db;

    for (db = &conn->wq; (wq = *db) != NULL; /* nop */)
    {
        *db = wq->next;
        CsrPmemFree(wq);
    }
}

/****************************************************************************
NAME
    att_attr_exe_set - set the attribute value during the execute write

NOTES
    #defined in att_private.h
*/

att_result_t att_attr_exe_set(ATT_FUNC_STATE
                          att_attr_t **attrp, att_writeq_t *wq)
{
    att_result_t rc = ATT_RESULT_SUCCESS;
    att_attr_t *attr = *attrp;

   rc = att_attr_set(ATT_ARG &attr, wq->offs,
                                  wq->len, wq->value);

    return rc;
}


#endif /* INSTALL_ATT_MODULE && BUILD_FOR_HOST */
