/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_sm_device_db.c

\brief  Security Manager devices database
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "dm_security_manager.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "dm_mgt_interface.h"
#include "csr_util.h"

#include "tbdaddr.h"


#ifdef SM_HAS_FUNCTION_FOR_AES
#include "aes.h"
#endif

#ifndef DISABLE_DM_BREDR

/* !\brief linked list of services. */
typedef struct DM_SM_SERVICE_AUTHORISATION_LIST_T_tag
{
    struct DM_SM_SERVICE_AUTHORISATION_LIST_T_tag *p_next;
    DM_SM_SERVICE_T service;
    CsrUint16 authorisation;
} DM_SM_SERVICE_AUTHORISATION_LIST_T;

/*! \brief Device record structure. */
typedef struct DM_SM_DEVICE_RECORD_T_tag
{
    /*! Next device in database */
    struct DM_SM_DEVICE_RECORD_T_tag *p_next;

    /*! Device address */
    TYPED_BD_ADDR_T addrt;

#define DM_SM_TEMPORARY     0x0001
#define DM_SM_TRUSTED       0x0002
    CsrUint16 flags;

#define DM_SM_KEY_INDEX(key) ((key) - 1)
    DM_SM_KEYS_T keys;

    /*! List of services authorised to connect. */
    DM_SM_SERVICE_AUTHORISATION_LIST_T *p_authorised_services;
} DM_SM_DEVICE_RECORD_T;

/*! \brief The Security Manager device database */
DM_SM_DEVICE_RECORD_T *sm_device_db;

static DM_SM_DEVICE_RECORD_T **dm_sm_find_device(
        const TYPED_BD_ADDR_T *const addrt);
static DM_SM_DEVICE_RECORD_T *dm_sm_add_device(
        const TYPED_BD_ADDR_T *const addrt,
        const CsrBool temporary);
static void dm_sm_free_service_authorisation_list(DM_SM_DEVICE_RECORD_T *p_dev);
static void dm_sm_delete_device_record(DM_SM_DEVICE_RECORD_T **pp_dev);
static void dm_sm_replace_bredr_link_key(
        CsrUint8 new_link_key_type,
        const CsrUint8 *new_link_key,
        DM_SM_KEY_ENC_BREDR_T **store);
static void dm_sm_delete_device_keys(DM_SM_KEYS_T *keys);
static void dm_sm_merge_keys(DM_SM_KEYS_T *dst, const DM_SM_KEYS_T *src);

void dm_sm_device_db_init(void)
{
    sm_device_db = NULL;
}

/*! \brief Add an entry to the device database

    Devices with a given Bluetooth address may be registered in this database,
    together with a link key and indication of trusted status. In addition,
    link keys generated during pairing will always be entered into an
    existing entry in the database.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_add_device_req(DM_UPRIM_T *const p_uprim)
{
    DM_SM_ADD_DEVICE_CFM_T *cfm;
    DM_SM_DEVICE_RECORD_T *p_dev;

    p_dev = dm_sm_add_device(&p_uprim->dm_sm_add_device_req.addrt, FALSE);

    /* Keys */
    if ((p_uprim->dm_sm_add_device_req.keys.present & DM_SM_KEYS_EXISTING)
            == DM_SM_KEYS_REPLACE_EXISTING)
    {
        dm_sm_delete_device_keys(&p_dev->keys);
    }
    dm_sm_merge_keys(&p_dev->keys, &p_uprim->dm_sm_add_device_req.keys);

    /* Trust */
    switch (p_uprim->dm_sm_add_device_req.trust)
    {
        case DM_SM_TRUST_UNCHANGED:
            break;

        case DM_SM_TRUST_DISABLED:
            p_dev->flags &= ~DM_SM_TRUSTED;
            break;

        case DM_SM_TRUST_ENABLED:
            p_dev->flags |= DM_SM_TRUSTED;
            break;
    }

    /* Reply */
    cfm = zpnew(DM_SM_ADD_DEVICE_CFM_T);
    cfm->type = DM_SM_ADD_DEVICE_CFM;
    cfm->phandle = p_uprim->dm_sm_add_device_req.phandle;
    send_dm_sm_common((DM_UPRIM_T*)cfm,
                      &p_uprim->dm_sm_add_device_req.addrt);
}

