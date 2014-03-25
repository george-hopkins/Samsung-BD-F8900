/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdslib.c

DESCRIPTION:       SDC access library for building SDC downstream primitives.



****************************************************************************/

#include "csr_synergy.h"


/*=============================================================================*
    ANSI C & System-wide Header Files
 *============================================================================*/

#if defined(USE_BLUESTACK_DLL)
#include "bluestack.h" /* path removed */
#else
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "csr_bt_tasks.h" /* path removed */
#include "csr_bt_core_stack_pmalloc.h" /* path removed */
#endif

#if defined(INSTALL_SDP_MODULE) || defined(BUILD_FOR_HOST)

/*============================================================================*
    Interface Header Files
 *============================================================================*/

#include "hci_prim.h" /* path modified */
#include "l2cap_prim.h" /* path modified */
#include "dm_prim.h" /* path modified */
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */

/*============================================================================*
    Local Header File
 *============================================================================*/

#include "sdslib.h" /* path removed */

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
 *      sds_register_req
 *
 *  DESCRIPTION
 *      Build and send an SDS_REGISTER_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sds_register_req(
    CsrSchedQid phandle,      /* routing handle */
    CsrUint8 *service_rec,   /* Pointer to service record data */
    CsrUint16 num_rec_bytes  /* Number of bytes in the service record data  */
    )
{
    SDS_REGISTER_REQ_T *prim;

    prim = zpnew(SDS_REGISTER_REQ_T);

    prim->type = SDS_REGISTER_REQ;
    prim->phandle = phandle;
    prim->service_rec = service_rec;
    prim->num_rec_bytes = num_rec_bytes;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sds_unregister_req
 *
 *  DESCRIPTION
 *      Build and send an SDS_UNREGISTER_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sds_unregister_req(
    CsrSchedQid phandle,      /* routing handle */
    CsrUint32 svc_rec_hndl   /* service record handle */
    )
{
    SDS_UNREGISTER_REQ_T *prim;

    prim = zpnew(SDS_UNREGISTER_REQ_T);

    prim->type = SDS_UNREGISTER_REQ;
    prim->phandle = phandle;
    prim->svc_rec_hndl = svc_rec_hndl;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sds_config_req
 *
 *  DESCRIPTION
 *      Build and send an SDS_CONFIG_REQ primitive to SDP.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sds_config_req(
    CsrSchedQid phandle,
    CsrUint16 mtu,
    CsrUint16 flags
    )
{
    SDS_CONFIG_REQ_T *prim;

    prim = zpnew(SDS_CONFIG_REQ_T);

    prim->type = SDS_CONFIG_REQ;
    prim->phandle = phandle;
    prim->l2cap_mtu = mtu;
    prim->flags = flags;

    CsrSchedMessagePut(SDP_IFACEQUEUE, SDP_PRIM, prim);
}

#endif /* defined(INSTALL_SDP_MODULE) || defined(BUILD_FOR_HOST) */
