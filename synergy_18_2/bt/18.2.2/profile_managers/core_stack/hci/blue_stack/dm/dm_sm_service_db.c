/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_sm_service_db.c

\brief  Security Manager services database
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "dm_security_manager.h"
#include "dm_mgt_interface.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "tbdaddr.h"
#include "csr_util.h"

#ifndef DISABLE_DM_BREDR

/*! \brief The Security Manager service database */
DM_SM_SERVICE_RECORD_T *dm_sm_service_db;

static void dm_sm_copy_response(DM_UPRIM_T *const request,
                                CsrUint16 size,
                                dm_prim_t type);
static DM_SM_SERVICE_RECORD_T *add_service(const DM_SM_SERVICE_T *app_service,
                                           const BD_ADDR_T *bd_addr,
                                           dm_security_level_t security_level);
static void delete_service(const DM_SM_SERVICE_T *service,
                           const BD_ADDR_T *bd_addr);

void dm_sm_service_db_init(void)
{
    dm_sm_service_db = NULL;
}

/*! \brief Register a service into the service database.

    This is a 'normal' service registration and is not device-specific.
    It always refers to incoming connections and may also refer to
    outgoing connections.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_register_req(DM_UPRIM_T *const p_uprim)
{
    DM_SM_REGISTER_CFM_T *cfm;
    DM_SM_SERVICE_RECORD_T *p_rec;

    p_rec = add_service(&p_uprim->dm_sm_register_req.service,
                        NULL,
                        p_uprim->dm_sm_register_req.security_level);

    p_rec->u.outgoing_ok = p_uprim->dm_sm_register_req.outgoing_ok;

    cfm = pcopy(p_uprim, sizeof(DM_SM_REGISTER_CFM_T));
    cfm->type = DM_SM_REGISTER_CFM;
    cfm->service.channel = p_rec->service.channel;
    DM_SendMessage(cfm);
}

/*! \brief Register a device-specific outgoing service in the service database.

    This is a device-specific service registration, referring only to one
    device of a given Bluetooth address. It is only applicable to outgoing
    services. This type of registration will override any normal service
    registration where applicable.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_register_outgoing_req(DM_UPRIM_T *const p_uprim)
{
    DM_SM_REGISTER_OUTGOING_CFM_T *cfm;
    DM_SM_SERVICE_RECORD_T *p_rec;

    p_rec = add_service(
        &p_uprim->dm_sm_register_outgoing_req.connection.service,
        &TBDADDR_ADDR(p_uprim->dm_sm_register_outgoing_req.connection.addrt),
        p_uprim->dm_sm_register_outgoing_req.security_level);

    cfm = pcopy(p_uprim, sizeof(DM_SM_REGISTER_OUTGOING_CFM_T));
    cfm->type = DM_SM_REGISTER_OUTGOING_CFM;
    cfm->connection.service.channel = p_rec->service.channel;
    DM_SendMessage(cfm);
}

/*! \brief Unregister a service from the service database.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_unregister_req(DM_UPRIM_T *const p_uprim)
{
    delete_service(&p_uprim->dm_sm_unregister_req.service, NULL);
    dm_sm_copy_response(p_uprim,
                        sizeof(DM_SM_UNREGISTER_REQ_T),
                        DM_SM_UNREGISTER_CFM);
}

/*! \brief Unregister an outgoing service from the service database.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_unregister_outgoing_req( DM_UPRIM_T *const p_uprim )
{
    delete_service(&p_uprim->dm_sm_unregister_outgoing_req.connection.service,
      &TBDADDR_ADDR(p_uprim->dm_sm_unregister_outgoing_req.connection.addrt));

    dm_sm_copy_response(p_uprim,
                        sizeof(DM_SM_UNREGISTER_OUTGOING_REQ_T),
                        DM_SM_UNREGISTER_OUTGOING_CFM);
}

/*! \brief Find service in services database.

    If p_bd_addr is NULL then we search for a standard service record
    in the service database. Otherwise we search for a device specific
    outgoing record in the service database and fall back to searching
    for a standard record if we can't find one.

    \param p_service Pointer to service to find.
    \param p_bd_addr Pointer to Bluetooth address for a device-specific
           outgoing record search.
    \returns Pointer to pointer to record or pointer to NULL if not found.
*/
DM_SM_SERVICE_RECORD_T **find_service(const DM_SM_SERVICE_T *service,
                                      const BD_ADDR_T *bd_addr)
{
    DM_SM_SERVICE_RECORD_T **pp_rec, *p_rec;

    for (pp_rec = &dm_sm_service_db;
            (p_rec=*pp_rec) != NULL;
            pp_rec = &p_rec->p_next)
    {
        if (service_match(service, &p_rec->service))
        {
            if (p_rec->inout)
            {
                if (bd_addr == NULL || p_rec->u.outgoing_ok)
                    break;
            }
            else if (bd_addr != NULL
                    && bd_addr_eq(&p_rec->u.bd_addr, bd_addr))
                break;
        }
    }

    return pp_rec;
}

CsrBool service_match(const DM_SM_SERVICE_T *const p_service1,
        const DM_SM_SERVICE_T *const p_service2)
{
    if ((p_service1->protocol_id & ~SEC_PROTOCOL_MODIFY_SSP_ONLY)
                == (p_service2->protocol_id & ~SEC_PROTOCOL_MODIFY_SSP_ONLY)
            && p_service1->channel == p_service2->channel)
        return TRUE;

    return FALSE;
}