/*! \brief The application wishes to read an entry in the device database.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_read_device_req(DM_UPRIM_T *const p_uprim)
{
    DM_SM_DEVICE_RECORD_T *p_dev;
    DM_SM_READ_DEVICE_CFM_T *cfm = zpnew(DM_SM_READ_DEVICE_CFM_T);
    const TYPED_BD_ADDR_T *addrt = &p_uprim->dm_sm_read_device_req.addrt;

    cfm->type = DM_SM_READ_DEVICE_CFM;
    cfm->phandle = p_uprim->dm_sm_read_device_req.phandle;

    if ((p_dev = *dm_sm_find_device(addrt)) == NULL)
    {
        cfm->status = HCI_ERROR_UNSPECIFIED;
    }
    else
    {
        cfm->trust = dm_sm_trust(addrt, NULL) == DM_SM_ACCEPTABLE
            ? DM_SM_TRUST_ENABLED : DM_SM_TRUST_DISABLED;

        dm_sm_merge_keys(&cfm->keys, &p_dev->keys);
    }

    send_dm_sm_common((DM_UPRIM_T*)cfm, addrt);
}

/*! \brief Remove a device from the device database.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_remove_device_req(DM_UPRIM_T *const p_uprim)
{
    DM_SM_REMOVE_DEVICE_CFM_T *cfm = zpnew(DM_SM_REMOVE_DEVICE_CFM_T);

    cfm->type = DM_SM_REMOVE_DEVICE_CFM;
    cfm->phandle = p_uprim->dm_sm_remove_device_req.phandle;
    if (!dm_sm_remove_device(&p_uprim->dm_sm_remove_device_req.addrt,
                             FALSE))
    {
        cfm->status = HCI_ERROR_UNSPECIFIED;
    }

    send_dm_sm_common((DM_UPRIM_T*)cfm,
                      &p_uprim->dm_sm_remove_device_req.addrt);
}

/*! \brief Search for a device in the device database.

    \param p_bd_addr Pointer to Bluetooth address of device.
    \returns Pointer to device record or NULL if not found.
*/
static DM_SM_DEVICE_RECORD_T **dm_sm_find_device(
        const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_DEVICE_RECORD_T **pp_dev, *p_dev;

    for(pp_dev = &sm_device_db;
        (p_dev = *pp_dev) != NULL
                && !tbdaddr_eq(&p_dev->addrt, addrt);
        pp_dev = &p_dev->p_next)
        ;

    return pp_dev;
}

/*! \brief Search for a device in the device database
     and return true if it is present otherwise false.

    \param p_bd_addr Pointer to Bluetooth address of device.
    \returns TRUE if device is present in SM database
             FALSE if device is not present in SM database.
*/
CsrBool dm_sm_is_device_present(const TYPED_BD_ADDR_T *const addrt)
{
    return(*dm_sm_find_device(addrt)!=NULL);
}

static DM_SM_DEVICE_RECORD_T *dm_sm_add_device(
        const TYPED_BD_ADDR_T *const addrt,
        const CsrBool temporary)
{
    DM_SM_DEVICE_RECORD_T *p_dev;

    if ((p_dev = *dm_sm_find_device(addrt)) == NULL)
    {
        p_dev = zpnew(DM_SM_DEVICE_RECORD_T);
        tbdaddr_copy(&p_dev->addrt, addrt);
        p_dev->flags = DM_SM_TEMPORARY;

        p_dev->p_next = sm_device_db;
        sm_device_db = p_dev;
    }

    if (!temporary)
        p_dev->flags &= ~DM_SM_TEMPORARY;

    return p_dev;
}


CsrUint8 dm_sm_get_device_link_key(
        const TYPED_BD_ADDR_T *const addrt,
        CsrUint16 **pp_key)
{
    DM_SM_DEVICE_RECORD_T *p_dev;
    CsrUint8 link_key_type = DM_SM_LINK_KEY_NONE;

    if ((p_dev = *dm_sm_find_device(addrt)) != NULL)
    {
        DM_SM_KEY_ENC_BREDR_T *key =
            p_dev->keys.u[DM_SM_KEY_INDEX(DM_SM_KEY_ENC_BREDR)].enc_bredr;

        if (key != NULL)
        {
            if (pp_key != NULL)
                *pp_key = key->link_key;

            link_key_type = (CsrUint8)key->link_key_type;
        }
    }

    return link_key_type;
}

