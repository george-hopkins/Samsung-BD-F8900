/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdclib.c

DESCRIPTION:       SDC access library for building SDC downstream primitives.



****************************************************************************/

#include "csr_synergy.h"


/*=============================================================================*
    ANSI C & System-wide Header Files
 *============================================================================*/

#if defined(USE_BLUESTACK_DLL)
#include "bluestack.h"        /* The standard BlueStack interface */
#else
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h"        /* Common Functions - strlog */
#include "csr_mblk.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#endif

#if defined(INSTALL_SDP_MODULE) || defined(BUILD_FOR_HOST)

/*============================================================================*
    Interface Header Files
 *============================================================================*/

#include "hci_prim.h" /* path modified */
#include "l2cap_prim.h" /* path modified */
#include "dm_prim.h" /* path modified */
#include "sdc_prim.h" /* path modified */

/*============================================================================*
    Local Header File
 *============================================================================*/

#include "sdclib.h"

/*============================================================================*
    Public Data
 *============================================================================*/
/* None */

/*============================================================================*
    Private Defines
 *============================================================================*/

/* This code is used both inside BlueStack and by applications above BlueStack.
 * Therefore use of memory functions and task IDs depends on whether we are
 * building for the BlueStack DLL.
 */
#if defined(USE_BLUESTACK_DLL)
#define pmalloc BST_pmalloc
#define pnew BST_pnew
#define zpnew BST_zpnew
#define CsrSchedMessagePut BST_putMessage
#define SDP_IFACEQUEUE bst_sdp_iface_queue
#endif

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

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_service_search_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_SERVICE_SEARCH_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_service_search_req(
    CsrSchedQid phandle,          /* routing handle */
    BD_ADDR_T *p_bd_addr,       /* remote device */
    CsrUint16 size_srch_pttrn,   /* size of search pattern */
    CsrUint8 *srch_pttrn,        /* pointer to the search pattern */
    CsrUint16 max_num_recs       /* maximum records to return */
    )
{
    SDC_SERVICE_SEARCH_REQ_T *prim;

    prim = zpnew(SDC_SERVICE_SEARCH_REQ_T);

    prim->type = SDC_SERVICE_SEARCH_REQ;
    prim->phandle = phandle;
    prim->bd_addr = *p_bd_addr;
    prim->size_srch_pttrn = size_srch_pttrn;
    prim->srch_pttrn = srch_pttrn;
    prim->max_num_recs = max_num_recs;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_service_attribute_req
 *
 *  DESCRIPTION
 *      Build and send a SDC_SERVICE_ATTRIBUTE_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_service_attribute_req(
    CsrSchedQid phandle,          /* routing handle */
    BD_ADDR_T *p_bd_addr,       /* remote device */
    CsrUint32 svc_rec_hndl,      /* remote service handle */
    CsrUint16 size_attr_list,    /* size of attribute list */
    CsrUint8 *attr_list,         /* pointer to the attribute list */
    CsrUint16 max_num_attr       /* maximum bytes per response */
    )
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *prim;

    prim = zpnew(SDC_SERVICE_ATTRIBUTE_REQ_T);

    prim->type = SDC_SERVICE_ATTRIBUTE_REQ;
    prim->phandle = phandle;
    prim->bd_addr = *p_bd_addr;
    prim->svc_rec_hndl = svc_rec_hndl;
    prim->size_attr_list = size_attr_list;
    prim->attr_list = attr_list;
    prim->max_num_attr = max_num_attr;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_service_search_attribute_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_SERVICE_SEARCH_ATTRIBUTE_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_service_search_attribute_req(
    CsrSchedQid phandle,          /* routing handle */
    BD_ADDR_T *p_bd_addr,       /* remote device */
    CsrUint16 size_srch_pttrn,   /* size of search pattern */
    CsrUint8 *srch_pttrn,        /* pointer to the search pattern */
    CsrUint16 size_attr_list,    /* size of attribute list */
    CsrUint8 *attr_list,         /* pointer to the attribute list */
    CsrUint16 max_num_attr       /* maximum bytes per response */
    )
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *prim;

    prim = zpnew(SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T);

    prim->type = SDC_SERVICE_SEARCH_ATTRIBUTE_REQ;
    prim->phandle = phandle;
    prim->bd_addr = *p_bd_addr;
    prim->size_srch_pttrn = size_srch_pttrn;
    prim->srch_pttrn = srch_pttrn;
    prim->size_attr_list = size_attr_list;
    prim->attr_list = attr_list;
    prim->max_num_attr = max_num_attr;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_terminate_primitive_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_TERMINATE_PRIMITIVE_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_terminate_primitive_req(
    CsrSchedQid phandle           /* routing handle */
    )
{
    SDC_TERMINATE_PRIMITIVE_REQ_T *prim;

    prim = zpnew(SDC_TERMINATE_PRIMITIVE_REQ_T);

    prim->type = SDC_TERMINATE_PRIMITIVE_REQ;
    prim->phandle = phandle;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_open_search_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_OPEN_SEARCH_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_open_search_req(
    CsrSchedQid phandle,          /* routing handle */
    BD_ADDR_T *p_bd_addr        /* remote device */
    )
{
    SDC_OPEN_SEARCH_REQ_T *prim;

    prim = zpnew(SDC_OPEN_SEARCH_REQ_T);

    prim->type = SDC_OPEN_SEARCH_REQ;
    prim->phandle = phandle;
    prim->bd_addr = *p_bd_addr;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_close_search_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_CLOSE_SEARCH_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_close_search_req(
    CsrSchedQid phandle           /* routing handle */
    )
{
    SDC_CLOSE_SEARCH_REQ_T *prim;

    prim = zpnew(SDC_CLOSE_SEARCH_REQ_T);

    prim->type = SDC_CLOSE_SEARCH_REQ;
    prim->phandle = phandle;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_config_req
 *
 *  DESCRIPTION
 *      Build and send an SDC_CONFIG_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdc_config_req(
    CsrSchedQid phandle,
    CsrUint16 mtu,
    CsrUint16 flags
    )
{
    SDC_CONFIG_REQ_T *prim;

    prim = zpnew(SDC_CONFIG_REQ_T);

    prim->type = SDC_CONFIG_REQ;
    prim->phandle = phandle;
    prim->l2cap_mtu = mtu;
    prim->flags = flags;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

#endif /* defined(INSTALL_SDP_MODULE) || defined(BUILD_FOR_HOST) */