CsrBool connection_setup_match(const DM_CONNECTION_SETUP_T *const p_cs1,
        const DM_CONNECTION_SETUP_T *const p_cs2)
{
    if (service_match(&p_cs1->connection.service, &p_cs2->connection.service)
        && tbdaddr_eq(&p_cs1->connection.addrt, &p_cs2->connection.addrt)
        && p_cs1->incoming == p_cs2->incoming)
    {
        return TRUE;
    }

    return FALSE;
}


/*! \brief Adds a service record to the database.

    \param p_service Pointer to service structure.
    \param p_bd_addr Pointer to Bluetooth address of remote device, or NULL.
    \param sec Pointer to service security structure.
    \returns Pointer to service record structure.
*/
static DM_SM_SERVICE_RECORD_T *add_service(
        const DM_SM_SERVICE_T *app_service,
        const BD_ADDR_T *bd_addr,
        dm_security_level_t security_level)
{
    DM_SM_SERVICE_RECORD_T *p_rec;
    DM_SM_SERVICE_RECORD_T **pp_rec;
    DM_SM_SERVICE_T service = *app_service;

    if (service.channel == 0)
    {
        /* We must choose an unused channel number. We start from 0xFFFF
           because high values are not valid RFCOMM channels or L2CAP
           PSMs. */
        for (service.channel = 0xFFFF;
             service.channel != 0 && *find_service(&service, bd_addr) != NULL;
             --service.channel)
        {
            ;
        }

        /* Note. It's safe to assume that a spare channel will be found
           because the registration of 0xFFFF channels for a single protocol
           is ludicrous. However, even if it does happen, and we've somehow
           managed not to run out of memory, all that will occur is the
           creation of a service record for channel 0, which isn't a
           disaster. */
    }

    /* Search for an existing record */
    pp_rec = find_service(&service, bd_addr);

    if ((p_rec = *pp_rec) == NULL || (p_rec->inout && bd_addr != NULL))
    {
        /* Existing record not found, add a new one to list.

           It is important that device-specific records are added to the
           start of the list and the others are added to the end of the
           list. The find_service() function depends upon this.

           No way of communicating failure to application, so panic
           on heap exhaustion. */
        p_rec = zpnew(DM_SM_SERVICE_RECORD_T);
        p_rec->service = service;
        if (bd_addr == NULL)
        {
            /* Add to end of list. We know that *pp_rec == NULL */
            p_rec->inout = TRUE;
            *pp_rec = p_rec;
        }
        else
        {
            /* Add to start of list. We don't care about *pp_rec */
            p_rec->p_next = dm_sm_service_db;
            bd_addr_copy(&p_rec->u.bd_addr, bd_addr);
            dm_sm_service_db = p_rec;
        }
    }

    p_rec->security_level = security_level;

    return p_rec;
}


/*! \brief Remove a service record from the database

    If p_bd_addr is NULL then we try to remove a standard entry, otherwise
    we try to remove a device-specific outgoing service entry.

    \param service Pointer to service structure.
    \param p_bd_addr Pointer to Bluetooth address of remote device, or NULL.
*/
static void delete_service(const DM_SM_SERVICE_T *service,
                           const BD_ADDR_T *bd_addr)
{
    DM_SM_SERVICE_RECORD_T *p_rec;
    DM_SM_SERVICE_RECORD_T **pp_rec;

    /* Search for the record */
    pp_rec = find_service(service, bd_addr);
    p_rec = *pp_rec;

    if (p_rec != NULL && (bd_addr == NULL || !p_rec->inout))
    {
        /* Remove the record from the list */
        *pp_rec = p_rec->p_next;

        /* Delete the record */
        CsrPmemFree(p_rec);
    }
}

DM_SM_MITM_SERVICES_T dm_sm_mitm_services(CsrUint16 mitm_mask)
{
    CsrUint16 mitm = DM_SM_MITM_SERVICES_ALL_BIT;

    DM_SM_SERVICE_RECORD_T *p_rec;

    for (p_rec = dm_sm_service_db; p_rec != NULL; p_rec = p_rec->p_next)
    {
        if ((p_rec->security_level & mitm_mask) == 0)
            mitm &= ~DM_SM_MITM_SERVICES_ALL_BIT;
        else
            mitm |= DM_SM_MITM_SERVICES_SOME_BIT;
    }

    return mitm == DM_SM_MITM_SERVICES_ALL_BIT
        ? DM_SM_MITM_SERVICES_NONE : (DM_SM_MITM_SERVICES_T)mitm;
}

static void dm_sm_copy_response(DM_UPRIM_T *const request,
                                CsrUint16 size,
                                dm_prim_t type)
{
    DM_UPRIM_T *response = pcopy(request, size);
    response->type = type;
    DM_SendMessage(response);
}

#ifdef ENABLE_SHUTDOWN
/*! \brief Unregisters the service record.

    Used when deinit for DM is called.
*/
void dm_sm_unregister_service_record(void)
{
    DM_SM_SERVICE_RECORD_T *tmp;

    while (dm_sm_service_db != NULL)
    {
        tmp = dm_sm_service_db;
        dm_sm_service_db = tmp->p_next;
        CsrPmemFree(tmp);
    }
}
#endif

#endif /* DISABLE_DM_BREDR */