#ifdef INSTALL_SM_MODULE
static CsrBool dm_sm_get_key(DM_SM_UKEY_T *ukey,
                            DM_SM_KEY_TYPE_T type,
                            const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_DEVICE_RECORD_T *p_dev;

    if ((p_dev = *dm_sm_find_device(addrt)) != NULL)
    {
        CsrUint16 key = (DM_SM_KEY_MASK) & (p_dev->keys.present >>
                ((DM_SM_NUM_KEY_BITS) * DM_SM_KEY_INDEX(type)));

        if (type == key)
        {
            *ukey = p_dev->keys.u[DM_SM_KEY_INDEX(key)];
            return TRUE;
        }
    }

    return FALSE;
}

CsrUint16 dm_sm_get_diversifier(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_UKEY_T ukey;

    return dm_sm_get_key(&ukey, DM_SM_KEY_DIV, addrt) ? ukey.div : 0;
}

CsrBool dm_sm_get_csrk(CsrUint16 *csrk, const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_UKEY_T ukey;

    if (dm_sm_get_key(&ukey, DM_SM_KEY_SIGN, addrt) && ukey.sign != NULL)
    {
        CsrMemCpy(csrk, ukey.sign->csrk, sizeof(ukey.sign->csrk));
        return TRUE;
    }

    return FALSE;
}

TYPED_BD_ADDR_T *dm_sm_resolve_address(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_DEVICE_RECORD_T *dev;

    for (dev = sm_device_db; dev != NULL; dev = dev->p_next)
    {
        if (DM_SM_KEY_ID == ((DM_SM_KEY_MASK) & (dev->keys.present
                    >> (DM_SM_NUM_KEY_BITS * DM_SM_KEY_INDEX(DM_SM_KEY_ID)))))
        {
            DM_SM_UKEY_T ukey = dev->keys.u[DM_SM_KEY_INDEX(DM_SM_KEY_ID)];

            if (ukey.id != NULL && SM_PRIVACY_CHECK_HASH(ukey.id->irk,
                                                         addrt,
                                                         &dev->addrt))
            {
                return &dev->addrt;
            }
        }
    }

    return NULL;
}

DM_SM_KEY_ENC_CENTRAL_T *dm_sm_get_enc_key(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_UKEY_T ukey;

    return dm_sm_get_key(&ukey, DM_SM_KEY_ENC_CENTRAL, addrt)
        ? ukey.enc_central : NULL;
}
#endif

CsrBool dm_sm_remove_device(
        const TYPED_BD_ADDR_T *const addrt,
        const CsrBool temporary_only)
{
    DM_SM_DEVICE_RECORD_T **pp_dev;

    if (*(pp_dev = dm_sm_find_device(addrt)) != NULL)
    {
        /* Always remove 'life of ACL' service authorisation list. */
        dm_sm_free_service_authorisation_list(*pp_dev);

        if (!temporary_only || ((*pp_dev)->flags & DM_SM_TEMPORARY) != 0)
        {
            dm_sm_delete_device_record(pp_dev);
            return TRUE;
        }
    }

    return FALSE;
}

void dm_sm_update_device_link_key(const TYPED_BD_ADDR_T *const addrt,
                                  const CsrUint8 *const p_key,
                                  CsrUint8 key_type)
{
    DM_SM_DEVICE_RECORD_T *p_dev;

    if (p_key == NULL)
        key_type = DM_SM_LINK_KEY_NONE;

    /* Create temporary device record, if necessary. */
    p_dev = dm_sm_add_device(addrt, TRUE);

    dm_sm_replace_bredr_link_key(
            key_type,
            p_key,
            &p_dev->keys.u[DM_SM_KEY_INDEX(DM_SM_KEY_ENC_BREDR)].enc_bredr);

    /* If we've added a key to the device record then make a note of it */
    if (p_dev->keys.u[DM_SM_KEY_INDEX(DM_SM_KEY_ENC_BREDR)].enc_bredr != NULL)
    {
        p_dev->keys.present |= DM_SM_KEY_ENC_BREDR <<
                (DM_SM_NUM_KEY_BITS * DM_SM_KEY_INDEX(DM_SM_KEY_ENC_BREDR));
    }

    /* There is no point in keeping temorary device records hanging around
       unless there is a link key. */
    if (key_type == DM_SM_LINK_KEY_NONE)
    {
        dm_sm_remove_device(addrt, TRUE);
        /* Note: p_dev might have been freed. */
    }
}

void dm_sm_update_keys(const TYPED_BD_ADDR_T *const addrt,
                       const DM_SM_KEYS_T *keys)
{
    if (keys->present != 0)
    {
        DM_SM_DEVICE_RECORD_T *p_dev;

        /* Create temporary device record, if necessary. */
        p_dev = dm_sm_add_device(addrt, TRUE);
        dm_sm_merge_keys(&p_dev->keys, keys);
    }
}

DM_SM_RESULT_T dm_sm_trust(const TYPED_BD_ADDR_T *const addrt,
                           const DM_SM_SERVICE_T *const p_service)
{
    DM_SM_DEVICE_RECORD_T *p_dev;
    DM_SM_RESULT_T result = DM_SM_NOT_FOUND;

    if ((p_dev = *dm_sm_find_device(addrt)) != NULL)
    {
        DM_SM_SERVICE_AUTHORISATION_LIST_T **pp_sl, *p_sl;

        if (p_service != NULL)
        {
            for (pp_sl = &p_dev->p_authorised_services;
                    (p_sl = *pp_sl) != NULL;
                    pp_sl = &p_sl->p_next)
            {
                if (service_match(p_service, &p_sl->service))
                {
                    result = DM_SM_UNACCEPTABLE;
                    if ((p_sl->authorisation & (1 << DM_SM_AUTHORISE_ACCEPT_BIT)) != 0)
                        result = DM_SM_ACCEPTABLE;

                    if ((p_sl->authorisation & (1 << DM_SM_AUTHORISE_LIFE_OF_ACL_BIT)) == 0)
                    {
                        /* This one valid for one use only, so remove. */
                        *pp_sl = p_sl->p_next;
                        CsrPmemFree(p_sl);
                    }
                    return result;
                }
            }
        }

        if ((p_dev->flags & DM_SM_TRUSTED) != 0)
            result = DM_SM_ACCEPTABLE;
    }

    return result;
}

/*! \brief Given a link key type and bluetooth address, determines whether
           the link key is acceptable.

    \param addrt Pointer to Bluetooth address of remote device.
    \returns TRUE if it is acceptable, FALSE if it isn't.
*/
DM_SM_RESULT_T dm_sm_current_link_key_acceptable(
    const TYPED_BD_ADDR_T *const addrt,
    CsrUint16 **pp_key)
{
    DM_SM_RESULT_T result = DM_SM_UNACCEPTABLE;

    switch (dm_sm_get_device_link_key(addrt, pp_key))
    {
        case DM_SM_LINK_KEY_LEGACY:
            if (legacy_security_ok(addrt))
                result = DM_SM_ACCEPTABLE;
            break;

        case DM_SM_LINK_KEY_UNAUTHENTICATED:
            if (auth_key_required(addrt)
                    == HCI_MITM_NOT_REQUIRED_NO_BONDING)
                result = DM_SM_ACCEPTABLE;
            break;

        case DM_SM_LINK_KEY_AUTHENTICATED:
        case DM_SM_LINK_KEY_DEBUG:
            result = DM_SM_ACCEPTABLE;
            break;

        case DM_SM_LINK_KEY_NONE:
            result = DM_SM_NOT_FOUND;
            break;
    }

    return result;
}

DM_SM_RESULT_T dm_sm_current_security_acceptable(
        const TYPED_BD_ADDR_T *const addrt,
        CsrUint16 security_requirements)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) == NULL)
        return DM_SM_NOT_FOUND;

    if(dm_acl_is_ble(p_acl))
    {
        /* Check to see if encryption is present. */
        if ((security_requirements & DM_SM_SECURITY_ENCRYPTION) != 0)
        {
            if (!DM_SM_IS_LINK_ENCRYPTED(p_acl))
                return DM_SM_UNACCEPTABLE;
        }

        /* Check MITM requirements */
        if ((security_requirements & DM_SM_SECURITY_MITM_PROTECTION) != 0)
        {
            if(!dm_sm_is_ulp_link_authenticated(addrt))
                return DM_SM_UNACCEPTABLE;
        }
    }
    else
    {
        /*
         * Below piece of code is at present utilized only for
         * ATT/GATT over BR/EDR.
         * In that case this function would be called with minimum
         * security requirement as encryption.
         */
        /* link should always be encrypted */
        if (!DM_SM_IS_LINK_ENCRYPTED(p_acl))
            return DM_SM_UNACCEPTABLE;

        /*
         * First bit of authentication requirements is sufficient
         * for testing if authenticated link key is required or not
         * as this bit covers all three - MITM with no bonding, dedicated
         * bonding and general bonding scenario.
         */
        if(security_requirements & HCI_MITM_REQUIRED_NO_BONDING)
        {
            if(DM_SM_LINK_KEY_AUTHENTICATED != dm_sm_get_device_link_key(addrt,NULL))
                return DM_SM_UNACCEPTABLE;
        }
    }

    return DM_SM_ACCEPTABLE;
}

/*
 * This fucntion finds if an LE link is authenticated or not.
 */
CsrBool dm_sm_is_ulp_link_authenticated(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_DEVICE_RECORD_T *p_dev;

        /*
         * To Do - to check if MITM security requirement can come just by
         * adding a device in database with MITM. It does not mandate a
         * link is authenticated.
         */
    return (((p_dev = *dm_sm_find_device(addrt)) != NULL)&&
        ((p_dev->keys.security_requirements & DM_SM_SECURITY_MITM_PROTECTION) != 0));
}



void dm_sm_store_authorisation(DM_CONNECTION_T *connection,
                               CsrUint16 authorisation)
{
    if ((authorisation & ((1 << DM_SM_AUTHORISE_LIFE_OF_ACL_BIT)
                    | (1 << DM_SM_AUTHORISE_TWICE_BIT))) != 0)
    {
        DM_SM_SERVICE_AUTHORISATION_LIST_T *p_service;
        DM_SM_DEVICE_RECORD_T *p_dev;

        /* Store result for life of ACL in device database. */
        p_dev = dm_sm_add_device(&connection->addrt, TRUE);

        /* Can't be already in the list or we wouldn't have asked
           for authorisation. So add to beginning. */
        p_service = pnew(DM_SM_SERVICE_AUTHORISATION_LIST_T);
        p_service->authorisation = authorisation;
        p_service->service = connection->service;
        p_service->p_next = p_dev->p_authorised_services;
        p_dev->p_authorised_services = p_service;
    }
}

/*! \brief Remove 'life of ACL' service authorisation list.

    \param p_dev Pointer to device record structure.
*/
static void dm_sm_free_service_authorisation_list(DM_SM_DEVICE_RECORD_T *p_dev)
{
    DM_SM_SERVICE_AUTHORISATION_LIST_T *p_service;

    while ((p_service = p_dev->p_authorised_services) != NULL)
    {
        p_dev->p_authorised_services = p_service->p_next;
        CsrPmemFree(p_service);
    }
}

static void dm_sm_replace_bredr_link_key(CsrUint8 new_link_key_type,
                                         const CsrUint8 *new_link_key,
                                         DM_SM_KEY_ENC_BREDR_T **store)
{
    CsrUint16 old_link_key_type = DM_SM_LINK_KEY_NONE;
    DM_SM_KEY_ENC_BREDR_T *enc_bredr = *store;

    if (enc_bredr != NULL)
        old_link_key_type = enc_bredr->link_key_type;

    CsrPmemFree(enc_bredr);
    enc_bredr = NULL;

    /* Try to work out what sort of link key a 'changed' link key really
       is. If there is an existing link key then assume the new one is
       the same type. */
    if (new_link_key_type == DM_SM_LINK_KEY_CHANGED
            && old_link_key_type != DM_SM_LINK_KEY_NONE)
    {
        new_link_key_type = (CsrUint8)old_link_key_type;
    }

    /* Only allow certain types of link key */
    switch (new_link_key_type)
    {
        case DM_SM_LINK_KEY_CHANGED:
        case DM_SM_LINK_KEY_LEGACY:
        case DM_SM_LINK_KEY_UNAUTHENTICATED:
        case DM_SM_LINK_KEY_AUTHENTICATED:
            enc_bredr = pnew(DM_SM_KEY_ENC_BREDR_T);
            enc_bredr->link_key_type = new_link_key_type;
            CsrBtMemCpyPack(enc_bredr->link_key, new_link_key, SIZE_LINK_KEY/2);
            break;
    }

    *store = enc_bredr;
}

static void dm_sm_delete_device_keys(DM_SM_KEYS_T *keys)
{
    DM_SM_UKEY_T *key;
    CsrUint16 present;

    for (key = keys->u, present = keys->present;
            present != 0;
            ++key, present >>= DM_SM_NUM_KEY_BITS)
    {
        DM_SM_KEY_TYPE_T key_type = (DM_SM_KEY_TYPE_T)
                (present & ((1 << DM_SM_NUM_KEY_BITS) - 1));

        switch (key_type)
        {
            case DM_SM_KEY_ENC_BREDR:
            case DM_SM_KEY_ENC_CENTRAL:
            case DM_SM_KEY_SIGN:
            case DM_SM_KEY_ID:
                CsrPmemFree(key->none);
                break;

            default:
                break;
        }

        key->none = NULL;
    }

    keys->present = 0;
}

/* Assume that destination will always be ordered correctly. */
static void dm_sm_merge_keys(DM_SM_KEYS_T *dst, const DM_SM_KEYS_T *src)
{
    const DM_SM_UKEY_T *key;
    CsrUint16 present;

    for (key = src->u, present = src->present;
            present != 0;
            ++key, present >>= DM_SM_NUM_KEY_BITS)
    {
        CsrUint16 length = 0;

        DM_SM_KEY_TYPE_T key_type = (DM_SM_KEY_TYPE_T)
            (present & ((1 << DM_SM_NUM_KEY_BITS) - 1));

        switch (key_type)
        {
            case DM_SM_KEY_NONE:
                break;

            case DM_SM_KEY_ENC_BREDR:
                length = sizeof(DM_SM_KEY_ENC_BREDR_T);
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                dst->encryption_key_size = src->encryption_key_size;
                length = sizeof(DM_SM_KEY_ENC_CENTRAL_T);
                break;

            case DM_SM_KEY_DIV:
                dst->present |= DM_SM_KEY_DIV << (DM_SM_NUM_KEY_BITS * DM_SM_KEY_INDEX(DM_SM_KEY_DIV));
                dst->u[DM_SM_KEY_INDEX(DM_SM_KEY_DIV)].div = key->div;
                break;

            case DM_SM_KEY_SIGN:
                length = sizeof(DM_SM_KEY_SIGN_T);
                break;

            case DM_SM_KEY_ID:
                length = sizeof(DM_SM_KEY_ID_T);
                break;

            case DM_SM_KEY_DIV_CSRK:
                /*
                 * We should never reach here as "present" is & with 3 bits (111).
                 * Therefore whenever DM_SM_KEY_DIV_CSRK is given from application,
                 * it will be handled in switch case DM_SM_KEY_DIV.
                 */
                break;
        }

        if (length != 0)
        {
            dst->present |= key_type << (DM_SM_NUM_KEY_BITS * DM_SM_KEY_INDEX(key_type));
            CsrPmemFree(dst->u[DM_SM_KEY_INDEX(key_type)].none);
            dst->u[DM_SM_KEY_INDEX(key_type)].none = pcopy(key->none, length);
        }
    }

    /*
     * Update security requirements if anything has changed.
     * There is no different security requirement for each key.
     * All the keys combined makes one security requirement.
     */
    dst->security_requirements = src->security_requirements;

}

static void dm_sm_delete_device_record(DM_SM_DEVICE_RECORD_T **pp_dev)
{
    DM_SM_DEVICE_RECORD_T *p_dev;

    if ((p_dev = *pp_dev) != NULL)
    {
        /* Remove device from list */
        *pp_dev = p_dev->p_next;

        dm_sm_delete_device_keys(&p_dev->keys);
        dm_sm_free_service_authorisation_list(p_dev);
        CsrPmemFree(p_dev);
    }
}

#ifdef ENABLE_SHUTDOWN
/*! \brief Unregisters the device record.

    Used when deinit for DM is called.
*/
void dm_sm_unregister_device_record(void)
{
    while (sm_device_db != NULL)
        dm_sm_delete_device_record(&sm_device_db);
}
#endif


void dm_sm_update_sec_requirements(const TYPED_BD_ADDR_T *const addrt,
                                   CsrUint16 security_requirements)
{
    DM_SM_DEVICE_RECORD_T *p_dev;
    /* update security requirements to SM database */
    /* Create a new temporary device record */
    p_dev = dm_sm_add_device(addrt, TRUE);

    p_dev->keys.security_requirements |= security_requirements;
}

#endif /* DISABLE_DM_BREDR */
